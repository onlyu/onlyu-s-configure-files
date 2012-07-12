/*
任务管理器
外部对任务的一般操作，如接受任务、放弃任务等都在这操作，而不是直接操作任务obj
*/

#include <macros.h>
#include <user_key.h>
#include <var_prop.h>
#include <frames.h>
#include <debug.h>
#include <menu.h>
#include "mission.h"
#include "/rc/rpc/mission.h"

#define MISSIONS_DIR "module/mission/missions/"

static object logger;
static string headNode = "1001";


static void preloadAllMissions();

// 能否接任务
int canAcceptMission(object user, string missId);

// 任务状态
int getMissionState(object user, string missId);

// 取得指定任务obj
object getMissionObj(string missId)
{
        // xassert( sizeof(missId), missId);
        return find_object(sprintf("%s%s.c", MISSIONS_DIR, missId), 1);
}

// 创建任务追踪对象
static class MissionFollowTarget createFollowTarget(string missId, mapping followData)
{
        class MissionFollowTarget followTarget = new(class MissionFollowTarget);

        followTarget->id = missId;
        followTarget->sceneId = followData["sceneId"];
        followTarget->dx = followData["dx"];
        followTarget->dy = followData["dy"];
        followTarget->npcId = followData["npcId"];
	followTarget->fubenId = followData["fubenId"];
        return followTarget;
}

// 创建任务协议对象
static class MissionInfo createMissionPto(object user, string missId)
{
        object missObj = getMissionObj(missId);
        class MissionInfo pto = new(class MissionInfo);

        pto->id = missId;
        pto->type = missObj->getType();
        pto->state = getMissionState(user, missId);
	pto->acceptTime = missObj->getAcceptTime(user);
        pto->name = missObj->getName();
        pto->followText = missObj->getFollowDesc(user);
        pto->desc = missObj->getDesc();

        mapping followData = missObj->getFollowTarget(user);
        pto->followTarget = createFollowTarget(missId, followData);
        return pto;
}

// 是否分派了任务
int isAssignMission(object user, string missId)
{
        mapping list = MISSION_DATA->getAssignList(user);
        if( !undefinedp(list[missId]) ) return 1;
        return 0;
}

// 任务状态
int getMissionState(object user, string missId)
{
        if( isAssignMission(user, missId) ) return  MISSION_STATE_ASSIGN;

        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) return MISSION_STATE_NULL;
        return missObj->getState(user);
}

// 是否处于任务过程状态
int isProgressMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) return 0;
        return missObj->isProgress(user);
}

// 是否处于就绪状态
int isReadyMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) return 0;
        return missObj->isReady(user);
}

// 是否已完成的任务 不在玩家存盘上的任务一概视为未完成
int isCompletedMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) return 0;
        return missObj->isComplete(user);
}

// 是否已完成过的任务，包括不在玩家存盘上的任务
int hasCompletedMission(object user, string missId)
{
	mapping list = MISSION_DATA->getCompletedList(user);
	return !undefinedp(list[missId]);
}

// 有没有指定任务
int hasMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) {
                return 0;
        }

        return missObj->hasMission(user);
}

// 获取身上已接任务数
int getMissionAmount(object user)
{
	int amount = 0;
	mapping missions = user->getMission();

	foreach(string missId, mapping _ in missions) {
		if( isProgressMission(user, missId) || isReadyMission(user, missId) ) {
			amount++;
		}
	}
	return amount;
}

// 分派一个任务 慎用，一般都是登录时构建好的assignList，并不需要此接口操作
//static void assignMission(object user, string missId)
void assignMission(object user, string missId)
{
	int uid = user->getId();
        MISSION_DATA->addAssignMission(user, missId);

	if( canAcceptMission(user, missId) ) {
		class MissionInfo pto = createMissionPto(user, missId);
		rpc_client_add_canaccept_mission(uid, pto);
	}
}

// 取消分派任务
static void cancelAssignMission(object user, string missId)
{
	MISSION_DATA->delAssignMission(user, missId);

	int uid = user->getId();
	rpc_client_del_canaccept_mission(uid, missId);
}

// 基本的接受任务条件(只检测基本的等级规则)
int canBaseAcceptMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) return 0;
        return missObj->canBaseAccept(user);
}

// 能否接任务
int canAcceptMission(object user, string missId)
{
	if( hasMission(user, missId) ) return 0;
	if( !isAssignMission(user, missId) ) return 0;

        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) return 0;
        return missObj->canAccept(user);
}

// 接受一个任务
int acceptMission(object user, string missId)
{
	if( !canAcceptMission(user, missId) ) return 0;

        object missObj = getMissionObj(missId);
        if( objectp(missObj) && missObj->accept(user) ) {
        	// 取消任务的分派
		cancelAssignMission(user, missId);

                class MissionInfo pto = createMissionPto(user, missId);
                rpc_client_accept_mission(user->getId(), pto);
                return 1;
        }

        return 0;
}

// 完成任务后隐藏某些NPC
static void hideNpc(object user, int sysId, string missId)
{
	MISSION_DATA->addHideNpc(user, sysId, missId);

	int npcId = Import("UNIQ_NPC")->systemId2NpcId(sysId);
	if( !npcId ) return;

	int uid = user->getId();
	int *npcPos = Import("SCENE")->getNpcLocation(npcId);
	int *userPos = user->getLocation();
	if( npcPos[0] != userPos[0] ) return;

	//debug_message("hide npc:%d, %d, %s", sysId, npcId, missId);
	//TODO
	//"cmd/scene"->sendDelChar(uid, npcId);
}

// 完成任务后显示某些NPC
static void showNpc(object user, int sysId, string missId)
{
	MISSION_DATA->delHideNpc(user, sysId);

	int npcId = Import("UNIQ_NPC")->systemId2NpcId(sysId);
	if( !npcId ) return;

	int uid = user->getId();
	int *npcPos = Import("SCENE")->getNpcLocation(npcId);
	int *userPos = user->getLocation();
	if( npcPos[0] != userPos[0] ) return;

	//debug_message("show npc:%d, %d, %s", sysId, npcId, missId);
	//TODO
	//"cmd/scene"->sendAddNpc(uid, npcId, 0);
}

// 尝试完成任务
int tryCompleteMission(object user, string missId)
{
        if( !hasMission(user, missId) ) return 0;

        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) {
                return 0;
        }

        int uid = user->getId();
        int ret = missObj->tryComplete(user, 0);
        if( !ret ) return 0;

        rpc_client_complete_mission(uid, missId);

	// 发奖
        int money = missObj->getMoney();
	if(money > 0) Import("UTIL")->addCash(user, money);

        int exp = missObj->getExp();
	if(exp > 0) {
		Import("UTIL")->addUserExp(user, exp);
		Import("UTIL")->addAllFightSummonExp(user, exp);
	}

        mapping items = missObj->getItems();
	if( mapp(items) ) {
		foreach(string type, int amount in items) {
			Import("ITEM_UTIL")->addNewItem(user, type, amount, ITEM_BAG_NORMAL);
		}
	}

	// 删除前置任务
        string preId = missObj->getPreId();
        if( stringp(preId) && sizeof(preId) ) {
                object preObj = getMissionObj(preId);
                if( objectp(preObj) ) {
                        preObj->delMission(user);
                } else {
                        debug_message(sprintf("del pre mission fail, can't find mission obj %s", preId));
                }
        }

	// 加入已完成任务列表
	MISSION_DATA->addCompletedMission(user, missId);

	// 处理NPC隐藏
	int *hideList = missObj->getHideNpcOnComplete();
	if( arrayp(hideList) ) {
		foreach(int sysId in hideList) {
			hideNpc(user, sysId, missId);
		}
	}

	// 处理NPC显示
	int *showList = missObj->getShowNpcOnComplete();
	if( arrayp(showList) ) {
		foreach(int sysId in showList) {
			showNpc(user, sysId, missId);
		}
	}

        // 分派下级任务
        string *nextIds = missObj->getNextIds();
	if( arrayp(nextIds) ) {
                foreach(string nextId in nextIds) {
			assignMission(user, nextId);
                }
        }

        return ret;
}

// 放弃任务
int cancelMission(object user, string missId)
{
        if( !hasMission(user, missId) ) return 0;

        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) {
                return 0;
        }

        int uid = user->getId();

        if( missObj->cancelMission(user) ) {
                rpc_client_cancel_mission(uid, missId);
		assignMission(user, missId);
                return 1;
        }
        return 0;
}

// 删除任务 删除前置任务、任务超时、回收任务等
int delMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) {
                return 0;
        }

        int uid = user->getId();

        if( missObj->delMission(user) ) {
                rpc_client_del_mission(uid, missId);
		//assignMission(user, missId);
                return 1;
        }
        return 0;
}

// 刷新任务
int renewMission(object user, string missId)
{
        object missObj = getMissionObj(missId);
        if( !objectp(missObj) ) {
                return 0;
        }

        class MissionInfo pto = createMissionPto(user, missId);
        debug_message("renewMission %O", pto);
        rpc_client_update_mission(user->getId(), pto);
}

// 已接(过程中)任务列表
string *getProgressMissions(object user)
{
        mapping missions = user->getMission();
        string *missList = ({});

        foreach( string missId, mapping missData in missions ) {
                if( isProgressMission(user, missId) ) {
                        missList += ({ missId });
                }
        }

        return missList;
}

// 可接任务列表
string *getCanAcceptMissions(object user)
{
        int uid = user->getId();
        mapping assignList = MISSION_DATA->getAssignList(user);
        string *missList = ({});

        // 检查玩家身上的分派任务能否接
        foreach(string missId, int _ in assignList) {
                if( canAcceptMission(user, missId) ) {
                        missList += ({ missId });
                }
        }

        return missList;
}

// 就绪任务列表
string *getReadyMissions(object user)
{
        mapping missions = user->getMission();
        string *missList = ({});

        foreach(string missId, mapping missData in missions) {
                if( isReadyMission(user, missId) ) {
                        missList += ({ missId });
                }
        }
        return missList;
}

/* 清除一些多余任务存盘数据，正常来说只会在内服才需要调用，内服的巫师指令会导致数据冗余
1.无效数据：如某任务已在任务表里删除，却还在人物存盘上
2.冗余数据：人物存盘上存着同一条任务线上的两个已完成任务
*/
void clearDirtyMission(object user)
{
        string *allMission = "module/mission/preload"->getData();
        mapping missions = user->getMission();

        // 删除无效数据
        foreach(string missId in keys(missions)) {
                if( member_array(missId, allMission) == -1 ) {
                        map_delete(missions, missId);
                }
        }

        // 删除冗余数据(只删除已完成的冗余数据)
        foreach(string missId in keys(missions)) {
                object missObj = getMissionObj(missId);
                string preId;

                while( objectp(missObj) && (preId = missObj->getPreId()) ) {
                        if( missions[preId] ) {
                                if( isCompletedMission(user, missId) && isCompletedMission(user, preId) ) {
                                        map_delete(missions, preId);
                                }
                        }

                        missObj = getMissionObj(preId);
                }
        }
}


// 所有已接、可接任务、可完成任务
static void syncAllMissions(object user)
{
	int uid = user->getId();
        string *missList = getProgressMissions(user) + getCanAcceptMissions(user) + getReadyMissions(user);
        debug_message(sprintf("syncAllMissions:%O", missList));

        for(int i = 0; i < sizeof(missList); i++) {
                string missId = missList[i];
                object missObj = getMissionObj(missId);
                class MissionInfo pto = createMissionPto(user, missId);
		rpc_client_update_mission(uid, pto);
        }
}

// 放弃一个任务
void rpc_server_cancel_mission(object user, string missId)
{
        if( cancelMission(user, missId) ) {
                rpc_client_cancel_mission(user->getId(), missId);
        }
}

static void dealNode(object user, string missId, mapping list)
{
	if( !hasCompletedMission(user, missId) ) return;

	object missObj = getMissionObj(missId);
	if( !objectp(missObj) ) return;

	int *hideList = missObj->getHideNpcOnComplete();
	if( arrayp(hideList) ) {
		foreach(int sysId in hideList) {
			if( undefinedp(list[sysId]) ) list[sysId] = missId;
		}
	}

	int *showList = missObj->getShowNpcOnComplete();
	if( arrayp(showList) ) {
		foreach(int sysId in showList) {
			if( !undefinedp(list[sysId]) ) map_delete(list, sysId);
		}
	}

	string *nextIds = missObj->getNextIds();
	if( arrayp(nextIds) ) {
		foreach(string nextId in nextIds) {
			dealNode(user, nextId, list);
		}
	}
}

static mapping buildHideNpcCache(object user)
{
	mapping list = ([]);
	dealNode(user, headNode, list);
	debug_message("hideNpcList:%O", list);
	return list;
}

/*static*/ mapping buildAssignCache(object user)
{
	mapping assignList = ([]);
	mapping completedList = MISSION_DATA->getCompletedList(user);

	foreach(string missId, int _ in completedList) {
		object missObj = getMissionObj(missId);
		if( !objectp(missObj) ) continue;

		string *nextIds = missObj->getNextIds();
		if( !arrayp(nextIds) ) continue;

		foreach(string nextId in nextIds) {
			if( !hasMission(user, nextId) && undefinedp(completedList[nextId]) ) {
				assignList[nextId] = 1;
			}
		}
	}

	debug_message("assignList:%O", assignList);
	return assignList;
}

// 建立已完成任务缓存
/*static*/ mapping buildCompletedCache(object user)
{
	mapping completedList = ([]);
        string *missions = keys(user->getMission());

        foreach(string missId in missions) {
                if( !isCompletedMission(user, missId) ) continue;

		object missObj;
		while(1) {
			if( !stringp(missId) || !sizeof(missId) ) break;

			missObj = getMissionObj(missId);
			if( !objectp(missObj) ) {
				logger->Warn("buildCompletedCache fail, can't find mission obj %s", missId);
				debug_message(sprintf("buildCompletedCache fail, can't find mission obj %s", missId));
				break;
			}

			// 已加入已完成列表，不需要往上遍历了
			if( !undefinedp(completedList[missId]) ) break;

			completedList[missId] = 1;
			missId = missObj->getPreId();
		}
	}

	debug_message("completedList:%O", completedList);
	return completedList;
}

// 首次登录
void firstLogin(object user)
{
        // 如果是新玩家，则给第一个任务
	assignMission(user, headNode);
        acceptMission(user, headNode);
}

void loginCheck(object user)
{
        if(0) clearDirtyMission(user);

	mapping completedList = buildCompletedCache(user);
	MISSION_DATA->setCompletedList(user, completedList);

        mapping assignList = buildAssignCache(user);
        MISSION_DATA->setAssignList(user, assignList);

	mapping hideNpcList = buildHideNpcCache(user);
	MISSION_DATA->setHideNpcList(user, hideNpcList);

        string *missList = getProgressMissions(user) + getCanAcceptMissions(user) + getReadyMissions(user);

        debug_message("loginCheck misslist:%O", missList);
        foreach(string missId in missList) {
                object missObj = getMissionObj(missId);
                if( objectp(missObj) ) {
                        missObj->loginCheck(user); 
                }
        }

        syncAllMissions(user);
}

void delAllMission(object user)
{
        mapping mission = user->getMission();
	foreach(string missId, mapping _ in mission) {
		delMission(user, missId);
	}
        clearDBVar(mission);

	mapping assignList = MISSION_DATA->getAssignList(user);
	foreach(string missId, mapping _ in assignList) {
		delMission(user, missId);
	}
	MISSION_DATA->setAssignList(user, ([]));
}

// 加载所有任务
static void preloadAllMissions()
{
        string *allMission = "module/mission/preload"->getData();

        foreach(string missId in allMission) {
                string file = sprintf("%s%s.c", MISSIONS_DIR, missId);
                object missObj = find_object(file, 1);
                xassert( missObj, missId );
        }
        
        debug_message("preloadAllMissions finish");
}

void init()
{
        preloadAllMissions();
}



void create()
{
        logger = Import("LOG")->New("mission");
        call_out("init", 2);
}

