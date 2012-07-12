
#ifndef FS_DBI_H
#define FS_DBI_H

#include "fs_comm.h"
#include "fs_mbuf.h"

#include <stdlib.h>
#include <sys/queue.h>

FS_BEGIN_DECLS
/*
** INTEGER = char|short|int 
** STRING = char*
** TYPE = INTEGER|STRING|nil
** FIELD = INTEGER:integer_value|STRING:string_len:string_data|nil
** ATOM = FIELD
** KEY = key_len:ATOM+
** VALUE = FIELD
** GDI = (KEY:VALUE)*
*/

#define FS_DBI_INT 0
#define FS_DBI_REAL 1
#define FS_DBI_STRING 2
#define FS_DBI_ARRAY 3
#define FS_DBI_MAPPING 4
#define FS_DBI_NIL 5

#define COMMON_FIELD  int type

typedef unsigned FS_DBI_CNT_TYPE;


typedef struct fs_dbi_nil_s
{
	COMMON_FIELD;
} fs_dbi_nil_t;

typedef struct fs_dbi_int_s
{
	COMMON_FIELD;
	int n;
} fs_dbi_int_t;

typedef struct fs_dbi_real_s
{
	COMMON_FIELD;
	float r;
} fs_dbi_real_t;

typedef struct fs_dbi_string_s
{
	COMMON_FIELD;
	unsigned len;
	char str[0];
} fs_dbi_string_t;

typedef struct fs_dbi_arr_s
{
	COMMON_FIELD;
	int cnt;
} fs_dbi_arr_t;

typedef struct fs_dbi_mapping_s
{
	COMMON_FIELD;
	int pair;
} fs_dbi_mapping_t;


typedef union fs_dbi_tvalue_u 
{
	struct {
		COMMON_FIELD;
	} common;
	fs_dbi_nil_t nil;
	fs_dbi_int_t number;
	fs_dbi_real_t real;
	fs_dbi_string_t string;
	fs_dbi_arr_t arr;
	fs_dbi_mapping_t map;
} fs_dbi_tvalue_t;

////////////////////////////////

typedef struct fs_dbi_array_s
{
	unsigned *cnt;
	fs_mbuf_t mbuf;
} fs_dbi_array_t;


void fs_dbi_array_construct(fs_dbi_array_t *array);
void fs_dbi_array_destruct(fs_dbi_array_t *array);
void fs_dbi_array_reset(fs_dbi_array_t *array);

void *fs_dbi_array_alloc(fs_dbi_array_t *array, unsigned size);

void fs_dbi_array_push_int(fs_dbi_array_t *array, int n);
void fs_dbi_array_push_real(fs_dbi_array_t *array, float r);
void fs_dbi_array_push_nil(fs_dbi_array_t *array);

void fs_dbi_array_push_string(fs_dbi_array_t *array,  char *str);
void fs_dbi_array_push_lstring(fs_dbi_array_t *array,  char *str, size_t len);

void fs_dbi_array_push_array(fs_dbi_array_t *array, int cnt);
void fs_dbi_array_set_array(fs_dbi_array_t *array, void *arr, int cnt);

void fs_dbi_array_push_mapping(fs_dbi_array_t *array, int cnt);
void fs_dbi_array_set_mapping(fs_dbi_array_t *array, void *map, int pair);

int fs_dbi_tvalue_size(fs_dbi_tvalue_t *tvalue);


typedef struct fs_dbi_array_iter_s {
	int i;
	FS_DBI_CNT_TYPE cnt;
	void *arr;
	unsigned size;
	fs_dbi_tvalue_t *tv;
} fs_dbi_array_iter_t;

inline static void FS_DBI_ARRAY_ITER_FIRST(fs_dbi_array_iter_t *iter, void *arr, unsigned size) 
{
	FS_DBI_CNT_TYPE *cnt = arr;
	iter->i = 0;
	iter->cnt = *cnt;
	iter->arr = arr;
	iter->size = size;
	iter->tv = iter->cnt == 0 ? NULL : (fs_dbi_tvalue_t*)(cnt + 1);
}

inline static void FS_DBI_ARRAY_ITER_NEXT(fs_dbi_array_iter_t *iter)
{
	iter->tv = ++iter->i < iter->cnt ? 
		(fs_dbi_tvalue_t*)((char *)iter->tv + fs_dbi_tvalue_size(iter->tv))
		: NULL;
}

inline static unsigned FS_DBI_ARRAY_LEN(fs_dbi_array_t *array) 
{
	unsigned size = array->mbuf.data_size;

	if (size >= sizeof(FS_DBI_CNT_TYPE)) {
		return size - sizeof(FS_DBI_CNT_TYPE);
	} else {
		return 0;
	}
}
	
FS_END_DECLS

#endif /*FS_DBI_H*/

