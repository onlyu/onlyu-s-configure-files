#ifndef __LPC_VM_CONFIG_H__
#define __LPC_VM_CONFIG_H__

#include "cc.h"

// ------------------------------------------
// 系统必须参数
// ------------------------------------------
#define RESERVED_SIZE      (0)

#define NUM_CONSTS         (5)

#define NUM_CONFIG_STRS    (10)
#define NUM_CONFIG_INTS    (10)

#define INHERIT_CHAIN_SIZE (60)

#define TIME_TO_RESET      (600)

// src/stralloc.c里面存贮字符串的table的table_size
// 会取比HTABLE_SIZE小的最大的2^n-1
#define HTABLE_SIZE        (7001)
// 1501够吗? 2010/05/20 prophet 1501 -->> 1<<16
//#define OTABLE_SIZE        (1501)
#define OTABLE_SIZE        (1<<16)

#define MAX_MAPPING_SIZE   (60000)
#define MAX_ARRAY_SIZE     (60000)
#define MAX_BUFFER_SIZE    (200000000)
#define MAX_STRING_LENGTH  (400000)
#define READ_FILE_MAX_SIZE (400000)

#define MAX_COST           (15000000)
#define RUN_COST           (15000000)

#define MAX_BITS           (2400)

#define NULL_MSG           "0"

#define APPLY_CACHE_SIZE   (1 << APPLY_CACHE_BITS)

// ------------------------------------------


// hard code 死以下参数
#define INCLUDE_DIRS       "/include"
#define LOG_DIR            "/log"
#define SAVE_BINARIES      "/bin"

#define SIMUL_EFUN         "/rc/simul_efun"
#define DEBUG_LOG_FILE     "debug.log"
#define SWAP_FILE          "/sys/swap/swapfile"

/*
#ifdef GAME_MODULE
	// TODO:争取放入调用参数中
#endif // GAME_MODULE
#ifdef DB_MODULE
	// TODO:争取放入调用参数中
	#define QTZ_ServerEngine_LIB  "../logic_db"
#endif // DB_MODULE
#ifdef NETD_MODULE
	// TODO:争取放入调用参数中
	#define QTZ_ServerEngine_LIB  "../logic_netd"
#endif // NETD_MODULE
*/
#define QTZ_ServerEngine_LIB  "../logic"

extern char game_lib_bin_path[];
#define BIN_DIR              (game_lib_bin_path)

// 以下参数尽量由脚本提供
#define QTZ_ServerEngine_NAME "封神东游"
// TODO:确认下包大小
#define MAX_BYTE_TRANSFER  (20000)

#endif
