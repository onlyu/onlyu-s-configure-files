// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5001")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 20)
// 任务名称
RESET_ONUPDATE_VAR(Name, "收集避水珠")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(东海龙宫一)收集避水珠")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "渔民们时有出海捕鱼遇风浪而溺亡的，虽然那避水珠我等凡人携带了也不可自由入海，但出了意外总是能保住性命。听说英雄入海取过避水珠，能否帮渔民们寻一些回来呢？")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 13)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 14)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "英雄真是功德无量！")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 89600)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 156000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "希望能造福渔民。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 13)
// 任务id
RESET_ONUPDATE_VAR(Id, "50011")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "去东海龙宫1击败怪物，取回避水珠。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
