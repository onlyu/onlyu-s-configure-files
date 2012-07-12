#include <user_key.h>
mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "setheroattr attr,value",
		"Format" : "cmdid,string,int",
		"Alias" : ({"加经验",}),
	]);
}


mapping doCmd(int uid, string attr, int value)
{
	
	string* validAttr = ({"hp", "attack", "defence", "qiangfa", "kangfa", "speed"});
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}

	int icon = user->getIcon();

	debug_message("user icon:%d", icon);
	if (value <= 0)
	{
		return ([ "IsSuccess" : 0, "Ret" : "值要大于0" ]); 
	}
	if (member_array(attr, validAttr)==-1)
		return ([ "IsSuccess" : 0, "Ret" : "无效的属性" ]); 

	user->setFightValue(attr, value);
	return  ([ "IsSuccess" : 0, "Ret" : "更改属性值成功"]);
}
