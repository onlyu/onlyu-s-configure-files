mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "newsummon type",
		"Format" : "cmdid,int",
		"Alias" : ({"给新灵兽",}),
	]);
}

mapping doCmd(int uid, int summonType)
{
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		log_file("newSummon", "nouser");
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}
	int summonId = Import("SUMMON_UTIL")->addNewSummonByType(user, summonType);

	if (!summonId)
		return ([ "IsSuccess" : 0, "Ret" : "请输入正确的伙伴类型" ]); 
	

	return  ([ "IsSuccess" : 0, "Ret" : "添加灵兽成功"]);
}
