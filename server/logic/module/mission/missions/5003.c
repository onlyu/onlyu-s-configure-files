// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5004", "50031", "50032", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5002")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "觐见龙王")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(水晶宫)觐见龙王")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "既已料理了宫门守卫，就赶紧进入水晶宫仔细查探吧，那水晶宫一向祥和，近日戾气外露，定有变故。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 16)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "情况怎样？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 127400)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 224000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "东海有小龙被巫族抓走，龙王请我等相救。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 5)
// 任务id
RESET_ONUPDATE_VAR(Id, "5003")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "再探水晶宫。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
