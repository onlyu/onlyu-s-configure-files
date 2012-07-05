
#include <libintl.h>
#include <locale.h>

#include "lpc.h"
#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "fs_lpc_op.h"

void f__()
{
	SET_ALL_ARGS;
	char *msgid = ARGS(0)->u.string;
	char *msg = gettext(msgid);
	POP_ALL_ARGS;
	copy_and_push_string(msg);
}

void f_init_gettext()
{
	SET_ALL_ARGS;
	char *domain = ARGS(0)->u.string;
	char *bind_domain = ARGS(1)->u.string;

	char *rlocale = setlocale(LC_ALL, "");
	char *rbind = bindtextdomain(domain, bind_domain);
	char *rdomain = textdomain(domain);
	POP_ALL_ARGS;

	svalue_t *v;
	mapping_t *m = allocate_mapping(1);

	v = map_insert_str_key(m, "locale");
	if (rlocale) {
		set_svalue_malloced_string(v, string_copy(rlocale, "init gettext"));
	} else {
		set_svalue_malloced_string(v, string_copy("", "init gettext"));
	}

	v = map_insert_str_key(m, "binddomain");
	if (rbind) {
		set_svalue_malloced_string(v, string_copy(rbind, "init gettext"));
	} else {
		set_svalue_malloced_string(v, string_copy("", "init gettext"));
	}

	v = map_insert_str_key(m, "domain");
	if (rdomain) {
		set_svalue_malloced_string(v, string_copy(rdomain, "init gettext"));
	} else {
		set_svalue_malloced_string(v, string_copy("", "init gettext"));
	}

	push_refed_mapping(m);
}

