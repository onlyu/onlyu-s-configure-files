#include "marsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m_buffer_allocate(m_buffer_t *buf, unsigned int len)
{
	buf->data = (char*)pool_malloc(len);
	if (buf->data == 0)
	{
		return  - 1;
	}
	buf->pos = 0;
	buf->len = len;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void m_buffer_free(m_buffer_t *buf)
{
	pool_free(buf->data);
	buf->data = NULL;
	buf->pos = 0;
	buf->len = 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#define DECLARE_PUSH_INT(bit, type)\
int push_int##bit(m_buffer_t *p, type s)\
{\
if (p == NULL || p->pos + sizeof(type) > p->len || p->data == NULL) {\
return -1;\
}\
memcpy(p->data + p->pos, &s, sizeof(type));\
p->pos += sizeof(type);\
return 0;\
}

DECLARE_PUSH_INT(8, char)DECLARE_PUSH_INT(16, short)DECLARE_PUSH_INT(32, int)

int push_shortstr(m_buffer_t *p, char *s)
{
	size_t len;

	if (p == NULL)
	{
		return  - 1;
	}
	len = strlen(s);
	if (p->pos + sizeof(char) + len > p->len || p->data == NULL)
	{
		return  - 1;
	}
	push_int8(p, len);
	memcpy(p->data + p->pos, s, len);
	p->pos += len;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int push_varstr(m_buffer_t *p, char *s)
{
	size_t len;

	if (p == NULL)
	{
		return  - 1;
	}
	len = strlen(s);
	if (p->pos + sizeof(short) + len > p->len || p->data == NULL)
	{
		return  - 1;
	}
	push_int16(p, len);
	memcpy(p->data + p->pos, s, len);
	p->pos += len;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int push_raw(m_buffer_t *p, char *s, unsigned int len)
{
	if (p == NULL || p->pos + len > p->len || p->data == NULL)
	{
		return  - 1;
	}
	memcpy(p->data + p->pos, s, len);
	p->pos += len;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int s_push_int32(m_buffer_t *p, int s)
{
	if (p == NULL)
	{
		return  - 1;
	}
	if (push_int32(p, s) ==  - 1)
	{
		char *tmp;
		size_t tlen = p->len;

		while (p->pos + sizeof(int) > tlen)
		{
			tlen += 256;
		}
		tmp = (char*)pool_realloc(p->data, tlen);
		if (tmp == NULL)
		{
			return  - 1;
		}
		p->data = tmp;
		p->len = tlen;
		return push_int32(p, s);
	}
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int s_push_raw(m_buffer_t *p, char *s, unsigned int len)
{
	if (p == NULL)
	{
		printf("%s %d\n", __FILE__, __LINE__);
		return  - 1;
	}
	//ÄÚ´æ²»¹»ÁË
	if (push_raw(p, s, len) ==  - 1)
	{
		char *tmp;
		size_t tlen = p->len;

		while (p->pos + len > tlen)
		{
			tlen += 256;
		}
		tmp = (char*)pool_realloc(p->data, tlen);
		if (tmp == NULL)
		{
			printf("%s %d\n", __FILE__, __LINE__);
			return  - 1;
		}
		p->data = tmp;
		p->len = tlen;
		return push_raw(p, s, len);
	}
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int s_push_str(m_buffer_t *p, char *s)
{
	return s_push_raw(p, s, strlen(s) );
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int m_size(m_buffer_t *buf)
{
	return buf->pos;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

char *m_data(m_buffer_t *buf)
{
	return buf->data;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void m_attach(m_buffer_t *buf, char *data, unsigned int len)
{
	buf->len = len;
	buf->pos = 0;
	buf->data = data;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#define DECLARE_POP_INT(bit, type)\
int pop_int##bit(m_buffer_t *up, type *d)\
{\
if (up == NULL || up->data == NULL || up->pos + sizeof(type) > up->len ) {\
return -1;\
}\
memcpy(d, up->data + up->pos, sizeof(type));\
up->pos += sizeof(type);\
return 0;\
}

DECLARE_POP_INT(8, char)DECLARE_POP_INT(16, short)DECLARE_POP_INT(32, int)

int pop_varstr(m_buffer_t *up, char *data, unsigned int data_len)
{
	short len;
	unsigned short tmp;

	if (up == NULL || up->data == NULL || up->pos + sizeof(short) > up->len)
	{
		return  - 1;
	}
	pop_int16(up, &len);
	if (up->pos + len > up->len)
	{
		return  - 1;
	}
	tmp = len;
	if (data_len < tmp)
	{
		tmp = data_len;
	}
	memcpy(data, up->data + up->pos, tmp);
	if (tmp >= data_len)
	{
		data[data_len - 1] = '\0';
	}
	else
	{
		data[tmp] = '\0';
	}
	up->pos += len;
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int pop_raw(m_buffer_t *up, char *data, unsigned int len)
{
	if (up == NULL || up->data == NULL || up->pos + len > up->len)
	{
		return  - 1;
	}
	memcpy(data, up->data + up->pos, len);
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void m_buffer_clean(m_buffer_t *buf)
{
	buf->pos = 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef TEST
	#include <stdio.h>
	int main(int arg, char *argv[])
	{
		m_buffer_t mb;
		char buf[256];
		char *src = "abcdefghijklmnABASDFASDFS";
		int i = 0;
		m_buffer_allocate(&mb, 1);
		for (i = 0; i < 200; ++i)
		{
			s_push_raw(&mb, src, strlen(src) );
			s_push_raw(&mb, " ", 1);
			s_push_raw(&mb, src, strlen(src) );
			s_push_raw(&mb, "\n", 1);
		}
		printf("RES:%s %d %d\n", m_str(&mb), m_size(&mb), mb.len);
		m_buffer_free(&mb);
	}
#endif
