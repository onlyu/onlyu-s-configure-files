#ifndef _rc_rpc_scene
#define _rc_rpc_scene

#include "/rc/rpc/rpc_id.h"


class msg_new_player_s {
	int id;
	int icon;
	int pos;
	/*
	int x;
	int y;
	*/
	int addon;
	int timestamp;
	string name;
	string title;
}

/*
class msg_del_player_s {
	int delid;
}
*/

class msg_player_move_path_s {
	string path;
	int timestamp;
}

class msg_player_move_s {
	int id;
	int cur_pos;
	int mov_pos;
	/*
	int cur_x;
	int cur_y;
	int mov_x;
	int mov_y;                                                                                                                                                                                      
	*/
}

class msg_scene_prop_s {
	int weather;
	int isnight;
	int synctime;    //走路同步时间(1s)
	int warfog;
}

class msg_scene_object_show_s {
	int ob;
	int is_show;
}

class msg_enter_scene_s {
	int scene;
	int x;
	int y;
}

class msg_goto_s {
	int x;
	int y;
	int timestamp;
}

class msg_new_npc_s {
	int id;
	int icon;
	int pos;
	/*
	int x;
	int y;
	*/
	string name;
	string title;
	int addon;
	int dir;
	int is_effect_npc;      /*机关类:1, 默认:0*/
}

/*
class msg_del_npc_s {
	int delid;
}
*/

class msg_click_npc_s {
	int npcnum;
}

#define ST_CHAR_NULL         0 
#define ST_CHAR_WALK         1
#define ST_CHAR_DIE          2
#define ST_CHAR_RUSH         3
#define ST_CHAR_DEFEND       4
#define ST_CHAR_GUARD        5
#define ST_CHAR_RUN          6
#define ST_CHAR_MAGIC        7
#define ST_CHAR_STAND        8
#define ST_CHAR_STAND2		 9
#define ST_CHAR_HIT         10
#define ST_CHAR_HIT2        11


#define SHOW_IN_WARFOG       0            /*可见开灯*/
#define HIDE_IN_WARFOG       1            /*不显示*/
#define OPEN_WARFOG          2            /*显示不开灯*/

// 人物,NPC状态添加
class msg_char_status_s {
	int id;

	// 状态
	int status;
	int status_sec;
	int status_tar;
}

class msg_char_warfog_prop_s {
	int id;
	int fog_affect;        /*0(右方):可见开灯, 1(敌方):不显示, 2(中立):显示不开灯*/
}

class msg_char_effect_s {
	int id;
	int effect_id;     /*特效ID, 添加特效时生成*/
	int res_id;        /*特效资源ID*/
	int last_time;
	int play_times;
}

class msg_del_effect_s {
	int id;
	int effect_id;
}

// 注意，以下两个结构被rc/rpc/summon_decl.c引用，修改需要注意
class msg_new_summon_s {
	int id;
	int icon;
	string name;
	int follow_owner;
}

/*
class msg_del_summon_s {
	int delid;
}
*/

// 注意，以下两个结构被rc/rpc/team_decl.c引用，修改需要注意
class msg_member_s {
	int uid;
}

class msg_team_s {
	int leader;
	class msg_member_s *members;
	int is_full;
}

class msg_scene_id_s {
	int scene_id;
}

class msg_name_title_color_s {
	int char_id;
	int name_color;
	int title_color;
}


// 头顶图标
class msg_head_addon_s {
	int char_id;
	int addon;
}
// 阵营
class msg_char_faction_s{
	int uid; // 玩家id
	int faction; // 阵营编号
}
// 飞行
class msg_del_chars_s {
	int *charids;
}

class msg_del_char_s {
	int charid;
}

void rpc_client_goto(int uid, class msg_goto_s goto_msg) { fs_rpc_call(uid, RPC_CLIENT_GOTO, goto_msg); }
void rpc_server_move(int uid, class msg_player_move_path_s path);

void rpc_client_move(int uid, class msg_player_move_s move_msg) { fs_rpc_call(uid, RPC_CLIENT_MOVE, move_msg); }
void rpc_client_walk(int uid, class msg_player_move_s move_msg) { fs_rpc_call(uid, RPC_CLIENT_WALK, move_msg); }
void rpc_client_add_player(int uid, class msg_new_player_s new_msg) { fs_rpc_call(uid, RPC_CLIENT_ADD_PLAYER, new_msg); }
// void rpc_client_del_player(int uid, class msg_del_player_s del_msg) { fs_rpc_call(uid, RPC_CLIENT_DEL_PLAYER, del_msg); }

void rpc_client_enter_scene(int uid, class msg_enter_scene_s enter_msg) { fs_rpc_call(uid, RPC_CLIENT_ENTER_SCENE, enter_msg); }
void rpc_client_scene_prop(int uid, class msg_scene_prop_s scene_prop) { fs_rpc_call(uid, RPC_CLIENT_SCENE_PROP, scene_prop); }
void rpc_client_scene_object_show(int uid, class msg_scene_object_show_s scene_ob) { fs_rpc_call(uid, RPC_CLIENT_SCENE_OBJECT_SHOW, scene_ob); }

void rpc_client_add_npc(int uid, class msg_new_npc_s new_msg) { fs_rpc_call(uid, RPC_CLIENT_ADD_NPC, new_msg); }
// void rpc_client_del_npc(int uid, class msg_del_npc_s del_msg) { fs_rpc_call(uid, RPC_CLIENT_DEL_NPC, del_msg); }


void rpc_client_add_summon(int uid, class msg_new_summon_s new_msg) { fs_rpc_call(uid, RPC_CLIENT_ADD_SUMMON, new_msg); }
// void rpc_client_del_summon(int uid, class msg_del_summon_s del_msg) { fs_rpc_call(uid, RPC_CLIENT_DEL_SUMMON, del_msg); }

void rpc_client_char_status(int uid, class msg_char_status_s prop_msg) { fs_rpc_call(uid, RPC_CLIENT_CHAR_STATUS, prop_msg); }

void rpc_server_click_npc(int uid, class msg_click_npc_s click_info);
void rpc_server_move_test(int uid, class msg_player_move_s move_msg);

// 队伍广播协议
/*广播队伍进入视野*/
void rpc_client_team(int uid, class msg_team_s team_info) { fs_rpc_call(uid, RPC_CLIENT_TEAM, team_info); }
void rpc_client_char_add_effect(int uid, class msg_char_effect_s effect_info)  { fs_rpc_call(uid, RPC_CLIENT_CHAR_ADD_EFFECT, effect_info); }
void rpc_client_char_del_effect(int uid, class msg_del_effect_s del_info) { fs_rpc_call(uid, RPC_CLIENT_CHAR_DEL_EFFECT, del_info); }
void rpc_client_char_warfog_prop(int uid, class msg_char_warfog_prop_s prop_msg) { fs_rpc_call(uid, RPC_CLIENT_CHAR_WARFOG_PROP, prop_msg); }
void rpc_client_reset_warfog(int uid, class msg_scene_id_s scene) { fs_rpc_call(uid, RPC_CLIENT_RESET_WARFOG, scene); }
// 阵营，处于敌对阵营的玩家和npc表现是不一样的, 帮战等玩法中使用
void rpc_client_char_faction(int uid, class msg_char_faction_s faction_msg) { fs_rpc_call(uid, RPC_CLIENT_CHAR_FACTION, faction_msg); }

void rpc_client_char_name_title_color(int uid, class msg_name_title_color_s color_msg) { fs_rpc_call(uid, RPC_CLIENT_CHAR_NAME_TITLE_COLOR, color_msg); }

void rpc_client_char_set_head_addon(int uid, class msg_head_addon_s addon_msg) { fs_rpc_call(uid, RPC_CLIENT_CHAR_SET_HEAD_ADDON, addon_msg); }


class msg_follow_info_s {
	int player_id;
	int follow_id;
}

void rpc_client_char_follow(int uid, class msg_follow_info_s follow_info) { fs_rpc_call(uid, RPC_CLIENT_CHAR_FOLLOW, follow_info); }

void rpc_client_del_chars(int uid, class msg_del_chars_s del_msg) { fs_rpc_call(uid, RPC_CLIENT_DEL_CHARS, del_msg); }
void rpc_client_del_char(int uid, class msg_del_char_s del_msg) { fs_rpc_call(uid, RPC_CLIENT_DEL_CHAR, del_msg); }

class msg_dir_info_s {
	int charid;
	int dir;
}
void rpc_client_char_dir(int uid, class msg_dir_info_s dir_info) { fs_rpc_call(uid, RPC_CLIENT_CHAR_DIR, dir_info); }

class msg_parts_equip_prop_s {
	int charid;
	int parts_weapon;
}

void rpc_client_char_parts_equip_prop(int uid, class msg_parts_equip_prop_s equip_info) { fs_rpc_call(uid, RPC_CLIENT_CHAR_PARTS_EQUIP_PROP, equip_info); }
// void rpc_client_char_parts_color_prop(int uid, int charid, int color) { fs_rpc_call(uid, RPC_CLIENT_CHAR_PARTS_COLOR_PROP, charid, color); }
#endif
