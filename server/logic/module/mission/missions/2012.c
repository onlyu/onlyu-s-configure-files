// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FINISH_FUBEN;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"2013", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "2011")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "遍查群书")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(入藏书阁内)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "……最珍贵的藏书都在藏书阁内，要是给我一点时间查阅，定然不负恩泽。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 10)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 8)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "白云苍狗多翻覆，沧海桑田几变更……想这不周山虽然是极出名的，但世易时移早已不知仙山何处啊……")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 12)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 1600)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "说重点！！！")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FINISH_FUBEN)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 10)
// 任务id
RESET_ONUPDATE_VAR(Id, "2012")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "进入藏书阁")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
