// 用户头文件
// ----------------------------------------


#ifndef __FS_USER_H__
#define __FS_USER_H__

#include "lpc.h"
#include "lpc_incl.h"

#include "stack.h"

#include "fs_prop_key.h"

#define USER_OBJ "char/user"
#define VALIDATE_MOVE_FREQ_MASK 8 //走路检测的频率，每几个走路包就进行检测


// 用户数据
typedef struct fs_user_s {
	// usernum
	int usernum; 
	// 所有用户中的顺序
	int index;
	// 网络fd
	int iFd;

	// -----------------------------------
	// 为加快速度直接在结构体中存放的变量
	// 1、战斗信息
	int iFighting;
	// 2、队伍ID, 如果在队伍中，则记录此ID
	int iTeamID;
	// 3、队伍暂离变量, 之所以加这个变量，因为暂离的玩家不影响自己走路
	int iTempLeave; 
	// 4、摆摊ID
	int iStallID;
	// -----------------------------------

	// 脚本对象
	object_t* user_obj;

	//最后一次收网络数据包的时间戳
	time_t last_pto_timestamp;


	//和客户端同步的某些指定网络协议的发送数量的统计
	int specific_pto_count;
	int specific_pto_error_count;
	int specific_pto_threshold;
	int specific_pto_step;
	int specific_pto_mod;
	int specific_pto_start;

	int name_hash;
	int title_hash;
	int name_title_hash;

	int parts_weapon;
} fs_user_t;

void init_user_mng(unsigned int online, unsigned int login);
object_t* GetUserLpcObj( int usernum );
fs_user_t* FindUserOnline( int usernum );
svalue_t * get_user_prop(fs_user_t *user, svalue_t *key);
svalue_t * get_user_temp_prop(fs_user_t *user, svalue_t *key);
int get_user_teamid(int usernum);
void fs_user_object_update(object_t *obj);
int *get_all_users(int *user_num);
int fs_gamed_close_uid_connection(int usernum);
//void fs_dat_object_update(object_t *obj);
void init_specific_pto_stats(fs_user_t *user);
void set_specific_pto_stats(fs_user_t *user, int threshold, int step, int mod);
void specific_pto_stats_start(fs_user_t *user);

#endif // __FS_USER_H__
