#include "/rc/rpc/zhaocaimao.h"
#define PILIANG_ZHAOCAI_TIMES 10

void rpc_server_mcat_pad(object user)
{
	Import("ZHAOCAIMAO")->sendPadInfo(user);
}

void rpc_server_mcat_zhaocai(object user)
{
	//debug_message("zhaocai rpc id:%d", user->getId());
	Import("ZHAOCAIMAO")->zhaocai(user, 1);
}

void rpc_server_piliang_zhaocai(object user, int query, int no_more_hint)
{
	debug_message("zhaocai piliang id:%d query:%d hint:%d", user->getId(), query, no_more_hint);

	if (!query && no_more_hint)
	{
		Import("ZHAOCAIMAO")->setNoMoreHint(user);
	}

	int isGiveHint = Import("ZHAOCAIMAO")->giveHintOrNot(user);
	int leftTimes = Import("ZHAOCAIMAO")->getLeftTimes(user);
	int actualTimes = leftTimes < PILIANG_ZHAOCAI_TIMES ? leftTimes : PILIANG_ZHAOCAI_TIMES;
	if (actualTimes <= 0)
	{
		user->tellMe("您的招财次数已用完，充值VIP可增加新的次数");
		return;
	}
	if (!isGiveHint || !query )
	{
		Import("ZHAOCAIMAO")->zhaocai(user, actualTimes);
		return;
	}

	int yuanbao = Import("ZHAOCAIMAO")->getMultiTimeYuanbao(user, actualTimes);
	int tradeCash = Import("ZHAOCAIMAO")->getZhaocaiCash(user) * actualTimes;
	rpc_client_piliang_zhaocai_confirm(user->getId(), yuanbao, actualTimes, tradeCash);
	
}


















































