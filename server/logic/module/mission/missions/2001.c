// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"2011", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "1032")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 任务名称
RESET_ONUPDATE_VAR(Name, "京城奇人")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(找到济天下)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "我听闻在皇城有一个奇人，名济天下，自称天下事无所不知，如果是要在人间寻找女娲遗址，我想你们不妨去寻访他，或可了解一二。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 10)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "哈哈哈，这天下事还少有吾不知……什么，你们想问女娲遗迹……这，这是咩啊- -！")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 10)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 1100)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "莫非连你也不知道么。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "2001")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "找到济天下")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
