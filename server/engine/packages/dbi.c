
#include "fs_dbi.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

void fs_dbi_array_construct(fs_dbi_array_t *array)
{
	fs_mbuf_init(&array->mbuf, 0);
	array->cnt = FS_MBUF_ALLOC(&array->mbuf, sizeof(*array->cnt));
	*array->cnt = 0;
}

void fs_dbi_array_reset(fs_dbi_array_t *array)
{
	fs_mbuf_reset(&array->mbuf);
	array->cnt = FS_MBUF_ALLOC(&array->mbuf, sizeof(*array->cnt));
	*array->cnt = 0;
}

void fs_dbi_array_destruct(fs_dbi_array_t *array)
{
	array->cnt = NULL;
	fs_mbuf_destruct(&array->mbuf);
}

void *fs_dbi_array_alloc(fs_dbi_array_t *array, unsigned size)
{
	void *val = FS_MBUF_ALLOC(&array->mbuf, size);
	(*array->cnt)++;
	return val;
}

void fs_dbi_array_push_int(fs_dbi_array_t *array, int n)
{
	fs_dbi_int_t *val = fs_dbi_array_alloc(array, sizeof(*val));
	val->type = FS_DBI_INT;
	val->n = n;
}

void fs_dbi_array_push_real(fs_dbi_array_t *array, float r)
{
	fs_dbi_real_t *val = fs_dbi_array_alloc(array, sizeof(*val));
	val->type = FS_DBI_REAL;
	val->r = r;
}

void fs_dbi_array_push_nil(fs_dbi_array_t *array)
{
	fs_dbi_nil_t *val = fs_dbi_array_alloc(array, sizeof(*val));
	val->type = FS_DBI_NIL;
}

void fs_dbi_array_push_lstring(fs_dbi_array_t *array,  char *str, size_t len)
{
	fs_dbi_string_t *val = fs_dbi_array_alloc(array, sizeof(*val) + len);
	val->type = FS_DBI_STRING;
	val->len = len;
	memcpy(val->str, str, len);
}

void fs_dbi_array_push_string(fs_dbi_array_t *array,  char *str)
{
	//the caller check the str not to be null
	fs_dbi_array_push_lstring(array, str, strlen(str));
}

void fs_dbi_array_push_array(fs_dbi_array_t *array, int cnt)
{
	fs_dbi_arr_t *val = fs_dbi_array_alloc(array, sizeof(*val));
	val->type = FS_DBI_ARRAY;
	val->cnt = cnt;
}

void fs_dbi_array_set_array(fs_dbi_array_t *array, void *arr, int cnt)
{
	fs_dbi_arr_t *val = arr;
	val->type = FS_DBI_ARRAY;
	val->cnt = cnt;
}

void fs_dbi_array_push_mapping(fs_dbi_array_t *array, int pair)
{
	fs_dbi_mapping_t *val = fs_dbi_array_alloc(array, sizeof(*val));
	val->type = FS_DBI_MAPPING;
	val->pair = pair;
}

void fs_dbi_array_set_mapping(fs_dbi_array_t *array, void *map, int pair)
{
	fs_dbi_mapping_t *val = map;
	val->type = FS_DBI_MAPPING;
	val->pair = pair;
}

int fs_dbi_tvalue_size(fs_dbi_tvalue_t *tvalue)
{
	switch (tvalue->common.type) {
	case FS_DBI_NIL:	
		return sizeof(fs_dbi_nil_t);
	case FS_DBI_INT:
		return sizeof(fs_dbi_int_t);
	case FS_DBI_REAL:
		return sizeof(fs_dbi_real_t);
	case FS_DBI_ARRAY:	
		return sizeof(fs_dbi_arr_t);
	case FS_DBI_MAPPING:
		return sizeof(fs_dbi_mapping_t);
	case FS_DBI_STRING:
		return (sizeof(fs_dbi_string_t) + tvalue->string.len);
	default:
		assert(0);
		return -1;
	}
}

