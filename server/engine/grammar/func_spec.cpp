# 1 "grammar/func_spec.c"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "grammar/func_spec.c"
# 1 "grammar/spec.h" 1

# 1 "grammar/std.h" 1




# 1 "grammar/../include/arch.h" 1
# 6 "grammar/std.h" 2






# 1 "grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "grammar/options_incl.h" 2
# 13 "grammar/std.h" 2
# 1 "grammar/configure.h" 1
# 14 "grammar/std.h" 2







# 1 "grammar/../include/portability.h" 1
# 22 "grammar/std.h" 2
# 1 "grammar/../include/macros.h" 1
# 90 "grammar/../include/macros.h"
# 1 "grammar/../include/malloc.h" 1
# 91 "grammar/../include/macros.h" 2
# 23 "grammar/std.h" 2
# 2 "grammar/spec.h" 2
# 2 "grammar/func_spec.c" 2
# 1 "grammar/op_spec.c" 1





operator pop_value, push, efun0, efun1, efun2, efun3, efunv;

operator number, real, byte, nbyte, string, short_string, const0, const1;

operator aggregate, aggregate_assoc;







operator branch_when_zero, branch_when_non_zero, branch;
operator bbranch_when_zero, bbranch_when_non_zero, bbranch;

operator branch_ne, branch_ge, branch_le, branch_eq, bbranch_lt;

operator foreach, next_foreach, exit_foreach;
operator loop_cond_local, loop_cond_number;
operator loop_incr;
operator while_dec;

operator lor, land;

operator catch, end_catch;
operator time_expression, end_time_expression;

operator switch;

operator call_function_by_address, call_inherited, return, return_zero;


operator eq, ne, le, lt, ge, gt;

operator inc, dec, pre_inc, post_inc, pre_dec, post_dec;

operator transfer_local;


operator local, local_lvalue;
operator global, global_lvalue;
operator member, member_lvalue;
operator index, index_lvalue;
operator rindex, rindex_lvalue;
operator nn_range, nn_range_lvalue, rn_range, rn_range_lvalue;
operator rr_range, rr_range_lvalue, nr_range, nr_range_lvalue;
operator ne_range, re_range;


operator add_eq, sub_eq, and_eq, or_eq, xor_eq, lsh_eq, rsh_eq, mult_eq;
operator div_eq, mod_eq, assign;

operator void_add_eq, void_assign, void_assign_local;

operator add, subtract, multiply, divide, mod, and, or, xor, lsh, rsh;
operator not, negate, compl;

operator function_constructor;
operator simul_efun;

operator sscanf;
operator parse_command;

operator new_class, new_empty_class;
operator expand_varargs;
# 3 "grammar/func_spec.c" 2
# 23 "grammar/func_spec.c"
mixed call_other(object | string | object *, string | mixed *,...);
mixed evaluate(mixed, ...);

object this_object();

int to_int(string | float | int | buffer);
float to_float(string | float | int);

object clone_object(string, ...);
mixed previous_object(int default: 0);
object *all_previous_objects previous_object(int default: -1);
mixed *call_stack(int default: 0);
int sizeof(mixed);
int strlen sizeof(string);
void destruct(object);
string file_name(object default: F_THIS_OBJECT);

string *explode(string, string);
mixed implode(mixed *, string | function, void | mixed);

int member_array(mixed, string | mixed *, void | int);
int random(int);

string lower_case(string);
string replace_string(string, string, string,...);
string replace_string_reg(string, string, string);
int restore_object(string, void | int);
int save_object(string, void | int);



mixed *get_dir(string, int default: 0);
int strsrch(string, string | int, int default: 0);
int substr(string, string | int, int default: 0);

object find_object(string, int default: 0);
object load_object find_object(string, int default: 1);


mapping allocate_mapping(int);
mixed *values(mapping);
mixed *keys(mapping);






void map_delete(mapping, mixed);


int clonep(mixed default: F_THIS_OBJECT);
int intp(mixed);
int undefinedp(mixed);
int nullp undefinedp(mixed);
int floatp(mixed);
int stringp(mixed);

int functionp(mixed);
int pointerp(mixed);
int arrayp pointerp(mixed);
int objectp(mixed);
int classp(mixed);
string typeof(mixed);


int bufferp(mixed);


buffer allocate_buffer(int, int default:0);
mixed read_buffer(string | buffer, void | int, void | int);
int write_buffer(string | buffer, int, string | buffer | int);


int inherits(string, object);
void replace_program(string);
mixed regexp(string | string *, string, void | int);

mixed *allocate(int);

int write_file(string, string, void | int);
int rename(string, string);
int write_bytes(string, int, string | buffer );

int file_size(string);
string read_bytes(string, void | int, void | int);
string read_file(string, void | int, void | int);
int cp(string, string);

int mkdir(string);
int rm(string);
int rmdir(string);


string clear_bit(string, int);
int test_bit(string, int);
string set_bit(string, int);
int next_bit(string, int);

string crypt(string, string | int);
string md5_hexdigest(string);


string ctime(int);
mixed *localtime(int);
string function_exists(string, void | object, void | int);

object *objects(void | string | function, void | object);



mixed *sort_array(mixed *, int | string | function, ...);
void throw(mixed);
int time();
mapping unique_mapping(mixed *, string | function, ...);
string *deep_inherit_list(object default:F_THIS_OBJECT);
string *shallow_inherit_list(object default:F_THIS_OBJECT);



string *inherit_list shallow_inherit_list(object default:F_THIS_OBJECT);


string sprintf(string,...);
int mapp(mixed);
mixed *stat(string, int default: 0);

int memory_info(object | void);


object *children(string);

void error(string);
int uptime();
int strcmp(string, string);
# 167 "grammar/func_spec.c"
mixed filter(mixed * | mapping, string | function, ...);
mixed filter_array filter(mixed *, string | function, ...);
mapping filter_mapping filter(mapping, string | function, ...);

mixed map(string | mapping | mixed *, string | function, ...);
mapping map_mapping map(mapping, string | function, ...);

mixed *map_array map(mixed *, string | function, ...);

string malloc_status();
void dumpallobj(string | void);

string dump_file_descriptors();
string query_load_average();





int reclaim_objects();

int set_eval_limit(int);
int reset_eval_cost set_eval_limit(int default: 0);
int eval_cost set_eval_limit(int default: -1);
int max_eval_cost set_eval_limit(int default: 1);
# 211 "grammar/func_spec.c"
void shutdown(void | int);

mixed* keys_by_value(mapping);

void os_command(string, string, string);

string get_config_file();
int is_auth_server();


void lpc_gc(int);

string fs_memory_info();
# 1 "packages/fs_mysql_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/fs_mysql_spec.h"
# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 2 "packages/fs_mysql_spec.h" 2

int mysql_connect(mixed *);
int mysql_close(int);

mixed mysql_execute(int, string);
int cursor_close(int);
buffer *cursor_fetch(int);
# 1 "packages/fs_dbd_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/fs_dbd_spec.h"

# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 3 "packages/fs_dbd_spec.h" 2






string lpc2pystr(mixed);
mixed pystr2lpc(string);
mixed fs_restore_from_file(string);
int fs_save_to_file(string, mapping);
# 1 "packages/fs_http_lpc_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/fs_http_lpc_spec.h"
# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 2 "packages/fs_http_lpc_spec.h" 2


int fs_http_request(mixed *, mixed *, mixed *);
string fs_url_encode_map(mapping);
string fs_url_encode(string);
string fs_url_decode(string);
int fs_curl_request(mapping, function, mixed);
# 1 "packages/fs_netd_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/fs_netd_spec.h"

# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 3 "packages/fs_netd_spec.h" 2

void fs_new_user(int, int);
int fs_is_uid_connected(int);
int fs_close_uid_connection(int);
int fs_close_vfd_connection(int);
int fs_get_vfd_by_uid(int);
int fs_get_uid_by_vfd(int);
string fs_get_ip_by_vfd(int);
# 1 "packages/fs_rpc_lpc_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/fs_rpc_lpc_spec.h"

# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 3 "packages/fs_rpc_lpc_spec.h" 2


void fs_rpc_call(int|int*, int, ...);
void fs_rpc_call_loop(int, int, ...);
int fs_last_pto_timestamp(int);
void fs_rpc_make_cfg(mapping *, string, string);
void fs_exclude_stats_pto(int *);

void fs_set_validation(int, int, int, int);
void fs_validation_start(int);
# 1 "packages/fs_main_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/fs_main_spec.h"


# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 4 "packages/fs_main_spec.h" 2

void main_loop(void);

int call_out(string | function, int,...);
int remove_call_out( int );


int fs_enable_dirty_object(object);
int fs_enable_dirty_map(mapping);
buffer fs_marshal_dirty_map(mapping);
void fs_merge_dirty_map(mapping, buffer);
buffer fs_marshal_map(mapping);

buffer fs_marshal_dirty_object(object);
void fs_merge_dirty_object(mapping, buffer);


int user_create(int);
int user_destroy(int);
int gamed_close_uid_connection(int);

int fs_save_all_user(int);
int fs_save_all_dat(mapping, int);
int fs_db_sync(int);
int fs_db_gamed_exit();

int fs_db_object_new(int|string, object);
int fs_db_object_load(int|string, int, int);
int fs_db_object_unload(int|string);
int fs_db_object_save(int|string, object, int);
int fs_db_object_rename(int|string, object, string);
int fs_db_object_restore(object, buffer);
void fs_dirty_mem_pool_stat();




mixed get_user( int );
int is_online( int );
int logout( int );
void update_max_login(int);

mixed get_all_users(int default:0);
int get_online_count();



int new_item(string);
int is_item_online(int);
int restore_item(string, mapping);
void destroy_item(int);
mapping get_item_map(int, int);
void SetItemTemp(int, string|int, mixed);
mixed GetItemTemp(int, string|int);
void SetItemSave(int, string|int, mixed);
mixed GetItemSave(int, string|int);


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


int new_summon(string);
mapping get_summon_map(int, int);
void destroy_summon(int);
int restore_summon(string, mapping);
int is_summon_online(int);
void SetSummonTemp(int, string|int, mixed);
mixed GetSummonTemp(int, string);
void SetSummonSave(int, string|int, mixed);
mixed GetSummonSave(int, string);


void update_lpc_object(object);
void dump_memory_pool();
int *get_time_of_day();
void enable_log_pto(int);

string fs_iconv(string, string, string);

int fs_dns_resolve(string, function, mixed);


string _(string);
mapping init_gettext(string, string);
# 1 "packages/contrib_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/contrib_spec.h"
# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 2 "packages/contrib_spec.h" 2

mixed copy(mixed);
string *functions(object, int default: 0);
string *variables(object, int default: 0);
int file_length(string);
string upper_case(string);
int replaceable(object, void | string*);
string program_info(void | object);
void store_variable(string, mixed);
mixed fetch_variable(string);
void debug_message(string, ...);
object function_owner(function);
string repeat_string(string, int);
# 1 "packages/sockets_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/sockets_spec.h"
# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 2 "packages/sockets_spec.h" 2



int socket_create(string | function );



int socket_bind_and_listen(int, int, string | function );


int socket_accept(int, string | function, string | function);


int socket_connect(int, string, string | function, string | function);

int socket_write(int, mixed);

int socket_close(int);


string socket_address(int );

string dump_socket_status();
# 1 "packages/develop_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/develop_spec.h"
# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 2 "packages/develop_spec.h" 2

mixed debug_info(int, object);
int refs(mixed);




void dump_prog(object, ...);
# 1 "packages/math_spec.h"
# 1 "/home/chenyh/work/trunk/engine//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "packages/math_spec.h"
# 1 "packages/../grammar/spec.h" 1

# 1 "packages/../grammar/std.h" 1




# 1 "packages/../grammar/../include/arch.h" 1
# 6 "packages/../grammar/std.h" 2






# 1 "packages/../grammar/options_incl.h" 1
# 1 "./packages/local_options.h" 1





# 1 "./grammar/options.h" 1
# 7 "./packages/local_options.h" 2
# 1 "./include/cc.h" 1
# 8 "./packages/local_options.h" 2
# 1 "packages/../grammar/options_incl.h" 2
# 13 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/configure.h" 1
# 14 "packages/../grammar/std.h" 2







# 1 "packages/../grammar/../include/portability.h" 1
# 22 "packages/../grammar/std.h" 2
# 1 "packages/../grammar/../include/macros.h" 1
# 90 "packages/../grammar/../include/macros.h"
# 1 "packages/../grammar/../include/malloc.h" 1
# 91 "packages/../grammar/../include/macros.h" 2
# 23 "packages/../grammar/std.h" 2
# 2 "packages/../grammar/spec.h" 2
# 2 "packages/math_spec.h" 2

float cos(float);
float sin(float);
float tan(float);
float asin(float);
float acos(float);
float atan(float);
float sqrt(float);
float log(float);
float pow(float, float);
float exp(float);
float floor(float);
float ceil(float);

mixed max(mixed *, int | void);
mixed min(mixed *, int | void);
