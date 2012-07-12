#ifndef __SJCG_MAIN__
#define __SJCG_MAIN__
#include "time.h"

#define MINGLEI_NAME "mname"
#define SCENE_NAME "sname"

class SJFightResult
{
	int hurtGWHp;
	int win;
}


void initTimer();

void brdcstCanStart();

void brdcstReady();

void brdcstEnd();

static void start(int uid);

void brdcstRankList();

void sendRankList(object user);

void sendSelfRank(object user);

void sendSelfReady(object user);

void sendSelfWaitStart(object user);

void sendSelfStart(object user);

void sendSelfCanStart(object user);

int* getAllUserList();

void addUser(object user);

void onFightEnd(object user, int win, int totalHurt);

void onFightEndWin(object user,  int totalHurt);

void onFightEndLose(object user,  int totalHurt);

void startFight(object user);

void reqStart(object user);

void resortRankList();

void updateUserHurt(object user, int newFightHurt);

void onTimerReady();

void onTimerCanEnter();

void onTimerStart();

void onTimerEnd();

void giveEndReward(object user);

void giveAllEndReward();

int getPeriod();

int userAlreadyIn(object user);

int getBeforeStartTime();

void enterNextLevel(object user, int round);

void giveFightAward(object user);

void giveLevelAward(object user);
#endif 
