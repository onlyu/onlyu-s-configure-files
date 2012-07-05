
#ifndef _FS_RPC_C_H
#define _FS_RPC_C_H

#include "fs_rpc.h"
#include "fs_rpc_id.h"

extern fs_rpc_class_meta_t fs_rpc_class_metas[];
extern fs_rpc_function_meta_t fs_rpc_function_metas[];


#define FS_RPC_C_TYPE \
	void *data;	\
	fs_rpc_int_t *intp;	\
	fs_rpc_string_t *stringp;	\

typedef struct fs_rpc_array_s {;
	fs_rpc_size_t n;
	union {
	FS_RPC_C_TYPE
	}u;
} fs_rpc_array_t;

#ifdef __FreeBSD__
#elif MS_WINDOWS
#else
#error "not support platform"
#endif

#endif /*_FS_RPC_C_H*/
