
#ifndef _FS_COMMON_H_
#define	_FS_COMMON_H_

#include "fs.h"
#include "fs_ipc.h"
#include "fs_mbuf.h"
#include "fs_comm.h"

#include "apr_hash.h"
#include "apr_pools.h"

#include <event.h>
#include <evutil.h>


#define FS_ERROR_OUTBUF_FULL -2


#define TIME_TAG_BEGIN(sw, what)	\
	struct timeval what##_begin, what##_end, what##_dif; \
	if ((sw)) {	\
		gettimeofday(&what##_begin, NULL);	\
	}
#define TIME_TAG_END(sw, what,fmt,args...) \
	if ((sw)) {\
		gettimeofday(&what##_end, NULL); \
		timersub(&what##_end, &what##_begin, &what##_dif); \
		fprintf(stderr, #what " timediff sec=%ld,usec=%ld,"fmt"\n", what##_dif.tv_sec, what##_dif.tv_usec, ##args);\
	}


typedef struct fs_net_acceptor_s fs_net_acceptor_t;
typedef struct fs_net_connection_s fs_net_connection_t;
typedef void (*eventcb)(int fd, short evtype, void *arg);

struct fs_net_acceptor_s {
	int fd;
	struct event ev;
	void *udata;
};

struct fs_net_connection_s {
	int fd;
	fs_vfd_t vfd;
	size_t max_outbuf;
	struct bufferevent *bufev;
	void *udata;
};

typedef struct {
	int sig;
	struct event ev;
} fs_signal_t;


FS_BEGIN_DECLS

fs_net_acceptor_t * fs_net_acceptor_new(int fd, eventcb acceptcb , void *udata);
void fs_net_acceptor_destroy(fs_net_acceptor_t *acceptor);
fs_net_connection_t * fs_net_connection_new(int fd, evbuffercb readcb, evbuffercb writecb, 
					everrorcb errorcb, size_t max_outbuf, void *udata);
fs_net_connection_t * fs_net_connection_new_ext(int fd, evbuffercb readcb, evbuffercb writecb, 
					everrorcb errorcb, size_t max_outbuf, size_t ext);
void fs_net_connection_destroy(fs_net_connection_t *conn);
int fs_net_connection_send(fs_net_connection_t *conn, const void *buf, size_t len);
int fs_net_connection_send_packet(fs_net_connection_t *conn, const void *header, size_t hdrlen, 
				const void *payload, size_t pldlen);
int fs_net_connection_try_flush(fs_net_connection_t *conn);
void fs_net_connection_max_outbuf(fs_net_connection_t *conn, size_t max_outbuf);

int fs_net_listen(const char *ip, unsigned short port, int backlog, int block);
int fs_net_set_nonblock(int fd);
int fs_net_set_block(int fd, int *rflag);
int fs_net_connect(const char *ip, unsigned short port, int block);

void fs_signal_listen(fs_signal_t *s, int sig, eventcb handler);
void fs_signal_remove(fs_signal_t *s);
void init_signal(eventcb signal_event, void(*signal_interrupt)(int));

int fs_ng_send_cmd(fs_net_connection_t *conn, fs_ng_cmd_t cmd, fs_vfd_t vfd, void *buf, size_t len);
int fs_gd_send_cmd(fs_net_connection_t *conn, fs_gd_cmd_t cmd, fs_uid_t uid, void *buf, size_t len);

struct fs_mbuf_s;
int fs_gd_send_cmd_mbuf(fs_net_connection_t *conn, fs_gd_header_t *header, struct fs_mbuf_s *mbuf);
int fs_gd_send_sync(fs_net_connection_t *conn, fs_gd_header_t *hdr, fs_mbuf_t *mbuf);
struct evbuffer * fs_gd_recv_sync(int fd);

int fs_net_mbuf_send(fs_net_mbuf_t *netbuf, fs_net_connection_t *conn);


FS_END_DECLS


///////////////////
typedef void (*fs_stack_detor_t)(void *obj, void *udata);

typedef struct {
	size_t obj_cnt;
	size_t obj_origsize;
	size_t obj_size;
	char *obj;
	int top;
	fs_stack_detor_t detor;
	void *udata;
} fs_stack_t;


void fs_stack_init(fs_stack_t *stack, size_t count, size_t size, fs_stack_detor_t detor, void *udata);
void fs_stack_destroy(fs_stack_t *stack);
int fs_stack_push(fs_stack_t *stack, void *obj);
void *fs_stack_pop(fs_stack_t *stack);
int fs_stack_is_full(fs_stack_t *stack);
int fs_stack_is_empty(fs_stack_t *stack);
size_t fs_stack_size(fs_stack_t *stack);

#define FS_STACK_FOREACH(st, i, objtype, o)	\
	for (i = 0; (i < (st)->top) && (o = (objtype)&(st)->obj[i*(st)->obj_size]); ++i)


typedef struct fs_ng_buf_s {
	fs_ng_header_t *head;
	fs_mbuf_t mbuf;
} fs_ng_buf_t;

void fs_ng_buf_init(fs_ng_buf_t *ngbuf);
void fs_ng_buf_reset(fs_ng_buf_t *ngbuf);
void fs_ng_buf_destroy(fs_ng_buf_t *ngbuf);
int fs_ng_buf_send(fs_ng_buf_t *ngbuf, fs_ng_cmd_t cmd, fs_uid_t uid, fs_net_connection_t *conn);

typedef struct fs_hh_buf_s {
	fs_hh_header_t *head;
	fs_mbuf_t mbuf;
} fs_hh_buf_t;

void fs_hh_buf_init(fs_hh_buf_t *ngbuf);
void fs_hh_buf_reset(fs_hh_buf_t *ngbuf);
void fs_hh_buf_destroy(fs_hh_buf_t *ngbuf);
unsigned fs_hh_buf_send(fs_hh_buf_t *ngbuf, fs_hh_cmd_t cmd, fs_net_connection_t *conn);

typedef struct {
	apr_pool_t *pool;
	apr_hash_t *hash;
} fs_apr_hash_t;

void fs_apr_hash_construct(fs_apr_hash_t *table);
void fs_apr_hash_destruct(fs_apr_hash_t *table);

typedef struct {
	int fd;
	struct evbuffer *inevb;
	struct evbuffer *outevb;
} fs_iobuf_t;

#endif /* _FS_COMMON_H_ */

