// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4010", "40091", "40092", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4008")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "无劫山一")
// 任务名称
RESET_ONUPDATE_VAR(Name, "毒蜥卵")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关无劫山一)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "这第一种药材毒蜥卵，通体白净，有少量淡蓝色的半点，能补肺养血、滋阴润燥，可以在毒蜥蜴的巢穴里找到。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 6)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 30)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "各位拿到毒蜥卵了么？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 85400)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 169000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "毒蜥卵拿到了。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "取得毒蜥卵。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 6)
// 任务id
RESET_ONUPDATE_VAR(Id, "4009")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 46)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
