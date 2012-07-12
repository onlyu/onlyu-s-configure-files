
#include "fs_config.h"
#include "fs_comm.h"
#include "fs_ipc.h"
#include "fs_rpc.h"
#include "fs_rpc_lpc.h"
#include "fs_rpc_c.h"
#include "fs_rpc_id.h"
#include "lpc_args_op.h"
#include "fs_log.h"
#include "yauthLib.h"


#include "zopen.h"

#include "http_lpc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rc4.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LZW_BITS 12

#define MAX_CLIENT_PAYLOAD 65535

//在lpc 里面启动所设置的参数
extern int auth_flag;

static struct event_base * ev_base;
static struct evhttp *auth_httpd = NULL;

#define RSA_BUF_SIZE 512 
#define RSA_CHALLENGE_SIZE 8
#define RC4_KEY_SIZE 4

static unsigned char rsa_result_buf[RSA_BUF_SIZE];
static RSA *server_prikey = NULL;
static RSA *client_pubkey = NULL;

//for 64-bits machine
// total max buf size: 512k * 20k = 1024M = 1G
static size_t client_max_outbuf = (512 * 1024 * 1024);
static size_t game_max_outbuf = (1.8 * 1024 * 1024 * 1024);

static char *game_ip = "127.0.0.1";
static unsigned short game_port = 60001;
static char *client_ip = "0.0.0.0";
static unsigned short client_port = 60000;
static unsigned short host_port = 0;
static const int backlog = 5120;

static int host_id = -1;

static fs_net_acceptor_t *game_acceptor = NULL;
static fs_net_connection_t *game_conn = NULL;

static fs_net_acceptor_t *client_acceptor = NULL;

static fs_vfd_t conn_alloc_vfd = FS_CLIENT_START_VFD;


static fs_rpc_lpc_t *netd_rpc_lpc = NULL;

static int is_client_serviece_open = 0;

typedef struct client_base_s {
	apr_pool_t *pool;
	apr_hash_t *conn_hash;
	apr_hash_t *user_hash;
} client_base_t;


typedef struct client_net_stat_s {
	unsigned in_byte;
	unsigned out_byte;
	unsigned packet;
} client_net_stat_t;

typedef enum {
	LOGIN_ING = 0,
	LOGIN_OK = 1,
} game_stat_t;

typedef enum {
	SESSION_VERIFY = 0,
	SESSION_SIGN,
	SESSION_VERSION,
	SESSION_TRAFFIC,
} session_stat_t;

#define SESSION_STAT_READ_TIMEOUT_SEC 15 

typedef struct client_stat_s {
	fs_uid_t uid;
	game_stat_t game_stat;
	client_net_stat_t net_stat;
	zwriter_t zwriter;
	char ip[32];
	char dstip[32];
	unsigned char session[RSA_CHALLENGE_SIZE];
	session_stat_t sstat;
	RC4_KEY rc4_key;
} client_stat_t;

#define CLIENT_UDATA(conn) ((client_stat_t*)((conn)->udata))
#define CLIENT_GAME_STAT(conn) (CLIENT_UDATA(conn)->game_stat)
#define CLIENT_UID(conn) (CLIENT_UDATA(conn)->uid)
#define CLIENT_ZWRITER(conn) (CLIENT_UDATA(conn)->zwriter)

static client_base_t client_base = {NULL, NULL};

typedef struct host_stat_s {
	unsigned hid;
} host_stat_t;

#define HOST_STAT(conn) ((host_stat_t*)((conn)->udata))
#define HOST_HID(conn) (HOST_STAT(conn)->hid)

static fs_net_mbuf_t *client_rpc_buf;
static fs_net_mbuf_t *gamed_rpc_buf;

static void close_common(fs_net_connection_t *conn, int isactive)
{
	int uid = CLIENT_UDATA(conn) ? CLIENT_UID(conn) : 0;

	if (isactive) {
		log_info(MOD_NETD, "active close fd:%d,vfd:%d,uid:%d,conn_cnt:%d,user_cnt:%d", 
			conn->fd, conn->vfd, uid,apr_hash_count(client_base.conn_hash), apr_hash_count(client_base.user_hash));
	} else {
		log_info(MOD_NETD, "passive close fd:%d,vfd:%d,uid:%d,conn_cnt:%d,user_cnt:%d", 
			conn->fd, conn->vfd, uid,apr_hash_count(client_base.conn_hash), apr_hash_count(client_base.user_hash));
	}

	if (auth_flag) {
		call_script_func("cmd/auth_netd", "client_connection_close_cb", "i", conn->vfd); 
	} else {
		call_script_func("cmd/login", "client_connection_close_cb", "i", conn->vfd); 
	}

	apr_hash_set(client_base.conn_hash, &conn->vfd, sizeof(conn->vfd), NULL);
	if (CLIENT_UDATA(conn)) {
		apr_hash_set(client_base.user_hash, &CLIENT_UID(conn), sizeof(fs_uid_t), NULL);
	}

	zwriter_deinit(&CLIENT_ZWRITER(conn));
	fs_net_connection_destroy(conn);
}

static void passive_close_client(fs_net_connection_t *conn)
{
	close_common(conn, 0);
}

//server active to close the client
static void active_close_client(fs_net_connection_t *conn)
{
	fs_net_connection_try_flush(conn);
	close_common(conn, 1);
}

static void active_close_all_client()
{
	apr_hash_index_t *hi;
	void *conn;
	// not thread safe for the iterator of apr_hash unless you porvide the apr_pool for allocating iterator
	for (hi = apr_hash_first(NULL, client_base.conn_hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &conn);
		active_close_client(conn); // can remove the entry from apr_hash when iterating
	}
}


static void send_to_client(fs_net_connection_t *conn, void *buf, size_t len)
{
	if (len > 0) {
		zwriter_clear(&CLIENT_ZWRITER(conn));
		zwriter_write(&CLIENT_ZWRITER(conn), buf, len);
		//fprintf(stderr, "lzw send to client, fd:%d,rawlen:%ld,zlen:%u\n", conn->vfd, len, CLIENT_ZWRITER(conn).zoff);
		fs_net_connection_send(conn,  CLIENT_ZWRITER(conn).zbuf, CLIENT_ZWRITER(conn).zoff);
	}
}


inline static void to_client(fs_net_connection_t *conn, void *buf, size_t len)
{
	fs_nc_header_t hdr;
	FS_NC_SET_HEADER(&hdr, len);
	if (0 < fs_net_connection_send_packet(conn, &hdr, sizeof(hdr), buf, len)) {
		log_error(MOD_NETD, "send client conn error");
	}
}

inline static void vfd_to_client(fs_vfd_t vfd, void *buf, size_t len)
{
	fs_net_connection_t *conn = apr_hash_get(client_base.conn_hash, &vfd, sizeof(vfd));
	if (conn != NULL) {
		to_client(conn, buf, len);
		//fprintf(stderr, "send protocol, len:%ld\n", len);
		//send_to_client(conn, buf, len);
	}
}

inline static void uid_to_client(fs_uid_t uid, void *buf, size_t len)
{
	fs_net_connection_t *conn = apr_hash_get(client_base.user_hash, &uid, sizeof(uid));
	//fprintf(stderr, "send protocol to client, len:%ld\n", len);
	if (conn != NULL) {
		to_client(conn, buf, len);
		//send_to_client(conn, buf, len);
	}
}

inline static int udata_to_gamed(fs_uid_t uid, void *buf, size_t len)
{
	int ret = fs_ng_send_cmd(game_conn, FS_NG_USER, uid, buf, len);
	if (ret < 0) { 
		log_error(MOD_NETD, "send game conn error");
	}
	return ret;
}

inline static int netd_call_gamed(fs_uid_t uid, void *buf, size_t len)
{
	int ret = fs_ng_send_cmd(game_conn, FS_NG_RPC, uid, buf, len);
	if (ret < 0) { 
		log_error(MOD_NETD, "netd to gamed conn error");
	}
	return ret;
}

static int client_version(fs_vfd_t vfd, fs_protocol_id_t pid, void *buf, size_t len)
{
	int ret;
	fs_inbuf_t inbuf;	
	FS_INBUF_INIT(&inbuf, buf, len);
	ret = fs_rpc_dispatch(netd_rpc_lpc, vfd, &inbuf);
	return ret;
}


#define LOGIN_PTO_LIST \
	case RPC_SERVER_VERSION:\
	case RPC_SERVER_LOGIN:\
	case RPC_SERVER_NEW_UID:

//注意：不要直接改 client_net_hook这个函数的代码，把协议id加入上面的对应宏中就可以了。

static int client_netd_hook(fs_net_connection_t *conn, fs_protocol_id_t pid, void *buf, size_t len)
{
	fs_inbuf_t inbuf;	
	FS_INBUF_INIT(&inbuf, buf, len);
	//fprintf(stderr, "client net hook pid:%u,len:%ld\n", pid, len);

	/*
	//netd 的登录协议
	if (CLIENT_GAME_STAT(conn) == LOGIN_OK) {
		switch (pid) {
			LOGIN_PTO_LIST
				log_error(MOD_NETD, "discard auth or login pto:%d\n", pid);
				//return -1;
				return 1; 
			default: //其他到gamed的协议不hook
				return 0;
		}
	} else { //logining
		switch (pid) {
			LOGIN_PTO_LIST {
				int ret = fs_rpc_dispatch(netd_rpc_lpc, conn->vfd, &inbuf);
				if (ret < 0) {
					return ret; //0 成功 -1 出错
				}
				return 1;
			}
			default:
				log_error(MOD_NETD, "discard nonlogin pto:%d\n", pid);
				//return -1;
				return 1;
		}
	}
	*/
	switch (pid) {
		LOGIN_PTO_LIST {
			int ret = fs_rpc_dispatch(netd_rpc_lpc, conn->vfd, &inbuf);
			if (ret < 0) {
				return ret; //0 成功 -1 出错
			}
			return 1;
		}
		default:
			return 0;
	}
}

static void get_challenge(unsigned char *buf, unsigned len)
{
	unsigned *r;
	char *p;
	unsigned i;
	unsigned f = len / sizeof(unsigned);
	unsigned m = len - f * sizeof(unsigned);
	
	for (i = 0, r = (unsigned *)buf; i < f; ++i) {
		*r++ = (unsigned)rand();
	}

	for (i = 0, p = (char*)r; i < m; ++i) {
		*p++ = (char)rand();
	}

}

static void set_read_timeout(fs_net_connection_t *conn, int timeout)
{
	//bufferevent_settimeout 来重复设置已有事件的timeout有不确定的问题，所以
	//干脆删除原有的事件再重新设置
	bufferevent_disable(conn->bufev, EV_READ);
	conn->bufev->timeout_read = timeout;
	bufferevent_enable(conn->bufev, EV_READ);
}

static int client_input_dispatch(fs_net_connection_t *conn, fs_nc_header_t *header, void *buf, size_t len, void *udata)
{
	// for safe,check if the client exists or not, because the arg is referenced by others
	//fprintf(stderr, "client input:head->payload_len:%d,  payload len:%ld\n", header->payload_len, len);
	fs_net_connection_t *exist_conn = apr_hash_get(client_base.conn_hash, &conn->vfd, sizeof(conn->vfd));
	if (exist_conn == NULL) {
		log_error(MOD_NETD, "dispatch error no such conn vfd:%d", conn->vfd);
		return -1;
	} 

	//printf("dispatch vfd:%d, evfd:%d\n", conn->vfd, exist_conn->vfd);
	assert(exist_conn == conn);

	switch (CLIENT_UDATA(conn)->sstat) {
	case SESSION_VERIFY: {
		int declen = RSA_public_decrypt(len, buf, rsa_result_buf, client_pubkey, RSA_PKCS1_PADDING);
		if (declen != RSA_CHALLENGE_SIZE || memcmp(rsa_result_buf, CLIENT_UDATA(conn)->session, RSA_CHALLENGE_SIZE)) {
			log_error(MOD_NETD, "client bad ignature vfd:%d,ip:%s", conn->vfd, CLIENT_UDATA(conn)->ip);
			goto error;
		} else {
			//fprintf(stderr, "server verify the client ok vfd:%d,ip:%s\n", conn->vfd, CLIENT_UDATA(conn)->ip);
			CLIENT_UDATA(conn)->sstat = SESSION_SIGN;
			//去掉客户端身份验证的timeout时间
			set_read_timeout(conn, 0);
		}
		break;
	}
	case SESSION_SIGN: {
		if (len != RSA_CHALLENGE_SIZE) {
			log_error(MOD_NETD, "bad client challenge size vfd=%d,ip:%s,len:%lu", 
				conn->vfd, CLIENT_UDATA(conn)->ip, len);
			goto error;
		}
		int enclen = RSA_private_encrypt(len, buf, rsa_result_buf, server_prikey, RSA_PKCS1_PADDING); 
		assert(enclen > 0);
		send_to_client(conn, rsa_result_buf, enclen);
		//fprintf(stderr, "server signature itself.vfd:%d,ip:%s\n", conn->vfd, CLIENT_UDATA(conn)->ip);

		//send the session key
		get_challenge(CLIENT_UDATA(conn)->session, RC4_KEY_SIZE);
		enclen = RSA_public_encrypt(RC4_KEY_SIZE, CLIENT_UDATA(conn)->session, 
			rsa_result_buf, client_pubkey, RSA_PKCS1_PADDING); 
		send_to_client(conn, rsa_result_buf, enclen);
		CLIENT_UDATA(conn)->sstat = SESSION_VERSION;
		//fprintf(stderr, "server issue session key.vfd:%d,ip:%s\n", conn->vfd, CLIENT_UDATA(conn)->ip);
		RC4_set_key(&CLIENT_UDATA(conn)->rc4_key, RC4_KEY_SIZE, CLIENT_UDATA(conn)->session);
		break;
	}
	case SESSION_VERSION: {
		unsigned char rc4buf[len];
		RC4(&CLIENT_UDATA(conn)->rc4_key, len, buf, rc4buf);
		fs_protocol_id_t *pid = (fs_protocol_id_t*)rc4buf;
		//fprintf(stderr, "recv client version pid=%d\n", *pid);
		if (*pid != RPC_SERVER_VERSION) {
			log_error(MOD_NETD, "recv client bad version vfd=%d,ip=%s,pid=%d",
				conn->vfd, CLIENT_UDATA(conn)->ip, *pid);
			goto error;
		}
		CLIENT_UDATA(conn)->sstat = SESSION_TRAFFIC;
		//这个VERSION 协议必须要hook住
		if (0 > client_version(conn->vfd, *pid, rc4buf, len)) {
			goto error;
		}
		break;
	}
	case SESSION_TRAFFIC: {
		int ret;
		/*
		unsigned char rc4buf[len];
		RC4(&CLIENT_UDATA(conn)->rc4_key, len, buf, rc4buf);
		fs_protocol_id_t *pid = (fs_protocol_id_t*)rc4buf;
		ret = client_netd_hook(conn, *pid, rc4buf, len);
		*/
		fs_protocol_id_t *pid = (fs_protocol_id_t*)buf;
		ret = client_netd_hook(conn, *pid, buf, len);
		if (ret == 0) {
			//udata_to_gamed(CLIENT_UID(conn), rc4buf, len);//netd 非验证服才能给gamed转发数据
			udata_to_gamed(CLIENT_UID(conn), buf, len);//netd 非验证服才能给gamed转发数据
			return 0;
		} else if(ret == 1) {  // hook the pto
			return 0;
		} else {
			goto error;
		}
		break;
	}
	default :
		log_error(MOD_NETD, "error bad stat vfd:%d,ip:%s", conn->vfd, CLIENT_UDATA(conn)->ip);
		goto error;
	}

	return 0;

error:
	active_close_client(conn);
	log_error(MOD_NETD, "close bad stat client.vfd:%d,ip:%s", conn->vfd, CLIENT_UDATA(conn)->ip);
	return -1;
}
	
/*
static void client_connection_recv(struct bufferevent *bufev, void *arg)
{
	READ_PACKET(fs_nc_header_t, bufev, arg, client_input_dispatch, NULL);
}
*/
static void client_connection_recv(struct bufferevent *bufev, void *arg)
{
	//READ_PACKET(fs_nc_header_t, bufev, arg, client_input_dispatch, NULL);
	size_t bufsz = 0;				
	size_t packet_sz = 0;				
	fs_nc_header_t *header = NULL;			
	fs_net_connection_t *conn = arg;		
	int ret;

	while ((bufsz = EVBUFFER_LENGTH(EVBUFFER_INPUT(bufev))) > 0) {		
		if (bufsz < sizeof(*header))  					
			break;							
		header = (fs_nc_header_t *)EVBUFFER_DATA(EVBUFFER_INPUT(bufev));	
		if (header->payload_len > MAX_CLIENT_PAYLOAD) { //too large
			log_error(MOD_NETD, "close client too large packet vfd=%d,len=%u", conn->vfd, header->payload_len);
			active_close_client(conn);
			return;
		}


		packet_sz = sizeof(*header) + header->payload_len;		
		if (bufsz < packet_sz) 						
			break;								
		//(packet_handler)(conn, header, header + 1, header->payload_len, (udata));	
		ret = client_input_dispatch(conn, header, header + 1, header->payload_len, NULL);	
		evbuffer_drain(EVBUFFER_INPUT(bufev), packet_sz);			
		if (ret < 0) {
			break;
		}
	}										
}

static void client_connection_error(struct bufferevent *bufev, short what, void *arg)
{
	//client close the socket or other resons.
	/*
	#define EVBUFFER_READ           0x01
	#define EVBUFFER_WRITE          0x02
	#define EVBUFFER_EOF            0x10
	#define EVBUFFER_ERROR          0x20
	#define EVBUFFER_TIMEOUT        0x40
	*/

	fs_net_connection_t *conn = arg;
	fs_net_connection_t *exist_conn = apr_hash_get(client_base.conn_hash, &conn->vfd, sizeof(conn->vfd));
	assert(exist_conn == conn);

	log_info(MOD_NETD, "client connection close vfd=%d,what=%02x,srcip=%s,dstip=%s", 
				conn->vfd, what, CLIENT_UDATA(conn)->ip, CLIENT_UDATA(conn)->dstip);
	if (CLIENT_UDATA(conn)) {
		fs_net_connection_t *user_conn = apr_hash_get(client_base.user_hash, &CLIENT_UID(conn), sizeof(fs_uid_t));
		if (user_conn != NULL) {
			fs_ng_send_cmd(game_conn, FS_NG_REMOVE, CLIENT_UID(user_conn), NULL, 0);
		}
	}

	passive_close_client(exist_conn);
}


int CanAcceptIp(char *ip)
{
	static char *valid_ip[] = { 
		"127.0.0.",
		NULL, 
	};

	char **p;
	int iplen = strlen(ip);

	for (p = valid_ip; *p != NULL; ++p) {
		int netlen = strlen(*p);
		if (netlen <= iplen && 0 == memcmp(ip, *p, netlen)) {
			return 1;
		}
	}

	return 0;
}


static void client_accept(int fd, short evtype, void *arg)
{
	//fs_net_acceptor_t *acceptor = arg;
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	struct sockaddr_in dstaddr;
	socklen_t dstsocklen = sizeof(dstaddr);
	
	int conn_fd = accept(fd, (struct sockaddr*)&addr, &socklen);
	if (conn_fd < 0) {
		perror("client accept error");
		return;
	}

        fs_net_connection_t *conn = fs_net_connection_new_ext(conn_fd, client_connection_recv, 
					NULL, client_connection_error, client_max_outbuf, sizeof(client_stat_t));
	conn->vfd = conn_alloc_vfd++;
	inet_ntoa_r(addr.sin_addr, CLIENT_UDATA(conn)->ip, sizeof(CLIENT_UDATA(conn)->ip));

	if (0 == getsockname(conn_fd, (struct sockaddr*)&dstaddr, &dstsocklen)) {
		inet_ntoa_r(dstaddr.sin_addr, CLIENT_UDATA(conn)->dstip, sizeof(CLIENT_UDATA(conn)->dstip));
	} 

	fs_net_set_nonblock(conn_fd);
	CLIENT_GAME_STAT(conn) = LOGIN_ING;
	//CLIENT_UDATA(conn)->sstat = SESSION_VERIFY;
	CLIENT_UDATA(conn)->sstat = SESSION_TRAFFIC;
	apr_hash_set(client_base.conn_hash, &conn->vfd, sizeof(conn->vfd), conn); 
	log_info(MOD_NETD, "accept client fs:%d,vfd:%d,srcip:%s,dstip:%s", 
			conn_fd, conn->vfd, CLIENT_UDATA(conn)->ip, CLIENT_UDATA(conn)->dstip);

	/*
	zwriter_init(&CLIENT_ZWRITER(conn), LZW_BITS);
	get_challenge(CLIENT_UDATA(conn)->session, RSA_CHALLENGE_SIZE);
	//printf("server issue the challenge\n");
	send_to_client(conn, CLIENT_UDATA(conn)->session, RSA_CHALLENGE_SIZE);
	//set timeout
	set_read_timeout(conn, SESSION_STAT_READ_TIMEOUT_SEC);
	*/
}


static void game_input_remove(fs_ng_header_t *header, void *buf, size_t len)
{
	/*注意不要给gamed close消息，否则会死循环 */
	//printf("game input removed\n");
	fs_net_connection_t *conn = apr_hash_get(client_base.user_hash, &header->u.uid, sizeof(header->u.uid));
	if (conn != NULL) {
		log_error(MOD_NETD, "game close client.vfd:%d,ip:%s", conn->vfd, CLIENT_UDATA(conn)->ip);
		active_close_client(conn);
	}
}

static void game_input_unicast(fs_ng_header_t *header, void *buf, size_t len)
{
	switch (header->u.uid) {
		case FS_NETD_UID: {
			fs_inbuf_t inbuf;	
			FS_INBUF_INIT(&inbuf, buf, len);
			fs_rpc_dispatch(netd_rpc_lpc, header->u.uid, &inbuf);
			break;
	   }
		default:
			uid_to_client(header->u.uid, buf, len);
	}
}

static void game_input_multicast(fs_ng_header_t *header, void *buf, size_t len)
{
	int i;
	assert(header->u.uid_count >= 0);

	fs_uid_t *uid = buf;
	void *payload = uid + header->u.uid_count;
	size_t payload_len = len - ((char*)payload - (char*)buf);

	fs_nc_header_t hdr;
	FS_NC_SET_HEADER(&hdr, payload_len);

	// printf("game input multicast\n");
	for(i = 0; i < header->u.uid_count; ++i, ++uid) {
		fs_net_connection_t *conn = apr_hash_get(client_base.user_hash, uid, sizeof(*uid));
		if (conn != NULL) {
			/*
			if (0 < fs_net_connection_send_packet(conn, &hdr, sizeof(hdr), payload, payload_len)) {
				printf("send client conn error\n");
			}
			*/
			//send_to_client(conn, payload, payload_len);
			to_client(conn, payload, payload_len);
		}
	}
}

static void game_input_broadcast(fs_ng_header_t *header, void *buf, size_t len)
{
	apr_hash_index_t *hi;
	void *conn;

	fs_nc_header_t hdr;
	FS_NC_SET_HEADER(&hdr, len);

	for (hi = apr_hash_first(NULL, client_base.user_hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &conn);
		if (CLIENT_GAME_STAT((fs_net_connection_t*)conn) == LOGIN_OK) {
			/*
			if (0 < fs_net_connection_send_packet(conn, &hdr, sizeof(hdr), buf, len)) {
				printf("send client conn error\n");
			}
			*/
			//send_to_client(conn, buf, len);
			to_client(conn, buf, len);
		}
	}
}

static void game_input_dispatch(fs_net_connection_t * gconn, fs_ng_header_t *header, void *buf, size_t len, void *udata)
{
	//printf("game input cmd:%d\n", header->cmd);
	switch (header->cmd) {
	case FS_NG_REMOVE:
		game_input_remove(header, buf, len);
		break;
	case FS_NG_UNICAST:
		game_input_unicast(header, buf, len);
		break;
	case FS_NG_MULTICAST:
		game_input_multicast(header, buf, len);
		break;
	case FS_NG_BROADCAST:
		game_input_broadcast(header, buf, len);
		break;
	default:
		log_error(MOD_NETD, "game input unknow cmd:%d", header->cmd);
	}
}

static void game_connection_recv(struct bufferevent *bufev, void *arg)
{
	READ_PACKET(fs_ng_header_t, bufev, arg, game_input_dispatch, NULL);
}



static void close_client_service()
{
	if (is_client_serviece_open) {
		log_info(MOD_NETD, "close client service");
		if (game_conn != NULL) {
			fs_net_connection_destroy(game_conn);
			game_conn = NULL;
		}

		if (client_acceptor != NULL) {
			fs_net_acceptor_destroy(client_acceptor);
			client_acceptor = NULL;
		}

		is_client_serviece_open = 0;
	}
}


static void game_connection_error(struct bufferevent *bufev, short what, void *arg)
{
	log_error(MOD_NETD, "connection of gamed error:%d", what);
	active_close_all_client();
	close_client_service();
}


static int open_client_service(int fd)
{
	if (!is_client_serviece_open) {
		log_info(MOD_NETD, "open client service");
		if (game_conn == NULL) {
			//fs_net_acceptor_t *acceptor = arg;
			int conn_fd = accept(fd, NULL, NULL);
			if (conn_fd < 0) {
				perror("accept game connection");
				return -1 ;
			}
			log_info(MOD_NETD, "game accept:%d", conn_fd);
			fs_net_set_nonblock(conn_fd);
			game_conn = fs_net_connection_new(conn_fd, game_connection_recv, NULL, 
					game_connection_error, game_max_outbuf, NULL);
		}
		if (client_acceptor == NULL) {
			log_info(MOD_NETD, "listen on client:%s, %d", client_ip, client_port);
			int client_fd = fs_net_listen(client_ip, client_port, backlog, 0);
			if (client_fd < 0) {
				log_info(MOD_NETD, "listen on client error:%s, %d", client_ip, client_port);
				return -1;
			}
			client_acceptor = fs_net_acceptor_new(client_fd, client_accept, NULL);
		}


		is_client_serviece_open = 1;
		return 0;
	}

	return -1;
}


static void game_accept(int fd, short evtype, void *arg)
{
	if (0 > open_client_service(fd)) {
		int conn_fd = accept(fd, NULL, NULL);
		log_error(MOD_NETD, "deny another gamed connection");
		//handle the event and throw it.
		close(conn_fd);
	};
}

static void shutdown_logic()
{
	active_close_all_client();
}

void netd_shutdown_daemon()
{
	shutdown_logic();
	close_client_service();

	if (game_acceptor != NULL) 
		fs_net_acceptor_destroy(game_acceptor);

	//todo :destroy the client conn

	if (auth_httpd != NULL) {
		http_server_destroy(auth_httpd);
	}

	fs_net_mbuf_free(gamed_rpc_buf);
	fs_net_mbuf_free(client_rpc_buf);

	apr_pool_destroy(client_base.pool);

	apr_terminate();
	event_loopexit(NULL);

	if (server_prikey) {
		RSA_free(server_prikey);
	}
	if (client_pubkey) {
		RSA_free(client_pubkey);
	}

	exit(0);
}


int mbuf_to_client(fs_net_connection_t *conn, unsigned headsize, fs_mbuf_t *mbuf, struct bufferevent *bufev)
{
	if (mbuf->data_size > 0) {
		fs_mbuf_blk_t *blk;

		char zbuf[mbuf->data_size];
		int off = 0;
		

		for (blk = mbuf->head; blk != NULL; blk = blk->next) {
			unsigned len = FS_MBUF_BLK_DATA_LEN(blk);
			if (len > 0) {
				//bufferevent_write(bufev, blk->head, len);
				memcpy(zbuf + off, blk->head, len);
				off += len;
			}
		}

		int datasz = mbuf->data_size - headsize;
		zwriter_clear(&CLIENT_ZWRITER(conn));
		zwriter_write(&CLIENT_ZWRITER(conn), zbuf + headsize, datasz);

		//fprintf(stderr, "lzw mbufto client, datalen:%u, zlen:%u\n", datasz, CLIENT_ZWRITER(conn).zoff);
		fs_net_connection_send(conn,  CLIENT_ZWRITER(conn).zbuf, CLIENT_ZWRITER(conn).zoff);
	}

	return mbuf->data_size;
}


//netd login call & auth call
static int rpc_call_client(int argc, svalue_t *argv)
{
	fs_vfd_t vfd; 
	if (fs_rpc_lpc_pack_arg(argc, argv, netd_rpc_lpc, client_rpc_buf, &vfd)) {
		return -1;
	}

	fs_net_connection_t *conn = apr_hash_get(client_base.conn_hash, &vfd, sizeof(vfd));
	if (conn == NULL) {
		log_error(MOD_NETD, "error! no client vfd:%d", vfd);
		return -1;
	}

	FS_NC_SET_HEADER((fs_nc_header_t*)client_rpc_buf->head, FS_NET_MBUF_PAYLOAD_LEN(client_rpc_buf));
	fs_net_mbuf_send(client_rpc_buf, conn);
	//mbuf_to_client(conn, client_rpc_buf->head_size, &client_rpc_buf->mbuf, conn->bufev);
	return 0;
} 

static int rpc_call_gamed(int argc, svalue_t *argv)
{
	int gamed_id;
	if (fs_rpc_lpc_pack_arg(argc, argv, netd_rpc_lpc, gamed_rpc_buf, &gamed_id)) {
		return -1;
	}

	FS_NG_SET_HEADER(gamed_rpc_buf->head, FS_NG_RPC, gamed_id, FS_NET_MBUF_PAYLOAD_LEN(gamed_rpc_buf));
	fs_net_mbuf_send(gamed_rpc_buf, game_conn);
	return 0;
} 

static int rpc_call_imp(int argc, svalue_t *argv)
{
	if (argv[0].type != T_NUMBER) {
		log_error(MOD_NETD, "error! wrong rpc destination type:%s", type_name(argv[0].type));
		return -1;
	}
	int to = argv[0].u.number;
	if (to == FS_GAMED_UID) {
		return rpc_call_gamed(argc, argv);
	} else {
		return rpc_call_client(argc, argv);
	}
}


void f_fs_is_uid_connected(void)
{
	SET_ALL_ARGS;
	fs_uid_t uid = ARGS(0)->u.number;
	POP_ALL_ARGS;
	fs_net_connection_t *conn = apr_hash_get(client_base.user_hash, &uid, sizeof(uid));
	int ret = conn == NULL ? 0 : 1;
	push_number(ret);
}

void f_fs_close_uid_connection(void)
{
	SET_ALL_ARGS;
	fs_uid_t uid = ARGS(0)->u.number;
	POP_ALL_ARGS;
	fs_net_connection_t *conn = apr_hash_get(client_base.user_hash, &uid, sizeof(uid));
	if (conn != NULL) {
		log_error(MOD_NETD, "close uid connection.vfd:%d,ip:%s", conn->vfd, CLIENT_UDATA(conn)->ip);
		active_close_client(conn);
		push_number(0);
	} else {
		push_number(-1);
	}
}

void f_fs_close_vfd_connection(void)
{
	SET_ALL_ARGS;
	fs_vfd_t vfd = ARGS(0)->u.number;
	POP_ALL_ARGS;
	fs_net_connection_t *conn = apr_hash_get(client_base.conn_hash, &vfd, sizeof(vfd));
	if (conn != NULL) {
		log_error(MOD_NETD, "close vfd connection.vfd:%d,ip:%s", conn->vfd, CLIENT_UDATA(conn)->ip);
		active_close_client(conn);
		push_number(0);
	} else {
		push_number(-1);
	}
}

void f_fs_get_vfd_by_uid(void)
{
	SET_ALL_ARGS;
	fs_uid_t uid = ARGS(0)->u.number;
	POP_ALL_ARGS;
	fs_net_connection_t *conn = apr_hash_get(client_base.user_hash, &uid, sizeof(uid));
	if (conn != NULL) {
		push_number(conn->vfd);
	} else {
		push_number(0);
	}
}

void f_fs_get_uid_by_vfd(void)
{
	SET_ALL_ARGS;
	fs_vfd_t vfd = ARGS(0)->u.number;
	POP_ALL_ARGS;
	fs_net_connection_t *conn = apr_hash_get(client_base.conn_hash, &vfd, sizeof(vfd));
	if (conn != NULL) {
		push_number(CLIENT_UID(conn));
	} else {
		push_number(0);
	}
}

void f_fs_get_ip_by_vfd(void)
{
	SET_ALL_ARGS;
	fs_vfd_t vfd = ARGS(0)->u.number;
	POP_ALL_ARGS;
	fs_net_connection_t *conn = apr_hash_get(client_base.conn_hash, &vfd, sizeof(fs_vfd_t));
	if (conn != NULL) {
		copy_and_push_string(CLIENT_UDATA(conn)->ip);
		//todo
	} else {
		push_number(0);
	}
}

void f_fs_new_user(void)
{
	SET_ALL_ARGS;
	fs_vfd_t vfd = ARGS(0)->u.number;
	fs_uid_t uid = ARGS(1)->u.number;
	fs_net_connection_t *conn = apr_hash_get(client_base.conn_hash, &vfd, sizeof(vfd));
	if (conn != NULL) {
		//printf("login ok and new game user uid:%d,vfd:%d\n", uid, vfd);
		CLIENT_GAME_STAT(conn) = LOGIN_OK;
		CLIENT_UID(conn) = uid;
		apr_hash_set(client_base.user_hash, &CLIENT_UID(conn), sizeof(fs_uid_t), conn);
	}
	
	POP_ALL_ARGS;
}


static void signal_event(int sig, short evtype, void *arg)
{
	log_info(MOD_NETD, "netd catch signal:%d", sig);
	if (sig == SIGINT) {
		exit(0);
	}
}

static void emergency_interrupt(int sig)
{
	log_error(MOD_NETD, "emergency interrupt:%d", sig);
}

void static init()
{
	ev_base = event_init();
	init_signal(signal_event, emergency_interrupt);

        apr_initialize();

	client_rpc_buf = fs_net_mbuf_new(sizeof(fs_nc_header_t));
	gamed_rpc_buf = fs_net_mbuf_new(sizeof(fs_ng_header_t));

	setproctitle("-netd_%s", fs_config_get_string(ENGINE_NAME));

	apr_pool_create(&client_base.pool, NULL);
	client_base.conn_hash = apr_hash_make(client_base.pool);
	client_base.user_hash = apr_hash_make(client_base.pool);

 
	int game_fd = fs_net_listen(game_ip, game_port, backlog, 0);
	if (game_fd < 0) {
		perror("listen for game");
		exit(1);
	}
	log_info(MOD_NETD, "listen on game:%s, %d", game_ip, game_port);
	game_acceptor = fs_net_acceptor_new(game_fd, game_accept, NULL);


	if (host_port > 0) {
		//初始化http server
		if ((auth_httpd = http_server_init(ev_base, "0.0.0.0", host_port)) == NULL) {
			log_error(MOD_NETD, "init http server error. port:%d", host_port);
			assert(0);
		}
		log_info(MOD_NETD, "http host listen on:%d", host_port);
	}

	char *rpc_cfg = fs_config_get_string(RPC_CFG);
	netd_rpc_lpc = fs_rpc_lpc_create(rpc_cfg, rpc_call_imp);

}


static void setup_config()
{
	char *gip = fs_config_get_string(NETD_IP);
	if (gip != NULL) {
		game_ip = strdup(gip);
	}

	int gport = fs_config_get_int(NETD_PORT);
	if (gport >= 0) {
		game_port = gport;
	}

	int cport = fs_config_get_int(NETD_OUTER_PORT);
	if (cport >= 0) {
		client_port = cport;
	}
	
	int hport = fs_config_get_int(NETD_HOST_PORT);
	log_info(MOD_NETD, "get host port:%d", hport);
	if (hport >= 0) {
		host_port = hport;
	}

	int hid = fs_config_get_int(HOST_ID);
	if (hid == 0) {
		log_error(MOD_NETD, "have to specify the host id");
		assert(0);
	}

	host_id = hid;

	log_info(MOD_NETD, "netd host_id:%d,client_ip:%s,client_port:%d,game_ip:%s,game_port:%d,host_port:%d",
		host_id, client_ip, client_port, game_ip, game_port, host_port);
}

void setup_rsa()
{
	FILE *fpri = fopen("etc/config/server_privatekey.pem", "r");
        assert(fpri);
	server_prikey = RSA_new();
        assert(PEM_read_RSAPrivateKey(fpri, &server_prikey, NULL, NULL));
	fclose(fpri);


        FILE *fpub = fopen("etc/config/client_publickey.pem", "r");
        assert(fpub);
        client_pubkey = RSA_new();
        assert(PEM_read_RSA_PUBKEY(fpub, &client_pubkey, NULL, NULL));
	fclose(fpub);
}

void module_init_netd()
{
	setup_config();
	init();
	//setup_rsa();

	// 初始化时钟
	int sec, usec;
	sec = fs_config_get_int(SYSTEM_INTERVAL_SEC); 
	usec = fs_config_get_int(SYSTEM_INTERVAL_USEC); 
	int callout_init( int sec, int usec);
	callout_init( sec, usec );

}

void system_startup_netd()
{
	call_script_func("/rc/netd_main", "main", "");
	event_dispatch();
}
