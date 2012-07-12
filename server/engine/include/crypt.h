#ifndef CRYPT_H
	#define CRYPT_H

	/* Number of bytes in salt (raw, fully used, not printable-encoded bytes). 
	 * This is configurable, but don't set it too high (up to say 10). But 4
	 * is plenty really, 2^32 salts possible (the DES-style crypt used only 4096).
	 */
	#define MD5_SALTLEN  4

	/* Magic string, used in hashing. Configurable. */
	#define MD5_MAGIC "fsxy"

	/* Maximum length (in bytes) of digested data. 
	 * This is /not/ configurable! Don't change it.
	 */
	#define MD5_MAXLEN  55

	/* This salt value is used in generating salts. It must
	 * be a valid salt as returned by md5crypt(), or the
	 * routine will go into an infinite loop when generating
	 * a salt from an unrecognized string. Any value returned
	 * from md5crypt("anything", NULL, NULL) is valid here.
	 */
	#define MD5_VALID_SALT "canoelck!ggddl`l`lg`bjblodlfcljdcnhffib`c"

	typedef unsigned char byte;

	char *custom_crypt(CONST char *key, CONST char *salt, byte *rawout);

	/* the length for the md5 digest */
	#define MD5_LEN		16

	/* the max size for the md5 input buffer */
	#define MAX_SIZE	256
	unsigned char *md5_digest(unsigned char *buf, const char *input, int size);

	void hexencode(const unsigned char *src, int len, char *dest);
	int hexdecode(const char *src, unsigned char *dest, int size);
#endif
