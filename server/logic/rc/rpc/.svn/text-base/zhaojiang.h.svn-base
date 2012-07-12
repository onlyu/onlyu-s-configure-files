#ifndef _rc_rpc_zhaojiang
#define _rc_rpc_zhaojiang

#include "/rc/rpc/rpc_id.h"


#include "/include/zhaojiang.h"

void rpc_server_zhaojiang_list(object user);

void rpc_client_zhaojiang_list(int uid,  class ZhaojiangFellow* summonList) { fs_rpc_call(uid, RPC_CLIENT_ZHAOJIANG_LIST, summonList); }

void rpc_server_zhaojiang_get_fellow(object user, int type);

void rpc_client_zhaojiang_get_fellow(int uid, int type) { fs_rpc_call(uid, RPC_CLIENT_ZHAOJIANG_GET_FELLOW, type); }

void rpc_server_zhaojiang_detail(object user, int type);

void rpc_client_zhaojiang_detail(int uid, int type, class FellowDetail fellowDetail) { fs_rpc_call(uid, RPC_CLIENT_ZHAOJIANG_DETAIL, type, fellowDetail); }
#endif
