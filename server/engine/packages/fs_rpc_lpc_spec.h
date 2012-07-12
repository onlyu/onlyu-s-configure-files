
#include "../grammar/spec.h"

/* fs_rpc_call(int|int* vfd, int protocol_id, rpc_argv...) */
void fs_rpc_call(int|int*, int, ...);
void fs_rpc_call_loop(int, int, ...);
int fs_last_pto_timestamp(int);
void fs_rpc_make_cfg(mapping *, string, string);
void fs_exclude_stats_pto(int *);  
// uid, threshold, step, mod
void fs_set_validation(int, int, int, int);
void fs_validation_start(int);

