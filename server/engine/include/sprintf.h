#ifndef SPRINTF_H
	#define SPRINTF_H

	void svalue_to_string(svalue_t *, outbuffer_t *, int, int, int);
	char *string_print_formatted(char *, int, svalue_t*);
	int b_vsnprintf(char *, int, char *, int, svalue_t*);
#endif
