#include <user_key.h>
mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "setsummonattr type,attr,value",
		"Format" : "cmdid,int,string,int",
		"Alias" : ({"加经验",}),
	]);
}


mapping doCmd(int uid, int type, string attr, int value)
{
	
	string* validAttr = ({"hp", "attack", "defence", "qiangfa", "kangfa", "speed"});
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}
	int summonId = Import("SUMMON_UTIL")->getSummonIdByType(user, type);
	if (!summonId)
		return  ([ "IsSuccess" : 0, "Ret" : "不存在的类型"]);

	if (value <= 0)
	{
		return ([ "IsSuccess" : 0, "Ret" : "值要大于0" ]); 
	}
	if (member_array(attr, validAttr)==-1)
		return ([ "IsSuccess" : 0, "Ret" : "无效的属性" ]); 

	Import("SUMMON_UTIL")->setFightValue(summonId, attr, value);
	Import("SUMMON_UTIL")->rpcSummonInfo(user->getId(), summonId);
	return  ([ "IsSuccess" : 0, "Ret" : "更改属性值成功"]);
}
