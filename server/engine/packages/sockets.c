// lpc socket的封装
// --------------------------------------------------
// 

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "socket_err.h"
#include "lpc_socket.h"
#include "stack.h"
#include "eoperators.h"


/* flags for socket_close */
#define SC_FORCE        1
#define SC_DO_CALLBACK  2
#define SC_FINAL_CLOSE  4

static lpc_socket_t* lpc_socks = NULL;
// 空闲的socket栈
static ST_I *free_socket_stack = NULL;

static void call_callback(object_t *owner_ob, callback_t* callback, int num_arg); 

void init_sockets()
{
        int size, i;

        if ( lpc_socks != NULL)
        {
                // 只能初始化一次
                printf( "lpc sockets已经初始化过，请勿重复初始化\n" );
                return;

        }

        if (init_stack_int(&free_socket_stack) != 1)
        {
                debug_message("sockets stack init error\n");
                exit(-11);
        }

        // 初始化lpc_sockets;
        size = sizeof(lpc_socket_t) * MAX_LPC_SOCKET;
        lpc_socks = (lpc_socket_t*)MALLOC(size);
        bzero(lpc_socks, size);

	for( i = 0; i < MAX_LPC_SOCKET;i++)
	{
		lpc_socks[i].index = i;
		lpc_socks[i].fd = -1;

		lpc_socks[i].state = CLOSED;

		free_socket_stack->push( free_socket_stack, i );
	}
}

/*
 * set socket non-blocking
 */         
        
INLINE int set_socket_nonblocking (int  fd, int  which)
{
    int result;

    result = ioctl(fd, FIONBIO, &which);

    if (result == -1)
        printf("set_socket_nonblocking: ioctl\n");

    return result;
}

static void clear_callback( callback_t* callback )
{
	switch( callback->func_type )
	{
		case T_FUNCTION:
			free_funp(callback->function.f);
			callback->function.f = NULL;
			break;
		case T_STRING:
			free_string(callback->function.s);
			callback->function.s = NULL;
			break;
		case T_INVALID:
			break;
		default:
			printf("socket_create: old closee_callback's type is Error\n");
			break;
	}

	callback->func_type = T_INVALID;
	callback->function.s = NULL;
}

static void set_close_callback(int i, svalue_t * close_callback) 
{

	clear_callback( &(lpc_socks[i].close_callback));

	if ( close_callback )
	{
		lpc_socks[i].close_callback.func_type = close_callback->type;
		switch( close_callback->type)
		{
		case T_FUNCTION: 
			lpc_socks[i].close_callback.function.f = close_callback->u.fp;
			close_callback->u.fp->hdr.ref++;
			break;
		case T_STRING:
			lpc_socks[i].close_callback.function.s = make_shared_string(close_callback->u.string);
			break;
		default:
			lpc_socks[i].read_callback.func_type = T_INVALID; 
			break;
		}

		return ;
	}
}

static void set_read_callback(int i, svalue_t * read_callback) 
{
	clear_callback( &(lpc_socks[i].read_callback));

	if ( read_callback )
	{
		lpc_socks[i].read_callback.func_type = read_callback->type;
		switch( read_callback->type)
		{
		case T_FUNCTION: 
			lpc_socks[i].read_callback.function.f = read_callback->u.fp;
			read_callback->u.fp->hdr.ref++;
			break;
		case T_STRING:
			lpc_socks[i].read_callback.function.s = make_shared_string(read_callback->u.string);
			break;
		default:
			lpc_socks[i].read_callback.func_type = T_INVALID; 
			break;
		}

		return ;
	}
}

static void set_onconnect_callback(int i, svalue_t * onconnect_callback) 
{
	clear_callback( &(lpc_socks[i].onconnect_callback) );

	if ( onconnect_callback )
	{
		switch( onconnect_callback->type)
		{
		case T_FUNCTION: 
			lpc_socks[i].onconnect_callback.func_type = onconnect_callback->type;
			lpc_socks[i].onconnect_callback.function.f = onconnect_callback->u.fp;
			onconnect_callback->u.fp->hdr.ref++;
			break;
		case T_STRING:
			lpc_socks[i].onconnect_callback.func_type = onconnect_callback->type;
			lpc_socks[i].onconnect_callback.function.s = make_shared_string(onconnect_callback->u.string);
			break;
		default:
			lpc_socks[i].onconnect_callback.func_type = T_INVALID; 
			break;
		}

		return ;
	}
}

static void set_listen_callback(int i, svalue_t * listen_callback) 
{

	clear_callback( &(lpc_socks[i].listen_callback));

	if ( listen_callback )
	{
		lpc_socks[i].listen_callback.func_type = listen_callback->type;
		switch( listen_callback->type)
		{
		case T_FUNCTION: 
			lpc_socks[i].listen_callback.function.f = listen_callback->u.fp;
			listen_callback->u.fp->hdr.ref++;
			break;
		case T_STRING:
			lpc_socks[i].listen_callback.function.s = make_shared_string(listen_callback->u.string);
			break;
		default:
			lpc_socks[i].listen_callback.func_type = T_INVALID; 
			break;
		}

		return ;
	}
}


// 创建socket
//int socket_create(string | function close_callback);
//int socket_create(string | function );
/*
 * Create an LPC efun socket
 */
int socket_create(svalue_t * close_callback)
{
	int type, i, fd, optval;

	type = SOCK_STREAM;

	free_socket_stack->pop(free_socket_stack, &i);

	if (i < 0) 
	{
		printf("获取空闲lpc socket失败！！！\n");		
		return EESOCKET; 
	}

	fd = socket(AF_INET, type, 0);
	if (fd == -1) {
		printf("socket_create: socket\n");		
		close(fd);
		free_socket_stack->push(free_socket_stack, i);
		return EESOCKET;
	}

	optval = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(optval)) == -1) {
		printf("socket_create: setsockopt\n");
		close(fd);
		free_socket_stack->push(free_socket_stack, i);
		return EESETSOCKOPT;
	}
	if (set_socket_nonblocking(fd, 1) == -1)
	{
		printf("socket_create: set_socket_nonblocking\n");
		close(fd);
		free_socket_stack->push(free_socket_stack, i);
		return EENONBLOCK;
	}

	lpc_socks[i].fd = fd;

	set_close_callback(i, close_callback);

	lpc_socks[i].state = UNBOUND;

	//current_object->flags |= O_EFUN_SOCKET;
	lpc_socks[i].owner_ob = current_object;

	return i;
}

/*
 * Write a message on an LPC efun socket
 */
int socket_write(int lpc_fd, svalue_t * message)
{
	int len, off;
	char *buf;

	if (lpc_fd < 0 || lpc_fd >= MAX_LPC_SOCKET)
		return EEFDRANGE;

	if (lpc_socks[lpc_fd].state == CLOSED )
		return EEBADF;

	if (lpc_socks[lpc_fd].owner_ob != current_object)
		return EESECURITY;

	if (lpc_socks[lpc_fd].state != DATA_XFER)
		return EENOTCONN;

	switch (message->type) {
	case T_BUFFER:
		len = message->u.buf->size;
		buf = (char *) DMALLOC(len, TAG_TEMPORARY, "socket_write: T_BUFFER");
		if (buf == NULL)
			fatal("OS error: Out of memory");
		memcpy(buf, message->u.buf->item, len);
		break;
	case T_STRING:
		len = SVALUE_STRLEN(message);
		buf = (char *) DMALLOC(len + 1, TAG_TEMPORARY, "socket_write: T_STRING");
		if (buf == NULL)
			fatal("OS error: Out of memory");

		// strcpy(buf, message->u.string);
		memcpy( buf, message->u.string, len );
		// buf[len] = '\0';
		break;
	case T_ARRAY:
		{
			int i, limit;
			svalue_t *el;

			len = message->u.arr->size * sizeof(int);
			buf = (char *) DMALLOC(len + 1, TAG_TEMPORARY, "socket_write: T_ARRAY");
			if (buf == NULL)
				fatal("OS error: Out of memory");
			el = message->u.arr->item;
			limit = len / sizeof(int);
			for (i = 0; i < limit; i++) {
				switch (el[i].type) {
				case T_NUMBER:
					memcpy((char *) &buf[i * sizeof(int)],
							(char *) &el[i].u.number, sizeof(int));
					break;
				case T_REAL:
					memcpy((char *) &buf[i * sizeof(int)], (char *) &el[i].u.real,
							sizeof(int));
					break;
				default:
					break;
				}
			}
			break;
		}
	default:
		return EETYPENOTSUPP;
	}

	if ( lpc_socks[lpc_fd].connect_ev )
	{
		off = bufferevent_write(lpc_socks[lpc_fd].connect_ev, buf, len); 
	}
	else
	{
		return EESEND;
	}

	if (off == -1) {
		FREE(buf);
		switch (socket_errno) 
		{
		case EWOULDBLOCK:
			return EEWOULDBLOCK;
		default:
			printf("socket_write: send line 348\n");
			return EESEND;
		}
	}

	FREE(buf);

	return EESUCCESS;
}



int socket_close(int lpc_fd, int flags)
{

	// 校验状态
	if (lpc_fd < 0 || lpc_fd >= MAX_LPC_SOCKET)
		return EEFDRANGE;
	if (lpc_socks[lpc_fd].state == CLOSED)
		return EEBADF;
	if (!(flags & SC_FORCE) && lpc_socks[lpc_fd].owner_ob != current_object)
		return EESECURITY;

	if (flags & SC_DO_CALLBACK) {
		push_number(lpc_fd);
		call_callback(lpc_socks[lpc_fd].owner_ob, &(lpc_socks[lpc_fd].close_callback), 1);
	}

	set_read_callback(lpc_fd, 0);
	set_onconnect_callback(lpc_fd, 0);
	set_close_callback(lpc_fd, 0);

	// 清除libevent的事件
        if ( lpc_socks[lpc_fd].listen_ev )
        {
                event_del( lpc_socks[lpc_fd].listen_ev );
                FREE( lpc_socks[lpc_fd].listen_ev );
        }
	// 连接事件
	if ( lpc_socks[lpc_fd].connect_ev )
        {
                bufferevent_free( lpc_socks[lpc_fd].connect_ev );
        }

	// 关闭lpc_fd对应的真实fd
	while (close(lpc_socks[lpc_fd].fd) == -1 && socket_errno == EINTR)
	{
		;	/* empty while */
	}

	lpc_socks[lpc_fd].fd = -1;
	lpc_socks[lpc_fd].state = CLOSED;

	// 压栈
	free_socket_stack->push( free_socket_stack, lpc_fd );

	return EESUCCESS;
}

static void call_callback(object_t *owner_ob, callback_t* callback, int num_arg) 
{
	switch(callback->func_type)
	{
	case T_FUNCTION:
		safe_call_function_pointer(callback->function.f, num_arg);
		break;
	case T_STRING:
		if (callback->function.s[0] == APPLY___INIT_SPECIAL_CHAR)
			error("OS error: Illegal function name.\n");
		safe_apply(callback->function.s, owner_ob, num_arg, ORIGIN_DRIVER);
		break;
	default:
		pop_n_elems( num_arg );	
		break;
	}

}

void listen_handler(int fd, short eventtype, void * args)
{
	lpc_socket_t* lpc_sock = (lpc_socket_t*)args;

	printf( "listened :%d\n", fd);

	// TODO:

	push_number(lpc_sock->index);
	call_callback(lpc_sock->owner_ob, &(lpc_sock->listen_callback), 1);
}

void read_handler(struct bufferevent * ev, void *args)
{
	lpc_socket_t* lpc_sock = (lpc_socket_t*)args;
	struct evbuffer *buffer;
	int bufsz;
	char* p;

	printf( "read :%d@%d\n", lpc_sock->fd, lpc_sock->index);

	// TODO:
	buffer = EVBUFFER_INPUT(ev);
	bufsz = EVBUFFER_LENGTH(buffer);

	// 构建字符串
	p = new_string(bufsz, "socket read_handler");
	evbuffer_remove(buffer, p, bufsz);
        p[bufsz] = '\0';

	push_number(lpc_sock->index);
	push_malloced_string(p);

	call_callback(lpc_sock->owner_ob, &(lpc_sock->read_callback), 2);

	free_string( p );
}

void write_handler(struct bufferevent * ev, void *args)
{
	lpc_socket_t* lpc_sock = (lpc_socket_t*)args;

	printf( "write :%d@%d\n", lpc_sock->fd, lpc_sock->index);

	// 连接完毕
	if ( lpc_sock->connected ) return;	

	lpc_sock->connected = 1;

	push_number(lpc_sock->index);

	call_callback(lpc_sock->owner_ob, &(lpc_sock->onconnect_callback), 1);
}

void socket_error_handler(struct bufferevent * ev, short what, void *args)
{
	printf(" In socket socket_error_handler\n" );
}

static void copy_close_callback(int to, int from) 
{
	clear_callback( &(lpc_socks[to].close_callback));

	lpc_socks[to].close_callback.func_type = lpc_socks[from].close_callback.func_type;

	switch( lpc_socks[from].close_callback.func_type )
	{
	case T_FUNCTION:
		lpc_socks[to].close_callback.function.f = lpc_socks[from].close_callback.function.f;
		lpc_socks[from].close_callback.function.f->hdr.ref++;
		break;
	case T_STRING:
		lpc_socks[to].close_callback.function.s = make_shared_string(lpc_socks[from].close_callback.function.s);	
		break;
	}
}

// socket绑定端口
// 并监听fd设置accept回调函数
//int socket_bind(int fd, int port, string | function listen_callback);
//int socket_bind_and_listen(int, int, string | function );
int socket_bind_and_listen(int lpc_fd, int port, svalue_t * listen_callback)
{
	// 检查lpc_socket的状态
	socklen_t len;
	struct sockaddr_in sin;

	if (lpc_fd < 0 || lpc_fd >= MAX_LPC_SOCKET)
	{
		return EEFDRANGE;
	}

	if (lpc_socks[lpc_fd].state == CLOSED )
	{
		return EEBADF;
	}

	if (lpc_socks[lpc_fd].owner_ob != current_object)
	{
		return EESECURITY;
	}

	if (lpc_socks[lpc_fd].state != UNBOUND)
	{
		return EEISBOUND;
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons((u_short) port);

	if (bind(lpc_socks[lpc_fd].fd, (struct sockaddr *) & sin, sizeof(sin)) == -1) 
	{
		switch (socket_errno) 
		{
		case EADDRINUSE:
			return EEADDRINUSE;
		default:
			printf("socket_bind: bind\n");
			return EEBIND;
		}
	}

	len = sizeof(sin);
	if (getsockname(lpc_socks[lpc_fd].fd, (struct sockaddr *) &lpc_socks[lpc_fd].l_addr, &len) == -1) 
	{
		printf("socket_bind: getsockname\n");
		return EEGETSOCKNAME;
	}
	lpc_socks[lpc_fd].state = BOUND;

        if (listen(lpc_socks[lpc_fd].fd, BACKLOG) == -1)
        {
                printf("socket_listen: listen\n");
                return EELISTEN;
        }
        lpc_socks[lpc_fd].state = LISTEN;

        set_listen_callback(lpc_fd, listen_callback);
        //current_object->flags |= O_EFUN_SOCKET;

	// TODO:libevent
	if ( lpc_socks[lpc_fd].listen_ev )
	{
		event_del( lpc_socks[lpc_fd].listen_ev );
		FREE( lpc_socks[lpc_fd].listen_ev );
	}	

	lpc_socks[lpc_fd].listen_ev = (struct event*) MALLOC( sizeof( struct event));

	event_set(lpc_socks[lpc_fd].listen_ev, lpc_socks[lpc_fd].fd, EV_READ|EV_PERSIST, listen_handler, &(lpc_socks[lpc_fd]));
        event_add(lpc_socks[lpc_fd].listen_ev, NULL);

        return EESUCCESS;
}

// accept函数,设置回调的read_callback,onconnect_callback
// int socket_accept(int fd, string | function readcallback, string | function writecallback);
int socket_accept(int lpc_fd, svalue_t* rcb, svalue_t* wcb )
{
	socklen_t len;
	int accept_fd, i;
	struct sockaddr_in sin;
	struct hostent *hp;

	if (lpc_fd < 0 || lpc_fd >= MAX_LPC_SOCKET)
		return EEFDRANGE;
	if (lpc_socks[lpc_fd].state == CLOSED)
		return EEBADF;
	if (lpc_socks[lpc_fd].owner_ob != current_object)
		return EESECURITY;
	if (lpc_socks[lpc_fd].state != LISTEN)
		return EENOTLISTN;

	len = sizeof(sin);
	accept_fd = accept(lpc_socks[lpc_fd].fd, (struct sockaddr *) & sin, &len);
	if (accept_fd == -1)
	{
		switch (socket_errno)
		{
		case EWOULDBLOCK:
			return EEWOULDBLOCK;
		case EINTR:
			return EEINTR;
		default:
			printf("socket_accept: accept\n");
			return EEACCEPT;
		}
	}

	free_socket_stack->pop(free_socket_stack, &i);

	if ( i < 0)
	{
		close(accept_fd);
		printf("socket_accept: 无空闲lpc_fd\n");
		return i;
	}

	/*
	EV_SET(&kev, accept_fd, EVFILT_READ, EV_ADD, 0, 0, (void*)(long)(LPC_IDENT_BASE + i) );
	if ( kevent(kq, &kev, 1, NULL, 0, NULL) < 0 )
	{
		debug_message("kevent failed %s : %d, errno : %d\n", __FILE__, __LINE__, errno);
		OS_socket_close(accept_fd);
		return EESOCKET;
	}
	*/

	lpc_socks[i].fd = accept_fd;
	lpc_socks[i].state = DATA_XFER;
	lpc_socks[i].l_addr = lpc_socks[lpc_fd].l_addr;
	lpc_socks[i].r_addr = sin;
	lpc_socks[i].owner_ob = NULL;

	hp = gethostbyaddr((char *) &sin.sin_addr.s_addr, (int) sizeof(sin.sin_addr.s_addr), AF_INET);
	if (hp != NULL) 
	{
		strncpy(lpc_socks[i].name, hp->h_name, ADDR_BUF_SIZE);
		lpc_socks[i].name[ADDR_BUF_SIZE - 1] = '\0';
	} 
	else
	{
		lpc_socks[i].name[0] = '\0';
	}

	lpc_socks[i].owner_ob = current_object;

	set_read_callback(i, rcb);
	set_onconnect_callback(i, wcb);
	copy_close_callback(i, lpc_fd);

	if ( lpc_socks[lpc_fd].connect_ev )
	{
		bufferevent_free( lpc_socks[lpc_fd].connect_ev );
	}

	lpc_socks[i].connect_ev = bufferevent_new(accept_fd, read_handler, write_handler, socket_error_handler, &(lpc_socks[i]) );
        bufferevent_enable(lpc_socks[i].connect_ev, EV_READ|EV_WRITE);

	//current_object->flags |= O_EFUN_SOCKET;

	return i;
}

/*
 * Convert a string representation of an address to a sockaddr_in
 */
static int socket_name_to_sin(char * name, struct sockaddr_in * sin)
{
	int port;
	char *cp, addr[ADDR_BUF_SIZE];

	strncpy(addr, name, ADDR_BUF_SIZE);
	addr[ADDR_BUF_SIZE - 1] = '\0';

	cp = strchr(addr, ' ');

	if (cp == NULL) return 0;

	*cp = '\0';
	port = atoi(cp + 1);

	sin->sin_family = AF_INET;
	sin->sin_port = htons((u_short) port);
	sin->sin_addr.s_addr = inet_addr(addr);

	return 1;
}

// 
int socket_connect(int lpc_fd, char* name, svalue_t* rcb, svalue_t* wcb )
{
	if (lpc_fd < 0 || lpc_fd >= MAX_LPC_SOCKET)
		return EEFDRANGE;
	if (lpc_socks[lpc_fd].state == CLOSED)
		return EEBADF;
	if (lpc_socks[lpc_fd].owner_ob != current_object)
		return EESECURITY;
	switch (lpc_socks[lpc_fd].state) 
	{
	case CLOSED:
	case UNBOUND:
	case BOUND:
		break;
	case LISTEN:
		return EEISLISTEN;
	case DATA_XFER:
		return EEISCONN;
	}

	if (!socket_name_to_sin(name, &lpc_socks[lpc_fd].r_addr))
		return EEBADADDR;

	set_read_callback(lpc_fd, rcb);
	set_onconnect_callback(lpc_fd, wcb);

	//current_object->flags |= O_EFUN_SOCKET;

	if (connect(lpc_socks[lpc_fd].fd, (struct sockaddr *) & lpc_socks[lpc_fd].r_addr, sizeof(struct sockaddr_in)) == -1) 
	{
		switch (socket_errno) 
		{
		case EINTR:
			return EEINTR;
		case EADDRINUSE:
			return EEADDRINUSE;
		case EALREADY:
			return EEALREADY;
		case ECONNREFUSED:
			return EECONNREFUSED;
		case EINPROGRESS:
			break;
		default:
			printf("socket_connect: connect\n");
			return EECONNECT;
		}
	}
	lpc_socks[lpc_fd].state = DATA_XFER;

	if ( lpc_socks[lpc_fd].connect_ev )
	{
		bufferevent_free( lpc_socks[lpc_fd].connect_ev );
	}

	lpc_socks[lpc_fd].connect_ev = bufferevent_new(lpc_fd, read_handler, write_handler, socket_error_handler, &(lpc_socks[lpc_fd]) );
        bufferevent_enable(lpc_socks[lpc_fd].connect_ev, EV_READ|EV_WRITE);


	return EESUCCESS;

}

/*          
 * Return the remote address for an LPC efun socket
 */     
int get_socket_address(int lpc_fd, char * addr, int * port)
{               
	if (lpc_fd < 0 || lpc_fd >= MAX_LPC_SOCKET) {
		addr[0] = '\0';
		*port = 0;
		return EEFDRANGE;
	}           
	*port = (int) ntohs(lpc_socks[lpc_fd].r_addr.sin_port);
	sprintf(addr, "%s", inet_ntoa(lpc_socks[lpc_fd].r_addr.sin_addr));
	return EESUCCESS;
} 

void f_socket_create ()
{
	SET_ALL_ARGS
	int res;
	svalue_t* cb = ARGS(0);
	res = socket_create(cb);		
	POP_ALL_ARGS

	push_number( res );
}

void f_socket_close ()
{
	SET_ALL_ARGS
	int lpc_fd = ARGS(0)->u.number;
	int res;

	POP_ALL_ARGS

	res = socket_close(lpc_fd, 0);

	push_number( res );
}

void f_socket_address ()
{
	SET_ALL_ARGS
	char *str;
	int port;
	char addr[ADDR_BUF_SIZE];
	char buf[2 * ADDR_BUF_SIZE]; /* a bit of overkill to be safe */
	int lpc_fd = ARGS(0)->u.number;

	get_socket_address(lpc_fd, addr, &port);
	sprintf(buf, "%s %d", addr, port);
	str = string_copy(buf, "f_socket_address");

	POP_ALL_ARGS

	push_malloced_string(str);
}

void f_socket_bind_and_listen ()
{
	SET_ALL_ARGS
	int lpc_fd = ARGS(0)->u.number;
	int port = ARGS(1)->u.number;
	svalue_t* cb = ARGS(2);
	int res;

	res = socket_bind_and_listen(lpc_fd, port, cb);

	POP_ALL_ARGS

	push_number( res );
}

void f_socket_accept ()
{
	SET_ALL_ARGS
	int lpc_fd = ARGS(0)->u.number;
	svalue_t* rcb = ARGS(1);
	svalue_t* wcb = ARGS(2);
	int res;

	if (!(rcb->type & (T_STRING | T_FUNCTION))) {
		bad_arg(2, F_SOCKET_ACCEPT);
	}

	if (!(wcb->type & (T_STRING | T_FUNCTION))) {
		bad_arg(3, F_SOCKET_ACCEPT);
	}

	res = socket_accept(lpc_fd, rcb, wcb);

	POP_ALL_ARGS

	push_number( res );
}

void f_socket_connect ()
{
	SET_ALL_ARGS
	int lpc_fd = ARGS(0)->u.number;
        char* name= ARGS(1)->u.string;
        svalue_t* rcb = ARGS(2);
        svalue_t* wcb = ARGS(3);
	int res;

        if (!(rcb->type & (T_STRING | T_FUNCTION))) {
                bad_arg(3, F_SOCKET_CONNECT);
        }

        if (!(wcb->type & (T_STRING | T_FUNCTION))) {
                bad_arg(4, F_SOCKET_CONNECT);
        }

	res = socket_connect(lpc_fd, name, rcb, wcb);

	POP_ALL_ARGS

        push_number( res );
}

void f_socket_write ()
{
	SET_ALL_ARGS
        int lpc_fd = ARGS(0)->u.number;
        svalue_t* msg = ARGS(1);
        int res;

        res = socket_write(lpc_fd, msg);

        POP_ALL_ARGS

        push_number( res );
}

void f_dump_socket_status ()
{
	SET_ALL_ARGS
        POP_ALL_ARGS

	push_constant_string("Not Support!!!");	
}
