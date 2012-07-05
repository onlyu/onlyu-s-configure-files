
#include </rc/rpc/fight.h>
#include <var_prop.h>
#include <user_key.h>
#include <summon.h>
#include <fightmatrix.h>
#include "/module/fight/fight.h"
#include "/module/fight/skill.h"
#include <fightmatrix.h>
#include <debug.h>

#define MAX_BOUT_NUM  100

#define FT_UNDEFINED (-1)

#define FT_POSITON_START 1
#define FT_POSITON_END 18

#define FT_HANDLER_MOVIE_END 1
#define FT_HANDLER_MOVIE_START 2

class Handler {
	int type;
	string file;
	string func;
	mixed *args;
}

// 自增的战斗ID
MEMORY_VAR(incFightId, 1)


MEMORY_VAR(allFight, ([]))

static object logger;

static int nextFightId()
{
	return ++incFightId;
}

void create()
{
	logger = Import("LOG")->New("fight");
}

static class Fight getFight(int fightId)
{
	return allFight[fightId];
}


static class FightSide newFightSide()
{
	return new(class FightSide, warriors:([]));
}

static class FightResult newFightResult()
{
	return new(class FightResult, win:FT_UNDEFINED, death:({ ([]), ([]),}));
}


static void addHandler(class Fight fight, int type, string file, string func, mixed* args...)
{
	class Handler handler = new (class Handler, type:type, file:file, func:func, args:args);
        fight->handler[type] += ({ handler, });
}

static class Fight newFight(int *place)
{
	int now = time();
	int fightId = nextFightId();
	class Fight fight = new(class Fight);

	fight->id = fightId;
	fight->uids = ({});
	fight->sceneId = place[0];
	fight->x = place[1];
	fight->y = place[2];
	fight->warriors = ([]);
	fight->userWarriors = ([]);
	fight->bout = 0;
	fight->boutMax = MAX_BOUT_NUM;
	fight->moveBar = 0;
	fight->handler = ([ FT_HANDLER_MOVIE_END:({}), FT_HANDLER_MOVIE_START:({}),]);
	fight->result = newFightResult();
	fight->side = ({ newFightSide() , newFightSide() });
	fight->output = FIGHT_OUTPUT->newOutput();

	addHandler(fight, FT_HANDLER_MOVIE_START, __FILE__, "onmovieStartDefault");
	addHandler(fight, FT_HANDLER_MOVIE_END, __FILE__, "onMovieEndDefault");

	allFight[fightId] = fight;
	return fight;
}


static void delFight(int fightId)
{
	map_delete(allFight, fightId);
}



static void overrideHandler(class Fight fight, int type, string file, string func, mixed* args...)
{
	class Handler handler = new (class Handler, type:type, file:file, func:func, args:args);
	fight->handler[type][0] = handler;
}

//called by external file
void addMovieEndHandler(class Fight fight, string func, mixed* args...)
{
	string file = caller_name();
	addHandler(fight, FT_HANDLER_MOVIE_END, file, func, args...);
}
void overrideMovieEndHandler(class Fight fight, string func, mixed* args...)
{
	string file = caller_name();
	overrideHandler(fight, FT_HANDLER_MOVIE_END, file, func, args...);
}

//called by external file
void addRecordEndHandler(class Fight fight, string func, mixed* args...)
{
	string file = caller_name();
	addHandler(fight, FT_HANDLER_MOVIE_START, file, func, args...);
}

void overrideRecordEndHandler(class Fight fight, string func, mixed* args...)
{
	string file = caller_name();
	overrideHandler(fight, FT_HANDLER_MOVIE_START, file, func, args...);
}

static void callHandler(class Fight fight, mixed type, mixed *args...)
{
	logger->Debug("try call fight handler file:%O\n", type);
	mapping hook = fight->handler;
        if (undefinedp(hook[type])) {
		logger->Debug("empty  call fight handler file:%O\n", type);
		return ;
        }    

        foreach (class Handler handler in hook[type]) {
                mixed *hargs = ({handler->func}) + args;
                if (arrayp(handler->args)) {
                        hargs += handler->args;;
                }    
		logger->Debug("call fight handler file:%s,fun:%s\n", handler->file, handler->func);
                call_other(handler->file, hargs);
        } 
}


static int addWarrior(class Fight fight, class Warrior w)
{
	int pos = w->position;
	fight->warriors[pos] = w;
	fight->side[SIDE(pos)]->warriors[pos] = w;
	W_FIGHT(w) = fight;
	if (w->type == W_TYPE_USER) {
		fight->uids += ({w->uid});
		fight->userWarriors[w->uid] = w;
		object user = get_user(w->uid);
		if (objectp(user)) {
			user->setTemp(KT_FIGHT, fight);
		}
	}
}

static int delWarrior(class Fight fight, class Warrior w)
{
	int pos = w->position;
	map_delete(fight->side[SIDE(pos)]->warriors, pos);
	map_delete(fight->warriors, pos);
	//FIGHT_OUTPUT->cmdDelWarrior(fight->output, w);
	if (sizeof(fight->side[SIDE(pos)]->warriors) == 0) {
		fight->result->win = !SIDE(w->position);
	}
}

static void addCamp(class Fight fight, int side, class Wvalue *wvs)
{
	//logger->Debug("add camp side=%d,ws=%O\n", side, wvs);
	foreach(class Wvalue wv in wvs) {
		class Warrior w = WARRIOR_MODULE->createWarrior(wv->type, wv->warrior, POSITION(side, wv->position));
		addWarrior(fight, w);
	}
}


static int getMaxSpeed(class Fight fight)
{
	int max = 0;
	foreach (int wid, class Warrior w in fight->warriors) {
		if (SPEED(w) > max) {
			max = SPEED(w);
		}
	}
	return max;
}

static class Fight doNew(int *place, class Wvalue * attack, class Wvalue * defense)
{
	if (sizeof(attack) > 0 && sizeof(defense) > 0) {
		class Fight fight = newFight(place);

		addCamp(fight, FT_SIDE_ATTACK, attack);
		addCamp(fight, FT_SIDE_DEFENSE, defense);
		fight->moveTime = 2;
		fight->moveBar = fight->moveTime * getMaxSpeed(fight);
		FIGHT_OUTPUT->place(fight->output, place[0], place[1], place[2]);

		mapping users = ([]);
		foreach (int pos, class Warrior w in fight->warriors) {
			if (W_IS_USER(w)) {
				int side = SIDE(w->position);
				users["" + w->uid] = (["side":side]);
			}
		}
		FIGHT_OUTPUT->initUser(fight->output, users);

		foreach (int pos, class Warrior w in fight->warriors) {
			FIGHT_OUTPUT->initAddWarrior(fight->output, w);
		}

		return fight;
	} else {
		return 0;
	}
}

class Fight createFight(object user, mixed* attack, mixed* defense)
{
	int *place = user->getLocation();
	place[0] = Import("SCENE")->getLocationMapId(user);
	logger->Debug("fight start uid=%d", user->getId());	
	class Fight fight = doNew(place, attack, defense);
	return fight;
}

static object * getWarriorUser(class Fight fight)
{
	object * users = ({});
	foreach (int uid in fight->uids) {
		object user = get_user(uid);
		if (user) {
			users += ({user});
		}
	}

	return users;
}

int onmovieStartDefault(class Fight fight)
{
	foreach(object user in getWarriorUser(fight)) {
		user->leaveScene();
	}
}

int onMovieEndDefault(class Fight fight, int uid)
{
	object user = get_user(uid);
	if (objectp(user)) {
		class Warrior w = fight->userWarriors[uid];
		int *location = w->location;
		user->enterScene(location[0], location[1], location[2]);
	}
}

int movieStart(class Fight fight)
{
	string record;
	logger->Debug("record end win=%d", fight->result->win);
	record = FIGHT_OUTPUT->record(fight->output);
	if (stringp(record)) {
		callHandler(fight, FT_HANDLER_MOVIE_START, fight);
		rpc_client_fight_movie_start(fight->uids, record);
	} else {
		logger->Error("bad record");
	}
}

static int movieEnd(class Fight fight, int uid)
{
	logger->Debug("fight end win=%d", fight->result->win);
	callHandler(fight, FT_HANDLER_MOVIE_END, fight, uid);
	Import("HOOK")->fightEnd(getWarriorUser(fight), fight);
	//todo !! multi user may delete the fight many times
	delFight(fight->id);
}

void die(class Fight fight, class Warrior w)
{
	int pos = w->position;
	fight->result->death[SIDE(pos)][pos] = w;
	delWarrior(fight, w);
	logger->Debug("die w=%d", w->position);
	FIGHT_OUTPUT->cmdDie(fight->output, w);
}

static class Skill selectSkill(class Fight fight, class Warrior w)
{
	if (SP(w) >= W_SP_FULL) {
		return w->skill;
	} else {
		return w->normalAttack;
	}
}

static int performSkill(class Fight fight, class Warrior w)
{
	class Skill sk = selectSkill(fight, w);
	if (sk) {
		SKILL_MODULE->perform(sk, w, fight);
		return 1;
	} 
	return 0;
}


static class Warrior move(class Fight fight)
{
	class Warrior fastest = 0;
	float minTime = fight->moveTime + 1;
	float len, time;
	int speed;

	assert(sizeof(fight->warriors) > 0);
	foreach (int wid, class Warrior w in fight->warriors) {
		speed = SPEED(w);
		if (speed == 0) {
			speed = 1;
			SPEED(w) = speed;
		}

		len = fight->moveBar - w->movePos;
		time = len / speed;

		logger->Debug("calc fastest bout=%d,w=%d,speed=%d,len=%f,time=%f,minTime=%f", 
			fight->bout, w->position, SPEED(w), len, time, minTime);

		if (minTime > time) {
			minTime = time;
			fastest = w;
			logger->Debug("tmp fastest bout=%d,w=%d,speed=%d,len=%f,time=%f,minTime=%f", 
				fight->bout, fastest->position, SPEED(fastest), len, time, minTime);
		}
	}

	foreach (int wid, class Warrior w in fight->warriors) {
		if (w->position == fastest->position) {
			w->movePos = 0.0;
			logger->Debug("pick fastest bout=%d,w=%d,speed=%d,movePos=%f,moveBar=%d", 
				fight->bout, w->position, SPEED(w), w->movePos, fight->moveBar);
		} else {
			w->movePos += (minTime * SPEED(w));
			logger->Debug("move fastest bout=%d,w=%d,speed=%d,movePos=%f,moveBar=%d", 
				fight->bout, w->position, SPEED(w), w->movePos, fight->moveBar);
		}
	}


	return fastest;
}

//todo
static int isEnd(class Fight fight)
{
	return fight->result->win != FT_UNDEFINED || sizeof(fight->warriors) == 0; 
}

void start(class Fight fight)
{
	while (++fight->bout < fight->boutMax) {
		class Warrior w = move(fight);
		assert(w != 0);
		assert(performSkill(fight, w));
		if (isEnd(fight)) {
			// movieStart(fight);
			return ;
		}
	}

	fight->result->win = FT_SIDE_DEFENSE;
	// movieStart(fight);
}

int deadNpcNum(class Fight fight, int side, string name)
{
	int c = 0;
	if (fight) {
		foreach (int pos, class Warrior w in fight->result->death[side]) {
			if (w->name == name) {
				c++;
			}
		}
	}

	return c;
}

int defenseDeadNpcNum(class Fight fight, string name)
{
	return deadNpcNum(fight, FT_SIDE_DEFENSE, name);
}

int isAttackWin(class Fight fight)
{
	return fight->result->win == FT_SIDE_ATTACK;
}

int isDefenceWin(class Fight fight)
{
	return fight->result->win == FT_SIDE_DEFENSE;
}

int * getUserLocation(class Fight fight, int uid)
{
	class Warrior w = fight->userWarriors[uid];
	return w->location;
}

static mixed* cat(mixed * camp, string type, mixed w, int pos)
{
	camp += ({ new(class Wvalue, type:type, warrior:w, position:pos), });
	//logger->Debug("fight cat:%O\n", camp);
	return camp;
}

mixed * catUser(mixed * camp, object user, int pos)
{
	return cat(camp, W_TYPE_USER, user, pos);
}

mixed * catNpc(mixed *camp, mapping npc, int pos)
{
	return cat(camp, W_TYPE_NPC, npc, pos);
}

mixed * catSummon(mixed *camp, int summonId, int pos)
{
	return cat(camp, W_TYPE_SUMMON, summonId, pos);
}

mixed * userCamp(object user)
{
	mixed * camp = ({});
	mapping layout = user->getFightMatrix();
	foreach(int pos, class FightGridInfo info in layout) {
		if (info->type == FIGHT_GRID_HERO) {
			camp = catUser(camp, user, pos);
		} else if (info->type == FIGHT_GRID_SUMMON) {
			camp = catSummon(camp, info->id, pos);
		}
	}
	return camp;
}

void rpc_server_fight_movie_end(object user)
{
	int uid = user->getId();
	logger->Debug("fight movie end uid=%d", uid);
	class Fight fight = user->getTemp(KT_FIGHT);
	if (fight) {
		//logger->Debug("fight movie end enter uid=%d,sceneId=%d,x=%d,y=%d", uid, sceneId, x, y);
		movieEnd(fight, uid);
	}
}

void rpc_server_fight_pk(object user, int uid)
{
	object other = get_user(uid);
	if (objectp(other)) {
		mixed * attack = userCamp(user);
		mixed * defense = userCamp(other);
		class Fight fight = createFight(user, attack, defense);
		start(fight);
		// 播电影
		movieStart(fight);
	}
}

/*
class Fight testFight(object user, object target)
{
	mixed * attack = userCamp(user);
	mixed * defense = userCamp(target);
	class Fight fight = createFight(user, attack, defense);
	start(fight);
	return fight;
}
*/
