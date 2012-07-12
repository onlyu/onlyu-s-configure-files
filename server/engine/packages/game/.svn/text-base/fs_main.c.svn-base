// 封神主模块
// ------------------------------------------------------
// 1、实现主循环
// 2、玩家、NPC、物品基本管理
// 3、存盘
// ------------------------------------------------------

#include "fs_main.h"
#include "fs_config.h"
#include "fs_rpc_lpc.h"
#include "fs_rpc_c.h"
#include "fs_dbi.h"
#include "fs_callout.h"
#include "fs_user.h"
#include "http_lpc.h"
#include "fs_db.h"
#include "fs_callout.h"
#include "fs_curl.h"
#include "fs_dns.h"

// ======================================================
#include "fs_comm.h"
#include "fs_ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>

#include "fs_common.h"
#include "fs_npc.h"
#include "fs_summon.h"
#include "fs_item.h"
#include "fs_log.h"
#include "fs_data_save.h"
#include "fs_char_ob.h"
#include "fs_hlist.h"

static const int backlog = 15;
static size_t max_outbuf = (1.8 * 1024 * 1024 * 1024);

static char *netd_ip = "127.0.0.1";
static unsigned short default_netd_port = 60001;

static char *dbd_ip = "127.0.0.1";
static unsigned short default_dbd_port = 60010;

fs_net_connection_t *netd_conn = NULL;
fs_net_connection_t *dbd_conn = NULL;

static fs_net_mbuf_t *db_netbuf = NULL;

int is_gamed_start = 0;
int g_sync_dbd_fd = -1;
struct evhttp *g_httpd = NULL;
time_t last_pto_timestamp_cache = 0;

fs_rpc_lpc_t *gamed_rpc_lpc = NULL;

#define NETD_MBUF(conn) (&((fs_ng_buf_t*)conn->udata)->mbuf)

#define PACKET_SIZE_PER_FRAME 100  //(30 300 30000) usec * 100 = (3,000  30,000  3,000,000) usec


void static init_connect();
static void shutdown_server();
// ======================================================


// libevent中,如果需要处理信号,只能将信号注册到一个libevent实例上。
struct event_base * ev_base;

// 事件的优先级管理
// -------------------------------------------------------
// 系统设置5级事件管理
// 0 信号处理,保证信号在每帧最优先被处理
// 1 预留
// 2 一般网络包处理用
// 3 预留
// 4 给心跳使用
// 为防止心跳优先级别太低无法产生心跳,在网络包处理N个之后，
// 必须check一下心跳。这样在系统网络繁忙的时候，心跳可能会由
// 网络包的收发来激活。
// @see also include/fs_main.h 注释
// -------------------------------------------------------



inline fs_evtimer_t* new_evtimer_data()
{

	return (fs_evtimer_t*)MALLOC( sizeof( fs_evtimer_t ));
}

inline void free_evtimer_data(fs_evtimer_t* evtimer)
{
	FREE( evtimer )	;
}

static void timer_handler(int fd, short evnet, void *arg)
{
	fs_evtimer_t* evtimer_data = (fs_evtimer_t*)arg;

	if ( evtimer_data->type == TIMER_PERSIST)
	{
		add_timer(evtimer_data->period, evtimer_data->callback, evtimer_data->arg);
	}

	(*evtimer_data->callback)(evtimer_data->arg);

	// 釋放
	free_evtimer_data( evtimer_data );
}

// 系统要处理的信号
static void signal_deal( )
{
	/*
	// 暂时不做信号处理，交由虚拟机主函数作。
	// 防止main_loop中出现死循环时，不响应信号
	struct event sig_term;
	struct event sig_int;

	// SIGHTERM信号回调
	event_set(&sig_term, SIGTERM, EV_SIGNAL|EV_PERSIST, sig_term_handler, &sig_term);
	event_priority_set(&sig_term, FS_EV_PRIORITY_SIG);

	// SIGHINT信号回调
	event_set(&sig_int, SIGINT, EV_SIGNAL|EV_PERSIST, sig_int_handler, &sig_int);
	event_priority_set(&sig_int, FS_EV_PRIORITY_SIG);


	event_add(&sig_term, NULL);
	event_add(&sig_int, NULL);
	*/
}

// 初始化引擎关于libevnet部分
int init_libevent()
{
	// 初始化libevent
	ev_base = event_init();
	// 初始化事件处理级别
	event_base_priority_init(ev_base, FS_EV_PRIORITY_MAX);

	// 设置信号处理
	signal_deal();

	return 0;
}

// 增加一个永久定时器
// struct timeval period 周期
// void (*callback)(void *) 回调函数 
// void* arg 参数
// return 0 定时器添加成功
int add_timer(struct timeval period, void (*callback)(void *), void * arg)
{
	struct event * timer;
	fs_evtimer_t* evtimer_data;

	// 分配空间
	evtimer_data = new_evtimer_data();
	
	timer = &evtimer_data->timer;

	// 初始化数据
	evtimer_data->type = TIMER_PERSIST;
	evtimer_data->period.tv_sec = period.tv_sec;
	evtimer_data->period.tv_usec = period.tv_usec;
	evtimer_data->callback = callback;
	evtimer_data->arg = arg;

	// 设置timer的回调 
	evtimer_set(timer, timer_handler, evtimer_data);
	//event_priority_set(timer, FS_EV_PRIORITY_CAL);
	//提升callout的等级优先于网络数据
	event_priority_set(timer, FS_EV_PRIORITY_REMARK1);
	evtimer_add(timer, &period);


	return 0;
}

// 增加一个一次性定时器
// struct timeval period 周期
// void (*callback)(void *) 回调函数 
// void* arg 参数
// return 0 定时器添加成功
int add_timer_once(struct timeval period, void (*callback)(void *), void * arg)
{
	struct event * timer;
	fs_evtimer_t* evtimer_data;

	// 分配空间
	evtimer_data = new_evtimer_data();

	timer = &evtimer_data->timer;

	// 初始化数据
	evtimer_data->type = TIMER_ONCE;
	evtimer_data->period.tv_sec = period.tv_sec;
	evtimer_data->period.tv_usec = period.tv_usec;
	evtimer_data->callback = callback;
	evtimer_data->arg = arg;

	// 设置timer的回调 
	evtimer_set(timer, timer_handler, evtimer_data);
	event_priority_set(timer, FS_EV_PRIORITY_CAL);

	evtimer_add(timer, &period);

	return 0;
}

void test_timer(void* arg)
{
	int this_time = (int) time(NULL);
	printf( "In test_timer %d\n", this_time );
}

void test_timer_once(void* arg)
{
	int this_time = (int) time(NULL);
	printf( "In test_timer_once %d\n", this_time );
}


// 游戏的主循环
// ------------------------------------------------------
extern int eval_cost;
extern int max_cost;

int main_loop()
{
	int ret;
	int this_time = (int) time(NULL);

	
	printf( "Begin LOOP time %d\n", this_time );

	//void rpc_c_test();
	//rpc_c_test();

	max_cost = MAX_COST;
	eval_cost = max_cost;

	// 事件分发
	ret = event_dispatch();

	printf("ret = %d\n", ret);

	return (0);
}

// 腳本用主循環函數體
void f_main_loop()
{
	main_loop();
}

// =============================================================

static void active_close_all_client()
{
}

static void netd_input_new(fs_ng_header_t *header, void *buf, size_t len)
{
	log_info(MOD_GAMED, "netd new user uid:%d", header->u.uid);
}

void netd_remove(fs_uid_t uid);
static void netd_input_remove(fs_ng_header_t *header, void *buf, size_t len)
{
	netd_remove(header->u.uid);
	log_info(MOD_GAMED, "netd close new connection:%d", header->u.uid);
	call_script_func("cmd/login_gamed", "client_connection_close_cb", "i", header->u.uid);
}

inline static int fs_rpc_c_call(fs_ng_cmd_t cmd, fs_uid_t *uidp, int uidc, fs_protocol_id_t pid, void *value)
{
	fs_uid_t uid;
	fs_rpc_function_t *fun = fs_rpc_function_at(gamed_rpc_lpc->function_table, pid);
	if (fun == NULL) {
		log_error(MOD_GAMED, "error! no such c rpc call.pid:%d", pid);
		return -1;
	}

	fs_ng_buf_reset(netd_conn->udata);
	fs_mbuf_t *mbuf = NETD_MBUF(netd_conn);

	if (cmd == FS_NG_MULTICAST) {
		/*
		int i;
		for (i = 0; i < uidc; ++i) {
			FS_MBUF_ENQ_TYPE(mbuf, &uidp[i], fs_uid_t);
		}
		*/
		FS_MBUF_ENQ(mbuf, uidp, uidc * sizeof(fs_uid_t));
		uid = uidc;
	} else {
		uid = *uidp;
	}

	FS_MBUF_ENQ_TYPE(mbuf, &pid, fs_protocol_id_t);
	if (fs_rpc_c_pack(fun, value, mbuf)) {
		return -1;
	}

	fs_ng_buf_send(netd_conn->udata, cmd, uid, netd_conn);
	return 0;
} 

int fs_rpc_c_multicast(fs_uid_t *uidp, int uidc, fs_protocol_id_t pid, void *value)
{
	return fs_rpc_c_call(FS_NG_MULTICAST, uidp, uidc, pid, value);
}

int fs_rpc_c_unicast(fs_uid_t uid, fs_protocol_id_t pid, void *value)
{
	return fs_rpc_c_call(FS_NG_UNICAST, &uid, 1, pid, value);
}

int fs_rpc_c_broadcast(fs_protocol_id_t pid, void *value)
{
	fs_uid_t uid = FS_BROADCAST_UID;
	return fs_rpc_c_call(FS_NG_BROADCAST, &uid, 1, pid, value);
}

void fs_game_login_ok(fs_uid_t uid, int usernum )
{
	fs_ng_send_cmd(netd_conn, FS_NG_LOGIN_OK, uid, NULL, 0);
}

int fs_gamed_close_uid_connection(fs_uid_t uid)
{
	fs_ng_send_cmd(netd_conn, FS_NG_REMOVE, uid, NULL, 0);
	return 0;
}

//玩家输入的网络信息
static void netd_input_user(fs_ng_header_t *header, void *buf, size_t len)
{
	fs_inbuf_t inbuf;	
	FS_INBUF_INIT(&inbuf, buf, len);
	fs_rpc_user_dispatch(gamed_rpc_lpc, header->u.uid, &inbuf);
}

static void netd_input_rpc(fs_ng_header_t *header, void *buf, size_t len)
{
	fs_inbuf_t inbuf;	
	FS_INBUF_INIT(&inbuf, buf, len);
	fs_rpc_dispatch(gamed_rpc_lpc, header->u.uid, &inbuf);
}



static void netd_input(fs_net_connection_t *conn, fs_ng_header_t *header, void *buf, size_t len, void *udata)
{
	//printf("netd input cmd:%d, header->len:%d, payload len=%ld\n", header->cmd, header->payload_len, len);
	switch (header->cmd) {
	case FS_NG_NEW:
		netd_input_new(header, buf, len);
		break;
	case FS_NG_REMOVE:
		netd_input_remove(header, buf, len);
		break;
	case FS_NG_USER:
		netd_input_user(header, buf, len);
		break;
	case FS_NG_RPC:
		netd_input_rpc(header, buf, len);
		break;
	default:
		log_error(MOD_GAMED, "netd input unknow cmd:%d", header->cmd);
	}
}

static void netd_connection_recv(struct bufferevent *bufev, void *arg)
{
	//在这里设置时间戳，可以省些cpu，但是如果处理网络协议的时间过长，那会导致时间戳滞后比较大。
	last_pto_timestamp_cache = time(NULL);
	//READ_PACKET(fs_ng_header_t, bufev, arg, netd_input, NULL);

	size_t bufsz = 0;				
	size_t packet_sz = 0;				
	fs_ng_header_t *header = NULL;			
	fs_net_connection_t *conn = arg;		
	int cnt = 0;

	while ((bufsz = EVBUFFER_LENGTH(EVBUFFER_INPUT(bufev))) > 0) {		
		if (bufsz < sizeof(*header))  					
			break;							
		header = (fs_ng_header_t *)EVBUFFER_DATA(EVBUFFER_INPUT(bufev));	
		packet_sz = sizeof(*header) + header->payload_len;		
		if (bufsz < packet_sz) 						
			break;								

		netd_input(conn, header, header + 1, header->payload_len, NULL);	
		evbuffer_drain(EVBUFFER_INPUT(bufev), packet_sz);			

		if (++cnt >= PACKET_SIZE_PER_FRAME) {
			update_clock(NULL);
			cnt = 0;
			//break;
		}
	}
}

static void netd_connection_error(struct bufferevent *bufev, short what, void *arg)
{
	log_error(MOD_GAMED, "netd connection error. what:0x%02x,errno:%d,size=%lu\n", 
		what, errno,  EVBUFFER_LENGTH(EVBUFFER_OUTPUT(bufev)));
	shutdown_server();
}


static void dbd_input_rpc(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	fs_inbuf_t inbuf;	
	FS_INBUF_INIT(&inbuf, buf, len);
	fs_rpc_dispatch(gamed_rpc_lpc, header->uid, &inbuf);
}

static void dbd_input(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len, void *udata)
{
	// printf("dbd input cmd:%d, header->len:%d, payload len=%ld\n", header->cmd, header->payload_len, len);
	char *data = header->payload_len == 0 ? NULL : buf;
	assert(header->payload_len == len);
	switch (header->cmd) {
		case FS_GD_CMD_DB_OBJ_DATA:
			fs_db_object_recv(header, data, len);
			break;
		case FS_GD_RPC:
			dbd_input_rpc(conn, header, data, len);
			break;
		default:
			log_error(MOD_GAMED, "dbd input unknow cmd:%d", header->cmd);
        }
}

static void dbd_connection_recv(struct bufferevent *bufev, void *arg)
{
	READ_PACKET(fs_gd_header_t, bufev, arg, dbd_input, NULL);
}

static void dbd_connection_error(struct bufferevent *bufev, short what, void *arg)
{
	log_error(MOD_GAMED, "connection of dbd error. what:0x%02x,errno:%d,size=%lu", 
		what, errno,  EVBUFFER_LENGTH(EVBUFFER_OUTPUT(bufev)));
	shutdown_server();
}

static void shutdown_server()
{
	if (dbd_conn != NULL) {
		fs_net_set_block(dbd_conn->fd, NULL);
	}
	call_script_func("/rc/signal", "gamed_on_shutdown", "");
	if(dbd_conn != NULL) {
		evbuffer_write(EVBUFFER_OUTPUT(dbd_conn->bufev), dbd_conn->fd);
	}

	active_close_all_client();
	fs_net_mbuf_free(db_netbuf);

	if (netd_conn != NULL) {
		fs_ng_buf_destroy(netd_conn->udata);
		fs_net_connection_destroy(netd_conn);
		netd_conn = NULL;
	}

	if (dbd_conn != NULL) {
		fs_net_connection_destroy(dbd_conn);
		dbd_conn = NULL;
	}

	if (g_httpd != NULL) {
		http_server_destroy(g_httpd);
	}

	//todo :destroy the client conn
	fs_db_object_module_exit();
	apr_terminate();
	fs_curl_deinit();
	fs_dns_deinit();
	event_loopexit(NULL);
	log_error(MOD_GAMED, "exit for shutdow");
	exit(0);
}

void static init_connect()
{
	char *ip = fs_config_get_string(NETD_IP);
	int port = fs_config_get_int(NETD_PORT);
	if (ip != NULL) {
		netd_ip = strdup(ip);
	}
	if (port >= 0) {
		default_netd_port = port;
	}

	ip = fs_config_get_string(DBD_IP);
	port = fs_config_get_int(DBD_PORT);
	if (ip != NULL) {
		dbd_ip = strdup(ip);
	}
	if (port >= 0) {
		default_dbd_port = port;
	}
		
	/* 先连接dbd，再连接netd，然后netd才对外提供服务 */
	int dbd_fd = fs_net_connect(dbd_ip, default_dbd_port, 1);
	if (dbd_fd < 0) {
		log_error(MOD_GAMED, "exit for fail to connect dbd");
		exit(1);
	} 
	fs_net_set_nonblock(dbd_fd);
	
	dbd_conn = fs_net_connection_new(dbd_fd, dbd_connection_recv, NULL, dbd_connection_error, max_outbuf, NULL);
	log_info(MOD_GAMED, "connect to async dbd ok:%d", dbd_fd);

	//为了和dbd保证两条socket的顺序，这里强制睡眠一下
	log_info(MOD_GAMED, "sleep for 3 secondes\n");
	sleep(3);

	int sync_dbd_fd = fs_net_connect(dbd_ip, default_dbd_port, 1);
	if (sync_dbd_fd < 0) {
		log_error(MOD_GAMED, "exit for fail to sync connect dbd\n");
		exit(1);
	}
	g_sync_dbd_fd = sync_dbd_fd;
	log_info(MOD_GAMED, "connect to sync dbd ok:%d", sync_dbd_fd);


	int netd_fd = fs_net_connect(netd_ip, default_netd_port, 1);
	if (netd_fd < 0) {
		log_error(MOD_GAMED, "exit for fail to connect netd");
		exit(1);
	}
	fs_net_set_nonblock(netd_fd);
	netd_conn = fs_net_connection_new_ext(netd_fd, netd_connection_recv, NULL, netd_connection_error, 
						max_outbuf, sizeof(fs_ng_buf_t));
	fs_ng_buf_init(netd_conn->udata);
	log_info(MOD_GAMED, "connect to async netd ok:%d", netd_fd);

	int http_port = fs_config_get_int(HTTPD_PORT);
	if (http_port > 0) {
		//初始化http server
		if ((g_httpd = http_server_init(ev_base, "0.0.0.0", http_port)) == NULL) {
			log_error(MOD_GAMED, "init http server error. port:%d", http_port);
			assert(0);
		}
		log_info(MOD_GAMED, "httpd listen on:%d", http_port);
	}
}

// =============================================================

static int to_gamed(int argc, svalue_t *argv)
{
	fs_protocol_id_t pid = argv[1].u.number;
	fs_uid_t uid;
	fs_ng_cmd_t cmd;

	fs_rpc_function_t *function = fs_rpc_function_at(gamed_rpc_lpc->function_table, pid);
	if (function == NULL)  {
		log_error(MOD_GAMED, "no such rpc function to call.pid:%d", pid);
		return -1;
	}

	fs_ng_buf_reset(netd_conn->udata);
	fs_mbuf_t *mbuf = NETD_MBUF(netd_conn);

	switch (argv[0].type) {
		case T_ARRAY: {
			int i;
			array_t *uids = argv[0].u.arr;
			for (i = 0; i < uids->size; ++i) {
				assert(uids->item[i].type & T_NUMBER);
				uid = uids->item[i].u.number;
				FS_MBUF_ENQ_TYPE(mbuf, &uid, fs_uid_t);
			}
			uid = uids->size;
			cmd = FS_NG_MULTICAST;
			break;
	      	}
		case T_NUMBER:{
			uid = argv[0].u.number;
			cmd = (uid == FS_BROADCAST_UID) ? FS_NG_BROADCAST : FS_NG_UNICAST;
			break;
		}
		default: {
			log_error(MOD_GAMED, "bad type uid for rpc call");
			return -1;
		 }
	}

	// add the pid
	FS_MBUF_ENQ_TYPE(mbuf, &pid, fs_protocol_id_t);

	// skip the uid arg for arg_class field, and skip the pid arg.
	if (fs_rpc_lpc_pack(function, &argv[2], argc - 2, mbuf)) {
		return -1;
	}

	fs_ng_buf_send(netd_conn->udata, cmd, uid, netd_conn);

	return 0;
}

static int to_dbd(int argc, svalue_t *argv)
{
	int gamed_id;
	if (fs_rpc_lpc_pack_arg(argc, argv, gamed_rpc_lpc, db_netbuf, &gamed_id)) {
		return -1;
	}

	FS_GD_SET_HEADER((fs_gd_header_t*)db_netbuf->head, FS_GD_RPC, gamed_id, FS_NET_MBUF_PAYLOAD_LEN(db_netbuf));
	fs_net_mbuf_send(db_netbuf, dbd_conn);
	return 0;
}


static int rpc_call_imp(int argc, svalue_t *argv)
{
	if (argv[0].type == T_NUMBER) {
		int to = argv[0].u.number;
		if (to == FS_DBD_UID) {
			return to_dbd(argc, argv);
		}
	} 

	return to_gamed(argc, argv);
}

void load_config()
{
	/*
	svalue_t * config_sv = get_config_svalue(); 
	assert(config_sv != NULL);
	*/

	// 依次读出配置给各个模块使用
	// 1、系时钟部分
	// --------------------------------------------
	{
		// 只能初始化一次，不提供脚本动态更新接口
		int sec, usec;
		sec = fs_config_get_int(SYSTEM_INTERVAL_SEC); 
		usec = fs_config_get_int(SYSTEM_INTERVAL_USEC); 
		callout_init( sec, usec );
	}	

	// --------------------------------------------
	// TODO:2、在线人数部分
	// --------------------------------------------
	{
		int max_online, max_login;

		max_online = (unsigned int)fs_config_get_int(MAX_ONLINE_KEY);
		max_login = (unsigned int)fs_config_get_int(MAX_LOGIN_KEY);

		fprintf( stderr, "init user manage .......: max_online: %d, max_login: %d", max_online, max_login );

		init_user_mng(max_online, max_login);
		printf( "done \n" );
	}

	// --------------------------------------------
	// TODO:3、协议配置部分
	// --------------------------------------------
	
	// --------------------------------------------
}

static void signal_event(int sig, short evtype, void *arg)
{
	log_info(MOD_GAMED, "catch signal:%d", sig);
	call_script_func("/rc/signal", "gamed_on_signal", "i", sig);
	if (sig == SIGINT) {
		shutdown_server();
	}
}

static void emergency_interrupt(int sig)
{
	log_error(MOD_GAMED, "emergency interrupt:%d", sig);
}

void restore_gamed_proc_title()
{
	setproctitle("-gamed_%s", fs_config_get_string(ENGINE_NAME));
}

void init_sockets();
void module_init_gamed()
{
	//初始化脏数据的内存池管理
	dirty_mem_pool_setup();

	init_libevent();
	init_signal(signal_event, emergency_interrupt);
	fs_curl_init();
	fs_dns_init();

	init_sockets();

	// 获取配置，并根据配置初始化 
	load_config();


	apr_initialize();

	int max_online = (unsigned int)fs_config_get_int(MAX_ONLINE_KEY);

	fs_db_object_module_init();
	
	// 初始化原子表
	fs_default_init_atom_table();
	// 初始化NPC
	fs_sys_init_npc_module(max_online * 60, NPC_START_ID, NPC_END_ID);
	// 初始化物品
	fs_sys_init_item_module(max_online * 300, ITEM_START_ID, ITEM_END_ID);
	// 初始化召唤兽
	fs_sys_init_summon_module(max_online * 30, SUMMON_START_ID, SUMMON_END_ID);

	/* LOG开关*/
	//fs_set_log_switch_module(MOD_SCENE);
	fs_set_log_switch_level(LOG_ERROR);


	void fs_user_manage_init();
	fs_user_manage_init();
	restore_gamed_proc_title();
}

void system_startup_gamed()
{

	assert(is_gamed_start == 0);
	// 初始化网络连接
	init_connect();

	db_netbuf = fs_net_mbuf_new(sizeof(fs_gd_header_t));

	char *rpc_cfg = fs_config_get_string(RPC_CFG);
	gamed_rpc_lpc = fs_rpc_lpc_create(rpc_cfg, rpc_call_imp);

	is_gamed_start = 1;
	// 主流程
	call_script_func("/rc/main", "main", "");
}

void f_dump_memory_pool()
{
	SET_ALL_ARGS

	fs_dump_char_ob();
	fs_dump_hlist_node();

	POP_ALL_ARGS
}

void f_get_time_of_day()
{
	struct timeval nowval;

	array_t *vec;

	SET_ALL_ARGS

	vec = allocate_array(2);

	gettimeofday(&nowval,0);

	vec->item[0].type = T_NUMBER;
	vec->item[0].u.number = nowval.tv_sec;
	vec->item[1].type = T_NUMBER;
	vec->item[1].u.number = nowval.tv_usec / 1000;

	POP_ALL_ARGS

	push_refed_array(vec);
}

//int
void f_enable_log_pto()
{
	SET_ALL_ARGS
	enable_log_pto(ARGS(0)->u.number);
	POP_ALL_ARGS
}

