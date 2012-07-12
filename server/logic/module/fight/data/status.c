#include "/module/fight/warrior.h"
#include "/module/fight/status.h"

// -------------------  Auto Generate Begin --------------------

mapping data = ([ 
 
	"AD":([ 
	  "fire":({"hit", }),  
	  "name":"物理伤害",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_HP(t,P(AD)); }, ]),
	"MD":([ 
	  "fire":({"hit", }),  
	  "name":"法术伤害",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_HP(t,P(MD)); }, ]),
	"Heal":([ 
	  "fire":({"hit", }),  
	  "name":"法术治疗",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_HP(t,P(Heal)); }, ]),
	"HOT":([ 
	  "fire":({"hit", "before", }),  
	  "animation":100014,  
	  "name":"持续治疗",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_HP(t,P(HOT)); }, ]),
	"DOT":([ 
	  "fire":({"hit", "before", }),  
	  "animation":100012,  
	  "name":"持续伤害",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_HP(t,P(DOT)); }, ]),
	"SPUp":([ 
	  "fire":({"hit", }),  
	  "name":"增加怒气",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_SP(t,P(SPUp)); }, ]),
	"SPDown":([ 
	  "fire":({"hit", }),  
	  "name":"减少怒气",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_SP(t,P(SPDown)); }, ]),
	"Stun":([ 
	  "fire":({"hit", }),  
	  "animation":100013,  
	  "name":"眩晕",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_HP(t,1); }, ]),
	"Conf":([ 
	  "fire":({"hit", }),  
	  "animation":100011,  
	  "name":"混乱",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_HP(t,1); }, ]),
	"SpeUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加速",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_SPEED(t,P(SpeUp)); }, ]),
	"SpeDown":([ 
	  "fire":({"hit", }),  
	  "animation":100015,  
	  "name":"减速",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_SPEED(t,P(SpeDown)); }, ]),
	"AttUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加攻",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_ATTACK(t,P(AttUp)); }, ]),
	"AttDown":([ 
	  "fire":({"hit", }),  
	  "animation":100015,  
	  "name":"减攻",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_ATTACK(t,P(AttDown)); }, ]),
	"MagUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加强法",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_QIANGFA(t,P(MagUp)); }, ]),
	"MagDown":([ 
	  "fire":({"hit", }),  
	  "animation":100015,  
	  "name":"减强法",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_QIANGFA(t,P(MagDown)); }, ]),
	"DefUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加防",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_DEFENCE(t,P(DefUp)); }, ]),
	"DefDown":([ 
	  "fire":({"hit", }),  
	  "animation":100015,  
	  "name":"减防",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_DEFENCE(t,P(DefDown)); }, ]),
	"ResUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加抗",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_KANGFA(t,P(ResUp)); }, ]),
	"ResDown":([ 
	  "fire":({"hit", }),  
	  "animation":100015,  
	  "name":"减抗",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { SUB_KANGFA(t,P(ResDown)); }, ]),
	"CriUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加暴击",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_BAO_JI(t,P(CriUp)); }, ]),
	"ParUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加招架",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_ZHAO_JIA(t,P(ParUp)); }, ]),
	"DodUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加闪避",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_DODGE(t,P(DodUp)); }, ]),
	"CAUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加反击",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_FAN_JI(t,P(CAUp)); }, ]),
	"ERUp":([ 
	  "fire":({"hit", }),  
	  "animation":100014,  
	  "name":"加异常抗性",  
	  "effect":function (class Status st, class Warrior w, class Warrior t) { ADD_YC_DODGE(t,P(ERUp)); }, ]),
	"ActUp":([ 
	  "fire":({"hit", }),  
	  "name":"前进", ]),
	"ActDown":([ 
	  "fire":({"hit", }),  
	  "name":"击退", ]),
 
]);

mapping get_data()
{
	return data;
}
// from tmp/data/技能表.xls
// -------------------  Auto Generate End   --------------------
