// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"3001", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "2017")
// 任务名称
RESET_ONUPDATE_VAR(Name, "前往不周山")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(前往不周山)")
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 11)
// 任务id
RESET_ONUPDATE_VAR(Id, "2018")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
