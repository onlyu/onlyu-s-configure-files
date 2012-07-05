#include <user_key.h>
#include <var_prop.h>
#include </rc/rpc/fuben.h>

#include "/module/fight/fight.h"

static object logger;
static mapping fubenData;
static mapping mingLeiData;
static mapping numericData;
static mapping layoutData;
static mapping rewardData;
static mapping triggerData;
static mapping starData = ([0:1,20:2,40:3,60:4,80:5]);
static mapping fubenMapData;
MEMORY_VAR(allFubenEntry, ([]))
MEMORY_VAR(allFubenEntrySet, ([]))

#define GRID_WIDTH 650
#define GET_FUBEN(user)  ((user)->getTemp(KT_FUBEN))

#define FB_ROAM_WIDTH 200
#define FB_ROAM_HEIGHT 350

#define FB_ICON "icon"
#define FB_NAME "fbName"
#define FB_ML_NAME "name"
#define FB_X "x"
#define FB_Y "y"
#define FB_ACTION_PLAY "play"
#define FB_LAYOUT "layout"

#define FB_FIRE "fire"
#define FB_FIRE_ARG "fireArg"
#define FB_ACTION "action"
#define FB_POSITION "position"

#define FB_FIRE_FIGHT_END "战斗结束"
#define FB_FIRE_START "进入副本"
#define FB_WIN "胜"
#define FB_LOST "败"

class Trap {
	int id;
	int x;
}

class Fuben {
	int id;
	int isDone;
	int sceneId;
	int start;
	int * from;
	int * fightHpPercents;
	int * losePosList;
	int entryNpcId;
	class Trap *traps;
}


class FubenEntry {
	int id;
	string name;
	int entryNpcId;
	int mapId;
	int x;
	int y;
	int tili;
	string taskId;
}

class FubenEntrySet {
	class FubenEntry * entrys;
}

static void enter(object user, int entryNpcId, int fubenId);
static int fight(object user, class Fuben fuben, class Trap trap);


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
	mixed *triggers = triggerData[fuben->id];
	foreach (mapping trigger in triggers) {
		if (trigger[FB_FIRE] == FB_FIRE_START) {
			runAction(user, trigger[FB_ACTION]);
		}
	}
}

static void fireFightEndTrigger(object user, class Fuben fuben, string fightResult, int fightCnt)
{
	mixed *triggers = triggerData[fuben->id];
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

static mapping getNpcNumeric(int key)
{
	mapping datas = "module/fuben/data/numeric"->get_data();
	return datas[key];
}

static mixed *defenseLayout(string layoutName)
{
	mapping layout = layoutData[layoutName];
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

static mapping getFightRewardItems(mapping mingleiData)
{
	mapping rewardItemData = mingleiData["item"];
	////debug_message("fightRewardItems:%O rewardData:%O", mingleiData, rewardItemData);
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

static mapping getFubenRewardItems(int fubenId)
{
	mapping rewardData = fubenData[fubenId];
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

static int getFubenRewardExp(int fubenId)
{
	mapping rewardData = fubenData[fubenId];
	if (undefinedp(rewardData))
		return 0;
	return rewardData["exp"];
}

static int getFubenRewardCash(int fubenId)
{
	mapping rewardData = fubenData[fubenId];
	if (undefinedp(rewardData))
		return 0;
	return rewardData["money"];
}

static class FubenRewardItem* packRewardItems(mapping itemMap)
{
	class FubenRewardItem* rewardItems = ({});
	class FubenRewardItem rewardItem;
	foreach (string type, int amount in itemMap)
	{
		rewardItem = new (class FubenRewardItem);
		rewardItem->type = type;
		rewardItem->amount = amount;
		rewardItem->name = getItemName(type);
		rewardItem->icon = getItemIcon(type);
		rewardItems += ({rewardItem});
	}
	return rewardItems;
}

static void giveRewardItem(object user,mapping itemMap)
{
	
	foreach (string type, int amount in itemMap)
	{
		////debug_message("add reward item:%s" , type);
		Import("ITEM_UTIL")->addNewItem(user, type, amount); 
	}
	return;
}

static int hpPercentToStar(int hpPercent)
{
	int lowBound = 0;
	int star = 1;
	foreach( int bound,int _star in starData)  
	{
		if (hpPercent > bound && bound > lowBound)
			lowBound = bound;
			
	}
	if (lowBound)
		star = starData[lowBound];
	return star;
}

static int calculateFubenStar(class Fuben fuben)
{
	int size = sizeof(fuben->fightHpPercents);
	int total = 0;
	foreach(int hpPercent in fuben->fightHpPercents)
	{
		total += hpPercent;
	}
	int averageHpPercent = to_int(total / size);
	return hpPercentToStar(averageHpPercent);
}
	
//static int movieEnd(class Fight fight, object user, int sceneId, int x, int y)
void onFightEnd(class Fight fight, int uid, class Trap trap)
{
	logger->Log("fuben fight end uid:%d trap:%d", uid, trap->id);
	object user = get_user(uid);
	if (objectp(user)) {
		int win = Import("FIGHT")->isAttackWin(fight) ? 1 : 0;
		int *location = Import("FIGHT")->getUserLocation(fight, uid);
		//according to the client
		int x = win ? location[1] : to_int(trap->x - FB_ROAM_WIDTH * 3); 
		logger->Log("fuben fight end uid=%d,trap=%d,win=%d,scene=%d,x=%d,y=%d,ex=%d", 
			uid, trap->x, win, location[0], location[1], location[2], x);
		
		user->enterScene(location[0], x, location[2]);

		class Fuben fuben = GET_FUBEN(user);
		if (fuben) {
			class FubenFightResult result = new(class FubenFightResult);
			result->win = win;
			if (win) {
				Import("SCENE")->delChar(uid, trap->id);
				fuben->traps -= ({trap});
				result->exp = mingLeiData[fuben->id][trap->id]["exp"];
				result->cash = mingLeiData[fuben->id][trap->id]["money"];
				Import("UTIL")->addUserExp(user, result->exp);
				Import("UTIL")->addCash(user, result->cash);
				Import("SUMMON_UTIL")->addAllFightSummonExp(user, result->exp);
				mapping itemMap = getFightRewardItems(mingLeiData[fuben->id][trap->id]);
				logger->Log("fight result cash=%d exp=%d items:%O", result->cash, result->exp, itemMap);
				result->items = packRewardItems(itemMap);
				giveRewardItem(user, itemMap);
				rpc_client_fuben_fight_result(uid, result);
				fuben->fightHpPercents += ({80});
				fireFightEndTrigger(user, fuben, FB_WIN, trap->id + 1);
			}
			else {
				result->exp = 0;
				result->cash = 0;
				result->items = ({});
				rpc_client_fuben_fight_result(uid, result);
				fireFightEndTrigger(user, fuben, FB_LOST, trap->id + 1);
			}
		} 
	}
}

void rpc_server_fuben_fight(object user, int fubenId, int trapId)
{
	class Fuben fuben = GET_FUBEN(user);
	//debug_message("fuben fight traps:%O fubenId:%d trapId:%d", fuben->traps, fubenId, trapId);
	if (!fuben || fuben->id != fubenId) 
		return;
	class Trap trap = fuben->traps[0];
	if (trap->id != trapId)
		return;
	fight(user, fuben, trap);
}

void endFuben(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (!fuben) 
		return;

	if (sizeof(fuben->traps) == 0 && !fuben->isDone) {
		fuben->isDone = 1;
		class FubenEvaluate evaluate = new (class FubenEvaluate);
		evaluate->star = calculateFubenStar(fuben);
		evaluate->exp = getFubenRewardExp(fuben->id);
		evaluate->cash = getFubenRewardCash(fuben->id);;
		user->setFubenStar(fuben->id, evaluate->star);
		Import("UTIL")->addUserExp(user, evaluate->exp);
		Import("UTIL")->addCash(user, evaluate->cash);
		Import("SUMMON_UTIL")->addAllFightSummonExp(user, evaluate->exp);
		mapping fubenItemMap = getFubenRewardItems(fuben->id);
		logger->Log("fuben result cash=%d exp=%d items:%O", evaluate->cash, evaluate->exp, fubenItemMap);
		giveRewardItem(user, fubenItemMap);
		evaluate->items = packRewardItems(fubenItemMap);

		rpc_client_fuben_evaluate(user->getId(), evaluate);
	}
}

void rpc_server_fuben_continue(object user)
{
	endFuben(user);	
}

static int fight(object user, class Fuben fuben, class Trap trap)
{
	mapping mingLei = mingLeiData[fuben->id][trap->id];
	int uid = user->getId();
	mixed * attack = Import("FIGHT")->userCamp(user);
	mixed * defense = defenseLayout(mingLei[FB_LAYOUT]);
	
	class Fight fight = Import("FIGHT")->createFight(user, attack, defense);
	Import("FIGHT")->overrideMovieEndHandler(fight, "onFightEnd", trap);
	Import("FIGHT")->start(fight);
	Import("FIGHT")->movieStart(fight);
}

void walkFuben(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (fuben) {
		int uid = user->getId();
		int *pos = user->getLocation();
		int x = pos[1];

		//logger->Log("user move uid=%d,sceneId=%d,x=%d,y=%d", uid, pos[0], pos[1], pos[2]);
		foreach(class Trap trap in fuben->traps) {
			logger->Debug("user move trap uid=%d,x=%d,trapx=%d", uid, x, trap->x);
			if (trap->x < x) {
				fight(user, fuben, trap);
			}
			break;
		}
	}
}

static void addClientNpc(object user, class Fuben fuben)
{
	mapping  *mingLeis = mingLeiData[fuben->id];
	class FubenNpc *npcs = ({});

	foreach (class Trap trap in fuben->traps) {
		mapping m = mingLeis[trap->id];
		class FubenNpc npc = new (class FubenNpc);
		npc->id = trap->id;
		npc->icon = m[FB_ICON];
		npc->x = m[FB_X] - FB_ROAM_WIDTH;
		npc->y = m[FB_Y];
		npc->name = m[FB_ML_NAME];
		npc->title = "";
		npc->roamWidth = FB_ROAM_WIDTH;
		npc->roamHeight = FB_ROAM_HEIGHT;
		npcs += ({npc});
	}

	//logger->Debug("add client npc:%O\n", npcs);
	rpc_client_fuben_add_npc(user->getId(), npcs);
}

void onPlayerEnterScene(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (fuben) {
		addClientNpc(user, fuben);
	}
}

static int exitFuben(object user, class Fuben fuben)
{
	int *pos = fuben->from;

	Import("HOOK")->fubenEnd(user, fuben);
	user->delTemp(KT_FUBEN);
	user->enterScene(pos[0], pos[1], pos[2]);
}

static int reEnter(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (!fuben)
		return 0;
	if (sizeof(fuben->traps))
		return 0;
	int fubenId = fuben->id;
	int entryNpcId = fuben->entryNpcId;
	exitFuben(user, fuben);
	enter(user, entryNpcId, fubenId);
		
}

void rpc_server_fuben_exit(object user, int fubenId)
{
	class Fuben fuben = GET_FUBEN(user);
	if (fuben && fuben->id == fubenId) {
		exitFuben(user, fuben);
	}
}

static int nameCmp(string a, string b)
{
	return (a > b) ? -1 : 1;
}

static int sortEntry(mixed a, mixed b)
{
	class FubenEntry entryA = a;
	class FubenEntry entryB = b;
	return nameCmp(entryA->name, entryB->name);
}

static int canEnterFuben(object user, class FubenEntry entry)
{
	return 1;
	if(Import("MISSION_MAIN")->canBaseAcceptMission(user, entry->taskId) && (Import("MISSION_MAIN")->hasCompletedMission(user, entry->taskId) || Import("MISSION_MAIN")->hasMission(user, entry->taskId))) 
	{
		////debug_message("can base accept:%s", entry->taskId);

		return 1;
	}
	return 0;
}

class FubenEntryInfo* packFubenEntryInfoList(object user, class FubenEntrySet entrySet)
{
	class FubenEntryInfo* entryInfoList = ({});
	class FubenEntryInfo entryInfo;
	
	foreach(class FubenEntry entry in entrySet->entrys)
	{
		
		if (!canEnterFuben(user, entry)) {
			////debug_message("taskId :%s can not accept", entry->taskId);
			continue;	
		}
		entryInfo = new(class FubenEntryInfo);
		entryInfo->fubenId = entry->id;
		entryInfo->name = entry->name;
		entryInfo->mapId = entry->mapId;
		entryInfo->star = user->getFubenStar(entry->id);
		entryInfo->x = fubenMapData[entry->id]["x"];
		entryInfo->y = fubenMapData[entry->id]["y"];
		entryInfoList += ({entryInfo});
	}
	////debug_message("entry List:%O", entryInfoList);
	return entryInfoList;
}

int fubenEntryList(object user, int entryNpcId)
{
	class FubenEntrySet entrySet = allFubenEntrySet[entryNpcId];
	if (!undefinedp(entrySet)) {
		class FubenEntryInfo* entryInfoList = packFubenEntryInfoList(user, entrySet);
		////debug_message("entry List:%O", entryInfoList);
		rpc_client_transport(user->getId(), entryNpcId, entryInfoList);
		return 1;
	}
	return 0;
}


static void newFuben(object user, class FubenEntry entry)
{
	mapping  *mingLeis = mingLeiData[entry->id];
	////debug_message("newFuben entry:%O mingLeis:%O", entry, mingLeis);
	if (mingLeis) {
		int uid = user->getId();
		int sceneId = Import("SCENE")->createVirtualScene(entry->mapId);
		int *pos = user->getLocation();
		class Fuben fuben = new(class Fuben);
		fuben->id = entry->id;
		fuben->sceneId = sceneId;
		fuben->start = time();
		fuben->isDone = 0;
		fuben->from = pos;
		fuben->traps = ({});
		fuben->losePosList = ({});
		fuben->entryNpcId = entry->entryNpcId;
		fuben->fightHpPercents = ({});

		//user->subTili(entry->tili);
		logger->Log("fuben enter uid=%d,fubenId=%d,tili=%d,from=%O", uid, entry->id, entry->tili, pos);
		
		int idx = 0;
		logger->Debug("dump mingleis:%O\n", mingLeis);
		int prePosX = entry->x;
		foreach(mapping m in mingLeis) {
			fuben->traps += ({new (class Trap, id:idx, x:m[FB_X]),});
			fuben->losePosList += ({to_int((prePosX + m[FB_X])/2)});
			prePosX = m[FB_X];
			idx++;
		}
		////debug_message("fuben losePoseList:%O", fuben->losePosList);
		user->setTemp(KT_FUBEN, fuben);
		//logger->Debug("dump traps:%O\n", fuben->traps);

		logger->Log("create fuben ok name=%s,mapId=%d,sceneId=%d,x=%d,y=%d", 
			entry->name, entry->mapId, sceneId, entry->x, entry->y);
		user->enterScene(sceneId, entry->x, entry->y);
		rpc_client_fuben_start(uid, entry->id, entry->name);
		
		fireEnterFubenTrigger(user, fuben);
		return;
	}
}

static class FubenEntry getEntryByFubenId(class FubenEntrySet entrySet, int fubenId)
{
	foreach(class FubenEntry entry in entrySet->entrys)
	{
		if (entry->id == fubenId)
			return entry;
	}
	return 0;
}

static void enter(object user, int entryNpcId, int fubenId)
{
	class FubenEntrySet entrySet = allFubenEntrySet[entryNpcId];
	class FubenEntry entry = getEntryByFubenId(entrySet, fubenId);
	if (!intp(entry)) {
		/*
		if (user->getTili() < entry->tili) {
			user->tellMe("你的体力不足");
			logger->Log("fuben not engouth tili uid=%d", uid);
			return;
		}
		*/
		if (!canEnterFuben(user, entry)) {
			logger->Log("fuben not accept taskId:%s uid=%d", entry->taskId, user->getId());
			return;
		}
		newFuben(user, entry);
	}
}

int getFubenId(class Fuben fuben)
{
	return fuben->id;
}

string getFubenName(class Fuben fuben)
{
	class FubenEntry entry = allFubenEntry[fuben->id];
	return entry->name;
}

int isDone(object user, class Fuben fuben)
{
	return fuben->isDone;
}

void logoutCheck(object user)
{
	class Fuben fuben = GET_FUBEN(user);
	if (fuben) {
		user->setPosition(fuben->from[0], fuben->from[1], fuben->from[2]);
	}
}

static void initFubenEntry()
{
	foreach(int id, mapping info in fubenData) {
		class FubenEntry entry = new(class FubenEntry);
		entry->id = id;
		entry->name = info[FB_NAME];
		entry->entryNpcId = info["npcId"];
		entry->mapId = info["scene"];
		entry->x = info["x"];
		entry->y = info["y"];
		entry->tili = info["tili"];
		entry->taskId = info["taskId"];

		allFubenEntry[entry->id] = entry;

		class FubenEntrySet entrySet = allFubenEntrySet[entry->entryNpcId];
		if (undefinedp(entrySet)) {
			entrySet = new(class FubenEntrySet);
			entrySet->entrys = ({});
			allFubenEntrySet[entry->entryNpcId] = entrySet;
		}
		entrySet->entrys += ({entry});
	}

	foreach(int entryNpcId, class FubenEntrySet entrySet in allFubenEntrySet) {
		entrySet->entrys = sort_array(entrySet->entrys, (:sortEntry:));
	}
}

static int sortMingLei(mapping a, mapping b)
{
	return a[FB_X] < b[FB_X] ? -1 : 1;
}

static mapping * initTrap(string fubenName)
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

static void initMingLeiData()
{
	mingLeiData = ([]);

	foreach(int fid, class FubenEntry entry in allFubenEntry) {
		mingLeiData[fid] = initTrap(entry->name);
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
        triggerData = ([]);

	foreach(int fid, class FubenEntry entry in allFubenEntry) {
		triggerData[fid] = initTrigger(entry->name);
	}
}

void rpc_server_transport(object user, int transportId, int fubenId)
{
	enter(user, transportId, fubenId);
} 

void rpc_server_reenter(object user)
{
	reEnter(user);
}

void create()
{
        fubenData = "module/fuben/data/fuben"->get_data();
        layoutData = "module/fuben/data/layout"->get_data();
        numericData = "module/fuben/data/numeric"->get_data();
        rewardData = "module/fuben/data/reward"->get_data();
	fubenMapData = "module/fuben/data/map"->get_data();
	initFubenEntry();
	initMingLeiData();
	initTriggerData();

	logger = Import("LOG")->New("fuben");
}

