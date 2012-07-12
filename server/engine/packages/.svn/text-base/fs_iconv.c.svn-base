

#include <iconv.h>
//#include <stdio.h>
//#include <string.h>

#include "lpc.h"
#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "fs_lpc_op.h"

//string content, string from_encoding string to_encoding
void f_fs_iconv()
{
	SET_ALL_ARGS;
	//todo check the type
	svalue_t *svcontent = ARGS(0);
	svalue_t *svfrom = ARGS(1);
	svalue_t *svto = ARGS(2);

	char *from = svfrom->u.string;
	char *to = svto->u.string;
	const char *content = svcontent->u.string;
	size_t contentlen = SVALUE_STRLEN(svcontent);
	size_t buflen = contentlen * 3 + 1;
	char *buf = calloc(1, buflen);
	size_t left = buflen;
	char *result = buf;

	iconv_t cnv = iconv_open(to, from);
	if (cnv == (iconv_t)(-1)) {
		goto error;
	}
	if (iconv(cnv, &content, &contentlen, &result, &left) == (size_t)(-1)) {
		goto error;
	}
	if (-1 == iconv_close(cnv)) {
		goto error;
	}

	if (left == 0) {
		goto error;
	}
	//为了保证安全性不crash，强制在buf后面加一个 '\0',如果是二进制的数据，lpc的string 就处理不了了。
	buf[buflen - left] = '\0';

	POP_ALL_ARGS;
	copy_and_push_string(buf);
	free(buf);
	return;
error:
	POP_ALL_ARGS;
	push_undefined();
	free(buf);
}

