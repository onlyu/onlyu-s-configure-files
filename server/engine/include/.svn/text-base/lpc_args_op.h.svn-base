// lpc函数参数操作头文件
#ifndef __LPC_ARGS_OP_H__
#define __LPC_ARGS_OP_H__

#include "lpc_incl.h"

svalue_t* __args;

#define SET_ALL_ARGS \
	int __num_arg = st_num_arg; \
	__args = st_num_arg?(sp - __num_arg + 1):NULL; 

#define ARGS(n) (__args + n)
        
#define POP_ALL_ARGS do{\
        pop_n_elems(__num_arg);\
	__args = NULL;\
}while(0);

#endif // __LPC_ARGS_OP_H__
