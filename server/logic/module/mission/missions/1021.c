// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"1022", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "10122")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "上昆仑山")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(上昆仑山)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "昆仑山上有众神镇守天地，下有我这样的护道人维护一方平安。但前不久我竟和山门失去联系，我想是发生了什么事情，希望你能替我上山求问仙长。")
// 物品奖励
RESET_ONUPDATE_VAR(Items, (["ZH0001":10, ]))
// 副本id
RESET_ONUPDATE_VAR(FubenId, 3)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "昆仑山上竟然有妖邪拦路？")
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 4)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 400)
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 1)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "仿佛无人管束在争夺灵气。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "1021")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "上昆仑山。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
