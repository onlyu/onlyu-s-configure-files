
#ifndef _FS_IPC_H_
#define _FS_IPC_H_

#include "fs.h"

#include <sys/types.h>

FS_BEGIN_DECLS

// ipc header between netd and game

typedef int32_t fs_vfd_t; 
typedef int32_t fs_uid_t;  //注意，这里是符号数，否则要相应改动 FS_BROADCAST_VFD
typedef int32_t fs_hid_t;  //服务器id

/*
// 以下的类型是要通过网络发送给客户端的，所以通信双方一定要保证正确一致。
typedef uint32_t fs_protocol_id_t;
typedef int32_t fs_rpc_int_t;
typedef uint32_t fs_rpc_size_t;
*/

//客户端从这个vfd开始分配
#define FS_CLIENT_START_VFD 	256
//表示给客户端广播的VFD	
#define FS_BROADCAST_VFD	0   
//表示发送给game进程的vfd
#define FS_GAMED_VFD   1          
#define FS_NETD_VFD    2   
#define FS_DBD_VFD     3 

//gamed 表示广播的uid
#define FS_BROADCAST_UID	(-1)
//gamed 给netd的uid
#define FS_GAMED_UID	    (-2)
#define FS_NETD_UID	        (-3)
#define FS_DBD_UID	        (-4)

#define FS_NULL_UID	(0)

/*
// 由于客户端的长度是ushort, 如果这里再用ushort的话，要加上广播的vfd，会使和客户端通信的数据实际上变短了。
typedef uint16_t fs_ng_header_payload_len_t;
typedef uint16_t fs_ng_header_cmd_t;
*/

typedef struct fs_ng_header_s {
	/*
	fs_ng_header_payload_len_t payload_len;
	fs_ng_header_cmd_t cmd;
	*/
	uint32_t payload_len:24,
		 cmd:8;
	//不同的cmd有不同的解释，为vfd或者MULTICAST & BROADCAST的长度,考虑到结构体对齐，为4个字节。
	union {
		fs_uid_t uid; 
		fs_uid_t uid_count;
	} u;
} fs_ng_header_t;


FS_CTASSERT(sizeof(fs_ng_header_t) == 8);

typedef enum {
	FS_NG_NEW = 0, // 新建一个玩家 net->game
	FS_NG_REMOVE,  //删除一个玩家 net<->game
	FS_NG_UNICAST, //给一个指定的玩家发数据 game->net
	FS_NG_MULTICAST, //给多个指定的玩家发数据 game->net
	FS_NG_BROADCAST, //给所有登录成功的玩家发数据 game->net
	FS_NG_LOGIN_OK, //玩家登录成功 game->net
	FS_NG_USER, //玩家发给游戏引擎的数据 net->game
	FS_NG_RPC, //net->game rpc call
} fs_ng_cmd_t;


static inline void FS_NG_SET_HEADER(fs_ng_header_t *header, unsigned command, unsigned union_value, unsigned payload_size)
{				 
	(header)->payload_len = (payload_size);
	(header)->cmd = (command);								
	if ((command) == FS_NG_BROADCAST)							
		(header)->u.uid_count = (union_value);						
	else (header)->u.uid = (union_value);							
}

/*
inline void FS_NG_SET_HEADER(fs_ng_header_t *header, fs_ng_cmd_t cmd, fs_vfd_t vfd, uint32_t payload_len)
{
	header->payload_len = payload_len;						
	header->cmd = cmd;								
	if (cmd == FS_NG_BROADCAST)							
		header->u.vfd_count = vfd;						
	else header->u.vfd = vfd;
} 
*/

//服务器之间的通信，用于登录和玩家跳服, host to host
typedef struct fs_hh_header_s {
	uint32_t payload_len:24,
		 cmd:8;
} fs_hh_header_t;

FS_CTASSERT(sizeof(fs_hh_header_t) == 4);

#define FS_HH_SET_HEADER(header, command, payload_size)						\
do {												\
	(header)->cmd = (command);								\
	(header)->payload_len = (payload_size);							\
} while(0)

typedef enum {
	FS_HH_NEW = 0, 
	FS_HH_REMOVE,
	FS_HH_RPC, 
} fs_hh_cmd_t;

typedef struct fs_hh_new_arg_s {
	int32_t hid;
} fs_hh_new_arg_t;

FS_CTASSERT(sizeof(fs_hh_new_arg_t) == 4);

// 和客户端通信要考虑结构体的填充，不要随便修改结构体成员的类型!如改变则要改相应的代码。 
// 4字节对齐且不填充才能对网络buff进行强制类型转换成结构体。
// ipc header between netd and client
typedef uint32_t fs_nc_payload_len_t;

typedef struct fs_nc_header_s  fs_nc_header_t;

struct fs_nc_header_s {
	fs_nc_payload_len_t payload_len;
//}__packed __aligned(2);
};

#define FS_NC_SET_HEADER(header, payload_size) ((header)->payload_len = (payload_size))

FS_CTASSERT(sizeof(fs_nc_header_t) == 4);


// between game & db
typedef enum {
	FS_GD_CMD_DB_OBJ_NEW = 0, 
	FS_GD_CMD_DB_OBJ_UNLOAD,
	FS_GD_CMD_DB_OBJ_LOAD,
	FS_GD_CMD_DB_OBJ_LOAD_SYNC,
	FS_GD_CMD_DB_OBJ_DATA_DIRTY,
	FS_GD_CMD_DB_OBJ_DATA,
	FS_GD_RPC,
	FS_GD_CMD_SYNC,
	FS_GD_CMD_RENAME,
	FS_GD_CMD_GAMED_EXIT,
} fs_gd_cmd_t;

//db --> gamed stat
#define FS_GD_STAT_OK 0
#define FS_GD_STAT_NULL 1
#define FS_GD_STAT_ERROR 2

#define FS_GD_FLAG_INT_KEY 1
#define FS_GD_FLAG_STR_KEY 2


#define FS_GD_ASYNC 0
#define FS_GD_SYNC 1

#define FS_GD_SINGLE_THREAD 0
#define FS_GD_MULTI_W_THREAD 1
#define FS_GD_MULTI_RW_THREAD 2

typedef struct fs_gd_header_s {
	uint32_t cmd:16,
		 stat:8, //db回传给gamed
		 flag:8; //int or string key 
	uint32_t payload_len;
	fs_uid_t uid;
} fs_gd_header_t;

FS_CTASSERT(sizeof(fs_gd_header_t) == 12);


#define FS_GD_SET_HEADER(header, command, user_id, payload) 	\
do { 								\
	(header)->cmd = (command);				\
	(header)->stat = FS_GD_STAT_OK;			\
	(header)->uid = (user_id);				\
	(header)->payload_len = (payload);			\
} while (0)

	

/////////////////////////

#define READ_PACKET(header_type, bufev, arg, packet_handler, udata)	\
do {							\
	size_t bufsz = 0;				\
	size_t packet_sz = 0;				\
	header_type *header = NULL;			\
	fs_net_connection_t *conn = arg;		\
	while ((bufsz = EVBUFFER_LENGTH(EVBUFFER_INPUT(bufev))) > 0) {		\
		if (bufsz < sizeof(*header))  					\
			break;							\
		header = (header_type *)EVBUFFER_DATA(EVBUFFER_INPUT(bufev));	\
		packet_sz = sizeof(*header) + header->payload_len;		\
		if (bufsz < packet_sz) 						\
			break;								\
		(packet_handler)(conn, header, header + 1, header->payload_len, (udata));	\
		evbuffer_drain(EVBUFFER_INPUT(bufev), packet_sz);			\
	}										\
} while(0)

FS_END_DECLS

#endif /* _FS_IPC_H_ */


