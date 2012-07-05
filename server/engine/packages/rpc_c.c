
#include "fs_rpc_c.h"


fs_rpc_class_meta_t fs_rpc_class_metas[] = {
	{NULL, 0, 0, NULL,},
}; 

#ifdef __FreeBSD__
fs_rpc_function_meta_t fs_rpc_function_metas[] = {
	{NULL, NULL,},
};
#elif MS_WINDOWS
fs_rpc_function_meta_t fs_rpc_function_metas[] = {
	{NULL, NULL,},
};
#else
#error "not support platform"
#endif
