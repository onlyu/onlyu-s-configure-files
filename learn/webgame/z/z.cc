#include "z.h"
#include "zopen.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


void zreader_init(zreader_t *reader, int bits)
{
	unsigned total = 1024;
	struct s_zstate *unzip = zopen("r", bits, reader);
	assert(unzip);

	reader->unzip = unzip;

	reader->off = 0;
	reader->total = total;
	reader->buf = (char*)malloc(reader->total);

	reader->zoff = 0;
	reader->zlen = 0;
	reader->ztotal = total;
	reader->zbuf = (char*)malloc(reader->ztotal);

	reader->eobcnt = 0;
	reader->eobtotal = 1;
	reader->eob = (unsigned*)calloc(reader->eobtotal, sizeof(*reader->eob));
}

void zreader_eob(zreader_t *reader)
{
	if (reader->eobtotal == reader->eobcnt) {
		reader->eobtotal *= 2;
		//fprintf(stderr, "eob total realoc:%d\n", reader->eobtotal);
		reader->eob = (unsigned *)realloc(reader->eob, reader->eobtotal * sizeof(*reader->eob));
	}
	reader->eob[reader->eobcnt++] = reader->off;
	//fprintf(stderr, "add eob cnt:%d, eob:%d\n", reader->eobcnt, reader->off);
}

void zreader_output(zreader_t *reader, char c)
{
	if (reader->off >= reader->total) {
		reader->total *= 2;
		//fprintf(stderr, "output total realoc:%d\n", reader->total);
		reader->buf = (char*)realloc(reader->buf, reader->total);
	}
	reader->buf[reader->off++] = c;
}


void zreader_deinit(zreader_t *reader)
{
	free(reader->buf);
	free(reader->eob);
	free(reader->zbuf);
	zclose(reader->unzip);
	free(reader->unzip);
}

void resetroffset(struct s_zstate *zs);

void zreader_clear(zreader_t *reader)
{
	if (reader->eobcnt > 0) {
		unsigned eob = reader->eob[reader->eobcnt - 1];
		unsigned sz = reader->off - eob;
		//fprintf(stderr, "*********zclear eob:%d, sz:%d,off:%d\n", eob, sz, reader->off);
		memmove(reader->buf, reader->buf + eob, sz);
		reader->off = sz;
		reader->eobcnt = 0;
	}
}


int zreader_read(zreader_t *reader, const char *zbuf, unsigned zlen)
{
	unsigned zsz = reader->zlen - reader->zoff;
	memmove(reader->zbuf, reader->zbuf + reader->zoff, zsz);
	if ((reader->ztotal - zsz) < zlen) {
		reader->ztotal += (zlen > reader->ztotal ? zlen : reader->ztotal);
		//fprintf(stderr, "zreader ztotal realoc:%d\n", reader->ztotal);
		reader->zbuf = (char*)realloc(reader->zbuf, reader->ztotal);
	}
	memmove(reader->zbuf + zsz, zbuf, zlen);
	reader->zoff = 0;
	reader->zlen = zsz + zlen;
	resetroffset(reader->unzip);

	while(zread(reader->unzip)) {
		zreader_eob(reader);
	}

	return reader->eobcnt;
}



void zwriter_init(zwriter_t *writer, int bits)
{
	unsigned ztotal =1024;
	struct s_zstate *zip = zopen("w", bits, writer);
	assert(zip);

	writer->zip = zip;

	writer->ztotal = ztotal;
	writer->zoff = 0;
	writer->zbuf = (char*)malloc(writer->ztotal);
}

void zwriter_deinit(zwriter_t *writer)
{
	free(writer->zbuf);
	zclose(writer->zip);
	free(writer->zip);
}

void zwriter_clear(zwriter_t *writer)
{
	writer->zoff = 0;
}

unsigned zwriter_fwrite(zwriter_t *writer, const void *ptr, unsigned sz, unsigned nmemb)
{
	size_t len = sz * nmemb;
	unsigned left = writer->ztotal - writer->zoff;
	if (left < len) {
		writer->ztotal += (len > writer->ztotal ? len : writer->ztotal);
		writer->zbuf = (char*)realloc(writer->zbuf, writer->ztotal);
	}

	memcpy(writer->zbuf + writer->zoff, ptr, len); 
	writer->zoff += len;
	//fprintf(stderr, "fwrite size:%d\n", len);

	return len;
}


int zwriter_write(zwriter_t *writer, const char *buf, unsigned len)
{
	int w = zwrite(writer->zip, buf, len);
	return w;
}




#ifdef Z_TEST
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

typedef struct {
	char *buf;
	unsigned len;
} pk_t; 

long line = 0;
long inbytes = 0;
long outbytes = 0;

pk_t *get_pk(pk_t *pk, FILE *fin)
{
	int pklen = rand()%320;
	if (pklen == 0) {
		pklen = 1;
	}

	char *buf = (char*)malloc(pklen);
	int g;

	//if ((g = read(0, buf, pklen)) != pklen) {
	if ((g = fread(buf, 1, pklen, fin)) != pklen) {
		free(buf);
		//fprintf(stderr, "try pklen:%d, but:%d\n", pklen, g);
		return NULL;
	}

	line++;
	inbytes += pklen;
	
	pk->len = pklen;
	pk->buf = buf;
	return pk;
}

int main()
{
	FILE *fin = fopen("/dev/stdin", "r");

	zwriter_t writer;
	zwriter_init(&writer, 12);
	

	zreader_t reader;
	zreader_init(&reader, 12);

	time_t tm = time(NULL);
	srand(tm);

	#define pknum 20
	pk_t pks[pknum];
	memset(pks, 0, sizeof(pks));

	int i;
	while(1) {
		//fprintf(stderr, "zreader read*********************\n");
		for (i = 0; i < pknum; ++i) {
			pk_t *pk = get_pk(&pks[i], fin);
			if (pk == NULL) 
				goto end;
			zwriter_write(&writer, pk->buf, pk->len);
			
		}
		outbytes += writer.zoff;

		
		unsigned wc = 0;
		unsigned w = 0;
		while (wc < writer.zoff) {
			w = rand()%8;
			if (w == 0)  
				w = 1;

			w = w > (writer.zoff - wc) ? (writer.zoff - wc): w;

			zreader_read(&reader, writer.zbuf + wc, w); 
			wc += w;
		}

		assert(wc == writer.zoff);
		//fprintf(stderr, "zreader read: zlen:%d\n", writer.zoff);
		//zreader_read(&reader, writer.zbuf, writer.zoff); 

		//fprintf(stderr, "pkcnt:%d, eobcnt:%d\n", pknum, reader.eobcnt);
		assert(reader.eobcnt == pknum);

		unsigned start = 0;
		for (i = 0; i < pknum; ++i) {
			//fprintf(stderr, "pklen:%d, start:%d, end:%d, len:%d\n", pks[i].len, start, reader.eob[i], reader.eob[i] - start);
			assert(pks[i].len == reader.eob[i] - start);
			assert(memcmp(pks[i].buf, reader.buf + start, pks[i].len) == 0);
			start = reader.eob[i];
		}


		zwriter_clear(&writer);
		zreader_clear(&reader);
		for (i = 0; i < pknum; ++i) {
			if (pks[i].buf != NULL) {
				free(pks[i].buf);
			}
		}
	}

	zwriter_deinit(&writer);
	zreader_deinit(&reader);

end:
	//printf("test line:%ld,in:%ld,out:%ld, rat:%f\n", line, inbytes, outbytes, (1.0*outbytes)/inbytes);
	return 0;
}

#endif

