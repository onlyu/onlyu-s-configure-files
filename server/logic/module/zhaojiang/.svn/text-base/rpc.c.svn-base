#include "/rc/rpc/zhaojiang.h"
#include <summon_key.h>


void rpc_server_zhaojiang_list(object user)
{
	class ZhaojiangFellow* fellowList =  Import("ZHAOJIANG")->getCanGetFellowList(user);
	//debug_message("zhaojiang list:%O,uid:%d", fellowList, user->getId());
	rpc_client_zhaojiang_list(user->getId(), fellowList);
}

void rpc_server_zhaojiang_get_fellow(object user, int type)
{
	Import("ZHAOJIANG")->beMyFellow(user, type);
}

void rpc_server_zhaojiang_detail(object user, int type)
{

	mapping summonBaseData = Import("SUMMON_UTIL")->getSummonBaseData(type);
	if (undefinedp(summonBaseData))
		return;
	class FellowDetail fellowDetail =  Import("ZHAOJIANG")->packFellowDetail(summonBaseData, type);
	//debug_message("summon detail:%O", fellowDetail);
	rpc_client_zhaojiang_detail(user->getId(), type, fellowDetail);
}
