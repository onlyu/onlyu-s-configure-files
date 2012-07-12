mapping getConf()
{
	return ([
		"Level" : 1,
		"HelpMsg" : "giveitem id,ctype,count",
		"Format" : "int,string,int",
		"Alias" : ({"ni",}),
	]);
}

mapping doCmd(int uid, string ctype, int count)
{
	log_file("newitem", "uid=%d ctype=%s count=%d", uid, ctype, count);
	object user = get_user(uid);
	if (!objectp(user)) 
	{
		log_file("newitem", "nouser");
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}
	//give item
	int itemId = Import("ITEM_UTIL")->addNewItem(user, ctype, count);
	if (!itemId)
	{
		log_file("newitem", "give fail");
		return ([ "IsSuccess" : 0, "Ret" : "生成物品失败,请检查类型和数量" ]); 
	}
	return  ([ "IsSuccess" : 0, "Ret" : "生成物品成功"]);
}
