#if 0
#include <summon_key.h>
#include <summon.h>
#include <debug.h>
#include <macros.h>
#include "/module/achieve/achieve_def.h"
#include "/module/achieve/achieve_arg_def.h"

static mapping mpSummonProp = 
#include "prop.inl"
;

private object logger;
static string summon_skill_key = "summon_skill";

void create_logger()
{
	logger = Import("LOG")->New("summon");

	// globalkey, skill lost, skill got
	logger->SetDBFormat(summon_skill_key, "%s,%s,%s");
}


#include "skill_grid.c"

// 存储方式
// 技囊：基础技能的String * 
// 身上技能表：mapping = ([string cSkill:int iLevel,])
// 技能位置表：mapping = ([int iPos:string cSkill,])
// 天赋技能：mapping = ([string cSkill:int iLevel,])


// key value 互斥
static mapping mpExclusion = ([
		"1":"2", "3":"4", "5":"6", "7":"8", "9":"10", "11":"12", "13":"14", "15":"16", "17":"18", "19":"20",
		"21":"22", "23":"24", "25":"26", "27":"28", "29":"30", "31":"32", "33":"34", "35":"36", "37":"38",
		"162":"163", "165":"166", "167":"168", "169":"170", "171":"172", "174":"175", "179":"180", "181":"182",
		"184":"185", "187":"188", "189":"190", "193":"194", "195":"196", "197":"198", "310":"311", "315":"316",
		"317":"318", "319":"320", "324":"325", "326":"327", "328":"329", "330":"331", "332":"333", "334":"335",
		"336":"337", "339":"340", "341":"342", "462":"463", "464":"465", "466":"467", "468":"469", "470":"471",
		"472":"473", "477":"478", "485":"486", "487":"488", "489":"490", "491":"492", "493":"494", "495":"496",
		"497":"498", "499":"500", "501":"502", "503":"504", "505":"506", "507":"508", "612":"613", "614":"615",
		"616":"617", "618":"619", "620":"621", "622":"623", "624":"625", "626":"627", "629":"630", "632":"633",
		"634":"635", "639":"640", "641":"642", "643":"644", "646":"647", "648":"649", "650":"651",
		"652":"653", "654":"655", "762":"763", "764":"765", "766":"767", "769":"770", "771":"772", "774":"775", "776":"777",
		"778":"779", "780":"781", "783":"784", "785":"786", "788":"789", "790":"791", "794":"795", "797":"798", "799":"800",
		// 5境界技能互斥表
		"793":"804", "638":"656", "645":"657", "39":"50", "40":"51", "41":"52", "42":"53", "161":"202", "321":"350",
		"484":"513", 
		]);

static string * HiLevelSkills = ({});
static mapping mpRealmPos = ([	]);
static mapping mpPosRealm = ([ // 位置对应的境界
			1 : JINGJIE_YUANSHENG,
			2 : JINGJIE_YUANSHENG,
			3 : JINGJIE_XUANZHAO, 
			4 : JINGJIE_XUANZHAO,
			5 : JINGJIE_ZHUJI,
			6 : JINGJIE_KAIGUANG,
			7 : JINGJIE_LINGXU,
			]);

// 是否高级技能
int IsHiLevelSkill(string cSkill)
{
	return member_array(cSkill, HiLevelSkills) != -1;
}

// 境界对应的技能数量
static mapping mpSkillLimit = ([
		JINGJIE_YUANSHENG : 2,
		JINGJIE_XUANZHAO : 4,
		JINGJIE_ZHUJI : 5,
		JINGJIE_KAIGUANG : 6,
		JINGJIE_LINGXU : 7,
		]);

// 自动生成		
static mapping mpRaceSkills = ([
		/*
		0 : ([ // 种族:公用,仙灵,精怪，魂，魔，异物
				1 : ({ "5", "9", "11", "13", "15", "17", "19", "35", "39", "40", "41", "42",}), // 境界
				2
				3
				4
			]),
		*/	
		]);

// 各方向对应的天赋技能
static mapping mpTypeTalentSkill = ([
		SUMMON_TYPICAL_GONG : "43", // 攻
		SUMMON_TYPICAL_FA : "44", // 法
		SUMMON_TYPICAL_XUE : "45", // 血
		SUMMON_TYPICAL_MIN : "46", // 敏
		]);

// 各种族对应的方向天赋升级所需要的技能个数
static mapping mpRaceTalentUpgrade = ([
		SUMMON_RACE_XIANLING : ([1:21,2:41,3:58,4:64,5:70,6:82,]), // 仙灵
		SUMMON_RACE_JINGGUAI : ([1:20,2:40,3:54,4:60,5:66,6:78,]), // 精怪
		SUMMON_RACE_HUN : ([1:21,2:42,3:57,4:64,5:70,6:82,]), // 魂
		SUMMON_RACE_MO : ([1:22,2:43,3:58,4:64,5:70,6:82,]), // 魔
		SUMMON_RACE_YIWU : ([1:22,2:42,3:57,4:64,5:70,6:82,]), // 异物
		]);

// 技能境界表 自动生成
static mapping mpRealmSkillTbl = ([ ]);
// 技能对应的种族  自动生成
static mapping mpRaceSkillTbl = ([ ]);
// 技能名字表 自动生成
static mapping mpNameSkillTbl = ([ ]);
// 每境界之间技囊个数差别
// NOTE:这数据并不是从mpRaceTalentUpgrade里取的!
static mapping mpRealmDelta = ([
	SUMMON_RACE_XIANLING : ([JINGJIE_YUANSHENG:21,JINGJIE_XUANZHAO:20,JINGJIE_ZHUJI:17,JINGJIE_KAIGUANG:12, JINGJIE_LINGXU:12,]), // 仙灵
	SUMMON_RACE_JINGGUAI : ([JINGJIE_YUANSHENG:20,JINGJIE_XUANZHAO:20,JINGJIE_ZHUJI:14,JINGJIE_KAIGUANG:12, JINGJIE_LINGXU:12,]), // 精怪
	SUMMON_RACE_HUN : ([JINGJIE_YUANSHENG:21,JINGJIE_XUANZHAO:21,JINGJIE_ZHUJI:15,JINGJIE_KAIGUANG:13, JINGJIE_LINGXU:12,]), // 魂
	SUMMON_RACE_MO : ([JINGJIE_YUANSHENG:22,JINGJIE_XUANZHAO:21,JINGJIE_ZHUJI:15,JINGJIE_KAIGUANG:12, JINGJIE_LINGXU:12,]), // 魔
	SUMMON_RACE_YIWU : ([JINGJIE_YUANSHENG:22,JINGJIE_XUANZHAO:20,JINGJIE_ZHUJI:15,JINGJIE_KAIGUANG:13, JINGJIE_LINGXU:12,]), // 异物
]);

int RealmSkillLibAmount(int iRace, int iLevel)
{
	int i = mpRealmDelta[iRace][iLevel];

	if ( i < 1 )
	{	
		debug_message( sprintf("iRace %d, iLevel %d have no mpRealmDelta", iRace, iLevel) );
		i = 24;
	}

	//debug_message( sprintf("iRace %d, iLevel %d, i %d", iRace, iLevel, i) );
	//debug_message( sprintf("%O", mpRealmDelta[iRace]) );
	return i ;
}

string SkillName(string cSkill)
{
	// return Import("SKILL_UTIL")->GetSkillName(cSkill);
	// debug_message( sprintf("cSkill %s", cSkill) );
	// debug_message( sprintf("All %O", mpNameSkillTbl) );
	return mpNameSkillTbl[cSkill];
}

int IsSummonSkill(string cSkill)
{
	return sizeof(cSkill) < 6 ;
}

// 0 表示公用种族
int IsSummonRace( int iRace)
{
	return (iRace >= 0 && iRace <= 5);
}

void create()
{
	mapping mpTmp = ([]);

	create_logger();
	create_logger_grid();
	// 生成高阶技能表
	HiLevelSkills = values(mpExclusion);

	// 生成互斥全表
	foreach(string cKey, string cValue in mpExclusion)
	{
		mpTmp[cValue] = cKey;
	}
		
	foreach ( string cKey, string cValue in mpTmp)
		mpExclusion[cKey] = cValue;

	foreach ( string _cSkill, mapping _mpTmp in "module/skill/all_skill.c"->get_data() )
	{
		int iRace, iRealm;
		string cName;

		_cSkill = BaseSkillId(_cSkill);
		cName = _mpTmp["name"];
		// 生成技能名字对应表
		mpNameSkillTbl[_cSkill] = cName; // 所有技能中文名都cache一份

		if ( ! IsSummonSkill(_cSkill) )
			continue;

		iRace = _mpTmp["class"];

		if ( ! IsSummonRace(iRace) )
			continue;

		iRealm = _mpTmp["jingjie"];
		// iRealm == 0 表示天赋技能
		if ( iRealm < 1 )
			continue;

		if ( undefinedp(mpRaceSkills[iRace]) ) 
			mpRaceSkills[iRace] = ([]);

		if ( undefinedp(mpRaceSkills[iRace][iRealm]) )
			mpRaceSkills[iRace][iRealm] = ({});

		if ( member_array(_cSkill, mpRaceSkills[iRace][iRealm]) != -1 )
			continue;

		// 生成 mpRaceSkills
		mpRaceSkills[iRace][iRealm] += ({ _cSkill });
		// 生成技能境界表\种族技能表
		mpRealmSkillTbl[_cSkill] = iRealm;
		mpRaceSkillTbl[_cSkill] = iRace;
	}

	//导出境界与技能格的对应关系
	foreach(int pos, int jingjie in mpPosRealm){
		if( undefinedp(mpRealmPos[jingjie]) ){
			mpRealmPos[jingjie] = ({ });
		}
		mpRealmPos[jingjie] += ({pos});
		mpRealmPos[jingjie] = sort_array(mpRealmPos[jingjie], 1);
	}

	/* // 此逻辑有错，请勿启用
	foreach( int _race, mapping _tmp in mpRaceTalentUpgrade)
	{
		if ( undefinedp(mpRealmDelta[_race]) )
			mpRealmDelta[_race] = ([]);

		foreach( int _level, int _total in _tmp)
		{	
			mpRealmDelta[_race][_level] = _total - mpRaceTalentUpgrade[_race][_level-1];
		}
	}
	*/

	//debug_message( sprintf("mpRealmDelta = %O", mpRealmDelta) );
}

int IsSkillFull(int summid)
{
	int iRealm = GetSummonTemp(summid, KT_SUMMON_REALM);

	return sizeof(GetSummonSave(summid, M_SUMM_SKILLS)) >= mpSkillLimit[iRealm];
}

// 是否能添加技能
varargs int CanAddSkill(int summid, string cSkill, string * ExcludeSkills)
{
	string * AllSkills ;

	if ( mpRealmSkillTbl[cSkill] > GetSummonTemp(summid, KT_SUMMON_REALM) )
		return 0;

	AllSkills = keys(GetSummonSave(summid, M_SUMM_SKILLS));
	if ( ExcludeSkills )
	{
		AllSkills -= ExcludeSkills;
	}	
	foreach (string cTmp in AllSkills)
	{
		//debug_message( sprintf("check skill %s, cSkill %s, mpExclusion[cTmp]=%s, %d", cTmp, cSkill, mpExclusion[cTmp], mpExclusion[cTmp] == cSkill) );
		if ( mpExclusion[cTmp] == cSkill )
			return 0;
	}
	return 1;
}
// 升级已有技能
int UpgradeSkill(int summid, string skillid)
{
	int iLevel, iOwner ;
	mapping mpSkill;

	mpSkill = GetSummonSave(summid, M_SUMM_SKILLS);
	iLevel = mpSkill[skillid] ;

	assert( iLevel > 0 );

	if ( iLevel >= GetSummonTemp(summid, KT_SUMMON_REALM) )
		return 0;

	iLevel += 1;
	mpSkill[skillid] = iLevel ;
	SetSummonSave(summid, M_SUMM_SKILLS, mpSkill);

#ifdef __SUMMON_SKILL_CACHE__	
	AllSkillGrids(summid);
#endif	

	iOwner = GetSummonTemp(summid, KT_SUMMON_OWNER);
	Import("SUMMON_UTIL")->CalSummon(summid);
	Import("SUMMON_UTIL")->rpc_SummonInfo(iOwner, summid);
	Import("SUMMON_UTIL")->rpc_SummonSkill(iOwner, summid);
	Import("SUMMON_UTIL")->rpc_SummonElement(iOwner, summid);

	return iLevel;
}


int DoUseQiangxiao(object oUser, int itemnum, int summid)
{
	mapping mpSkills;
	string * Skills;
	int iRealm, iOwner, iLevel ;
	
	if ( !summid || !is_summon_online(summid) )
	{
		oUser->TellMe("此物品只能对灵兽使用。");
		return 0;
	}

	iOwner = oUser->GetId();
	if ( GetSummonTemp(summid, KT_SUMMON_OWNER) != iOwner )
	{
		oUser->TellMe("对别人的灵兽使用强效要诀？有创意。");
		return 0;
	}	
	
	iRealm = GetSummonTemp(summid, KT_SUMMON_REALM);
	mpSkills = GetSummonSave(summid, M_SUMM_SKILLS);
	Skills = shuffle(keys(mpSkills));

	foreach ( string cSkill in Skills )
	{
		iLevel = mpSkills[cSkill];
		if ( iLevel >= iRealm )
			continue;

		if ( UpgradeSkill(summid, cSkill) )
		{
			iLevel += 1;
			logger->Log("%d use %s Qiangxiao, %s summon's skill %s upgrade to %d OnUseQiangxiao.", iOwner, GetItemSave(itemnum, K_GLOBAL_KEY),  GetSummonSave(summid, K_GLOBAL_KEY), cSkill, iLevel);

			oUser->TellMe( sprintf("%s的%s技能升级到了%d。", GetSummonSave(summid, K_SUMMON_NAME), SkillName(cSkill), iLevel) );

			/*
			// 新手目标统计 灵兽激活六个四级技能
			Import("SUMMON_UTIL")->SummonSkillLvAch(oUser, summid);
			*/
			return 1;
		}
	}

	oUser->TellMe("此灵兽没有技能可以升级");
	return 0;
}

// 升级
void OnUpgrade(int summid)
{
	mapping mpSkills;
	int iOwner, iRealm;
	object oUser;
	// 每次灵兽升级 max（0,1%+（人物境界-技能等级）/100）
	int iGrade = GetSummonSave(summid, K_SUMMON_GRADE);
	iOwner = GetSummonTemp(summid, KT_SUMMON_OWNER);
	oUser = get_user(iOwner);
	assert( oUser );

	iRealm = oUser->GetiJingjie();
	if ( !iRealm) iRealm = 1;

	mpSkills = GetSummonSave(summid, M_SUMM_SKILLS);
	foreach ( string cSkill, int iLevel in mpSkills )
	{
		if ( iLevel >= iRealm )
			continue;

		// 1%+（人物境界-技能等级）/100
		if ( random(100) < (iRealm-iLevel+1) )
		{
			iLevel += 1;
			logger->Log("%d's %s summon, skill %s upgrade to %d OnUpgrade.", GetSummonTemp(summid, KT_SUMMON_OWNER), GetSummonSave(summid, K_GLOBAL_KEY), cSkill, iLevel);

			if ( UpgradeSkill(summid, cSkill) )
			{	
				oUser->TellMe( sprintf("%s的%s技能升级到了%d。", GetSummonSave(summid, K_SUMMON_NAME), SkillName(cSkill), iLevel) );
				oUser->StatsExec(ACHIEVE_LING_WU_JI_NENG,1);
				break;
			}	
		}
	}
}

// 升境界
void OnUpRealm(object oUser, int summid)
{
	int iRealm;
	iRealm = GetSummonTemp(summid, KT_SUMMON_REALM);

	// 天赋技能不升级了
	/*
	int bUpgrade = 0;
	mapping mpTalentSkills ;

	string * RealmSkills = ({ "611", "312", "164", "461", "761", });

	mpTalentSkills = GetSummonSave(summid, M_SUMM_TALENTSKILLS);

	foreach ( string cSkill, int iLevel in mpTalentSkills )
	{	
		if ( member_array(cSkill, RealmSkills) == -1 )
			continue;

		if ( iLevel > iRealm )
			continue;

		mpTalentSkills[cSkill] = iLevel + 1;
		bUpgrade = 1;
		logger->Log("%d's %s summon, skill %s upgrade to %d OnUpRealm.", GetSummonTemp(summid, KT_SUMMON_OWNER), GetSummonSave(summid, K_GLOBAL_KEY), cSkill, iLevel) );
	}

	if ( bUpgrade )
	{
		SetSummonSave(summid, M_SUMM_TALENTSKILLS, mpTalentSkills);
		Import("SUMMON_UTIL")->rpc_SummonSkill(GetSummonTemp(summid, KT_SUMMON_OWNER), summid); 
	}
	*/

	oUser->TellMe( sprintf("您的灵兽[%s]升至%s境界！当前技能栏增加到%d个", GetSummonSave( summid, K_SUMMON_NAME), "data/jingjie_data"->GetJingjieName(iRealm),mpSkillLimit[iRealm]) );
}

int SkillLevel(int summid, string skillid)
{
	mapping mpSkills;

	mpSkills = GetSummonSave(summid, M_SUMM_SKILLS); 

	return mpSkills[skillid];
}

// 添加技能
varargs int AddSkill(int summid, string skillid, int iLevel, int iPos)
{
	int uid ;
	string cGblKey;
	mapping mpSkill ;

	mpSkill = GetSummonSave(summid, M_SUMM_SKILLS);	

	assert( undefinedp(mpSkill[skillid]) && iLevel > 0);

	if ( !CanAddSkill(summid, skillid) )
		return 0;

	cGblKey = GetSummonSave(summid, K_GLOBAL_KEY);
	if ( cGblKey && sizeof(cGblKey) )
	{	
		logger->Log("summon %s addskill %s, level %d at pos %d.", cGblKey, skillid, iLevel, iPos);
		logger->DB(summon_skill_key, cGblKey, "", skillid);
	}	

	mpSkill[skillid] = iLevel;
	SetSummonSave(summid, M_SUMM_SKILLS, mpSkill);	

	if ( iPos ) GridSetSkill(summid, iPos, skillid);
	else GridAddSkill(summid, skillid);

	Import("SUMMON_UTIL")->CalSummon(summid);

	uid = GetSummonTemp(summid, KT_SUMMON_OWNER);

	Import("SUMMON_UTIL")->rpc_SummonInfo(uid, summid);
	Import("SUMMON_UTIL")->rpc_SummonElement(uid, summid);
#ifdef __SUMMON_SKILL_CACHE__	
	AllSkillGrids(summid);
#endif	
	return iLevel;
}

// 删除已有技能
varargs void DelSkill(int summid, string skillid, int IsPto)
{
	mapping mpSkill ;

	mpSkill = GetSummonSave(summid, M_SUMM_SKILLS);	
	map_delete(mpSkill, skillid);
	SetSummonSave(summid, M_SUMM_SKILLS, mpSkill);	
	logger->Log("summon %s delskill %s.", GetSummonSave(summid, K_GLOBAL_KEY), skillid);
	logger->DB(summon_skill_key, GetSummonSave(summid, K_GLOBAL_KEY), skillid, "");
	GridDelSkill(summid, skillid);

	if ( IsPto )
	{	
		Import("SUMMON_UTIL")->CalSummon(summid);
		Import("SUMMON_UTIL")->rpc_SummonInfo( GetSummonTemp(summid, KT_SUMMON_OWNER), summid);
	}
#ifdef __SUMMON_SKILL_CACHE__	
	AllSkillGrids(summid);
#endif	
	
}

// 对境界的限制 UserFunc
varargs int CanAddSkillLib(int summid, string skillid, int UserFunc)
{
	int iJingjie = 0;

	if ( !is_summon_online(summid) )
		return 0;

	if ( UserFunc )
	{
		int iOwner = GetSummonTemp(summid, KT_SUMMON_OWNER);
		assert(iOwner);
		object oUser = get_user(iOwner);
		assert(oUser);
		// 改成对人物的境界
		iJingjie = oUser->GetiJingjie();
	}
	else
	{
		iJingjie = GetSummonTemp(summid, KT_SUMMON_REALM);
	}
	// debug_message(sprintf("summid %d, skillid %s, %d<= %d\n", summid, skillid, mpRealmSkillTbl[skillid], iJingjie));
	return mpRealmSkillTbl[skillid] <= iJingjie;
}

int SkillRealm(string skillid)
{
	return mpRealmSkillTbl[skillid];
}

void OnAddSkillLibAch(object oUser, int iLevel, int iRealm)
{
	mapping arg = ([]);
	arg[AK_SUMMON_SKILLLIB_LEVEL] = iLevel;
	oUser->StatsGroupExec(ACH_SUMMON_SKILLLIB, 1, arg);

	arg = ([]);
	arg[AK_SUMMON_SKILLLIB_LEVEL] = iLevel;
	arg[AK_SUMMON_JINGJIE] = iRealm;

	oUser->StatsGroupExec(ACH_SUMMON_SKILLLLIB_REALM, 1, arg);
}

// 增添技囊的技能
varargs int AddSkillLib(int summid, string skillid, int UserFunc)
{
	string * Skills;
	string cTypeSkill;
	mapping mpTalentSkills;
	int iType, iAmount, iRace, iLevel, iBakLevel;

	Skills = GetSummonSave(summid, A_SUMM_SKILL_LIB);	

	if ( member_array(skillid, Skills) != -1 )
		return 0;

	if ( !CanAddSkillLib(summid, skillid, UserFunc) )
		return 0;

	Skills += ({ skillid });
	SetSummonSave(summid, A_SUMM_SKILL_LIB, Skills);	

	// 相应的 方向天赋 技能升级检测
	iAmount = sizeof(Skills);
	mpTalentSkills = GetSummonSave(summid, M_SUMM_TALENTSKILLS);
	iType = GetSummonSave(summid, K_SUMMON_TYPE);
	cTypeSkill = mpTypeTalentSkill[ mpSummonProp[iType][SUMMON_TYPICAL] ];
	iRace = mpSummonProp[iType][SUMMON_RACE];
	iBakLevel = mpTalentSkills[cTypeSkill];

	iLevel = 0;
	for ( int _iLevel=sizeof(mpRaceTalentUpgrade[iRace]); _iLevel >0; _iLevel -- )
	{
		// debug_message( sprintf("%d >= %d", iAmount, mpRaceTalentUpgrade[iRace][_iLevel]  ) );
		if ( iAmount >= mpRaceTalentUpgrade[iRace][_iLevel] )
		{
			iLevel = _iLevel;
			break;
		}
	}

	int iRealm = GetSummonTemp(summid, KT_SUMMON_REALM);
	//debug_message( sprintf("OnAddSkillLib: %s, iAmount %d, iLevel %d", skillid, iAmount, iLevel) );
	if ( iLevel > 0 && iBakLevel != iLevel ) // 天赋技能是否要升级？
	{
		int _owner = GetSummonTemp(summid, KT_SUMMON_OWNER);
		object _user = get_user(_owner);
		_user->TellMe( sprintf("灵兽#R%s#n的技囊技能达到%d个，#R%s#n技能升至%d级",  GetSummonSave(summid, K_SUMMON_NAME), iAmount, SkillName(cTypeSkill), iLevel) );
		mpTalentSkills[cTypeSkill] = iLevel;
		SetSummonSave(summid, M_SUMM_TALENTSKILLS, mpTalentSkills);
		Import("SUMMON_UTIL")->CalSummon(summid);
		Import("SUMMON_UTIL")->rpc_SummonInfo(_owner, summid);
		OnAddSkillLibAch(_user, iLevel, iRealm);
	}

	return 1;
}

int DelSkillLib(int summid, string skillid)
{
	string * Skills;

	Skills = GetSummonSave(summid, A_SUMM_SKILL_LIB);	

	if ( member_array( skillid, Skills) != -1 )
		return 0;

	Skills -= ({ skillid });
	SetSummonSave(summid, A_SUMM_SKILL_LIB, Skills);	

	return 1;
}

// 境界对应的技囊中技能数量
static mapping mpSkillLibInfo = ([
		JINGJIE_YUANSHENG : (["base": 2, "random": 0, ]), // 0的几率多一个技能
		JINGJIE_XUANZHAO : (["base": 2, "random": 0, ]), // 0的几率多一个技能
		JINGJIE_ZHUJI : (["base": 2, "random": 5, ]), // 1/5的几率
		JINGJIE_KAIGUANG : (["base": 2, "random": 5, ]), // 1/5的几率
		JINGJIE_LINGXU : (["base": 2, "random": 5, ]), // 1/5的几率
		]);

mapping GetRaceSkills(int iRace)
{
	return mpRaceSkills[iRace];
}

mapping GenSummonSkills(int iType)
{
	string * aSkillLib, * Skills = ({});
	mapping mpTalentSkills;
	string cSkill;
	mapping mpSkills = ([]), mpSkillRes = ([]);
	int iTotal, i, iRealm;
	int iRace;

	if (undefinedp(mpSummonProp[iType])) {
		debug_message(sprintf("没有相应的Type[%d]", iType));
		return ([]);
	}

	//debug_message( sprintf("mpRaceSkills %O", mpRaceSkills) );

	iRace = mpSummonProp[iType][SUMMON_RACE];
	iRealm = mpSummonProp[iType][KT_SUMMON_REALM];
	mpTalentSkills = mpSummonProp[iType][SUMMON_TALENT_SKILL];
	int _random = mpSkillLibInfo[iRealm]["random"];
	_random = _random ? (random(_random) == 0) : 0;
	iTotal = mpSkillLibInfo[iRealm]["base"] + _random;
	aSkillLib = ({});

	// 1个当前境界技能  公用、专精 任意
	cSkill = rand_array( mpRaceSkills[random(2)?0:iRace][iRealm] );
	aSkillLib += ({ cSkill });
	iTotal -= 1;
	/*
	// 1个当前境界-1的技能 公用、专精 任意
	for ( i=0; i < 1 && iTotal > 0; i++ )
	{
		cSkill = rand_array( mpRaceSkills[random(2)?0:iRace][iRealm>2?(iRealm-1):1] );

		if (member_array(cSkill, aSkillLib) != -1)
		{
			i -=1 ; // 如果技能库小于设定该有技能数，会死循环！
			continue;
		}	

		Skills += ({ cSkill }); // A全部出现，出现必定是1级
		aSkillLib += ({ cSkill });
		iTotal -= 1;
	}
	*/

	// 剩下的随机取:3境界的在1-2境界中随机取，4境界，在1-3境界取,2和1境界在1
	while ( iTotal > 0 )
	{
		cSkill = rand_array( mpRaceSkills[random(2)?0:iRace][ iRealm>2?random(iRealm-1)+1:1] );
		if (member_array(cSkill, aSkillLib) != -1)
		{
			// 如果技能库小于设定该有技能数，会死循环！
			continue;
		}
		aSkillLib += ({ cSkill });
		iTotal -= 1;
	}

	// 现在已经不激活当前技能了 20110-12-31
	/*
	// 设置当前技能：
	foreach ( cSkill in Skills )
	{
		mpSkills[cSkill] = 1; 
	}
	*/

#ifdef __DEBUG_SUM_SKILL__
	{
		foreach ( cSkill in Skills )
			if ( mpRaceSkillTbl[cSkill] != 0 && iRace != mpRaceSkillTbl[cSkill] )
			{
				debug_message( sprintf("\n\n\n\n\n\n\nERROR!!!%s 应该种族是 %d, 表中的种族: %d", cSkill, iRace, mpRaceSkillTbl[cSkill] ) );
			}

		foreach ( cSkill in aSkillLib )
			if ( mpRaceSkillTbl[cSkill] != 0 && iRace != mpRaceSkillTbl[cSkill] )
			{
				debug_message( sprintf("\n\n\n\n\n\n\nERROR!!!%s 应该种族是 %d, 表中的种族: %d", cSkill, iRace, mpRaceSkillTbl[cSkill] ) );
			}
	}	
#endif	

	mpSkillRes[A_SUMM_SKILL_LIB] = aSkillLib;
	mpSkillRes[M_SUMM_SKILLS] = mpSkills;
	mpSkillRes[M_SUMM_TALENTSKILLS] = mpTalentSkills;

	//debug_message( sprintf("mpSkillRes %O", mpSkillRes) );
	return mpSkillRes;
}

// 给灵兽初始化所有技能格子，并赋予技能
void InitSkills(int summid, mapping mpSkills)
{
	// 清空技能
	SetSummonSave(summid, M_SUMM_SKILLS, ([]) );
	SetSummonSave(summid, M_SUMM_SKILL_GRID, ([]) );
	// 设置技能
	foreach (string cSkill, int iLevel in mpSkills)
	{
		//debug_message( sprintf("%x add skill %s,%d", summid, cSkill, iLevel) );
		AddSkill(summid, cSkill, iLevel);	
	}	
}

// 洗灵兽的技能
int ResetSummonSkills(int summid)
{
	mapping mpSkillRes = ([]);
	int	iType = GetSummonSave(summid, K_SUMMON_TYPE);

	mpSkillRes = GenSummonSkills(iType);
	// 设置技能库
	SetSummonSave(summid, A_SUMM_SKILL_LIB, mpSkillRes[A_SUMM_SKILL_LIB]);
	InitSkills(summid, mpSkillRes[M_SUMM_SKILLS]);
	// 天赋技能 // 自带天赋技能
	SetSummonSave(summid, M_SUMM_TALENTSKILLS, mpSkillRes[M_SUMM_TALENTSKILLS]);
}

int *GetGridCanWield(string cSkill, int iSummonRealm)
{
	int iRealm = mpRealmSkillTbl[cSkill];
	if(undefinedp(mpRealmPos[iRealm]))
	{
		return ({});
	}
	else
	{
		int *aGrids = ({});
		for(int i = iRealm; i <= iSummonRealm; ++i)
		{
			aGrids += mpRealmPos[i];
		}
		return aGrids;
	}
}


// 技能学习逻辑
// 2011年12月29日之前的逻辑，之前是随机上技能，现在是启用wield_skill的逻辑
int DoLearnSkill(object oUser, int summid, int iPos, int IsSameRealm)
{	
	mapping SkillGrids;
	string skill_to_del = NULL_KEY;
	int iGridRealm;
	string * SkillLib, * AllSkills, * ExcludeSkils = ({}) ;
	string cAddSkill;
	int uid = oUser->GetId();
	
	iGridRealm = mpPosRealm[iPos];
	if ( ! iGridRealm )
	{
		oUser->TellMe("位置不对？");
		return 0;
	}

	if ( iGridRealm > GetSummonTemp(summid, KT_SUMMON_REALM) )
	{
		oUser->TellMe("灵兽境界没有达到学习此格技能要求。");
		return 0;
	}

	SkillGrids = AllSkillGrids(summid);

	if (SkillGrids[iPos])// 替换旧技能
	{
		skill_to_del = SkillGrids[iPos]["skill"];
		if ( SkillLevel(summid, skill_to_del) < 1 )
		{
			oUser->TellMe("您的灵兽没有此技能");	
			return 0;
		}	
	}
	else // 新加技能的逻辑
	{
		// 判断技能个数
		if ( IsSkillFull(summid))
		{
			oUser->TellMe("此灵兽不能再增加技能了。");	
			return 0;
		}
	}

	// 取出所有适合该技能囊的技能
	SkillLib = filter_array(GetSummonSave(summid, A_SUMM_SKILL_LIB), function(string _skill, int _realm, int _samerealm) { return _samerealm? mpRealmSkillTbl[_skill] == _realm : mpRealmSkillTbl[_skill] <= _realm; }, iGridRealm, IsSameRealm); 

	//debug_message( sprintf("SkillLib is %O", SkillLib) );

	SkillLib = shuffle(SkillLib);
	AllSkills = keys(GetSummonSave(summid, M_SUMM_SKILLS));
	foreach ( string cSkill in SkillLib )
	{
		if ( SkillLevel(summid, cSkill) >= 1 )
			continue;

		//新的技能不会超过该灵兽的境界；
		//不会随机到替换技能之外的已有技能，或其高、低级版的技能；
		//有技能互斥关系也不能添加，但能添加技能自己的高低级
		if ( ! CanAddSkill(summid, cSkill, ExcludeSkils) )
			continue;

		if ( cSkill == skill_to_del )
			continue;

		cAddSkill = cSkill;
		break;
	}

	if ( !cAddSkill || cAddSkill == NULL_KEY )
	{
		oUser->TellMe("没有合适的技能可以进行替换");
		return 0;
	}	

	logger->Log("%d's %s summon, learn %s skill, lost %s skill.", GetSummonTemp(summid, KT_SUMMON_OWNER), GetSummonSave(summid, K_GLOBAL_KEY), cAddSkill, skill_to_del); 
	logger->DB(summon_skill_key, GetSummonSave(summid, K_GLOBAL_KEY), skill_to_del, cAddSkill);

	if ( skill_to_del != NULL_KEY )
	{	
		DelSkill(summid, skill_to_del);
		oUser->TellMe( sprintf("您的灵兽忘记了%s技能。", SkillName(skill_to_del)) );
	}

	AddSkill(summid, cAddSkill, 1, iPos);
	oUser->TellMe( sprintf("您的灵兽学会了%s技能。", SkillName(cAddSkill)) );

	return 1;
}

int DoWieldSkill(object oUser, int summid, int IsOn, string cSkill)
{
	mapping SkillGrids;
	int iGridRealm;
	string * SkillLib = ({}) ;
	int uid = oUser->GetId();
	int iSummmonRealm = GetSummonTemp(summid, KT_SUMMON_REALM);
	// 删掉skill
	if ( !IsOn )
	{
		if ( SkillLevel(summid, cSkill) < 1 )
		{
			oUser->TellMe("#63奇怪，你这个技能没有激活，怎么能取消激活呢？");
			return 0;
		}
		logger->Log("%d's %s summon, unload %s skill.", GetSummonTemp(summid, KT_SUMMON_OWNER), GetSummonSave(summid, K_GLOBAL_KEY), cSkill); 
		logger->DB(summon_skill_key, GetSummonSave(summid, K_GLOBAL_KEY), "Del", cSkill);
		DelSkill(summid, cSkill, 1);
		oUser->TellMe(sprintf("成功取消激活%s技能", SkillName(cSkill)));
		return 1;
	}	

	if ( SkillLevel(summid, cSkill)  >= 1) 
	{
		oUser->TellMe("灵兽已经学会此技能了。");	
		return 0;
	}

	iGridRealm = mpRealmSkillTbl[cSkill];
	if ( iGridRealm > iSummmonRealm)
	{
		oUser->TellMe("灵兽境界没有达到学习此格技能要求。");
		return 0;
	}

	if ( ! CanAddSkill(summid, cSkill) )
	{
		oUser->TellMe("由于技能高低级互斥关系，此灵兽不能学习此技能。");	
		return 0;
	}

	SkillLib = GetSummonSave(summid, A_SUMM_SKILL_LIB); 
	if ( member_array(cSkill, SkillLib) == -1 )
	{
		oUser->TellMe("此灵兽还未融合学会该技能。");
		return 0;
	}

	if ( IsSkillFull(summid) )
	{
		oUser->TellMe("适合该技能的技能栏已经全部激活，请先取消激活一个该境界技能");
		return 0;
	}

	SkillGrids = AllSkillGrids(summid);
	int *aGridCanWield = GetGridCanWield(cSkill, iSummmonRealm);
	// 取出所有适合该技能囊的技能
	aGridCanWield = filter_array(aGridCanWield, function(int iPos, mapping mSummonSkills) { return undefinedp(mSummonSkills[iPos]); }, SkillGrids); 

	//可用的技能格子已经满了
	if(sizeof(aGridCanWield) == 0)
	{
		oUser->TellMe("适合该技能的技能栏已经全部激活，请先取消激活一个该境界技能");
		return 0;
	}
	
	//取出第一个空置的格子放进技能
	int iPos = aGridCanWield[0];

	logger->Log("%d's %s summon, learn %s skill.", GetSummonTemp(summid, KT_SUMMON_OWNER), GetSummonSave(summid, K_GLOBAL_KEY), cSkill); 
	logger->DB(summon_skill_key, GetSummonSave(summid, K_GLOBAL_KEY), "Add", cSkill);

	AddSkill(summid, cSkill, 1, iPos);
	oUser->TellMe( sprintf("您的灵兽成功激活了%s技能。", SkillName(cSkill)) );
	return 1;
}




// 获取召唤兽当前能够获得的技能序列
mixed *GetAllSummonSkills(int iSummon)
{
	int iSummRealm, iRealm, iRace, iSummType;
	int _realm;
	mixed *lsAllSkills;
	mixed *lsRaceSkills;

	iSummType = GetSummonSave(iSummon, K_SUMMON_TYPE);

	if (undefinedp(mpSummonProp[iSummType])) {
		debug_message(sprintf("没有相应的Type[%d]", iSummType));
		return ({});
	}

	iSummRealm = GetSummonTemp(iSummon, KT_SUMMON_REALM);
	iRace = mpSummonProp[iSummType][SUMMON_RACE];
	
	lsAllSkills = ({});
	_realm = 1;

	while (_realm <= iSummRealm) { 
		// 获得公用技能
		lsRaceSkills = mpRaceSkills[0][_realm];
		lsAllSkills += lsRaceSkills;
		// 获得专精技能
		lsRaceSkills = mpRaceSkills[iRace][_realm];
		lsAllSkills += lsRaceSkills;
		_realm++;
	}

	return lsAllSkills;
}

// 使用秘录
int DoUseMiLu(object oUser, int iItem, int iTarget)
{
	int iOwner, iSkillID, iType;
	mixed *lsAllSkills;
	mixed *MainSkillLib;
	string cSkillId;

	if (iTarget == 0 || !is_summon_online(iTarget))
	{
		oUser->TellMe("此物品只能对灵兽使用");
		return 0;
	}

	iOwner = GetSummonTemp(iTarget, KT_SUMMON_OWNER);
	if (iOwner != oUser->GetId()) {
		oUser->TellMe("此物品只能对自己的灵兽使用");
		return 0;
	}


	lsAllSkills = GetAllSummonSkills(iTarget);
	// 去掉已经学习上的技囊
	MainSkillLib = GetSummonSave(iTarget, A_SUMM_SKILL_LIB);

	lsAllSkills -= MainSkillLib;

	if (!sizeof(lsAllSkills)) {
		oUser->TellMe("你的灵兽无法领悟到新的技能!");
		return 0;
	}
	cSkillId = rand_array(lsAllSkills);

	//debug_message(sprintf("skill lib: %O, %O, %O, %s", GetSummonSave(iTarget, M_SUMM_SKILLS), lsAllSkills, MainSkillLib, cSkillId));

	if (!AddSkillLib(iTarget, cSkillId)) {
		oUser->TellMe("无法添加新的技能!");
		return 0;
	}

	// AddSkill(iTarget, cSkillId, 1, 0);
	oUser->TellMe( sprintf("您的灵兽学会了%s技能。", SkillName(cSkillId)) );

	logger->Log("[%d] use milu %s skill:%s, %s", oUser->GetId(), GetSummonSave(iTarget, K_GLOBAL_KEY), cSkillId, SkillName(cSkillId));
	Import("SUMMON_UTIL")->rpc_SummonSkill(oUser->GetId(), iTarget);
	return 1;
	// varargs int AddSkill(int summid, string skillid, int iLevel, int iPos)
}

string GetTypeTalentSkill( int iType )
{
	int iTypical = mpSummonProp[iType][SUMMON_TYPICAL];
	return mpTypeTalentSkill[iTypical];
}

int GetTypeTalentSkillUpgradeAmount( int iType, int iLevel )
{
	int iRace = mpSummonProp[iType][SUMMON_RACE];
	return mpRaceTalentUpgrade[iRace][iLevel];
}
#endif
