// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5009", "50081", "50082", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5007")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "破除封印")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "去$(天地龙门二)与地护者一战")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "天地龙门之二重守卫乃地护者，快去接招吧。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 21)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "地护者已破，佩服佩服。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 172300)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 269000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "我等还需努力。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 5)
// 任务id
RESET_ONUPDATE_VAR(Id, "5008")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "击败天地龙门之天护者。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
