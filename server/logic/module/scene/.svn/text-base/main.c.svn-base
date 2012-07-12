
#include <var_prop.h>
#include <user_key.h>
#include <npc_key.h>
#include <macros.h>
#include <debug.h>
#include "/rc/rpc/lscene.h"

static object logger;
#define SCENE_TYPE_REAL 0
#define SCENE_TYPE_VIRTUAL 1

#define VIRTUAL_SCENE_START 10000

MEMORY_VAR(allMap, ([]))
MEMORY_VAR(allScene, ([]))
MEMORY_VAR(allTypeScene, ({([]), ([])}))

MEMORY_VAR(virtualSceneIdx, VIRTUAL_SCENE_START)

class Map {
	int id;
	string name;
	int width;
	int height;
}

class Scene {
	int id;
	int type;
	class Map map;
	int *uids;
	class SceneEntity *userEntities;
	mapping user;
	mapping npc;
	class SceneEntity *npcEntities;
	mapping ext;
}

#define USER_GET_SCENE(user) ((user)->getTemp(KT_USER_SCENE))
#define USER_SET_SCENE(user,sc) ((user)->setTemp(KT_USER_SCENE, (sc)))
#define USER_DEL_SCENE(user) ((user)->delTemp(KT_USER_SCENE))

#define NPC_GET_SCENE(nid) (GetNpc((nid),K_NPC_SCENE))
#define NPC_SET_SCENE(nid,sceneId) (SetNpc((nid),K_NPC_SCENE,(sceneId)))

static class Scene createScene(int id, int type, int mapId)
{
	class Map map = allMap[mapId];
	if (map) {
		class Scene scene = new(class Scene);
		scene->id = id;
		scene->type = type;
		scene->map = map;
		scene->uids = ({});
		scene->user = ([]);
		scene->userEntities = ({});
		scene->npc = ([]);
		scene->npcEntities = ({});
		allScene[id] = scene;
		allTypeScene[type][id] = scene;
		return scene;
	} else {
		return 0;
	}
}

static class Scene createRealScene(int mapId)
{
	class Scene sc = createScene(mapId, SCENE_TYPE_REAL, mapId);
	return sc;
}

int createVirtualScene(int mapId)
{
	class Scene sc = createScene(virtualSceneIdx++, SCENE_TYPE_VIRTUAL, mapId);
	return sc == 0 ? 0 : sc->id;
}

int isVirtualScene(int sceneId)
{
	return sceneId >= VIRTUAL_SCENE_START;
}

int getLocationMapId(object user)
{
	class Scene scene = USER_GET_SCENE(user);
	if (scene) {
		return scene->map->id;
	}
	return 0;
}

static void freeScene(class Scene scene)
{
	map_delete(allScene, scene->id);
	map_delete(allTypeScene[scene->type], scene->id);
}

static class SceneEntity leave(class Scene scene, object user, int refresh)
{
	int uid = user->getId();
	class SceneEntity entity = scene->user[uid];
	if (entity) {
		map_delete(scene->user, uid);
		USER_DEL_SCENE(user);
		scene->uids = keys(scene->user);
		scene->userEntities = values(scene->user);
		if (refresh) {
			rpc_client_scene_del(scene->uids, uid);
		}
		logger->Debug("player remove uid=%d,sceneId=%d,mapId=%d",
			uid, scene->id, scene->map->id);
	}

	return entity;
}


static class SceneEntity nLeave(class Scene scene, int nid)
{
	class SceneEntity entity = scene->npc[nid];

	if (entity) {
		map_delete(scene->npc, nid);
		scene->npcEntities = values(scene->npc);
		rpc_client_scene_del(scene->uids, nid);
		logger->Debug("npc leave nid=%s,sceneId=%d,mapId=%d",
			entity->name, scene->id, scene->map->id);

		logger->Debug("dump n leave scene:%O", scene);
	}

	return entity;
}

static int enter(class Scene scene, object user, int x, int y)
{
	int uid = user->getId();
	/*if (scene->user[uid]) {
		return 0;
	}*/

	class SceneEntity entity;
	class Scene preScene = USER_GET_SCENE(user);
	if (preScene) {
		entity = leave(preScene, user, 1);
	} else {
		entity = new(class SceneEntity);
		entity->id = uid;
		entity->icon = user->getIcon();
		entity->name = user->getName();
		entity->title = user->getTitle();
	}
	entity->x = x;
	entity->y = y;
	
	USER_SET_SCENE(user, scene);
	user->setLocation(scene->id, x, y);
	//to me
	rpc_client_scene_hero_enter(uid, scene->id, scene->map->id, entity);
	rpc_client_scene_add_player(({uid}), scene->userEntities);
	rpc_client_scene_add_npc(({uid}), scene->npcEntities);
	logger->Debug("user enter to me uid=%d,users=%O", uid, scene->userEntities);
	//to other
	rpc_client_scene_add_player(scene->uids, ({entity}));
	logger->Debug("user enter to others uid=%d,others=%O", uid, scene->uids);

	scene->user[uid] = entity;
	scene->uids = keys(scene->user);
	scene->userEntities = values(scene->user);
	logger->Debug("user enter ok uid=%d,sceneId=%d,mapId=%d,x=%d,y=%d,uids=%O",
		uid, scene->id, scene->map->id, x, y, scene->uids);
	Import("FUBEN_BASE")->onPlayerEnterScene(user);

	return 1;	
}

int * getNpcLocation(int nid)
{
	int sceneId = NPC_GET_SCENE(nid);
	class Scene scene = allScene[sceneId];
	if (scene) {
		class SceneEntity entity = scene->npc[nid];
		if (entity) {
			return ({scene->id, entity->x, entity->y});
		}
	}
}

void delChar(int uid, int cid)
{
	rpc_client_scene_del(({uid}), cid);
}


void dump()
{
	class Scene scene = allScene[1001];
	logger->Debug("dump fuck scene :%O", scene);

}

static int nEnter(class Scene scene, int nid, int x, int y)
{
	if (scene->npc[nid]) {
		return 0;
	}

	class SceneEntity entity = new(class SceneEntity);
	entity->id = nid;
	entity->icon = GetNpc(nid, K_NPC_ICON);
	entity->x = x;
	entity->y = y;
	entity->name = GetNpc(nid, K_NPC_NAME);
	entity->title = GetNpc(nid, K_NPC_TITLE);
	if (!stringp(entity->title)) {
		entity->title = "";
	}
	
	NPC_SET_SCENE(nid, scene->id);
	scene->npc[nid] = entity;
	scene->npcEntities = values(scene->npc);
	rpc_client_scene_add_npc(scene->uids, ({entity}));

	logger->Debug("npc enter nid=%d,name=%s,sceneId=%d,mapId=%d,x=%d,y=%d",
		nid, entity->name, scene->id, scene->map->id, x, y);

	//logger->Debug("dump n enter scene:%O", scene);
	return 1;
}



static int move(class Scene scene, object user, int x, int y)
{
	int uid = user->getId();
	class SceneEntity entity = scene->user[uid];
	entity->x = x;
	entity->y = y;
	user->setPosition(x, y);

	class SceneMove mv = new(class SceneMove);
	mv->id = user->getId();
	mv->sceneId = scene->id;
	mv->x = x;
	mv->y = y;
	int * uids = scene->uids - ({uid});
	rpc_client_scene_move(scene->uids, mv);

	logger->Debug("user move uid=%d,sceneId=%d,mapId=%d,x=%d,y=%d",
			uid, scene->id, scene->map->id, x, y);
	Import("FUBEN_BASE")->walk(user);
}

int playerEnter(object user, int sceneId, int x, int y)
{
	class Scene scene = allScene[sceneId];
	if (scene) {
		return 0 != enter(scene, user, x, y);
	} else {
		logger->Debug("player enter bad scene uid=%d,sceneId=%d", user->getId(), sceneId);
		return 0;
	}
}

int npcEnter(int nid, int sceneId, int x, int y)
{
	class Scene scene = allScene[sceneId];
	if (scene) {
		return 0 != nEnter(scene, nid, x, y);
	} else {
		logger->Debug("npc enter bad scene nid=%d,sceneId=%d", nid, sceneId);
		return 0;
	}
}

int npcLeave(int nid)
{
	int sceneId = NPC_GET_SCENE(nid);
	class Scene scene = allScene[sceneId];
	
	if (scene) {
		return 0 != nLeave(scene, nid);
	}
	return 0;
}

void playerMove(object user, int sceneId, int x, int y)
{
	class Scene scene = USER_GET_SCENE(user);
	if (scene) {
		if (scene->id == sceneId) {
			move(scene, user, x, y);
		} else {
			logger->Debug("skip move uid=%d,sceneId=%d,x=%d,y=%d",user->getId(), sceneId, x, y);
		}
	}
}

void removeUser(object user)
{
	class Scene scene = USER_GET_SCENE(user);
	if (scene) {
		leave(scene, user, 1);
	}
}

void rpc_server_scene_move(object user, class SceneMove move)
{
	playerMove(user, move->sceneId, move->x, move->y);
}

void rpc_server_scene_click_npc(object user, int nid)
{
	logger->Debug("click npc uid=%d,nid=%d", user->getId(), nid);
	"/module/scene/npc"->clickNpc(user->getId(), nid);
}

void changeLayout(object user, int layout)
{
	class Scene scene = USER_GET_SCENE(user);
	if( !scene ) return;

	int uid = user->getId();
	class SceneEntity entity = scene->user[uid];
	if( !entity ) return;

	user->setLayout(layout);
	entity->icon = layout;
	rpc_client_scene_change_layout(scene->uids, uid, layout);
}

static void loadAllScene()
{
	allMap = ([]);
	mapping mapInfos = "module/scene/data/scene_info"->get_data();
	foreach(int mapId, mapping info in mapInfos) {
		class Map map = new(class Map);
		map->id = mapId;
		map->name = info["name"];
		map->width = info["width"];
		map->height = info["height"];
		allMap[mapId] = map;
		createRealScene(mapId);
		//logger->Debug("load real scene mapId=%d", mapId);
	}
}


void create()
{
	logger = Import("LOG")->New("scene");
}

void initWorld()
{
	loadAllScene();
	"/module/scene/npc"->loadAllNpc();
}

int canNpcShow(int uid, int npcId)
{
	object user = get_user(uid);
	if( !objectp(user) ) return 1;

        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
	if( !sysId ) return 1;

	mapping hideList = Import("MISSION_DATA")->getHideNpcList(user);
	if( undefinedp(hideList[sysId]) ) return 1;

	debug_message("NpcSHowOff:%d", sysId);
	return 0;
}

void loginCheck(object user)
{
	int *pos = user->getLocation();
	logger->Debug("login before enter uid=%d,sceneId=%d,x=%d,y=%d", user->getId(), pos[0], pos[1], pos[2]);
	if (isVirtualScene(pos[0])) {
	 	user->setDefaultLocation();
		pos = user->getLocation();
	}
	logger->Debug("login enter uid=%d,sceneId=%d,x=%d,y=%d", user->getId(), pos[0], pos[1], pos[2]);
	user->enterScene(pos[0], pos[1], pos[2]);
}

