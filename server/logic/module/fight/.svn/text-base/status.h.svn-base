// 技能头文件
// ---------------------------------------

#ifndef __STATUS_H__

#define __STATUS_H__

#define STATUS_MODULE "/module/fight/status"

#include "/module/fight/skill.h"
#include "/module/fight/warrior.h"

#define ST_FIRE_HIT "hit"
#define ST_FIRE_BEFORE "before"
#define ST_FIRE_AFTER "after"
#define ST_EFFECT "effect"

#define ST_REL_SET	"add" 
#define ST_REL_ADD	"set"
#define ST_REL_MUT	"mut"

class StatusDesc {
	string type;
	string name;
	string * fire;
	function effect;
}

class Status {
	int id;
	int bout;
	int boutMax;
	class StatusDesc desc;
	class Skill sk;
	class Warrior caster;
	mapping predo;
	mapping ext;
}

#endif // __STATUS_H__
