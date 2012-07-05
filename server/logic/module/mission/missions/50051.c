// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5005")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "巫魔尖兵")
// 任务名称
RESET_ONUPDATE_VAR(Name, "修理巫魔尖兵")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(踏仙台一)修理巫魔尖兵")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "近日巫族出没，在渔村烧杀抢掠，实在可恨之极。英雄如此骁勇，能帮我们教训一下那些恶心的巫魔尖兵吗？")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 13)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 17)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "真是大快人心！")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 115300)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 181000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "小事一桩。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 38)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 13)
// 任务id
RESET_ONUPDATE_VAR(Id, "50051")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "去教训巫魔尖兵。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
