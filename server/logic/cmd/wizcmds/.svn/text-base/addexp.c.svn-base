mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "addexp 1000",
		"Format" : "cmdid,int",
		"Alias" : ({"加经验",}),
	]);
}


mapping doCmd(int uid, int exp)
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
	user->addExp(exp);
	return  ([ "IsSuccess" : 0, "Ret" : "增加经验成功"]);
}
