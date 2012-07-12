// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"10112", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "1011")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 任务名称
RESET_ONUPDATE_VAR(Name, "再领装备")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(与大娘对话)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "我还叫大娘为你特别缝制了一套衣衫，你去找她领了来吧。")
// 物品奖励
RESET_ONUPDATE_VAR(Items, (["ZH0001":5, ]))
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "来，就是这一套了，大娘看着你长大的，大小肯定合适。")
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 100)
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 3)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 100)
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 3)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "谢谢您，真是很合身啊。")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 1)
// 任务id
RESET_ONUPDATE_VAR(Id, "10111")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "与大娘对话。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------


















