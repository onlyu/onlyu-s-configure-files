
#include <common_key.h>
#include <debug.h>
#include "/module/fight/fight.h"
#include "/module/fight/skill.h"
#include "/module/fight/status.h"

static object logger;

void create()
{
	logger = Import("LOG")->New("fight");
}


static string *ALL_FIGHT_ATTR = ({
	FT_HP,
	FT_MAX_HP,
	FT_SP,
	FT_MAX_SP,
	FT_ATTACK,
	FT_DEFENCE,
	FT_QIANGFA,
	FT_KANGFA,
	FT_SPEED,
	FT_DODGE,
	FT_HIT,
	FT_BAO_JI,
	FT_KANG_BAO,
	FT_ZHAO_JIA,
	FT_PO_JI,
	FT_YC_HIT,
	FT_YC_DODGE,
	FT_FAN_JI,
	FT_LIANG_JI,
	FT_BI_SHA,
	FT_HURT_MORE,
	FT_HURT_LESS,
});


static class Warrior initWarrior(string type, string name, int icon, mapping fightAttr, int position)
{
	class Warrior w = new(class Warrior);
	w->isDead = 0;
	w->type = type;
	w->name = name;
	w->icon = icon;
	w->position = position;
	w->status = ({});
	w->uid = 0;
	w->movePos = 0.0;
	w->fightAttr = fightAttr;
	w->skill = 0;

	if (W_IS_USER(w) || W_IS_SUMMON(w)) {
		SP(w) = W_INIT_SP;
	}
	logger->Debug("init numeric w=%d,attr=%O\n", w->position, w->fightAttr);
	return w;
}

varargs static void modFightAttr(class Warrior w, string key, int delta, class Status st)
{
	int flag = 1;
	if(delta == 0) return;

	w->fightAttr[key] += delta;

	if( key == FT_HP && w->fightAttr[FT_HP] > w->fightAttr[FT_MAX_HP] )
		w->fightAttr[FT_HP] = w->fightAttr[FT_MAX_HP];
	else if( key == FT_SP && w->fightAttr[FT_SP] > w->fightAttr[FT_MAX_SP] )
		w->fightAttr[FT_SP] = w->fightAttr[FT_MAX_SP];

	if (floatp(delta)) {
		delta = to_int(delta);
	}
	if (!(intp(delta))) {
		//logger->Debug("mod attr delta error w=%d,st=%s,delta=%O,predo=%O", 
		//	w->position, st->desc->type, delta, st->predo);
		assert(intp(delta));
	}
	if (!stringp(key)) {
		//logger->Debug("mod attr key error w=%d,st=%s,key=%O", w->position, st->desc->type, key);
		assert(stringp(key));
	}
	//logger->Debug("mod attr w=%d,st=%s,stid=%d,key=%s,delta=%O", w->position, st->desc->type, st->id, key, delta);
	FIGHT_OUTPUT->cmdMod(W_FIGHT(w)->output, w, key, delta, flag);

	if (key == FT_HP) {
		if (HP(w) <= 0) {
			w->isDead = 1;
			FIGHT_MAIN->die(W_FIGHT(w), w);
		}
	}
}

void costSP(class Warrior w, int delta)
{
	modFightAttr(w, FT_SP, delta);
}

void addFightAttr(class Warrior w, string key, int delta, class Status st)
{
	if (delta <= 0) {
		delta = 1;
	} 
	modFightAttr(w, key, delta, st);
}

void subFightAttr(class Warrior w, string key, int delta, class Status st)
{
	if (delta <= 0) {
		delta = 1;
	} 
	if (key == FT_HP) {
		if (SKILL_MODULE->isNormalAttack(st->sk)) {
			modFightAttr(st->caster,  FT_SP, 20, st);
			modFightAttr(w, FT_SP, 15, st);
		} else {
			delta += P(PREDO_PREDEFINED_SP)/W_ADD_SP_DENOMINATOR;
		}
	}

	modFightAttr(w, key, -delta, st);
}

class Status * getStatusByType(class Warrior w, string  stType)
{
	class Status *sts = ({});
	foreach(class Status st in w->status) {
		if (st->desc->type == stType) {
			sts += ({st});
		}
	}
	return sts;
}

void addStatus(class Warrior w, class Status st)
{
	if (STATUS_MODULE->onAdd(st, w)) {
		w->status += ({st});
		logger->Debug("add status w=%d,st=%s,stid=%d,stbout=%d", w->position, st->desc->type,st->id, st->boutMax);
		FIGHT_OUTPUT->cmdAddStatus(W_FIGHT(w)->output, w, st);
	}
}

void delStatus(class Warrior w, class Status *sts)
{
	if (sizeof(sts) > 0) {
		w->status -= sts;
		int *stIds = ({});
		foreach(class Status st in sts) {
			stIds += ({st->id});
			logger->Debug("del status w=%d,st=%s,stid=%d", w->position, st->desc->type, st->id);
			FIGHT_OUTPUT->cmdDelStatus(W_FIGHT(w)->output, w, st);
		}
	}
}

void delStatusById(class Warrior w, int stId)
{
	foreach(class Status st in w->status) {
		if (st->id == stId) {
			delStatus(w, ({st}));
			return;
		}
	}
}

void delStatusByType(class Warrior w, string type)
{
	class Status *sts = getStatusByType(w, type);
	delStatus(w, sts);
}

int hasStatus(class Warrior w, int stId)
{
	foreach(class Status st in w->status) {
		if (st->id == stId) {
			return 1;
		}
	}

	return 0;
}

int hasAnyStatus(class Warrior w, int *stIds)
{
	int idx;
	foreach(class Status st in w->status) {
		idx = member_array(st->id, stIds);
		if (idx >= 0) {
			return 1;
		}
	}
	return 0;
}

void beforePerform(class Warrior w)
{
	foreach(class Status st in w->status) {
		STATUS_MODULE->before(st, w);
	}
}

void afterPerform(class Warrior w)
{
	class Status *sts = ({});
	foreach(class Status st in w->status) {
		if (STATUS_MODULE->after(st, w)) {
			sts += ({st});
		}
	}

	if (sizeof(sts) > 0) {
		delStatus(w, sts);
	}
}

static mapping getUserFightAttr(object user)
{
	mapping fightAttr = ([]);
	foreach(string key in ALL_FIGHT_ATTR)
	{
		fightAttr[key] = user->getFightAttr(key);
	}
	fightAttr[FT_MAX_HP] = fightAttr[FT_HP];
	fightAttr[FT_MAX_SP] = 100;
	return fightAttr;
}

static class Warrior newUserWarrior(object user, int position)
{
	int uid = user->getId();
	string name = user->getName(); 
	int icon = user->getIcon();
	mapping fightAttr = getUserFightAttr(user);
	class Warrior w = initWarrior(W_TYPE_USER, name, icon, fightAttr, position);
	w->uid = uid;
	w->location = user->getLocation();
	string skillId = user->getSkill();
	string normalAttack = user->getNormalAttack();
	w->skill = SKILL_MODULE->newSkill(skillId);
	w->normalAttack = SKILL_MODULE->newSkill(normalAttack);
	assert(w->normalAttack && w->skill);

	return w;
}

static mapping getNpcFightAttr(mapping npcData)
{
	mapping fightAttr = ([]);
	foreach(string key in ALL_FIGHT_ATTR)
	{
		fightAttr[key] = npcData[key];
	}
	fightAttr[FT_MAX_HP] = fightAttr[FT_HP];
	fightAttr[FT_MAX_SP] = 100;
	return fightAttr;
}

static class Warrior newNpcWarrior(mapping npcData, int position)
{
	mapping fightAttr = getNpcFightAttr(npcData);	
	string name = npcData["name"];
	int icon = npcData["icon"];
	string skillId = npcData["skill"];
	string normalAttack = npcData["normalAttack"];
	class Warrior w = initWarrior(W_TYPE_NPC, name, icon, fightAttr, position);
	w->skill = SKILL_MODULE->newSkill(skillId);
	w->normalAttack = SKILL_MODULE->newSkill(normalAttack);
	assert(w->normalAttack && w->skill);
	return w;
}

static mapping getSummonFightAttr(int summonId)
{
	mapping fightAttr = ([]);
	foreach(string key in ALL_FIGHT_ATTR)
	{
		fightAttr[key] = Import("SUMMON_UTIL")->getSummonFightValue(summonId, key);
	}
	fightAttr[FT_MAX_HP] = fightAttr[FT_HP];
	fightAttr[FT_MAX_SP] = 100;
	return fightAttr;
}

static class Warrior newSummonWarrior(int summonId, int position)
{
	mapping fightAttr = getSummonFightAttr(summonId);
	string name = Import("SUMMON_UTIL")->getSummonName(summonId);
	int icon = Import("SUMMON_UTIL")->getSummonResource(summonId);

	class Warrior w = initWarrior(W_TYPE_SUMMON, name, icon, fightAttr, position);
	string skillId = Import("SUMMON_UTIL")->getSummonSkill(summonId);
	string normalAttack = Import("SUMMON_UTIL")->getSummonNormalAttack(summonId);
	w->skill = SKILL_MODULE->newSkill(skillId);
	w->normalAttack = SKILL_MODULE->newSkill(normalAttack);
	assert(w->normalAttack && w->skill);
	return w;
}

class Warrior createWarrior(string type, mixed mix, int position)
{
	class Warrior w = 0;

	switch (type) {
	case W_TYPE_USER:
		w = newUserWarrior(mix, position);
		break;
	case W_TYPE_SUMMON:
		w = newSummonWarrior(mix, position);
		break;
	case W_TYPE_NPC:
		w = newNpcWarrior(mix, position);
		break;
	default:
	}

	return w;
}

