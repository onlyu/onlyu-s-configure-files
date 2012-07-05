
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

void rpc_client_scene_hero_enter(int uid, int sceneId, int mapId, class SceneEntity hero);
void rpc_client_scene_add_player(int *uids, class SceneEntity *player);
void rpc_client_scene_add_npc(int *uids, class SceneEntity *npc);

void rpc_client_scene_del(int *uids, int id);
void rpc_client_scene_move(int *uids, class SceneMove move);

void rpc_server_scene_move(object user, class SceneMove move);
void rpc_server_scene_click_npc(object user, int npcId);

void rpc_client_scene_change_layout(int *uids, int uid, int layout);
