// 封神主程序的.h
// ------------------------------------
#ifndef __FS_MAIN_H__
	#define __FS_MAIN_H__

	#include <std.h>
	#include <config.h>
	#include <event.h>
	#include <signal.h> 
	#include "lpc.h"
	#include "lpc_incl.h"

	extern void shutdownOS(int exit_code);
	extern svalue_t *call_script_func(char *objectName, char *funcName, char *paramFmt, ...);

	// 事件的优先级管理
	// -------------------------------------------------------
	// 系统设置5级事件管理
	// 0 信号处理,保证信号在每帧最优先被处理
	// 1 预留
	// 2 一般网络包处理用
	// 3 预留
	// 4 给心跳(call_out)使用
	// 为防止心跳优先级别太低无法产生心跳,在网络包处理N个之后，
	// 必须check一下心跳。这样在系统网络繁忙的时候，心跳可能会由
	// 网络包的收发来激活。
	// -------------------------------------------------------
	#define FS_EV_PRIORITY_MAX     (5)
	#define FS_EV_PRIORITY_SIG     (0)
	#define FS_EV_PRIORITY_REMARK1 (1)
	#define FS_EV_PRIORITY_NET     (2)
	#define FS_EV_PRIORITY_REMARK2 (3)
	#define FS_EV_PRIORITY_CAL     (4)

	// RPC X机制的类型
	#define T_RPC_X_USR_FIELD      (1)
	#define T_RPC_X_USR_SCENE      (2)
	#define T_RPC_X_USR_FIELD_NOT_FIGHT     (3)                                                                                                                                                      
	#define T_RPC_X_USR_SCENE_NOT_FIGHT     (4)
	#define T_RPC_X_SCENE          (5) 
	#define T_RPC_X_ALL_USERS      (6) 
	#define T_RPC_X_SCENE_NOT_FIGHT   (7) 


	// 定义一个timer回调的结构
	// 记录每个timer的类型
	// 0 TIMER_PERSIST 永久
	// 1 TIMER_ONCE    一次性
	#define TIMER_PERSIST    0
	#define TIMER_ONCE       1
	typedef struct fs_evtimer_s
	{
		struct event timer;
		// 类型
		char type;
		// 周期
		struct timeval period;
		// 回调函数
		void(*callback) (void*);
		// 回调参数
		void *arg;
	} fs_evtimer_t;

	// 增加一个永久的timer
	int add_timer(struct timeval tv, void(*callback) (void*), void *arg);
	// 增加一个一次性的timer
	int add_timer_once(struct timeval tv, void(*callback) (void*), void *arg);
	void restore_gamed_proc_title();

#endif // __FS_MAIN_H__
