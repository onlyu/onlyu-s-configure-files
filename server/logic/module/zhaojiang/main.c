#include "main.h"
#include <summon_key.h>
#include "/rc/rpc/zhaojiang.h"

int getZhaojiangGrade(object user)
{
	return 10;
}

int isZhaojiangGradeOK(object user, int summonType)
{
	return Import("SUMMON_UTIL")->getZhaojiangGrade(summonType) <= getZhaojiangGrade(user);
}

int canBeMyFellow(object user, int summonType)
{
	if (!Import("SUMMON_UTIL")->isValidSummon(summonType))
		return 0;
	int totalSummonAmount = user->getTotalSummonAmount();
	int maxHoldSummonAmount = user->getMaxHoldSummonAmount();
	if (totalSummonAmount >= maxHoldSummonAmount)
	{
		return 0;
	}

	return isZhaojiangGradeOK(user, summonType);
}


class ZhaojiangFellow* getCanGetFellowList(object user)
{
	int* allSummonList = Import("SUMMON_UTIL")->getAllSummon();
	
	class ZhaojiangFellow* zhaomuList = ({});
	class ZhaojiangFellow tempZhaojiangFellow;
	foreach (int summonType in allSummonList)
	{
		if (!isZhaojiangGradeOK(user, summonType))
			continue;
		tempZhaojiangFellow = new (class ZhaojiangFellow);
		tempZhaojiangFellow->type = summonType;
		tempZhaojiangFellow->name = Import("SUMMON_UTIL")->getNameByType(summonType);
		tempZhaojiangFellow->icon = summonType;
		tempZhaojiangFellow->isFellow = Import("SUMMON_UTIL")->hasSummonByType(user, summonType);
		tempZhaojiangFellow->zhaojiangGrade = Import("SUMMON_UTIL")->getZhaojiangGrade(summonType);
		if (tempZhaojiangFellow->zhaojiangGrade == 0)
			continue;
		zhaomuList += ({tempZhaojiangFellow});
	}
	//debug_message("zhaojiang list:%O", zhaomuList);
	return zhaomuList;
}

int beMyFellow(object user, int summonType)
{
	if (Import("SUMMON_UTIL")->hasSummonByType(user, summonType))
	{
		debug_message("zhaojiang already has uid:%d type:%d", user->getId(), summonType);
		return 0;
	}

	if (!canBeMyFellow(user, summonType))
	{
		debug_message("zhaojiang can't be my fellow uid:%d type:%d", user->getId(), summonType);
		return 0;
	}

	int ret = Import("SUMMON_UTIL")->addNewSummonByType(user, summonType);
	if (ret)
	{
		rpc_client_zhaojiang_get_fellow(user->getId(), summonType);
		
		return 1;
	}
	return 0;
}


class FellowDetail packFellowDetail(mapping summonBaseData, int type)
{
	class FellowDetail fellowDetail = new(class FellowDetail);
	fellowDetail->naili = summonBaseData[KT_SUMMON_INI_NAILI];
	fellowDetail->nailiGrow = to_int(summonBaseData[KT_SUMMON_INI_NAILI_GROW] * 100);
	fellowDetail->liliang = summonBaseData[KT_SUMMON_INI_LILIANG];
	fellowDetail->liliangGrow = to_int(summonBaseData[KT_SUMMON_INI_LILIANG_GROW]*100);
	fellowDetail->minjie = summonBaseData[KT_SUMMON_INI_MINJIE];
	fellowDetail->minjieGrow = to_int(summonBaseData[KT_SUMMON_INI_MINJIE_GROW]*100);
	fellowDetail->zhili = summonBaseData[KT_SUMMON_INI_ZHILI];
	fellowDetail->zhiliGrow = to_int(summonBaseData[KT_SUMMON_INI_ZHILI_GROW]*100);
	fellowDetail->shenli = 250;
	fellowDetail->zhanli = 999;
	int bdSkillId = summonBaseData[K_SUMMON_BDSKILL];
	mapping bdSkillData = "module/fight/data/shenge_skill"->get_data()[bdSkillId];
	
	fellowDetail->bdSkill = bdSkillData["name"];
	fellowDetail->bdSkillDesc = bdSkillData["desc"];

	string zdskillId = Import("SUMMON_UTIL")->getSummonBaseAttr(type, K_SUMMON_SKILL);
	mapping skillData = "module/fight/data/skill"->get_data()[zdskillId];
	fellowDetail->zdSkill = skillData["name"];
	fellowDetail->zdSkillDesc = skillData["info"];
	fellowDetail->generalDesc = summonBaseData["summondesc"];
	return fellowDetail;
}
