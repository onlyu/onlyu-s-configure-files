


mapping getConf()
{
	return ([
		"Level" : 99,
		"HelpMsg" : "call xxxx->foo()",
		"Alias" : ({}),
		"Format" : "cmdid,...",
		"explode": 1,
	]);
}

mapping call_func(int uid, string call_str)
{
	mixed *ArgList, mixResult;
	string *ParamList;
	string cObjName, cFunc, cParam;
	object oModule;
	string cRet = "";
	object oUser = get_user(uid);
	if (!call_str || !sizeof(call_str))
	{
		cRet = "用法错误: "+getConf()["HelpMsg"];
		return ([ "IsSuccess" : 0, "Ret" : cRet]) ;
	}

	if ( sscanf(call_str, "%s->%s(%s)", cObjName, cFunc, cParam) != 3 )
	{
		cRet = "用法错误: "+getConf()["HelpMsg"];
		return ([ "IsSuccess" : 0, "Ret" : cRet]) ;
	}

	debug_message(sprintf("%O, %O, %O", cObjName, cFunc, cParam));
	sscanf(cObjName, "\"%s\"", cObjName);
	// oUser->TellMe("参数中如果有array和mapping，请用，替换,");

	if (all_trim(cObjName) == "self") {
		oModule = oUser;
	}
	else
	{
		oModule = load_object(cObjName);
		if ( !objectp(oModule) )
		{
			cRet = sprintf("没有%s这个object.", cObjName);
			return ([ "IsSuccess" : 0, "Ret" : cRet]) ;
		}
	}	

	ParamList = explode(cParam, ",");
	ArgList   = ({ cFunc });
	foreach ( string cTmp in ParamList )
	{
		mixed _mTmp;
		if ( (strsrch(cTmp, "(:") != -1 && strsrch(cTmp, ":)") != -1)
			|| (strsrch(cTmp, "({") != -1 && strsrch(cTmp, "})") != -1)
			|| (strsrch(cTmp, "([") != -1 && strsrch(cTmp, "])") != -1) )
		{
			cTmp = replace_string(cTmp, "，", ",");
		}

		if (all_trim(cTmp) == "self") _mTmp = oUser;
		else if (cTmp[0] == '@') {   // 表示是玩家对象
			int tmpid = to_int(cTmp[1..sizeof(cTmp)]);
			if (intp(tmpid) && is_online(tmpid)) {
				_mTmp = get_user(tmpid);
			} else {
				_mTmp = atom(cTmp);
			}
		}
		else _mTmp = atom(cTmp);
		ArgList += ({ _mTmp });
	}

	mixResult = call_other(oModule, ArgList);
	cRet = sprintf("%s = %O\n", call_str, mixResult);

	return ([ "IsSuccess" : 1, "Ret" : cRet]) ;
}

mapping create_npc(int uid, string path) 
{
	int npc = Import("UTIL")->CreateNpc(path);
	object user = get_user(uid);
	int *pos = user->getLocation();
	Import("SCENE")->npcEnter(npc, pos[0], pos[1], pos[2]);

	return ([ "IsSuccess" : 1, "Ret" : ""]) ;
}

mapping doCmd(int uid, string call_str)
{
	switch (call_str) {
		case "tianzun":
			return create_npc(uid, "char/npc/mission_provider.c");
		case "xiubu":
			return create_npc(uid, "char/npc/fix_mission_npc");
		case "asan":
			return create_npc(uid, "char/npc/test_npc");
		case "zuzhiren":
			return create_npc(uid, "module/family_game/zuzhiren");
		case "jianduren":
			return create_npc(uid, "module/family_game/jianduren");
		case "yw_jianduren":
			return create_npc(uid, "module/yanwu/jianduren");
		case "home":
			return create_npc(uid, "module/home/manager_npc.c");
		case "org":
			return create_npc(uid, "module/org/test_npc.c");
		default:
			return call_func(uid, call_str);
			break;
	}
	return ([ "IsSuccess" : 0, "Ret" : ""]) ;
}
