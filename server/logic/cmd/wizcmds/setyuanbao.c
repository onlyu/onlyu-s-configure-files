#include <user_key.h>

mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "setyuanbao (0,1),1000 0,1代表绑定和非绑定",
		"Format" : "cmdid,int,int",
		"Alias" : ({"加钱拉",}),
	]);
}


mapping doCmd(int uid, int isBangding, int amount)
{
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		log_file("newitem", "nouser");
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}
	
	if (amount <= 0)
	{
		return ([ "IsSuccess" : 0, "Ret" : "钱要大于0,傻啊" ]); 
	}
	if (isBangding)
		user->setBangDingYB(amount);
	else
		user->setUnBangDingYB(amount);

	"char/user_cmd"->sendYuanbaoInfo(user);
	return  ([ "IsSuccess" : 0, "Ret" : "增加YB成功"]);

}
