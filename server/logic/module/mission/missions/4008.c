// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4009", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4007")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "解药")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(与镇长对话)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "这样吧，先去问问镇长，看有没有办法。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 6)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "诸位来得正好，大巫祝已经在调配蛊毒的解药了，但还缺几样药材。所幸的是这些药材在西北面的无极山中都能找到，只是这无极山瘴气甚重，遍布毒物，常人无法前往，只能烦劳各位了。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 6)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 17200)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 33900)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "乐意效劳。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "与镇长对话。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 8)
// 任务id
RESET_ONUPDATE_VAR(Id, "4008")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 46)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
