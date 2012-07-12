// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4009")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "无劫山一")
// 任务名称
RESET_ONUPDATE_VAR(Name, "清理毒物（一）")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关无劫山一)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "无劫山瘴气重重、毒物遍地，帮我除掉一些，以防镇上百姓为其所伤。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 8)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 30)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "之前的事情办得怎么样了？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 68300)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 126000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "已经搞定了。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 20)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "通关20次无劫山一")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 8)
// 任务id
RESET_ONUPDATE_VAR(Id, "40092")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 46)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
