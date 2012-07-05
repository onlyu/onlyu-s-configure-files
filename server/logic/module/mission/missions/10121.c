// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"10122", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "1012")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 任务名称
RESET_ONUPDATE_VAR(Name, "强化装备")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(与铁匠对话)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "强化之后，不论武器还是防具的属性都会得到大幅增长。当遇到强敌时，强化装备可让战斗更轻松。")
// 物品奖励
RESET_ONUPDATE_VAR(Items, (["ZH0001":8, ]))
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "哈哈，你带来的材料真不错，用来强化装备再好不过了。")
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 2)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 300)
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 2)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "请您教我强化之法吧。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "10121")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "与铁匠对话。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
