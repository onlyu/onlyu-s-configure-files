mapping getConf()
{
	return ([
		"Level" : 2,
		"HelpMsg" : "goto 1001,1,1|1,1",
		"Format" : "cmdid,int,*,*",
		"Alias" : ({"跳转场景",}),
	]);
}

mapping doCmd(int uid, int i1, int i2, int i3)
{
	int scene;
	int user = get_user(uid);

	i2 = to_int(i2);
	i3 = to_int(i3);
	if (i2 && !i3) {
		int *pos = user->getLocation();
		scene = pos[0];
		if (scene) {
			user->enterScene(scene, i1, i2);
		}
	} else if ( i3 ) {
		//player_enter_scene(uid, i1, i2, i3);
		user->enterScene(i1, i2, i3);
	}
	else if (i1) {
		int sid = i1;
		int npcid;

		/*
		// NPCID
		if (is_npc_online(sid)) {
			int *pos = get_npc_pos(sid);
			user->enterScene(uid, pos[0], pos[1], pos[2]);
			return ([ "IsSuccess" : 1, "Ret" : "成功" ]);
		}

		// NPCSID
		npcid = "module/uniq_npc_id.c"->systemId2NpcId(sid);
		if (npcid && is_npc_online(npcid)) {
			int *pos = get_npc_pos(npcid);
			player_enter_scene(uid, pos[0], pos[1], pos[2]);
			return ([ "IsSuccess" : 1, "Ret" : "成功" ]);
		}
		
		// USER
		if (is_online(sid)) {
			int *pos = get_user_pos(sid);
			player_enter_scene(uid, pos[0], pos[1], pos[2]);
			return ([ "IsSuccess" : 1, "Ret" : "成功" ]);
		}
		*/
	}
	else
	{
		return ([ "IsSuccess" : 0, "Ret" : "请输入参数" ]);
	}	
	return ([ "IsSuccess" : 1, "Ret" : "成功" ]);
}
