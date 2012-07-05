// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"1032", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "1022")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "玉虚宫前")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(前往玉虚宫)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "你便随这仙童前往玉虚宫吧，你的使命我已经看不透了。")
// 物品奖励
RESET_ONUPDATE_VAR(Items, (["ZH0001":12, ]))
// 副本id
RESET_ONUPDATE_VAR(FubenId, 5)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "姜子牙仙长都出现了，我以前在山上学道时都未能见他几面。")
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 4)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 700)
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 1)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "确实仙风道骨。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "1031")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "前往玉虚宫。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
