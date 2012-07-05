// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5006", "50051", "50052", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5004")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "再救龙裔")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(踏仙台二)营救龙裔")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "小龙不救回，龙王一定忧心万分，英雄歇息够了就上路吧。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 18)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "巫族喽啰纵然厉害，想来也敌不过几位英雄。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 144200)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 242000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "过奖了，我等还需再入踏仙台。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 5)
// 任务id
RESET_ONUPDATE_VAR(Id, "5005")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "再探踏仙台。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
