// 用户数据文件
// ------------------------------------------------------
// 本文件将只存放一个用户的数据
// 对数据测操作，请参照char/user_op.c
// 引擎中会存贮用户数据，脚本管理char/user_op实例
// 在char/user_op中会缓存用户数据的实例
// ------------------------------------------------------

static object logger;
void setProp(mixed key, mixed value);
varargs mixed getTemp(mixed key, mixed defValue);
void setTemp(mixed key, mixed defValue);
int getIcon(); 
mapping getDayData();
int upGrades();
int calNextExp();
int getGrade();
int getExp();
void addExp(int expAdded);
int getHP();
private void onUpOneGrade();
private void onUpGrades();
int getBaseProp(int grade);
int getBuzhengNaili(int grade);
int getNaili();
int getFujiaNaili();
void restoreAllTempAttr();

#include <macros.h>
// 数据include from user_data
#include <user_data.h>
#include <user_key.h>
#include <debug.h>
#include "/char/item_op.h"
#include "/char/summon_op.h"
#include "/rc/rpc/user_cmd.h"
#include "/module/fight/fight.h"
#include <frames.h>
#include "/char/friend_op.h"
#include "/char/zhaocaimao_op.h"
//////////////////////////////////////////////////////////
//返回值声明区
static int GRADE_UP=1;
static int GRADE_STAY=0;
#define DAY_TILI 100
//////////////////////////////////////////////////////////

void tellTips(string format, mixed *args...);

int getCash();
int getTotalYB();
int getId();
int getTili();
void setTili(int tili);
string getTitle();
static void releaseResource();

string getTitle()
{
	return "二百五";
}

int getTili()
{
	mapping dayData = getDayData();
	if (undefinedp(dayData[K_USER_TILI]))
	{
		dayData[K_USER_TILI] = DAY_TILI;	
	}
	return dayData[K_USER_TILI];
}

void setTili(int tili)
{
	if (tili < 0)
		return;
	mapping dayData = getDayData();
	dayData[K_USER_TILI] = tili;
}


void giveTestSummons()
{
	int* testSummonTypes = ({20001, 20002, 20003,20004, 20005, 20006 });
	foreach (int summonType in testSummonTypes)
	{
		int summonId = Import("SUMMON_UTIL")->addNewSummonByType(this_object(), summonType);
		//debug_message(sprintf("givetestsummon id=%d", summonId));
	}
}

void giveTestItems()
{
	Import("ITEM_UTIL")->addNewItem(this_object(), "ZH0001", 3, ITEM_BAG_NORMAL);
	Import("ITEM_UTIL")->addNewItem(this_object(), "EQ0001", 6, ITEM_BAG_NORMAL);
	delSameTypeItemAmount("ZH0001", 1, ITEM_BAG_NORMAL);
	debug_message("item same type amount:%d", getSameTypeItemAmount("ZH0001", ITEM_BAG_NORMAL));
}

static int checkUpGrade();

void RestoreAllItem();
void MoveItem(int itemId, int badId, int pos);


int getHeroFightPos()
{
	return prop[K_USER_FIGHT_POS];
}

void setHeroFightPos(int pos)
{
	prop[K_USER_FIGHT_POS] = pos;
}

void initDefaultSkill()
{
	prop[K_USER_SKILL_LIST] = ({"zhanji"});
	prop[K_USER_SKILL] =  "zhanji";
}

string getSkill()
{
	return prop[K_USER_SKILL]; 
}

string getNormalAttack()
{
	int icon = getIcon();
	debug_message("get normaAttack icon:%d", icon);
	
	return "yuanchengwuli";
	return "data/user/init_attr"->get_data()[icon][K_USER_NORMAL_ATTACK];
}

void restoreAllSummon()
{
	//restore Summon
	//add summon to frames
	//send info to client
	foreach(int bagId, mapping mpSummons in summon) 
	{
		foreach( int pos, mapping mpSave in mpSummons) 
		{
			int summonId;
			
			if (catch(summonId = Import("SUMMON_UTIL")->restoreSummon(mpSave)) != 0) {
				//debug_message(sprintf("[%d] pos[%d] load summon error: %O", usernum, pos, mpSave));
				continue;
			}

			assert(summonId > 0);
/*

			if (ItemIsTimeOut( ItemId ))
			{
				//DestroyItem( ItemId );
				continue;
			}
*/
			addRestoreSummon(summonId, bagId, pos);
			addSummonType(mpSave[K_SUMMON_TYPE], summonId);
		}
	}
}

void initSummonEquipFrames()
{
	foreach(int bagId, mapping mpSummons in summon) 
	{
		foreach( int pos, mapping mpSave in mpSummons) 
		{
			if (!mpSave[K_SUMMON_EQUIP_BAGID]) {
				int equipBagId = createSummonEquipFrames(mpSave[K_GLOBAL_KEY]);
				if (!equipBagId) {
					//debug_message("[%d] 灵兽创建装备栏失败 %s", usernum, mpSave[K_GLOBAL_KEY]);
					return 0;
				}
				mpSave[K_SUMMON_EQUIP_BAGID] = equipBagId;
			}
			else {
				initItemFrame(mpSave[K_SUMMON_EQUIP_BAGID]);
			}
		}
	}
}

varargs mixed getProp(mixed key, mixed defValue)
{
	mixed v = prop[key];

	if (undefinedp(v) && !undefinedp(defValue)) {
		prop[key] = defValue;
		return defValue;
	} else {
		return v;
	}
}

void setProp(mixed key, mixed value)
{
	prop[key] = value;
}

void delProp(mixed key)
{
	map_delete(prop, key);	
}

void setTemp(mixed key, mixed val)
{
	temp_var[key] = val;
	return;
}

void delTemp(mixed key)
{
	map_delete(temp_var, key);
}

varargs mixed getTemp(mixed key, mixed defValue)
{
	mixed v = temp_var[key];

	if (undefinedp(v) && !undefinedp(defValue)) {
		temp_var[key] = defValue;
		return defValue;
	} else {
		return v;
	}
}


void RestoreAllItem()
{
	//debug_message("enter Restore All Item");
	foreach(int bagId, mapping mpItems in item) 
	{
		foreach( int iPos, mapping mpSave in mpItems) 
		{
			int ItemId;
			
			if (catch(ItemId = Import("ITEM_UTIL")->restoreItem(mpSave)) != 0) {
				//debug_message(sprintf("[%d] pos[%d] load item error: %O", usernum, iPos, mpSave));
				continue;
			}

			assert(ItemId > 0);
/*

			if (ItemIsTimeOut( ItemId ))
			{
				//DestroyItem( ItemId );
				continue;
			}
*/
			//debug_message("restoreOneItem Item: %d, %d, %d", ItemId, bagId, iPos);
			AddRestoreItem(ItemId, bagId, iPos);
		}
	}
}
void initFellowFightPlate()
{
}

string getName()
{
	return prop[K_USER_NAME];
}

int getCash()
{
	return prop[K_USER_CASH];
}

int getBangDingYB()
{
	return prop[K_USER_BANGDING_YB];
}

int getUnBangDingYB()
{
	return prop[K_USER_UNBANGDING_YB];
}

void setBangDingYB(int yb)
{
	if(yb < 0)
		return;
	prop[K_USER_BANGDING_YB] = yb;
}

void setUnBangDingYB(int yb)
{
	if(yb < 0)
		return;
	prop[K_USER_UNBANGDING_YB] = yb;
}

int getTotalYB()
{
	return getBangDingYB() + getUnBangDingYB();
}

/*
@return -1 yb值小于=0 或不够减 >= 0 为剩余的元宝数
*/
int minusYB(int yb)
{
		
	int bangdingYB = getBangDingYB();	
	int unBangdingYB = getUnBangDingYB();
	int totalYB = bangdingYB + unBangdingYB;
	if (yb <= 0 || yb > totalYB)
		return -1;

	int bangdingYBLeft = bangdingYB - yb;
	int unBangdingYBNeed = yb - bangdingYB;
	unBangdingYBNeed = unBangdingYBNeed > 0 ? unBangdingYBNeed : 0;
	bangdingYBLeft = bangdingYBLeft <= 0 ? 0 : bangdingYBLeft;
	int unBangdingYBLeft = unBangdingYB - unBangdingYBNeed;
	setBangDingYB(bangdingYBLeft);
	setUnBangDingYB(unBangdingYBLeft);
	int totalLeft = bangdingYBLeft + unBangdingYBLeft;
	//todo send protocal
	"char/user_cmd"->sendYuanbaoInfo(this_object());
	return totalLeft;
}

int addCash(int cashAdd)
{
	if (cashAdd <= 0)
		return 0;

	int lastCash = getCash();

	if (MAX_CASH - cashAdd < prop[K_USER_CASH])
	{
		tellTips("您的钱数已达到上限");
		prop[K_USER_CASH] = MAX_CASH;
	}
	else
	{
		prop[K_USER_CASH] += cashAdd;
	}
	"char/user_cmd"->sendCashInfo(this_object());

	return (prop[K_USER_CASH] - lastCash);
}

// 扣除金钱
int subCash(int cashSub)
{
	if (cashSub <= 0)
		return 0;

	int curCash = getCash();
	int lastCash = curCash;

	curCash -= cashSub;

	curCash = curCash < 0 ? 0 : curCash;

	prop[K_USER_CASH] = curCash;

	"char/user_cmd"->sendCashInfo(this_object());
	return (lastCash - curCash);
}

int hasCash(int cash)
{
	return (prop[K_USER_CASH] >= cash);
}

void setName(string name)
{
	prop[K_USER_NAME] = name;
}


int getGrade()
{
	return prop[K_USER_GRADE];
}

int getId()
{
	return usernum;
}


int getMaxHP()
{

	return temp_att[KT_USER_FIGHT_FINAL][KT_USER_MAX_HP];
}


int getDefence()
{
	return temp_att[KT_USER_FIGHT_FINAL][KT_USER_DEFENCE];
}



int getQiangfa()
{
	return temp_att[KT_USER_FIGHT_FINAL][KT_USER_QIANGFA];
}


int getKangfa()
{
	return temp_att[KT_USER_FIGHT_FINAL][KT_USER_KANGFA];
}


int getSpeed()
{
	return temp_att[KT_USER_FIGHT_FINAL][KT_USER_SPEED];
}

int getZhili()
{
	return temp_att[KT_USER_FIGHT_BASE][KT_USER_ZHILI];
}

int getLiliang()
{
	return temp_att[KT_USER_FIGHT_BASE][KT_USER_LILIANG];
}

int getMinjie()
{
	return temp_att[KT_USER_FIGHT_BASE][KT_USER_MINJIE];
}

int getNaili()
{
	return temp_att[KT_USER_FIGHT_BASE][KT_USER_NAILI];
}

private mapping calculateBaseAttr_()
{
	int grade = getGrade();
	int growRate = 10;
	mapping baseAttr = ([]);
	baseAttr[KT_USER_NAILI] =  to_int(pow(grade,1.2)/2.5) + 25;
	baseAttr[KT_USER_LILIANG] =  to_int(pow(grade,1.2)/2.5) + 25;
	baseAttr[KT_USER_ZHILI] =  to_int(pow(grade,1.2)/2.5) + 25;
	baseAttr[KT_USER_MINJIE] =  to_int(pow(grade,1.2)/2.5) + 25;
	return baseAttr;
}

int getFightAttr(string key)
{
	return temp_att[KT_USER_FIGHT_FINAL][key];
}

void sendHeroSpeciInfo()
{
	"char/user_cmd"->rpc_server_hero_specinfo(this_object());
}

static void calculateJingyingAttr(mapping finalAttr)
{
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

void calculateAllAttr()
{
	/*
	//主属性 
	mapping baseAttr = ([]);
	
	mapping equipAttr = ([]);

	// 力量，敏捷
	mapping total = ([]);


	// 计算攻击，速度.. 速度百分比

	*/
	temp_att = ([]);
	mapping baseAttr = calculateBaseAttr_();
	mapping equipAttr = ([]);
	mapping finalAttr = ([]);


	mapping equipBagAttr = "module/equip/main"->getBagEquipAttr(this_object(), ITEM_BAG_EQUIP);

	baseAttr[KT_USER_NAILI] += equipBagAttr[NAILI];
	baseAttr[KT_USER_LILIANG] += equipBagAttr[LILIANG];
	baseAttr[KT_USER_ZHILI] += equipBagAttr[ZHILI];
	baseAttr[KT_USER_MINJIE] += equipBagAttr[MINJIE];

	baseAttr[KT_USER_NAILI] = to_int(baseAttr[KT_USER_NAILI] * (100 + equipBagAttr[NAILI_RATE]) / 100);
	baseAttr[KT_USER_LILIANG] = to_int(baseAttr[KT_USER_LILIANG] * (100 + equipBagAttr[LILIANG_RATE]) / 100);
	baseAttr[KT_USER_ZHILI] = to_int(baseAttr[KT_USER_ZHILI] * (100 + equipBagAttr[ZHILI_RATE]) / 100);
	baseAttr[KT_USER_MINJIE] = to_int(baseAttr[KT_USER_MINJIE] * (100 + equipBagAttr[MINJIE_RATE]) / 100);

	// 装备属性计算
	equipAttr[KT_USER_MAX_HP] = equipBagAttr[FT_HP];
	equipAttr[KT_USER_DEFENCE] = equipBagAttr[FT_DEFENCE];
	equipAttr[KT_USER_ATTACK] = equipBagAttr[FT_ATTACK];
	equipAttr[KT_USER_QIANGFA] = equipBagAttr[FT_QIANGFA];
	equipAttr[KT_USER_KANGFA] = equipBagAttr[FT_KANGFA];
	equipAttr[KT_USER_SPEED] = equipBagAttr[FT_SPEED];

	finalAttr[KT_USER_MAX_HP] = equipAttr[KT_USER_MAX_HP] + to_int(baseAttr[KT_USER_NAILI] * 5 + 0.5);
	finalAttr[KT_USER_DEFENCE] = equipAttr[KT_USER_DEFENCE] + to_int(baseAttr[KT_USER_LILIANG] * 0.5 + 0.5);
	finalAttr[KT_USER_ATTACK] = equipAttr[KT_USER_ATTACK] + to_int(baseAttr[KT_USER_LILIANG] * 1.5 + 0.5);
	finalAttr[KT_USER_QIANGFA] = equipAttr[KT_USER_QIANGFA] + to_int(baseAttr[KT_USER_ZHILI] * 1.5 + 0.5);
	finalAttr[KT_USER_KANGFA] = equipAttr[KT_USER_KANGFA] + to_int(baseAttr[KT_USER_ZHILI] * 0.5 + 0.5);
	finalAttr[KT_USER_SPEED] = equipAttr[KT_USER_SPEED] + to_int(baseAttr[KT_USER_MINJIE] * 1.2 + 0.5);

	//精英属性计算
	calculateJingyingAttr(finalAttr);
	
	temp_att[KT_USER_FIGHT_BASE] = baseAttr;
	temp_att[KT_USER_FIGHT_FINAL] = finalAttr;
	prop[K_USER_HP_KEY] = finalAttr[KT_USER_MAX_HP];
}

int getAttack()
{
	return temp_att[KT_USER_FIGHT_FINAL][KT_USER_ATTACK];
}

int getHeroFightMatrixPos()
{
	return prop[K_USER_FIGHT_POS];
}

void initHeroFightPos()
{
	if (undefinedp(prop[K_USER_FIGHT_POS]))
		prop[K_USER_FIGHT_POS] = 5;
}

int getIcon() 
{
	return prop[K_USER_ICON];
}

int getLayout()
{
	return prop[K_USER_LAYOUT];
}

void setLayout(int v)
{
	prop[K_USER_LAYOUT] = v;
}

void setPosition(int x, int y)
{
	prop[K_USER_X] = x;
	prop[K_USER_Y] = y;
}

int * getPosition()
{
	return ({prop[K_USER_X], prop[K_USER_Y]});
}


int enterScene(int sceneId, int x, int y)
{
	return Import("SCENE")->playerEnter(this_object(), sceneId, x, y);
}

int leaveScene()
{
	return Import("SCENE")->removeUser(this_object());
}

//强制改变位置，sceneId应该为实场景的地图id
void setLocation(int sceneId, int x, int y)
{
	prop[K_USER_Z] = sceneId;
	setPosition(x, y);
}

int * getLocation()
{
	return ({prop[K_USER_Z], prop[K_USER_X], prop[K_USER_Y]});
}

void setDefaultLocation()
{
	setLocation(1001, 600, 500);
}

static void sendLoginInfo()
{
	"char/user_cmd"->new_hero(this_object());
	"char/user_cmd"->sendYuanbaoInfo(this_object());
	sendHeroSpeciInfo();
	sendAllItems();
	sendAllSummons();
	sendFightMatrixInfo();
}

void setup(int uid)
{
	usernum = uid;

	initItemFrames();
	initSummonEquipFrames();
	RestoreAllItem();

	initHeroFightPos();
	initSummonGlobalKeyMap();
	initSummonFrames();
	restoreAllSummon();

	initFightMatrix();

	//初始化玩家战斗位置
	calculateAllAttr();

}

void test(object user)
{
	"module/fight/main"->test(user);
}

static void commonLoginCheck(object user, int relogin)
{
	// 任务模块会影响NPC的显示，所以要先于场景模块的loginCheck
	Import("MISSION_MAIN")->loginCheck(user);
	Import("SCENE")->loginCheck(user);
	Import("FRIEND")->loginCheck(user);
	Import("ZHAOCAIMAO")->loginCheck(user);
	Import("DAZUO")->loginCheck(user);
	Import("EQUIP")->loginCheck(user);
}

static void firstLoginCheck(object user)
{
        commonLoginCheck(user, 0);
        Import("MISSION_MAIN")->firstLogin(user);
}

void login()
{
	sendLoginInfo();
	commonLoginCheck(this_object(), 0);
}

void reLogin()
{
	sendLoginInfo();
	commonLoginCheck(this_object(), 1);
}

static void Logout()
{
	"char/user_cmd"->logoutCheck(this_object());
	Import("SCENE")->removeUser(this_object());
        releaseResource();
}

//call back by engine
void logout()
{
	Logout();
}

void onUserLoad(int uid)
{
	setup(uid);
	login();
}

static void init(int uid, int icon, string name)
{
	usernum = uid;
	prop[K_USER_NAME] = name;
	prop[K_USER_ICON] = icon;
	prop[K_USER_LAYOUT] = icon;
	prop[K_USER_GRADE] = 1;
	prop[K_USER_CASH] = 0;
	setDefaultLocation();
	initDefaultSkill();

	prop[K_USER_CREATED_TIME] = time();
}

void born(int uid, int icon, string name)
{
        //debug_message("new user born, uid:%d, icon:%d, name:%s", uid, icon, name);
	init(uid, icon, name);
	setup(uid);
	sendLoginInfo();
	Import("DB")->new_user(this_object());
	firstLoginCheck(this_object());
}

string getUMS()
{
        return "jun37213721";
}

mapping getMission()
{
        return mission;
}

// ------------------------------------------------------------------------------


int getExp()
{
	return prop[K_USER_EXP];
}

int getHP()
{
	return to_int(prop[K_USER_HP_KEY]);
}

int getGradeUpExp(int iGrade)
{
	mapping gradeExpDict =  "data/user/exp"->get_data();
        return gradeExpDict[iGrade]["expr"];
}

int getMaxExp(int grade)
{
	return getGradeUpExp(grade);
}

int checkUpGrade()
{
	int grade = getGrade();
        int exp = getExp();
        int gradeUpExp = getGradeUpExp(grade);
	if (exp >= gradeUpExp)
		return GRADE_UP;
	return GRADE_STAY;
}

void addExp(int expAdded)
{
	if (expAdded <= 0)
		return;
	prop[K_USER_EXP] += expAdded;	
	upGrades();
	"char/user_cmd"->sendExpInfo(this_object());
	return;
}

private void onUpOneGrade()
{
	int oldGrade = prop[K_USER_GRADE];
	int newGrade = prop[K_USER_GRADE] = oldGrade + 1;
	int exp = getExp();
	int oldGradeMaxExp = getMaxExp(oldGrade);
	int newExp = exp - oldGradeMaxExp;
	if (newExp < 0)
		return;
	prop[K_USER_EXP] = newExp;
	//restoreAllTempAttr();
	prop[K_USER_HP_KEY] = getMaxHP();
	class HeroGradeUpInfo gradeUpInfo = new(class HeroGradeUpInfo);
	gradeUpInfo->hp = gradeUpInfo->maxHP = getMaxHP();
	gradeUpInfo->grade = newGrade;
	calculateAllAttr();	
	sendHeroSpeciInfo();
	rpc_client_hero_gradeup(usernum, gradeUpInfo);

	//更新战阵大小
	sendMaxFightAmount();
}

private void onUpGrades()
{

}

int upGrades()
{
	int upGradeFlag = 0;
	while(1)
	{
		if (checkUpGrade() == GRADE_UP)
		{
			upGradeFlag = 1;
			onUpOneGrade();
		}
		else
			break;
	}
	if (upGradeFlag)
	{
		onUpGrades();
		return GRADE_UP;
	}
	return GRADE_STAY;
}

// 提示频道
void tellTips(string format, mixed *args...)
{
	// TODO: 暂时用系统频道
	
	string msg = "";
	if (sizeof(args)) {
		msg = sprintf(format, args...);	
	} else {
		msg = format;
	}
	"module/chat"->tips(usernum, "", msg);
}

void tellMe(string format, mixed *args...)
{
	string msg = "";
	if (sizeof(args)) {
		msg = sprintf(format, args...);	
	} else {
		msg = format;
	}
	"module/chat"->tellMe(usernum, msg);
}

static void releaseResource()
{
        Import("DB")->unload_user(this_object());

	/*
        DestSummonFrames();
        DestItemFrames();
        RemoveFollowNpcs();
	*/
}

mapping getEquip()
{
	if (undefinedp(prop[K_USER_EQUIP])) {
		prop[K_USER_EQUIP] = ([]);
	}
	return prop[K_USER_EQUIP];
}

mapping getFuben()
{
	if (undefinedp(prop[K_USER_FUBEN])) {
		prop[K_USER_FUBEN] = ([]);
	}
	return prop[K_USER_FUBEN];
}

mapping getFubenStarMap()
{
	mapping fubenData = getFuben();
	if (undefinedp(fubenData[K_USER_FUBEN_STAR]))
	{
		fubenData[K_USER_FUBEN_STAR] = ([]);
	}
	return fubenData[K_USER_FUBEN_STAR];
}

int getFubenStar(int fubenId)
{
	return getFubenStarMap()[fubenId];
}

void setFubenStar(int fubenId, int stars)
{
	int oldStars = getFubenStar(fubenId);
	mapping fubenStarMap = getFubenStarMap();
	if (stars > oldStars)
	{
		fubenStarMap[fubenId] = stars;
	}
	debug_message("set fuben star uid:%d fubenId:%d stars:%d oldStars:%d fubenStarMap:%O", getId(), fubenId, stars, oldStars, fubenStarMap);
}

mapping getDayData()
{
	int dayno = get_rela_day_no();
	if (prop[K_USER_DAY_DATE] != dayno) {
		prop[K_USER_DAY_DATE] = dayno;
		prop[K_USER_DAY_DATA] = ([]);
	}
	return prop[K_USER_DAY_DATA];
}

void setFightValue(string attr, int value)
{
	temp_att[KT_USER_FIGHT_FINAL][attr] = value;
	"char/user_cmd"->sendAttrUpdate(this_object(), attr, value);
}

void setvLogin()
{
	temp_var[KT_USER_VLOGIN] = 1;
}

void isvLogin()
{
	return temp_var[KT_USER_VLOGIN];
}
