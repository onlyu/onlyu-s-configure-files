
#include "/rc/rpc/user_cmd.h"
#include <user_key.h>
#include <macros.h>

static object logger;

void sendAttrUpdate(object user, string attr, int value)
{
	class HeroAttrKV* attrList = ({new(class HeroAttrKV, key:attr, value:value)});
	rpc_client_hero_attr_update(user->getId(), attrList);
}

	
void new_hero(object user)
{
	class HeroProfile heroProfile = new (class HeroProfile);
	heroProfile->uid = user->getId();
	heroProfile->icon = user->getProp(K_USER_ICON);
	heroProfile->name = user->getName();
	heroProfile->cash = user->getCash();
	heroProfile->tili = user->getTili();
	heroProfile->title = user->getTitle();
	rpc_client_hero_profile(user->getId(), heroProfile);
}



void sendCashInfo(object user)
{
	class HeroAttrKV* attrList = ({new(class HeroAttrKV, key:K_USER_CASH, value:user->getCash())});
	rpc_client_hero_attr_update(user->getId(), attrList);
	
}

void sendYuanbaoInfo(object user)
{
	class HeroAttrKV* attrList = ({new(class HeroAttrKV, key:KT_USER_YUANBAO, value:user->getTotalYB())});
	debug_message("send yb uid:%d info:%O", user->getId(), attrList);
	rpc_client_hero_attr_update(user->getId(), attrList);
	
}

void sendExpInfo(object user)
{
	class HeroAttrKV* attrList = ({new(class HeroAttrKV, key:K_USER_EXP, value:user->getExp())});
	rpc_client_hero_attr_update(user->getId(), attrList);
}

class HeroSkillInfo* packZDSkillInfoList(object user)
{ 
	return ({new (class HeroSkillInfo, name:"战击", desc:"近身物理攻击，150%伤害", xibie:1)});
	string* skillIdList = user->getSkillIdList();
	foreach (string skillId in skillIdList)
	{
	}
}

class HeroSkillInfo packBDSkillInfo(object user)
{
	return new (class HeroSkillInfo, name:"天命", desc:"增加5%爆击和5%破击", xibie:1);
}

void rpc_server_hero_specinfo(object user)
{
	class HeroSpecInfo heroSpecInfo = new (class HeroSpecInfo);
	heroSpecInfo->exp = user->getExp();
	heroSpecInfo->hp = user->getHP();
	heroSpecInfo->maxHP = user->getMaxHP();
	heroSpecInfo->maxExp = user->getMaxExp(user->getGrade());
	heroSpecInfo->grade = user->getGrade();
	heroSpecInfo->attack = user->getAttack();
	heroSpecInfo->qiangfa = user->getQiangfa();
	heroSpecInfo->kangfa = user->getKangfa();
	heroSpecInfo->speed = user->getSpeed();
	heroSpecInfo->defence = user->getDefence();

	heroSpecInfo->liliang = user->getLiliang();
	heroSpecInfo->naili = user->getNaili();
	heroSpecInfo->zhili = user->getZhili();
	heroSpecInfo->minjie = user->getMinjie();
	heroSpecInfo->zhanli = 250;
	heroSpecInfo->bdSkill = packBDSkillInfo(user);
	heroSpecInfo->zdSkillList = packZDSkillInfoList(user);
	rpc_client_hero_specinfo(user->getId(), heroSpecInfo);
}


void logoutCheck(object user)
{
	Import("FRIEND")->logoutCheck(user);
	Import("ZHAOCAIMAO")->logoutCheck(user);
	Import("FUBEN_SYS")->logoutCheck(user);
}

void loginCheck(object user)
{
}

void create()
{
	logger = Import("LOG")->New("user_cmd");
}


