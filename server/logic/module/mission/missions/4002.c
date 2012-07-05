// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4003", "40021", "40022", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4001")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "麒麟崖二")
// 任务名称
RESET_ONUPDATE_VAR(Name, "麒麟崖北")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关麒麟崖二)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "这次往崖的北面看看，传闻有人在那边看见过麒麟。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 7)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 25)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "这次有没有什么发现？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 58800)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 134000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "还是没什么特别的踪迹。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "完成麒麟崖北部区域的搜索。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 7)
// 任务id
RESET_ONUPDATE_VAR(Id, "4002")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 41)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
