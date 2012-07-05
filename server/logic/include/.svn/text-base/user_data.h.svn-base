// 用户数据部分
// ------------------------------------
#ifndef __USER_DATA_H__
#define __USER_DATA_H__

// 持久化到DB的变量
static string * _GDBArray = ({"prop", "skill", "mission", "friends", "item", "summon", "equip", "status", });

// 存在内存中的变量，重启消失
static string * _GMemoryArray = ({ "item", "summon", "temp_var", "temp_att", });

// 用户ID
int usernum = 0;

// 以下数据在引擎内部做脏数据管理,可以直接同步DB
// ------------------------------------------------------
// 基本属性
mapping prop = ([]);
// 技能
mapping skill = ([]);
// 任务属性
mapping mission = ([]);
// 好友属性
mapping friends = ([]);
// 外部状态
mapping status = ([]);
// ------------------------------------------------------
mapping item = ([]);

mapping summon = ([]);

//装备的存盘数据，包括：装备栏、装备集, 材料集 和自定义装备组
mapping equip = ([]);


//以下是非存盘的数据
//------------------------------
//物品的背包信息
mapping item_frame = ([]);
//召唤兽的背包信息
mapping summon_frame = ([]);
// 临时技能
mapping temp_skills = ([]);
// 法宝临时数据
mapping temp_fabao = ([]);
// ------------------------------------------------------


// 下面数据临时存贮，无需同步DB
// ------------------------------------------------------
static mapping temp_var = ([]);
// 临时属性,用来存放一些临时战斗属性,方便复制到战斗中
static mapping temp_att = ([]);
// ------------------------------------------------------

#endif // __USER_DATA_H__

