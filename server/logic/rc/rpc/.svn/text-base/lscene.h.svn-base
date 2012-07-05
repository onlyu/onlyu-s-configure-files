#ifndef _rc_rpc_lscene
#define _rc_rpc_lscene

#include "/rc/rpc/rpc_id.h"


class SceneEntity {
	int id;
	int icon;
	int x;
	int y;
	string name;
	string title;
}

class SceneMove {
	int id;
	int sceneId;
	int x;
	int y;
}

void rpc_client_scene_hero_enter(int uid, int sceneId, int mapId, class SceneEntity hero) { fs_rpc_call(uid, RPC_CLIENT_SCENE_HERO_ENTER, sceneId, mapId, hero); }
void rpc_client_scene_add_player(int *uids, class SceneEntity *player) { fs_rpc_call(uids, RPC_CLIENT_SCENE_ADD_PLAYER, player); }
void rpc_client_scene_add_npc(int *uids, class SceneEntity *npc) { fs_rpc_call(uids, RPC_CLIENT_SCENE_ADD_NPC, npc); }

void rpc_client_scene_del(int *uids, int id) { fs_rpc_call(uids, RPC_CLIENT_SCENE_DEL, id); }
void rpc_client_scene_move(int *uids, class SceneMove move) { fs_rpc_call(uids, RPC_CLIENT_SCENE_MOVE, move); }

void rpc_server_scene_move(object user, class SceneMove move);
void rpc_server_scene_click_npc(object user, int npcId);

void rpc_client_scene_change_layout(int *uids, int uid, int layout) { fs_rpc_call(uids, RPC_CLIENT_SCENE_CHANGE_LAYOUT, uid, layout); }
#endif
