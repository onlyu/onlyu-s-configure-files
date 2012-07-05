#ifndef __SJCG_DB__
#define __SJCG_DB__

#include <save.h>
//活动相关的全局key
#define GLOBAL_SECTION "global"
#define PHASE "phase"   //活动当前阶段
#define PHASE_START_TIME "phaseStartTime"   //活动当前阶段开始时间
#define RANK_LIST "rankList"

//玩家个人相关的数据
#define USER_SECTION "user"
#define LEVEL "level"
#define WARRIOR_INDEX "warriorIndex"
#define TOTAL_HURT "totalHurt"
#define ROUND "round"
#define RANK "rank"


#define FIRST_WARRIOR 1
#define LAST_WARRIOR 10

#define FIRST_LEVEL 1
#define FINAL_LEVEL 3
class RankData
{
	int uid;
	int hurtTotal;
}

DB_VAR(mpSanjieSave, ([]))
static string saveName = "sanjiecg";




void setLevel(int uid)
{
	mpSanjieSave[USER_SECTION][uid][LEVEL] += 1;
}

void getLevel(int uid)
{
	return mpSanjieSave[USER_SECTION][uid][LEVEL];
}

void initUserData(int uid)
{
	int size = sizeof(mpSanjieSave);
	mpSanjieSave[USER_SECTION][uid] = ([LEVEL:1,WARRIOR_INDEX:1,TOTAL_HURT:0, RANK:size + 1]);
	mpSanjieSave[GLOBAL_SECTION][RANK_LIST] += ({new (class RankData, uid:uid, hurtTotal:0)});
}

int addUserHurt(int uid, int newHurt)
{
	mpSanjieSave[USER_SECTION][uid][TOTAL_HURT] += newHurt;
	return mpSanjieSave[USER_SECTION][uid][TOTAL_HURT];
}

void setUserFightWarrior(int uid, int warrior)
{
	mpSanjieSave[USER_SECTION][uid][WARRIOR_INDEX] = warrior;
}

int getUserFightWarrior(int uid)
{
	return mpSanjieSave[USER_SECTION][uid][WARRIOR_INDEX];
}

mapping getUserData(int uid)
{
	return mpSanjieSave[USER_SECTION][uid];
}

int hasUser(int uid)
{
	return (!undefinedp(mpSanjieSave[USER_SECTION][uid]));
}

void initData()
{
	setSaveName(saveName);
	doRestore();
	if (undefinedp(mpSanjieSave[GLOBAL_SECTION]))
	{
		mpSanjieSave[GLOBAL_SECTION] = ([]);
	}

	if (undefinedp(mpSanjieSave[USER_SECTION]))
	{
		mpSanjieSave[USER_SECTION] = ([]);
	}

	if (undefinedp(mpSanjieSave[GLOBAL_SECTION][PHASE]))
	{
		mpSanjieSave[GLOBAL_SECTION][PHASE] = 0;
	}

	if (undefinedp(mpSanjieSave[GLOBAL_SECTION][PHASE_START_TIME]))
	{
		mpSanjieSave[GLOBAL_SECTION][PHASE_START_TIME] = 0;
	}

	if (undefinedp(mpSanjieSave[GLOBAL_SECTION][RANK_LIST]))
	{
		mpSanjieSave[GLOBAL_SECTION][RANK_LIST] = ({});
	}
}

void resortRankList()
{
}

#endif

