// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5007", "50061", "50062", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5005")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "马到功成")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(踏仙台上)营救龙裔")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "时日久了恐生变故，英雄此行可务必要将小龙们救回，我替这东海里的水族同胞们多谢几位了！")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 19)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "东海传遍了英雄将小龙救回的事迹，是整个水族的恩人了！")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 153100)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 250000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "何需如此客气，路见不平拔刀相助乃我修行之人分内之事。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 5)
// 任务id
RESET_ONUPDATE_VAR(Id, "5006")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "速至踏仙台上营救小龙。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
