#include "base.h"


/*
复用FUBEN_BASE模块的玩法模块 一定要提供onFightEnd(object user, class Fight fight, int win, int exp, int cash, mapping itemMap)接口
并在填副本表时填入该字段
*/

static object logger;
static mapping fubenDataMap;
static mapping mingLeiDataMap;
static mapping numericDataMap;
static mapping layoutDataMap;
static mapping rewardDataMap;
static mapping triggerDataMap;



//副本BASE对外提供的接口
int enterFuben(object user, int fubenId);

void exitFuben(object user);

int getFubenId(object user);

class MingLei getNextMingLei(object user);

int isDone(object user);

void fight(object user, class MingLei mingLei);

int getFubenRewardExp(int fubenId);

int getFubenRewardCash(int fubenId);

mapping getFubenRewardItems(int fubenId);

void onPlayerEnterScene(object user);

//副本BASE对外接口 END

//内部功能函数声明 END
static void onFightEnd(class Fight fight, int uid, class MingLei trap);

static void walk(object user);

static void fightMingLei(object user, int fubenId, int mingLeiId);

static void initMingLeiData();

static mapping* initMingLei(string fubenName);

static int sortMingLei(mapping a, mapping b);

static mixed *defenseLayout(string layoutName);

static mapping getNpcNumeric(int key);

static void addClientNpc(object user, class Fuben fuben);
//内部功能函数声明 END

int isDone(object user)
{
	if (getFubenId(user) && !getNextMingLei(user))
		return 1;
	return 0;
}

static void runAction(object user, mixed * action)
{
	if (undefinedp(action)) {
		return;
	}

	string name = action[0];
	switch (name) {
	case FB_ACTION_PLAY:
		string dramaId = action[1];
		logger->Log("run action play drama uid=%d,dramaId=%s", user->getId(), dramaId);
		"module/client_drama"->testPlay(user, dramaId);
		break;
	default:
	}
}

static void fireEnterFubenTrigger(object user, class Fuben fuben)
{
	mixed *triggers = triggerDataMap[fuben->id];
	foreach (mapping trigger in triggers) {
		if (trigger[FB_FIRE] == FB_FIRE_START) {
			runAction(user, trigger[FB_ACTION]);
		}
	}
}

static void fireFightEndTrigger(object user, class Fuben fuben, string fightResult, int fightCnt)
{
	mixed *triggers = triggerDataMap[fuben->id];
	foreach (mapping trigger in triggers) {
		if (trigger[FB_FIRE] == FB_FIRE_FIGHT_END) {
			mixed* fireArg = trigger[FB_FIRE_ARG];
			//debug_message("fightEnd fireArg:%O, fightResult:%s fightCnt:%d", fireArg, fightResult, fightCnt);
			if (fireArg && sizeof(fireArg) == 2 && 
				fireArg[0] == fightResult && fireArg[1] == fightCnt) {
				runAction(user, trigger[FB_ACTION]);
			}
		}
	}
}

static mapping * initTrigger(string fubenName)
{
        mapping triggers = "module/fuben/data/trigger"->get_data();
	mapping *t = ({});
	foreach(int tid, mapping trigger in triggers) {
		if (fubenName == trigger[FB_NAME]) {
			t += ({trigger});
		}
	}
	return t;
}

static void initTriggerData()
{
        triggerDataMap = ([]);

	foreach(int fid, mapping fubenData in fubenDataMap) {
		triggerDataMap[fid] = initTrigger(fubenData[FB_NAME]);
	}
}

static void addClientNpc(object user, class Fuben fuben)
{
	mapping  *mingLeis = mingLeiDataMap[fuben->id];
	class FubenNpc *npcs = ({});

	foreach (class MingLei mingLei in fuben->mingLeiList) {
		if (mingLei->id < fuben->nextMingLeiIdx)
			continue;
		mapping m = mingLeis[mingLei->id];
		class FubenNpc npc = new (class FubenNpc);
		npc->id = mingLei->id;
		npc->icon = m[FB_ICON];
		npc->x = m[FB_X] - FB_ROAM_WIDTH;
		npc->y = m[FB_Y];
		npc->name = m[FB_ML_NAME];
		npc->title = "";
		npc->roamWidth = FB_ROAM_WIDTH;
		npc->roamHeight = FB_ROAM_HEIGHT;
		npcs += ({npc});
	}

	//debug_message("add client npc:%O\n", npcs);
	rpc_client_fuben_add_npc(user->getId(), npcs);
}

void onPlayerEnterScene(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	//debug_message("new fuben on enter Scene fuben:%O", fuben);
	if (fuben) {
		addClientNpc(user, fuben);
	}
}
mapping getFubenRewardItems(int fubenId)
{
	mapping rewardData = fubenDataMap[fubenId];
	mapping rewardItems = ([]);
	if (undefinedp(rewardData))
		return rewardItems;
	mapping rewardItemData = rewardData["item"];
	if (undefinedp(rewardItemData) || !sizeof(rewardItemData))
		return rewardItems;
	foreach (string type, int chances in rewardItemData)
	{
		if (random(99) < chances)
			rewardItems[type] = 1;
	}
	return rewardItems;
	
}

int getFubenRewardExp(int fubenId)
{
	mapping rewardData = fubenDataMap[fubenId];
	if (undefinedp(rewardData))
		return 0;
	return rewardData["exp"];
}

int getFubenRewardCash(int fubenId)
{
	mapping rewardData = fubenDataMap[fubenId];
	if (undefinedp(rewardData))
		return 0;
	return rewardData["money"];
}

void exitFuben(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	int* pos = fuben->fromLocations;
	Import("HOOK")->fubenEnd(user, fuben);
	user->delTemp(KT_FUBEN);
	user->enterScene(pos[0], pos[1], pos[2]);
}

int getFubenId(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (!fuben)
		return 0;
	return fuben->id;
}

class MingLei getNextMingLei(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (!fuben)
		return 0;
	if (fuben->nextMingLeiIdx >= sizeof(fuben->mingLeiList))
		return 0;
	return fuben->mingLeiList[fuben->nextMingLeiIdx];
}

mapping getFightRewardItems(int fubenId, int mingLeiId)
{
	mapping mingLeiData = mingLeiDataMap[fubenId][mingLeiId];
	if (undefinedp(mingLeiData) || !sizeof(mingLeiData))
		return ([]);
	
	mapping rewardItemData = mingLeiData["item"];
	if (undefinedp(rewardItemData) || !sizeof(rewardItemData))
		return ([]);

	mapping rewardItems = ([]);
	foreach (string type, int chances in rewardItemData)
	{
		if (random(99) < chances)
			rewardItems[type] = 1;
	}
	return rewardItems;
}


void onFightEnd(class Fight fight, int uid, class MingLei mingLei)
{
	object user = get_user(uid);
	if (!objectp(user))
		return;
	class Fuben fuben = GET_FUBEN(user);
	if (!fuben)
		return;

	int win = Import("FIGHT")->isAttackWin(fight) ? 1 : 0;
	//debug_message("onFightEnd uid:%d,win:%d,mingLeiId:%d,mingLeiIdx:%d", uid,win,mingLei->id,fuben->nextMingLeiIdx);
	int *location = Import("FIGHT")->getUserLocation(fight, uid);
	int x = win ? location[1] : to_int(mingLei->x - FB_ROAM_WIDTH * 3); 
	user->enterScene(location[0], x, location[2]);
	int exp = 0;
	int cash = 0;
	mapping itemMap = ([]);

	if(win)
	{
		Import("SCENE")->delChar(uid, mingLei->id);
		fuben->nextMingLeiIdx += 1;
		exp = mingLeiDataMap[fuben->id][mingLei->id]["exp"];
		cash = mingLeiDataMap[fuben->id][mingLei->id]["money"];
		itemMap = getFightRewardItems(fuben->id, mingLei->id);
		fireFightEndTrigger(user, fuben, FB_WIN, fuben->nextMingLeiIdx);
	}
	else
		fireFightEndTrigger(user, fuben, FB_LOST, fuben->nextMingLeiIdx);
		
	Import(fuben->wanfaModule)->onFightEnd(user, fight, win, exp, cash, itemMap);
}

static mapping getNpcNumeric(int key)
{
	return numericDataMap[key];
}


static mixed *defenseLayout(string layoutName)
{
	mapping layout = layoutDataMap[layoutName];
	mapping position = layout[FB_POSITION];
	mixed * defense = ({});

	foreach(int row, mixed * record in position) {
		int col;
		int cnt = sizeof(record);
		for(col = 0; col < cnt; col++) {
			int key = record[col];
			if (key > 0) {
				mapping npc = getNpcNumeric(key);
				defense = Import("FIGHT")->catNpc(defense, npc, (row - 1) * cnt + col + 1);
			}
		}
	}
	
	return defense;
}

static void create()
{
        fubenDataMap = "module/fuben/data/fuben"->get_data();
        layoutDataMap = "module/fuben/data/layout"->get_data();
        numericDataMap = "module/fuben/data/numeric"->get_data();
	initMingLeiData();
	initTriggerData();
	logger = Import("LOG")->New("fuben_base");
}

void fight(object user, class MingLei mingLei)
{
	class Fuben fuben = GET_FUBEN(user);
	if (!fuben)
		return;
	//debug_message("fight mingLeiId:%d nextMingLeiIdx:%d", mingLei->id,fuben->nextMingLeiIdx);
	if (fuben->nextMingLeiIdx != mingLei->id)
		return;
	mapping mingLeiData = mingLeiDataMap[fuben->id][mingLei->id];
	int uid = user->getId();
	mixed* attack = Import("FIGHT")->userCamp(user);
	mixed* defense = defenseLayout(mingLeiData[FB_LAYOUT]);
	debug_message("fight defense:%O ", defense);
	
	class Fight fight = Import("FIGHT")->createFight(user, attack, defense);
	Import("FIGHT")->overrideMovieEndHandler(fight, "onFightEnd", mingLei);
	Import("FIGHT")->start(fight);
	Import("FIGHT")->movieStart(fight);
}

int enterFuben(object user, int fubenId)
{
	mapping fubenData = fubenDataMap[fubenId];
	if(!mapp(fubenData)) 
		return 0;
	int sceneId = fubenData[FB_SCENE];
	if(!fubenData[FB_MULTI_PLAYER])
		sceneId = Import("SCENE")->createVirtualScene(sceneId);
	
	int *pos = user->getLocation();
	class Fuben fuben = new(class Fuben);
	fuben->id = fubenId;
	fuben->sceneId = sceneId;
	fuben->startTime = time();
	fuben->fromLocations = pos;
	fuben->name = fubenData[FB_NAME];
	fuben->wanfaModule = fubenData[FB_WANFA_MODULE];
	if (!fuben->wanfaModule)
		fuben->wanfaModule = "FUBEN_SYS";

	fuben->nextMingLeiIdx = 0;
	fuben->mingLeiList = ({});
	int idx = 0;
	mapping  *mingLeis = mingLeiDataMap[fubenId];
	foreach(mapping m in mingLeis) {
		fuben->mingLeiList += ({new (class MingLei, id:idx, x:m[FB_X]),});
		idx++;
	}
	
	user->setTemp(KT_FUBEN, fuben);
	//debug_message("enterFuben new sceneId:%d fuben:%O", sceneId, GET_FUBEN(user));
	user->enterScene(sceneId, fubenData[FB_X], fubenData[FB_Y]);
	rpc_client_fuben_start(user->getId(), fuben->id, fuben->name);
	//Import(fuben->wanfaModule)->onEnterFuben(user, fuben);
	fireEnterFubenTrigger(user, fuben);
	return 1;
}

void walk(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (fuben) {
		int uid = user->getId();
		int *pos = user->getLocation();
		int x = pos[1];
		//debug_message("out fuben:%d mingLeiIdx:%d x:%d", fuben->id,  fuben->nextMingLeiIdx, x);
		foreach(class MingLei mingLei in fuben->mingLeiList) {
			if (mingLei->x < x && fuben->nextMingLeiIdx == mingLei->id && fuben->nextMingLeiIdx < sizeof(fuben->mingLeiList)) {
		//debug_message("walk fuben fuben:%d mingLei:%d mingLeiIdx:%d x:%d", fuben->id, mingLei->x, fuben->nextMingLeiIdx, x);
				fight(user, mingLei);
				break;
			}
		}
	}
}

static void initMingLeiData()
{
	mingLeiDataMap = ([]);
	foreach(int fubenId, mapping fubenData in fubenDataMap) {
		mingLeiDataMap[fubenId] = initMingLei(fubenData[FB_NAME]);
	}
}

static int sortMingLei(mapping a, mapping b)
{
	return a[FB_X] < b[FB_X] ? -1 : 1;
}

static mapping* initMingLei(string fubenName)
{
        mapping mingLei = "module/fuben/data/minglei"->get_data();
	mapping *m = ({});
	foreach(int mid, mapping ml in mingLei) {
		if (fubenName == ml[FB_NAME]) {
			m += ({ml});
		}
	}
	m = sort_array(m, (:sortMingLei:));
	return m;
}
