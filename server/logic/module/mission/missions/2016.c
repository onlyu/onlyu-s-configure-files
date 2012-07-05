// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"2017", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "20151")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "破境（二）")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(探索幻境2)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我们继续跟随您前行。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "这个幻境只是将我们的意识拖入，其实我们一直在司天台上。若我所料不差，那个巫族祭祀应该在持续施法维持幻境，找到他便有办法破境而出。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 11)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 12)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "找到了，那个巫族祭祀的意识就躲藏在这幻境深处，方向就和他在司天台的站位一样。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 12)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 4100)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "我们去把他揪出来。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 11)
// 任务id
RESET_ONUPDATE_VAR(Id, "2016")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "探索幻境2")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
