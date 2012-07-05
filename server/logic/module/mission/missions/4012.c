// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"4013", "40121", "40122", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4011")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "无劫山顶")
// 任务名称
RESET_ONUPDATE_VAR(Name, "蛇衔草")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(通关无劫山顶)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "作为药引的蛇衔草只生长在无极山顶，据说还有异兽守护其左右，请一定小心。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 6)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 33)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "各位拿到蛇衔草了么？")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 9)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 104900)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 192000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "蛇衔草拿到了。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "取得蛇衔草。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 6)
// 任务id
RESET_ONUPDATE_VAR(Id, "4012")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 49)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
