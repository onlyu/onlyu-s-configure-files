#include <macros.h>
#include <npc_key.h>
#include <summon_key.h>
//#include "/rc/rpc/scene.h"

/*
void sendDelChar(int uid, int charId)
{
	class msg_del_char_s del_msg = new(class msg_del_char_s, charid: charId);
	// debug_message(sprintf("[%d] del npc : %O", uid, del_msg));
	rpc_client_del_char(uid, del_msg);
}

void broadCastDelChar(int uid, int charId)
{
	class msg_del_char_s del_msg = new(class msg_del_char_s, charid: charId);
	fs_rpc_x_call(uid, T_RPC_X_USR_FIELD_NOT_FIGHT, RPC_CLIENT_DEL_CHAR, del_msg);
}


class msg_new_npc_s packNpcInfo(int npcid, int *pos, int addon)
{
	class msg_new_npc_s npc_info = new(class msg_new_npc_s);
	npc_info->id = npcid;
	npc_info->icon = GetNpc(npcid, K_NPC_ICON);
	npc_info->pos = 0;
	npc_info->pos |= pos[2];
	npc_info->pos |= pos[1] << 16;
	npc_info->name = GetNpc(npcid, K_NPC_NAME);
	string title = GetNpc(npcid, K_NPC_TITLE);
	npc_info->title = "";
	if (sizeof(title)) npc_info->title = title;
	npc_info->addon = addon;

	npc_info->dir = 0;
	int dir = GetNpc(npcid, K_NPC_DIR);
	if (dir) {
		npc_info->dir = dir;
	}
	npc_info->is_effect_npc = 0;
	return npc_info;
}

void sendAddNpc(int uid, int npcid, int addon)
{
	int *pos = get_npc_pos(npcid);
	class msg_new_npc_s npc_info = packNpcInfo(npcid, pos, addon);

	rpc_client_add_npc(uid, npc_info);
}

*/
