// 战斗头文件
// ---------------------------------------

#ifndef __WARRIOR_H__
#define __WARRIOR_H__

#include <common_key.h>
#include "/module/fight/skill.h"

#define WARRIOR_MODULE "module/fight/warrior"

#define W_TYPE_USER "user"
#define W_TYPE_NPC "npc"
#define W_TYPE_SUMMON "summon"


class Warrior {
	string type;
	int uid;
	int *location;
	int isDead;
	int position;
	string name;
	int icon;
	mixed *status;
	float movePos;
	mixed fight;
	mapping fightAttr;
	class Skill skill;
	class Skill normalAttack;
	mapping ext;
}

class Wvalue {
	string type;
	mixed warrior;
	int position;
}

#define W_IS_NPC(w) (w->type == W_TYPE_NPC)
#define W_IS_USER(w) (w->type == W_TYPE_USER)
#define W_IS_SUMMON(w) (w->type == W_TYPE_SUMMON)

#define W_FIGHT(w) ((class Fight)(w)->fight)

#define W_INIT_SP 40
#define W_SP_FULL 100
#define W_ADD_SP_DENOMINATOR 200
#define W_ATTACK_ADD_SP 20
#define W_ATTACKED_ADD_SP 15

// 获取战斗属性
#define W_GET(w,key) ((w)->fightAttr[key])
#define W_ADD(w,key,delta,st) WARRIOR_MODULE->addFightAttr(w,key,delta,st)
#define W_SUB(w,key,delta,st) WARRIOR_MODULE->subFightAttr(w,key,delta,st)

#define ATTACK(w) W_GET(w,FT_ATTACK)
#define DEFENCE(w) W_GET(w,FT_DEFENCE)
#define QIANGFA(w) W_GET(w,FT_QIANGFA)
#define KANGFA(w) W_GET(w,FT_KANGFA)

#define HP(w) W_GET(w,FT_HP) 
#define MAX_HP(w) W_GET(w,FT_MAX_HP) 
#define SP(w) W_GET(w,FT_SP)
#define MAX_SP(w) W_GET(w,FT_MAX_SP) 
#define SPEED(w) W_GET(w,FT_SPEED)

#define DODGE(w) W_GET(w,FT_DODGE) // 闪避
#define HIT(w) W_GET(w,FT_HIT) // 命中
#define BAO_JI(w) W_GET(w,FT_BAO_JI) // 暴击
#define KANG_BAO(w) W_GET(w,FT_KANG_BAO) // 抗暴击
#define ZHAO_JIA(w) W_GET(w,FT_ZHAO_JIA) // 招架
#define PO_JI(w) W_GET(w,FT_PO_JI) // 破击
#define YC_HIT(w) W_GET(w,FT_YC_HIT) // 异常命中
#define YC_DODGE(w) W_GET(w,FT_YC_DODGE) // 异常抗性
#define FAN_JI(w) W_GET(w,FT_FAN_JI) // 反击
#define LIANG_JI(w) W_GET(w,FT_LIANG_JI) // 连击
#define BI_SHA(w) W_GET(w,FT_BI_SHA) // 必杀
#define HURT_MORE(w) W_GET(w,FT_HURT_MORE) // 伤害加深
#define HURT_LESS(w) W_GET(FT_HURT_LESS) // 伤害减免

#define ADD_HP(w,delta) W_ADD(w,FT_HP,(delta),st);
#define ADD_SPEED(w,delta) W_ADD(w,FT_SPEED,(delta),st);
#define ADD_SP(w,delta) W_ADD(w,FT_SP,delta,st);
#define ADD_ATTACK(w,delta) W_ADD(w,FT_ATTACK,delta,st);
#define ADD_DEFENCE(w,delta) W_ADD(w,FT_DEFENCE,delta,st);
#define ADD_QIANGFA(w,delta) W_ADD(w,FT_QIANGFA,delta,st);
#define ADD_KANGFA(w,delta) W_ADD(w,FT_KANGFA,delta,st);

#define ADD_DODGE(w,delta) W_ADD(w, FT_DODGE, delta, st);
#define ADD_HIT(w,delta) W_ADD(w, FT_HIT, delta, st);
#define ADD_BAO_JI(w,delta) W_ADD(w, FT_BAO_JI, delta, st);
#define ADD_KANG_BAO(w,delta) W_ADD(w, FT_KANG_BAO, delta, st);
#define ADD_ZHAO_JIA(w,delta) W_ADD(w, FT_ZHAO_JIA, delta, st);
#define ADD_PO_JI(w,delta) W_ADD(w, FT_PO_JI, delta, st);
#define ADD_YC_HIT(w,delta) W_ADD(w, FT_YC_HIT, delta, st);
#define ADD_YC_DODGE(w,delta) W_ADD(w, FT_YC_DODGE, delta, st);
#define ADD_FAN_JI(w,delta) W_ADD(w, FT_FAN_JI, delta, st);
#define ADD_LIANG_JI(w,delta) W_ADD(w, FT_LIANG_JI, delta, st);
#define ADD_BI_SHA(w,delta) W_ADD(w, FT_BI_SHA, delta, st);
#define ADD_HURT_MORE(w,delta) W_ADD(w, FT_HURT_MORE, delta, st);
#define ADD_HURT_LESS(w,delta) W_ADD(w, FT_HURT_LESS, delta, st);

#define SUB_HP(w,delta) W_SUB(w,FT_HP,(delta),st);
#define SUB_SPEED(w,delta) W_SUB(w,FT_SPEED,(delta),st);
#define SUB_SP(w,delta) W_SUB(w, FT_SP, (delta), st);
#define SUB_ATTACK(w,delta) W_SUB(w, FT_ATTACK, (delta), st);
#define SUB_DEFENCE(w,delta) W_SUB(w, FT_DEFENCE, (delta), st);
#define SUB_QIANGFA(w,delta) W_SUB(w, FT_QIANGFA, (delta), st);
#define SUB_KANGFA(w,delta) W_SUB(w, FT_KANGFA, (delta), st);

#define SUB_DODGE(w,delta) W_SUB(w, FT_DODGE, delta, st);
#define SUB_HIT(w,delta) W_SUB(w, FT_HIT, delta, st);
#define SUB_BAO_JI(w,delta) W_SUB(w, FT_BAO_JI, delta, st);
#define SUB_KANG_BAO(w,delta) W_SUB(w, FT_KANG_BAO, delta, st);
#define SUB_ZHAO_JIA(w,delta) W_SUB(w, FT_ZHAO_JIA, delta st);
#define SUB_PO_JI(w,delta) W_SUB(w, FT_PO_JI, delta, st);
#define SUB_YC_HIT(w,delta) W_SUB(w, FT_YC_HIT, delta, st);
#define SUB_YC_DODGE(w,delta) W_SUB(w, FT_YC_DODGE, delta, st);
#define SUB_FAN_JI(w,delta) W_SUB(w, FT_FAN_JI, delta, st);
#define SUB_LIANG_JI(w,delta) W_SUB(w, FT_LIANG_JI, delta, st);
#define SUB_BI_SHA(w,delta) W_SUB(w, FT_BI_SHA, delta, st);
#define SUB_HURT_MORE(w,delta) W_SUB(w, FT_HURT_MORE, delta, st);
#define SUB_HURT_LESS(w,delta) W_SUB(w, FT_HURT_LESS, delta, st);

// predo
#define PREDO_PREDEFINED_SP "__wg01_predo_predefined_SP"
#define P(key) ((st)->predo["##key"])

#endif // __WARRIRO_H__

