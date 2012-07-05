
#include "fs_comm.h"
#include "fs_mbuf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

/////////////////////////////////////////////////////////////////

fs_net_acceptor_t* fs_net_acceptor_new(int fd, eventcb acceptcb, void *udata)
{
	fs_net_acceptor_t *acceptor = calloc(1, sizeof(fs_net_acceptor_t));
	acceptor->fd = fd;
	acceptor->udata = udata;
	event_set(&acceptor->ev, fd, EV_READ|EV_PERSIST, acceptcb, acceptor);
	event_add(&acceptor->ev, NULL);
	return acceptor;
}

void fs_net_acceptor_destroy(fs_net_acceptor_t *acceptor)
{
	event_del(&acceptor->ev);
	close(acceptor->fd); //todo: should not close the fd here
	free(acceptor);
}

inline static fs_net_connection_t * connection_new(int fd, evbuffercb readcb, 
			evbuffercb writecb, everrorcb errorcb, size_t max_outbuf, size_t obj_size)
{
	fs_net_connection_t *conn = calloc(1, obj_size);
	conn->fd = fd;
	conn->max_outbuf = max_outbuf;
	conn->bufev = bufferevent_new(fd, readcb, writecb, errorcb, conn);
	bufferevent_enable(conn->bufev, EV_READ|EV_WRITE);
	return conn ;
}

fs_net_connection_t * fs_net_connection_new(int fd, evbuffercb readcb, evbuffercb writecb, 
						everrorcb errorcb, size_t max_outbuf,void *udata)
{
	fs_net_connection_t *conn = connection_new(fd, readcb, writecb, errorcb, max_outbuf, sizeof(fs_net_connection_t));
	conn->udata = udata;
	return conn ;
}

fs_net_connection_t * fs_net_connection_new_ext(int fd, evbuffercb readcb, evbuffercb writecb, 
						everrorcb errorcb, size_t max_outbuf, size_t ext)
{
	fs_net_connection_t *conn = connection_new(fd, readcb, writecb, errorcb, max_outbuf, sizeof(fs_net_connection_t) + ext);
	conn->udata = conn + 1;
	return conn ;
}

void fs_net_connection_destroy(fs_net_connection_t *conn)
{
	bufferevent_free(conn->bufev);
	close(conn->fd); //todo : should not close the fd  here
	free(conn);
}

int fs_net_connection_try_flush(fs_net_connection_t *conn)
{
	size_t size = EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev));
        if (size > 0) {   
		// write just one time, not loop write to avoid blocking the server running.
		int w = evbuffer_write(EVBUFFER_OUTPUT(conn->bufev),  conn->fd); 
		return  w < 0 ? -1 : size - w;
        }   
	return 0;
}

void fs_net_connection_max_outbuf(fs_net_connection_t *conn, size_t max_outbuf) {
	conn->max_outbuf = max_outbuf;
}

int fs_net_connection_send(fs_net_connection_t *conn, const void *buf, size_t len)
{
	/*
	size_t size = EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev));
	if (size + len >= conn->max_outbuf) {
		return FS_ERROR_OUTBUF_FULL;
	}
	*/
	int ret = bufferevent_write(conn->bufev, buf, len);
	if (ret != 0) {
		fprintf(stderr, "net send error.bufsize=%lu,errno=%d\n",
			EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev)), errno);
	}
	return ret;
}

int fs_net_connection_send_packet(fs_net_connection_t *conn, const void *header, size_t hdrlen, 
				const void *payload, size_t pldlen)
{
	int ret;
	/*
	size_t size = EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev));

	if (size + hdrlen + pldlen >= conn->max_outbuf) {
		return FS_ERROR_OUTBUF_FULL;
	}
	*/

	if ((ret = bufferevent_write(conn->bufev, header, hdrlen)) == 0) {
		ret = bufferevent_write(conn->bufev, payload, pldlen);
		if (ret != 0) {
			fprintf(stderr, "net send packet payload error.bufsize=%lu,errno=%d\n",
				EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev)), errno);
		}
	} else {
		fprintf(stderr, "net send packet header error.bufsize=%lu,errno=%d\n",
			EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev)), errno);
	}
	return ret;
}



int fs_net_convert_addr(const char*ip, unsigned short port, struct sockaddr_in *addr)
{
	struct in_addr inaddr;
	memset(&inaddr, 0, sizeof(inaddr));
	memset(addr, 0, sizeof(*addr));
	int ret = inet_aton(ip, &inaddr);
	if (1 == ret) {
		addr->sin_family = AF_INET;
		addr->sin_port = htons(port);
		//addr->sin_addr.s_addr = inaddr.s_addr;
		addr->sin_addr = inaddr;
	}

	assert(ret == 1);

	return ret;
}

int fs_net_set_nonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int fs_net_set_block(int fd, int *rflag)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (rflag != NULL) {
		*rflag = flag;
	}
	return fcntl(fd, F_SETFL, flag & (~O_NONBLOCK));
}

int fs_net_connect(const char *ip, unsigned short port, int block)
{
	struct sockaddr_in addr;
	int fd;
	printf("connect to %s:%d\n", ip, port);
	if (1 == fs_net_convert_addr(ip, port, &addr)) {
		fd = socket(PF_INET, SOCK_STREAM, 0);
		if (0 > fd) {
			perror("fs net socket");
			return -1;
		}
		if (!block) {
			if (0 > fs_net_set_nonblock(fd)){
				perror("fs net fcntl noblock");
				close(fd);
				return -1;
			}
		}
		if (0 > connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
			if (!block && errno == EINPROGRESS) {
				return fd;
			}
			perror("fs net connect");
			close(fd);
			return -1;
		}

		return fd;
	}

	return -1;
}

int fs_net_listen(const char *ip, unsigned short port, int backlog, int block)
{
	struct sockaddr_in addr;
	int fd;
	if (1 == fs_net_convert_addr(ip, port, &addr)) {
		fd = socket(PF_INET, SOCK_STREAM, 0);
		if (0 > fd) {
			perror("fs net socket");
			return -1;
		}
		int reuse = 1;
		if (0 > setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int))) {
			perror("fs net setsockopt");
			close(fd);
			return -1;
		}
		if (0 > bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
			perror("fs net bind");
			close(fd);
			return -1;
		}
		if (0 > listen(fd, backlog)) {
			perror("fs net listen");
			close(fd);
			return -1;
		}
		if (!block) {
			if (0 > fs_net_set_nonblock(fd)){
				perror("fs net fcntl noblock");
				close(fd);
				return -1;
			}
		}

		return fd;
	}

	return -1;
}


void fs_signal_listen(fs_signal_t *s, int sig, eventcb handler)
{
	s->sig = sig;
	event_set(&s->ev, sig, EV_SIGNAL|EV_PERSIST, handler, s);
	event_add(&s->ev, NULL);
}

void  fs_signal_remove(fs_signal_t *sig)
{
	event_del(&sig->ev);
}


void init_signal(eventcb signal_event, void(*signal_interrupt)(int))
{
	int i;
        int sig_list[] = {SIGUSR1, SIGTERM, SIGHUP, /*SIGTSTP,*/ SIGINT,SIGPIPE,SIGCHLD, };
	#define SIGNAL_NUM (sizeof(sig_list)/sizeof(sig_list[0]))
	static fs_signal_t sig_ctl[SIGNAL_NUM];

        for(i = 0; i < SIGNAL_NUM; i++) {   
		fs_signal_listen(&sig_ctl[i], sig_list[i], signal_event);
        }   

        signal(SIGUSR2, signal_interrupt) ;
}

int fs_ng_send_cmd(fs_net_connection_t *conn, fs_ng_cmd_t cmd, fs_uid_t uid, void *buf, size_t len)
{
        fs_ng_header_t nghdr;

	if (buf == NULL) { 
		FS_NG_SET_HEADER(&nghdr, cmd, uid, 0);
		return fs_net_connection_send(conn, &nghdr, sizeof(nghdr));
	} else {
		FS_NG_SET_HEADER(&nghdr, cmd, uid, len);
		return fs_net_connection_send_packet(conn, &nghdr, sizeof(nghdr), buf, len);
	}

}

int fs_gd_send_cmd(fs_net_connection_t *conn, fs_gd_cmd_t cmd, fs_uid_t uid, void *buf, size_t len)
{
	fs_gd_header_t header;

	if (buf == NULL || len == 0) {
		FS_GD_SET_HEADER(&header, cmd, uid, 0);
		return fs_net_connection_send(conn, &header, sizeof(header));
	} else {
		FS_GD_SET_HEADER(&header, cmd, uid, len);
		return fs_net_connection_send_packet(conn, &header, sizeof(header), buf, len);
	}
}

/*
int                                                                                                                                                                                                        
bufferevent_add(struct event *ev, int timeout)                                                                                                                                                                    
{                                                                                                                                                                                                                 
	    struct timeval tv, *ptv = NULL;                                                                                                                                                                               
		if (timeout) {                                                                                                                                                                                                
				evutil_timerclear(&tv);                                                                                                                                                                                   
				tv.tv_sec = timeout;                                                                                                                                                                                      
				ptv = &tv;                                                                                                                                                                                                
		}                                                                                                                                                                                                             
			                                                                                                                                                                                                                  
		return (event_add(ev, ptv));                                                                                                                                                                                  
}     

void schedule_write(fs_net_connection_t *conn)
{
	struct bufferevent *bufev = conn->bufev;
	if ((bufev->enabled & EV_WRITE)) {                                                                                                                                                                  
		int ret = bufferevent_add(&bufev->ev_write, bufev->timeout_write); 
		fprintf(stderr, "bufferevent add ret=%d\n", ret);
	}
}
*/

static int fs_mbuf_to_bufferevent(fs_mbuf_t *mbuf, fs_net_connection_t *conn)
{
	if (mbuf->data_size > 0) {
		fs_mbuf_blk_t *blk;
		/*
		size_t size = EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev));
		if (size + mbuf->data_size >= conn->max_outbuf) {
			fprintf(stderr, "error mbuf to bufev full.bufsize=%lu,maxbuf=%lu\n", size, conn->max_outbuf);
			schedule_write(conn);
			return -1;
		}
		*/

		for (blk = mbuf->head; blk != NULL; blk = blk->next) {
			unsigned len = FS_MBUF_BLK_DATA_LEN(blk);
			if (len > 0) {
				int ret = bufferevent_write(conn->bufev, blk->head, len);
				if (ret != 0) {
					fprintf(stderr, "mbuf to bufferevent error.bufsize=%lu,errno=%d\n",
						EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev)), errno);
				}
			}
		}
	}

	return mbuf->data_size;
}

int fs_gd_send_cmd_mbuf(fs_net_connection_t *conn, fs_gd_header_t *header, fs_mbuf_t *mbuf)
{
	size_t headlen = sizeof(fs_gd_header_t);
	/*
	size_t size = EVBUFFER_LENGTH(EVBUFFER_OUTPUT(conn->bufev));
	if (size + headlen + mbuf->data_size >= conn->max_outbuf) {
		fprintf(stderr, "error gd to bufev full.bufsize=%lu,maxbuf=%lu\n", size, conn->max_outbuf);
		return -1;
	}
	*/

	header->payload_len = mbuf->data_size;
	bufferevent_write(conn->bufev, header, headlen);
	fs_mbuf_to_bufferevent(mbuf, conn);

	return mbuf->data_size + headlen;
}


int fs_net_mbuf_send(fs_net_mbuf_t *netbuf, fs_net_connection_t *conn)
{
	return fs_mbuf_to_bufferevent(&netbuf->mbuf, conn);
}

static int mbuf_to_evbuffer(fs_mbuf_t *mbuf, struct evbuffer *buf)
{
	if (mbuf->data_size > 0) {
		fs_mbuf_blk_t *blk;
		for (blk = mbuf->head; blk != NULL; blk = blk->next) {
			unsigned len = FS_MBUF_BLK_DATA_LEN(blk);
			if (len > 0) {
				evbuffer_add(buf, blk->head, len);
			}
		}
	}

	return mbuf->data_size;
}

int fs_gd_send_sync(fs_net_connection_t *conn, fs_gd_header_t *hdr, fs_mbuf_t *mbuf)
{
	unsigned left;
	int ret;
	int flag;
	struct evbuffer *outevb = EVBUFFER_OUTPUT(conn->bufev);

	if (mbuf == NULL) {
		hdr->payload_len = 0;
		evbuffer_add(outevb, hdr, sizeof(*hdr));
	} else {
		hdr->payload_len = mbuf->data_size;
		evbuffer_add(outevb, hdr, sizeof(*hdr));
		mbuf_to_evbuffer(mbuf, outevb);
	}

	fs_net_set_block(conn->fd, &flag);
	for(left = EVBUFFER_LENGTH(outevb); left > 0; ) {
		ret = write(conn->fd, EVBUFFER_DATA(outevb), left);
		if (ret > 0) {
			left -= ret;
			evbuffer_drain(outevb, ret);
		} else if (ret < 0) {
			//todo: singal check ....
			evbuffer_drain(outevb, EVBUFFER_LENGTH(outevb));
			perror("rpc call db sync write error");
			goto error;
		}
	}
	assert(EVBUFFER_LENGTH(outevb) == 0);
	fcntl(conn->fd, F_SETFL, flag);
	return 0;

error:
	fcntl(conn->fd, F_SETFL, flag);
	return -1;
}

struct evbuffer * fs_gd_recv_sync(int fd)
{
	struct evbuffer *inevb = evbuffer_new();
	int ret = 0;				
	size_t packet_sz = 0;				
	fs_gd_header_t *header = NULL;			
	size_t bufsz = 1024;
	char buf[bufsz];

	//fprintf(stderr, "warn sync recv from gamed\n");
	while (1) {		
		ret = read(fd, buf, bufsz); 
		if (ret < 0) {
			perror("rpc call db sync read error");
			if (ret == EINTR) {
				fprintf(stderr, "inter sync recv\n");
				continue;
			}
			return NULL;
		} else if (ret == 0) {
			fprintf(stderr, "sync db socket close\n");
			assert(0);
			return NULL;
		} else {
			evbuffer_add(inevb, buf, ret);
			if (EVBUFFER_LENGTH(inevb) < sizeof(fs_gd_header_t))  					
				continue;							

			if (header == NULL) {
				header = (fs_gd_header_t *)EVBUFFER_DATA(inevb);	
				packet_sz = sizeof(fs_gd_header_t) + header->payload_len;		
			}

			if (EVBUFFER_LENGTH(inevb) >= packet_sz)
				break;								
		}
	}										

	//fprintf(stderr, "warn sync recv from gamed ok\n");
	assert(EVBUFFER_LENGTH(inevb) == packet_sz);
	return inevb;
}

void fs_ng_buf_init(fs_ng_buf_t *ngbuf)
{
	fs_mbuf_init(&ngbuf->mbuf, 0);
	ngbuf->head = FS_MBUF_ALLOC(&ngbuf->mbuf, sizeof(*ngbuf->head));
}

void fs_ng_buf_reset(fs_ng_buf_t *ngbuf)
{
	fs_mbuf_reset(&ngbuf->mbuf);
	ngbuf->head = FS_MBUF_ALLOC(&ngbuf->mbuf, sizeof(*ngbuf->head));
}

void fs_ng_buf_destroy(fs_ng_buf_t *ngbuf)
{
	fs_mbuf_destruct(&ngbuf->mbuf);
}

int fs_ng_buf_send(fs_ng_buf_t *ngbuf, fs_ng_cmd_t cmd, fs_uid_t uid, fs_net_connection_t *conn)
{
	FS_NG_SET_HEADER(ngbuf->head, cmd, uid, ngbuf->mbuf.data_size - sizeof(*ngbuf->head));
	return fs_mbuf_to_bufferevent(&ngbuf->mbuf, conn);
}

void fs_hh_buf_init(fs_hh_buf_t *hhbuf)
{
	fs_mbuf_init(&hhbuf->mbuf, 0);
	hhbuf->head = FS_MBUF_ALLOC(&hhbuf->mbuf, sizeof(*hhbuf->head));
}

void fs_hh_buf_reset(fs_hh_buf_t *hhbuf)
{
	fs_mbuf_reset(&hhbuf->mbuf);
	hhbuf->head = FS_MBUF_ALLOC(&hhbuf->mbuf, sizeof(*hhbuf->head));
}

void fs_hh_buf_destroy(fs_hh_buf_t *hhbuf)
{
	fs_mbuf_destruct(&hhbuf->mbuf);
}

unsigned fs_hh_buf_send(fs_hh_buf_t *hhbuf, fs_hh_cmd_t cmd, fs_net_connection_t *conn)
{
	FS_HH_SET_HEADER(hhbuf->head, cmd, hhbuf->mbuf.data_size - sizeof(*hhbuf->head));
	fs_mbuf_to_bufferevent(&hhbuf->mbuf, conn);
	return hhbuf->mbuf.data_size;
}

void fs_apr_hash_construct(fs_apr_hash_t *table)
{
	apr_pool_create(&table->pool, NULL);
	table->hash = apr_hash_make(table->pool);
}

void fs_apr_hash_destruct(fs_apr_hash_t *table)
{
	apr_pool_destroy(table->pool);
}
//////////////////////////////////

void fs_stack_init(fs_stack_t *stack, size_t count, size_t size, fs_stack_detor_t detor, void *udata)
{
	stack->obj_cnt = count;
	stack->obj_origsize = size;
	stack->obj_size = 4 * ((size + 4 - 1) / 4); //align  to  4
	stack->obj = calloc(count, stack->obj_size);
	stack->top = 0;
	stack->detor = detor;
	stack->udata = udata;
}

void fs_stack_destroy(fs_stack_t *stack)
{
	if (stack->detor) {
		int i;
		for (i = 0; i < stack->top; ++i) {
			stack->detor(&stack->obj[i * stack->obj_size], stack->udata);
		}
	}

	free(stack->obj);
	stack->obj = NULL;
}

#define FS_STACK_IS_EMPTY(st) ((st)->top == 0)
#define FS_STACK_IS_FULL(st) ((st)->top == (st)->obj_cnt)

int fs_stack_is_empty(fs_stack_t *stack)
{
	return FS_STACK_IS_EMPTY(stack);
}

int fs_stack_is_full(fs_stack_t *stack)
{
	return FS_STACK_IS_FULL(stack);
}

size_t fs_stack_size(fs_stack_t *stack)
{
	return stack->top;
}

int fs_stack_push(fs_stack_t *stack, void *obj)
{
	if (FS_STACK_IS_FULL(stack))
		return -1;

	memcpy(&stack->obj[stack->top++ * stack->obj_size], obj, stack->obj_origsize);
	return stack->top;
}

void *fs_stack_pop(fs_stack_t *stack)
{
	if (FS_STACK_IS_EMPTY(stack))
		return NULL;

	return (void*)&stack->obj[--stack->top * stack->obj_size];
}

