
#ifndef _FS_LPCOB_TABLE_H_
#define _FS_LPCOB_TABLE_H_

#include "fs_hlist.h"
#include "fs_mem_pool.h"
#include "lpc_incl.h"

typedef struct fs_lpc_object_s {
	object_t *ob;
	/* save info*/
	mapping_t *save_map;

	struct fs_lpc_object_s *next;
} fs_lpc_object_t;

typedef struct fs_char_ob_htbl_s {
	fs_lpc_object_t ** htbl;
	int num;
	int hsize;
} fs_char_ob_htbl_t;

void fs_update_lpc_object(object_t *);
fs_lpc_object_t* fs_find_lpc_object(char *path);


#endif
