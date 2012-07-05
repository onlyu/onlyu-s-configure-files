#ifndef _rc_rpc_user_cmd
#define _rc_rpc_user_cmd

#include "/rc/rpc/rpc_id.h"


// 玩家信息
class HeroProfile {
	int uid;
	int icon;
	string name;
	int cash;
	int tili;
	string title;
}

class HeroSkillInfo 
{
	string name;
	string desc;
	int xibie;
}
class HeroSpecInfo {
	int exp; 
	int maxExp;
	int hp;
	int grade;
	int maxHP;
	int attack;
	int qiangfa;
	int kangfa;
	int speed;
	int defence;

	int liliang;
	int zhili;
	int minjie;
	int naili;
	int zhanli;
	class HeroSkillInfo* zdSkillList;
	class HeroSkillInfo bdSkill;
}

class HeroGradeUpInfo{
	int hp;
	int maxHP;
	int grade;
}

class HeroAttrKV
{
	string key;
	int value;
}
// 玩家信息
void rpc_client_hero_gradeup(int uid, class HeroGradeUpInfo info) { fs_rpc_call(uid, RPC_CLIENT_HERO_GRADEUP, info); }
void rpc_client_hero_profile(int uid, class HeroProfile profile) { fs_rpc_call(uid, RPC_CLIENT_HERO_PROFILE, profile); }
void rpc_server_hero_specinfo(object user);
void rpc_client_hero_specinfo(int uid, class HeroSpecInfo specinfo) { fs_rpc_call(uid, RPC_CLIENT_HERO_SPECINFO, specinfo); }

//属性更新
void rpc_client_hero_attr_update(int uid, class HeroAttrKV* attrKVList) { fs_rpc_call(uid, RPC_CLIENT_HERO_ATTR_UPDATE, attrKVList); }

#endif
