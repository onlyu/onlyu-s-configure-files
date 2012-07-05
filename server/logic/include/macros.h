// 一些常用的宏
#ifndef    _MACROS_H_
#define    _MACROS_H_
//#include <item_key.h>

#define K_GLOBAL_KEY             "globalKey"

#define NPC_BASE                "char/npc/npc_base"

#define MAX_CASH 2100000000
#define YES			1
#define NO			0

// 定义系统NPC的数量
#define  SYS_NPC_NUM             10000

/* rpc */
// 引擎也对应一份定义，需要同步修改
#define T_RPC_X_USR_FIELD               (1)
#define T_RPC_X_USR_SCENE               (2)
#define T_RPC_X_USR_FIELD_NOT_FIGHT     (3)
#define T_RPC_X_USR_SCENE_NOT_FIGHT     (4)
#define T_RPC_X_SCENE                   (5)
#define T_RPC_X_ALL_USERS               (6)
#define T_RPC_X_SCENE_NOT_FIGHT         (7)

//gamed 表示广播的uid                                                                                                                                                                                
#define FS_BROADCAST_UID         (-1)                                                                                                                                                                     
//gamed 给netd的uid                                                                                                                                                                                       
#define FS_GAMED_UID             (-2)                                                                                                                                                                     
#define FS_NETD_UID              (-3)                                                                                                                                                                     
#define FS_DBD_UID               (-4) 

// 性别
#define T_GENDER_MALE       1
#define T_GENDER_FEMALE     2

// 心跳300秒一次
#define HEARTBEAT_TICKET 300

#define PERM_VAR 1 
#define TEMP_VAR 0 

//聊天频道
//
#define CHANNEL_TEAM 0  //组队频道
#define CHANNEL_PRIVATE 1  //私聊频道
#define CHANNEL_WORLD 2  //世界频道
#define CHANNEL_CURRENT 3  //当前频道
#define CHANNEL_SYSTEM 4  //系统频道
#define CHANNEL_TIPS   5  //提示频道
#define CHANNEL_GROUP 6 //团队频道
#define CHANNEL_ORG 7   //帮派频道
#define CHANNEL_BROADCAST 9 // 传音
#define CHANNEL_KEFU 10 //和客服在线聊天的频道
#define CHANNEL_FRIEND	11 //好友消息

//登录时的验证类型
#define VERIFY_175GAME 1
#define VERIFY_WEIBO 2

#define WEIBO_POSTFIX "~wb"


// ====================================================
// 全局的物品宏加上G_

#define G_ITEM_ICON    "icon"
#define G_ITEM_DESC    "desc"
#define G_ITEM_NAME    "name"

#endif

