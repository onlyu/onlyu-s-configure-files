#ifndef _rc_rpc_block
#define _rc_rpc_block

#include "/rc/rpc/rpc_id.h"

class pos_t {
	int x;
	int y;
}

class block_area_t {
	int is_block;
	class pos_t *pos;
}	

void rpc_client_scene_block(int uid, class block_area_t block_area) { fs_rpc_call(uid, RPC_CLIENT_SCENE_BLOCK, block_area); }
#endif
