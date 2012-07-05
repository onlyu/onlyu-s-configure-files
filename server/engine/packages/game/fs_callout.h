#ifndef __FS_CALLOUT_H__
#define __FS_CALLOUT_H__ 

#include "uthash.h"

typedef struct fs_pending_callout_s {
	// handler
	int handler;
	// del flg
	int del_flg;
	// 函数指针或者函数名
	union string_or_func function;
	// 对象
	object_t *ob;
	// 参数
	array_t *vs;

	UT_hash_handle hh;
} fs_pending_callout_t;


// 时钟系统初始化函数
int callout_init( int sec, int usec);
void update_clock(void* arg);

#endif
