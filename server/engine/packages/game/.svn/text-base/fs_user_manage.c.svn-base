// 用户管理模块

#include "fs_user.h"
#include "lpc_args_op.h"
#include "fs_marshal.h"
#include "fs_comm.h"
#include "fs_data_save.h"
#include "fs_int_array.h"
#include "fs_db.h"
#include "fs_hlist.h"
#include "fs_mem_pool.h"
#include "fs_common.h"

#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

extern int is_gamed_start;

// 最大在线人数
static unsigned int max_online = 0;
// 允许登录人数
static unsigned int max_login = 0;

static int global_uidp[MAX_ONLINE * 2];
static int global_uid_num = 0;

// 管理用户对象的数据结构
// 所有的玩家 
static fs_user_t* all_users = NULL;
// 空闲的user栈
ST_I *null_users_stack = NULL; 
// 在线玩家
// fs_user_t* online_users = NULL;

fs_obj_hash_t *online_user_tbl;					/*所有玩家*/
// fs_memory_pool_t *gpUserPool;				/*玩家内存池*/

// ===============================================================
// 
// ===============================================================
void update_max_login( unsigned int login )
{
	int i;
	if ( login < max_login )
	{
		debug_message( "ERROR:new max_login is small than old [%d].to set [%d]", max_login, login );
		return;
	}

	for( i = max_login; i < login; i ++)
	{
		// 压入空vfd栈
		if(null_users_stack->push(null_users_stack, i) != 1)
		{
			debug_message("vfd stack full, can not push %d\n", i);
			exit(-12);
		}
	}
	
	max_login = login;
}

// 初始化用户管理系统
void init_user_mng(unsigned int online, unsigned int login)
{
	int size;

	max_online = online;

	assert(login <= max_online);

	if ( all_users != NULL)
	{
		// 只能初始化一次
		debug_message( "WARNING: user manage system is initialized.\n" );
		return;
	
	}

	if (init_stack_int(&null_users_stack) != 1)
	{
		debug_message("user stack init error\n");
		exit(-11);
	}

	//设置存盘变量的信息
	object_t *user_obj = clone_object(USER_OBJ, 0); 	
	if (init_user_save_var(user_obj)) {
		fprintf(stderr, "setup user save vars error %s\n", USER_OBJ);
		exit(1);
	}
	//todo ? !
	//free_object(user_obj, "init user manager");

	// 初始化all_users;
	size = sizeof(fs_user_t) * max_online;
	all_users = (fs_user_t *)MALLOC(size);
	bzero(all_users, size);

	online_user_tbl = fs_ht_create_hash_table(online);

	// 设置最大可登录人数
	update_max_login( login );

}

static int destroy_user(int usernum)
{
	fs_user_t* user = FS_HT_FIND_INT(online_user_tbl, usernum);
	if (user != NULL) {

		// 从在线列表中删除
		FS_HT_REMOVE_INT(online_user_tbl, usernum);

		// destruct object 
		destruct_object(user->user_obj);

		// 存盘完毕释放user_obj
		//free_object(user->user_obj, "RemoveUser");
		// 将index放回
		null_users_stack->push(null_users_stack, user->index);	
		return 0;
	}
	return -1;
}

// 删除目标用户
// 返回值
// 0 正常删除
// -1 目标已经被删除
// -2 目标存盘失败
int RemoveUser( int usernum )
{
	fs_user_t* user;

	user = FS_HT_FIND_INT(online_user_tbl, usernum);

	// 玩家不在线
	if ( user == NULL )
	{
		printf("fail to remove user:%d\n", usernum);
		return -1;
	}

	// 玩家处于战斗中
	if ( user->iFighting )
	{
		svalue_t *ret;

		ret = call_script_func("module/fight/main", "QuitInFight", "ii", user->iFighting, usernum ); 

		if (ret != NULL && ret->type == T_NUMBER && ret->u.number == 1) 
		{
			return -2;
		}
	}

	// 调用脚本的登出检查
	call_object_func(user->user_obj, "logout", ""); 
	
	debug_message( "remove user: %d\n", usernum );

	return destroy_user(usernum);
}

// 查询玩家在线 
fs_user_t* FindUserOnline( int usernum )
{
	return FS_HT_FIND_INT(online_user_tbl, usernum);
}

// 取玩家脚本对象 
object_t* GetUserLpcObj( int usernum )
{
	fs_user_t* user = FindUserOnline( usernum );

	if ( user == NULL)
	{
		return NULL;
	}

	return user->user_obj;
}

// 玩家是否在线
int IsOnline( int usernum )
{
	fs_user_t* user = FindUserOnline( usernum );

        if ( user )
        {
                return 1;
        }
        
        return 0;
}


// ======================
// 取属性值
svalue_t * get_user_prop(fs_user_t *user, svalue_t *key)
{
	svalue_t *val;
	svalue_t *prop = find_object_variable(user->user_obj, "prop");

	if (!prop || prop->type != T_MAPPING) {
		// TODO:
		// error
		return &const0u;
	}

	val = find_in_mapping(prop->u.map, key);
	return val;
}

svalue_t * get_user_temp_prop(fs_user_t *user, svalue_t *key)
{
	svalue_t *val;
	svalue_t *prop = find_object_variable(user->user_obj, "temp_var");

	if (!prop || prop->type != T_MAPPING) {
		// TODO:
		// error
		return &const0u;
	}

	val = find_in_mapping(prop->u.map, key);
	return val;
}

// ========================================================================
// 和netd的联系
// ========================================================================

// fd断线 
void netd_remove(fs_uid_t uid)
{
	// 找到iFd对应的usernum,调用RemoveUser 
	RemoveUser(uid);
}

// 玩家主动断线
void to_netd_quit( int usernum )
{
}


// ========================================================================
// efun接口
// ========================================================================

// get_user
void f_get_user( )
{
	object_t* ret;

	//
	int usernum;

	// 设置所有参数
	SET_ALL_ARGS;
	usernum = ARGS(0)->u.number; 
	POP_ALL_ARGS;

	ret = GetUserLpcObj( usernum ); 	

	if ( ret )
	{
		push_object( ret );
		return;
	}

	push_number(0);
}


//todo：have to close the netd connection
void f_logout()
{
	int ret;
	//
	int usernum = sp->u.number; 

	ret = RemoveUser( usernum ); 	

	put_number( ret );
}

// is_online 
void f_is_online()
{
	int ret;
	int usernum = sp->u.number;

	ret = IsOnline(usernum);

	put_number( ret );
}

void init_specific_pto_stats(fs_user_t *user)
{
	user->specific_pto_start = 0;
	user->specific_pto_count = 0;
	user->specific_pto_error_count = 0;
	user->specific_pto_threshold = 0;
	user->specific_pto_step = 0;
	user->specific_pto_mod = 1;
}

void set_specific_pto_stats(fs_user_t *user, int threshold, int step, int mod)
{
	if (mod == 0) {
		mod = 1;
	}
	user->specific_pto_start = 0;
	user->specific_pto_count = 0;
	user->specific_pto_error_count = 0;
	user->specific_pto_threshold = threshold;
	user->specific_pto_step = step;
	user->specific_pto_mod = mod;
}

void specific_pto_stats_start(fs_user_t *user)
{
	user->specific_pto_start = 1;
}

fs_user_t *new_user(fs_uid_t uid)
{
	int index;
	fs_user_t *user;

	// 
	if (null_users_stack->pop(null_users_stack, &index) == 0) {
		return NULL;
	}

	user = &all_users[index];

	memset(user, 0x00, sizeof(fs_user_t));

	user->index = index;
	user->usernum = uid;
	user->iFd = 0; //todo : fix te vfd

	init_specific_pto_stats(user);

	// 将数据塞到在线查询结构中去
	FS_HT_INSERT_INT(online_user_tbl, uid, user);

	user->user_obj = clone_object(USER_OBJ, 0); 	


	return user;
}

fs_uid_t *get_all_users(int *user_num)
{
	fs_user_t *user;
	fs_ht_node_t *hnode;

	global_uid_num = 0;

	FOR_EACH_HTABLE_OB(online_user_tbl, hnode)
	{
		user = (fs_user_t *) hnode->ob;
		global_uidp[global_uid_num++] = user->usernum;
	}
	*user_num = global_uid_num;
	return global_uidp;
}

void online_user_traverse(object_t *ob, char* funcname)
{
	fs_ht_node_t *hnode;
	fs_user_t *user;
	int index = 0;

	FOR_EACH_HTABLE_OB(online_user_tbl, hnode)
	{
		user = (fs_user_t *) hnode->ob;

		call_object_func(ob, funcname, "o", user->user_obj);	

		index++;
	}
}


//新建一个新建uid的玩家
static int user_create(fs_uid_t uid)
{
	fs_user_t *user;

	if (IsOnline(uid)) {
		return -1;
	}

	user = new_user(uid);
	if (user == NULL) {
		//todo log
		return -2;
	}

	/*
	call_object_func(user->user_obj, "NewID", "i", uid);

	*/
	return 0;
}

void f_user_create()
{
	fs_uid_t uid = sp->u.number;
	int ret = user_create(uid);
	put_number(ret);
}

void f_user_destroy()
{
	fs_uid_t uid = sp->u.number;
	int ret = destroy_user(uid);
	put_number(ret);
}

void f_gamed_close_uid_connection()
{
	fs_uid_t uid = sp->u.number;
	int ret = fs_gamed_close_uid_connection(uid);
	put_number(ret);
}


// 更新USER_OBJ
void fs_user_object_update( object_t *ob )
{
	int userObjLen = strlen( USER_OBJ );

	if (strlen(ob->name) == userObjLen && strncmp( ob->name, USER_OBJ, strlen(USER_OBJ) ) == 0)
	{
		fs_user_t *user;
		program_t * tmpProp;
		fs_ht_node_t *hnode;

		/*
		fs_obj_save_vars_t *save_vars = fs_obj_save_vars_renew(user_save_vars, ob, user_var_list);
		if (save_vars == NULL) {
			fprintf(stderr, "update user object save vars error!\n");
			return;
		}
		user_save_vars = save_vars;
		*/

		FOR_EACH_HTABLE_OB(online_user_tbl, hnode)
		{
			user = (fs_user_t *) hnode->ob;

			tmpProp = user->user_obj->prog;	
			user->user_obj->prog = ob->prog;
			// 引用计数处理
			reference_prog(user->user_obj->prog, "fs_user_object_update");
			free_prog(tmpProp, 1);
		}
		fprintf(stderr, "update user object ok!\n");
	}
}

int add_timer(struct timeval period, void (*callback)(void *), void * arg);

void fs_user_manage_init()
{
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	//add_timer(tv, user_db_callout, NULL);
}

void f_get_all_users(void )
{
	SET_ALL_ARGS
	int size = FS_HT_COUNT(online_user_tbl);
	int index = 0;
	array_t * v;
	fs_user_t *user;
	fs_ht_node_t *hnode;
	int is_uid = ARGS(0)->u.number;


	POP_ALL_ARGS	

	if (!size)
	{
		push_number( 0 );
		return ;
	}

	v = allocate_empty_array(size);

	FOR_EACH_HTABLE_OB(online_user_tbl, hnode)
	{
		user = (fs_user_t *) hnode->ob;
		if (is_uid) {
			v->item[index].type = T_NUMBER;
			v->item[index].u.number = user->usernum;
		}
		else {
			v->item[index].type = T_OBJECT;
			v->item[index].u.ob = user->user_obj;
			add_ref(user->user_obj, "f_get_all_users");
		}

		index ++;
	}


	push_refed_array( v );
}

void f_get_online_count(void )
{
	SET_ALL_ARGS
	int size = FS_HT_COUNT(online_user_tbl);
	POP_ALL_ARGS	

	push_number( size );
}

void f_update_max_login(void )
{
	SET_ALL_ARGS
	int maxLogin = ARGS(0)->u.number;

	POP_ALL_ARGS

	update_max_login(maxLogin);
}

void f_online_user_traverse()
{
	SET_ALL_ARGS
	object_t *ob = ARGS(0)->u.ob;
	char* funcname = ARGS(1)->u.string;
	POP_ALL_ARGS

	if (ob->flags & O_DESTRUCTED)
	{
		return ;
	}

	online_user_traverse( ob, funcname );
}
