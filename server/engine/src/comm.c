/*
 *  comm.c -- communications functions and more.
 *            Dwayne Fontenot (Jacques@TMI)
 *  Windows 95 support by George Reese (Descartes of Borg)
 */
#include "std.h"
#include "lpc_incl.h"
#include "applies.h"
#include "main.h"
#include "comm.h"
#include "backend.h"
#include "eoperators.h"
#include "debug.h"
#include "file.h"

#define _debug( fmt, args... ) 	

int total_users = 0;
int kq;
struct kevent *eventlist;


void outbuf_zero(outbuffer_t *outbuf)
{
	outbuf->real_size = 0;
	outbuf->buffer = 0;
} 

int outbuf_extend(outbuffer_t *outbuf, int l)
{
	int limit;
	DEBUG_CHECK(l < 0, "Negative length!.\n");

	if (outbuf->buffer)
	{
		limit = MSTR_SIZE(outbuf->buffer);
		if (outbuf->real_size + l > limit)
		{
			if (outbuf->real_size == USHRT_MAX)
			{
				return 0;
			}
			 /* TRUNCATED */

			/* assume it's going to grow some more */
			limit = (outbuf->real_size + l) *2;
			if (limit > USHRT_MAX)
			{
				limit = outbuf->real_size + l;
				if (limit > USHRT_MAX)
				{
					outbuf->buffer = extend_string(outbuf->buffer, USHRT_MAX);
					return USHRT_MAX - outbuf->real_size;
				}
			}
			outbuf->buffer = extend_string(outbuf->buffer, limit);
		}
	}
	else
	{
		outbuf->buffer = new_string(l, "outbuf_add");
		outbuf->real_size = 0;
	}
	return l;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void outbuf_add(outbuffer_t *outbuf, char *str)
{
	int l, limit;

	if (!outbuf)
	{
		return ;
	}
	l = strlen(str);
	if (outbuf->buffer)
	{
		limit = MSTR_SIZE(outbuf->buffer);
		if (outbuf->real_size + l > limit)
		{
			if (outbuf->real_size == USHRT_MAX)
			{
				return ;
			}
			 /* TRUNCATED */

			/* assume it's going to grow some more */
			limit = (outbuf->real_size + l) *2;
			if (limit > USHRT_MAX)
			{
				limit = outbuf->real_size + l;
				if (limit > USHRT_MAX)
				{
					outbuf->buffer = extend_string(outbuf->buffer, USHRT_MAX);
					strncpy(outbuf->buffer + outbuf->real_size, str, USHRT_MAX - outbuf->real_size);
					outbuf->buffer[USHRT_MAX] = 0;
					outbuf->real_size = USHRT_MAX;
					return ;
				}
			}
			outbuf->buffer = extend_string(outbuf->buffer, limit);
		}
	}
	else
	{
		outbuf->buffer = new_string(l, "outbuf_add");
		outbuf->real_size = 0;
	}
	strcpy(outbuf->buffer + outbuf->real_size, str);
	outbuf->real_size += l;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void outbuf_addchar(outbuffer_t *outbuf, char c)
{
	int limit;

	if (!outbuf)
	{
		return ;
	}

	if (outbuf->buffer)
	{
		limit = MSTR_SIZE(outbuf->buffer);
		if (outbuf->real_size + 1 > limit)
		{
			if (outbuf->real_size == USHRT_MAX)
			{
				return ;
			}
			 /* TRUNCATED */

			/* assume it's going to grow some more */
			limit = (outbuf->real_size + 1) *2;
			if (limit > USHRT_MAX)
			{
				limit = outbuf->real_size + 1;
				if (limit > USHRT_MAX)
				{
					outbuf->buffer = extend_string(outbuf->buffer, USHRT_MAX);
					*(outbuf->buffer + outbuf->real_size) = c;
					outbuf->buffer[USHRT_MAX] = 0;
					outbuf->real_size = USHRT_MAX;
					return ;
				}
			}
			outbuf->buffer = extend_string(outbuf->buffer, limit);
		}
	}
	else
	{
		outbuf->buffer = new_string(80, "outbuf_add");
		outbuf->real_size = 0;
	}
	*(outbuf->buffer + outbuf->real_size++) = c;
	*(outbuf->buffer + outbuf->real_size) = 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void outbuf_addv(outbuffer_t *outbuf, char *format, ...)
{
	char buf[LARGEST_PRINTABLE_STRING + 1];
	va_list args;
	V_DCL(char *format);
	V_DCL(outbuffer_t *outbuf);

	V_START(args, format);
	V_VAR(outbuffer_t *, outbuf, args);
	V_VAR(char *, format, args);

	vsprintf(buf, format, args);
	va_end(args);

	if (!outbuf)
	{
		return ;
	}

	outbuf_add(outbuf, buf);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void outbuf_fix(outbuffer_t *outbuf)
{
	if (outbuf && outbuf->buffer)
	{
		outbuf->buffer = extend_string(outbuf->buffer, outbuf->real_size);
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void outbuf_push(outbuffer_t *outbuf)
{
	(++sp)->type = T_STRING;
	if (outbuf && outbuf->buffer)
	{
		outbuf->buffer = extend_string(outbuf->buffer, outbuf->real_size);

		sp->subtype = STRING_MALLOC;
		sp->u.string = outbuf->buffer;
	}
	else
	{
		sp->subtype = STRING_CONSTANT;
		sp->u.string = "";
	}
}
