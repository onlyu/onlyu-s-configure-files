#include "spec.h"
#include "op_spec.c"
/*
 * This file specifies types and arguments for efuns.
 * An argument can have two different types with the syntax 'type1 | type2'.
 * An argument is marked as optional if it also takes the type 'void'.
 *
 * Comment out the efuns that you do not want.  Be careful not to comment
 * out something that you need.
 *
 * The order in which the efuns are listed here is the order in which opcode
 * #'s will be assigned.  It is in your interest to move the least frequently
 * used efuns to the bottom of this file (and the most frequently used
 * ones to the top).  The opcprof() efun could help you find out which
 * efuns are most often and least often used.  The reason for ordering
 * the efuns is that only the first 255 efuns are represented using
 * a single byte.  Any additional efuns require two bytes.
 */

/* most frequently used functions */

/* These next few efuns are used internally; do not remove them */
mixed call_other(object | string | object *, string | mixed *,...);
mixed evaluate(mixed, ...);
/* default argument for some efuns */
object this_object();
/* used for implicit float/int conversions */
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
//string capitalize(string);
string *explode(string, string);
mixed implode(mixed *, string | function, void | mixed);

int member_array(mixed, string | mixed *, void | int);
int random(int);

string lower_case(string);
string replace_string(string, string, string,...);
string replace_string_reg(string, string, string);
int restore_object(string, void | int);
int save_object(string, void | int);
//由zhj实现
//string save_variable(mixed);
//mixed restore_variable(string);
mixed *get_dir(string, int default: 0);
int strsrch(string, string | int, int default: 0);
int substr(string, string | int, int default: 0);

object find_object(string, int default: 0);
object load_object find_object(string, int default: 1);

/* mapping functions */
mapping allocate_mapping(int);
mixed *values(mapping);
mixed *keys(mapping);
#ifdef COMPAT_32
mapping map_delete(mapping, mixed);
mapping m_delete map_delete(mapping, mixed);
mixed *m_values values(mapping);
mixed *m_indices keys(mapping);
#else
void map_delete(mapping, mixed);
#endif

int clonep(mixed default: F_THIS_OBJECT);
int intp(mixed);
int undefinedp(mixed);
int nullp undefinedp(mixed);
int floatp(mixed);
int stringp(mixed);
//int virtualp(object);
int functionp(mixed);
int pointerp(mixed);
int arrayp pointerp(mixed);
int objectp(mixed);
int classp(mixed);
string typeof(mixed);

#ifndef DISALLOW_BUFFER_TYPE
int bufferp(mixed);

/* allocate_buffer()多用一个参数 */
buffer allocate_buffer(int, int default:0);
mixed read_buffer(string | buffer, void | int, void | int);
int write_buffer(string | buffer, int, string | buffer | int);
#endif

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

/* the bit string functions */
string clear_bit(string, int);
int test_bit(string, int);
string set_bit(string, int);
int next_bit(string, int);

string crypt(string, string | int);
string md5_hexdigest(string);

// string oldcrypt(string, string | int);
string ctime(int);
mixed *localtime(int);
string function_exists(string, void | object, void | int);

object *objects(void | string | function, void | object);
//void set_heart_beat(int);
//int get_heart_beat(object default:F_THIS_OBJECT);

mixed *sort_array(mixed *, int | string | function, ...);
void throw(mixed);
int time();
mapping unique_mapping(mixed *, string | function, ...);
string *deep_inherit_list(object default:F_THIS_OBJECT);
string *shallow_inherit_list(object default:F_THIS_OBJECT);
#ifdef COMPAT_32
string *inherit_list deep_inherit_list(object default:F_THIS_OBJECT);
#else
string *inherit_list shallow_inherit_list(object default:F_THIS_OBJECT);
#endif
//void printf(string,...);
string sprintf(string,...);
int mapp(mixed);
mixed *stat(string, int default: 0);

int memory_info(object | void);

//   int get_char(string | function,...);
object *children(string);

void error(string);
int uptime();
int strcmp(string, string);

#if (defined(RUSAGE) || defined(GET_PROCESS_STATS) || defined(TIMES)) || defined(LATTICE)
mapping rusage();
#endif				/* RUSAGE */

#ifdef CACHE_STATS
string cache_stats();
#endif

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

//string origin();

/* the infrequently used functions */

int reclaim_objects();

int set_eval_limit(int);
int reset_eval_cost set_eval_limit(int default: 0);
int eval_cost set_eval_limit(int default: -1);
int max_eval_cost set_eval_limit(int default: 1);

#ifdef DEBUG_MACRO
void set_debug_level(int);
#endif

#if defined(OPCPROF) || defined(OPCPROF_2D)
void opcprof(string | void);
#endif

#ifdef PROFILE_FUNCTIONS
mapping *function_profile(object default:F_THIS_OBJECT);
#endif

#ifdef DEBUG
// void swap(object);		/* Only used for debugging */
#endif
// int resolve(string, string);

/* shutdown is at the end because it is only called once per boot cycle :) */
void shutdown(void | int);

mixed* keys_by_value(mapping);

void os_command(string, string, string);

string get_config_file(); 
int is_auth_server();

// gc
void lpc_gc(int);

string fs_memory_info();
