// 测试文件

#include "../grammar/spec.h"

void main_loop(void);

int call_out(string | function, int,...);
int remove_call_out( int );

// 脏数据管理用efun
int fs_enable_dirty_object(object);
int fs_enable_dirty_map(mapping);
buffer fs_marshal_dirty_map(mapping);
void fs_merge_dirty_map(mapping, buffer);
buffer fs_marshal_map(mapping);

buffer fs_marshal_dirty_object(object);
void fs_merge_dirty_object(mapping, buffer);
//dat module to restore the data from dbd
// 用户管理有关
int user_create(int);
int user_destroy(int);
int gamed_close_uid_connection(int);

int fs_save_all_user(int);
int fs_save_all_dat(mapping, int);
int fs_db_sync(int);
int fs_db_gamed_exit();
//////////////////////////
int fs_db_object_new(int|string, object);
int fs_db_object_load(int|string, int, int);
int fs_db_object_unload(int|string);
int fs_db_object_save(int|string, object, int);
int fs_db_object_rename(int|string, object, string);
int fs_db_object_restore(object, buffer);
void fs_dirty_mem_pool_stat();
//////////////////////////



mixed get_user( int ); // 测试用函数
int is_online( int );   // 测试用函数
int logout( int );      // 测试用函数
void update_max_login(int);
// 
mixed get_all_users(int default:0);
int get_online_count();


/*Item*/
int new_item(string);
int is_item_online(int);
int restore_item(string, mapping);
void destroy_item(int);
mapping get_item_map(int, int);
void SetItemTemp(int, string|int, mixed);
mixed GetItemTemp(int, string|int);
void SetItemSave(int, string|int, mixed);
mixed GetItemSave(int, string|int);

/*npc*/
int new_npc(string);
int restore_npc(string, mapping);
void destroy_npc(int);
void SetNpc(int, string|int, mixed);
mixed GetNpc(int, string|int);
void SetNpcSave(int, string|int, mixed);
mixed GetNpcSave(int, string|int);
int is_npc_online(int);
object get_npc_object(int);
mapping get_npc_map(int, int);
void set_npc_map(int, mapping, int);

/*summon*/
int new_summon(string);
mapping get_summon_map(int, int);
void destroy_summon(int);
int restore_summon(string, mapping);
int is_summon_online(int);
void SetSummonTemp(int, string|int, mixed);
mixed GetSummonTemp(int, string);
void SetSummonSave(int, string|int, mixed);
mixed GetSummonSave(int, string);

// 其他
void update_lpc_object(object);
void dump_memory_pool();
int *get_time_of_day();
void enable_log_pto(int);
//string content, string from_encoding string to_encoding
string fs_iconv(string, string, string);
//string host, function cb, mixed udata
int fs_dns_resolve(string, function, mixed);

//gettext
string _(string);
mapping init_gettext(string, string);

