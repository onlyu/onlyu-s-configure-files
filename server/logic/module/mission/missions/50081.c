// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5008")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "拉蛮头领")
// 任务名称
RESET_ONUPDATE_VAR(Name, "收集巫族腰牌")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(踏仙台上)收集巫族腰牌")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "看来是个有用的东西，我去设法收集一些来，应能暂护妇孺周全。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "你看这个牌子，那次拉蛮来抢东西时落下的，被我捡到了。是不是很好看？拿着这个牌子，其他巫族怪物都不伤害我了，真好，嘻嘻！")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 14)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 19)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "咦，你取来那么多一样的牌子。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 137800)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 201000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "敏儿把这些牌子分给夕颜姐姐和其他小朋友们吧，有了这个巫族小怪物就不欺负你们了。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 20)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 14)
// 任务id
RESET_ONUPDATE_VAR(Id, "50081")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "打败拉蛮头领，收集足够的巫族腰牌。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
