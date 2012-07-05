// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4005", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4003")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "危机")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(与黄土对话)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "事不宜迟，赶紧去找黄土商量。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 8)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "感谢各位相助，掳走小女的乃是巫族兵士，他们的山寨就在镇子南面，镇长要赤土留下来加强镇上的防御，防止敌人再次来袭，就让我陪各位一同前往。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 8)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 14100)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 29600)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "事不宜迟，赶紧出发吧。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "与黄土对话。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 7)
// 任务id
RESET_ONUPDATE_VAR(Id, "4004")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 43)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
