
mapping getConf()
{
	return ([
		"Level" : 99,
		"HelpMsg" : "eval",
		"Format": "...",
	]);
}

mapping doCmd(string cContent)
{
	object oFile;
	string cFileName = "/tmp_eval_file.c";
	string cStr = "";
	string * Includes = ({});
	mixed mRes;
	mapping includeFile = ([]);

	Includes = get_dir("/include/*.h");
	//Includes = ({ "user_key.h", "user_attribute.h", "item_key.h", "npc_key.h", "summon_key.h", "macros.h", "team_macros.h", });

	if ( sizeof(Includes))
	{
		foreach ( string x in Includes )
		{
			// cache.h和save.h不能同时include
			// 所以这里不include save.h
			// 这样eval里面如果有DoRestore和DoSave将走cache.h的逻辑，
			// 直接生效
			if (x == "save.h") continue;
			// lru逻辑不再eval里面包含
			if (x == "lru.h") continue;
			cStr += sprintf("#include <%s>\n", x);	
		}
	}

        debug_message(cContent);
	write_file(cFileName, cStr + "mixed eval() { "+cContent+"; }\n", 1);

	if (find_object(cFileName)) 
		destruct(find_object(cFileName));

	oFile = "/rc/globals"->XLoad(cFileName);

	mRes = oFile->eval();

	return ([ "IsSuccess" :1, "Ret" : sprintf("%O",mRes) ]);
}

