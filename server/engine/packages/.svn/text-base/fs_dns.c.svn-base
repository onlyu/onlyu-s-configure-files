
#include <netinet/in.h>
#include <event.h>
#include <evdns.h>
#include <errno.h>

#include "lpc.h"
#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "fs_lpc_op.h"
#include "fs_curl.h"

typedef struct {
	svalue_t hostname;
	svalue_t function;
	svalue_t udata;
} fs_resolve_t;

static void free_resolve(fs_resolve_t *resolv)
{
	free_svalue(&resolv->hostname, "dns");
	free_svalue(&resolv->function, "dns");
	free_svalue(&resolv->udata, "dns");
	free(resolv);
}

static void evdns_callback(int result, char type, int count, int ttl, void *addresses, void *arg)
{
	fs_resolve_t *resolv = (fs_resolve_t*)arg;
	int argc = 2;

	if (result == DNS_ERR_NONE) { 
		mapping_t *iplist = allocate_mapping(0);
		svalue_t *v;
		char buf[256];
		int i;
		//only support ip4
		if (type == EVDNS_TYPE_A && count >= 0) {
			for(i = 0; i < count; ++i) {
				memset(buf, 0, sizeof(buf));
				if (NULL != inet_ntoa_r(((struct in_addr *)addresses)[i], buf, sizeof(buf))) {
					v = map_insert_str_key(iplist, buf);
					set_svalue_number(v, ttl);
				}
			}
		}
		push_refed_mapping(iplist);
        } else {
		const char * err = evdns_err_to_string(result);
		copy_and_push_string((char*)err);
	}

	push_svalue(&resolv->udata);
	safe_call_function_pointer(resolv->function.u.fp, argc);
	free_resolve(resolv);
}

int fs_dns_init()
{
	return evdns_init();
}

void fs_dns_deinit()
{
	//nop
}

static int resolve(svalue_t *hostname, svalue_t *function, svalue_t *udata)
{
	if (hostname->type != T_STRING) {
		return -1;
	}
	if (function->type != T_FUNCTION) {
		return -1;
	}

	fs_resolve_t *resolv = calloc(sizeof(fs_resolve_t), 1);
	resolv->hostname = const0u;
	resolv->function = const0u;
	resolv->udata = const0u;
	assign_svalue(&resolv->hostname, hostname);
	assign_svalue(&resolv->function, function);
	assign_svalue(&resolv->udata, udata);

	if (0 == evdns_resolve_ipv4(resolv->hostname.u.string, 0, evdns_callback, resolv)) {
		return 0;
	} else {
		free_resolve(resolv);
		return -1;
	}
}

void f_fs_dns_resolve()
{
	int ret;
	SET_ALL_ARGS;
	svalue_t *hostname = ARGS(0);
	svalue_t *function = ARGS(1);
	svalue_t *udata = ARGS(2);
	ret = resolve(hostname, function, udata);
	POP_ALL_ARGS;
	push_number(ret);
}

