// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"2016", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "2015")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "破境（一）")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(探索幻境1)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "好，就请先生指路。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "无妨，只要能感应到星力，我便可以指引方向。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 11)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 11)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "果然与我所料不差，我们走了许久，方位却没怎么变过。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 12)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 3400)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "还请先生赐教。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 11)
// 任务id
RESET_ONUPDATE_VAR(Id, "20151")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "探索幻境1")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
