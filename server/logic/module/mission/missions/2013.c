// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"2014", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "2012")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "登司天台")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(前往司天台)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "不好意思，我只是感慨一下下。观星图变化，记九州分野乃司天监职责，我想司天台上司天监可能会知道如今不周山所在吧。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 10)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 9)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "每天晚上司天监便在台上观星，现在上去应该便可以见到他。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 12)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 2000)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "现在便可登天台。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 10)
// 任务id
RESET_ONUPDATE_VAR(Id, "2013")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "前往司天台")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
