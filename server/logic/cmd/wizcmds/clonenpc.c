#include <npc_key.h>

mapping getConf()
{
	return ([
		"Level" : 4,
		"HelpMsg" : "clonenpc npc_path",
		"Alias" : ({}),
		"Format" : "cmdid,string",
	]);
}

mapping doCmd(int uid, string path)
{
	int npc = new_npc(path);
        SetNpc(npc, K_NPC_BIRTH, time());
	object user = get_user(uid);
	int *pos = user->getLocation();
	
	if ( npc < 1 ) 
		return (["IsSuccess" :0, "Ret" : "路径错误？" ]);

	Import("SCENE")->npcEnter(npc, pos[0], pos[1], pos[2]);
	return ([ "IsSuccess" : 1, "Ret" : ""]) ;
}	
