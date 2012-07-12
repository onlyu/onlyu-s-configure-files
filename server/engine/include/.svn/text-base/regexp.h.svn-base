#ifndef _REGEXP_H_
	#define _REGEXP_H_



	#define NSUBEXP  10
	typedef struct regexp
	{
		char *startp[NSUBEXP];
		char *endp[NSUBEXP];
		char regstart; /* Internal use only. */
		char reganch; /* Internal use only. */
		char *regmust; /* Internal use only. */
		int regmlen; /* Internal use only. */
		char program[1]; /* Unwarranted chumminess with compiler. */
	} regexp;

	extern int regnarrate;
	extern int regexp_user;
	extern char *regexp_error;

	void regdump(regexp*);
	//Update By Prophet For Insert POSIX STD REGEXP
	regexp *_regcomp(unsigned char *, int);
	int _regexec(regexp *, char*);
	char *regsub(regexp *, char *, char *, int);

#endif
