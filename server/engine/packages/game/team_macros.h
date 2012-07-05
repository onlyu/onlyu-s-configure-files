#ifndef _TEAM_MACROS_H_
#define _TEAM_MACROS_H_

// 注意: 本文件脚本，引擎各存一份!

#define TEAM_NUM_MAX          5                 /*队伍人数上限*/
#define TEAM_MEMBERS          ({0,0,0,0,0,})     /*初始化队员, 与人上限相关*/
#define TEAM_DEFAULT_NUM      5                  /*队伍默认人数设置上限*/

#define TEAM_REQ_MAX          20                 /*队伍请求人数上限*/
#define TEAM_INVITE_MAX       10                 /*队伍邀请人数上限*/         

// 队伍状态, (因为位置为>0的数, 所以状态的定义只能小于0)
#define ST_LEAVE             (-1)                /*暂离*/
#define ST_IN                (0)                 /*正常*/

// 队员的属性
#define K_POS                  "pos"             /*位置*/
#define K_STATE                "state"           /*状态*/

// 人物身上变量信息
#define I_TEAMNO              "iTeamNO"          /*队伍编号*/

// 队伍信息
#define I_LEADER              "iLeader"          /*队长*/
#define LS_MEMBERS            "lsMembers"        /*成员, ([ member: pos ]), ([ user: 1, user2: -1, ]) 1表示位置，-1表示暂离 */
#define LS_P2M                "lsP2M"            /*位置反查表 Pos2Member */
#define I_UPLIMIT             "iUpLimit"         /*人数限制*/
#define I_GRADE_MIN           "iGradeMin"        /*允许加入的队员等级下限*/
#define I_GRADE_MAX           "iGradeMax"        /*允许加入的队员等级上限*/
#define I_LINEUP              "iLineup"          /*队形*/
#define LS_REQ_PLAYERS        "lsReqPlayers"

#endif
