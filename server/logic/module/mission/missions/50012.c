// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5001")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "龙尾龟")
// 任务名称
RESET_ONUPDATE_VAR(Name, "收集龟壳")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(东海龙宫一)收集龟壳")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "世事多变天象难测，日日占卜龟壳的消耗量还真大。好在这海里多的是龟，你能帮我收集些龟壳来吗？")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 13)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 14)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "多谢你了，有了这龟壳我也可以靠占卜来趋吉避凶，助我等渔民出海平安。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 89600)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 156000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "何需客气。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 38)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 13)
// 任务id
RESET_ONUPDATE_VAR(Id, "50012")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "打败龙尾龟，收集龟壳。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
