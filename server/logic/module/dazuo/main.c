#include <user_key.h>
#include "main.h"
#include "/rc/rpc/dazuo.h"


static mapping mpExpData;

int checkInDazuo(object user)
{
	if (getStartTime(user))
		return 1;
	return 0;
}

int getLastModiTime(object user)
{
	return user->getProp(K_USER_DAZUO_LASTMODI);
}

int getMaxTime(object user)
{
	//todo related to vip lvl
	return 150;
}

int getExp(object user)
{
	return mpExpData[user->getGrade()]["exp"];
}

int getTotalExp(object user)
{
	return user->getProp(K_USER_DAZUO_EXP);
}

void setTotalExp(object user, int exp)
{
	user->setProp(K_USER_DAZUO_EXP, exp);
}

void setStartTime(object user, int startTime)
{
	user->setProp(K_USER_DAZUO_START, startTime);
}

int getStartTime(object user)
{
	return user->getProp(K_USER_DAZUO_START);
}
	
int getLastModifiedTime(object user)
{
	return user->getProp(K_USER_DAZUO_LASTMODI);
}

int setLastModifiedTime(object user, int time)
{
	return user->setProp(K_USER_DAZUO_LASTMODI, time);
}

void onTime(int uid)
{
	object user = get_user(uid);
	if (!objectp(user))
		return;
	
	if (!getStartTime(user))
		return;


	int now = time();
	int totalTime = now - getStartTime(user);
	int remainingTime = getMaxTime(user) - totalTime;
	if (totalTime >= getMaxTime(user))
	{
		debug_message("onTime over %d", uid);
		leave(user);
		return;
	}

	int exp = getExp(user);
	user->addExp(exp);
	int totalExpBefore = getTotalExp(user);
	int totalExp = exp + totalExpBefore;
	setLastModifiedTime(user, now);
	setTotalExp(user, totalExp);
	rpc_client_dazuo_addexp(uid, exp);
	rpc_client_dazuo_info(user->getId(), remainingTime, totalExp); 
	debug_message("adzuo add exp uid:%d exp:%d totalExp:%d", uid, exp, totalExp);

	call_out((:onTime:), DAZUO_PERIOD, uid);
}


void start(object user)
{
	if (user->getGrade() < DAZUO_START_GRADE)
	{
		user->tellMe("%d级以后才能开始打坐", DAZUO_START_GRADE);
		return;
	}

	if (checkInDazuo(user))
		return;
	int startTime = time();
	int totalTime = getMaxTime(user);
	setStartTime(user, startTime);
	setLastModifiedTime(user, startTime);
	setTotalExp(user, 0);
	debug_message("dazuo info uid:%d remainingTime:%d", user->getId(), totalTime);
	rpc_client_dazuo_start(user->getId());
	rpc_client_dazuo_info(user->getId(),totalTime,0); 
	call_out((:onTime:), DAZUO_PERIOD, user->getId());
}

void clear(object user)
{
	setStartTime(user, 0);
	setTotalExp(user, 0);
	setLastModifiedTime(user, 0);
}

void leave(object user)
{
	clear(user);
	debug_message("dazuo leave uid:%d", user->getId());
	rpc_client_dazuo_leave(user->getId());
}


void loginCheck(object user)
{
	int lastModiTime = getLastModiTime(user);	
	int startTime = getStartTime(user);
	int isEnd = 1;
	if (!lastModiTime)
		return;

	int now = time();
	int totalTime = now - startTime;
	int maxEndTime = startTime + getMaxTime(user);
	int endTime = now > maxEndTime ? maxEndTime : now;
	if (maxEndTime > now)
		isEnd = 0;
	int actualTime = endTime - lastModiTime;
	int exp = (actualTime / DAZUO_PERIOD) * getExp(user);
	int totalExp = getTotalExp(user) + exp;
	if (!isEnd)
	{
		int remainingTime = maxEndTime - now;
		debug_message("dazuo info uid:%d remainingTime:%d", user->getId(), remainingTime);
		rpc_client_dazuo_info(user->getId(), remainingTime, totalExp); 
		debug_message("dazuo reenter uid:%d exp:%d", user->getId(), exp);
		rpc_client_dazuo_addexp(user->getId(), exp);
		call_out((:onTime:), DAZUO_PERIOD, user->getId());
		return;
	}
	
	clear(user);
	debug_message("adzuo add exp uid:%d exp:%d totalExp:%d", user->getId(), exp, totalExp);
	rpc_client_dazuo_addexp(user->getId(), exp);
}

void create()
{
	mpExpData = "module/dazuo/data"->get_data();
}





















































