
#ifndef _FS_DB_H
#define _FS_DB_H

#include "lpc.h"
#include "lpc_incl.h"
#include "fs_user.h"
#include "fs_ipc.h"

typedef struct {
	char *name;
	unsigned index;
} fs_obj_save_meta_t;

typedef struct fs_obj_save_vars_s {
	unsigned cnt;
	fs_obj_save_meta_t meta[0];
} fs_obj_save_vars_t;

extern char *dat_var_list;
extern char *user_var_list;

fs_obj_save_vars_t *fs_obj_save_vars_new(object_t *obj, char *var_list);
void fs_obj_save_vars_free(fs_obj_save_vars_t *save_vars);
fs_obj_save_vars_t *fs_obj_save_vars_renew(fs_obj_save_vars_t *save_vars, object_t *obj, char *var_list);

int init_user_save_var(object_t *user_obj);

int fs_db_object_recv(fs_gd_header_t *header, void *data, size_t len);
#endif 

