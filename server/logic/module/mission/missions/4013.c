// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_FIND_NPC;
#include <var_prop.h>

 
// 分派任务
RESET_ONUPDATE_VAR(NextIds, ({"5001", }))
// 前置任务
RESET_ONUPDATE_VAR(PreId, "4012")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_MAIN)
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 1)
// 任务名称
RESET_ONUPDATE_VAR(Name, "灵石")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "$(与东海灵石对话)")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我这就去。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "感谢你们所做的一切，听闻诸位还要找寻蛟龙的下落，我年轻的时候曾经去过一个地方，叫什么东海渔村，村口供奉着一座灵石，关于东海之事无所不知，你们去问问看吧。")
// 交任务NPC
RESET_ONUPDATE_VAR(CompleteNpc, 5)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "(灵石发出五彩的光芒，好像在欢迎你的到来。)")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 5)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 21900)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 40000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "真神奇……")
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_FIND_NPC)
// 任务描述
RESET_ONUPDATE_VAR(Desc, "与东海灵石对话。")
// 接任务NPC
RESET_ONUPDATE_VAR(AcceptNpc, 6)
// 任务id
RESET_ONUPDATE_VAR(Id, "4013")
// 最低等级
RESET_ONUPDATE_VAR(MinGrade, 50)

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
