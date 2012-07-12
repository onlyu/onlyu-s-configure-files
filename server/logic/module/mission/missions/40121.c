// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4012")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "金环蛇")
// 任务名称
RESET_ONUPDATE_VAR(Name, "金环蛇")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(消灭金环蛇)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "金环蛇凶暴残忍、威胁极大，帮我消灭他们。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 8)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 33)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "金环蛇除掉了么？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 83900)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 144000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "已经搞定了。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 40)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "消灭40名金环蛇")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 8)
// 任务id
RESET_ONUPDATE_VAR(Id, "40121")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 49)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
