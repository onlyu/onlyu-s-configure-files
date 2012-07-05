#include "/module/fight/warrior.h"
#include "/module/fight/skill.h"
#include "/module/fight/status.h"

// -------------------  Auto Generate Begin --------------------

mapping data = ([ 
 
	"ADMelee":([ 
	  "status":"AD",  
	  "melee":1,  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1; }, ]),
	"ADRange":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1; }, ]),
	"MD":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*1; }, ]),
	"SPUp40":([ 
	  "status":"SPUp",  
	  "targetCamp":CAMP_ME,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SPUp)=40; }, ]),
	"SPUp60":([ 
	  "status":"SPUp",  
	  "targetCamp":CAMP_ME,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SPUp)=60; }, ]),
	"SPDown40":([ 
	  "status":"Spdown",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SPDown)=40; }, ]),
	"SPDownAll":([ 
	  "status":"Spdown",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SPDown)=SP(t); }, ]),
	"ActDown20":([ 
	  "status":"ActDown",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(ActDown)=0; }, ]),
	"Stun45":([ 
	  "status":"Stun",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 450+YC_HIT(w)-YC_DODGE(t); }, ]),
	"Stun60":([ 
	  "status":"Stun",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 600+YC_HIT(w)-YC_DODGE(t); }, ]),
	"Stun75":([ 
	  "status":"Stun",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 750+YC_HIT(w)-YC_DODGE(t); }, ]),
	"Conf60":([ 
	  "status":"Conf",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 600+YC_HIT(w)-YC_DODGE(t); }, ]),
	"SpeUp20":([ 
	  "status":"SpeUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SpeUp)=SPEED(t)+min(QIANGFA(w),SPEED(t))*0.2; }, ]),
	"SpeUp30":([ 
	  "status":"SpeUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SpeUp)=SPEED(t)+min(QIANGFA(w),SPEED(t))*0.3; }, ]),
	"SpeDown20":([ 
	  "status":"SpeDown",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(SpeDown)=SPEED(t)-min(QIANGFA(w),SPEED(t))*0.2; }, ]),
	"AttUp20":([ 
	  "status":"AttUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AttUp)=ATTACK(t)+min(ATTACK(w),ATTACK(t))*0.2; }, ]),
	"AttDown20":([ 
	  "status":"AttDown",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AttDown)=ATTACK(t)-min(ATTACK(w),ATTACK(t))*0.2; }, ]),
	"MagUp20":([ 
	  "status":"MagUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MagUp)=QIANGFA(t)+min(QIANGFA(w),QIANGFA(t))*0.2; }, ]),
	"MagDown20":([ 
	  "status":"MagDown",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MagDown)=QIANGFA(t)-min(QIANGFA(w),QIANGFA(t))*0.2; }, ]),
	"DefUp20":([ 
	  "status":"DefUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(DefUp)=DEFENCE(t)+min(ATTACK(w),DEFENCE(t))*0.2; }, ]),
	"DefDown20":([ 
	  "status":"DefDown",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(DefDown)=DEFENCE(t)-min(ATTACK(w),DEFENCE(t))*0.2; }, ]),
	"ResUp20":([ 
	  "status":"ResUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(ResUp)=KANGFA(t)+min(QIANGFA(w),KANGFA(t))*0.2; }, ]),
	"ResDown20":([ 
	  "status":"ResDown",  
	  "targetCamp":CAMP_ENEMY,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(ResDown)=KANGFA(t)-min(QIANGFA(w),KANGFA(t))*0.2; }, ]),
	"CriUp30":([ 
	  "status":"CriUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(CriUp)=0.3; }, ]),
	"ParUp30":([ 
	  "status":"ParUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(ParUp)=0.3; }, ]),
	"DodUp30":([ 
	  "status":"DodUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(DodUp)=0.3; }, ]),
	"CAUp30":([ 
	  "status":"CAUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(CAUp)=0.3; }, ]),
	"ERUp10":([ 
	  "status":"ERUp",  
	  "targetCamp":CAMP_ME,  
	  "bout":2,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(ERUp)=0.1; }, ]),
	"leitingwanjun":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=99999999; }, ]),
	"huangquanchandou":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=99999999; }, ]),
	"liudaolunhui":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=99999999; }, ]),
	"zhanji":([ 
	  "status":"AD",  
	  "melee":1,  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.5; }, ]),
	"faxianghuti":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.25; }, ]),
	"fengzhou":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*1.5; }, ]),
	"benyue":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.65; }, ]),
	"luorishenjian":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(QIANGFA(w)-KANGFA(t))*0.85; }, ]),
	"tianhai":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return 1000; },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*1.3; }, ]),
	"chunyangjian":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.35; }, ]),
	"lieyanranlei":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*0.55; }, ]),
	"duopojinling":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.75; }, ]),
	"fengxing":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*0.95; }, ]),
	"zhouyu":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.6; }, ]),
	"xiaoxiangdiyin":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*0.65; }, ]),
	"tageerxing":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*0.75; }, ]),
	"shuilongyin":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*1.4; }, ]),
	"huakaiyishun":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.25; }, ]),
	"lingboweibu":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.7; }, ]),
	"longdan":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.25; }, ]),
	"batiwushuang":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.25; }, ]),
	"zhenshequnmo":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*0.65; }, ]),
	"hunfeiqianli":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.65; }, ]),
	"hongxiutianxiang":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.35; }, ]),
	"xizipengxin":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.6; }, ]),
	"jiuzhuanxuangong":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*1.6; }, ]),
	"guosetianxiang":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.3; }, ]),
	"santouliubi":([ 
	  "status":"AD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(AD)=(ATTACK(w)-DEFENCE(t))*0.85; }, ]),
	"tianyinfumo":([ 
	  "status":"MD",  
	  "targetCamp":CAMP_ENEMY,  
	  "rate":function (class Warrior w, class Warrior t) { return HIT(w)-DODGE(t); },  
	  "preDo":function (class Skill sk, class Status st, class Warrior w, class Warrior t) { P(MD)=(QIANGFA(w)-KANGFA(t))*0.6; }, ]),
 
]);

mapping get_data()
{
	return data;
}
// from tmp/data/技能表.xls
// -------------------  Auto Generate End   --------------------
