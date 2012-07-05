/*  A Bison parser, made from make_func.y with Bison version GNU Bison version 1.24
 */

#define YYBISON 1  /* Identify Bison output.  */

#define ID	258
#define NUM	259
#define DEFAULT	260
#define OPERATOR	261

#line 1 "make_func.y"

#define EDIT_SOURCE
#define NO_OPCODES
#include "std.h"
#include "make_func.h"
#include "lex.h"
#include "preprocess.h"
#include "edit_source.h"

#ifdef WIN32
	#include <process.h>
#endif 

int num_buff = 0;
int op_code, efun_code, efun1_code;
char *oper_codes[MAX_FUNC];
char *efun_codes[MAX_FUNC],  *efun1_codes[MAX_FUNC];
char *efun_names[MAX_FUNC],  *efun1_names[MAX_FUNC];
char *key[MAX_FUNC],  *buf[MAX_FUNC];

int min_arg =  - 1, limit_max = 0;

/*
 * arg_types is the types of all arguments. A 0 is used as a delimiter,
 * marking next argument. An argument can have several types.
 */
int arg_types[400], last_current_type;

/*
 * Store the types of the current efun. They will be copied into the
 * arg_types list if they were not already there (to save memory).
 */
int curr_arg_types[40], curr_arg_type_size;

struct type
{
	char *name;
	int num;
} types[] = 
{
	{
		"void", VOID
	}

	, 
	{
		"int", INT
	}

	, 
	{
		"string", STRING
	}

	, 
	{
		"object", OBJECT
	}

	, 
	{
		"mapping", MAPPING
	}

	, 
	{
		"mixed", MIXED
	}

	, 
	{
		"unknown", UNKNOWN
	}

	, 
	{
		"float", FLOAT
	}

	, 
	{
		"function", FUNCTION
	}

	, 
	{
		"buffer", BUFFER
	}

	, 
};

#define NELEMS(arr) 	(sizeof arr / sizeof arr[0])

#line 54 "make_func.y"
typedef union
{
	int number;
	char *string;
} YYSTYPE;

#ifndef YYLTYPE
	typedef struct yyltype
	{
		int timestamp;
		int first_line;
		int first_column;
		int last_line;
		int last_column;
		char *text;
	} yyltype;

	#define YYLTYPE yyltype
#endif 

#include <stdio.h>

#ifndef __cplusplus
	#ifndef __STDC__
		#define const
	#endif 
#endif 



#define YYFINAL		40
#define YYFLAG		-32768
#define YYNTBASE	15

#define YYTRANSLATE(x) ((unsigned)(x) <= 261 ? yytranslate[x] : 29)

static const char yytranslate[] = 
{
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 10, 11, 12, 2, 8, 2, 14, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 9, 7, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 13, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 3, 4,
	5, 6
};

#if YYDEBUG != 0
	static const short yyprhs[] = 
	{
		0, 0, 1, 4, 6, 8, 12, 14, 18, 20, 22, 23, 24, 28, 32, 41, 43, 46, 48, 49, 51, 55, 57, 59, 61, 65
	};

	static const short yyrhs[] = 
	{
		 - 1, 15, 16, 0, 17, 0, 22, 0, 6, 18, 7, 0, 19, 0, 18, 8, 19, 0, 3, 0, 3, 0, 0, 0, 5, 9, 4, 0, 5, 9, 3, 0, 23, 3, 20, 10, 25, 21, 11, 7, 0, 24, 0, 24, 12, 0, 3, 0, 0, 26, 0, 25, 8, 26, 0, 28, 0, 23,
	0, 27, 0, 28, 13, 27, 0, 14, 14, 14, 0
	};

#endif 

#if YYDEBUG != 0
	static const short yyrline[] = 
	{
		0, 67, 67, 69, 69, 71, 73, 73, 75, 91, 91, 93, 94, 100, 107, 185, 185, 187, 205, 206, 207, 209, 217, 227, 228, 229
	};

	static const char *const yytname[] = 
	{
		"$", "error", "$undefined.", "ID", "NUM", "DEFAULT", "OPERATOR", "';'", "','", "':'", "'('", "')'", "'*'", "'|'", "'.'", "specs", "spec", "operator", "op_list", "op", "optional_ID", 
	"optional_default", "func", "type", "basic", "arg_list", "typel2", "arg_type", "typel", ""
	};
#endif 

static const short yyr1[] = 
{
	0, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 21, 21, 22, 23, 23, 24, 25, 25, 25, 26, 27, 28, 28, 28
};

static const short yyr2[] = 
{
	0, 0, 2, 1, 1, 3, 1, 3, 1, 1, 0, 0, 3, 3, 8, 1, 2, 1, 0, 1, 3, 1, 1, 1, 3, 3
};

static const short yydefact[] = 
{
	1, 0, 17, 0, 2, 3, 4, 0, 15, 8, 0, 6, 10, 16, 5, 0, 9, 0, 7, 18, 0, 22, 11, 19, 23, 21, 0, 0, 0, 0, 0, 25, 0, 20, 0, 24, 13, 12, 14, 0, 0
};

static const short yydefgoto[] = 
{
	1, 4, 5, 10, 11, 17, 29, 6, 21, 8, 22, 23, 24, 25
};

static const short yypact[] = 
{
	 - 32768, 1,  - 32768,  - 1,  - 32768,  - 32768,  - 32768, 5,  - 7,  - 32768, 2,  - 32768, 11,  - 32768,  - 32768,  - 1,  - 32768, 6,  - 32768,  - 3, 3,  - 32768,  - 2,  - 32768,  - 32768, 7, 4, 10,
	- 3, 12, 18,  - 32768, 9,  - 32768, 8,  - 32768,  - 32768,  - 32768,  - 32768, 22,  - 32768
};

static const short yypgoto[] = 
{
	 - 32768,  - 32768,  - 32768,  - 32768, 13,  - 32768,  - 32768,  - 32768, 23,  - 32768,  - 32768, 14,  - 5,  - 32768
};


#define YYLAST		42


static const short yytable[] = 
{
	2, 39, 9, 27, 2, 13, 28, 3, 12, 14, 15, 20, 36, 37, 16, 38, 19, 26, 31, 32, 30, 2, 40, 34, 7, 35, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33
};

static const short yycheck[] = 
{
	3, 0, 3, 5, 3, 12, 8, 6, 3, 7, 8, 14, 3, 4, 3, 7, 10, 14, 14, 9, 13, 3, 0, 11, 1, 30,  - 1,  - 1, 15,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1,  - 1, 28
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
Bison output file, you may use that output file without restriction.
This special exception was added by the Free Software Foundation
in version 1.24 of Bison.  */

#ifndef alloca
	#ifdef __GNUC__
		#define alloca __builtin_alloca
	#else /* not GNU C.  */
		#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
			#include <alloca.h>
		#else /* not sparc */
			#if defined (MSDOS) && !defined (__TURBOC__)
				#include <malloc.h>
			#else /* not MSDOS, or __TURBOC__ */
				#if defined(_AIX)
					#include <malloc.h>
					#pragma alloca
				#else /* not MSDOS, __TURBOC__, or _AIX */
					#ifdef __hpux
						#ifdef __cplusplus
							extern "C"
							{
								void *alloca(unsigned int);
							};
						#else /* not __cplusplus */
							void *alloca();
						#endif /* not __cplusplus */
					#endif /* __hpux */
				#endif /* not _AIX */
			#endif /* not MSDOS, or __TURBOC__ */
		#endif /* not sparc.  */
	#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
when the %semantic_parser declaration is not specified in the grammar.
It was written by Richard Stallman by simplifying the hairy parser
used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
It is replaced by the list of actions, each action
as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
This remains here temporarily to ease the
transition to the new meaning of YYERROR, for GCC.
Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
if (yychar == YYEMPTY && yylen == 1)				\
{ yychar = (token), yylval = (value);			\
yychar1 = YYTRANSLATE (yychar);				\
YYPOPSTACK;						\
goto yybackup;						\
}								\
else								\
{ yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
	#define YYLEX		yylex()
#endif 

#ifdef YYPURE
	#ifdef YYLSP_NEEDED
		#ifdef YYLEX_PARAM
			#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
		#else 
			#define YYLEX		yylex(&yylval, &yylloc)
		#endif 
	#else /* not YYLSP_NEEDED */
		#ifdef YYLEX_PARAM
			#define YYLEX		yylex(&yylval, YYLEX_PARAM)
		#else 
			#define YYLEX		yylex(&yylval)
		#endif 
	#endif /* not YYLSP_NEEDED */
#endif 

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

	int yychar; /*  the lookahead symbol		*/
	YYSTYPE yylval; /*  the semantic value of the		*/
	/*  lookahead symbol			*/

	#ifdef YYLSP_NEEDED
		YYLTYPE yylloc; /*  location data for the lookahead	*/
		/*  symbol				*/
	#endif 

	int yynerrs; /*  number of parse errors so far       */
#endif /* not YYPURE */

#if YYDEBUG != 0
	int yydebug; /*  nonzero means print parse trace	*/
	/* Since this is uninitialized, it does not stop multiple parsers
	from coexisting.  */
#endif 

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef YYINITDEPTH
	#define YYINITDEPTH 200
#endif 

/*  YYMAXDEPTH is the maximum size the stacks can grow to
(effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
	#undef YYMAXDEPTH
#endif 

#ifndef YYMAXDEPTH
	#define YYMAXDEPTH 10000
#endif 

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
	int yyparse(void);
#endif 

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
	#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else /* not GNU C or C++ */
	#ifndef __cplusplus

		/* This is the most reliable way to avoid incompatibilities
		in available built-in functions on various systems.  */
		static void __yy_memcpy(from, to, count)char *from;
		char *to;
		int count;
		{
			register char *f = from;
			register char *t = to;
			register int i = count;

			while (i-- > 0)
			{
				*t++ =  *f++;
			}
		}

	#else /* __cplusplus */

		/* This is the most reliable way to avoid incompatibilities
		in available built-in functions on various systems.  */
		static void __yy_memcpy(char *from, char *to, int count)
		{
			register char *f = from;
			register char *t = to;
			register int i = count;

			while (i-- > 0)
			{
				*t++ =  *f++;
			}
		}

	#endif 
#endif 

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
into yyparse.  The argument should have type void *.
It should actually point to an object.
Grammar actions can access the variable by casting it
to the proper pointer type.  */

#ifdef YYPARSE_PARAM
	#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else 
	#define YYPARSE_PARAM
	#define YYPARSE_PARAM_DECL
#endif 

int yyparse(YYPARSE_PARAM)YYPARSE_PARAM_DECL
{
	register int yystate;
	register int yyn;
	register short *yyssp;
	register YYSTYPE *yyvsp;
	int yyerrstatus; /*  number of tokens to shift before error messages enabled */
	int yychar1 = 0; /*  lookahead token as an internal (translated) token number */

	short yyssa[YYINITDEPTH]; /*  the state stack			*/
	YYSTYPE yyvsa[YYINITDEPTH]; /*  the semantic value stack		*/

	short *yyss = yyssa; /*  refer to the stacks thru separate pointers */
	YYSTYPE *yyvs = yyvsa; /*  to allow yyoverflow to reallocate them elsewhere */

	#ifdef YYLSP_NEEDED
		YYLTYPE yylsa[YYINITDEPTH]; /*  the location stack			*/
		YYLTYPE *yyls = yylsa;
		YYLTYPE *yylsp;

		#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
	#else 
		#define YYPOPSTACK   (yyvsp--, yyssp--)
	#endif 

	int yystacksize = YYINITDEPTH;

	#ifdef YYPURE
		int yychar;
		YYSTYPE yylval;
		int yynerrs;
		#ifdef YYLSP_NEEDED
			YYLTYPE yylloc;
		#endif 
	#endif 

	YYSTYPE yyval; /*  the variable used to return		*/
	/*  semantic values from the action	*/
	/*  routines				*/

	int yylen;

	#if YYDEBUG != 0
		if (yydebug)
		{
			fprintf(stderr, "Starting parse\n");
		}
	#endif 

	yystate = 0;
	yyerrstatus = 0;
	yynerrs = 0;
	yychar = YYEMPTY; /* Cause a token to be read.  */

	/* Initialize stack pointers.
	Waste one element of value and location stack
	so that they stay on the same level as the state stack.
	The wasted elements are never initialized.  */

	yyssp = yyss - 1;
	yyvsp = yyvs;
	#ifdef YYLSP_NEEDED
		yylsp = yyls;
	#endif 

	/* Push a new state, which is found in  yystate  .  */
	/* In all cases, when you get here, the value and location stacks
	have just been pushed. so pushing a state here evens the stacks.  */
	yynewstate: 

	*++yyssp = yystate;

	if (yyssp >= yyss + yystacksize - 1)
	{
		/* Give user a chance to reallocate the stack */
		/* Use copies of these so that the &'s don't force the real ones into memory. */
		YYSTYPE *yyvs1 = yyvs;
		short *yyss1 = yyss;
		#ifdef YYLSP_NEEDED
			YYLTYPE *yyls1 = yyls;
		#endif 

		/* Get the current used size of the three stacks, in elements.  */
		int size = yyssp - yyss + 1;

		#ifdef yyoverflow
			/* Each stack pointer address is followed by the size of
			the data in use in that stack, in bytes.  */
			#ifdef YYLSP_NEEDED
				/* This used to be a conditional around just the two extra args,
				but that might be undefined if yyoverflow is a macro.  */
				yyoverflow("parser stack overflow",  &yyss1, size *sizeof(*yyssp),  &yyvs1, size *sizeof(*yyvsp),  &yyls1, size *sizeof(*yylsp),  &yystacksize);
			#else 
				yyoverflow("parser stack overflow",  &yyss1, size *sizeof(*yyssp),  &yyvs1, size *sizeof(*yyvsp),  &yystacksize);
			#endif 

			yyss = yyss1;
			yyvs = yyvs1;
			#ifdef YYLSP_NEEDED
				yyls = yyls1;
			#endif 
		#else /* no yyoverflow */
			/* Extend the stack our own way.  */
			if (yystacksize >= YYMAXDEPTH)
			{
				yyerror("parser stack overflow");
				return 2;
			}
			yystacksize *= 2;
			if (yystacksize > YYMAXDEPTH)
			{
				yystacksize = YYMAXDEPTH;
			}
			yyss = (short*)alloca(yystacksize *sizeof(*yyssp) );
			__yy_memcpy( (char*)yyss1, (char*)yyss, size *sizeof(*yyssp) );
			yyvs = (YYSTYPE*)alloca(yystacksize *sizeof(*yyvsp) );
			__yy_memcpy( (char*)yyvs1, (char*)yyvs, size *sizeof(*yyvsp) );
			#ifdef YYLSP_NEEDED
				yyls = (YYLTYPE*)alloca(yystacksize *sizeof(*yylsp) );
				__yy_memcpy( (char*)yyls1, (char*)yyls, size *sizeof(*yylsp) );
			#endif 
		#endif /* no yyoverflow */

		yyssp = yyss + size - 1;
		yyvsp = yyvs + size - 1;
		#ifdef YYLSP_NEEDED
			yylsp = yyls + size - 1;
		#endif 

		#if YYDEBUG != 0
			if (yydebug)
			{
				fprintf(stderr, "Stack size increased to %d\n", yystacksize);
			}
		#endif 

		if (yyssp >= yyss + yystacksize - 1)
		{
			YYABORT;
		}
	}

	#if YYDEBUG != 0
		if (yydebug)
		{
			fprintf(stderr, "Entering state %d\n", yystate);
		}
	#endif 

	goto yybackup;
	yybackup: 

	/* Do appropriate processing given the current state.  */
	/* Read a lookahead token if we need one and don't already have one.  */
	/* yyresume: */

	/* First try to decide what to do without reference to lookahead token.  */

	yyn = yypact[yystate];
	if (yyn == YYFLAG)
	{
		goto yydefault;
	}

	/* Not known => get a lookahead token if don't already have one.  */

	/* yychar is either YYEMPTY or YYEOF
	or a valid token in external form.  */

	if (yychar == YYEMPTY)
	{
		#if YYDEBUG != 0
			if (yydebug)
			{
				fprintf(stderr, "Reading a token: ");
			}
		#endif 
		yychar = YYLEX;
	}

	/* Convert token to internal form (in yychar1) for indexing tables with */

	if (yychar <= 0)
	 /* This means end of input. */
	{
		yychar1 = 0;
		yychar = YYEOF; /* Don't call YYLEX any more */

		#if YYDEBUG != 0
			if (yydebug)
			{
				fprintf(stderr, "Now at end of input.\n");
			}
		#endif 
	}
	else
	{
		yychar1 = YYTRANSLATE(yychar);

		#if YYDEBUG != 0
			if (yydebug)
			{
				fprintf(stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
				/* Give the individual parser a way to print the precise meaning
				of a token, for further debugging info.  */
				#ifdef YYPRINT
					YYPRINT(stderr, yychar, yylval);
				#endif 
				fprintf(stderr, ")\n");
			}
		#endif 
	}

	yyn += yychar1;
	if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
	{
		goto yydefault;
	}

	yyn = yytable[yyn];

	/* yyn is what to do for this token type in this state.
	Negative => reduce, -yyn is rule number.
	Positive => shift, yyn is new state.
	New state is final state => don't bother to shift,
	just return success.
	0, or most negative number => error.  */

	if (yyn < 0)
	{
		if (yyn == YYFLAG)
		{
			goto yyerrlab;
		}
		yyn =  - yyn;
		goto yyreduce;
	}
	else if (yyn == 0)
	{
		goto yyerrlab;
	}

	if (yyn == YYFINAL)
	{
		YYACCEPT;
	}

	/* Shift the lookahead token.  */

	#if YYDEBUG != 0
		if (yydebug)
		{
			fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
		}
	#endif 

	/* Discard the token being shifted unless it is eof.  */
	if (yychar != YYEOF)
	{
		yychar = YYEMPTY;
	}

	*++yyvsp = yylval;
	#ifdef YYLSP_NEEDED
		*++yylsp = yylloc;
	#endif 

	/* count tokens shifted since error; after three, turn off error status.  */
	if (yyerrstatus)
	{
		yyerrstatus--;
	}

	yystate = yyn;
	goto yynewstate;

	/* Do the default action for the current state.  */
	yydefault: 

	yyn = yydefact[yystate];
	if (yyn == 0)
	{
		goto yyerrlab;
	}

	/* Do a reduction.  yyn is the number of a rule to reduce with.  */
	yyreduce: yylen = yyr2[yyn];
	if (yylen > 0)
	{
		yyval = yyvsp[1-yylen];
	}
	 /* implement default value of the action */

	#if YYDEBUG != 0
		if (yydebug)
		{
			int i;

			fprintf(stderr, "Reducing via rule %d (line %d), ", yyn, yyrline[yyn]);

			/* Print the symbols being reduced, and their result.  */
			for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
			{
				fprintf(stderr, "%s ", yytname[yyrhs[i]]);
			}
			fprintf(stderr, " -> %s\n", yytname[yyr1[yyn]]);
		}
	#endif 


	switch (yyn)
	{

		case 8:
			#line 76 "make_func.y"
			{
				char f_name[500], c;
				int i = 2;
				sprintf(f_name, "F_%s", yyvsp[0].string);
				while ( (c = f_name[i]) )
				{
					if (islower(c) )
					{
						f_name[i++] = toupper(c);
					}
					else
					{
						i++;
					}
				}
				oper_codes[op_code] = (char*)malloc(i + 1);
				strcpy(oper_codes[op_code], f_name);
				free(yyvsp[0].string);

				op_code++;
				;
				break;
			}
		case 10:
			#line 91 "make_func.y"
			{
				yyval.string = "";
				;
				break;
			}
		case 11:
			#line 93 "make_func.y"
			{
				yyval.string = "DEFAULT_NONE";
				;
				break;
			}
		case 12:
			#line 95 "make_func.y"
			{
				static char buf[40];
				sprintf(buf, "%i", yyvsp[0].number);
				yyval.string = buf;
				;
				break;
			}
		case 13:
			#line 101 "make_func.y"
			{
				if (strcmp(yyvsp[0].string, "F_THIS_OBJECT") )
				{
					yyerror("Illegal default");
				}
				yyval.string = "DEFAULT_THIS_OBJECT";
				;
				break;
			}
		case 14:
			#line 108 "make_func.y"
			{
				char buff[500];
				char f_name[500];
				int i, len;
				if (min_arg ==  - 1)
				{
					min_arg = yyvsp[ - 3].number;
				}
				if (min_arg > 4)
				{
					mf_fatal("min_arg > 4\n");
				}
				if (yyvsp[ - 5].string[0] == '\0')
				{
					if (strlen(yyvsp[ - 6].string) + 1+2 > sizeof f_name)
					{
						mf_fatal("A local buffer was too small!(1)\n");
					}
					sprintf(f_name, "F_%s", yyvsp[ - 6].string);
					len = strlen(f_name);
					for (i = 0; i < len; i++)
					{
						if (islower(f_name[i]) )
						{
							f_name[i] = toupper(f_name[i]);
						}
					}
					if (min_arg == 1 && !limit_max && yyvsp[ - 3].number == 1)
					{
						efun1_codes[efun1_code] = (char*)malloc(len + 1);
						strcpy(efun1_codes[efun1_code], f_name);
						efun1_names[efun1_code] = (char*)malloc(len - 1);
						strcpy(efun1_names[efun1_code], yyvsp[ - 6].string);
						efun1_code++;
					}
					else
					{
						efun_codes[efun_code] = (char*)malloc(len + 1);
						strcpy(efun_codes[efun_code], f_name);
						efun_names[efun_code] = (char*)malloc(len - 1);
						strcpy(efun_names[efun_code], yyvsp[ - 6].string);
						efun_code++;
					}
				}
				else
				{
					if (strlen(yyvsp[ - 5].string) + 1+17 > sizeof f_name)
					{
						mf_fatal("A local buffer was too small(2)!\n");
					}
					sprintf(f_name, "F_%s | F_ALIAS_FLAG", yyvsp[ - 5].string);
					len = strlen(f_name);
					for (i = 0; i < len; i++)
					{
						if (islower(f_name[i]) )
						{
							f_name[i] = toupper(f_name[i]);
						}
					}
					free(yyvsp[ - 5].string);
				}
				for (i = 0; i < last_current_type; i++)
				{
					int j;
					for (j = 0; j + i < last_current_type && j < curr_arg_type_size; j++)
					{
						if (curr_arg_types[j] != arg_types[i + j])
						{
							break;
						}
					}
					if (j == curr_arg_type_size)
					{
						break;
					}
				}
				if (i == last_current_type)
				{
					int j;
					for (j = 0; j < curr_arg_type_size; j++)
					{
						arg_types[last_current_type++] = curr_arg_types[j];
						if (last_current_type == NELEMS(arg_types) )
						{
							yyerror("Array 'arg_types' is too small");
						}
					}
				}
				if (!strcmp(yyvsp[ - 6].string, "call_other") && !lookup_define("CAST_CALL_OTHERS") )
				{
					yyvsp[ - 7].number = MIXED;
				}
				sprintf(buff, "{\"%s\",%s,0,0,%d,%d,%s,%s,%s,%s,%s,%d,%s},\n", yyvsp[ - 6].string, f_name, min_arg, limit_max ?  - 1: yyvsp[ - 3].number, yyvsp[ - 7].number != VOID ? ctype(yyvsp[ - 7].number): 
			"TYPE_NOVALUE", etype(0), etype(1), etype(2), etype(3), i, yyvsp[ - 2].string);
				if (strlen(buff) > sizeof buff)
				{
					mf_fatal("Local buffer overwritten !\n");
				}

				key[num_buff] = yyvsp[ - 6].string;
				buf[num_buff] = (char*)malloc(strlen(buff) + 1);
				strcpy(buf[num_buff], buff);
				num_buff++;
				min_arg =  - 1;
				limit_max = 0;
				curr_arg_type_size = 0;
				;
				break;
			}
		case 16:
			#line 185 "make_func.y"
			{
				yyval.number = yyvsp[ - 1].number | 0x10000;
				;
				break;
			}
		case 17:
			#line 188 "make_func.y"
			{
				int i = NELEMS(types);
				yyval.number = 0;
				while (i--)
				{
					if (strcmp(yyvsp[0].string, types[i].name) == 0)
					{
						yyval.number = types[i].num;
						break;
					}
				}
				if (!yyval.number)
				{
					char buf[256];
					sprintf(buf, "Invalid type: %s", yyvsp[0].string);
					yyerror(buf);
				}
				free(yyvsp[0].string);
				;
				break;
			}
		case 18:
			#line 205 "make_func.y"
			{
				yyval.number = 0;
				;
				break;
			}
		case 19:
			#line 206 "make_func.y"
			{
				yyval.number = 1;
				if (yyvsp[0].number)
				{
					min_arg = 0;
				}
				;
				break;
			}
		case 20:
			#line 207 "make_func.y"
			{
				yyval.number = yyvsp[ - 2].number + 1;
				if (yyvsp[0].number)
				{
					min_arg = yyval.number - 1;
				}
				;
				break;
			}
		case 21:
			#line 210 "make_func.y"
			{
				yyval.number = yyvsp[0].number;
				curr_arg_types[curr_arg_type_size++] = 0;
				if (curr_arg_type_size == NELEMS(curr_arg_types) )
				{
					yyerror("Too many arguments");
				}
				;
				break;
			}
		case 22:
			#line 218 "make_func.y"
			{
				if (yyvsp[0].number != VOID)
				{
					curr_arg_types[curr_arg_type_size++] = yyvsp[0].number;
					if (curr_arg_type_size == NELEMS(curr_arg_types) )
					{
						yyerror("Too many arguments");
					}
				}
				yyval.number = yyvsp[0].number;
				;
				break;
			}
		case 23:
			#line 227 "make_func.y"
			{
				yyval.number = (yyvsp[0].number == VOID && min_arg ==  - 1);
				;
				break;
			}
		case 24:
			#line 228 "make_func.y"
			{
				yyval.number = (min_arg ==  - 1 && (yyvsp[ - 2].number || yyvsp[0].number == VOID) );
				;
				break;
			}
		case 25:
			#line 229 "make_func.y"
			{
				yyval.number = min_arg ==  - 1;
				limit_max = 1;
				;
				break;
			}
	}
	/* the action file gets copied in in place of this dollarsign */
	#line 487 "bison.simple"

	yyvsp -= yylen;
	yyssp -= yylen;
	#ifdef YYLSP_NEEDED
		yylsp -= yylen;
	#endif 

	#if YYDEBUG != 0
		if (yydebug)
		{
			short *ssp1 = yyss - 1;
			fprintf(stderr, "state stack now");
			while (ssp1 != yyssp)
			{
				fprintf(stderr, " %d",  *++ssp1);
			}
			fprintf(stderr, "\n");
		}
	#endif 

	*++yyvsp = yyval;

	#ifdef YYLSP_NEEDED
		yylsp++;
		if (yylen == 0)
		{
			yylsp->first_line = yylloc.first_line;
			yylsp->first_column = yylloc.first_column;
			yylsp->last_line = (yylsp - 1)->last_line;
			yylsp->last_column = (yylsp - 1)->last_column;
			yylsp->text = 0;
		}
		else
		{
			yylsp->last_line = (yylsp + yylen - 1)->last_line;
			yylsp->last_column = (yylsp + yylen - 1)->last_column;
		}
	#endif 

	/* Now "shift" the result of the reduction.
	Determine what state that goes to,
	based on the state we popped back to
	and the rule number reduced by.  */

	yyn = yyr1[yyn];

	yystate = yypgoto[yyn - YYNTBASE] +  *yyssp;
	if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] ==  *yyssp)
	{
		yystate = yytable[yystate];
	}
	else
	{
		yystate = yydefgoto[yyn - YYNTBASE];
	}

	goto yynewstate;

	yyerrlab:  /* here on detecting error */

	if (!yyerrstatus)
	/* If not already recovering from an error, report this error.  */
	{
		++yynerrs;

		#ifdef YYERROR_VERBOSE
			yyn = yypact[yystate];

			if (yyn > YYFLAG && yyn < YYLAST)
			{
				int size = 0;
				char *msg;
				int x, count;

				count = 0;
				/* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
				for (x = (yyn < 0 ?  - yyn: 0); x < (sizeof(yytname) / sizeof(char*) ); x++)
					if (yycheck[x + yyn] == x)
				    {
						size += strlen(yytname[x]) + 15, count++;
				    }
				msg = (char*)malloc(size + 15);
				if (msg != 0)
				{
					strcpy(msg, "parse error");

					if (count < 5)
					{
						count = 0;
						for (x = (yyn < 0 ?  - yyn: 0); x < (sizeof(yytname) / sizeof(char*) ); x++)
						if (yycheck[x + yyn] == x)
						{
							strcat(msg, count == 0 ? ", expecting `" : " or `");
							strcat(msg, yytname[x]);
							strcat(msg, "'");
							count++;
						}
					}
					yyerror(msg);
					free(msg);
				}
				else
				{
					yyerror("parse error; also virtual memory exceeded");
				}
			}
			else
		#endif /* YYERROR_VERBOSE */
		{
			yyerror("parse error");
		}
	}

	goto yyerrlab1;
	yyerrlab1:  /* here on error raised explicitly by an action */

	if (yyerrstatus == 3)
	{
		/* if just tried and failed to reuse lookahead token after an error, discard it.  */

		/* return failure if at end of input */
		if (yychar == YYEOF)
		{
			YYABORT;
		}

		#if YYDEBUG != 0
			if (yydebug)
			{
				fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
			}
		#endif 

		yychar = YYEMPTY;
	}

	/* Else will try to reuse lookahead token
	after shifting the error token.  */

	yyerrstatus = 3; /* Each real token shifted decrements this */

	goto yyerrhandle;

	yyerrdefault:  /* current state does not do anything special for the error token. */

	#if 0
		/* This is wrong; only states that explicitly want error tokens
		should shift them.  */
		yyn = yydefact[yystate]; /* If its default is to accept any token, ok.  Otherwise pop it.*/
		if (yyn)
		{
			goto yydefault;
		}
	#endif 

	yyerrpop:  /* pop the current state because it cannot handle the error token */

	if (yyssp == yyss)
	{
		YYABORT;
	}
	yyvsp--;
	yystate =  *--yyssp;
	#ifdef YYLSP_NEEDED
		yylsp--;
	#endif 

	#if YYDEBUG != 0
		if (yydebug)
		{
			short *ssp1 = yyss - 1;
			fprintf(stderr, "Error: state stack now");
			while (ssp1 != yyssp)
			{
				fprintf(stderr, " %d",  *++ssp1);
			}
			fprintf(stderr, "\n");
		}
	#endif 

	yyerrhandle: 

	yyn = yypact[yystate];
	if (yyn == YYFLAG)
	{
		goto yyerrdefault;
	}

	yyn += YYTERROR;
	if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
	{
		goto yyerrdefault;
	}

	yyn = yytable[yyn];
	if (yyn < 0)
	{
		if (yyn == YYFLAG)
		{
			goto yyerrpop;
		}
		yyn =  - yyn;
		goto yyreduce;
	}
	else if (yyn == 0)
	{
		goto yyerrpop;
	}

	if (yyn == YYFINAL)
	{
		YYACCEPT;
	}

	#if YYDEBUG != 0
		if (yydebug)
		{
			fprintf(stderr, "Shifting error token, ");
		}
	#endif 

	*++yyvsp = yylval;
	#ifdef YYLSP_NEEDED
		*++yylsp = yylloc;
	#endif 

	yystate = yyn;
	goto yynewstate;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#line 231 "make_func.y"


char *ctype(int n)
{
	static char buff[100]; /* 100 is such a comfortable size :-) */
	char *p = (char*)NULL;

	if (n &0x10000)
	{
		strcpy(buff, "TYPE_MOD_ARRAY|");
	}
	else
	{
		buff[0] = '\0';
	}
	n &= ~0x10000;
	switch (n)
	{
		case FLOAT:
			p = "TYPE_REAL";
			break;
		case FUNCTION:
			p = "TYPE_FUNCTION";
			break;
		case VOID:
			p = "TYPE_VOID";
			break;
		case STRING:
			p = "TYPE_STRING";
			break;
		case INT:
			p = "TYPE_NUMBER";
			break;
		case OBJECT:
			p = "TYPE_OBJECT";
			break;
		case MAPPING:
			p = "TYPE_MAPPING";
			break;
		case BUFFER:
			p = "TYPE_BUFFER";
			break;
		case MIXED:
			p = "TYPE_ANY";
			break;
		case UNKNOWN:
			p = "TYPE_UNKNOWN";
			break;
		default:
			yyerror("Bad type!");
	}
	strcat(buff, p);
	if (strlen(buff) + 1 > sizeof buff)
	{
		mf_fatal("Local buffer overwritten in ctype()");
	}
	return buff;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

char *etype1(int n)
{
	if (n &0x10000)
	{
		return "T_ARRAY";
	}
	switch (n)
	{
		case FLOAT:
			return "T_REAL";
		case FUNCTION:
			return "T_FUNCTION";
		case INT:
			return "T_NUMBER";
		case OBJECT:
			return "T_OBJECT";
		case MAPPING:
			return "T_MAPPING";
		case STRING:
			return "T_STRING";
		case BUFFER:
			return "T_BUFFER";
		case MIXED:
			return "T_ANY"; /* 0 means any type */
		default:
			yyerror("Illegal type for argument");
	}
	return "What?";
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

char *etype(int n)
{
	int i;
	int local_size = 100;
	char *buff = (char*)malloc(local_size);

	for (i = 0; i < curr_arg_type_size; i++)
	{
		if (n == 0)
		{
			break;
		}
		if (curr_arg_types[i] == 0)
		{
			n--;
		}
	}
	if (i == curr_arg_type_size)
	{
		return "T_ANY";
	}
	buff[0] = '\0';
	for (; curr_arg_types[i] != 0; i++)
	{
		char *p;
		if (curr_arg_types[i] == VOID)
		{
			continue;
		}
		if (buff[0] != '\0')
		{
			strcat(buff, "|");
		}
		p = etype1(curr_arg_types[i]);
		/*
		 * The number 2 below is to include the zero-byte and the next
		 * '|' (which may not come).
		 */
		if (strlen(p) + strlen(buff) + 2 > local_size)
		{
			fprintf(stderr, "Buffer overflow!\n");
			exit(1);
		}
		strcat(buff, etype1(curr_arg_types[i]) );
	}
	if (!strcmp(buff, "") )
	{
		strcpy(buff, "T_ANY");
	}
	return buff;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int ident(int);

int yylex()
{
	register int c;

	for (;;)
	{
		switch (c = getc(yyin) )
		{
			case ' ':
			case '\t':
				continue;
			case '\n':
				current_line++;
				continue;
			case '!':
				{
					char buff[2048];
					fgets(buff, 2047, yyin);
					fprintf(stderr, "Configuration problem: %s\n", buff);
					exit( - 1);
				}
			case '#':
				{
					int line;

					char aBuf[2048], fname[2048];
					fgets(aBuf, 2047, yyin);
					if (sscanf(aBuf, "%d \"%[^\"]\"", &line, fname) )
					{
						current_line = line;
						if (current_file)
						{
							free(current_file);
						}
						current_file = (char*)malloc(strlen(fname) + 1);
						strcpy(current_file, fname);
					}
					else
						if (sscanf(aBuf, "%d", &line) )
					    {
							current_line = line;
					    }
					current_line++;
					continue;
				}
			case EOF:
				fclose(yyin);
				return  - 1;
			default:
				if ( (c >= '0' && c <= '9') || c == '-')
				{
					int v;
					int neg = 0;
					v = 0;
					if (c == '-')
					{
						neg = 1;
						c = '0';
					}
					do
					{
						v = v * 10+(c - '0');
						c = getc(yyin);
					}
					while (c >= '0' && c <= '9');
					ungetc(c, yyin);
					yylval.number = (neg ?  - v: v);
					return NUM;
				}
				if (isalunum(c) )
				{
					return ident(c);
				}
				return c;
		}
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int ident(int c)
{
	char buff[100];
	int len;

	for (len = 0; isalunum(c); c = getc(yyin) )
	{
		buff[len++] = c;
		if (len == sizeof buff - 1)
		{
			yyerror("Too long indentifier");
			break;
		}
	}
	(void)ungetc(c, yyin);
	buff[len] = '\0';
	if (!strcmp(buff, "default") )
	{
		return DEFAULT;
	}
	if (!strcmp(buff, "operator") )
	{
		return OPERATOR;
	}

	yylval.string = (char*)malloc(strlen(buff) + 1);
	strcpy(yylval.string, buff);
	return ID;
}
