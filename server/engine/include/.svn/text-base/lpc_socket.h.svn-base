// --------------------------------------------------
// lpc socket头文件
// --------------------------------------------------
// 定义lpc_sokcet_t

#ifndef _LPC_SOCKET_H_
#define _LPC_SOCKET_H_

#include <event.h>
#include "std.h"
#include <openssl/ssl.h>

#define MAX_LPC_SOCKET (4096)   // 预留4K lpc socket
#define BACKLOG (5) // 套接口排队的最大连接个数

#define ADDR_BUF_SIZE   64      /* max length of address string    */

// CLOSED socket初始状态
// UNBOUND 创建完毕，未绑定状态
// BOUND  绑定完毕
// LISTEN 监听状态
// DATA_XFER 数据交换状态 
// 一般socket两种状态迁移
// 1、CLOSED -> UNBOUND -> BOUND -> LISTEN -> CLOSED
// 2、CLOSED -> DATA_XFER -> CLOSED
enum socket_state {
	CLOSED = 0, UNBOUND, BOUND, LISTEN, DATA_XFER, 
};

// callback定义
typedef struct {
	short func_type;
	union string_or_func function;
} callback_t;

typedef struct {
	int index;
	int fd;

	char connected;

	enum socket_state state;
	// 本地地址
	struct sockaddr_in l_addr;
	// 选段地址
	struct sockaddr_in r_addr;

	// 连接名字
	char name[ADDR_BUF_SIZE];

	// 监听事件
	struct event* listen_ev;
	struct bufferevent * connect_ev;

	// 记录回调对象
	object_t *owner_ob;
	// 关闭回调函数
	callback_t close_callback;

	// 监听回调或者读写回调
	union {
		callback_t listen_callback;
		struct {
			callback_t read_callback;
			callback_t onconnect_callback;
		};
	};
} lpc_socket_t; 

// 定义

#endif
