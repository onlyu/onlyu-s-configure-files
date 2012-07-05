#ifndef _MARSH_H
	#define _MARSH_H

	typedef struct
	{
		unsigned int len;
		unsigned int pos;
		char *data;
	} m_buffer_t;

	void m_attach(m_buffer_t *buf, char *data, unsigned int len);
	int m_size(m_buffer_t *buf);
	char *m_data(m_buffer_t *buf);
	char *m_str(m_buffer_t *buf);

	int push_int8(m_buffer_t *p, char s);
	int push_int16(m_buffer_t *p, short s);
	int push_int32(m_buffer_t *p, int s);
	int push_varstr(m_buffer_t *p, char *s);
	int push_raw(m_buffer_t *p, char *s, unsigned int len);

	int pop_int8(m_buffer_t *up, char *d);
	int pop_int16(m_buffer_t *up, short *d);
	int pop_int32(m_buffer_t *up, int *d);
	int pop_varstr(m_buffer_t *up, char *data, unsigned int data_len);
	int pop_raw(m_buffer_t *up, char *data, unsigned int len);

	int m_buffer_allocate(m_buffer_t *buf, unsigned int len);
	void m_buffer_free(m_buffer_t *buf);

	int s_push_raw(m_buffer_t *p, char *s, unsigned int len);
	int s_push_int32(m_buffer_t *p, int s);
	int s_push_str(m_buffer_t *p, char *s);
	void m_buffer_clean(m_buffer_t *buf);

#endif //_MARSH_H
