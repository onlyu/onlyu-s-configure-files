#ifndef __FS_CALLOUT_H__
#define __FS_CALLOUT_H__ 

#include "uthash.h"

typedef struct fs_pending_callout_s {
	// handler
	int handler;
	// del flg
	int del_flg;
	// ����ָ����ߺ�����
	union string_or_func function;
	// ����
	object_t *ob;
	// ����
	array_t *vs;

	UT_hash_handle hh;
} fs_pending_callout_t;


// ʱ��ϵͳ��ʼ������
int callout_init( int sec, int usec);
void update_clock(void* arg);

#endif
