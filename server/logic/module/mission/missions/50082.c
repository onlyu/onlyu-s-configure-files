// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5008")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 20)
// 任务名称
RESET_ONUPDATE_VAR(Name, "收集鱿鱼须")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(天地龙门一)收集鱿鱼须")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "鱿鱼须？好说。海底多的是鱿鱼，我去取来。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "爷爷为了治疗敏儿的眼疾，又偷偷下海去寻取鱿鱼须，回来时奄奄一息。敏儿好伤心！敏儿宁愿眼疾不恢复，也不要爷爷成日出事！")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 14)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 20)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "鱿鱼须？太好了，这下爷爷不用再偷偷的下海了。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 137800)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 201000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "敏儿乖，要快高长大才能好好报答爷爷。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 14)
// 任务id
RESET_ONUPDATE_VAR(Id, "50082")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "打败小守卫，收集足够的鱿鱼须。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
