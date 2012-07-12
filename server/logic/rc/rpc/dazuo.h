#ifndef _rc_rpc_dazuo
#define _rc_rpc_dazuo

#include "/rc/rpc/rpc_id.h"


void rpc_server_dazuo_start(object user);

void rpc_client_dazuo_start(int uid) { fs_rpc_call(uid, RPC_CLIENT_DAZUO_START); }

void rpc_server_dazuo_leave(object user);

void rpc_client_dazuo_leave(int uid) { fs_rpc_call(uid, RPC_CLIENT_DAZUO_LEAVE); }

void rpc_client_dazuo_info(int uid, int leftTime, int totalExp) { fs_rpc_call(uid, RPC_CLIENT_DAZUO_INFO, leftTime, totalExp); }

void rpc_client_dazuo_addexp(int uid, int exp) { fs_rpc_call(uid, RPC_CLIENT_DAZUO_ADDEXP, exp); }






























































#endif
