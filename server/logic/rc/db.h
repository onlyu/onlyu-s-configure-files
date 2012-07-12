
#ifndef _FS_DB_H_
#define _FS_DB_H_

#include <user_key.h>
#include <debug.h>

#define SAVE_ALL 0
#define SAVE_DIRTY 1

#define LOAD_ASYNC 0
#define LOAD_SYNC 1

#define USER_VLOGIN 0
#define USER_LOGIN 1
#define SIMULATE_USER_LOGIN 2

#define DB_TYPE_USER 1
#define DB_TYPE_DAT 2

#define DB_STAT_OK 0
#define DB_STAT_NULL 1
#define DB_STAT_EROR 2

#define DB_SYNC_SINGLE_THREAD 0
#define DB_SYNC_MULTI_W_THREAD 1
#define DB_SYNC_MULTI_RW_THREAD 2

#endif
