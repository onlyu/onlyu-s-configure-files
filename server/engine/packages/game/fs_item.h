#ifndef _FS_ITEM_H_
#define _FS_ITEM_H_

#include "fs_char_ob.h"
#include "fs_common.h"
#include "fs_prop_key.h"

typedef struct fs_item_object_s {
	// item��Ҫ���ƵĶ���, ����û�У�
} fs_item_object_t;

// ��ʼ��item module
void fs_sys_init_item_module(int max, int startid, int endid);
// �½�һ����Ʒ
int fs_new_item(char *type);
// ����һ����Ʒ
int fs_generate_item(mapping_t *m);
// ����һ����Ʒ
void fs_destroy_item(int itemid);

inline int is_item_online(int itemid);
#endif
