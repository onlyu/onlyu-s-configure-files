#include <save.h>

DB_VAR(mpArenaSave, ([]))
static string saveName = "arena";
int addNewUser(object user);
int getUserRank(int uid);
int _lastRank = 0;


#define RANK_DATA "rankData"
#define WUHUN "wuhun"
#define ROBBED_NORMAL_WUHUNS "robbedNormalWuhuns"
#define ROBBED_MUTATION_WUHUNS "robbedMutationWuhuns"
#define USER_DATA "userData"

//key in userData
#define USER_ID "uid"
#define BEST_RANK "bestRank"
#define UNSTOP_WIN_TIMES "unstopWinTimes"
#define UNSTOP_WIN_RANKUP "unstopWinRankup"
#define WUHUN_LIST "wuhunList"
#define IS_ROBBED "isRobbed"
#define RANK "rank"
#define USERNAME "name"

//key in wuhun
#define CREATE_TIME "createTime"
#define FIRST_ROB_TIME "firstRobTime"
#define LAST_ROB_TIME "lastRobTime"
#define OWNER_ID "ownerId"
#define IS_MUTATION "isMutation"
#define MUTATION_TIME "mutationTime"

private mapping getUserData(int uid);
private mapping getWuhunData(int wuhunId);
private int getOwnWuhunId(int uid);
private mapping getWuhunData(int wuhunId)
{
	return mpArenaSave[WUHUN][wuhunId];
}

int setUserRobbed(int uid, int isRobbed)
{
	mpArenaSave[USER_DATA][uid][IS_ROBBED] = isRobbed;
}

void updateRobbedWuhun(int wuhunId, int ownerId)
{
	mapping wuhunData = getWuhunData(wuhunId);
	int now = time();
	if (!wuhunData[FIRST_ROB_TIME])
	{
		wuhunData[FIRST_ROB_TIME] = now;
	}
	wuhunData[LAST_ROB_TIME] = now;
	wuhunData[OWNER_ID] = ownerId;
}

void updateUnstopWin(int uid, int newRank, int upRank)
{
	mapping userData = getUserData(uid);	
	if (undefinedp(userData))
		return;
	userData[BEST_RANK] = newRank;
	userData[UNSTOP_WIN_TIMES] += 1;
	userData[UNSTOP_WIN_RANKUP] += upRank;
}

void stopUnstopWin(int uid)
{
	mapping userData = getUserData(uid);	
	if (undefinedp(userData))
		return;
	userData[UNSTOP_WIN_TIMES] = 0;
	userData[UNSTOP_WIN_RANKUP] = 0;
}

private int getOwnWuhunId(int uid)
{
	return uid;
}

private void initNewUserWuhun(object user)
{
	int uid= user->getId();
	mapping allWuhunData = mpArenaSave[WUHUN];
	int wuhun = getOwnWuhunId(uid);
	allWuhunData[wuhun] = ([]);
	mapping oneWuhunData = allWuhunData[wuhun];
	oneWuhunData[CREATE_TIME] = 0;
	oneWuhunData[FIRST_ROB_TIME] = 0;
	oneWuhunData[LAST_ROB_TIME] = 0;
	oneWuhunData[MUTATION_TIME] = 0;
	oneWuhunData[IS_MUTATION] = 0;
	oneWuhunData[OWNER_ID] = user->getId();

	mapping userData = mpArenaSave[USER_DATA];
	userData[WUHUN_LIST] += ({getOwnWuhunId(uid)});
}

private void init()
{
	if(undefinedp(mpArenaSave[RANK_DATA]))
		mpArenaSave[RANK_DATA] = ({});

	if(undefinedp(mpArenaSave[WUHUN]))
		mpArenaSave[WUHUN] = ([]);

	if(undefinedp(mpArenaSave[ROBBED_NORMAL_WUHUNS]))
		mpArenaSave[ROBBED_NORMAL_WUHUNS] = ([]);

	if(undefinedp(mpArenaSave[ROBBED_MUTATION_WUHUNS]))
		mpArenaSave[ROBBED_MUTATION_WUHUNS] = ([]);

	if(undefinedp(mpArenaSave[USER_DATA]))
		mpArenaSave[USER_DATA] = ({});

	_lastRank = sizeof(mpArenaSave[RANK_DATA]) + 1;
}

private mapping newUserData(object user, int initRank)
{
	mapping userData = ([]);
	userData[BEST_RANK] = initRank;
	userData[RANK] = initRank;
	userData[UNSTOP_WIN_TIMES] = 0;
	userData[UNSTOP_WIN_RANKUP] = 0;
	userData[IS_ROBBED] = 0;
	userData[WUHUN_LIST] = ({});
	userData[USERNAME] = user->getName();
	return userData;
}


private void initNewUserData(object user)
{
	int initRank = sizeof(mpArenaSave[RANK_DATA]) + 1;
	mpArenaSave[USER_DATA][user->getId()] = newUserData(user, initRank);
}

private void create()
{
	setSaveName(saveName);
	doRestore();
	init();
}

int isOldUser(int uid)
{
	if (member_array(uid, mpArenaSave[RANK_DATA]) == -1)
		return 0;
	return 1;
}

int addNewUser(object user)
{
	if (isOldUser(user->getId()))
		return 0;

	mpArenaSave[RANK_DATA] += ({user->getId()});
	initNewUserData(user);
	initNewUserWuhun(user);
}

int getUserRank(int uid)
{
	return mpArenaSave[USER_DATA][uid][RANK];
}

int* getUserWuhunList(int uid)
{
	return mpArenaSave[USER_DATA][uid][WUHUN_LIST];
}

private void setUserRank(int uid, int rank)
{
	mpArenaSave[USER_DATA][uid][RANK] = rank;
}

private mapping getUserData(int uid)
{
	return mpArenaSave[USER_DATA][uid];
}

int robLoserWuhun(int winnerId, int loserId)
{
	int* loserWuhunList = getUserWuhunList(loserId);	
	if (!loserWuhunList || sizeof(loserWuhunList) <= 0)
		return 0;
	int* winnerWuhunList = getUserWuhunList(winnerId);
	if (!winnerWuhunList)
		return 0;

	int loserSize = sizeof(loserWuhunList);
	int winnerSize = sizeof(winnerWuhunList);
	int robbedWuhun = 0;
	if (winnerSize)
	{
		robbedWuhun = loserWuhunList[loserSize - 1];
	}
	else
	{
		int winnerWuhunId = getOwnWuhunId(winnerId);
		if (member_array(winnerWuhunId, loserWuhunList) != -1)
		{
			robbedWuhun = winnerWuhunId;
		}
	}
	if (!robbedWuhun)
		loserWuhunList = loserWuhunList - ({robbedWuhun});
	return robbedWuhun;
}

void replaceLoserRank(object winner, int loserId)
{
	int winnerId = winner->getId();
	int loserOldRank = getUserRank(loserId);
	int winnerOldRank = getUserRank(winnerId);
	if (loserOldRank < winnerOldRank)
		return;

	mapping loserData = getUserData(loserId);
	mapping winnerData = getUserData(winnerId);

	//update winner rank and related info
}

int* getRankList()
{
	return mpArenaSave[RANK_DATA];
}

string getUserName(int uid)
{
	return mpArenaSave[USER_DATA][uid][USERNAME];
}
