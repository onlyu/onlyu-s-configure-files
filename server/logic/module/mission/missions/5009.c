// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5010", "50091", "50092", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5008")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "再战守卫")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "去$(天地龙门三)与水护者一战")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "天地龙门之三重守卫乃水护者，快去接招吧。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 22)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "水护者已破，佩服佩服。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 182400)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 278000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "客气客气。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 5)
// 任务id
RESET_ONUPDATE_VAR(Id, "5009")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "击败天地龙门之天护者。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
