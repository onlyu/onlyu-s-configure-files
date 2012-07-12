#include <user_key.h>
#include <var_prop.h>
#include </rc/rpc/fuben.h>
#include "base.h"
#include "/module/fight/fight.h"

static object logger;
static mapping fubenData;
static mapping fubenMapData;

MEMORY_VAR(allFubenEntry, ([]))
MEMORY_VAR(allFubenEntrySet, ([]))
MEMORY_VAR(allFubenStars, ([]))

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

int getFightStar(object user, class Fight fight);
int getFubenStar(object user);

void onEnterFuben(object user, class Fuben fuben)
{
	rpc_client_fuben_start(user->getId(), fuben->id, fuben->name);
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

class FubenEntryInfo* packFubenEntryInfoList(object user, class FubenEntrySet entrySet)
{
	class FubenEntryInfo* entryInfoList = ({});
	class FubenEntryInfo entryInfo;
	
	foreach(class FubenEntry entry in entrySet->entrys)
	{
		
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

static void giveRewardItem(object user,mapping itemMap)
{
	
	foreach (string type, int amount in itemMap)
	{
		////debug_message("add reward item:%s" , type);
		Import("ITEM_UTIL")->addNewItem(user, type, amount); 
	}
	return;
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

void clearFubenStar(object user)
{
	int uid = user->getId();
	map_delete(allFubenStars, uid);
}

int isFubenStarClear(object user)
{
	return undefinedp(allFubenStars[user->getId()]);
}

void fightEndContinue(object user)
{
	int fubenId = Import("FUBEN_BASE")->getFubenId(user);
	if (!fubenId)
		return;
	class MingLei nextMingLei = Import("FUBEN_BASE")->getNextMingLei(user);

	if (!nextMingLei && !isFubenStarClear(user))
	{
		
		class FubenEvaluate evaluate = new (class FubenEvaluate);
		evaluate->star = getFubenStar(user);
		evaluate->exp = Import("FUBEN_BASE")->getFubenRewardExp(fubenId);
		evaluate->cash = Import("FUBEN_BASE")->getFubenRewardCash(fubenId);
		user->setFubenStar(fubenId, evaluate->star);
		Import("UTIL")->addUserExp(user, evaluate->exp);
		Import("UTIL")->addCash(user, evaluate->cash);
		Import("SUMMON_UTIL")->addAllFightSummonExp(user, evaluate->exp);
		mapping fubenItemMap = Import("FUBEN_BASE")->getFubenRewardItems(fubenId);
		logger->Log("fuben result cash=%d exp=%d items:%O", evaluate->cash, evaluate->exp, fubenItemMap);
		giveRewardItem(user, fubenItemMap);
		evaluate->items = packRewardItems(fubenItemMap);
		rpc_client_fuben_evaluate(user->getId(), evaluate);
		clearFubenStar(user);
	}
		
}


int fubenEntryList(object user, int entryNpcId)
{
	class FubenEntrySet entrySet = allFubenEntrySet[entryNpcId];
	if (!undefinedp(entrySet)) {
		class FubenEntryInfo* entryInfoList = packFubenEntryInfoList(user, entrySet);
		debug_message("entry List:%O", entryInfoList);
		rpc_client_transport(user->getId(), entryNpcId, entryInfoList);
		return 1;
	}
	return 0;
}

void addFightStar(int uid, int star)
{
	int* starArray = allFubenStars[uid];
	if (undefinedp(starArray) || !arrayp(starArray))
	{
		starArray = ({});
		allFubenStars[uid] = starArray;
	}
	starArray += ({star});
	allFubenStars[uid] = starArray;
}


int getFubenStar(object user)
{
	int uid = user->getId();
	int* starArray = allFubenStars[uid];
	if (undefinedp(starArray) || !arrayp(starArray))
		return 0;
	int size = sizeof(starArray);	
	int total = 0;
	foreach (int star in starArray)
		total += star;
	return to_int(total/size);
}


int getFightStar(object user, class Fight fight)
{
	return 4;
}

void onFightEnd(object user, class Fight fight, int win, int exp, int cash, mapping itemMap)
{
	class FubenFightResult result = new(class FubenFightResult);
	result->exp = exp;
	result->cash = cash;
	Import("UTIL")->addUserExp(user, result->exp);
	Import("UTIL")->addCash(user, result->cash);
	Import("SUMMON_UTIL")->addAllFightSummonExp(user, result->exp);
	result->items = packRewardItems(itemMap);
	result->win = 1;
	giveRewardItem(user, itemMap);
	int star = getFightStar(user, fight);
	int uid = user->getId();
	addFightStar(uid, star);
	rpc_client_fuben_fight_result(uid, result);
	return;
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

void create()
{
	logger = Import("LOG")->New("fuben");
	fubenMapData = "module/fuben/data/map"->get_data();
        fubenData = "module/fuben/data/fuben"->get_data();
	initFubenEntry();
}

void enterFuben(object user, int fubenId)
{
	Import("FUBEN_BASE")->enterFuben(user, fubenId);
	allFubenStars[user->getId()] = ({});
}

void exitFuben(object user)
{
	Import("FUBEN_BASE")->exitFuben(user);
	map_delete(allFubenStars, user->getId());
}

int reEnter(object user)
{
	int fubenId = Import("FUBEN_BASE")->getFubenId(user);
	if (!fubenId)
		return 0;
	if (Import("FUBEN_BASE")->getNextMingLei(user))
		return 0;
	exitFuben(user);
	enterFuben(user, fubenId);
}
