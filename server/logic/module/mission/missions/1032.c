// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"2001", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "1031")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "昆仑山颠")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(登上昆仑山巅)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "你就依姜子牙仙长的嘱咐，快去昆仑山颠使用天书之力吧。")
// 物品奖励
RESET_ONUPDATE_VAR(Items, (["ZH0001":13, ]))
// 副本id
RESET_ONUPDATE_VAR(FubenId, 6)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "不周山？那已经是一个遥远的传说了。")
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 4)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 900)
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 1)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "您有什么线索吗。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "1032")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "登上昆仑山巅。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
