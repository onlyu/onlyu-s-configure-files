


mapping getConf()
{
	return ([
		"Level" : 0,
		"HelpMsg" : "bianshen 10002",
		"Alias" : ({}),
		"Format" : "cmdid,int",
	]);
}

mapping doCmd(int uid, int layout)
{
	object user = get_user(uid);
	if( !objectp(user) ) {
		return ([ "IsSuccess" : 0, "Ret" : ""]) ;
	}

	Import("SCENE")->changeLayout(user, layout);
	return ([ "IsSuccess" : 1, "Ret" : "done"]) ;
}
