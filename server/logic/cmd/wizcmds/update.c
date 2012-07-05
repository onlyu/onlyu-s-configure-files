
mapping getConf()
{
	return ([
		"Level" : 99,
		"HelpMsg" : "update files",
		"Format" : "string",
		"Alias" : ({"reload",}),
	]);
}

mapping doCmd(string sUpdateFile) 
{
	/*
	if ( !find_object(sUpdateFile) )
		return ([ "IsSuccess" : 0, "Ret" : "file not exist." ]);
	*/

	if ("/module/util/update"->DoUpdate(sUpdateFile)) 
	{
		return ([ "IsSuccess" : 1, "Ret" : sprintf(" %s成功!", sUpdateFile) ]);
	} 
	else
		return ([ "IsSuccess" : 0, "Ret" : sprintf(" %s error", sUpdateFile) ]);
}
