#include <debug.h>
#include <frames.h>
#include <summon_key.h>
#include <item_key.h>
#include <user_key.h>
#include <summon.h>
#include <macros.h>
#include "/rc/rpc/summon.h"
#include "/module/fight/fight.h"

static object logger;
static mapping mpSummonProp;  

void rpcSummonInfo(int uid, int summonId);
class Summon summonInfoPack(int summonId);
int getSummonFightValue(int summonId, mixed key);
int getSummonBaseFightValue(int summonId, mixed key);
mixed getSummonBaseAttr(int type, mixed key);
int addNewSummonByType(object user, int type);
int createSummon(int type);
int restoreSummon(mapping mpSave);
int calculateSummon(int summonId);
int getGrade(int summonId);
void addExp(int summonId, int addedExp);
int getExp(int summonId);
private int upGrades(object user, int summonId);
private void onUpOneGrade(object user, int summonId);
int checkUpGrade(int summonId);
int getSummonIdByType(object user, int type);

varargs int addNewSummonById(object user, int summonId, int bagId);

mixed getSummonBaseAttr(int type, mixed key)
{
	return mpSummonProp[type][key];
}

string getName(int summonId)
{
	return GetSummonSave(summonId, K_SUMMON_NAME);
}

int* getAllFightSummon(object user)
{
	return user->getAllFightSummon();
}

int* getAllSummon()
{
	return keys(mpSummonProp);
}
int isValidSummon(int type)
{
	if (undefinedp(mpSummonProp[type]))
		return 0;
	return 1;
}

mapping getSummonBaseData(int type)
{
	return mpSummonProp[type];
}

int getZhaojiangGrade(int type)
{
	if (!isValidSummon(type))
		return 0;
	return mpSummonProp[type][K_SUMMON_ZHAOJIANG_GRADE];
}

int hasSummonByType(object user, int type)
{
	return user->hasSummonByType(type);
}

int restoreSummon(mapping mpSave)
{
	int type = mpSave[K_SUMMON_TYPE];
	int summonId;


	summonId = restore_summon(SUMMON_PATH, mpSave);

	if ( summonId < 0) 
	{
		//debug_message( sprintf("ERROR:Restore Summon失败！%O", mpSave) );
		return summonId;
	}
	
	SetSummonTemp(summonId, KT_SUMMON_RESOURCE, type);
	calculateSummon(summonId);

	return summonId;
}
/*
*return 0失败 大于0成功 值为summonId
*/
int addNewSummonByType(object user, int type)
{
	if (user->hasSummonByType(type))
	{
		//debug_message("添加灵兽error已有类型");
		return 0;	
	}
	int summonId = createSummon(type);
	if (!summonId)
	{
		//debug_message("添加灵兽error创建灵兽");
		return 0;
	}

	int pos = addNewSummonById(user, summonId);
	if (!pos)
	{
		//debug_message("添加灵兽error添加到包裹");
		destroy_summon(summonId);
		return 0;
	}
	user->addSummonType(type, summonId);
	//rpcSummonInfo(user->getId(), summonId);
	return summonId;
}

int destroySummon(int summonId)
{
	int equipBagId = GetSummonSave(summonId, K_SUMMON_EQUIP_BAGID);
	int bagId = GetSummonTemp(summonId, KT_SUMMON_BAG);
	int owner = GetSummonTemp(summonId, KT_SUMMON_OWNER);
	object user = get_user(owner);

	if (!objectp(user)) {
		destroy_summon(summonId);
		return 1;
	}

	if (equipBagId) {
		int items = user->getAllItemsInBag(equipBagId);
		foreach (int item in items) {
			if (!user->MoveItem(item, ITEM_BAG_NORMAL)) {
				// 物品栏满，无法删除召唤兽
				return 0;
			}
		}
	}

	if (user->removeSummonFromBag(summonId, bagId)) {
		destroy_summon(summonId);
		return 1;
	}
	return 0;
}

int getSummonFightValue( int summonId, mixed key) 
{
	mapping mpData = get_summon_map(summonId, TEMP_VAR);
	if ( undefinedp(mpData)) return 0;
	mapping tmpFightValue = get_summon_map(summonId, TEMP_VAR)[K_FIGHT_TOTAL];
	return tmpFightValue[key];
}
	
int getSummonBaseFightValue( int summonId, mixed key) 
{
	mapping mpData = get_summon_map(summonId, TEMP_VAR);
	if ( undefinedp(mpData)) return 0;
	mapping tmpFightValue = get_summon_map(summonId, TEMP_VAR)[K_FIGHT_BASE];
	return tmpFightValue[key];
}

static void calculateSummonJingying(int summonId, mapping finalAttr)
{
	if (summonId <= 0 || !mapp(finalAttr))
		return;
	
	finalAttr[FT_DODGE] = 0;
	finalAttr[FT_HIT] = 1000;
	finalAttr[FT_BAO_JI] = 0;
	finalAttr[FT_KANG_BAO] = 0;
	finalAttr[FT_ZHAO_JIA] = 0;
	finalAttr[FT_PO_JI] = 0;
	finalAttr[FT_YC_HIT] = 0;
	finalAttr[FT_YC_DODGE] = 0;
	finalAttr[FT_FAN_JI] = 0;
	finalAttr[FT_LIANG_JI] = 0;
	finalAttr[FT_BI_SHA] = 0;
	finalAttr[FT_HURT_MORE] = 0;
	finalAttr[FT_HURT_LESS] = 0;
}

mapping calculateSummonBase(int grade, int type)
{
	mapping mpTempBase = ([]);
	mpTempBase[KT_SUMMON_NAILI] =  to_int(pow(grade,1.2)/5*10) + mpSummonProp[type][KT_SUMMON_INI_NAILI]; 
	mpTempBase[KT_SUMMON_LILIANG] =  to_int(pow(grade,1.2)/5*10) + mpSummonProp[type][KT_SUMMON_INI_LILIANG];
	mpTempBase[KT_SUMMON_ZHILI] =  to_int(pow(grade,1.2)/5*10) + mpSummonProp[type][KT_SUMMON_INI_ZHILI];
	mpTempBase[KT_SUMMON_MINJIE] =  to_int(pow(grade,1.2)/5*10) + mpSummonProp[type][KT_SUMMON_INI_MINJIE];
	return mpTempBase;
}

int calculateSummon(int summonId)
{
	mapping mpPermVar, mpTempBase, mpTempTotal;
	int summonType = GetSummonSave(summonId, K_SUMMON_TYPE);
	
	mpPermVar = get_summon_map(summonId, PERM_VAR);
	mapping mpTempVar = get_summon_map(summonId, TEMP_VAR);

	int grade = mpPermVar[K_SUMMON_GRADE];
	int owner = mpTempVar[KT_SUMMON_OWNER];
	mpTempBase = calculateSummonBase(grade, summonType);
	mpTempTotal = ([]);
	
	//todo 下面获得经验要改
	//经验未导表，暂时写死	
	SetSummonTemp(summonId, KT_SUMMON_EXP_NEXT, 500 * grade ); 
	mpTempVar[K_FIGHT_BASE] = mpTempBase;
	mpTempVar[K_FIGHT_TOTAL] = mpTempTotal;

	int equipBagId = mpPermVar[K_SUMMON_EQUIP_BAGID];

	mapping equipBagAttr = ([]);

	object user = get_user(owner);

	if (owner && objectp(user)) {
		equipBagAttr = "module/equip/main"->getBagEquipAttr(user, equipBagId);

		if (sizeof(equipBagAttr)) {
			mpTempBase[KT_SUMMON_NAILI] += equipBagAttr[NAILI];
			mpTempBase[KT_SUMMON_LILIANG] += equipBagAttr[LILIANG];
			mpTempBase[KT_SUMMON_ZHILI] += equipBagAttr[ZHILI];
			mpTempBase[KT_SUMMON_MINJIE] += equipBagAttr[MINJIE];
		}
	}

	if (sizeof(equipBagAttr)) {
		mpTempBase[KT_USER_NAILI] = to_int(mpTempBase[KT_USER_NAILI] * (100 + equipBagAttr[NAILI_RATE]) / 100);
		mpTempBase[KT_USER_LILIANG] = to_int(mpTempBase[KT_USER_LILIANG] * (100 + equipBagAttr[LILIANG_RATE]) / 100);
		mpTempBase[KT_USER_ZHILI] = to_int(mpTempBase[KT_USER_ZHILI] * (100 + equipBagAttr[ZHILI_RATE]) / 100);
		mpTempBase[KT_USER_MINJIE] = to_int(mpTempBase[KT_USER_MINJIE] * (100 + equipBagAttr[MINJIE_RATE]) / 100);
	}

	mpTempTotal[KT_SUMMON_MAXHP] = equipBagAttr[FT_HP] + to_int(mpTempBase[KT_SUMMON_NAILI] * 5);
	mpTempTotal[KT_SUMMON_DEFENCE] = equipBagAttr[FT_DEFENCE] + to_int(mpTempBase[KT_SUMMON_LILIANG] * 0.5);
	mpTempTotal[KT_SUMMON_ATTACK] = equipBagAttr[FT_ATTACK] + to_int(mpTempBase[KT_SUMMON_LILIANG] * 1.5);
	mpTempTotal[KT_SUMMON_QIANGFA] = equipBagAttr[FT_QIANGFA] + to_int(mpTempBase[KT_SUMMON_ZHILI] * 1.5);
	mpTempTotal[KT_SUMMON_KANGFA] = equipBagAttr[FT_KANGFA] + to_int(mpTempBase[KT_SUMMON_ZHILI] * 0.5);
	mpTempTotal[KT_SUMMON_SPEED] = equipBagAttr[FT_SPEED] + to_int(mpTempBase[KT_SUMMON_MINJIE] * 1);

	calculateSummonJingying(summonId, mpTempTotal);
	
	mpPermVar[K_SUMMON_CUR_HP] = (mpTempTotal[KT_SUMMON_MAXHP]);

	//神格技能 属性
	int skillId = getSummonBaseAttr(summonType, K_SUMMON_BDSKILL);
	mapping skillData = "module/fight/data/shenge_skill"->get_data()[skillId];
	mapping shengeAttrMap = skillData["attr"];
		
	foreach (string shengeAttr, int value in shengeAttrMap)
	{
		mpTempTotal[shengeAttr] = value;
	}
	debug_message("summon Type tempTotal:%O", mpTempTotal);
	return 1;
}

int findSummonIdByEquipBagId(object user, int equipBagId)
{
	int *summons = user->getAllSummonsInBag();

	foreach (int summId in summons) {
		if (GetSummonSave(summId, K_SUMMON_EQUIP_BAGID) == equipBagId) {
			return summId;
		}
	}
	return 0;
}

string getNameByType(int type)
{
	return mpSummonProp[type][K_SUMMON_NAME];
}

string getSkillByType(int type)
{
	return mpSummonProp[type][K_SUMMON_SKILL];
}
/*
*return 0失败 大于0成功,值为summonId
*/
int createSummon(int type)
{
	int summonId;
	if (undefinedp(mpSummonProp[type]))
	{
		//debug_message(sprintf("灵兽类型[%d]不存在!", type));
		return 0;	
	}
	summonId = new_summon(SUMMON_PATH);
	if (!summonId) 
	{
		//debug_message("无法生成灵兽!");
		return 0;
	}

	SetSummonSave(summonId, K_SUMMON_TYPE, type);
	SetSummonSave(summonId, K_SUMMON_NAME, getNameByType(type));
	SetSummonSave(summonId, K_SUMMON_GRADE, 1);     /*等级*/
	SetSummonSave(summonId, K_SUMMON_EXP, 0);     /*等级*/
	//SetSummonSave(summonId, M_SUMM_SKILL_GRID, ([]));
	calculateSummon(summonId);
	SetSummonSave( summonId, K_SUMMON_CUR_HP, getSummonFightValue( summonId, KT_SUMMON_MAXHP) );
	SetSummonTemp( summonId, KT_SUMMON_RESOURCE, type);
	return summonId;
}

/*
*这个函数将新生的灵兽加到包裹里
*return 0失败 大于0为添加到的bag pos
*/
varargs int addNewSummonById(object user, int summonId, int bagId)
{
	if (summonId <= 0)
		return 0;
	SetSummonTemp(summonId, KT_SUMMON_OWNER, user->getId());
	if (undefinedp(bagId))
		bagId = SUMMON_BAG_NORMAL;
	int ret = user->addSummonToBag(summonId, bagId);
	int uid = user->getId();
	if (!ret) 
	{
		debug_message(sprintf("玩家[%d]添加灵兽[%O]失败!", uid, get_summon_map(summonId, 1)));
		destroy_summon(summonId);
		return 0;
	}
	
	if ( GetSummonSave(summonId, K_GLOBAL_KEY) == "" )
	{
		string globalKey = Import("GLOBAL_KEY")->new_summon_globalkey();	
		SetSummonSave(summonId, K_GLOBAL_KEY, globalKey);
		logger->Log("[%d]AddSummon,summonId=%d, type=%d, global_key=%s.", uid,summonId, GetSummonSave(summonId, K_SUMMON_TYPE), GetSummonSave(summonId, K_GLOBAL_KEY) );
		// 新增的灵兽GBLKEY添加到玩家身上
		mapping mpGlobalKeys = user->getTemp(KT_SUMMON_GLOBALKEY_MAP);
		if(undefinedp(mpGlobalKeys[globalKey])) {
			mpGlobalKeys[globalKey] = summonId;
		}
	}

	int equipBagId = user->createSummonEquipFrames(GetSummonSave(summonId, K_GLOBAL_KEY));

	if (!equipBagId) {
		debug_message("[%d] 灵兽创建装备栏失败 %s", uid, GetSummonSave(summonId, K_GLOBAL_KEY));
		destroy_summon(summonId);
		return 0;
	}
	SetSummonSave(summonId, K_SUMMON_EQUIP_BAGID, equipBagId);
	return ret;
}

void rpcSummonInfo(int uid, int summonId)
{
	class Summon summonInfo = summonInfoPack(summonId);

	rpc_client_summon_info(uid, summonInfo);
}	

private class SkillInfo packSummonBDSkill(int type)
{
	int skillId = getSummonBaseAttr(type, K_SUMMON_BDSKILL);
	class SkillInfo skillInfo = new(class SkillInfo);
	mapping skillData = "module/fight/data/shenge_skill"->get_data()[skillId];
	skillInfo->name = skillData["name"];
	skillInfo->desc = skillData["desc"];
	return skillInfo;
}

private class SkillInfo packSummonZDSkill(int type)
{
	string skillId = getSummonBaseAttr(type, K_SUMMON_SKILL);
	class SkillInfo skillInfo = new(class SkillInfo);
	mapping skillData = "module/fight/data/skill"->get_data()[skillId];
	debug_message("summon skillId:%s", skillId);
	skillInfo->name = skillData["name"];
	skillInfo->desc = skillData["info"];
	return skillInfo;
}

private class Summon summonInfoPack(int summonId)
{
	class Summon summonInfo = new(class Summon);	
	int summonType = GetSummonSave(summonId, K_SUMMON_TYPE);
	mapping mpInfo = mpSummonProp[summonType];
	summonInfo->summonId = summonId;
	summonInfo->typeid = summonType;
	summonInfo->icon = summonType;
	summonInfo->bagId = GetSummonTemp(summonId, KT_SUMMON_BAG);
	summonInfo->pos =  GetSummonTemp(summonId, KT_SUMMON_POS);
	summonInfo->name = GetSummonSave(summonId, K_SUMMON_NAME);
	summonInfo->grade = GetSummonSave(summonId, K_SUMMON_GRADE);
	summonInfo->hp = getSummonFightValue(summonId, KT_SUMMON_MAXHP);
	summonInfo->maxHP = getSummonFightValue(summonId, KT_SUMMON_MAXHP);
	summonInfo->attack = getSummonFightValue(summonId, KT_SUMMON_ATTACK);
	summonInfo->defence = getSummonFightValue(summonId, KT_SUMMON_DEFENCE);
	summonInfo->qiangfa = getSummonFightValue(summonId, KT_SUMMON_QIANGFA);
	summonInfo->kangfa = getSummonFightValue(summonId, KT_SUMMON_KANGFA);
	summonInfo->speed = getSummonFightValue(summonId, KT_SUMMON_SPEED);

	summonInfo->equipBagId = GetSummonSave(summonId, K_SUMMON_EQUIP_BAGID);

	summonInfo->naili = getSummonBaseFightValue(summonId, KT_SUMMON_NAILI);
	summonInfo->liliang = getSummonBaseFightValue(summonId, KT_SUMMON_LILIANG);
	summonInfo->minjie = getSummonBaseFightValue(summonId, KT_SUMMON_MINJIE);
	summonInfo->zhili = getSummonBaseFightValue(summonId, KT_SUMMON_ZHILI);
	summonInfo->xibie = getSummonBaseAttr(summonType, K_SUMMON_XIBIE);
	summonInfo->zhanli = 999;
	summonInfo->zdSkill = packSummonZDSkill(summonType);
	summonInfo->bdSkill = packSummonBDSkill(summonType);
	return summonInfo;	
}


void create()
{
	logger = Import("LOG")->New("summon_util");
	mpSummonProp = "module/summon/summon_base_data"->get_data();
}

int getExp(int summonId)
{
	return GetSummonSave(summonId, K_SUMMON_EXP);
}

private void onUpOneGrade(object user, int summonId)
{
	int oldGrade = getGrade(summonId);
	int exp = getExp(summonId);
	int oldGradeMaxExp = "data/user/exp"->getGradeMaxExp(oldGrade);
	int newExp = exp - oldGradeMaxExp;
	if (newExp < 0)
		return;
	SetSummonSave(summonId, K_SUMMON_EXP, newExp);
	SetSummonSave(summonId, K_SUMMON_GRADE, oldGrade + 1);
	calculateSummon(summonId);
	rpcSummonInfo(user->getId(), summonId);
	user->tellTips("伙伴%s升到%d级", getName(summonId), oldGrade + 1); 
}


int upGrades(object user, int summonId)
{
	int upGradeFlag = 0;
	while(1)
	{
		if (checkUpGrade(summonId))
		{
			upGradeFlag = 1;
			onUpOneGrade(user, summonId);
		}
		else
			break;
	}
	if (upGradeFlag)
		return 1;
	return 0;
}

int checkUpGrade(int summonId)
{
	int grade = getGrade(summonId);
        int exp = getExp(summonId);
	int gradeUpExp = "data/user/exp"->getGradeMaxExp(grade);
	if (exp >= gradeUpExp)
		return 1;
	return 0;
}

int getGrade(int summonId)
{
	return GetSummonSave(summonId, K_SUMMON_GRADE);
}


void addExp(int summonId, int addedExp)
{
	if (addedExp <= 0)
		return;
	int ownerId = GetSummonTemp(summonId, KT_SUMMON_OWNER);
	object user  = get_user(ownerId);
	if(!objectp(user))
		return;
	int curExp = GetSummonSave(summonId, K_SUMMON_EXP);
	SetSummonSave(summonId, K_SUMMON_EXP, curExp +  addedExp);
	user->tellTips("伙伴%s得到%d经验", getName(summonId), addedExp); 
	upGrades(user, summonId);
	return;
}


int getSummonResource(int summonId)
{
	return GetSummonTemp(summonId, KT_SUMMON_RESOURCE);
}

string getSummonName(int summonId)
{
	return GetSummonSave(summonId, K_SUMMON_NAME);
}

string getSummonSkill(int summonId)
{
	int type =  GetSummonSave(summonId, K_SUMMON_TYPE);
	return mpSummonProp[type][K_SUMMON_SKILL];
}

string getSummonNormalAttack(int summonId)
{
	int type =  GetSummonSave(summonId, K_SUMMON_TYPE);
	return mpSummonProp[type][K_SUMMON_NORMAL_ATTACK];
}
int getSummonGrade(int summonId)
{
	return GetSummonSave(summonId, K_SUMMON_GRADE);
}

int getSummonType(int summonId)
{
	return GetSummonSave(summonId, K_SUMMON_TYPE);
}

int getSummonIdByType(object user, int type)
{
	return user->getSummonIdByType(type);
}

//todo:del test only
int setFightValue(int summonId, string key, int value)
{
	mapping mpData = get_summon_map(summonId, TEMP_VAR);
	if ( undefinedp(mpData)) return 0;
	mapping tmpFightValue = get_summon_map(summonId, TEMP_VAR)[K_FIGHT_TOTAL];
	//debug_message("fight value map:%O key:%s", tmpFightValue, key);
	tmpFightValue[key] = value;
}

void addAllFightSummonExp(object user, int exp)
{
	int* fightSummonList = user->getAllFightSummon();
	//debug_message("addallsummon exp list:%O", fightSummonList);
	foreach (int summonId in fightSummonList)
	{
		addExp(summonId, exp);
	}
}
