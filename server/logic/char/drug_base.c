// 药品父类
#include <item.h>
inherit ITEM_BASE;
#include <item_key.h>
//#include <fight.h>
//#include <summon_key.h>

// 药品使用需要等级
int GetNeedGrade()
{
	return 0;
}

// 持续回合数
// 默认即时
int GetLastBout()
{
	return 0;
}

// 战斗外增加血量
int GetAddHpOut()
{
	return 0;
}

// 战斗外增加法量
int GetAddMpOut()
{
	return 0;
}

// 增加血量
int GetAddHp()
{
	return 0;
}

// 增加法量
int GetAddMp()
{
	return 0;
}

// 死亡能否使用
int CanUserInDie()
{
	return 0;
}

// 药品CD回合数
int GetDrugCDBout()
{
    return 0;
}

// 药品CD KEY
string GetDrugCDKey()
{
    return "";
}



// return > 0，外面删除
// 外面保证这个药品是在玩家身上的

/*************取消药品使用**********************
int DoUse(object oUser, int itemid, int iTarget)
{
	int iHp, iMp, bRevive, bDie, iRate;
	int iHpMax, iMpMax, iUHp, iUMp, iRes, iDelta ;
	int usernum = oUser->GetId();
	int iFightId, needGrade;
	object oTarget;
	mapping mpDrugInfo;

	iFightId = get_user_fightid(usernum);
	// 非战斗状态下使用
	if ( iFightId ) 
	{
		return 0;
	}	

	if ( undefinedp( iTarget ) )
	{
		debug_message("undefined iTarget");
	}

	needGrade = GetNeedGrade();

	if (needGrade && needGrade > oUser->GetiGrade() )
	{
		oUser->TellMe( "这药使用方法复杂，再历练历练再使用吧" );
		return 0;
	}  

	iRate = Import("PRODUCT")->OnUseDrug(oUser);
	iHp = to_int(GetAddHpOut()*iRate/1000);
	iMp = to_int(GetAddMpOut()*iRate/1000);

	if ( !iHp && !iMp )
	{
		// 不是药品？return
		oUser->TellMe( "此药品禁止战斗外使用" );
		return 0;
	}	

	iRes = 0;

	if ( iTarget != 0 ) // == 0 表示对自己使用
	{
		oTarget = get_user(iTarget);
	}
	else if (  iTarget == 0 || iTarget == oUser->GetId() )
	{
		oTarget = oUser;
	}

	if ( oTarget ) // user
	{	
		iHpMax = oTarget->GetAttribute(I_A_HP_MAX_KEY);
		iMpMax = oTarget->GetAttribute(I_A_MP_MAX_KEY);
		iUHp = oTarget->GetProp(K_USER_HP_KEY);
		iUMp = oTarget->GetProp(K_USER_MP_KEY);

		iDelta = iHpMax - iUHp;
		if ( iDelta > iHp ) 
			iDelta = iHp;

		if ( iHp && iDelta > 0)
		{
			oTarget->SetProp(K_USER_HP_KEY, iUHp + iDelta);
			iRes += iDelta;
		}	

		iDelta = iMpMax - iUMp;
		if ( iDelta > iMp )
			iDelta = iMp;

		if ( iMp && iDelta > 0)
		{
			oTarget->SetProp(K_USER_MP_KEY, iUMp + iDelta);
			iRes += iDelta;
		}	

		if ( iRes )
		{
			"/cmd/user"->RsyncHPMPToClient(oTarget);	
		}
		else
		{
			if ( iHp && iMp )
			{
				oUser->TellMe("您气血法力充足，无需使用药品");	
			}
			else if ( iHp )
			{
				oUser->TellMe("您气血充足，无需使用药品");	
			}
			else
			{
				oUser->TellMe("您法力充足，无需使用药品");	
			}
		}
	}
	else if ( is_summon_online(iTarget) ) // summon
	{
		iHpMax = Import("SUMMON_UTIL")->GetSummonFightValue(iTarget, I_A_HP_MAX_KEY);
		iMpMax = Import("SUMMON_UTIL")->GetSummonFightValue(iTarget, I_A_MP_MAX_KEY);
		iUHp = GetSummonSave(iTarget, I_SUMM_CUR_HP);
		iUMp = GetSummonSave(iTarget, I_SUMM_CUR_MP);

		iDelta = iHpMax - iUHp;
		if ( iDelta > iHp ) iDelta = iHp;

		if ( iHp && iDelta > 0 )
		{
			SetSummonSave(iTarget, I_SUMM_CUR_HP, iUHp + iDelta );
			iRes += iDelta;
			// rpc_client_summon_hp( GetSummonTemp(iTarget, I_T_SUMM_OWNER), iTarget, GetSummonSave(iTarget, I_SUMM_CUR_HP) );
		}
	
		iDelta = iMpMax - iUMp;
		if ( iDelta > iMp )
			iDelta = iMp;

		if ( iMp && iDelta > 0)
		{
			SetSummonSave(iTarget, I_SUMM_CUR_MP, iUMp + iDelta );
			iRes += iDelta;
			// rpc_client_summon_mp( GetSummonTemp(iTarget, I_T_SUMM_OWNER), iTarget, GetSummonSave(iTarget, I_SUMM_CUR_MP) );
		}

		if ( iRes )
		{
			"/cmd/summon"->RsyncHPMPToClient(iTarget);
		}
		else
		{
			if ( iHp && iMp )
			{
				oUser->TellMe("灵兽气血法力充足，无需使用药品");	
			}
			else if ( iHp )
			{
				oUser->TellMe("灵兽气血充足，无需使用药品");	
			}
			else
			{
				oUser->TellMe("灵兽法力充足，无需使用药品");	
			}
		}
	}	

	return iRes;
}
************************取消药品使用***************************/

int DoUse(object oUser, int itemid, int iTarget)
{
	// from 20120315, can not use drug out of fight
	oUser->TellMe("此物品不能使用");
	return 0;
}



int SpecialPurpose( int fightId, int warrior, int itemid, int target )
{
	return 0;
}

#define DRUG_HP "drug_hp"
#define DRUG_MP "drug_mp"
#define DRUG_REBORN "drug_reborn"

//static * cannotUseStatus = ({ DIE_STATUS, SEAL_STATUS, "fuguiwuwu", "rumeng", "mengdie", "jianzhiwanghun", "luopo", });
static * cannotRebornStatus = ({ "shibao", "huijie", });

int DoUseInFight(int fightId, int warrior, int itemid, int side, int position)
{
	//先删掉这个函数,需要再从fs代码导入
}


// 类名
string class_name()
{
    //return ::class_name() + ".drug";
}
