// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"1031", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "1021")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "天书入体")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(斩杀妖邪)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "山上定然出了大事，否则这些妖邪只会远避。你继续上山一探究竟，遇到妖物拦路尽管斩杀。")
// 物品奖励
RESET_ONUPDATE_VAR(Items, (["ZH0001":11, ]))
// 副本id
RESET_ONUPDATE_VAR(FubenId, 4)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "没想到平日被管束的蛟精也想作乱。不过你竟然得到天书入体，又开启了天命的神格。乱世降至，看来你是结束这乱世的关键人物。")
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 4)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 500)
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 1)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "还请您指引我。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "1022")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "斩杀妖邪。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
