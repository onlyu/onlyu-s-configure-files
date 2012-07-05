
#include "/module/fight/fight.h" 
#include "/module/fight/status.h"
#include "/module/fight/warrior.h"
#include <var_prop.h>
#include <debug.h>

#define STATUS_ID_START 10

static object logger;
static mapping statusDescCache;
MEMORY_VAR(gStatusId, STATUS_ID_START)


static void jiaxue_effect(class Status st, class Warrior w)
{
	ADD_HP(w, HP(w) * 2);
} 

static mapping predefinedEffect = ([
	"jiaxue":(:jiaxue_effect:),
]);

// 返回状态关系
string getStatusRel(string has, string add)
{
	mapping datas = "module/fight/data/status_rel"->get_data();
	if( !datas[has] || !datas[has][add] ) {
		debug_message("can't find status rel, has:%s, add:%s", has, add);
		return ST_REL_ADD;
	}
	return datas[has][add];
}

static function getEffect(class Status st)
{
	function f = st->desc->effect;
	if (!functionp(f)) {
		f = predefinedEffect[st->desc->type];
	}

	assert(f);
	return f;
}

#define IS_END(st) (st->bout >= st->boutMax)
#define ST_RUN_EFFECT evaluate(getEffect(st), st, st->caster, w)

void before(class Status st , class Warrior w)
{
	if (member_array(ST_FIRE_BEFORE, st->desc->fire) >= 0) {
		logger->Debug("status before effect w=%d,st=%s,stid=%d", w->position, st->desc->type, st->id);
		ST_RUN_EFFECT;
	}
}

int after(class Status st , class Warrior w)
{
	st->bout++;

	if (member_array(ST_FIRE_AFTER, st->desc->fire) >= 0) {
		logger->Debug("status after effect w=%d,st=%s,stid=%d", w->position, st->desc->type, st->id);
		ST_RUN_EFFECT;
	}

	return IS_END(st);
}

int onAdd(class Status st, class Warrior w)
{
	if (member_array(ST_FIRE_HIT, st->desc->fire) >= 0) {
		logger->Debug("status hit effect w=%d,st=%s,stid=%d", w->position, st->desc->type, st->id);
		ST_RUN_EFFECT;
	}
	return !IS_END(st);
}

class Status newStatus(string type, class Warrior w, class Skill sk, int boutMax, mapping predo)
{
	class Status st = new (class Status);
	class StatusDesc desc = statusDescCache[type];
	string msg = sprintf("status no desc st=%s", type);
	xassert(desc, msg);

	st->id = ++gStatusId;
	st->bout = 0;
	st->boutMax = boutMax;
	st->sk = sk;
	st->desc = desc;
	st->predo = predo;
	st->caster = w;


	return st;
}

// 状态表导表字段
#define ST_NAME "name"
#define ST_FIRE "fire"
#define ST_EFFECT "effect"


void initStatusDesc()
{
	mapping datas = "module/fight/data/status"->get_data();
	statusDescCache = ([]);

	foreach(string type, mapping data in datas) {
		class StatusDesc desc = new(class StatusDesc);
		desc->type = type;
		desc->name = data[ST_NAME];
		desc->fire = data[ST_FIRE];
		desc->effect = data[ST_EFFECT];

		statusDescCache[type] = desc;
	}
	debug_message("initStatusDesc finish!");
	//debug_message("statusDescCache:%O", statusDescCache);
}

void create()
{
	logger = Import("LOG")->New("fight");
	initStatusDesc();
}

