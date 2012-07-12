#include <user_key.h>

mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "addcash 1000",
		"Format" : "cmdid,int",
		"Alias" : ({"加钱拉",}),
	]);
}


mapping doCmd(int uid, int cash)
{
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		log_file("newitem", "nouser");
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}
	if (cash <= 0)
	{
		return ([ "IsSuccess" : 0, "Ret" : "钱要大于0,傻啊" ]); 
	}
	user->addCash(cash);
	return  ([ "IsSuccess" : 0, "Ret" : "增加金钱成功"]);
}
