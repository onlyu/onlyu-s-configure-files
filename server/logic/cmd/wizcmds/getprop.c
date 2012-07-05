mapping getConf()
{
	return ([
		"Level" : 3,
		"HelpMsg" : "getprop id,key",
		"Format" : "int,string",
		"Alias" : ({"取属性",}),
	]);
}

mapping doCmd(int uid, string cKey )
{
	int size;
	object oTmpUser;
	mixed value;

	if ( !isUserExist(uid) ) 
	{
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}

	oTmpUser = get_user(uid);

	if ( !objectp(oTmpUser)) 
	{
		return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
	}

	value = oTmpUser->getProp(cKey);

	return ([ "IsSuccess" : 1, "Ret" : sprintf("%O", value) ]); 
}
