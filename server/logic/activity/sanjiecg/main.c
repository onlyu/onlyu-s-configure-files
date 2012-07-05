#include "main.h"
#include "db.h"

static mapping mpLevelLastWarrior = ([1:3,2:6,3:10]);

void create()
{
	initTimer();
	initData();
}


void loginCheck(object user)
{
	int period = getPeriod();		
	switch(period)
	{
		case READY :
			sendSelfReady(user);
			break;
		case CAN_ENTER :
		case START :
			sendSelfCanStart(user);
			break;
		case END:
			break;
	}
}

void logoutCheck(object user)
{

}

void initTimer()
{
	int secondBeforeReady = getSecondBeforeReady();
	call_out("onTimerReady", secondBeforeReady);
}

void onTimerReady()
{
	//brdcstReady();
	call_out("onTimerCanEnter", 60);
}

void onTimerCanEnter()
{
	call_out("onTimerStart", 60);
}

void onTimerStart()
{
	call_out("onTimerEnd", 60);
}

void onTimerEnd()
{
	call_out("onTimerStart", 60);
}

void reqStart(object user)
{
	int period = getPeriod();		
	if (period != START && period != CAN_ENTER)
		return;
	if(userAlreadyIn(user))
		return;
	
	int beforeStartTime = getBeforeStartTime();
	if (beforeStartTime >= 0)
	{
		sendSelfWaitStart(user);
		call_out("start", beforeStartTime + 1, user->getId());
	}
	else
	{
		start(user->getId());
	}
}


static void start(int uid)
{
	object user = get_user(uid);
	if (!objectp(user))
		return;

	if(userAlreadyIn(user))
		return;
	int beforeStartTime = getBeforeStartTime();
	if (beforeStartTime >= 0)
	{
		call_out("start", beforeStartTime + 1, user->getId());
		return;
	}
	addUser(user);
	enterNextLevel(user, 1);
	sendSelfStart(user);

}

static void enterNextLevel(object user, int level)
{
	//get the secenId
	//enter Scene
	//add monster for the user
}

void addUser(object user)
{
	initUserData(user->getId());
}

int userAlreadyIn(object user)
{
	return hasUser(user->getId());
}

int getBeforeStartTime()
{
	return 0;
}

int getPeriod()
{
	return getPeriodByTime();
}

void onFightEnd(object user, int win, int totalHurt)
{
	if (win)	
	{
		onFightEndWin(user, totalHurt);
	}
	else
	{
		onFightEndLose(user, totalHurt);
	}
}

int isLevelLastWarrior(int warrior, int level)
{
	if (warrior <= 0)
		return 0;
	return mpLevelLastWarrior[level] == warrior;	
}

int isRoundLastWarrior(int warrior)
{
	return warrior == 10;
}

void onFightEndWin(object user, int totalHurt)
{
	addUserHurt(user->getId(), totalHurt);
	giveFightAward(user);
	int warrior = getUserFightWarrior(user->getId());
	int level = getLevel(user->getId());
	if (isLevelLastWarrior(warrior, level))
	{
		setUserFightWarrior(user->getId(), FIRST_WARRIOR);
		//increaseLevel(user->getId());
		if (level <= FINAL_LEVEL)
		{
			enterNextLevel(user, level);
		}
	}
	else
	{
		setUserFightWarrior(user->getId(), warrior + 1);
	}
	
}

void onFightEndLose(object user, int totalHurt)
{
	addUserHurt(user->getId(), totalHurt);
	giveFightAward(user);
}

void giveFightAward(object user)
{
}

void giveLevelAward(object user)
{
}
