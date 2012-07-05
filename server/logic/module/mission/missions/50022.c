// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5002")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "鳄鱼守卫")
// 任务名称
RESET_ONUPDATE_VAR(Name, "修理鳄鱼")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(东海龙宫二)修理鳄鱼")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "虽说敏儿胆子小了点，但这海里的鳄鱼也不需要天天上岸以吓唬她为乐啊！你帮我去修理下那些作恶的鳄鱼吧。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 13)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 15)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "这下可出了口恶气。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 95600)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 162000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "举手之劳。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 38)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 13)
// 任务id
RESET_ONUPDATE_VAR(Id, "50022")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "打败那些老上岸吓唬小孩的鳄鱼。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
