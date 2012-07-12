
#include "fs_marshal.h"

#include "fs_dbi.h"
#include "lpc.h"
#include "lpc_incl.h"

#include <assert.h>

void fs_marshal_array(array_t *array, fs_dbi_array_t *arr);

inline static void fs_marshal_(svalue_t *value, fs_dbi_array_t *arr)
{
	int type = value->type;

	if (type & T_NUMBER) {
		if (value == &const0u) {
			fs_dbi_array_push_nil(arr);
		}
		else {
			fs_dbi_array_push_int(arr, value->u.number);
		}
	} else if (type & T_STRING) {
		fs_dbi_array_push_lstring(arr, value->u.string, SVALUE_STRLEN(value));
	} else if (type & T_MAPPING) {
		fs_marshal_mapping(value->u.map, arr);
	} else if (type & T_ARRAY) {
		fs_marshal_array(value->u.arr, arr);
	} else if (type & T_REAL) {
		fs_dbi_array_push_real(arr, value->u.real);
	} else {
		fprintf(stderr, "[write_buffer_mixed2_local] unknow data type [%s]\n", type_name(type));
		assert(0);
	}
}

void fs_marshal_mapping(mapping_t *map, fs_dbi_array_t *arr)
{
	int i;
	mapping_node_t *elt, **table;

	fs_dbi_array_push_mapping(arr, map->count);

	for (i = 0, table = map->table; i <= map->table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			fs_marshal_(&elt->values[0], arr);
			fs_marshal_(&elt->values[1], arr);
		}
	}
}


void fs_marshal_array(array_t *array, fs_dbi_array_t *arr)
{
	int i;

	fs_dbi_array_push_array(arr, array->size);
	for (i = 0; i < array->size; i++)
		fs_marshal_(&array->item[i], arr);
}

void fs_marshal(svalue_t *value, fs_dbi_array_t *arr)
{
	fs_marshal_(value, arr);
}


inline static void fs_unmarshal_(svalue_t *value, fs_dbi_array_iter_t *iter);

static array_t *fs_unmarshal_array(fs_dbi_array_iter_t *iter)
{
	int i;
	int cnt = iter->tv->arr.cnt;
	array_t *arr = allocate_array(cnt);

	FS_DBI_ARRAY_ITER_NEXT(iter); //skip the array header
	for (i = 0; i < arr->size; i++)
		fs_unmarshal_(&arr->item[i], iter);

	return arr;
}


mapping_t *fs_unmarshal_mapping(fs_dbi_array_iter_t *iter)
{
	int i;
	svalue_t k, *v;
	int pair = iter->tv->map.pair;
	mapping_t *map = allocate_mapping(pair);

	FS_DBI_ARRAY_ITER_NEXT(iter); //skip the map header
	for(i = 0; i < pair; ++i) {
		fs_unmarshal_(&k, iter); //must be string or number
		assert(k.type & T_NUMBER || k.type & T_REAL || k.type & T_STRING);
		v = find_for_insert(map, &k, 0);
		fs_unmarshal_(v, iter); 
	}

	return map;
}

inline static void fs_unmarshal_(svalue_t *value, fs_dbi_array_iter_t *iter)
{
	fs_dbi_tvalue_t *tv = iter->tv;

	switch (tv->common.type) {
	case FS_DBI_NIL:	
		set_svalue_undefined(value);
		FS_DBI_ARRAY_ITER_NEXT(iter);
		break;
	case FS_DBI_INT: {
		set_svalue_number(value, tv->number.n);
		FS_DBI_ARRAY_ITER_NEXT(iter);
		break;
	}
	case FS_DBI_REAL: {
		set_svalue_real(value, tv->real.r);
		FS_DBI_ARRAY_ITER_NEXT(iter);
		break;
	}
	case FS_DBI_STRING: {
		int len = tv->string.len;
		char *p = new_string(len, "unmarsh dbi");
		memcpy(p, tv->string.str, len + 1);
		p[len] = '\0';
		set_svalue_malloced_string(value, p);
		FS_DBI_ARRAY_ITER_NEXT(iter);
		break;
	}
	case FS_DBI_ARRAY: {
		array_t *arr = fs_unmarshal_array(iter);
		set_svalue_array(value, arr);
		break;
	}
	case FS_DBI_MAPPING: {
		mapping_t *map = fs_unmarshal_mapping(iter);
		set_svalue_mapping(value, map);
		break;
	}
	default:
		assert(0);
	}
}



void fs_unmarshal(svalue_t *value, fs_dbi_array_iter_t *iter)
{
	fs_unmarshal_(value, iter);
}


