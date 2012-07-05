
#ifndef _Z_H_
#define _Z_H_

#include <stdlib.h>


typedef struct {
	char *buf;
	unsigned off;
	unsigned total;

	char *zbuf;
	unsigned zlen;
	unsigned zoff;
	unsigned ztotal;

	struct s_zstate *unzip;

	unsigned *eob;
	unsigned eobcnt;
	unsigned eobtotal;
} zreader_t;

void zreader_init(zreader_t *reader, int bits);
void zreader_deinit(zreader_t *reader);
void zreader_clear(zreader_t *reader);
int zreader_read(zreader_t *reader, const char *zbuf, unsigned zlen);
//int zreader_fread(zreader_t *reader, void * ptr, int sz, int nmemb);
void zreader_output(zreader_t *reader, char c);
void zreader_eob(zreader_t *reader);


typedef struct {
	char *zbuf;
	unsigned zoff;
	unsigned ztotal;
	struct s_zstate *zip;
} zwriter_t; 


void zwriter_init(zwriter_t *writer, int bits);
void zwriter_deinit(zwriter_t *writer);
void zwriter_clear(zwriter_t *writer);
int zwriter_write(zwriter_t *writer, const char *buf, unsigned len);
unsigned zwriter_fwrite(zwriter_t *writer, const void *ptr, unsigned sz, unsigned nmemb);

#endif
