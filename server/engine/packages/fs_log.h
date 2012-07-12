#ifndef _FS_LOG_H_
#define _FS_LOG_H_

#include <pthread.h>
#include <errno.h>
#include <assert.h>

typedef enum {
	MOD_ALL = 0,            /*所有模块都打开*/
	MOD_SCENE,
	MOD_CHAR_OB,
	MOD_ITEM,
	MOD_SUMMON,
	MOD_NPC,
	MOD_DATA_MAP,
	MOD_DB_CACHE,
	MOD_TEAM,
	MOD_MEM_POOL,
	MOD_NETD,
	MOD_GAMED,
	MOD_DB,
	MOD_NONE                /*所有LOG都关闭*/
} LOG_MODULE_E;

typedef enum {
	LOG_ALL   = 0x00,
	LOG_DEBUG = 0x01,        /*调试信息*/
	LOG_INFO  = 0x02,        /*常规信息*/
	LOG_WARNING = 0x04,
	LOG_ERROR = 0x08,        /*错误信息*/
	LOG_FATAL = 0x10,        
	LOG_NONE  = 0xFF
} LOG_LEVEL_E;

void fs_debug_printf(LOG_MODULE_E e_mod, LOG_LEVEL_E e_level, const char *strfile, int line, const char *pformat, ...);

#define log_print(mod, level, log...) \
    fs_debug_printf(mod, level, __FILE__, __LINE__, log)

#define log_debug(mod, log...) \
    fs_debug_printf(mod, LOG_DEBUG, __FILE__, __LINE__, log)

#define log_info(mod, log...) \
    fs_debug_printf(mod, LOG_INFO, __FILE__, __LINE__, log)

#define log_warning(mod, log...) \
    fs_debug_printf(mod, LOG_WARNING, __FILE__, __LINE__, log)

#define log_error(mod, log...) \
    fs_debug_printf(mod, LOG_ERROR, __FILE__, __LINE__, log)

#define log_fatal(mod, log...) \
    fs_debug_printf(mod, LOG_FATAL, __FILE__, __LINE__, log)


extern pthread_mutex_t dbd_log_lock;

#define LOG_LOCK_AQUIRE int threrrno = errno; {(void)threrrno; int ret = pthread_mutex_lock(&dbd_log_lock); assert(ret == 0);}
#define LOG_LOCK_RELEASE { int ret = pthread_mutex_unlock(&dbd_log_lock); assert(ret == 0);}

#define log_info_thr_safe(mod, log...)  do {\
	LOG_LOCK_AQUIRE;\
	log_info(mod, log);\
	LOG_LOCK_RELEASE;\
} while(0)

#define log_warning_thr_safe(mod, log...)  do {\
	LOG_LOCK_AQUIRE;\
	log_warning(mod, log);\
	LOG_LOCK_RELEASE;\
} while(0)

#define log_error_thr_safe(mod, log...)  do {\
	LOG_LOCK_AQUIRE;\
	log_error(mod, log);\
	LOG_LOCK_RELEASE;\
} while(0)


void fs_set_log_switch_level(LOG_LEVEL_E e_switch_level);
void fs_set_log_switch_module(LOG_MODULE_E e_switch_module);

#endif
