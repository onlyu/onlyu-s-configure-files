// -------------------  Auto Generate Begin --------------------


#include "/module/mission/mission.h"
inherit MISSION_KILL_NPC;
#include <var_prop.h>

 
// 前置任务
RESET_ONUPDATE_VAR(PreId, "5006")
// 任务类型
RESET_ONUPDATE_VAR(Type, TYPE_EXT)
// 怪物名称
RESET_ONUPDATE_VAR(EnemyName, "巫鸢尖兵")
// 任务名称
RESET_ONUPDATE_VAR(Name, "收集纸鸢")
// 追踪文本
RESET_ONUPDATE_VAR(FollowText, "前往$(踏仙台二)收集纸鸢")
// 接任务菜单选项
RESET_ONUPDATE_VAR(AcceptOption, "我去取来。")
// 接任务对话
RESET_ONUPDATE_VAR(AcceptTalk, "那些巫族也不是一无是处，一次劫掠后落下只纸鸢，精巧得很，敏儿好生喜欢。要是能多一些分给村里的小朋友们玩就好了。")
// 交任务Npc
RESET_ONUPDATE_VAR(CompleteNpc, 14)
// 副本id
RESET_ONUPDATE_VAR(FubenId, 18)
// 交任务对话
RESET_ONUPDATE_VAR(CompleteTalk, "好多纸鸢，够小伙伴们玩耍一阵了。")
// 追踪Npc
RESET_ONUPDATE_VAR(FollowNpc, 16)
// 经验奖励
RESET_ONUPDATE_VAR(Exp, 122400)
// 金钱奖励
RESET_ONUPDATE_VAR(Money, 188000)
// 交任务菜单选项
RESET_ONUPDATE_VAR(CompleteOption, "愿能搏你们一乐。")
// 目标数量
RESET_ONUPDATE_VAR(TargetNum, 38)
// 任务模版
RESET_ONUPDATE_VAR(Template, MISSION_KILL_NPC)
// 接任务Npc
RESET_ONUPDATE_VAR(AcceptNpc, 14)
// 任务id
RESET_ONUPDATE_VAR(Id, "50062")
// 任务描述
RESET_ONUPDATE_VAR(Desc, "去收集一些纸鸢。")

 

// from tmp/任务剧情/missions.xls
// -------------------  Auto Generate End   --------------------
