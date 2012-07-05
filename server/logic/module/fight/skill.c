
#include <debug.h>
#include "/module/fight/fight.h"
#include "/module/fight/warrior.h"
#include "/module/fight/skill.h"
#include "/module/fight/status.h"


static mapping outputStatusCache;
static mapping skillDescCache;

#define ANIMATION_NORMAL "attack"
#define ANIMATION_MAGIC "magic"

static object logger;

string getClass(class Skill sk)
{
	return "moudle/fight/skill.c";
}

int isNormalAttack(class Skill sk)
{
	return sk->desc->normalAttack;
}

string animationType(class Skill sk)
{
	return isNormalAttack(sk) ? ANIMATION_NORMAL : ANIMATION_MAGIC;
}

// 消耗
static int getCost(class Skill sk, class Warrior w)
{
	class SkillDesc desc = sk->desc;
	if( desc->cost ) {
		return evaluate(desc->cost, w);	
	}
	return 0;
}

// 命中率
static int getRate(class OutputStatus op, class Warrior w, class Warrior t)
{
	if( op->rate ) {
		return evaluate(op->rate, w, t);
	}
	return 0;
}

static void preDo(class OutputStatus op, class Status st, class Warrior w, class Warrior t)
{
	if( op->preDo ) {
		class Skill sk = st->sk;
		logger->Debug("skill predo sk=%s,st=%s,stid=%d", sk->desc->id, st->desc->type, st->id);
		evaluate(op->preDo, sk, st, w, t);
	}
}

static int canPerform(class Skill sk, class Warrior w)
{
	if( SP(w) < getCost(sk, w) ) return 0;

	string *denys = sk->desc->denyStatus;
	if( arrayp(denys) && sizeof(denys) ) {
		if( WARRIOR_MODULE->hasAnyStatus(w, denys) ) return 0;
	}

	return 1;
}

static int cost(class Skill sk, class Warrior w)
{
	int c = getCost(sk, w);
	assert( SP(w) >= c );

	SP(w) -= c;
	logger->Debug("skill cost w=%d,sk=%s,sp=%d", w->position, sk->desc->id, c);
	//WARRIOR_MODULE->costSP(w, c);
	return c;
}

static class Warrior *selectTargets(string targetPos, class OutputStatus op, class Warrior w, class Fight fight)
{
	int wpos = w->position;
	int side = op->targetCamp == CAMP_ME ? SIDE(wpos) : !SIDE(wpos);

	switch(targetPos) {
	case POS_SELF:
		return ({w});
	case POS_S: {
		class Warrior target = FIGHT_LAYOUT->selectPositionTarget(fight, side, FIGHT_LAYOUT->position2row(wpos));
		return target == 0 ? ({}) : ({target});
		}
	case POS_R:
		return FIGHT_LAYOUT->selectRowTargets(fight, side, FIGHT_LAYOUT->position2row(wpos));
	case POS_C:
		return FIGHT_LAYOUT->selectColTargets(fight, side, FIGHT_LAYOUT->position2col(wpos));
	case POS_C1:
		return FIGHT_LAYOUT->selectColTargets(fight, side, 0);
	case POS_C2:
		return FIGHT_LAYOUT->selectColTargets(fight, side, 1);
	case POS_C3:
		return  FIGHT_LAYOUT->selectColTargets(fight, side, 2);
	case POS_ALL:
		return values(fight->warriors);
	default:
		return ({});
	}
}

static void addStatus(class Warrior w, class Status status)
{
	class Status *stList = w->status;
	// 处理状态关系
	if( arrayp(stList) && sizeof(stList) ) {
		string *addList = ({});
		string *setList = ({});
		string *mutList = ({});
		foreach(class Status st in stList) {
			string rel = STATUS_MODULE->getStatusRel(st->desc->type, status->desc->type);

			if( rel == ST_REL_ADD ) addList += ({st->desc->type});
			else if( rel == ST_REL_SET ) setList += ({st->desc->type});
			else if( rel == ST_REL_MUT ) mutList += ({st->desc->type});
		}

		if( sizeof(mutList) ) {
			logger->Debug("skill st excluded w=%d,sk=%s,st=%s,stid=%d", w->position, 
				status->sk->desc->id, status->desc->type, status->id);
			return;
		}

		if( sizeof(setList) ) {
			logger->Debug("skill st set w=%d,sk=%s,st=%s,stid=%d", w->position, 
				status->sk->desc->id, status->desc->type, status->id);
			foreach(string stType in setList) {
				WARRIOR_MODULE->delStatusByType(w, stType);
			}
		}
	}

	WARRIOR_MODULE->addStatus(w, status);
}


static int isHit(class Skill sk, class OutputStatus op, class Warrior w, class Warrior t)
{
	int rate = getRate(op, w, t);
	logger->Debug("w=%d,t=%d,sk=%s,rate=%d", w->position, t->position, sk->desc->id, rate);
	return random(1000) <= rate;
}

int perform(class Skill sk, class Warrior w, class Fight fight)
{
	if(!canPerform(sk, w)) {
		FIGHT_OUTPUT->cmdSkip(fight->output, w);
		return 0;
	}


	mixed *opPairs = ({});
	mapping predo = ([]);
	class Warrior meleeTarget = 0;

	if (!isNormalAttack(sk)) {
		if (SP(w) > 0) {
			predo[PREDO_PREDEFINED_SP] = SP(w);
			logger->Debug("skill left sp w=%d,sk=%s,sp=%d", w->position, sk->desc->id, SP(w));
		}
	}

	foreach( string outputId, string targetPos  in sk->desc->outputStatus) {
		class OutputStatus op = outputStatusCache[outputId];
		class Warrior *targets = selectTargets(targetPos, op, w, fight);
		if (sizeof(targets) > 0) {
			opPairs += ({ ({op, targets}), });
			if (op->melee && meleeTarget == 0) {
				meleeTarget = targets[0];
			}
		}
	}

	if (meleeTarget) {
		logger->Debug("perform start w=%d,sk=%s,t=%d", w->position, sk->desc->id, meleeTarget->position);
	} else {
		logger->Debug("perform start w=%d,sk=%s,t=%d", w->position, sk->desc->id, 0);
	}

	int costSp = cost(sk, w);
	FIGHT_OUTPUT->cmdPerformStart(fight->output, w, sk, costSp, meleeTarget);
	WARRIOR_MODULE->beforePerform(w);

	foreach(mixed * pair in opPairs) {
		class OutputStatus op = pair[0];
		foreach (class Warrior t in pair[1]) {
			if (isHit(sk, op, w, t)) {
				class Status status = STATUS_MODULE->newStatus(op->status, w, sk, op->bout, predo);
				preDo(op, status, w, t);
				addStatus(t, status);
			} else {
				logger->Debug("perform miss w=%d,sk=%s,t=%d", w->position, sk->desc->id, t->position);
				FIGHT_OUTPUT->cmdMiss(fight->output, t);
			}
		}
	}

	WARRIOR_MODULE->afterPerform(w);
	FIGHT_OUTPUT->cmdPerformEnd(fight->output);
	logger->Debug("perform end w=%d,sk=%s", w->position, sk->desc->id);

	return 1;
}

class Skill newSkill(string id)
{
	class Skill sk;
	if( skillDescCache[id] ) {
		sk = new(class Skill);
		sk->id = id;
		sk->desc = skillDescCache[id];
	}

	return sk;
}

// 状态输出表导表字段
#define OP_ST_STATUS "status"
#define OP_ST_TARGET_CAMP "targetCamp"
#define OP_ST_TARGET_POS "targetPos"
#define OP_ST_RATE "rate"
#define OP_ST_PREDO "preDo"
#define OP_ST_BOUT "bout"
#define OP_ST_MELEE "melee"

void initOutputStatus()
{
	mapping datas = "module/fight/data/skill_status"->get_data();
	outputStatusCache = ([]);

	foreach(string opId, mapping data in datas) {
		class OutputStatus op = new(class OutputStatus);
		op->id = opId;
		op->status = data[OP_ST_STATUS];
		op->targetCamp = data[OP_ST_TARGET_CAMP];
		op->melee = data[OP_ST_MELEE];
		op->rate = data[OP_ST_RATE];
		op->preDo = data[OP_ST_PREDO];
		op->bout = data[OP_ST_BOUT];
		outputStatusCache[opId] = op;
	}
	debug_message("initOutputStatus finish!");
	//debug_message("outputStatusCache:%O", outputStatusCache);
}

// 技能表导表字段
#define SK_NAME "name"
#define SK_DENY_STATUS "denyStatus"
#define SK_OP_STATUS "outputStatus"
#define SK_COST "cost"
#define SK_NORMAL_ATTACK "normalAttack"

void initSkillDesc()
{
	mapping datas = "module/fight/data/skill"->get_data();
	skillDescCache = ([]);

	foreach(string skillId, mapping data in datas) {
		class SkillDesc desc  = new(class SkillDesc);
		desc->id = skillId;
		desc->name = data[SK_NAME];
		desc->normalAttack = data[SK_NORMAL_ATTACK];
		desc->denyStatus = data[SK_DENY_STATUS];
		desc->cost = data[SK_COST];

		if( data[SK_OP_STATUS] && mapp(data[SK_OP_STATUS]) ) {
			desc->outputStatus = ([]);
			foreach(string outputId, string selectPos in data[SK_OP_STATUS]) {
				if( !outputStatusCache[outputId] ) {
					debug_message("can't find output status: %s", outputId);
					continue;
				}
				desc->outputStatus[outputId] = selectPos; 
			}
		}

		skillDescCache[skillId] = desc;
	}
	debug_message("initSkillDesc finish!");
}

void create()
{
	logger = Import("LOG")->New("fight");
	initOutputStatus();
	initSkillDesc();
}

