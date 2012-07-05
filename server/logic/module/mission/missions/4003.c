// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4004", "40031", "40032", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4002")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "麒麟崖顶")
// 任务名称
RESET_ONUPDATE_VAR(Name, "麒麟崖西")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关麒麟崖顶)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "那么麒麟很有可能在崖的西面，路上小心越往深处走，越容易遇到凶猛的异兽。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 7)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 26)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "能够击败圣兽麒麟，诸位的实力真是非同一般啊，还望各位相助救回黑土。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 63600)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 141000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "自当尽力。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "完成麒麟崖西部区域的搜索。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 7)
// 任务id
RESET_ONUPDATE_VAR(Id, "4003")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 42)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
