// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4002", "40011", "40012", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4000")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "麒麟崖一")
// 任务名称
RESET_ONUPDATE_VAR(Name, "麒麟崖东")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关麒麟崖一)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "先从崖的东面开始，看看能否有所发现，切记天黑之前一定要回来。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 7)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 24)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "此行收获如何，有没有发现麒麟的踪迹？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 54300)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 128000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "没发现什么特别的。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "完成麒麟崖南部区域的搜索。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 7)
// 任务id
RESET_ONUPDATE_VAR(Id, "4001")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 40)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
