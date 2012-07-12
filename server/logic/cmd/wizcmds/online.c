mapping getConf()
{
	return ([
		"Level" : 4,
		"HelpMsg" : "online cnt",
		"Format" : "string",
		"Alias" : ({"ol",}),
	]);
}

mapping doCmd(string cAction)
{
	switch( cAction )
	{
		case "cnt" :
			return ([ "IsSuccess" : 1, "Ret" : sprintf("%d",get_online_count()) ]) ;
                /*
		case "dump" :
			return ([ "IsSuccess" : 1, "Ret" : "tdd/dump_info"->dump_all() ]) ;
                */
	}

	return ([ "IsSuccess" : 0, "Ret" : "尚未实现" ]) ;
}
