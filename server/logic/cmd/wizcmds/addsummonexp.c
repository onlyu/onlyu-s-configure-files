mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "addsummonexp type,exp",
		"Format" : "cmdid,int,int",
		"Alias" : ({"加经验",}),
	]);
}


mapping doCmd(int uid, int type, int exp)
{
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		log_file("newitem", "nouser");
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}
	if (exp <= 0)
	{
		return ([ "IsSuccess" : 0, "Ret" : "经验要大于0" ]); 
	}
	debug_message("cmd addsummonexp uid:%d type:%d exp:%d", uid, type, exp);
	int summonId = Import("SUMMON_UTIL")->getSummonIdByType(user, type);
	if (!summonId)
		return  ([ "IsSuccess" : 0, "Ret" : "不存在的类型"]);
	Import("SUMMON_UTIL")->addExp(summonId, exp);
	return  ([ "IsSuccess" : 0, "Ret" : "增加经验成功"]);
}
