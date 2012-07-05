// ==========================================
// Purpose  : lpc操作
// Created  : 2010-04-30
// By       : Mouse
// History  : 2010-04-30  Mouse create
// ==========================================

// LPC 数据类型(svalue_t *)的操作宏
//
// 说明: efun返回lpc类型的时候, 需要注意,如果是array_t, mapping_t需要增加引数. 
//       因为如果是脚本局部变量在函数结束,将会释放,引数减1. 
//       参考组队模块.
//
//       efun从脚本传进来的array_t, mapping_t类型, 压栈时引数会加1;
//

#ifndef _FS_LPC_OP_H_
#define _FS_LPC_OP_H_

#include "lpc_incl.h"

#define IS_LPC_INT(x) ((x)->type == T_NUMBER)
#define IS_LPC_STR(x) ((x)->type == T_STRING)
#define IS_LPC_ARR(x) ((x)->type == T_ARRAY)
#define IS_LPC_MAP(x) ((x)->type == T_MAPPING)
#define IS_LPC_UNDEF(x) ((x)->type == T_NUMBER && (x)->subtype == T_UNDEFINED)

#define IS_NULL(x) ((x) == &const0u)


#define TO_LPC_STR(s, x) do{ \
	s.type = T_STRING; \
	s.subtype = STRING_CONSTANT; \
	s.u.string=x; \
}while(0)

#define TO_LPC_INT(s, x) do { \
	s.type = T_NUMBER; \
	s.u.number = x; \
}while(0)

#define TO_LPC_MAP(s, x) do { \
	s.type = T_MAPPING; \
	s.u.map = x; \
}while(0)

#define LPC_ARR_SIZE(s)   (s)->u.arr->size
#define LPC_ARR_ITEM(s,x) (&((s)->u.arr->item[x]))

#define LPC_INT_VALUE(s)  ((s)->u.number)
#define LPC_STR_VALUE(s)  ((s)->u.string)
#define LPC_MAP_VALUE(s)  ((s)->u.map)
#define LPC_ARR_VALUE(s)  ((s)->u.arr)

#endif
