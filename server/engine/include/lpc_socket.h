// --------------------------------------------------
// lpc socketͷ�ļ�
// --------------------------------------------------
// ����lpc_sokcet_t

#ifndef _LPC_SOCKET_H_
#define _LPC_SOCKET_H_

#include <event.h>
#include "std.h"
#include <openssl/ssl.h>

#define MAX_LPC_SOCKET (4096)   // Ԥ��4K lpc socket
#define BACKLOG (5) // �׽ӿ��Ŷӵ�������Ӹ���

#define ADDR_BUF_SIZE   64      /* max length of address string    */

// CLOSED socket��ʼ״̬
// UNBOUND ������ϣ�δ��״̬
// BOUND  �����
// LISTEN ����״̬
// DATA_XFER ���ݽ���״̬ 
// һ��socket����״̬Ǩ��
// 1��CLOSED -> UNBOUND -> BOUND -> LISTEN -> CLOSED
// 2��CLOSED -> DATA_XFER -> CLOSED
enum socket_state {
	CLOSED = 0, UNBOUND, BOUND, LISTEN, DATA_XFER, 
};

// callback����
typedef struct {
	short func_type;
	union string_or_func function;
} callback_t;

typedef struct {
	int index;
	int fd;

	char connected;

	enum socket_state state;
	// ���ص�ַ
	struct sockaddr_in l_addr;
	// ѡ�ε�ַ
	struct sockaddr_in r_addr;

	// ��������
	char name[ADDR_BUF_SIZE];

	// �����¼�
	struct event* listen_ev;
	struct bufferevent * connect_ev;

	// ��¼�ص�����
	object_t *owner_ob;
	// �رջص�����
	callback_t close_callback;

	// �����ص����߶�д�ص�
	union {
		callback_t listen_callback;
		struct {
			callback_t read_callback;
			callback_t onconnect_callback;
		};
	};
} lpc_socket_t; 

// ����

#endif
