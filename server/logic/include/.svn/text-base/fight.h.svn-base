// 战斗头文件
// ---------------------------------------

#ifndef __FIGHT_H__

#define __FIGHT_H__

#include <user_key.h>

#define FIGHT_PATH "module/fight/"
// 战斗主模块
#define FIGHT_MAIN FIGHT_PATH"main"
// 战斗工具模块
#define FIGHT_UTIL FIGHT_PATH"util"
// 战斗数据模块
#define FIGHT_DATA FIGHT_PATH"fight_data"
// 战斗客户端协议管理模块
#define CLIENT_UTIL FIGHT_PATH"client_util"
// 战斗数值基类
#define FIGHT_DATA_BASE FIGHT_PATH"data_base" 
// 技能路径
#define SKILL_PATH "module/skill/"
// 状态路径
#define STATUS_PATH "module/status/"
// 状态通用用具
#define STATUS_UTIL STATUS_PATH"util" 
// 阵形文件
#define FIGHT_LINEUP FIGHT_PATH"lineup"

// 战斗NPC起始ID
#define FIGHT_NPC_START_ID (0x3FFFFFFF)

// 无敌状态
#define WUDI_STATUS "wudi"
#define SW_WUDI_STATUS "shengwu_wudi"
// 蓄力状态
#define XULI_STATUS "xuli" 
// 死亡状态
#define DIE_STATUS "die" 
// 隐身状态
#define YINSHEN_STATUS "yinshen" 
// 清谈状态
#define QINGTAN_STATUS "qingtan" 
// 被冰
#define SEAL_STATUS "seal"
// CD状态
#define CD_STATUS "cd" 
// 伤害状态
#define DAMAGE_STATUS "damage"
// 战斗数值修改状态
#define VALUE_STATUS "value_status" 
// 骷髅数值状态(本状态为战场状态)
#define KULOU_STATUS "kulou"
// 灵魂数值状态(本状态为战场状态)
#define LINGHUN_STATUS "linghun"

// 技能基类
#define SKILL_BASE SKILL_PATH"base" 
// 范围选择技能
#define SKILL_POSITION SKILL_PATH"position_skill" 
// 目标技能
#define SKILL_TARGET SKILL_PATH"target_skill" 
// 法宝技能
#define SKILL_FABAO SKILL_PATH"fabao_skill" 
// 被动技能
#define SKILL_PASSIVE SKILL_PATH"passive_skill" 
// 属性技能
#define SKILL_ATTRIBUTE SKILL_PATH"attribute_skill" 
// 帮派属性技能
#define SKILL_ORG_ATTRIBUTE SKILL_PATH"org_attr_skill" 
// 心法属性技能
#define SKILL_XINFA_ATTRIBUTE SKILL_PATH"xinfa_attr_skill" 

// 技能实用工具
#define SKILL_UTIL SKILL_PATH"util" 
// 站位模块
#define FIGHT_POSITION "module/fight/position"
// 战斗AI
#define FIGHT_AI "module/fight/ai"

// 状态基类
#define STATUS_BASE STATUS_PATH"base" 
// 可叠加状态基类
#define STATUS_STACKABLE STATUS_PATH"stackable"
// 战场状态 
#define FIGHT_STATUS_BASE STATUS_PATH"fight_status"
// 战场可叠加状态 
#define FIGHT_STATUS_STACKABLE STATUS_PATH"stackable_fight_status"
// 法宝状态
#define STATUS_FABAO STATUS_PATH"fabao"


// 玩家 
#define WI_TYPE_USER (99)
// 召唤兽
#define WI_TYPE_SUMM (1)
// NPC
#define WI_TYPE_NPC  (2)
// 法宝
#define WI_TYPE_FABAO (3)

// 战斗处于的状态
// 战斗初始化中
#define FIGHT_STAT_INITING (-1)
// 战斗等待指令中
#define FIGHT_STAT_WAIT_CMD (0)
// 战斗等待客户端绘图中
#define FIGHT_STAT_WAIT_DRAW (1)
// 战斗结束，后处理中
#define FIGHT_STAT_END (99)

// 攻击方
#define FIGHT_SIDE_ATTACK (1)
// 防守方
#define FIGHT_SIDE_DEFENSE (2)
// 客户端用同边
#define CLIENT_SIDE_SELF (1)
// 客户端用对边
#define CLIENT_SIDE_OTHER (2)

// 战斗最大回合数
#define MAX_BOUT (200)

// 默认战斗指令发送协议时间
#define MAX_CMD_TIME (60)
#define MIN_CMD_TIME (10)

// --------------------------------------------------------------
// 战斗常用的Key
// fightId
#define I_FIK_FIGHT_ID "fight_id"
// 战斗进行回合数
#define I_FIK_BOUT "bout"
// 下一个动作时间点
#define I_FIK_NEXT_TIME "next_time"
// 命令等待时长
#define I_FIK_CMD_TIME "cmd_time"
// 战斗的绘画时间
#define I_FIK_DRAW_TIME "draw_time"
// 战斗开始时间
#define I_FIK_BEGIN_TIME "begin_time"
// 战斗的绘画时间余数
#define F_FIK_DRAW_TIME_REMAINDER "draw_time_rem"
// 战斗中禁止召唤
#define I_FIK_CANNOT_SUMMON_FLG "cannot_summon_flg"
// 禁止法宝
#define I_FIK_CANNOT_FABAO_FLG "cannot_fabao_flg"
// 战斗类型
#define C_FIK_FIGHT_TYPE "fight_type" 
// 战斗所处状态 
#define I_FIK_FIGHT_STATUS "status"
// 战斗处理handler
#define I_FIK_FIGHT_HANDLER "handler"
// 战斗逃跑处理handler
#define I_FIK_ESCAPE_HANDLER "esc_handler"
// 战斗捕抓处理handler
#define I_FIK_CATCH_HANDLER "catch_handler"
// 战斗死亡处理handler
#define I_FIK_DIE_HANDLER "die_handler"
// 战斗空战标记
#define I_FIK_AIR_WAR_FLG "air_war_flg"
// 战斗处理传递的参数
#define I_FIK_HANDLER_ARGS "args"
// 战斗逃跑传递的参数
#define I_FIK_ESC_HANDLER_ARGS "esc_args"
// 战斗捕抓传递的参数
#define I_FIK_CATCH_HANDLER_ARGS "catch_args"
// 战斗死亡传递的参数
#define I_FIK_DIE_HANDLER_ARGS "die_args"
// 处理AI用object
#define C_FIK_AI_FILE "ai_file"
// 战场AI
#define A_FIK_FIGHT_AI_LIST "fight_ai_list"
// 死亡复活地点
#define A_FIK_REBORN_POS "reborn_pos"
// 逃跑重入场地点
#define A_FIK_ESCAPE_POS "escape_pos"
// 战斗地图 
#define I_FIK_WAR_MAP "war_map"
#define I_FIK_WAR_POS_X "war_pos_x"
#define I_FIK_WAR_POS_Y "war_pos_y"
// 战斗攻击方key
#define A_FIK_ATTACK_SIDE "attack_side"
// 战斗攻击方用户key
#define A_FIK_ATTACK_USER "attack_user"
// 战斗攻击方法宝
#define A_FIK_ATTACK_FABAO "attack_fabao"
// 战斗防御方key
#define A_FIK_DEFENSE_SIDE "defense_side"
// 战斗防御方用户key
#define A_FIK_DEFENSE_USER "defense_user"
// 战斗防御方法宝
#define A_FIK_DEFENSE_FABAO "defense_fabao"
// 攻击方阵型
#define I_FIK_ATTACK_LINEUP  "attack_lineup"
// 防守方阵型
#define I_FIK_DEFENSE_LINEUP  "defense_lineup"
// 战斗观战攻击方Key
#define A_FIK_ATTACK_WITNESS "attack_witness"
// 战斗观战防御方Key
#define A_FIK_DEFENSE_WITNESS "defense_witness"
// 战士的技能编号
#define M_FIK_SKILLS "skills"
// 战场状态Key
#define M_FIK_FIGHT_STATUS "fight_status"
// 战斗攻击方站位
#define M_FIK_ATTACK_POSITION "attack_position"
// 战斗防守方站位 
#define M_FIK_DEFENSE_POSITION "defense_position"
// 战场额外数据,用来给AI和程序逻辑用
#define M_FIK_EXTEND_DATA "extend_data"
// 战斗中速度变更标记
#define I_FIK_SPE_CHANGE_FLG "spe_change_flg"
// 战斗消耗方式
#define I_FIK_CONSUME_METHOD "consume_method"
// 战斗死亡惩罚(扣经验)
#define I_FIK_DIE_PUNISH     "die_punish"
// 战士常用Key
#define A_FWK_CMD "cmd"
// 战士ID 
#define I_FWK_ID  "id"
// 战士所在side
#define I_FWK_SIDE "side"
// 战士类型
#define I_FWK_TYPE "type"
// 战士造型
#define I_FWK_LAYOUT "layout"
// 战士名字
#define I_FWK_NAME "name"
// 战士武器强法属性
#define C_FWK_EQUIP_ATTR "equip_attr"
// 门派
#define I_FWK_SCHOOL "iSchool"
// 战士造型缩放比例
#define I_FWK_ZOOM "zoom"
// 战士变色32位色
#define I_FWK_COLOR "color"
// 战士面向
#define I_FWK_DIR "dir"
// 战士的武器换装
#define I_FWK_PARTS_WEAPON "parts_weapon"
// 战士在战场的位置
#define I_FWK_POSITION "position"
// 战士的战斗数值
#define I_FWK_VALUES "values"
// 战士的战斗状态
#define M_FWK_STATUS "status"
// 战士的主人，召唤兽一定有，NPC可能有
#define I_FWK_OWNER "owner"
// 战士的召唤兽,暂时只有玩家有
#define I_FWK_SUMM "summon"
// 战士的法宝，暂时只有玩家有
#define I_FWK_FABAO "fabao"
// 玩家的加入顺序 
#define I_FWK_INDEX "index"
// 所有未参战召唤兽
#define A_FWK_CAN_SUMM "can_summon"
// 所有已经参战召唤兽
#define A_FWK_CAN_NOT_SUMM "can_not_summon"
// 所有未参战的法宝
#define A_FWK_CAN_FABAO "can_fabao"
// 所有未参战的法宝
#define A_FWK_CAN_NOT_FABAO "can_not_fabao"

// 战斗中退出标记
#define A_FWK_QUIT_IN_FIGHT "quit_in_fight"
// 战斗中的panel
#define A_FWK_PANEL "fight_panel"
// 战士额外数据,用来给AI和程序逻辑用
#define M_FWK_EXTEND_DATA "extend_data"
// 战士AI
#define A_FWK_AI "ai"
// 战士AI处理文件
#define C_FWK_AI_FILE "ai_file"
// 战斗前气血，魔法
#define I_FWK_BEFOREFIGHT_HP "before_fight_hp"
#define I_FWK_BEFOREFIGHT_MP "before_fight_mp"


// fight warrior statu key
// 状态ID
#define FWSK_STATUS_ID "status_id" 
// 状态回合数
#define FWSK_STATUS_BOUT "bout" 
// 状态的最大回合数，用于状态刷新
#define FWSK_STATUS_MAX_BOUT "max_bout" 
// 状态施放方
#define FWSK_STATUS_SOURCE "source" 
// 状态释放对象
#define FWSK_STATUS_TARGET "target" 
// 状态参数
#define FWSK_STATUS_ARGS "args" 
// 状态命中率
#define FWSK_STATUS_RATE "hit_rate"
// 状态数据
#define FWSK_STATUS_DATA "data" 
// 定制状态伤害暴击率
#define FWSDK_CUSTOM_CRITICAL_RATE "critical_rate"
// 可叠加状态的叠加层数
#define FWSDK_STACK_CNT "stack_cnt"

#define STATUS_IDS_ADD_IN_ENDDO "add_in_enddo"

// 蓄力状态用
#define XULI_SKILL_ID "skillid"
#define XULI_TAG_SIDE "side"
#define XULI_TAG_POSITION "position"
// 蓄力结算时机
#define XULI_CAL_TIME "xulicaltime"

// 即时状态的回合数
#define IMMEDIATELY_BOUT (0) 
// 永久状态的回合数
#define PERMANENCE_BOUT (999) 

// --------------------------------------------------------------

// 对方
#define FIGHT_SIDE_OTHER(side) (3-side)

// 战斗数值
#define FIGHT_KEY_WIN_SIDE "I_FK_WIN_SIDE"

// 战斗指令宏
// 攻击
#define FIGHT_CMD_ATTACK "attack"
// 逃跑
#define FIGHT_CMD_ESCAPE "escape"
// 叫骂 
#define FIGHT_CMD_CURSE  "curse"
// 捕捉
#define FIGHT_CMD_CATCH  "catch"
// 继续蓄力
#define FIGHT_CMD_XULI "xuli"
// 物品使用
#define FIGHT_CMD_ITEM "item"
// 技能命令
#define FIGHT_CMD_SKILL "skill"
// 防御
#define FIGHT_CMD_DEFENSE "defense"
// 召唤
#define FIGHT_CMD_SUMMON "summon"
// 召还
#define FIGHT_CMD_SUMMON_BACK "summon_back"
// 离开观战
#define FIGHT_CMD_ESC_WITNESS "esc_witness"
// 法宝
#define FIGHT_CMD_FABAO "fabao"

// 攻击技能ID
#define ATTACK_SKILL_ID "1000000"
// 目标所在方宏
// 敌方
#define TARGET_SIDE_SELF (1)
// 己方
#define TARGET_SIDE_FRIEND (2)
// 自己 
#define TARGET_SIDE_ENEMY (4)
// 自己边
#define TARGET_SIDE_OWNER_TEAM (TARGET_SIDE_SELF|TARGET_SIDE_FRIEND)
// 除自己外的所有人
#define TARGET_SIDE_OTHER (TARGET_SIDE_FRIEND | TARGET_SIDE_ENEMY )
// 全员
#define TARGET_SIDE_ALL (TARGET_SIDE_SELF|TARGET_SIDE_FRIEND|TARGET_SIDE_ENEMY)
// 主人
#define TARGET_SIDE_OWNER (8)

// 敏降序
#define SORT_SPE_DESC (0)
// 攻击降序
#define SORT_ATTACK_DESC (1)
// 血降序
#define SORT_HP_DESC (2)
// 血升序
#define SORT_HP_ASC (3)
// 全随机
#define SORT_RANDOM (4)
// 法升序
#define SORT_MP_ASC (5)

// 蓄力回合结算时机
// 后发蓄力
#define XULI_CALCULATE_ENDDO (0)
// 先发发蓄力
#define XULI_CALCULATE_PREDO (1)

// 排序方式
// 升序
#define SORT_BY_ASC (0)
// 降序
#define SORT_BY_DESC (1)

// 风寒 星落 风魂 叠加层数
#define FH_XL_FH2_CNT "fh_xl_fh2_cnt" 

// --------------------------------------------------------------------------------------------
// 战斗菜单栏的构建
#define ATTACK_BUTTON  "attack"
#define CURSE_BUTTON   "10001"
#define ESCAPE_BUTTON  "escape"
#define HOLD_BUTTON    "hold"
#define DEFENSE_BUTTON "defense"
#define SKILL_BUTTON   "skill" 
#define CATCH_BUTTON   "catch"
#define ITEM_BUTTON    "item" 
#define SUMMON_BUTTON    "summon" 
#define SUMMON_BACK_BUTTON    "summon_back" 
#define ESCAPE_WITNESS_BUTTON    "esc_witness" 
#define FABAO_BUTTON    "fabao" 
// ============================================================================================
//                         "自定义操作项,按钮id,按钮显示文本,客户端发上来的命令,命令类型,命令描述参数1,....."
// 攻击
#define ATTACK_BUTTON_TXT  "自定义操作项,"ATTACK_BUTTON",攻击,"FIGHT_CMD_ATTACK",目标,TARGET_SIDE_ENEMY,"
// 诅咒(测试用)
#define CURSE_BUTTON_TXT   "自定义操作项,"CURSE_BUTTON",诅咒,"FIGHT_CMD_CURSE",目标,TARGET_SIDE_ENEMY,"
// 逃跑
#define ESCAPE_BUTTON_TXT  "自定义操作项,"ESCAPE_BUTTON",逃跑,"FIGHT_CMD_ESCAPE",指令,,"
// 蓄力
#define HOLD_BUTTON_TXT    "自定义操作项,"HOLD_BUTTON",蓄力,"FIGHT_CMD_XULI",指令,,",
// 防御
#define DEFENSE_BUTTON_TXT "自定义操作项,"DEFENSE_BUTTON",待机[D],"FIGHT_CMD_DEFENSE",指令,,"
// 技能
#define SKILL_BUTTON_TXT   "自定义操作项,"SKILL_BUTTON",法术[W],"FIGHT_CMD_SKILL",定制,,"
// 战斗物品使用
#define ITEM_BUTTON_TXT    "自定义操作项,"ITEM_BUTTON",物品[E],"FIGHT_CMD_ITEM",定制,,"
// 召唤
#define SUMMON_BUTTON_TXT  "自定义操作项,"SUMMON_BUTTON",召唤,"FIGHT_CMD_SUMMON",定制,,"
// 召还
#define SUMMON_BACK_BUTTON_TXT "自定义操作项,"SUMMON_BACK_BUTTON",召还,"FIGHT_CMD_SUMMON_BACK",指令,,"
// 捕捉
#define CATCH_BUTTON_TXT "自定义操作项,"CATCH_BUTTON",捕捉,"FIGHT_CMD_CATCH",目标,TARGET_SIDE_ENEMY,"
// 离开观战
#define ESC_WIT_BUTTON_TXT "自定义操作项,"ESCAPE_WITNESS_BUTTON",离开,"FIGHT_CMD_ESC_WITNESS",指令,,"
// 法宝
#define FABAO_BUTTON_TXT "自定义操作项,"FABAO_BUTTON ",法宝,"FIGHT_CMD_FABAO",定制,,"

#define DEFAULKT_USER_BTN_LST SKILL_BUTTON","ATTACK_BUTTON","ITEM_BUTTON","SUMMON_BUTTON","FABAO_BUTTON","DEFENSE_BUTTON","ESCAPE_BUTTON 
#define NOFABAO_USER_BTN_LST SKILL_BUTTON","ATTACK_BUTTON","ITEM_BUTTON","SUMMON_BUTTON","DEFENSE_BUTTON","ESCAPE_BUTTON 
//#define DEFAULKT_USER_BTN_LST SKILL_BUTTON","ATTACK_BUTTON","ITEM_BUTTON","SUMMON_BUTTON","DEFENSE_BUTTON","ESCAPE_BUTTON 
#define DEFAULT_SUMM_BTN_LST SKILL_BUTTON","ATTACK_BUTTON","ITEM_BUTTON","DEFENSE_BUTTON","ESCAPE_BUTTON 

#define DEFAULT_WITNESS_BTN_LST ESC_WIT_BUTTON_TXT 

// 战斗中定制按钮 
// 注:可能出现的按钮，必须在战斗初期一次发给客户端
// 这样，保证服务器指令处理时，知道如何处理
// 客户端先期准备好panel要使用的按钮
#define M_FIK_CUSTOM_BUTTON "custom_btn"

// 战斗定制panel
#define A_FIK_CUSTOM_PANEL "custom_pnl"
#define A_FIK_CUSTOM_SUMM_PANEL "custom_summ_pnl"
// 战斗扩展panel
#define A_FIK_EXTEND_PANEL "extend_pnl"
#define A_FIK_EXTEND_SUMM_PANEL "extend_summ_pnl"

// 战士定制panel
#define A_FWK_CUSTOM_PANEL "custom_pnl"
#define A_FWK_CUSTOM_SUMM_PANEL "custom_summ_pnl"
// 战士扩展panel
#define A_FWK_EXTEND_PANEL "extend_pnl"
#define A_FWK_EXTEND_SUMM_PANEL "extend_summ_pnl"
 
// 召唤兽的战斗ID
#define FIGHT_GRID_ID "fight_id"
// 添加过标记
#define SUMMON_ADDED_FLG "added_flg"

// --------------------------------------------------------------------------------------------

// 发送消息给全战斗
#define SENDMSG_TO_FIGHT(fightId, msg) FIGHT_MAIN->SendMsgToFight( fightId, msg ) 
#define SENDMSG_TO_FIGHT_BY_SIDE(fightId, side, msg) FIGHT_MAIN->SendMsgBySideFormat( fightId, side, msg ) 
#define SENDMSG_TO_WARRIOR(wi, msg) FIGHT_MAIN->rpc_client_fight_msg( wi, msg ) 


// --------------------------------------------------------------------------------------------
// extend key
#define K_EXT_DIE_NPCS     "dienpcs"

#define K_BAOBAO_FLAG      "K_BAOBAO_FLAG"
#define K_ANLEI_GRADE      "K_ANLEI_GRADE"
#define K_CATCH_RATE       "K_CATCH_RATE"

// 战斗消耗方式
// 默认
// 装备:每场，每件消耗1~3
// 召唤兽:每场按种类扣除一定值，死亡10
#define CONSUME_METHOD_DEFAULT (0)
// 免费
#define CONSUME_METHOD_NO_FEE (1)
// PK
#define CONSUME_METHOD_PK (2)
// 消耗时机
// 正常离场
#define CONSUME_TIMEING_DEFAULT (0)
// 死亡
#define CONSUME_TIMEING_DIE (1)

// PK
#define T_DIE_PUNISH_PK (2)
// 普通死亡惩罚(经验扣除)
#define T_DIE_PUNISH_NORMAL  (1)
// 不做死亡惩罚
#define T_DIE_PUNISH_NULL    (0)

//#define FIGHT_DEBUG

#ifdef FIGHT_DEBUG 
#define debug_log(msg) log_file("fight", sprintf("[%s][%d][%s] %s\n", __FILE__, __LINE__, short_time(), msg))
#else
#define debug_log(msg) 
#endif // FIGHT_DEBUG

#define FENGSHEN_TAI_POS ({ 1004, 35, 48, })
#define NEWBIE_REBORN_POS ({ 1001, 37, 249, })


#define SUMMON_TATTOO_PATH         "module/summon/tattoo/"
#define SUMMON_TATTOO_MAIN         SUMMON_TATTOO_PATH"main.c"
#define SUMMON_TATTOO_BASE         SUMMON_TATTOO_PATH"base.c"

#define REENTER_NORMAL (0)
#define REENTER_DIE (1)
#define REENTER_ESCAPE (2)

#define DEFAULT_UNSELECT_STATUSES ({ DIE_STATUS, WUDI_STATUS, SW_WUDI_STATUS, SEAL_STATUS, YINSHEN_STATUS, })

// 武器换装顺序
#define PARTS_WEAPON_INDEX (1)

#endif // __FIGHT_H__
