// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5003", "50021", "50022", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5001")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "探访受阻")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(东海龙宫二)打探水灵珠下落")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "我在这里受海水冲刷已有数百年，自是听闻过水灵珠这件至宝。只是据说此乃东海龙王收藏的宝物,轻易不得见之。你去水晶宫打探看看，或许会有收获。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 15)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "此行收获如何？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 119500)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 216000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "果然有古怪，那水晶宫竟有守卫将我等拦在门外，被我等料理了。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 5)
// 任务id
RESET_ONUPDATE_VAR(Id, "5002")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "前往水晶宫查探究竟。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
