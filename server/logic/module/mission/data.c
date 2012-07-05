/*
人物身上与任务相关的数据操作
mission = ([
        "missId": ([
                "base_data":([
                        "acceptTime":123123,
                        "completeTime": 123123123,
                ]),
                "ext_data":([]),
        ]),
])

*/

#include <user_key.h>
#include <debug.h>

#define MISSION_EXT_DATA "ext_data"
#define MISSION_BASE_DATA "base_data"

//任务裸数据
static mapping getPureMissionData(object user, string missId)
{
        mapping mission = user->getMission();
        return mission[missId];
}

//初始化任务裸数据
static mapping initPureMissionData(object user, string missId)
{
        mapping data = getPureMissionData(user, missId);

        if( undefinedp(data) ) {
                mapping mission = user->getMission();
                mission[missId] = ([]);
                data = mission[missId];
        }

        return data;
}

//清除任务裸数据
static void cleanPureMissionData(object user, string missId)
{
        mapping mission = user->getMission();
        if( !undefinedp(mission[missId]) ) map_delete(mission, missId);
}

mixed getMissionExtData(object user, string missId, mixed key)
{
        mapping data = getPureMissionData(user, missId);
        if( mapp(data) && data[MISSION_EXT_DATA] ) return data[MISSION_EXT_DATA][key];

        mixed undefine;
        return undefine;
}

int setMissionExtData(object user, string missId, mixed key, mixed value)
{
        mapping data = getPureMissionData(user, missId);
        if( mapp(data) ) {
                if( !data[MISSION_EXT_DATA] ) data[MISSION_EXT_DATA] = ([]);

                data[MISSION_EXT_DATA][key] = value;
                return 1;
        }

        return 0;
}

mixed getMissionBaseData(object user, string missId, mixed key)
{
        mapping data = getPureMissionData(user, missId);
        if( mapp(data) && data[MISSION_BASE_DATA] ) return data[MISSION_BASE_DATA][key];

        mixed undefine;
        return undefine;
}

int setMissionBaseData(object user, string missId, mixed key, mixed value)
{
        mapping data = getPureMissionData(user, missId);
        if( mapp(data) ) {
                if( !data[MISSION_BASE_DATA] ) data[MISSION_BASE_DATA] = ([]);

                data[MISSION_BASE_DATA][key] = value;
                return 1;
        }

        return 0;
}

void initMissionData(object user, string missId)
{
        initPureMissionData(user, missId);
}

void cleanMissionData(object user, string missId)
{
        cleanPureMissionData(user, missId);
}

// 分派任务数据

mapping getAssignList(object user)
{
        return user->getTemp(KT_USER_ASSIGN_MISSIONS);
}

void setAssignList(object user, mapping list)
{
        user->setTemp(KT_USER_ASSIGN_MISSIONS, list);
}

void addAssignMission(object user, string missId)
{
        mapping list = getAssignList(user);
	if( undefinedp(list[missId]) ) list[missId] = 1;
}

void delAssignMission(object user, string missId)
{
        mapping list = getAssignList(user);
	if( !undefinedp(list[missId]) ) map_delete(list, missId);
}

// end 分派任务数据

// 已完成任务数据

mapping getCompletedList(object user)
{
	return user->getTemp(KT_USER_COMPLETED_MISSIONS);
}

void setCompletedList(object user, mapping list)
{
	user->setTemp(KT_USER_COMPLETED_MISSIONS, list);
}

void addCompletedMission(object user, string missId)
{
	mapping list = getCompletedList(user);
	if( undefinedp(list[missId]) ) list[missId] = 1;
}

void delCompletedMission(object user, string missId)
{
	mapping list = getCompletedList(user);
	if( !undefinedp(list[missId]) ) map_delete(list, missId);
}

// end 已完成任务数据


// 隐藏某些NPC列表

mapping getHideNpcList(object user)
{
	return user->getTemp(KT_USER_HIDE_NPC);
}

void setHideNpcList(object user, mapping list)
{
	user->setTemp(KT_USER_HIDE_NPC, list);
}

void addHideNpc(object user, int sysId, string missId)
{
	mapping list = getHideNpcList(user);
	if( undefinedp(list[sysId]) ) list[sysId] = missId;
}

void delHideNpc(object user, int sysId)
{
	mapping list = getHideNpcList(user);
	if( !undefinedp(list[sysId]) ) map_delete(list, sysId);
}

// end 隐藏某些NPC列表
