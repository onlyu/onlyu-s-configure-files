
#ifndef _FS_MARSHAL_H
#define _FS_MARSHAL_H


#include "fs_dbi.h"
#include "lpc.h"
#include "lpc_incl.h"


void fs_marshal(svalue_t *value, fs_dbi_array_t *arr);
void fs_marshal_array(array_t *array, fs_dbi_array_t *arr);
void fs_marshal_mapping(mapping_t *map, fs_dbi_array_t *arr);

void fs_unmarshal(svalue_t *value, fs_dbi_array_iter_t *iter);
mapping_t *fs_unmarshal_mapping(fs_dbi_array_iter_t *iter);

#endif /*_fs_marshal_h */
