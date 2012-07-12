// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5009")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 20)
// 任务名称
RESET_ONUPDATE_VAR(Name, "龙门切磋（1）")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(天地龙门一)与天护者切磋武艺")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "英雄左右无事，不如再去找龙门的天护者切磋切磋技艺吧，也可多学几招。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 15)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 20)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "英雄果然威武，竟能多次挫败天护者。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 145900)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 208000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "切磋而已无需挂齿。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 15)
// 任务id
RESET_ONUPDATE_VAR(Id, "50091")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "与天护者切磋技艺。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
