#ifndef _rc_rpc_client_drama
#define _rc_rpc_client_drama

#include "/rc/rpc/rpc_id.h"


void rpc_client_drama_play(int uid, string dramaId) { fs_rpc_call(uid, RPC_CLIENT_DRAMA_PLAY, dramaId); }
void rpc_server_drama_finish(object user, string dramaId);

// 内服调试剧情用
void rpc_client_drama_test(int uid, string dramaId, string json) { fs_rpc_call(uid, RPC_CLIENT_DRAMA_TEST, dramaId, json); }
#endif
