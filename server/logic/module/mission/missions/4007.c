// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4008", "40071", "40072", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4006")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "山寨内")
// 任务名称
RESET_ONUPDATE_VAR(Name, "幕后黑手")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关山寨内)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "大巫祝就住在巫族山寨中最高的那顶帐篷里，击败他救回黑土吧。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 8)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 29)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "感谢各位救回了黑土，各位的大恩在下没齿难忘。不过巫民们所中的蛊毒已深，若不尽快解毒的话……")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 79500)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 162000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "可有良策？")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "在巫族山寨中寻找黑土的下落。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 8)
// 任务id
RESET_ONUPDATE_VAR(Id, "4007")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 45)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
