#ifndef _rc_rpc_zhaocaimao
#define _rc_rpc_zhaocaimao

#include "/rc/rpc/rpc_id.h"


void rpc_server_mcat_pad(object user);

void rpc_client_mcat_pad(int uid, int yuanbao, int cash, int times) { fs_rpc_call(uid, RPC_CLIENT_MCAT_PAD, yuanbao, cash, times); }

void rpc_server_mcat_zhaocai(object user);

void rpc_server_piliang_zhaocai(object user, int query, int no_more_hint);

void rpc_client_piliang_zhaocai_confirm(int uid, int yuanbao, int times, int cash) { fs_rpc_call(uid, RPC_CLIENT_PILIANG_ZHAOCAI_CONFIRM, yuanbao, times, cash); }


void rpc_client_mcat_finish(int uid, int isSuccess) { fs_rpc_call(uid, RPC_CLIENT_MCAT_FINISH, isSuccess); }















































#endif
