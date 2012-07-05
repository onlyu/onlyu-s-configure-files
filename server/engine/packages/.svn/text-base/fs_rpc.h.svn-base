
#ifndef _FS_RPC_H_
#define _FS_RPC_H_

#include "fs_mbuf.h"

#include <stdlib.h>
#include <stdint.h> //for uin32_t
#include <string.h>

#ifdef __FreeBSD__
#include <sys/stddef.h> //for offsetof
typedef uint32_t fs_protocol_id_t;
typedef int32_t fs_rpc_int_t;
typedef uint32_t fs_rpc_size_t;
#elif MS_WINDOWS

#else
#error "not support platform"
#endif

#define FS_RPC_INT 0
#define FS_RPC_STRING 1
#define FS_RPC_CLASS 2
#define FS_RPC_OBJECT 3 //only for unpack user object, 只能是rpc_server的第一个参数
#define FS_RPC_MIXED    4 //用来声明rpc_client的第一个参数，支持整数单播和整形数组多播
#define FS_RPC_BUFFER 5 //用来实现而进制的字符串

#define FS_RPC_ARRAY_VAR -1 /*变长数组, >= 0 留待扩展为定长数组*/
#define FS_RPC_ARRAY_NOT -2 /*非数组*/
#define FS_RPC_CLASS_INDEX_NOT -1

//预留一段协议id,脚本动态分配协议的起始
#define FS_RPC_LPC_PID_START 256
//预留一段协议id,c层动态分配协议的起始
//#define FS_RPC_C_PID_START 16 

//手动指定协议begin
//服务端给客户端发协议文件的内容
//#define FS_RPC_PID_UPDATE_PTO	1
//客户端给服务端发客户端的引擎版本和客户端脚本版本，外加协议文件的md5内容
//#define FS_RPC_PID_VERIFY_VERSION 2
//#define FS_RPC_PID_VERSION_RETURN 3
//手动指定协议end

#define FS_RPC_SERVER_PREFIX "rpc_server_"
#define FS_RPC_CLIENT_PREFIX "rpc_client_"

#define FS_RPC_CHECK_C_DECL(arg_cls) ((arg_cls)->field_count == 2 && (arg_cls)->field[1].type == FS_RPC_CLASS)
#define FS_RPC_STRUCT_ARG(arg_cls) ((arg_cls)->field[1].class_ptr)
/*
typedef struct fs_rpc_array_s {
	int elts;
	void *data;
} fs_rpc_array_t;
*/

typedef struct fs_rpc_string_s {
	fs_rpc_size_t n;  // strlen(str) + sizeof('\0'\)
	char *str;
} fs_rpc_string_t;


typedef void (* fs_rpc_c_function_t)(int vfd, void *arg);

typedef struct fs_rpc_field_s fs_rpc_field_t;

typedef struct fs_rpc_class_s {
	char *name;
	int c_size;
	int c_imp;
	int field_count;
	fs_rpc_field_t *field;
} fs_rpc_class_t;

struct fs_rpc_field_s {
	int type;
	int array;
	int class_index;
	fs_rpc_class_t *class_ptr;
	char *name;
	int c_offset;
	fs_rpc_class_t *parent;
};

static inline int fs_rpc_field_same(fs_rpc_field_t *lf, fs_rpc_field_t *rf)
{
	return lf->type == rf->type && lf->array == rf->array && lf->class_index == rf->class_index &&
		strcmp(lf->name, rf->name) == 0 && lf->c_offset == rf->c_offset;
}

static inline int fs_rpc_class_same(fs_rpc_class_t *lcls, fs_rpc_class_t *rcls)
{
	if (lcls == rcls) {
		return 1;
	}
	if (lcls == NULL || rcls == NULL) {
		return 0;
	}

	if ( strcmp(lcls->name, rcls->name) == 0 && 
		lcls->c_size == rcls->c_size && 
		lcls->c_imp == rcls->c_imp && 
		lcls->field_count == rcls->field_count) {
		int i;
		for (i = 0; i < lcls->field_count; ++i) {
			fs_rpc_field_t *lf = &lcls->field[i];
			fs_rpc_field_t *rf = &rcls->field[i];
			if (!fs_rpc_field_same(lf, rf)) {
				return 0;
			}
		}

		return 1;
	}

	return 0;
}

	

#define RPC_FLAG_EXCLUDED_PTO	(0x00000001)

#define TEST_RPC_EXCLUDED_PTO(flag) ((flag) & RPC_FLAG_EXCLUDED_PTO)
#define SET_RPC_EXCLUDED_PTO(flag) ((flag) |= RPC_FLAG_EXCLUDED_PTO)
#define CLEAR_RPC_EXCLUDED_PTO(flag) ((flag) &= ~RPC_FLAG_EXCLUDED_PTO)

typedef struct fs_rpc_function_s {
	int function_id;
	char *module;
	fs_rpc_class_t arg_class;
	fs_rpc_c_function_t c_function;
	void *udata;
	unsigned flag;
} fs_rpc_function_t;

typedef struct fs_rpc_field_meta_s {
	const char *name;
	int c_offset; //offsetof
} fs_rpc_field_meta_t;

typedef struct fs_rpc_class_meta_s {
	const char *name;
	int c_size; //sizeof
	int field_count;
	fs_rpc_field_meta_t *field_meta;
} fs_rpc_class_meta_t;

typedef struct fs_rpc_function_meta_s {
	const char *name;
	fs_rpc_c_function_t c_function;
} fs_rpc_function_meta_t;

typedef struct fs_rpc_class_table_s {
	int elts;
	fs_rpc_class_t *rpc_class;
} fs_rpc_class_table_t;

typedef struct fs_rpc_function_table_s {
	int elts;
	fs_rpc_function_t *rpc_function;
	//稀疏数组，用function_id做index来查找，但是数组里有空洞
	int sparse_elts;
	fs_rpc_function_t **sparse_rpc_function;
} fs_rpc_function_table_t;

typedef struct fs_rpc_table_s {
	fs_rpc_function_table_t *function_table;
	fs_rpc_class_table_t *class_table;
} fs_rpc_table_t;

#if defined(__cplusplus)
extern "C" {
#endif

int fs_rpc_table_create(fs_rpc_table_t *table, const char *file);
void fs_rpc_table_destroy(fs_rpc_table_t *table);
int fs_rpc_dump_cfg(fs_rpc_table_t *table, const char *file);
int fs_rpc_dump_c(fs_rpc_table_t *table, const char *hfile, const char *cfile);
int fs_rpc_dump_function_id(fs_rpc_function_table_t *table, const char *file);
int fs_rpc_dump_function_c_id(fs_rpc_function_table_t *table, const char *file);

void class_free(fs_rpc_class_t *cls);
fs_rpc_class_t *fs_rpc_class(fs_rpc_class_table_t *table, int index);
fs_rpc_function_t *fs_rpc_function(fs_rpc_function_table_t *table, int index);
int fs_rpc_class_index(fs_rpc_class_table_t *table, const char *name);
fs_rpc_function_t *fs_rpc_function_find(fs_rpc_function_table_t *table, const char *name);

int fs_rpc_function_set_id(fs_rpc_function_table_t *function_table, const char **names, const int *ids, size_t len);
void fs_rpc_function_qsort(fs_rpc_function_table_t *function_table);
fs_rpc_function_t *fs_rpc_function_bsearch(fs_rpc_function_table_t *table, fs_protocol_id_t function_id);

void fs_rpc_function_sort(fs_rpc_function_table_t *function_table);
fs_rpc_function_t *fs_rpc_function_at(fs_rpc_function_table_t *table, fs_protocol_id_t function_id);

int fs_rpc_c_pack(fs_rpc_function_t *function, void *struct_value, fs_mbuf_t *mbuf);
void * fs_rpc_c_unpack(fs_rpc_function_t *function, fs_mbuf_t *mbuf, fs_inbuf_t *inbuf);

void fs_rpc_foreach_function(fs_rpc_function_table_t *table, int (*action)(fs_rpc_function_t *fun, void *udata), void *udata);
char *fs_rpc_field_type_name(fs_rpc_field_t *field);

extern int g_log_pto_enable;
void enable_log_pto(int enable);
void log_pack(fs_rpc_function_t *function, fs_mbuf_t *mbuf);
void log_unpack(fs_rpc_function_t *function, fs_inbuf_t *inbuf);

#if defined(__cplusplus)
}
#endif

#endif /*_FS_RPC_H_ */

