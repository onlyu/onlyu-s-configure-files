// 技能头文件
// ---------------------------------------

#ifndef __SKILL_H__

#define __SKILL_H__

#define SKILL_MODULE "module/fight/skill"

// 技能类型
#define SK_TYPE_ACTIVE 1 //主动技能
#define SK_TYPE_PASSIVE 2 //被动技能

// 目标选择
#define CAMP_ENEMY 1 //敌方
#define CAMP_ME 2 //我方
#define POS_S "singleton" //单体
#define POS_R "row" //主角本行
#define POS_C "col" //主角本列
#define POS_C1 "col1" //列
#define POS_C2 "col2" //列
#define POS_C3 "col3" //列
#define POS_SELF "self" //自己
#define POS_ALL "all" //全部

#define SK_HIT 1
#define SK_GUARD 2
#define SK_MISS 3

class OutputStatus {
	string id;
	string status;
	int targetCamp;
	int melee;
	function rate;
	function preDo;
	int bout;
	mapping ext;
}

class SkillDesc {
	string id;
	string name;
	int normalAttack;
	string *denyStatus;
	mapping outputStatus;
	function cost;
	mapping ext;
}

class Skill {
	string id;
	class SkillDesc desc;
	mapping ext;
}


#endif // __STATUS_H__
