
#ifndef _FS_RPC_LPC_H_
#define _FS_RPC_LPC_H_

#include "fs_rpc.h"
#include "fs_ipc.h"
#include "lpc.h"
#include "lpc_incl.h"

//#include "interpret.h"
#include "program.h"
#include "compiler.h"
#include "macros.h"

typedef int (*rpc_lpc_func_t)(int argc, svalue_t *argv);

typedef struct {
	fs_rpc_table_t rpc_table;
	fs_rpc_function_table_t *function_table;
	rpc_lpc_func_t rpc_call_imp;
	fs_mbuf_t c_unpack_mbuf;
} fs_rpc_lpc_t;

fs_rpc_lpc_t * fs_rpc_lpc_create(const char *cfg, rpc_lpc_func_t imp);
int fs_rpc_lpc_called(int id, fs_rpc_function_t *function, fs_inbuf_t *inbuf);
int fs_rpc_lpc_unpack(fs_rpc_field_t *field, svalue_t *value, int field_count, fs_inbuf_t* inbuf);
int fs_rpc_lpc_pack(fs_rpc_function_t *function, svalue_t *value, int value_count, fs_mbuf_t *mbuf);

void fs_rpc_lpc_module_update(object_t *obj);
int fs_rpc_dispatch(fs_rpc_lpc_t *rpc_lpc, int id, fs_inbuf_t *inbuf);
int fs_rpc_user_dispatch(fs_rpc_lpc_t *rpc_lpc, int uid, fs_inbuf_t *inbuf);
int fs_rpc_lpc_pack_arg(int argc, svalue_t *argv, fs_rpc_lpc_t *rpc_lpc, fs_net_mbuf_t *netbuf, int *rid);

#endif /*_FS_RPC_LPC_H-*/
