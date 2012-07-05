#include <user_key.h>
#include "/module/zhaocaimao/main.h"
#include "/rc/rpc/zhaocaimao.h"



private int getMaxTimes(object user)
{
	//todo 根据vip等级来取最大次数
	return 200;
}

private int getUsedTimes(object user)
{
	return user->getZhaocaiTimes();
}


int getLeftTimes(object user)
{
	return getMaxTimes(user) - getUsedTimes(user);
}

int getMultiTimeYuanbao(object user, int times)
{
	if (times <= 0)
		return 0;
	int usedTimes = getUsedTimes(user);
	int startTime = usedTimes + 1;
	int endTime = times + usedTimes;
	int maxTimes = getMaxTimes(user);
	endTime = endTime > maxTimes ? maxTimes : endTime;
	int yuanbao = 0;
	mapping yuanbaoData = "module/zhaocaimao/yuanbao"->get_data();
	while(1)
	{
		if (startTime > endTime)
			break;
		yuanbao += yuanbaoData[startTime]["yuanbao"];	
		startTime++;
	}
	return yuanbao;
}

int getOneTimeYuanbao(object user)
{
	return getMultiTimeYuanbao(user, 1);	
}


int getZhaocaiCash(object user)
{
	mapping zhaocaiMoneyData = "module/zhaocaimao/grade_money"->get_data();
	return zhaocaiMoneyData[user->getGrade()]["cash"];
}

void zhaocai(object user, int times)
{
	if(times <= 0)
		return;
	int leftTimes = getLeftTimes(user);
	if (leftTimes <= 0)
	{
		user->tellMe("您的招财次数已用完，充值VIP可增加新的次数");
		rpc_client_mcat_finish(user->getId(), 0);
		return;
	}
	
	int actualTimes = times < leftTimes ? times : leftTimes;
	int yuanbaoNeeded = getMultiTimeYuanbao(user, actualTimes);
	int yuanbaoHas = user->getTotalYB();	
	if (yuanbaoHas < yuanbaoNeeded)
	{
		user->tellMe("元宝不足");
		rpc_client_mcat_finish(user->getId(), 0);
		return;
	}
	int cashBefore = user->getCash();
	user->minusYB(yuanbaoNeeded);
	int cashAdded = times * getZhaocaiCash(user);
	user->addCash(cashAdded);
	user->addZhaocaiTimes(actualTimes);
	rpc_client_mcat_finish(user->getId(), 1);
	sendPadInfo(user);
	return;
}

int giveHintOrNot(object user)
{
	int ret = user->giveZhaocaiHintOrNot();
	//debug_message("givehint %d", ret);
	return ret;
}

void setNoMoreHint(object user)
{
	debug_message("setnomorehint %d", 1);
	user->setZhaocaiNoMoreHint();
}

void loginCheck(object user)
{
}

void logoutCheck(object user)
{
}


void sendPadInfo(object user)
{
	int yuanbao = getOneTimeYuanbao(user);
	int leftTimes = getLeftTimes(user);
	int tradeCash = getZhaocaiCash(user);
	debug_message("pad info userid:%d yuanbao:%d tradeCash:%d leftTimes:%d", user->getId(), yuanbao, tradeCash, leftTimes);
	rpc_client_mcat_pad(user->getId(), yuanbao, tradeCash, leftTimes);
}

