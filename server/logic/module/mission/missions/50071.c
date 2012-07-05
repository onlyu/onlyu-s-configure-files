// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5007")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "踏仙台上")
// 任务名称
RESET_ONUPDATE_VAR(Name, "教训拉蛮头领")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(踏仙台上)教训拉蛮头领")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "女子小孩皆不放过，一看就不是什么好鸟！待我去教训教训他。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "巫族那个叫拉蛮头领的小头目，不光抢掠，还对夕颜图谋不轨。敏儿看见了上前争执，还被他放蛇吓唬，实在可恨之极！")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 13)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 19)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "真是大快人心，为我们渔村百姓出了口气！")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 130000)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 194000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "小事一桩。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 20)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 13)
// 任务id
RESET_ONUPDATE_VAR(Id, "50071")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "去教训拉蛮头领。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
