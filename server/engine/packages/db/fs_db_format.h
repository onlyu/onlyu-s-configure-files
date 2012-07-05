
#ifndef _FS_DB_FORMAT_H_
#define _FS_DB_FORMAT_H_


#include "lpc.h"
#include "lpc_incl.h"
#include <event.h>

int fs_db_format_thr_safe(svalue_t *sv, struct evbuffer *outbuf);
int fs_db_unformat(char *pathname, char *source, size_t len, svalue_t *value);

#endif /*_FS_DB_FORMAT_H_*/
