
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */



#include "std.h"
#include "compiler.h"
#include "lex.h"
#include "scratchpad.h"

#include "lpc_incl.h"
#include "simul_efun.h"
#include "generate.h"

#line 13 "grammar/grammar.y.pre"
/*
 * This is the grammar definition of LPC, and its parse tree generator.
 */

/* down to one global :)
   bits:
      SWITCH_CONTEXT     - we're inside a switch
      LOOP_CONTEXT       - we're inside a loop
      SWITCH_STRINGS     - a string case has been found
      SWITCH_NUMBERS     - a non-zero numeric case has been found
      SWITCH_RANGES      - a range has been found
      SWITCH_DEFAULT     - a default has been found
 */
int context;

/*
 * bison & yacc don't prototype this in y.tab.h
 */
int yyparse (void);




/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     L_STRING = 258,
     L_NUMBER = 259,
     L_REAL = 260,
     L_BASIC_TYPE = 261,
     L_TYPE_MODIFIER = 262,
     L_DEFINED_NAME = 263,
     L_IDENTIFIER = 264,
     L_EFUN = 265,
     L_INC = 266,
     L_DEC = 267,
     L_ASSIGN = 268,
     L_LAND = 269,
     L_LOR = 270,
     L_LSH = 271,
     L_RSH = 272,
     L_ORDER = 273,
     L_NOT = 274,
     L_IF = 275,
     L_ELSE = 276,
     L_SWITCH = 277,
     L_CASE = 278,
     L_DEFAULT = 279,
     L_RANGE = 280,
     L_DOT_DOT_DOT = 281,
     L_WHILE = 282,
     L_DO = 283,
     L_FOR = 284,
     L_FOREACH = 285,
     L_IN = 286,
     L_BREAK = 287,
     L_CONTINUE = 288,
     L_RETURN = 289,
     L_ARROW = 290,
     L_INHERIT = 291,
     L_COLON_COLON = 292,
     L_ARRAY_OPEN = 293,
     L_MAPPING_OPEN = 294,
     L_FUNCTION_OPEN = 295,
     L_NEW_FUNCTION_OPEN = 296,
     L_SSCANF = 297,
     L_CATCH = 298,
     L_TIME_EXPRESSION = 299,
     L_CLASS = 300,
     L_NEW = 301,
     L_PARAMETER = 302,
     LOWER_THAN_ELSE = 303,
     L_NE = 304,
     L_EQ = 305
   };
#endif
/* Tokens.  */
#define L_STRING 258
#define L_NUMBER 259
#define L_REAL 260
#define L_BASIC_TYPE 261
#define L_TYPE_MODIFIER 262
#define L_DEFINED_NAME 263
#define L_IDENTIFIER 264
#define L_EFUN 265
#define L_INC 266
#define L_DEC 267
#define L_ASSIGN 268
#define L_LAND 269
#define L_LOR 270
#define L_LSH 271
#define L_RSH 272
#define L_ORDER 273
#define L_NOT 274
#define L_IF 275
#define L_ELSE 276
#define L_SWITCH 277
#define L_CASE 278
#define L_DEFAULT 279
#define L_RANGE 280
#define L_DOT_DOT_DOT 281
#define L_WHILE 282
#define L_DO 283
#define L_FOR 284
#define L_FOREACH 285
#define L_IN 286
#define L_BREAK 287
#define L_CONTINUE 288
#define L_RETURN 289
#define L_ARROW 290
#define L_INHERIT 291
#define L_COLON_COLON 292
#define L_ARRAY_OPEN 293
#define L_MAPPING_OPEN 294
#define L_FUNCTION_OPEN 295
#define L_NEW_FUNCTION_OPEN 296
#define L_SSCANF 297
#define L_CATCH 298
#define L_TIME_EXPRESSION 299
#define L_CLASS 300
#define L_NEW 301
#define L_PARAMETER 302
#define LOWER_THAN_ELSE 303
#define L_NE 304
#define L_EQ 305




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    POINTER_INT pointer_int;
    int number;
    float real;
    char *string;
    int type;
    struct { short num_arg; char flags; } argument;
    ident_hash_elem_t *ihe;
    parse_node_t *node;
    function_context_t *contextp;
    struct {
	parse_node_t *node;
        char num;
    } decl; /* 5 */
    struct {
	char num_local;
	char max_num_locals;
	short context;
	short save_current_type;
	short save_exact_types;
    } func_block; /* 8 */



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1705

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  238
/* YYNRULES -- Number of states.  */
#define YYNSTATES  455

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   305

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    69,    59,    52,     2,
      63,    64,    58,    56,    66,    57,     2,    60,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    65,    62,
      55,     2,     2,    49,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    70,     2,    71,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,    50,    68,    61,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    53,    54
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    11,    13,    18,    20,
      22,    23,    25,    27,    29,    31,    33,    35,    36,    37,
      47,    51,    53,    55,    57,    60,    63,    65,    69,    70,
      75,    76,    84,    86,    88,    90,    93,    95,    98,   102,
     104,   105,   107,   110,   112,   116,   117,   120,   123,   128,
     130,   131,   133,   137,   140,   145,   146,   151,   153,   155,
     157,   158,   159,   165,   168,   173,   177,   181,   183,   187,
     188,   191,   194,   197,   199,   201,   203,   205,   207,   209,
     211,   214,   217,   218,   223,   224,   231,   232,   241,   242,
     253,   255,   257,   259,   261,   265,   266,   275,   276,   278,
     280,   282,   283,   294,   297,   300,   301,   305,   311,   314,
     316,   318,   322,   326,   330,   334,   338,   342,   346,   350,
     354,   358,   362,   366,   370,   374,   378,   380,   383,   386,
     389,   391,   395,   399,   403,   409,   413,   417,   421,   425,
     429,   433,   437,   441,   445,   449,   453,   457,   461,   465,
     469,   473,   476,   479,   482,   485,   488,   491,   494,   497,
     499,   501,   503,   505,   507,   510,   514,   515,   517,   520,
     522,   525,   527,   531,   532,   534,   537,   539,   543,   547,
     549,   551,   553,   555,   557,   558,   564,   568,   575,   583,
     592,   600,   606,   613,   619,   624,   626,   630,   632,   633,
     640,   644,   650,   655,   660,   665,   667,   671,   672,   676,
     684,   685,   689,   690,   694,   696,   698,   702,   706,   708,
     711,   715,   716,   720,   725,   730,   737,   742,   747,   754,
     762,   766,   770,   772,   775,   779,   783,   790,   791
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      73,     0,    -1,    74,    -1,    74,    82,    75,    -1,    -1,
      -1,    62,    -1,    97,    36,   154,    62,    -1,     5,    -1,
       4,    -1,    -1,    58,    -1,   103,    -1,    62,    -1,     1,
      -1,     8,    -1,     9,    -1,    -1,    -1,    98,    79,    81,
      83,    63,    95,    64,    84,    80,    -1,    98,   101,    62,
      -1,    76,    -1,    89,    -1,    85,    -1,    97,    65,    -1,
      79,    81,    -1,    86,    -1,    86,    66,    87,    -1,    -1,
      88,    98,    87,    62,    -1,    -1,    97,    45,    81,    67,
      90,    88,    68,    -1,     9,    -1,     8,    -1,     6,    -1,
      45,     8,    -1,    92,    -1,    93,    79,    -1,    93,    79,
      91,    -1,    91,    -1,    -1,    96,    -1,    96,    26,    -1,
      94,    -1,    96,    66,    94,    -1,    -1,     7,    97,    -1,
      97,   100,    -1,    63,    93,    79,    64,    -1,    93,    -1,
      -1,   102,    -1,   102,    66,   101,    -1,    79,    81,    -1,
      79,    81,    13,   134,    -1,    -1,    67,   104,   112,    68,
      -1,   103,    -1,   119,    -1,   123,    -1,    -1,    -1,   106,
      93,   107,   111,    62,    -1,    79,    91,    -1,    79,    91,
      13,   134,    -1,    93,    79,    91,    -1,   109,    13,   134,
      -1,   108,    -1,   108,    66,   111,    -1,    -1,   113,   112,
      -1,     1,    62,    -1,   133,    62,    -1,   161,    -1,   115,
      -1,   117,    -1,   127,    -1,   135,    -1,   105,    -1,    62,
      -1,    32,    62,    -1,    33,    62,    -1,    -1,    93,   114,
     111,    62,    -1,    -1,    27,    63,   133,    64,   116,   113,
      -1,    -1,    28,   118,   113,    27,    63,   133,    64,    62,
      -1,    -1,    29,    63,   126,    62,   125,    62,   125,    64,
     120,   113,    -1,     8,    -1,   109,    -1,     9,    -1,   121,
      -1,   121,    66,   121,    -1,    -1,    30,    63,   122,    31,
     134,    64,   124,   113,    -1,    -1,   133,    -1,   125,    -1,
     110,    -1,    -1,    22,    63,   133,    64,   128,    67,   106,
     130,   129,    68,    -1,   130,   129,    -1,   113,   129,    -1,
      -1,    23,   131,    65,    -1,    23,   131,    25,   131,    65,
      -1,    24,    65,    -1,   132,    -1,   154,    -1,   132,    50,
     132,    -1,   132,    51,   132,    -1,   132,    52,   132,    -1,
     132,    54,   132,    -1,   132,    53,   132,    -1,   132,    18,
     132,    -1,   132,    55,   132,    -1,   132,    16,   132,    -1,
     132,    17,   132,    -1,   132,    56,   132,    -1,   132,    57,
     132,    -1,   132,    58,   132,    -1,   132,    59,   132,    -1,
     132,    60,   132,    -1,    63,   132,    64,    -1,     4,    -1,
      57,     4,    -1,    19,     4,    -1,    61,     4,    -1,   134,
      -1,   133,    66,   134,    -1,   142,    13,   134,    -1,     1,
      13,   134,    -1,   134,    49,   134,    65,   134,    -1,   134,
      15,   134,    -1,   134,    14,   134,    -1,   134,    50,   134,
      -1,   134,    51,   134,    -1,   134,    52,   134,    -1,   134,
      54,   134,    -1,   134,    53,   134,    -1,   134,    18,   134,
      -1,   134,    55,   134,    -1,   134,    16,   134,    -1,   134,
      17,   134,    -1,   134,    56,   134,    -1,   134,    57,   134,
      -1,   134,    58,   134,    -1,   134,    59,   134,    -1,   134,
      60,   134,    -1,    99,   134,    -1,    11,   142,    -1,    12,
     142,    -1,    19,   134,    -1,    61,   134,    -1,    57,   134,
      -1,   142,    11,    -1,   142,    12,    -1,   143,    -1,   149,
      -1,   150,    -1,    78,    -1,    77,    -1,    34,    62,    -1,
      34,   133,    62,    -1,    -1,   138,    -1,   138,    66,    -1,
     134,    -1,   134,    26,    -1,   137,    -1,   138,    66,   137,
      -1,    -1,   140,    -1,   140,    66,    -1,   141,    -1,   140,
      66,   141,    -1,   134,    65,   134,    -1,   143,    -1,   158,
      -1,     8,    -1,     9,    -1,    47,    -1,    -1,    69,    63,
     144,   133,    64,    -1,   143,    35,    81,    -1,   143,    70,
     133,    25,   133,    71,    -1,   143,    70,    55,   133,    25,
     133,    71,    -1,   143,    70,    55,   133,    25,    55,   133,
      71,    -1,   143,    70,   133,    25,    55,   133,    71,    -1,
     143,    70,   133,    25,    71,    -1,   143,    70,    55,   133,
      25,    71,    -1,   143,    70,    55,   133,    71,    -1,   143,
      70,   133,    71,    -1,   153,    -1,    63,   133,    64,    -1,
     147,    -1,    -1,     6,   145,    63,    95,    64,   103,    -1,
      41,    65,    64,    -1,    41,    66,   138,    65,    64,    -1,
      40,   133,    65,    64,    -1,    39,   139,    71,    64,    -1,
      38,   136,    68,    64,    -1,   103,    -1,    63,   133,    64,
      -1,    -1,    43,   148,   146,    -1,    42,    63,   134,    66,
     134,   152,    64,    -1,    -1,    44,   151,   146,    -1,    -1,
      66,   142,   152,    -1,   155,    -1,   155,    -1,    63,   154,
      64,    -1,   154,    56,   154,    -1,     3,    -1,   155,     3,
      -1,    81,    65,   134,    -1,    -1,   157,    66,   156,    -1,
     159,    63,   136,    64,    -1,    46,    63,   136,    64,    -1,
      46,    63,    45,     8,   157,    64,    -1,     8,    63,   136,
      64,    -1,   160,    63,   136,    64,    -1,   143,    35,    81,
      63,   136,    64,    -1,    63,    58,   133,    64,    63,   136,
      64,    -1,    10,    37,    81,    -1,    10,    37,    46,    -1,
       9,    -1,    37,    81,    -1,     6,    37,    81,    -1,    81,
      37,    81,    -1,    20,    63,   133,    64,   113,   162,    -1,
      -1,    21,   113,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   199,   199,   203,   204,   207,   209,   218,   261,   269,
     278,   282,   290,   298,   303,   311,   316,   321,   339,   320,
     392,   399,   400,   401,   404,   412,   423,   424,   427,   429,
     434,   433,   481,   482,   500,   501,   520,   524,   532,   541,
     555,   560,   561,   575,   581,   592,   596,   604,   613,   621,
     623,   630,   631,   635,   643,   684,   683,   709,   709,   709,
     713,   719,   718,   740,   747,   776,   788,   815,   821,   833,
     837,   844,   852,   867,   868,   869,   870,   871,   872,   878,
     883,   906,   917,   916,   936,   935,   951,   950,   966,   965,
     991,  1012,  1018,  1036,  1042,  1052,  1051,  1073,  1077,  1081,
    1087,  1097,  1096,  1130,  1137,  1145,  1153,  1161,  1176,  1193,
    1204,  1221,  1226,  1231,  1236,  1241,  1246,  1255,  1260,  1265,
    1270,  1275,  1280,  1285,  1290,  1295,  1300,  1305,  1310,  1315,
    1323,  1328,  1336,  1373,  1379,  1404,  1411,  1418,  1425,  1430,
    1454,  1477,  1492,  1537,  1575,  1580,  1585,  1737,  1832,  1913,
    1918,  2014,  2036,  2058,  2081,  2091,  2103,  2128,  2151,  2173,
    2174,  2175,  2176,  2177,  2181,  2188,  2210,  2214,  2219,  2227,
    2232,  2240,  2247,  2261,  2266,  2271,  2279,  2290,  2309,  2317,
    2427,  2428,  2470,  2487,  2493,  2492,  2524,  2538,  2546,  2551,
    2559,  2567,  2572,  2577,  2621,  2675,  2676,  2681,  2683,  2682,
    2744,  2778,  2868,  2887,  2892,  2900,  2905,  2914,  2913,  2929,
    2940,  2939,  2955,  2961,  2976,  2985,  2986,  2991,  2999,  3000,
    3007,  3019,  3023,  3033,  3038,  3046,  3082,  3155,  3218,  3245,
    3267,  3295,  3316,  3317,  3332,  3348,  3364,  3399,  3404
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "L_STRING", "L_NUMBER", "L_REAL",
  "L_BASIC_TYPE", "L_TYPE_MODIFIER", "L_DEFINED_NAME", "L_IDENTIFIER",
  "L_EFUN", "L_INC", "L_DEC", "L_ASSIGN", "L_LAND", "L_LOR", "L_LSH",
  "L_RSH", "L_ORDER", "L_NOT", "L_IF", "L_ELSE", "L_SWITCH", "L_CASE",
  "L_DEFAULT", "L_RANGE", "L_DOT_DOT_DOT", "L_WHILE", "L_DO", "L_FOR",
  "L_FOREACH", "L_IN", "L_BREAK", "L_CONTINUE", "L_RETURN", "L_ARROW",
  "L_INHERIT", "L_COLON_COLON", "L_ARRAY_OPEN", "L_MAPPING_OPEN",
  "L_FUNCTION_OPEN", "L_NEW_FUNCTION_OPEN", "L_SSCANF", "L_CATCH",
  "L_TIME_EXPRESSION", "L_CLASS", "L_NEW", "L_PARAMETER",
  "LOWER_THAN_ELSE", "'?'", "'|'", "'^'", "'&'", "L_NE", "L_EQ", "'<'",
  "'+'", "'-'", "'*'", "'%'", "'/'", "'~'", "';'", "'('", "')'", "':'",
  "','", "'{'", "'}'", "'$'", "'['", "']'", "$accept", "all", "program",
  "possible_semi_colon", "inheritance", "real", "number", "optional_star",
  "block_or_semi", "identifier", "def", "$@1", "@2", "modifier_change",
  "member_name", "member_name_list", "member_list", "type_decl", "@3",
  "new_local_name", "atomic_type", "basic_type", "new_arg", "argument",
  "argument_list", "type_modifier_list", "type", "cast", "opt_basic_type",
  "name_list", "new_name", "block", "$@4", "decl_block",
  "local_declarations", "$@5", "new_local_def", "single_new_local_def",
  "single_new_local_def_with_init", "local_name_list", "statements",
  "statement", "$@6", "while", "$@7", "do", "$@8", "for", "$@9",
  "foreach_var", "foreach_vars", "foreach", "$@10", "for_expr",
  "first_for_expr", "switch", "$@11", "switch_block", "case", "case_label",
  "constant", "comma_expr", "expr0", "return", "expr_list",
  "expr_list_node", "expr_list2", "expr_list3", "expr_list4", "assoc_pair",
  "lvalue", "expr4", "@12", "@13", "expr_or_block", "catch", "@14",
  "sscanf", "time_expression", "@15", "lvalue_list", "string",
  "string_con1", "string_con2", "class_init", "opt_class_init",
  "function_call", "efun_override", "function_name", "cond",
  "optional_else_part", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,    63,
     124,    94,    38,   304,   305,    60,    43,    45,    42,    37,
      47,   126,    59,    40,    41,    58,    44,   123,   125,    36,
      91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    74,    74,    75,    75,    76,    77,    78,
      79,    79,    80,    80,    80,    81,    81,    83,    84,    82,
      82,    82,    82,    82,    85,    86,    87,    87,    88,    88,
      90,    89,    91,    91,    92,    92,    93,    94,    94,    94,
      95,    95,    95,    96,    96,    97,    97,    98,    99,   100,
     100,   101,   101,   102,   102,   104,   103,   105,   105,   105,
     106,   107,   106,   108,   108,   109,   110,   111,   111,   112,
     112,   112,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   114,   113,   116,   115,   118,   117,   120,   119,
     121,   121,   121,   122,   122,   124,   123,   125,   125,   126,
     126,   128,   127,   129,   129,   129,   130,   130,   130,   131,
     131,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     133,   133,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   135,   135,   136,   136,   136,   137,
     137,   138,   138,   139,   139,   139,   140,   140,   141,   142,
     143,   143,   143,   143,   144,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   145,   143,
     143,   143,   143,   143,   143,   146,   146,   148,   147,   149,
     151,   150,   152,   152,   153,   154,   154,   154,   155,   155,
     156,   157,   157,   158,   158,   158,   158,   158,   158,   158,
     159,   159,   160,   160,   160,   160,   161,   162,   162
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     0,     0,     1,     4,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     0,     0,     9,
       3,     1,     1,     1,     2,     2,     1,     3,     0,     4,
       0,     7,     1,     1,     1,     2,     1,     2,     3,     1,
       0,     1,     2,     1,     3,     0,     2,     2,     4,     1,
       0,     1,     3,     2,     4,     0,     4,     1,     1,     1,
       0,     0,     5,     2,     4,     3,     3,     1,     3,     0,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     4,     0,     6,     0,     8,     0,    10,
       1,     1,     1,     1,     3,     0,     8,     0,     1,     1,
       1,     0,    10,     2,     2,     0,     3,     5,     2,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     2,
       1,     3,     3,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     2,     3,     0,     1,     2,     1,
       2,     1,     3,     0,     1,     2,     1,     3,     3,     1,
       1,     1,     1,     1,     0,     5,     3,     6,     7,     8,
       7,     5,     6,     5,     4,     1,     3,     1,     0,     6,
       3,     5,     4,     4,     4,     1,     3,     0,     3,     7,
       0,     3,     0,     3,     1,     1,     3,     3,     1,     2,
       3,     0,     3,     4,     4,     6,     4,     4,     6,     7,
       3,     3,     1,     2,     3,     3,     6,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    45,     1,    45,    21,     5,    23,    22,    50,
      10,    46,     6,     3,    34,     0,     0,    24,    36,    49,
      47,    11,     0,     0,    51,   218,     0,     0,   215,    35,
      16,     0,    15,    53,    20,    10,     0,     0,     7,   219,
      30,     0,     0,     0,    52,   216,   217,    28,     0,     9,
       8,   198,   181,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   207,   210,     0,   183,     0,     0,
       0,     0,   163,   162,     0,     0,    54,     0,   159,   197,
     160,   161,   195,   214,   180,     0,     0,    40,    53,    45,
       0,     0,     0,     0,     0,     0,   152,   179,   153,   154,
     233,   169,     0,   171,   167,     0,     0,   174,   176,     0,
     130,     0,     0,     0,     0,     0,     0,   156,   155,    34,
       0,     0,    10,     0,   184,     0,   151,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   157,   158,     0,     0,     0,     0,
       0,    33,    32,    39,    10,    43,     0,    41,    31,    50,
      10,   133,   234,    40,     0,   231,   230,   170,     0,     0,
       0,     0,     0,     0,     0,   200,     0,     0,     0,    55,
     205,   208,   211,     0,     0,    35,     0,     0,   196,     0,
     235,   136,   135,   144,   145,   142,     0,   137,   138,   139,
     141,   140,   143,   146,   147,   148,   149,   150,   132,   186,
       0,     0,     0,     0,    37,    18,    42,     0,     0,    26,
       0,     0,   226,   204,   172,   178,   203,   177,   202,   131,
       0,     0,     0,     0,     0,   221,   224,     0,    48,     0,
       0,     0,     0,     0,   194,   223,   227,    38,     0,    44,
      25,    10,    29,     0,   201,   212,   206,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,    79,    82,    57,
      78,     0,     0,    74,    75,    58,    59,    76,     0,    77,
      73,     0,     0,   185,   134,     0,     0,   193,     0,   191,
       0,    14,    13,    19,    12,    27,   199,     0,     0,    71,
       0,     0,     0,     0,     0,     0,    80,    81,   164,     0,
      10,    56,    70,    72,   225,     0,     0,   228,     0,   192,
       0,     0,   187,   212,   209,     0,     0,     0,     0,    10,
       0,   100,    99,     0,    98,    90,    92,    91,    93,     0,
     165,     0,    67,     0,     0,   222,   229,     0,   188,   190,
     213,     0,   101,    84,     0,     0,     0,     0,     0,     0,
      63,    10,    83,     0,   189,   237,     0,     0,     0,    65,
      66,     0,    94,     0,     0,    68,   220,     0,   236,    60,
      85,     0,     0,    95,    64,   238,     0,     0,     0,     0,
       0,     0,    61,     0,    87,    88,    96,   126,     0,     0,
       0,     0,     0,   109,   110,   108,    10,     0,     0,     0,
       0,   128,   127,   129,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,   102,   103,    89,   125,     0,     0,   118,
     119,   116,   111,   112,   113,   115,   114,   117,   120,   121,
     122,   123,   124,    62,   107
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    13,     5,    72,    73,   341,   293,    74,
       6,    42,   248,     7,   219,   220,    89,     8,    47,   153,
      18,   268,   155,   156,   157,     9,    10,    75,    20,    23,
      24,   269,   234,   270,   386,   406,   342,   337,   331,   343,
     271,   407,   310,   273,   367,   274,   303,   275,   410,   338,
     339,   276,   389,   332,   333,   277,   366,   408,   409,   402,
     403,   278,   110,   279,   102,   103,   104,   106,   107,   108,
      77,    78,   189,    92,   181,    79,   114,    80,    81,   115,
     298,    82,    36,    83,   345,   281,    84,    85,    86,   280,
     378
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -335
static const yytype_int16 yypact[] =
{
    -335,    41,    49,  -335,    37,  -335,    -8,  -335,  -335,   126,
       7,  -335,  -335,  -335,  -335,    16,   130,  -335,  -335,  -335,
    -335,  -335,   323,     9,    62,  -335,    16,    -3,    71,    51,
    -335,    80,  -335,    23,  -335,     7,   -13,    16,  -335,  -335,
    -335,  1573,    92,   323,  -335,  -335,  -335,  -335,   172,  -335,
    -335,   155,   105,   123,   163,   221,   221,  1573,   323,  1102,
     489,  1573,   299,   200,  -335,  -335,   203,  -335,  1573,  1573,
     832,   220,  -335,  -335,   188,  1573,  1209,   230,   111,  -335,
    -335,  -335,  -335,    71,  -335,   226,   233,   170,   286,    14,
    1573,   323,   245,  1171,   142,  1238,  -335,    27,  -335,  -335,
    -335,  1276,   260,  -335,   267,   803,   282,   292,  -335,   301,
    1209,   309,  1573,  1573,    52,    52,   899,  -335,  -335,   124,
     366,  1573,     7,    -9,  -335,   323,  -335,  1573,  1573,  1573,
    1573,  1573,  1573,  1573,  1573,  1573,  1573,  1573,  1573,  1573,
    1573,  1573,  1573,  1573,  -335,  -335,  1573,   323,  1305,  1171,
    1171,  -335,  -335,  -335,     7,  -335,   313,    11,  -335,   128,
       7,  1209,  -335,   170,   314,  -335,  -335,  -335,   316,   966,
    1573,   317,   558,   318,  1573,  -335,   303,   222,  1573,  -335,
    -335,  -335,  -335,   375,   320,  -335,   106,   321,  -335,  1573,
    -335,   462,   393,   235,   235,   253,   870,   284,  1139,  1406,
     149,   149,   253,   227,   227,  -335,  -335,  -335,  1209,   324,
    1573,   -19,   332,   335,   363,  -335,  -335,   170,   323,   336,
     341,   340,  -335,  -335,  -335,  1209,  -335,  -335,  -335,  1209,
     349,  1573,  1573,   157,   696,  -335,  -335,   342,  -335,   162,
    1573,  1171,    60,   351,  -335,  -335,  -335,  -335,     8,  -335,
    -335,     7,  -335,   348,  -335,  1607,  -335,    19,   353,   354,
     355,  -335,   356,   364,   371,   372,  1372,  -335,  -335,  -335,
    -335,   367,   696,  -335,  -335,  -335,  -335,  -335,   135,  -335,
    -335,   168,  1171,  -335,  1209,   373,   420,  -335,  1573,  -335,
     103,  -335,  -335,  -335,  -335,  -335,  -335,   221,   374,  -335,
    1573,  1573,  1573,   765,  1035,   174,  -335,  -335,  -335,   152,
       7,  -335,  -335,  -335,  -335,   323,   376,  -335,  1573,  -335,
     118,   127,  -335,   370,  -335,   180,   186,   190,   414,     7,
     429,  -335,  -335,   392,   389,  -335,  -335,  -335,   390,   434,
    -335,   363,   402,   407,   405,  -335,  -335,   151,  -335,  -335,
    -335,   765,  -335,  -335,   408,   363,  1573,  1439,   174,  1573,
     459,     7,  -335,  1573,  -335,   452,   409,   765,  1573,  -335,
    1209,   412,  -335,   937,  1573,  -335,  1209,   765,  -335,  -335,
    -335,   239,  1506,  -335,  1209,  -335,   171,   422,   418,   765,
      31,   421,  -335,   627,  -335,  -335,  -335,  -335,   481,   483,
     484,    31,    13,   531,   440,  -335,     7,   627,   435,   627,
     765,  -335,  -335,  -335,  1072,    31,  -335,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,   442,  -335,  -335,  -335,  -335,  -335,   437,    72,   262,
     262,   290,  1004,  1340,  1414,  1347,  1347,   290,   266,   266,
    -335,  -335,  -335,  -335,  -335
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -335,  -335,  -335,  -335,  -335,  -335,  -335,    -6,  -335,     2,
    -335,  -335,  -335,  -335,  -335,   254,  -335,  -335,  -335,  -211,
    -335,    -7,   289,   344,  -335,     1,   436,  -335,  -335,   474,
    -335,   -89,  -335,  -335,  -335,  -335,  -335,   206,  -335,  -334,
     251,  -214,  -335,  -335,  -335,  -335,  -335,  -335,  -335,   153,
    -335,  -335,  -335,  -309,  -335,  -335,  -335,   -93,   138,   119,
    1267,   -53,   -29,  -335,   -83,  -162,   425,  -335,  -335,   368,
     -42,   -40,  -335,  -335,   423,  -335,  -335,  -335,  -335,  -335,
     216,  -335,   -14,   -15,  -335,  -335,  -335,  -335,  -335,  -335,
    -335
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -233
static const yytype_int16 yytable[] =
{
      28,    27,    19,   247,    22,    11,   243,   224,   109,   291,
     164,    28,    76,    96,    98,    97,    97,   123,    31,    25,
     272,     4,    28,    46,    33,   180,   180,   375,    99,    43,
     101,   105,    90,   184,    25,   397,    41,   216,   415,   117,
     118,     3,   123,    37,     4,    88,   126,   174,   371,    -2,
     398,    45,   244,    37,    12,   188,     4,   174,   272,    38,
     100,   161,   147,   122,   101,    21,   212,   213,   186,   224,
     292,    34,   431,   388,    39,   179,   397,   217,   416,    26,
     154,   299,   158,   101,   177,   286,   -17,   101,   399,   328,
     159,   398,   400,   162,   401,   211,   166,   148,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   178,   187,   208,   -15,   179,
     101,   101,  -179,  -179,  -179,   233,   174,   190,    35,   399,
     360,   287,    14,   400,    14,   438,   239,   365,    29,    30,
     101,   225,   -15,   105,   369,   229,   147,    40,   214,   209,
      32,    30,    19,   380,   218,    87,   154,   242,   285,   294,
     -16,    91,    15,   385,   296,   129,   130,   131,    93,   174,
     237,    16,   174,   120,   322,   396,    14,    14,   151,   152,
      14,   148,   335,   336,   174,    90,  -232,  -198,   165,   348,
     290,    17,    91,   174,   390,   391,   435,   313,   349,   316,
      94,   174,   101,   255,   138,   139,   140,   141,   142,   143,
     154,   284,   101,   309,   340,   120,   120,   174,   174,   120,
     250,   256,   364,   174,    25,   125,   283,    51,   174,    52,
      53,    54,   314,   320,   315,   321,   127,   128,   129,   130,
     131,   144,   145,   146,   351,   218,   174,   325,   326,   327,
     352,   334,   174,   101,   353,   323,   174,    97,    58,    59,
      60,    61,    62,   113,    64,   347,   116,    66,    67,   129,
     130,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   124,    95,   141,   142,   143,   232,   149,
      71,   139,   140,   141,   142,   143,   150,   329,   329,    41,
     129,   130,   131,   387,   334,   174,   417,   418,   163,   139,
     140,   141,   142,   143,   432,   381,   434,   344,   426,   427,
     428,   429,   430,   355,   428,   429,   430,   370,   168,   334,
     373,    32,    30,   169,   376,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   384,   426,   427,   428,   429,
     430,   329,    48,   171,    25,    49,    50,    51,   172,    52,
      53,    54,    55,    56,   111,   112,   173,   174,   230,   231,
      57,   151,   152,   175,   185,    28,   404,   215,   222,   392,
     223,   226,   228,   235,   236,   238,    28,   241,    58,    59,
      60,    61,    62,    63,    64,    65,   245,    66,    67,   246,
      28,   404,   251,   252,   253,   282,   288,   127,    68,   129,
     130,   131,    69,   254,    70,   179,   300,   301,   302,   304,
      71,    48,   289,    25,    49,    50,    51,   305,    52,    53,
      54,    55,    56,   306,   307,   311,   297,   317,   324,    57,
     346,   354,   356,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   357,   174,   358,    58,    59,    60,
      61,    62,    63,    64,    65,   359,    66,    67,   361,   362,
     363,   368,   374,   377,   382,   318,   379,    68,   129,   130,
     131,    69,   395,    70,   394,   411,   405,   412,   413,    71,
      48,   319,    25,    49,    50,    51,    37,    52,    53,    54,
      55,    56,   454,   433,   453,   295,   249,   221,    57,    44,
     330,   372,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   312,   393,   160,    58,    59,    60,    61,
      62,    63,    64,    65,   437,    66,    67,   176,   182,   350,
     227,     0,     0,     0,     0,     0,    68,   417,   418,   419,
      69,     0,    70,     0,     0,     0,     0,     0,    71,    48,
    -173,    25,    49,    50,    51,     0,    52,    53,    54,    55,
      56,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,   420,   421,   422,   423,   424,   425,   426,   427,   428,
     429,   430,     0,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,     0,    66,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,    69,
       0,    70,     0,     0,     0,     0,     0,    71,    48,  -175,
      25,    49,    50,   119,     0,    52,    53,    54,    55,    56,
       0,     0,     0,     0,     0,     0,    57,   258,     0,   259,
     390,   391,     0,     0,   260,   261,   262,   263,     0,   264,
     265,   266,     0,     0,    58,    59,    60,    61,    62,    63,
      64,    65,   120,    66,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,    69,   267,
      70,     0,     0,     0,   179,  -105,    71,   257,     0,    25,
      49,    50,   119,     0,    52,    53,    54,    55,    56,     0,
       0,     0,     0,     0,     0,    57,   258,     0,   259,     0,
       0,     0,     0,   260,   261,   262,   263,     0,   264,   265,
     266,     0,     0,    58,    59,    60,    61,    62,    63,    64,
      65,   120,    66,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,    69,   267,    70,
       0,     0,     0,   179,   -69,    71,    48,     0,    25,    49,
      50,   119,     0,    52,    53,    54,    55,    56,     0,     0,
       0,     0,     0,     0,    57,   258,     0,   259,     0,     0,
       0,     0,   260,   261,   262,   263,     0,   264,   265,   266,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
     120,    66,    67,     0,     0,     0,     0,   127,   128,   129,
     130,   131,    68,     0,     0,     0,    69,   267,    70,     0,
       0,     0,   179,    48,    71,    25,    49,    50,   119,     0,
      52,    53,    54,    55,    56,     0,     0,     0,     0,     0,
       0,    57,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,     0,     0,     0,     0,   170,    58,
      59,    60,    61,    62,    63,    64,    65,   120,    66,    67,
       0,     0,     0,     0,   127,   128,   129,   130,   131,    68,
     121,     0,     0,    69,     0,    70,     0,     0,     0,     0,
      48,    71,    25,    49,    50,    51,     0,    52,    53,    54,
      55,    56,     0,     0,     0,     0,     0,     0,    57,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,     0,     0,     0,   240,    58,    59,    60,    61,
      62,    63,    64,    65,   183,    66,    67,     0,     0,     0,
       0,   127,   128,   129,   130,   131,    68,     0,     0,     0,
      69,     0,    70,  -166,     0,     0,     0,    48,    71,    25,
      49,    50,    51,     0,    52,    53,    54,    55,    56,     0,
       0,     0,     0,     0,     0,    57,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,     0,     0,
       0,   383,     0,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,     0,     0,     0,     0,     0,
     417,   418,   419,    68,     0,     0,     0,    69,     0,    70,
    -168,     0,     0,     0,  -168,    71,    48,     0,    25,    49,
      50,   119,     0,    52,    53,    54,    55,    56,     0,     0,
       0,     0,     0,     0,    57,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
     120,    66,    67,     0,     0,     0,     0,     0,   417,   418,
     419,     0,    68,     0,     0,     0,    69,   -97,    70,     0,
       0,     0,     0,    48,    71,    25,    49,    50,    51,     0,
      52,    53,    54,    55,    56,     0,     0,     0,     0,     0,
       0,    57,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,     0,     0,     0,   436,     0,     0,    58,
      59,    60,    61,    62,    63,    64,    65,     0,    66,    67,
       0,     0,     0,     0,     0,   129,   130,   131,     0,    68,
       0,     0,     0,    69,     0,    70,     0,     0,     0,     0,
    -166,    71,    48,     0,    25,    49,    50,    51,     0,    52,
      53,    54,    55,    56,     0,     0,     0,     0,     0,     0,
      57,   135,   136,   137,   138,   139,   140,   141,   142,   143,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,    63,    64,    65,     0,    66,    67,     0,
       0,     0,     0,   127,   128,   129,   130,   131,    68,     0,
       0,     0,    69,     0,    70,  -166,     0,     0,     0,    48,
      71,    25,    49,    50,    51,     0,    52,    53,    54,    55,
      56,     0,     0,     0,     0,     0,     0,    57,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
      63,    64,    65,     0,    66,    67,     0,     0,     0,     0,
     127,   128,   129,   130,   131,    68,   121,     0,     0,    69,
       0,    70,   167,     0,     0,     0,    48,    71,    25,    49,
      50,    51,     0,    52,    53,    54,    55,    56,     0,     0,
       0,     0,     0,     0,    57,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,    66,    67,     0,     0,     0,   417,   418,   419,     0,
     210,     0,    68,   417,   418,   419,    69,     0,    70,     0,
       0,     0,     0,    48,    71,    25,    49,    50,    51,     0,
      52,    53,    54,    55,    56,     0,     0,     0,     0,     0,
       0,    57,   422,   423,   424,   425,   426,   427,   428,   429,
     430,     0,   425,   426,   427,   428,   429,   430,     0,    58,
      59,    60,    61,    62,    63,    64,    65,     0,    66,    67,
       0,     0,   129,   130,   131,     0,     0,     0,     0,    68,
     417,   418,   419,    69,   308,    70,     0,     0,     0,     0,
      48,    71,    25,    49,    50,    51,     0,    52,    53,    54,
      55,    56,     0,     0,     0,     0,     0,     0,    57,   136,
     137,   138,   139,   140,   141,   142,   143,   423,   424,   425,
     426,   427,   428,   429,   430,     0,    58,    59,    60,    61,
      62,    63,    64,    65,     0,    66,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
      69,   -97,    70,     0,     0,     0,     0,    48,    71,    25,
      49,    50,    51,     0,    52,    53,    54,    55,    56,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,    69,     0,    70,
     -97,     0,     0,     0,    48,    71,    25,    49,    50,    51,
       0,    52,    53,    54,    55,    56,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,    63,    64,    65,     0,    66,
      67,   127,   128,   129,   130,   131,     0,     0,     0,     0,
      68,     0,     0,     0,    69,     0,    70,     0,     0,     0,
       0,     0,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   414,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
       0,     0,     0,     0,     0,   414
};

static const yytype_int16 yycheck[] =
{
      15,    15,     9,   214,    10,     4,    25,   169,    61,     1,
      93,    26,    41,    55,    56,    55,    56,    70,    16,     3,
     234,     7,    37,    37,    22,   114,   115,   361,    57,    35,
      59,    60,    13,   116,     3,     4,    13,    26,    25,    68,
      69,     0,    95,    56,     7,    43,    75,    66,   357,     0,
      19,    64,    71,    56,    62,    64,     7,    66,   272,    62,
      58,    90,    35,    70,    93,    58,   149,   150,   121,   231,
      62,    62,   406,   382,     3,    67,     4,    66,    65,    63,
      87,    62,    68,   112,   113,    25,    63,   116,    57,   303,
      89,    19,    61,    91,    63,   148,    94,    70,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,    63,   122,   146,    67,    67,
     149,   150,    11,    12,    13,   178,    66,   125,    66,    57,
     341,    71,     6,    61,     6,    63,   189,   351,     8,     9,
     169,   170,    37,   172,   355,   174,    35,    67,   154,   147,
       8,     9,   159,   367,   160,    63,   163,   210,   241,   248,
      37,    37,    36,   377,   253,    16,    17,    18,    63,    66,
      64,    45,    66,    45,    71,   389,     6,     6,     8,     9,
       6,    70,     8,     9,    66,    13,    63,    63,    46,    71,
     243,    65,    37,    66,    23,    24,   410,    62,    71,   282,
      37,    66,   231,   232,    55,    56,    57,    58,    59,    60,
     217,   240,   241,   266,    62,    45,    45,    66,    66,    45,
     218,    64,    71,    66,     3,    37,    64,     6,    66,     8,
       9,    10,    64,   286,    66,   288,    14,    15,    16,    17,
      18,    11,    12,    13,    64,   251,    66,   300,   301,   302,
      64,   304,    66,   282,    64,   297,    66,   297,    37,    38,
      39,    40,    41,    63,    43,   318,    63,    46,    47,    16,
      17,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    63,    63,    58,    59,    60,    66,    63,
      69,    56,    57,    58,    59,    60,    63,   304,   305,    13,
      16,    17,    18,    64,   357,    66,    16,    17,    63,    56,
      57,    58,    59,    60,   407,   368,   409,   315,    56,    57,
      58,    59,    60,   329,    58,    59,    60,   356,    68,   382,
     359,     8,     9,    66,   363,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,   374,    56,    57,    58,    59,
      60,   358,     1,    71,     3,     4,     5,     6,    66,     8,
       9,    10,    11,    12,    65,    66,    65,    66,    65,    66,
      19,     8,     9,    64,     8,   390,   390,    64,    64,   386,
      64,    64,    64,     8,    64,    64,   401,    63,    37,    38,
      39,    40,    41,    42,    43,    44,    64,    46,    47,    64,
     415,   415,    66,    62,    64,    63,    55,    14,    57,    16,
      17,    18,    61,    64,    63,    67,    63,    63,    63,    63,
      69,     1,    71,     3,     4,     5,     6,    63,     8,     9,
      10,    11,    12,    62,    62,    68,    66,    64,    64,    19,
      64,    27,    13,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    62,    66,    66,    37,    38,    39,
      40,    41,    42,    43,    44,    31,    46,    47,    66,    62,
      65,    63,    13,    21,    62,    55,    67,    57,    16,    17,
      18,    61,    64,    63,    62,     4,    65,     4,     4,    69,
       1,    71,     3,     4,     5,     6,    56,     8,     9,    10,
      11,    12,    65,    68,    62,   251,   217,   163,    19,    35,
     304,   358,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,   272,   386,    89,    37,    38,    39,    40,
      41,    42,    43,    44,   415,    46,    47,   112,   115,   323,
     172,    -1,    -1,    -1,    -1,    -1,    57,    16,    17,    18,
      61,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,     1,
      71,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    61,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    69,     1,    71,
       3,     4,     5,     6,    -1,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    22,
      23,    24,    -1,    -1,    27,    28,    29,    30,    -1,    32,
      33,    34,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    62,
      63,    -1,    -1,    -1,    67,    68,    69,     1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    22,    -1,
      -1,    -1,    -1,    27,    28,    29,    30,    -1,    32,    33,
      34,    -1,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    69,     1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    -1,    22,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    -1,    32,    33,    34,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    57,    -1,    -1,    -1,    61,    62,    63,    -1,
      -1,    -1,    67,     1,    69,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    -1,    -1,    65,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    -1,    -1,    -1,    14,    15,    16,    17,    18,    57,
      58,    -1,    -1,    61,    -1,    63,    -1,    -1,    -1,    -1,
       1,    69,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    -1,    -1,    -1,    65,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    57,    -1,    -1,    -1,
      61,    -1,    63,    64,    -1,    -1,    -1,     1,    69,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    17,    18,    57,    -1,    -1,    -1,    61,    -1,    63,
      64,    -1,    -1,    -1,    68,    69,     1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    16,    17,
      18,    -1,    57,    -1,    -1,    -1,    61,    62,    63,    -1,
      -1,    -1,    -1,     1,    69,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    16,    17,    18,    -1,    57,
      -1,    -1,    -1,    61,    -1,    63,    -1,    -1,    -1,    -1,
      68,    69,     1,    -1,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    47,    -1,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    57,    -1,
      -1,    -1,    61,    -1,    63,    64,    -1,    -1,    -1,     1,
      69,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    57,    58,    -1,    -1,    61,
      -1,    63,    26,    -1,    -1,    -1,     1,    69,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    47,    -1,    -1,    -1,    16,    17,    18,    -1,
      55,    -1,    57,    16,    17,    18,    61,    -1,    63,    -1,
      -1,    -1,    -1,     1,    69,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    55,    56,    57,    58,    59,    60,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    -1,    16,    17,    18,    -1,    -1,    -1,    -1,    57,
      16,    17,    18,    61,    62,    63,    -1,    -1,    -1,    -1,
       1,    69,     3,     4,     5,     6,    -1,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    53,
      54,    55,    56,    57,    58,    59,    60,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      61,    62,    63,    -1,    -1,    -1,    -1,     1,    69,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    -1,    63,
      64,    -1,    -1,    -1,     1,    69,     3,     4,     5,     6,
      -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    46,
      47,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    61,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,   401,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   438
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    73,    74,     0,     7,    76,    82,    85,    89,    97,
      98,    97,    62,    75,     6,    36,    45,    65,    92,    93,
     100,    58,    79,   101,   102,     3,    63,   154,   155,     8,
       9,    81,     8,    81,    62,    66,   154,    56,    62,     3,
      67,    13,    83,    79,   101,    64,   154,    90,     1,     4,
       5,     6,     8,     9,    10,    11,    12,    19,    37,    38,
      39,    40,    41,    42,    43,    44,    46,    47,    57,    61,
      63,    69,    77,    78,    81,    99,   134,   142,   143,   147,
     149,   150,   153,   155,   158,   159,   160,    63,    81,    88,
      13,    37,   145,    63,    37,    63,   142,   143,   142,   134,
      81,   134,   136,   137,   138,   134,   139,   140,   141,   133,
     134,    65,    66,    63,   148,   151,    63,   134,   134,     6,
      45,    58,    93,   133,    63,    37,   134,    14,    15,    16,
      17,    18,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    11,    12,    13,    35,    70,    63,
      63,     8,     9,    91,    93,    94,    95,    96,    68,    97,
      98,   134,    81,    63,   136,    46,    81,    26,    68,    66,
      65,    71,    66,    65,    66,    64,   138,   134,    63,    67,
     103,   146,   146,    45,   136,     8,   133,    79,    64,   144,
      81,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,    81,
      55,   133,   136,   136,    79,    64,    26,    66,    79,    86,
      87,    95,    64,    64,   137,   134,    64,   141,    64,   134,
      65,    66,    66,   133,   104,     8,    64,    64,    64,   133,
      65,    63,   133,    25,    71,    64,    64,    91,    84,    94,
      81,    66,    62,    64,    64,   134,    64,     1,    20,    22,
      27,    28,    29,    30,    32,    33,    34,    62,    93,   103,
     105,   112,   113,   115,   117,   119,   123,   127,   133,   135,
     161,   157,    63,    64,   134,   136,    25,    71,    55,    71,
     133,     1,    62,    80,   103,    87,   103,    66,   152,    62,
      63,    63,    63,   118,    63,    63,    62,    62,    62,   133,
     114,    68,   112,    62,    64,    66,   136,    64,    55,    71,
     133,   133,    71,   142,    64,   133,   133,   133,   113,    93,
     109,   110,   125,   126,   133,     8,     9,   109,   121,   122,
      62,    79,   108,   111,    81,   156,    64,   133,    71,    71,
     152,    64,    64,    64,    27,    79,    13,    62,    66,    31,
      91,    66,    62,    65,    71,   113,   128,   116,    63,    91,
     134,   125,   121,   134,    13,   111,   134,    21,   162,    67,
     113,   133,    62,    64,   134,   113,   106,    64,   125,   124,
      23,    24,    93,   130,    62,    64,   113,     4,    19,    57,
      61,    63,   131,   132,   154,    65,   107,   113,   129,   130,
     120,     4,     4,     4,   132,    25,    65,    16,    17,    18,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,   111,   129,    68,   129,   113,    64,   131,    63,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,    62,    65
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:

    {
#line 217 "grammar/grammar.y.pre"
		yywarn("Extra ';'. Ignored.");
	    }
    break;

  case 7:

    {
#line 225 "grammar/grammar.y.pre"
		object_t *ob;
		inherit_t inherit;
		int initializer;

		(yyvsp[(1) - (4)].type) |= global_modifiers;

		if (var_defined)
		    yyerror("Illegal to inherit after defining global variables.");
		ob = find_object2((yyvsp[(3) - (4)].string));
		if (ob == 0) {
		    inherit_file = alloc_cstring((yyvsp[(3) - (4)].string), "inherit");
		    /* Return back to load_object() */
		    YYACCEPT;
		}
		scratch_free((yyvsp[(3) - (4)].string));
		inherit.prog = ob->prog;
		inherit.function_index_offset =
		      mem_block[A_RUNTIME_FUNCTIONS].current_size /
		      sizeof (runtime_function_u);
		inherit.variable_index_offset =
		      mem_block[A_VAR_TEMP].current_size /
		      sizeof (variable_t);
		inherit.type_mod = (yyvsp[(1) - (4)].type);
		add_to_mem_block(A_INHERITS, (char *)&inherit, sizeof inherit);
		copy_variables(ob->prog, (yyvsp[(1) - (4)].type));
		copy_structures(ob->prog);
		initializer = copy_functions(ob->prog, (yyvsp[(1) - (4)].type));
		if (initializer >= 0) {
		    /* initializer is an index into the object we're
		       inheriting's function table; this finds the
		       appropriate entry in our table and generates
		       a call to it */
		    switch_to_block(A_INITIALIZER);
		    generate_inherited_init_call(mem_block[A_INHERITS].current_size/sizeof(inherit_t) - 1, initializer);
		    switch_to_block(A_PROGRAM);
		}
	    }
    break;

  case 8:

    {
#line 267 "grammar/grammar.y.pre"
		CREATE_REAL((yyval.node), (yyvsp[(1) - (1)].real));
	    }
    break;

  case 9:

    {
#line 274 "grammar/grammar.y.pre"
		CREATE_NUMBER((yyval.node), (yyvsp[(1) - (1)].number));
	    }
    break;

  case 10:

    {
#line 281 "grammar/grammar.y.pre"
		(yyval.type) = 0;
	    }
    break;

  case 11:

    {
#line 285 "grammar/grammar.y.pre"
		(yyval.type) = TYPE_MOD_ARRAY;
	    }
    break;

  case 12:

    {
#line 292 "grammar/grammar.y.pre"
		(yyval.node) = (yyvsp[(1) - (1)].decl).node;
		if (!(yyval.node)) {
		    CREATE_RETURN((yyval.node), 0);
		}
            }
    break;

  case 13:

    {
#line 299 "grammar/grammar.y.pre"
		(yyval.node) = 0;
	    }
    break;

  case 14:

    {
#line 303 "grammar/grammar.y.pre"
		(yyval.node) = 0;
            }
    break;

  case 15:

    {
#line 310 "grammar/grammar.y.pre"
	      (yyval.string) = scratch_copy((yyvsp[(1) - (1)].ihe)->name);
	    }
    break;

  case 17:

    {
#line 318 "grammar/grammar.y.pre"
		(yyvsp[(1) - (3)].type) |= global_modifiers;
		/* Handle type checking here so we know whether to typecheck
		   'argument' */
		if ((yyvsp[(1) - (3)].type) & ~NAME_TYPE_MOD) {
		    exact_types = (yyvsp[(1) - (3)].type) | (yyvsp[(2) - (3)].type);
		} else {
		    if (pragmas & PRAGMA_STRICT_TYPES) {
			if (strcmp((yyvsp[(3) - (3)].string), "create") != 0)
		    yyerror("\"#pragma strict_types\" requires type of function");
			else
			    exact_types = TYPE_VOID; /* default for create() */
		    } else
			exact_types = 0;
		}
	    }
    break;

  case 18:

    {
#line 339 "grammar/grammar.y.pre"
		char *p = (yyvsp[(3) - (7)].string);
		(yyvsp[(3) - (7)].string) = make_shared_string((yyvsp[(3) - (7)].string));
		scratch_free(p);

		/* If we had nested functions, we would need to check */
		/* here if we have enough space for locals */

		/*
		 * Define a prototype. If it is a real function, then the
		 * prototype will be replaced below.
		 */

		(yyval.number) = NAME_UNDEFINED | NAME_PROTOTYPE;
		if ((yyvsp[(6) - (7)].argument).flags & ARG_IS_VARARGS) {
		    (yyval.number) |= NAME_TRUE_VARARGS;
		    (yyvsp[(1) - (7)].type) |= NAME_VARARGS;
		}
		define_new_function((yyvsp[(3) - (7)].string), (yyvsp[(6) - (7)].argument).num_arg, 0, (yyval.number), (yyvsp[(1) - (7)].type) | (yyvsp[(2) - (7)].type));
		/* This is safe since it is guaranteed to be in the
		   function table, so it can't be dangling */
		free_string((yyvsp[(3) - (7)].string));
		context = 0;
	    }
    break;

  case 19:

    {
#line 364 "grammar/grammar.y.pre"
		/* Either a prototype or a block */
		if ((yyvsp[(9) - (9)].node)) {
		    int fun;

		    (yyvsp[(8) - (9)].number) &= ~(NAME_UNDEFINED | NAME_PROTOTYPE);
		    if ((yyvsp[(9) - (9)].node)->kind != NODE_RETURN &&
			((yyvsp[(9) - (9)].node)->kind != NODE_TWO_VALUES
			 || (yyvsp[(9) - (9)].node)->r.expr->kind != NODE_RETURN)) {
			parse_node_t *replacement;
			CREATE_STATEMENTS(replacement, (yyvsp[(9) - (9)].node), 0);
			CREATE_RETURN(replacement->r.expr, 0);
			(yyvsp[(9) - (9)].node) = replacement;
		    }
		    if ((yyvsp[(6) - (9)].argument).flags & ARG_IS_PROTO) {
			yyerror("Missing name for function argument");
		    }
		    fun = define_new_function((yyvsp[(3) - (9)].string), (yyvsp[(6) - (9)].argument).num_arg,
					      max_num_locals - (yyvsp[(6) - (9)].argument).num_arg,
					      (yyvsp[(8) - (9)].number), (yyvsp[(1) - (9)].type) | (yyvsp[(2) - (9)].type));
		    if (fun != -1)
			COMPILER_FUNC(fun)->address =
			    generate_function(COMPILER_FUNC(fun), (yyvsp[(9) - (9)].node), max_num_locals);
		}
		free_all_local_names();
	    }
    break;

  case 20:

    {
#line 391 "grammar/grammar.y.pre"
		if (!(yyvsp[(1) - (3)].type))
			yyerror("Missing type for global variable declaration");
	    }
    break;

  case 24:

    {
#line 402 "grammar/grammar.y.pre"
		global_modifiers = (yyvsp[(1) - (2)].type);
	    }
    break;

  case 25:

    {
#line 409 "grammar/grammar.y.pre"
		if ((current_type & ~NAME_TYPE_MOD) == TYPE_VOID)
		    yyerror("Illegal to declare class member of type void.");
		add_local_name((yyvsp[(2) - (2)].string), current_type | (yyvsp[(1) - (2)].type));
		scratch_free((yyvsp[(2) - (2)].string));
	    }
    break;

  case 30:

    {
#line 429 "grammar/grammar.y.pre"
		ident_hash_elem_t *ihe;

		ihe = find_or_add_ident(
			   PROG_STRING((yyval.number) = store_prog_string((yyvsp[(3) - (4)].string))),
			   FOA_GLOBAL_SCOPE);
		if (ihe->dn.class_num == -1)
		    ihe->sem_value++;
		else {
		    /* Possibly, this should check if the definitions are
		       consistent */
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Illegal to redefine class ");
		    p = strput(p, end, (yyvsp[(3) - (4)].string));
		    yyerror(buf);
		}
		ihe->dn.class_num = mem_block[A_CLASS_DEF].current_size / sizeof(class_def_t);
	    }
    break;

  case 31:

    {
#line 451 "grammar/grammar.y.pre"
		class_def_t *sd;
		class_member_entry_t *sme;
		int i;

		sd = (class_def_t *)allocate_in_mem_block(A_CLASS_DEF, sizeof(class_def_t));
		i = sd->size = current_number_of_locals;
		sd->index = mem_block[A_CLASS_MEMBER].current_size / sizeof(class_member_entry_t);
		sd->name = (yyvsp[(5) - (7)].number);

		sme = (class_member_entry_t *)allocate_in_mem_block(A_CLASS_MEMBER, sizeof(class_member_entry_t) * current_number_of_locals);

		while (i--) {
		    sme[i].name = store_prog_string(locals_ptr[i]->name);
		    sme[i].type = type_of_locals_ptr[i];
		}

		free_all_local_names();
		scratch_free((yyvsp[(3) - (7)].string));
	    }
    break;

  case 33:

    {
#line 476 "grammar/grammar.y.pre"
		if ((yyvsp[(1) - (1)].ihe)->dn.local_num != -1) {
		    char buff[256];
		    char *end = EndOf(buff);
		    char *p;

		    p = strput(buff, end, "Illegal to redeclare local name '");
		    p = strput(p, end, (yyvsp[(1) - (1)].ihe)->name);
		    p = strput(p, end, "'");
		    yyerror(buff);
		}
		(yyval.string) = scratch_copy((yyvsp[(1) - (1)].ihe)->name);
	    }
    break;

  case 35:

    {
#line 494 "grammar/grammar.y.pre"
		if ((yyvsp[(2) - (2)].ihe)->dn.class_num == -1) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Undefined class '");
		    p = strput(p, end, (yyvsp[(2) - (2)].ihe)->name);
		    p = strput(p, end, "'");
		    yyerror(buf);
		    (yyval.type) = TYPE_ANY;
		} else
		    (yyval.type) = (yyvsp[(2) - (2)].ihe)->dn.class_num | TYPE_MOD_CLASS;
	    }
    break;

  case 37:

    {
#line 526 "grammar/grammar.y.pre"
		if ((yyvsp[(1) - (2)].type) == TYPE_VOID)
		    yyerror("Illegal to declare argument of type void.");
                (yyval.number) = ARG_IS_PROTO;
                add_local_name("", (yyvsp[(1) - (2)].type) | (yyvsp[(2) - (2)].type));
            }
    break;

  case 38:

    {
#line 533 "grammar/grammar.y.pre"
		if ((yyvsp[(1) - (3)].type) == TYPE_VOID)
		    yyerror("Illegal to declare argument of type void.");
                add_local_name((yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].type) | (yyvsp[(2) - (3)].type));
		scratch_free((yyvsp[(3) - (3)].string));
                (yyval.number) = 0;
	    }
    break;

  case 39:

    {
#line 541 "grammar/grammar.y.pre"
		if (exact_types) {
		    yyerror("Missing type for argument");
		}
		add_local_name((yyvsp[(1) - (1)].string), TYPE_ANY);
		scratch_free((yyvsp[(1) - (1)].string));
		(yyval.number) = 0;
            }
    break;

  case 40:

    {
#line 553 "grammar/grammar.y.pre"
		(yyval.argument).num_arg = 0;
                (yyval.argument).flags = 0;
	    }
    break;

  case 42:

    {
#line 559 "grammar/grammar.y.pre"
		int x = type_of_locals_ptr[max_num_locals-1];

		(yyval.argument) = (yyvsp[(1) - (2)].argument);
		(yyval.argument).flags |= ARG_IS_VARARGS;

		if (x != TYPE_ANY && !(x & TYPE_MOD_ARRAY))
		    yywarn("Variable to hold remainder of arguments should be an array.");
	    }
    break;

  case 43:

    {
#line 572 "grammar/grammar.y.pre"
		(yyval.argument).num_arg = 1;
                (yyval.argument).flags = (yyvsp[(1) - (1)].number);
	    }
    break;

  case 44:

    {
#line 577 "grammar/grammar.y.pre"
                (yyval.argument) = (yyvsp[(1) - (3)].argument);
		(yyval.argument).num_arg++;
                (yyval.argument).flags |= (yyvsp[(3) - (3)].number);
	    }
    break;

  case 45:

    {
#line 586 "grammar/grammar.y.pre"
		(yyval.type) = 0;
	    }
    break;

  case 46:

    {
#line 590 "grammar/grammar.y.pre"
		(yyval.type) = (yyvsp[(1) - (2)].type) | (yyvsp[(2) - (2)].type);
	    }
    break;

  case 47:

    {
#line 597 "grammar/grammar.y.pre"
		(yyval.type) = (yyvsp[(1) - (2)].type) | (yyvsp[(2) - (2)].type);
		current_type = (yyval.type);
	    }
    break;

  case 48:

    {
#line 605 "grammar/grammar.y.pre"
		(yyval.type) = (yyvsp[(2) - (4)].type) | (yyvsp[(3) - (4)].type);
	    }
    break;

  case 50:

    {
#line 613 "grammar/grammar.y.pre"
		(yyval.type) = TYPE_UNKNOWN;
	    }
    break;

  case 53:

    {
#line 625 "grammar/grammar.y.pre"
		if ((current_type & ~NAME_TYPE_MOD) == TYPE_VOID)
	    yyerror("Illegal to declare global variable of type void.");
		define_new_variable((yyvsp[(2) - (2)].string), current_type | (yyvsp[(1) - (2)].type) | global_modifiers);
		scratch_free((yyvsp[(2) - (2)].string));
	    }
    break;

  case 54:

    {
#line 632 "grammar/grammar.y.pre"
		parse_node_t *expr;
		int type = 0;

		if ((yyvsp[(3) - (4)].number) != F_ASSIGN)
	    yyerror("Only '=' is legal in initializers.");

		/* ignore current_type == 0, which gets a missing type error
		   later anyway */
		if (current_type) {
		    type = (current_type | (yyvsp[(1) - (4)].type) | global_modifiers) & ~NAME_TYPE_MOD;
		    if ((current_type & ~NAME_TYPE_MOD) == TYPE_VOID)
			yyerror("Illegal to declare global variable of type void.");
		    if (!compatible_types(type, (yyvsp[(4) - (4)].node)->type)) {
			char buff[256];
			char *end = EndOf(buff);
			char *p;

			p = strput(buff, end, "Type mismatch ");
			p = get_two_types(p, end, type, (yyvsp[(4) - (4)].node)->type);
			p = strput(p, end, " when initializing ");
			p = strput(p, end, (yyvsp[(2) - (4)].string));
			yyerror(buff);
		    }
		}
		switch_to_block(A_INITIALIZER);
		(yyvsp[(4) - (4)].node) = do_promotions((yyvsp[(4) - (4)].node), type);

		CREATE_BINARY_OP(expr, F_VOID_ASSIGN, 0, (yyvsp[(4) - (4)].node), 0);
		CREATE_OPCODE_1(expr->r.expr, F_GLOBAL_LVALUE, 0,
				define_new_variable((yyvsp[(2) - (4)].string), current_type | (yyvsp[(1) - (4)].type) | global_modifiers));
		generate(expr);
		switch_to_block(A_PROGRAM);
		scratch_free((yyvsp[(2) - (4)].string));
	    }
    break;

  case 55:

    {
#line 671 "grammar/grammar.y.pre"
	push_local_count(current_local_count);
	current_local_count = 0;
}
    break;

  case 56:

    {
#line 676 "grammar/grammar.y.pre"
		(yyval.decl).node = (yyvsp[(3) - (4)].node);
		(yyval.decl).num = current_local_count;
		current_local_count = pop_local_count();

	}
    break;

  case 60:

    {
#line 698 "grammar/grammar.y.pre"
                (yyval.decl).node = 0;
                (yyval.decl).num = 0;
            }
    break;

  case 61:

    {
#line 703 "grammar/grammar.y.pre"
		if ((yyvsp[(2) - (2)].type) == TYPE_VOID)
		    yyerror("Illegal to declare local variable of type void.");
                /* can't do this in basic_type b/c local_name_list contains
                 * expr0 which contains cast which contains basic_type
                 */
                current_type = (yyvsp[(2) - (2)].type);
            }
    break;

  case 62:

    {
#line 712 "grammar/grammar.y.pre"
                if ((yyvsp[(1) - (5)].decl).node && (yyvsp[(4) - (5)].decl).node) {
		    CREATE_STATEMENTS((yyval.decl).node, (yyvsp[(1) - (5)].decl).node, (yyvsp[(4) - (5)].decl).node);
                } else (yyval.decl).node = ((yyvsp[(1) - (5)].decl).node ? (yyvsp[(1) - (5)].decl).node : (yyvsp[(4) - (5)].decl).node);
                (yyval.decl).num = (yyvsp[(1) - (5)].decl).num + (yyvsp[(4) - (5)].decl).num;
            }
    break;

  case 63:

    {
#line 723 "grammar/grammar.y.pre"
		add_local_name((yyvsp[(2) - (2)].string), current_type | (yyvsp[(1) - (2)].type));
		scratch_free((yyvsp[(2) - (2)].string));
		(yyval.node) = 0;
	    }
    break;

  case 64:

    {
#line 729 "grammar/grammar.y.pre"
		int type = (current_type | (yyvsp[(1) - (4)].type)) & ~NAME_TYPE_MOD;

		if ((yyvsp[(3) - (4)].number) != F_ASSIGN)
		    yyerror("Only '=' is allowed in initializers.");
		if (!compatible_types((yyvsp[(4) - (4)].node)->type, type)) {
		    char buff[256];
		    char *end = EndOf(buff);
		    char *p;

		    p = strput(buff, end, "Type mismatch ");
		    p = get_two_types(p, end, type, (yyvsp[(4) - (4)].node)->type);
		    p = strput(p, end, " when initializing ");
		    p = strput(p, end, (yyvsp[(2) - (4)].string));

		    yyerror(buff);
		}

		(yyvsp[(4) - (4)].node) = do_promotions((yyvsp[(4) - (4)].node), type);

		CREATE_UNARY_OP_1((yyval.node), F_VOID_ASSIGN_LOCAL, 0, (yyvsp[(4) - (4)].node),
				  add_local_name((yyvsp[(2) - (4)].string), current_type | (yyvsp[(1) - (4)].type)));
		scratch_free((yyvsp[(2) - (4)].string));
	    }
    break;

  case 65:

    {
#line 757 "grammar/grammar.y.pre"
		if ((yyvsp[(1) - (3)].type) == TYPE_VOID)
		    yyerror(" Wrong declare local variable of type void.");

		(yyval.number) = add_local_name((yyvsp[(3) - (3)].string), (yyvsp[(1) - (3)].type) | (yyvsp[(2) - (3)].type));
		scratch_free((yyvsp[(3) - (3)].string));
	    }
    break;

  case 66:

    {
#line 768 "grammar/grammar.y.pre"
                int type = type_of_locals_ptr[(yyvsp[(1) - (3)].number)];

		if ((yyvsp[(2) - (3)].number) != F_ASSIGN)
		    yyerror("Only '=' is allowed in initializers.");
		if (!compatible_types((yyvsp[(3) - (3)].node)->type, type)) {
		    char buff[256];
		    char *end = EndOf(buff);
		    char *p;

		    p = strput(buff, end, "Type mismatch ");
		    p = get_two_types(p, end, type, (yyvsp[(3) - (3)].node)->type);
		    p = strput(p, end, " when initializing.");
		    yyerror(buff);
		}

		(yyvsp[(3) - (3)].node) = do_promotions((yyvsp[(3) - (3)].node), type);

		/* this is an expression */
		CREATE_BINARY_OP((yyval.node), F_ASSIGN, 0, (yyvsp[(3) - (3)].node), 0);
                CREATE_OPCODE_1((yyval.node)->r.expr, F_LOCAL_LVALUE, 0, (yyvsp[(1) - (3)].number));
	    }
    break;

  case 67:

    {
#line 794 "grammar/grammar.y.pre"
                (yyval.decl).node = (yyvsp[(1) - (1)].node);
                (yyval.decl).num = 1;
            }
    break;

  case 68:

    {
#line 799 "grammar/grammar.y.pre"
                if ((yyvsp[(1) - (3)].node) && (yyvsp[(3) - (3)].decl).node) {
		    CREATE_STATEMENTS((yyval.decl).node, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].decl).node);
                } else (yyval.decl).node = ((yyvsp[(1) - (3)].node) ? (yyvsp[(1) - (3)].node) : (yyvsp[(3) - (3)].decl).node);
                (yyval.decl).num = 1 + (yyvsp[(3) - (3)].decl).num;
            }
    break;

  case 69:

    {
#line 809 "grammar/grammar.y.pre"
		(yyval.node) = 0;
	    }
    break;

  case 70:

    {
#line 813 "grammar/grammar.y.pre"
		if ((yyvsp[(1) - (2)].node) && (yyvsp[(2) - (2)].node)) {
		    CREATE_STATEMENTS((yyval.node), (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node));
		} else (yyval.node) = ((yyvsp[(1) - (2)].node) ? (yyvsp[(1) - (2)].node) : (yyvsp[(2) - (2)].node));
            }
    break;

  case 71:

    {
#line 819 "grammar/grammar.y.pre"
		(yyval.node) = 0;
            }
    break;

  case 72:

    {
#line 826 "grammar/grammar.y.pre"
		(yyval.node) = insert_pop_value((yyvsp[(1) - (2)].node));
#ifdef DEBUG
		{
/* (deleted by dingdang 2001/8/28)
		    parse_node_t *replacement;
		    CREATE_STATEMENTS(replacement, $$, 0);
		    CREATE_OPCODE(replacement->r.expr, F_BREAK_POINT, 0);
		    $$ = replacement;
*/
		}
#endif
	    }
    break;

  case 78:

    {
#line 845 "grammar/grammar.y.pre"
                (yyval.node) = (yyvsp[(1) - (1)].decl).node;
                pop_n_locals((yyvsp[(1) - (1)].decl).num);
            }
    break;

  case 79:

    {
#line 850 "grammar/grammar.y.pre"
		(yyval.node) = 0;
	    }
    break;

  case 80:

    {
#line 854 "grammar/grammar.y.pre"
		if (context & SPECIAL_CONTEXT) {
		    yyerror("Cannot break out of catch { } or time_expression { }");
		    (yyval.node) = 0;
		} else
		if (context & SWITCH_CONTEXT) {
		    CREATE_CONTROL_JUMP((yyval.node), CJ_BREAK_SWITCH);
		} else
		if (context & LOOP_CONTEXT) {
		    CREATE_CONTROL_JUMP((yyval.node), CJ_BREAK);
		    if (context & LOOP_FOREACH) {
			parse_node_t *replace;
			CREATE_STATEMENTS(replace, 0, (yyval.node));
			CREATE_OPCODE(replace->l.expr, F_EXIT_FOREACH, 0);
			(yyval.node) = replace;
		    }
		} else {
		    yyerror("break statement outside loop");
		    (yyval.node) = 0;
		}
	    }
    break;

  case 81:

    {
#line 876 "grammar/grammar.y.pre"
		if (context & SPECIAL_CONTEXT)
		    yyerror("Cannot continue out of catch { } or time_expression { }");
		else
		if (!(context & LOOP_CONTEXT))
		    yyerror("continue statement outside loop");
		CREATE_CONTROL_JUMP((yyval.node), CJ_CONTINUE);
	    }
    break;

  case 82:

    {
#line 885 "grammar/grammar.y.pre"
	if ((yyvsp[(1) - (1)].type) == TYPE_VOID)
		yyerror("Illegal to declare local variable of type void.");
	/* can't do this in basic_type b/c local_name_list contains
	 * expr0 which contains cast which contains basic_type
	 */
	current_type = (yyvsp[(1) - (1)].type);
}
    break;

  case 83:

    {
#line 894 "grammar/grammar.y.pre"
	(yyval.node) = (yyvsp[(3) - (4)].decl).node;
	current_local_count += (yyvsp[(3) - (4)].decl).num;
}
    break;

  case 84:

    {
#line 902 "grammar/grammar.y.pre"
		(yyvsp[(1) - (4)].number) = context;
		context = LOOP_CONTEXT;
	    }
    break;

  case 85:

    {
#line 907 "grammar/grammar.y.pre"
		CREATE_LOOP((yyval.node), 1, (yyvsp[(6) - (6)].node), 0, optimize_loop_test((yyvsp[(3) - (6)].node)));
		context = (yyvsp[(1) - (6)].number);
	    }
    break;

  case 86:

    {
#line 915 "grammar/grammar.y.pre"
		(yyvsp[(1) - (1)].number) = context;
		context = LOOP_CONTEXT;
	    }
    break;

  case 87:

    {
#line 920 "grammar/grammar.y.pre"
		CREATE_LOOP((yyval.node), 0, (yyvsp[(3) - (8)].node), 0, optimize_loop_test((yyvsp[(6) - (8)].node)));
		context = (yyvsp[(1) - (8)].number);
	    }
    break;

  case 88:

    {
#line 928 "grammar/grammar.y.pre"
		(yyvsp[(1) - (8)].number) = context;
		context = LOOP_CONTEXT;
	    }
    break;

  case 89:

    {
#line 933 "grammar/grammar.y.pre"
		(yyval.decl).num = (yyvsp[(3) - (10)].decl).num; /* number of declarations (0/1) */

		(yyvsp[(3) - (10)].decl).node = insert_pop_value((yyvsp[(3) - (10)].decl).node);
		(yyvsp[(7) - (10)].node) = insert_pop_value((yyvsp[(7) - (10)].node));
		if ((yyvsp[(7) - (10)].node) && IS_NODE((yyvsp[(7) - (10)].node), NODE_UNARY_OP, F_INC)
		    && IS_NODE((yyvsp[(7) - (10)].node)->r.expr, NODE_OPCODE_1, F_LOCAL_LVALUE)) {
		    int lvar = (yyvsp[(7) - (10)].node)->r.expr->l.number;
		    CREATE_OPCODE_1((yyvsp[(7) - (10)].node), F_LOOP_INCR, 0, lvar);
		}

		CREATE_STATEMENTS((yyval.decl).node, (yyvsp[(3) - (10)].decl).node, 0);
		CREATE_LOOP((yyval.decl).node->r.expr, 1, (yyvsp[(10) - (10)].node), (yyvsp[(7) - (10)].node), optimize_loop_test((yyvsp[(5) - (10)].node)));

		context = (yyvsp[(1) - (10)].number);
	      }
    break;

  case 90:

    {
#line 952 "grammar/grammar.y.pre"
		if ((yyvsp[(1) - (1)].ihe)->dn.local_num != -1) {
		    CREATE_OPCODE_1((yyval.decl).node, F_LOCAL_LVALUE, 0, (yyvsp[(1) - (1)].ihe)->dn.local_num);
		} else
	        if ((yyvsp[(1) - (1)].ihe)->dn.global_num != -1) {
		    CREATE_OPCODE_1((yyval.decl).node, F_GLOBAL_LVALUE, 0, (yyvsp[(1) - (1)].ihe)->dn.global_num);
		} else {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "'");
		    p = strput(p, end, (yyvsp[(1) - (1)].ihe)->name);
		    p = strput(p, end, "' is not a local or a global variable.");
		    yyerror(buf);
		    CREATE_OPCODE_1((yyval.decl).node, F_GLOBAL_LVALUE, 0, 0);
		}
		(yyval.decl).num = 0;
            }
    break;

  case 91:

    {
#line 972 "grammar/grammar.y.pre"
                CREATE_OPCODE_1((yyval.decl).node, F_LOCAL_LVALUE, 0, (yyvsp[(1) - (1)].number));
		(yyval.decl).num = 1;
            }
    break;

  case 92:

    {
#line 977 "grammar/grammar.y.pre"
		char buf[256];
		char *end = EndOf(buf);
		char *p;

		p = strput(buf, end, "'");
		p = strput(p, end, (yyvsp[(1) - (1)].string));
		p = strput(p, end, "' is not a local or a global variable.");
		yyerror(buf);
		CREATE_OPCODE_1((yyval.decl).node, F_GLOBAL_LVALUE, 0, 0);
		scratch_free((yyvsp[(1) - (1)].string));
		(yyval.decl).num = 0;
	    }
    break;

  case 93:

    {
#line 994 "grammar/grammar.y.pre"
		CREATE_FOREACH((yyval.decl).node, (yyvsp[(1) - (1)].decl).node, 0);
		(yyval.decl).num = (yyvsp[(1) - (1)].decl).num;
            }
    break;

  case 94:

    {
#line 999 "grammar/grammar.y.pre"
		CREATE_FOREACH((yyval.decl).node, (yyvsp[(1) - (3)].decl).node, (yyvsp[(3) - (3)].decl).node);
		(yyval.decl).num = (yyvsp[(1) - (3)].decl).num + (yyvsp[(3) - (3)].decl).num;
            }
    break;

  case 95:

    {
#line 1007 "grammar/grammar.y.pre"
		(yyvsp[(3) - (6)].decl).node->v.expr = (yyvsp[(5) - (6)].node);
		(yyvsp[(1) - (6)].number) = context;
		context = LOOP_CONTEXT | LOOP_FOREACH;
            }
    break;

  case 96:

    {
#line 1013 "grammar/grammar.y.pre"
		(yyval.decl).num = (yyvsp[(3) - (8)].decl).num;

		CREATE_STATEMENTS((yyval.decl).node, (yyvsp[(3) - (8)].decl).node, 0);
		CREATE_LOOP((yyval.decl).node->r.expr, 2, (yyvsp[(8) - (8)].node), 0, 0);
		CREATE_OPCODE((yyval.decl).node->r.expr->r.expr, F_NEXT_FOREACH, 0);

		context = (yyvsp[(1) - (8)].number);
	    }
    break;

  case 97:

    {
#line 1026 "grammar/grammar.y.pre"
		CREATE_NUMBER((yyval.node), 1);
	    }
    break;

  case 99:

    {
#line 1034 "grammar/grammar.y.pre"
	 	(yyval.decl).node = (yyvsp[(1) - (1)].node);
		(yyval.decl).num = 0;
	    }
    break;

  case 100:

    {
#line 1039 "grammar/grammar.y.pre"
		(yyval.decl).node = (yyvsp[(1) - (1)].node);
		(yyval.decl).num = 1;
	    }
    break;

  case 101:

    {
#line 1047 "grammar/grammar.y.pre"
                (yyvsp[(1) - (4)].number) = context;
                context &= LOOP_CONTEXT;
                context |= SWITCH_CONTEXT;
                (yyvsp[(2) - (4)].number) = mem_block[A_CASES].current_size;
            }
    break;

  case 102:

    {
#line 1054 "grammar/grammar.y.pre"
                parse_node_t *node1, *node2;

                if ((yyvsp[(9) - (10)].node)) {
		    CREATE_STATEMENTS(node1, (yyvsp[(8) - (10)].node), (yyvsp[(9) - (10)].node));
                } else node1 = (yyvsp[(8) - (10)].node);

                if (context & SWITCH_STRINGS) {
                    NODE_NO_LINE(node2, NODE_SWITCH_STRINGS);
                } else if (context & SWITCH_RANGES) {
		    NODE_NO_LINE(node2, NODE_SWITCH_RANGES);
		} else {
                    NODE_NO_LINE(node2, NODE_SWITCH_NUMBERS);
                }
                node2->l.expr = (yyvsp[(3) - (10)].node);
                node2->r.expr = node1;
                prepare_cases(node2, (yyvsp[(2) - (10)].number));
                context = (yyvsp[(1) - (10)].number);
		(yyval.node) = node2;
		pop_n_locals((yyvsp[(7) - (10)].decl).num);
            }
    break;

  case 103:

    {
#line 1079 "grammar/grammar.y.pre"
               if ((yyvsp[(2) - (2)].node)){
		   CREATE_STATEMENTS((yyval.node), (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node));
               } else (yyval.node) = (yyvsp[(1) - (2)].node);
           }
    break;

  case 104:

    {
#line 1085 "grammar/grammar.y.pre"
               if ((yyvsp[(2) - (2)].node)){
		   CREATE_STATEMENTS((yyval.node), (yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].node));
               } else (yyval.node) = (yyvsp[(1) - (2)].node);
           }
    break;

  case 105:

    {
#line 1091 "grammar/grammar.y.pre"
               (yyval.node) = 0;
           }
    break;

  case 106:

    {
#line 1099 "grammar/grammar.y.pre"
                (yyval.node) = (yyvsp[(2) - (3)].node);
                (yyval.node)->v.expr = 0;

                add_to_mem_block(A_CASES, (char *)&((yyvsp[(2) - (3)].node)), sizeof((yyvsp[(2) - (3)].node)));
            }
    break;

  case 107:

    {
#line 1106 "grammar/grammar.y.pre"
                if ( (yyvsp[(2) - (5)].node)->kind != NODE_CASE_NUMBER
                    || (yyvsp[(4) - (5)].node)->kind != NODE_CASE_NUMBER )
                    yyerror("String case labels not allowed as range bounds");
                if ((yyvsp[(2) - (5)].node)->r.number > (yyvsp[(4) - (5)].node)->r.number) break;

		context |= SWITCH_RANGES;

                (yyval.node) = (yyvsp[(2) - (5)].node);
                (yyval.node)->v.expr = (yyvsp[(4) - (5)].node);

                add_to_mem_block(A_CASES, (char *)&((yyvsp[(2) - (5)].node)), sizeof((yyvsp[(2) - (5)].node)));
            }
    break;

  case 108:

    {
#line 1120 "grammar/grammar.y.pre"
                if (context & SWITCH_DEFAULT) {
                    yyerror("Duplicate default");
                    (yyval.node) = 0;
                    break;
                }
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_DEFAULT;
                (yyval.node)->v.expr = 0;
                add_to_mem_block(A_CASES, (char *)&((yyval.node)), sizeof((yyval.node)));
                context |= SWITCH_DEFAULT;
            }
    break;

  case 109:

    {
#line 1136 "grammar/grammar.y.pre"
                if ((context & SWITCH_STRINGS) && (yyvsp[(1) - (1)].pointer_int))
                    yyerror("Mixed case label list not allowed");

                if ((yyvsp[(1) - (1)].pointer_int)) context |= SWITCH_NUMBERS;
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_CASE_NUMBER;
                (yyval.node)->r.expr = (parse_node_t *)(yyvsp[(1) - (1)].pointer_int);
            }
    break;

  case 110:

    {
#line 1146 "grammar/grammar.y.pre"
		int str;

		str = store_prog_string((yyvsp[(1) - (1)].string));
                scratch_free((yyvsp[(1) - (1)].string));
                if (context & SWITCH_NUMBERS)
                    yyerror("Mixed case label list not allowed");
                context |= SWITCH_STRINGS;
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_CASE_STRING;
                (yyval.node)->r.number = str;
            }
    break;

  case 111:

    {
#line 1162 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) | (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 112:

    {
#line 1166 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) ^ (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 113:

    {
#line 1170 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) & (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 114:

    {
#line 1174 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) == (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 115:

    {
#line 1178 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) != (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 116:

    {
#line 1182 "grammar/grammar.y.pre"
                switch((yyvsp[(2) - (3)].number)){
                    case F_GE: (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) >= (yyvsp[(3) - (3)].pointer_int); break;
                    case F_LE: (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) <= (yyvsp[(3) - (3)].pointer_int); break;
                    case F_GT: (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) >  (yyvsp[(3) - (3)].pointer_int); break;
                }
            }
    break;

  case 117:

    {
#line 1190 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) < (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 118:

    {
#line 1194 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) << (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 119:

    {
#line 1198 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) >> (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 120:

    {
#line 1202 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) + (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 121:

    {
#line 1206 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) - (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 122:

    {
#line 1210 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) * (yyvsp[(3) - (3)].pointer_int);
            }
    break;

  case 123:

    {
#line 1214 "grammar/grammar.y.pre"
                if ((yyvsp[(3) - (3)].pointer_int)) (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) % (yyvsp[(3) - (3)].pointer_int); else yyerror("Modulo by zero");
            }
    break;

  case 124:

    {
#line 1218 "grammar/grammar.y.pre"
                if ((yyvsp[(3) - (3)].pointer_int)) (yyval.pointer_int) = (yyvsp[(1) - (3)].pointer_int) / (yyvsp[(3) - (3)].pointer_int); else yyerror("Division by zero");
            }
    break;

  case 125:

    {
#line 1222 "grammar/grammar.y.pre"
                (yyval.pointer_int) = (yyvsp[(2) - (3)].pointer_int);
            }
    break;

  case 126:

    {
#line 1226 "grammar/grammar.y.pre"
		(yyval.pointer_int) = (yyvsp[(1) - (1)].number);
	    }
    break;

  case 127:

    {
#line 1230 "grammar/grammar.y.pre"
                (yyval.pointer_int) = -(yyvsp[(2) - (2)].number);
            }
    break;

  case 128:

    {
#line 1234 "grammar/grammar.y.pre"
                (yyval.pointer_int) = !(yyvsp[(2) - (2)].number);
            }
    break;

  case 129:

    {
#line 1238 "grammar/grammar.y.pre"
                (yyval.pointer_int) = ~(yyvsp[(2) - (2)].number);
            }
    break;

  case 130:

    {
#line 1245 "grammar/grammar.y.pre"
		(yyval.node) = (yyvsp[(1) - (1)].node);
	    }
    break;

  case 131:

    {
#line 1249 "grammar/grammar.y.pre"
		CREATE_TWO_VALUES((yyval.node), (yyvsp[(3) - (3)].node)->type, insert_pop_value((yyvsp[(1) - (3)].node)), (yyvsp[(3) - (3)].node));
	    }
    break;

  case 132:

    {
#line 1256 "grammar/grammar.y.pre"
	        parse_node_t *l = (yyvsp[(1) - (3)].node), *r = (yyvsp[(3) - (3)].node);
		/* set this up here so we can change it below */

		CREATE_BINARY_OP((yyval.node), (yyvsp[(2) - (3)].number), r->type, r, l);

		if (exact_types && !compatible_types(r->type, l->type) &&
		    !((yyvsp[(2) - (3)].number) == F_ADD_EQ
		      && r->type == TYPE_STRING &&
		      COMP_TYPE(l->type, TYPE_NUMBER))) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Bad assignment ");
		    p = get_two_types(p, end, l->type, r->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}

		#ifdef DBVAR_NOASSIGN
		if (DBVAR_NODE_GET(l)) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Bad dbvar assignment ");
		    p = get_two_types(p, end, l->type, r->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}
		#endif

		(yyval.node)->l.expr = do_promotions(r, l->type);
	    }
    break;

  case 133:

    {
#line 1292 "grammar/grammar.y.pre"
		yyerror("Illegal LHS");
		CREATE_ERROR((yyval.node));
	    }
    break;

  case 134:

    {
#line 1297 "grammar/grammar.y.pre"
		parse_node_t *p1 = (yyvsp[(3) - (5)].node), *p2 = (yyvsp[(5) - (5)].node);

		if (exact_types && !compatible_types2(p1->type, p2->type)) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Types in ?: do not match ");
		    p = get_two_types(p, end, p1->type, p2->type);
		    p = strput(p, end, ".");
		    yywarn(buf);
		}

		/* optimize if last expression did F_NOT */
		if (IS_NODE((yyvsp[(1) - (5)].node), NODE_UNARY_OP, F_NOT)) {
		    /* !a ? b : c  --> a ? c : b */
		    CREATE_IF((yyval.node), (yyvsp[(1) - (5)].node)->r.expr, p2, p1);
		} else {
		    CREATE_IF((yyval.node), (yyvsp[(1) - (5)].node), p1, p2);
		}
		(yyval.node)->type = ((p1->type == p2->type) ? p1->type : TYPE_ANY);
	    }
    break;

  case 135:

    {
#line 1321 "grammar/grammar.y.pre"
		CREATE_LAND_LOR((yyval.node), F_LOR, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		if (IS_NODE((yyvsp[(1) - (3)].node), NODE_LAND_LOR, F_LOR))
		    (yyvsp[(1) - (3)].node)->kind = NODE_BRANCH_LINK;
	    }
    break;

  case 136:

    {
#line 1327 "grammar/grammar.y.pre"
		CREATE_LAND_LOR((yyval.node), F_LAND, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		if (IS_NODE((yyvsp[(1) - (3)].node), NODE_LAND_LOR, F_LAND))
		    (yyvsp[(1) - (3)].node)->kind = NODE_BRANCH_LINK;
	    }
    break;

  case 137:

    {
#line 1333 "grammar/grammar.y.pre"
		if (is_boolean((yyvsp[(1) - (3)].node)) && is_boolean((yyvsp[(3) - (3)].node)))
		    yywarn("bitwise operation on boolean values.");
		(yyval.node) = binary_int_op((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), F_OR, "|");
	    }
    break;

  case 138:

    {
#line 1339 "grammar/grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), F_XOR, "^");
	    }
    break;

  case 139:

    {
#line 1343 "grammar/grammar.y.pre"
		int t1 = (yyvsp[(1) - (3)].node)->type, t3 = (yyvsp[(3) - (3)].node)->type;
		if (is_boolean((yyvsp[(1) - (3)].node)) && is_boolean((yyvsp[(3) - (3)].node)))
		    yywarn("bitwise operation on boolean values.");
		if ((t1 & TYPE_MOD_ARRAY) || (t3 & TYPE_MOD_ARRAY)) {
		    if (t1 != t3) {
			if ((t1 != TYPE_ANY) && (t3 != TYPE_ANY) &&
			    !(t1 & t3 & TYPE_MOD_ARRAY)) {
			    char buf[256];
			    char *end = EndOf(buf);
			    char *p;

			    p = strput(buf, end, "Incompatible types for & ");
			    p = get_two_types(p, end, t1, t3);
			    p = strput(p, end, ".");
			    yyerror(buf);
			}
			t1 = TYPE_ANY | TYPE_MOD_ARRAY;
		    }
		    CREATE_BINARY_OP((yyval.node), F_AND, t1, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		} else (yyval.node) = binary_int_op((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), F_AND, "&");
	    }
    break;

  case 140:

    {
#line 1366 "grammar/grammar.y.pre"
		if (exact_types && !compatible_types2((yyvsp[(1) - (3)].node)->type, (yyvsp[(3) - (3)].node)->type)){
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "== always false because of incompatible types ");
		    p = get_two_types(p, end, (yyvsp[(1) - (3)].node)->type, (yyvsp[(3) - (3)].node)->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}
		/* x == 0 -> !x */
		if (IS_NODE((yyvsp[(1) - (3)].node), NODE_NUMBER, 0)) {
		    CREATE_UNARY_OP((yyval.node), F_NOT, TYPE_NUMBER, (yyvsp[(3) - (3)].node));
		} else
		if (IS_NODE((yyvsp[(3) - (3)].node), NODE_NUMBER, 0)) {
		    CREATE_UNARY_OP((yyval.node), F_NOT, TYPE_NUMBER, (yyvsp[(1) - (3)].node));
		} else {
		    CREATE_BINARY_OP((yyval.node), F_EQ, TYPE_NUMBER, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		}
	    }
    break;

  case 141:

    {
#line 1388 "grammar/grammar.y.pre"
		if (exact_types && !compatible_types2((yyvsp[(1) - (3)].node)->type, (yyvsp[(3) - (3)].node)->type)){
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "!= always true because of incompatible types ");
		    p = get_two_types(p, end, (yyvsp[(1) - (3)].node)->type, (yyvsp[(3) - (3)].node)->type);
		    p = strput(p, end, ".");
		    yyerror(buf);
		}
                CREATE_BINARY_OP((yyval.node), F_NE, TYPE_NUMBER, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
	    }
    break;

  case 142:

    {
#line 1402 "grammar/grammar.y.pre"
		if (exact_types) {
		    int t1 = (yyvsp[(1) - (3)].node)->type;
		    int t3 = (yyvsp[(3) - (3)].node)->type;

		    if (!COMP_TYPE(t1, TYPE_NUMBER)
			&& !COMP_TYPE(t1, TYPE_STRING)) {
			char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Bad left argument to '");
			p = strput(p, end, get_f_name((yyvsp[(2) - (3)].number)));
			p = strput(p, end, "' : \"");
			p = get_type_name(p, end, t1);
			p = strput(p, end, "\"");
			yyerror(buf);
		    } else if (!COMP_TYPE(t3, TYPE_NUMBER)
			       && !COMP_TYPE(t3, TYPE_STRING)) {
                        char buf[256];
			char *end = EndOf(buf);
			char *p;

                        p = strput(buf, end, "Bad right argument to '");
                        p = strput(p, end, get_f_name((yyvsp[(2) - (3)].number)));
                        p = strput(p, end, "' : \"");
                        p = get_type_name(p, end, t3);
			p = strput(p, end, "\"");
			yyerror(buf);
		    } else if (!compatible_types2(t1,t3)) {
			char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Arguments to ");
			p = strput(p, end, get_f_name((yyvsp[(2) - (3)].number)));
			p = strput(p, end, " do not have compatible types : ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
		    }
		}
                CREATE_BINARY_OP((yyval.node), (yyvsp[(2) - (3)].number), TYPE_NUMBER, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
	    }
    break;

  case 143:

    {
#line 1446 "grammar/grammar.y.pre"
                if (exact_types) {
                    int t1 = (yyvsp[(1) - (3)].node)->type, t3 = (yyvsp[(3) - (3)].node)->type;

                    if (!COMP_TYPE(t1, TYPE_NUMBER)
			&& !COMP_TYPE(t1, TYPE_STRING)) {
                        char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Bad left argument to '<' : \"");
                        p = get_type_name(p, end, t1);
			p = strput(p, end, "\"");
                        yyerror(buf);
                    } else if (!COMP_TYPE(t3, TYPE_NUMBER)
			       && !COMP_TYPE(t3, TYPE_STRING)) {
                        char buf[200];
			char *end = EndOf(buf);
			char *p;

                        p = strput(buf, end, "Bad right argument to '<' : \"");
                        p = get_type_name(p, end, t3);
                        p = strput(p, end, "\"");
                        yyerror(buf);
                    } else if (!compatible_types2(t1,t3)) {
                        char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Arguments to < do not have compatible types : ");
			p = get_two_types(p, end, t1, t3);
                        yyerror(buf);
                    }
                }
                CREATE_BINARY_OP((yyval.node), F_LT, TYPE_NUMBER, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
            }
    break;

  case 144:

    {
#line 1483 "grammar/grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), F_LSH, "<<");
	    }
    break;

  case 145:

    {
#line 1487 "grammar/grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), F_RSH, ">>");
	    }
    break;

  case 146:

    {
#line 1491 "grammar/grammar.y.pre"
		int result_type;

		if (exact_types) {
		    int t1 = (yyvsp[(1) - (3)].node)->type, t3 = (yyvsp[(3) - (3)].node)->type;

		    if (t1 == t3){
#ifdef CAST_CALL_OTHERS
			if (t1 == TYPE_UNKNOWN){
			    yyerror("Bad arguments to '+' (unknown vs unknown)");
			    result_type = TYPE_ANY;
			} else
#endif
			    result_type = t1;
		    }
		    else if (t1 == TYPE_ANY) {
			if (t3 == TYPE_FUNCTION) {
			    yyerror("Bad right argument to '+' (function)");
			    result_type = TYPE_ANY;
			} else result_type = t3;
		    } else if (t3 == TYPE_ANY) {
			if (t1 == TYPE_FUNCTION) {
			    yyerror("Bad left argument to '+' (function)");
			    result_type = TYPE_ANY;
			} else result_type = t1;
		    } else {
			switch(t1) {
			    case TYPE_STRING:
			    {
				if (t3 == TYPE_REAL || t3 == TYPE_NUMBER){
				    result_type = TYPE_STRING;
				} else goto add_error;
				break;
			    }
			    case TYPE_NUMBER:
			    {
				if (t3 == TYPE_REAL || t3 == TYPE_STRING)
				    result_type = t3;
				else goto add_error;
				break;
			    }
			case TYPE_REAL:
			    {
				if (t3 == TYPE_NUMBER) result_type = TYPE_REAL;
				else if (t3 == TYPE_STRING) result_type = TYPE_STRING;
				else goto add_error;
				break;
			    }
			    default:
			    {
				if (t1 & t3 & TYPE_MOD_ARRAY) {
				    result_type = TYPE_ANY|TYPE_MOD_ARRAY;
				    break;
				}
add_error:
				{
				    char buf[256];
				    char *end = EndOf(buf);
				    char *p;

				    p = strput(buf, end, "Invalid argument types to '+' ");
				    p = get_two_types(p, end, t1, t3);
				    yyerror(buf);
				    result_type = TYPE_ANY;
				}
			    }
			}
		    }
		} else
		    result_type = TYPE_ANY;

		switch ((yyvsp[(1) - (3)].node)->kind) {
		case NODE_NUMBER:
		    /* 0 + X */
		    if ((yyvsp[(1) - (3)].node)->v.number == 0 &&
			((yyvsp[(3) - (3)].node)->type == TYPE_NUMBER || (yyvsp[(3) - (3)].node)->type == TYPE_REAL)) {
			(yyval.node) = (yyvsp[(3) - (3)].node);
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.number += (yyvsp[(3) - (3)].node)->v.number;
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[(3) - (3)].node);
			(yyvsp[(3) - (3)].node)->v.real += (yyvsp[(1) - (3)].node)->v.number;
			break;
		    }
		    /* swapping the nodes may help later constant folding */
		    if ((yyvsp[(3) - (3)].node)->type != TYPE_STRING && (yyvsp[(3) - (3)].node)->type != TYPE_ANY)
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[(3) - (3)].node), (yyvsp[(1) - (3)].node));
		    else
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    break;
		case NODE_REAL:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real += (yyvsp[(3) - (3)].node)->v.number;
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real += (yyvsp[(3) - (3)].node)->v.real;
			break;
		    }
		    /* swapping the nodes may help later constant folding */
		    if ((yyvsp[(3) - (3)].node)->type != TYPE_STRING && (yyvsp[(3) - (3)].node)->type != TYPE_ANY)
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[(3) - (3)].node), (yyvsp[(1) - (3)].node));
		    else
			CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    break;
		case NODE_STRING:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_STRING) {
			/* Combine strings */
			int n1, n2;
			char *new, *s1, *s2;
			int l;

			n1 = (yyvsp[(1) - (3)].node)->v.number;
			n2 = (yyvsp[(3) - (3)].node)->v.number;
			s1 = PROG_STRING(n1);
			s2 = PROG_STRING(n2);
			new = (char *)DXALLOC( (l = strlen(s1))+strlen(s2)+1, TAG_COMPILER, "combine string" );
			strcpy(new, s1);
			strcat(new + l, s2);
			/* free old strings (ordering may help shrink table) */
			if (n1 > n2) {
			    free_prog_string(n1); free_prog_string(n2);
			} else {
			    free_prog_string(n2); free_prog_string(n1);
			}
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyval.node)->v.number = store_prog_string(new);
			FREE(new);
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    break;
		default:
		    /* X + 0 */
		    if (IS_NODE((yyvsp[(3) - (3)].node), NODE_NUMBER, 0) &&
			((yyvsp[(1) - (3)].node)->type == TYPE_NUMBER || (yyvsp[(1) - (3)].node)->type == TYPE_REAL)) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_ADD, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    break;
		}
	    }
    break;

  case 147:

    {
#line 1642 "grammar/grammar.y.pre"
		int result_type;

		if (exact_types) {
		    int t1 = (yyvsp[(1) - (3)].node)->type, t3 = (yyvsp[(3) - (3)].node)->type;

		    if (t1 == t3){
			switch(t1){
			    case TYPE_ANY:
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			        result_type = t1;
				break;
			    default:
				if (!(t1 & TYPE_MOD_ARRAY)){
				    type_error("Bad argument number 1 to '-'", t1);
				    result_type = TYPE_ANY;
				} else result_type = t1;
			}
		    } else if (t1 == TYPE_ANY){
			switch(t3){
			    case TYPE_REAL:
			    case TYPE_NUMBER:
			        result_type = t3;
				break;
			    default:
				if (!(t3 & TYPE_MOD_ARRAY)){
				    type_error("Bad argument number 2 to '-'", t3);
				    result_type = TYPE_ANY;
				} else result_type = t3;
			}
		    } else if (t3 == TYPE_ANY){
			switch(t1){
			    case TYPE_REAL:
			    case TYPE_NUMBER:
			        result_type = t1;
				break;
			    default:
				if (!(t1 & TYPE_MOD_ARRAY)){
				    type_error("Bad argument number 1 to '-'", t1);
				    result_type = TYPE_ANY;
				} else result_type = t1;
			}
		    } else if ((t1 == TYPE_REAL && t3 == TYPE_NUMBER) ||
			       (t3 == TYPE_REAL && t1 == TYPE_NUMBER)){
			result_type = TYPE_REAL;
		    } else if (t1 & t3 & TYPE_MOD_ARRAY){
			result_type = TYPE_MOD_ARRAY|TYPE_ANY;
		    } else {
			char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Invalid types to '-' ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
			result_type = TYPE_ANY;
		    }
		} else result_type = TYPE_ANY;

		switch ((yyvsp[(1) - (3)].node)->kind) {
		case NODE_NUMBER:
		    if ((yyvsp[(1) - (3)].node)->v.number == 0) {
			CREATE_UNARY_OP((yyval.node), F_NEGATE, (yyvsp[(3) - (3)].node)->type, (yyvsp[(3) - (3)].node));
		    } else if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.number -= (yyvsp[(3) - (3)].node)->v.number;
		    } else if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[(3) - (3)].node);
			(yyvsp[(3) - (3)].node)->v.real = (yyvsp[(1) - (3)].node)->v.number - (yyvsp[(3) - (3)].node)->v.real;
		    } else {
			CREATE_BINARY_OP((yyval.node), F_SUBTRACT, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    }
		    break;
		case NODE_REAL:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real -= (yyvsp[(3) - (3)].node)->v.number;
		    } else if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real -= (yyvsp[(3) - (3)].node)->v.real;
		    } else {
			CREATE_BINARY_OP((yyval.node), F_SUBTRACT, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    }
		    break;
		default:
		    /* optimize X-0 */
		    if (IS_NODE((yyvsp[(3) - (3)].node), NODE_NUMBER, 0)) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
		    }
		    CREATE_BINARY_OP((yyval.node), F_SUBTRACT, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		}
	    }
    break;

  case 148:

    {
#line 1736 "grammar/grammar.y.pre"
		int result_type;

		if (exact_types){
		    int t1 = (yyvsp[(1) - (3)].node)->type, t3 = (yyvsp[(3) - (3)].node)->type;

		    if (t1 == t3){
			switch(t1){
			    case TYPE_MAPPING:
			    case TYPE_ANY:
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			        result_type = t1;
				break;
			default:
				type_error("Bad argument number 1 to '*'", t1);
				result_type = TYPE_ANY;
			}
		    } else if (t1 == TYPE_ANY || t3 == TYPE_ANY){
			int t = (t1 == TYPE_ANY) ? t3 : t1;
			switch(t){
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			    case TYPE_MAPPING:
			        result_type = t;
				break;
			    default:
				type_error((t1 == TYPE_ANY) ?
					   "Bad argument number 2 to '*'" :
					   "Bad argument number 1 to '*'",
					   t);
				result_type = TYPE_ANY;
			}
		    } else if ((t1 == TYPE_NUMBER && t3 == TYPE_REAL) ||
			       (t1 == TYPE_REAL && t3 == TYPE_NUMBER)){
			result_type = TYPE_REAL;
		    } else {
			char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Invalid types to '*' ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
			result_type = TYPE_ANY;
		    }
		} else result_type = TYPE_ANY;

		switch ((yyvsp[(1) - (3)].node)->kind) {
		case NODE_NUMBER:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyval.node)->v.number *= (yyvsp[(3) - (3)].node)->v.number;
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[(3) - (3)].node);
			(yyvsp[(3) - (3)].node)->v.real *= (yyvsp[(1) - (3)].node)->v.number;
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_MULTIPLY, result_type, (yyvsp[(3) - (3)].node), (yyvsp[(1) - (3)].node));
		    break;
		case NODE_REAL:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real *= (yyvsp[(3) - (3)].node)->v.number;
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real *= (yyvsp[(3) - (3)].node)->v.real;
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_MULTIPLY, result_type, (yyvsp[(3) - (3)].node), (yyvsp[(1) - (3)].node));
		    break;
		default:
		    CREATE_BINARY_OP((yyval.node), F_MULTIPLY, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		}
	    }
    break;

  case 149:

    {
#line 1816 "grammar/grammar.y.pre"
		(yyval.node) = binary_int_op((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), F_MOD, "%");
	    }
    break;

  case 150:

    {
#line 1820 "grammar/grammar.y.pre"
		int result_type;

		if (exact_types){
		    int t1 = (yyvsp[(1) - (3)].node)->type, t3 = (yyvsp[(3) - (3)].node)->type;

		    if (t1 == t3){
			switch(t1){
			    case TYPE_NUMBER:
			    case TYPE_REAL:
			case TYPE_ANY:
			        result_type = t1;
				break;
			    default:
				type_error("Bad argument 1 to '/'", t1);
				result_type = TYPE_ANY;
			}
		    } else if (t1 == TYPE_ANY || t3 == TYPE_ANY){
			int t = (t1 == TYPE_ANY) ? t3 : t1;
			if (t == TYPE_REAL || t == TYPE_NUMBER)
			    result_type = t;
			else {
			    type_error(t1 == TYPE_ANY ?
				       "Bad argument 2 to '/'" :
				       "Bad argument 1 to '/'", t);
			    result_type = TYPE_ANY;
			}
		    } else if ((t1 == TYPE_NUMBER && t3 == TYPE_REAL) ||
			       (t1 == TYPE_REAL && t3 == TYPE_NUMBER)) {
			result_type = TYPE_REAL;
		    } else {
			char buf[256];
			char *end = EndOf(buf);
			char *p;

			p = strput(buf, end, "Invalid types to '/' ");
			p = get_two_types(p, end, t1, t3);
			yyerror(buf);
			result_type = TYPE_ANY;
		    }
		} else result_type = TYPE_ANY;

		/* constant expressions */
		switch ((yyvsp[(1) - (3)].node)->kind) {
		case NODE_NUMBER:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			if ((yyvsp[(3) - (3)].node)->v.number == 0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[(1) - (3)].node);
			    break;
			}
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.number /= (yyvsp[(3) - (3)].node)->v.number;
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			if ((yyvsp[(3) - (3)].node)->v.real == 0.0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[(1) - (3)].node);
			    break;
			}
			(yyval.node) = (yyvsp[(3) - (3)].node);
			(yyvsp[(3) - (3)].node)->v.real = ((yyvsp[(1) - (3)].node)->v.number / (yyvsp[(3) - (3)].node)->v.real);
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_DIVIDE, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    break;
		case NODE_REAL:
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_NUMBER) {
			if ((yyvsp[(3) - (3)].node)->v.number == 0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[(1) - (3)].node);
			    break;
			}
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real /= (yyvsp[(3) - (3)].node)->v.number;
			break;
		    }
		    if ((yyvsp[(3) - (3)].node)->kind == NODE_REAL) {
			if ((yyvsp[(3) - (3)].node)->v.real == 0.0) {
			    yyerror("Divide by zero in constant");
			    (yyval.node) = (yyvsp[(1) - (3)].node);
			    break;
			}
			(yyval.node) = (yyvsp[(1) - (3)].node);
			(yyvsp[(1) - (3)].node)->v.real /= (yyvsp[(3) - (3)].node)->v.real;
			break;
		    }
		    CREATE_BINARY_OP((yyval.node), F_DIVIDE, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		    break;
		default:
		    CREATE_BINARY_OP((yyval.node), F_DIVIDE, result_type, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
		}
	    }
    break;

  case 151:

    {
#line 1915 "grammar/grammar.y.pre"
		(yyval.node) = (yyvsp[(2) - (2)].node);
		(yyval.node)->type = (yyvsp[(1) - (2)].type);

		if (exact_types &&
		    (yyvsp[(2) - (2)].node)->type != (yyvsp[(1) - (2)].type) &&
		    (yyvsp[(2) - (2)].node)->type != TYPE_ANY &&
		    (yyvsp[(2) - (2)].node)->type != TYPE_UNKNOWN &&
		    (yyvsp[(1) - (2)].type) != TYPE_VOID) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Cannot cast ");
		    p = get_type_name(p, end, (yyvsp[(2) - (2)].node)->type);
		    p = strput(p, end, "to ");
		    p = get_type_name(p, end, (yyvsp[(1) - (2)].type));
		    yyerror(buf);
		}
	    }
    break;

  case 152:

    {
#line 1936 "grammar/grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_PRE_INC, 0, (yyvsp[(2) - (2)].node));
                if (exact_types){
                    switch((yyvsp[(2) - (2)].node)->type){
                        case TYPE_NUMBER:
                        case TYPE_ANY:
                        case TYPE_REAL:
                        {
                            (yyval.node)->type = (yyvsp[(2) - (2)].node)->type;
                            break;
                        }

                        default:
                        {
                            (yyval.node)->type = TYPE_ANY;
                            type_error("Bad argument 1 to ++x", (yyvsp[(2) - (2)].node)->type);
                        }
                    }
                } else (yyval.node)->type = TYPE_ANY;
	    }
    break;

  case 153:

    {
#line 1957 "grammar/grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_PRE_DEC, 0, (yyvsp[(2) - (2)].node));
                if (exact_types){
                    switch((yyvsp[(2) - (2)].node)->type){
                        case TYPE_NUMBER:
                        case TYPE_ANY:
                        case TYPE_REAL:
                        {
                            (yyval.node)->type = (yyvsp[(2) - (2)].node)->type;
                            break;
                        }

                        default:
                        {
                            (yyval.node)->type = TYPE_ANY;
                            type_error("Bad argument 1 to --x", (yyvsp[(2) - (2)].node)->type);
                        }
                    }
                } else (yyval.node)->type = TYPE_ANY;

	    }
    break;

  case 154:

    {
#line 1979 "grammar/grammar.y.pre"
		if ((yyvsp[(2) - (2)].node)->kind == NODE_NUMBER) {
		    (yyval.node) = (yyvsp[(2) - (2)].node);
		    (yyval.node)->v.number = !((yyval.node)->v.number);
		} else {
		    CREATE_UNARY_OP((yyval.node), F_NOT, TYPE_NUMBER, (yyvsp[(2) - (2)].node));
		}
	    }
    break;

  case 155:

    {
#line 1988 "grammar/grammar.y.pre"
		if (exact_types && !IS_TYPE((yyvsp[(2) - (2)].node)->type, TYPE_NUMBER))
		    type_error("Bad argument to ~", (yyvsp[(2) - (2)].node)->type);
		if ((yyvsp[(2) - (2)].node)->kind == NODE_NUMBER) {
		    (yyval.node) = (yyvsp[(2) - (2)].node);
		    (yyval.node)->v.number = ~(yyval.node)->v.number;
		} else {
		    CREATE_UNARY_OP((yyval.node), F_COMPL, TYPE_NUMBER, (yyvsp[(2) - (2)].node));
		}
	    }
    break;

  case 156:

    {
#line 1999 "grammar/grammar.y.pre"
		int result_type;
                if (exact_types){
		    int t = (yyvsp[(2) - (2)].node)->type;
		    if (!COMP_TYPE(t, TYPE_NUMBER)){
			type_error("Bad argument to unary '-'", t);
			result_type = TYPE_ANY;
		    } else result_type = t;
		} else result_type = TYPE_ANY;

		switch ((yyvsp[(2) - (2)].node)->kind) {
		case NODE_NUMBER:
		    (yyval.node) = (yyvsp[(2) - (2)].node);
		    (yyval.node)->v.number = -(yyval.node)->v.number;
		    break;
		case NODE_REAL:
		    (yyval.node) = (yyvsp[(2) - (2)].node);
		    (yyval.node)->v.real = -(yyval.node)->v.real;
		    break;
		default:
		    CREATE_UNARY_OP((yyval.node), F_NEGATE, result_type, (yyvsp[(2) - (2)].node));
		}
	    }
    break;

  case 157:

    {
#line 2023 "grammar/grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_POST_INC, 0, (yyvsp[(1) - (2)].node));
		(yyval.node)->v.number = F_POST_INC;
                if (exact_types){
                    switch((yyvsp[(1) - (2)].node)->type){
                        case TYPE_NUMBER:
		    case TYPE_ANY:
                        case TYPE_REAL:
                        {
                            (yyval.node)->type = (yyvsp[(1) - (2)].node)->type;
                            break;
                        }

                        default:
                        {
                            (yyval.node)->type = TYPE_ANY;
                            type_error("Bad argument 1 to x++", (yyvsp[(1) - (2)].node)->type);
                        }
                    }
                } else (yyval.node)->type = TYPE_ANY;
	    }
    break;

  case 158:

    {
#line 2045 "grammar/grammar.y.pre"
		CREATE_UNARY_OP((yyval.node), F_POST_DEC, 0, (yyvsp[(1) - (2)].node));
                if (exact_types){
                    switch((yyvsp[(1) - (2)].node)->type){
		    case TYPE_NUMBER:
		    case TYPE_ANY:
		    case TYPE_REAL:
		    {
			(yyval.node)->type = (yyvsp[(1) - (2)].node)->type;
			break;
		    }

		    default:
		    {
			(yyval.node)->type = TYPE_ANY;
			type_error("Bad argument 1 to x--", (yyvsp[(1) - (2)].node)->type);
		    }
                    }
                } else (yyval.node)->type = TYPE_ANY;
	    }
    break;

  case 164:

    {
#line 2074 "grammar/grammar.y.pre"
		if (exact_types && !IS_TYPE(exact_types, TYPE_VOID))
		    yywarn("Non-void functions must return a value.");
		CREATE_RETURN((yyval.node), 0);
	    }
    break;

  case 165:

    {
#line 2080 "grammar/grammar.y.pre"
		if (exact_types && !compatible_types((yyvsp[(2) - (3)].node)->type, exact_types & ~NAME_TYPE_MOD)) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Type of returned value doesn't match function return type ");
		    p = get_two_types(p, end, (yyvsp[(2) - (3)].node)->type, exact_types & ~NAME_TYPE_MOD);
		    yyerror(buf);
		}
		if (IS_NODE((yyvsp[(2) - (3)].node), NODE_NUMBER, 0)) {
		    CREATE_RETURN((yyval.node), 0);
		} else {
		    CREATE_RETURN((yyval.node), (yyvsp[(2) - (3)].node));
		}
	    }
    break;

  case 166:

    {
#line 2100 "grammar/grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), 0);
	    }
    break;

  case 167:

    {
#line 2104 "grammar/grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[(1) - (1)].node));
	    }
    break;

  case 168:

    {
#line 2108 "grammar/grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[(1) - (2)].node));
	    }
    break;

  case 169:

    {
#line 2115 "grammar/grammar.y.pre"
		CREATE_EXPR_NODE((yyval.node), (yyvsp[(1) - (1)].node), 0);
	    }
    break;

  case 170:

    {
#line 2119 "grammar/grammar.y.pre"
		CREATE_EXPR_NODE((yyval.node), (yyvsp[(1) - (2)].node), 1);
	    }
    break;

  case 171:

    {
#line 2126 "grammar/grammar.y.pre"
		(yyvsp[(1) - (1)].node)->kind = 1;

		(yyval.node) = (yyvsp[(1) - (1)].node);
	    }
    break;

  case 172:

    {
#line 2132 "grammar/grammar.y.pre"
		(yyvsp[(3) - (3)].node)->kind = 0;

		(yyval.node) = (yyvsp[(1) - (3)].node);
		(yyval.node)->kind++;
		(yyval.node)->l.expr->r.expr = (yyvsp[(3) - (3)].node);
		(yyval.node)->l.expr = (yyvsp[(3) - (3)].node);
	    }
    break;

  case 173:

    {
#line 2144 "grammar/grammar.y.pre"
		/* this is a dummy node */
		CREATE_EXPR_LIST((yyval.node), 0);
	    }
    break;

  case 174:

    {
#line 2149 "grammar/grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[(1) - (1)].node));
	    }
    break;

  case 175:

    {
#line 2153 "grammar/grammar.y.pre"
		CREATE_EXPR_LIST((yyval.node), (yyvsp[(1) - (2)].node));
	    }
    break;

  case 176:

    {
#line 2160 "grammar/grammar.y.pre"
		(yyval.node) = new_node_no_line();
		(yyval.node)->kind = 2;
		(yyval.node)->v.expr = (yyvsp[(1) - (1)].node);
		(yyval.node)->r.expr = 0;
		(yyval.node)->type = 0;
		/* we keep track of the end of the chain in the left nodes */
		(yyval.node)->l.expr = (yyval.node);
            }
    break;

  case 177:

    {
#line 2170 "grammar/grammar.y.pre"
		parse_node_t *expr;

		expr = new_node_no_line();
		expr->kind = 0;
		expr->v.expr = (yyvsp[(3) - (3)].node);
		expr->r.expr = 0;
		expr->type = 0;

		(yyvsp[(1) - (3)].node)->l.expr->r.expr = expr;
		(yyvsp[(1) - (3)].node)->l.expr = expr;
		(yyvsp[(1) - (3)].node)->kind += 2;
		(yyval.node) = (yyvsp[(1) - (3)].node);
	    }
    break;

  case 178:

    {
#line 2188 "grammar/grammar.y.pre"
		CREATE_TWO_VALUES((yyval.node), 0, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
            }
    break;

  case 179:

    {
#line 2195 "grammar/grammar.y.pre"
#define LV_ILLEGAL 1
#define LV_RANGE 2
#define LV_INDEX 4
                /* Restrictive lvalues, but I think they make more sense :) */
                (yyval.node) = (yyvsp[(1) - (1)].node);
                switch((yyval.node)->kind) {
		default:
		    yyerror("Illegal lvalue");
		    break;
		case NODE_PARAMETER:
		    (yyval.node)->kind = NODE_PARAMETER_LVALUE;
		    break;
		case NODE_TERNARY_OP:
		    (yyval.node)->v.number = (yyval.node)->r.expr->v.number;
		case NODE_OPCODE_1:
		case NODE_UNARY_OP_1:
		case NODE_BINARY_OP:
		    if ((yyval.node)->v.number >= F_LOCAL && (yyval.node)->v.number <= F_MEMBER)
			(yyval.node)->v.number++; /* make it an lvalue */
		    else if ((yyval.node)->v.number >= F_INDEX
			     && (yyval.node)->v.number <= F_RE_RANGE) {
                        parse_node_t *node = (yyval.node);
                        int flag = 0;
                        do {
                            switch(node->kind) {
			    case NODE_PARAMETER:
				node->kind = NODE_PARAMETER_LVALUE;
				flag |= LV_ILLEGAL;
				break;
			    case NODE_TERNARY_OP:
				node->v.number = node->r.expr->v.number;
			    case NODE_OPCODE_1:
			    case NODE_UNARY_OP_1:
			    case NODE_BINARY_OP:
				if (node->v.number >= F_LOCAL
				    && node->v.number <= F_MEMBER) {
				    node->v.number++;
				    flag |= LV_ILLEGAL;
				    break;
				} else if (node->v.number == F_INDEX ||
					 node->v.number == F_RINDEX) {
				    node->v.number++;
				    flag |= LV_INDEX;
				    break;
				} else if (node->v.number >= F_ADD_EQ
					   && node->v.number <= F_ASSIGN) {
				    if (!(flag & LV_INDEX)) {
					yyerror("Illegal lvalue, a possible lvalue is (x <assign> y)[a]");
				    }
				    if (node->r.expr->kind == NODE_BINARY_OP||
					node->r.expr->kind == NODE_TERNARY_OP){
					if (node->r.expr->v.number >= F_NN_RANGE_LVALUE && node->r.expr->v.number <= F_NR_RANGE_LVALUE)
					    yyerror("Illegal to have (x[a..b] <assign> y) to be the beginning of an lvalue");
				    }
				    flag = LV_ILLEGAL;
				    break;
				} else if (node->v.number >= F_NN_RANGE
					 && node->v.number <= F_RE_RANGE) {
				    if (flag & LV_RANGE) {
					yyerror("Can't do range lvalue of range lvalue.");
					flag |= LV_ILLEGAL;
					break;
				    }
					if (flag & LV_INDEX){
					yyerror("Can't do indexed lvalue of range lvalue.");
					flag |= LV_ILLEGAL;
					break;
				    }
				    if (node->v.number == F_NE_RANGE) {
					/* x[foo..] -> x[foo..<1] */
					parse_node_t *rchild = node->r.expr;
					node->kind = NODE_TERNARY_OP;
					CREATE_BINARY_OP(node->r.expr,
							 F_NR_RANGE_LVALUE,
							 0, 0, rchild);
					CREATE_NUMBER(node->r.expr->l.expr, 1);
				    } else if (node->v.number == F_RE_RANGE) {
					/* x[<foo..] -> x[<foo..<1] */
					parse_node_t *rchild = node->r.expr;
					node->kind = NODE_TERNARY_OP;
					CREATE_BINARY_OP(node->r.expr,
							 F_RR_RANGE_LVALUE,
							 0, 0, rchild);
					CREATE_NUMBER(node->r.expr->l.expr, 1);
				    } else
					node->r.expr->v.number++;
				    flag |= LV_RANGE;
				    node = node->r.expr->r.expr;
				    continue;
				}
			    default:
				yyerror("Illegal lvalue");
				flag = LV_ILLEGAL;
				break;
			    }
					if ((flag & LV_ILLEGAL) || !(node = node->r.expr)) break;
                        } while (1);
                        break;
		    } else
			yyerror("Illegal lvalue");
		    break;
                }
            }
    break;

  case 181:

    {
#line 2305 "grammar/grammar.y.pre"
              int i;
              if ((i = (yyvsp[(1) - (1)].ihe)->dn.local_num) != -1) {
		  CREATE_OPCODE_1((yyval.node), F_LOCAL, type_of_locals_ptr[i],i & 0xff);
		  if (current_function_context)
		      current_function_context->num_locals++;
              } else
		  if ((i = (yyvsp[(1) - (1)].ihe)->dn.global_num) != -1) {
		      if (current_function_context)
			  current_function_context->bindable = FP_NOT_BINDABLE;
                          CREATE_OPCODE_1((yyval.node), F_GLOBAL,
				      VAR_TEMP(i)->type & ~NAME_TYPE_MOD, i);
			#ifdef DBVAR_NOASSIGN
			DBVAR_NODE_SET((yyval.node), VAR_TEMP(i)->type);
			#endif
		      if (VAR_TEMP(i)->type & NAME_HIDDEN) {
			  char buf[256];
			  char *end = EndOf(buf);
			  char *p;

			  p = strput(buf, end, "Illegal to use private variable '");
			  p = strput(p, end, (yyvsp[(1) - (1)].ihe)->name);
			  p = strput(p, end, "'");
			  yyerror(buf);
		      }
		  } else {
		      char buf[256];
		      char *end = EndOf(buf);
		      char *p;

		      p = strput(buf, end, "Undefined variable '");
		      p = strput(p, end, (yyvsp[(1) - (1)].ihe)->name);
		      p = strput(p, end, "'");
		      if (current_number_of_locals < CFG_MAX_LOCAL_VARIABLES) {
			  add_local_name((yyvsp[(1) - (1)].ihe)->name, TYPE_ANY);
		      }
		      CREATE_OPCODE_1((yyval.node), F_LOCAL, TYPE_ANY, 0);
		      yyerror(buf);
		  }
	    }
    break;

  case 182:

    {
#line 2346 "grammar/grammar.y.pre"
		char buf[256];
		char *end = EndOf(buf);
		char *p;

		p = strput(buf, end, "Undefined variable '");
		p = strput(p, end, (yyvsp[(1) - (1)].string));
		p = strput(p, end, "'");
                if (current_number_of_locals < CFG_MAX_LOCAL_VARIABLES) {
                    add_local_name((yyvsp[(1) - (1)].string), TYPE_ANY);
                }
                CREATE_OPCODE_1((yyval.node), F_LOCAL, TYPE_ANY, 0);
                yyerror(buf);
                scratch_free((yyvsp[(1) - (1)].string));
            }
    break;

  case 183:

    {
#line 2362 "grammar/grammar.y.pre"
		CREATE_PARAMETER((yyval.node), TYPE_ANY, (yyvsp[(1) - (1)].number));
            }
    break;

  case 184:

    {
#line 2366 "grammar/grammar.y.pre"
		(yyval.contextp) = current_function_context;
		/* already flagged as an error */
		if (current_function_context)
		    current_function_context = current_function_context->parent;
            }
    break;

  case 185:

    {
#line 2373 "grammar/grammar.y.pre"
		parse_node_t *node;

		current_function_context = (yyvsp[(3) - (5)].contextp);

		if (!current_function_context || current_function_context->num_parameters == -2) {
		    /* This was illegal, and error'ed when the '$' token
		     * was returned.
		     */
		    CREATE_ERROR((yyval.node));
		} else {
		    CREATE_OPCODE_1((yyval.node), F_LOCAL, (yyvsp[(4) - (5)].node)->type,
				    current_function_context->values_list->kind++);

		    node = new_node_no_line();
		    node->type = 0;
		    current_function_context->values_list->l.expr->r.expr = node;
		    current_function_context->values_list->l.expr = node;
		    node->r.expr = 0;
		    node->v.expr = (yyvsp[(4) - (5)].node);
		}
	    }
    break;

  case 186:

    {
#line 2396 "grammar/grammar.y.pre"
		if (!IS_CLASS((yyvsp[(1) - (3)].node)->type)) {
		    yyerror("Left argument of -> is not a class");
		    CREATE_ERROR((yyval.node));
		} else {
		    CREATE_UNARY_OP_1((yyval.node), F_MEMBER, 0, (yyvsp[(1) - (3)].node), 0);
		    (yyval.node)->l.number = lookup_class_member(CLASS_IDX((yyvsp[(1) - (3)].node)->type),
						       (yyvsp[(3) - (3)].string),
						       &((yyval.node)->type));
		}
		scratch_free((yyvsp[(3) - (3)].string));
            }
    break;

  case 187:

    {
#line 2409 "grammar/grammar.y.pre"
			if ((yyvsp[(1) - (6)].node)->type != TYPE_MAPPING &&
               (yyvsp[(5) - (6)].node)->kind == NODE_NUMBER && (yyvsp[(5) - (6)].node)->v.number < 0)
		    yywarn("00 A negative constant as the second element of arr[x..y] no longer means indexing from the end.  Use arr[x..<y]");
                (yyval.node) = make_range_node(F_NN_RANGE, (yyvsp[(1) - (6)].node), (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node));
            }
    break;

  case 188:

    {
#line 2418 "grammar/grammar.y.pre"
                (yyval.node) = make_range_node(F_RN_RANGE, (yyvsp[(1) - (7)].node), (yyvsp[(4) - (7)].node), (yyvsp[(6) - (7)].node));
            }
    break;

  case 189:

    {
#line 2422 "grammar/grammar.y.pre"
		if ((yyvsp[(7) - (8)].node)->kind == NODE_NUMBER && (yyvsp[(7) - (8)].node)->v.number <= 1)
		    (yyval.node) = make_range_node(F_RE_RANGE, (yyvsp[(1) - (8)].node), (yyvsp[(4) - (8)].node), 0);
		else
		    (yyval.node) = make_range_node(F_RR_RANGE, (yyvsp[(1) - (8)].node), (yyvsp[(4) - (8)].node), (yyvsp[(7) - (8)].node));
            }
    break;

  case 190:

    {
#line 2429 "grammar/grammar.y.pre"
		if ((yyvsp[(6) - (7)].node)->kind == NODE_NUMBER && (yyvsp[(6) - (7)].node)->v.number <= 1)
		    (yyval.node) = make_range_node(F_NE_RANGE, (yyvsp[(1) - (7)].node), (yyvsp[(3) - (7)].node), 0);
		else
		    (yyval.node) = make_range_node(F_NR_RANGE, (yyvsp[(1) - (7)].node), (yyvsp[(3) - (7)].node), (yyvsp[(6) - (7)].node));
            }
    break;

  case 191:

    {
#line 2436 "grammar/grammar.y.pre"
                (yyval.node) = make_range_node(F_NE_RANGE, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), 0);
            }
    break;

  case 192:

    {
#line 2440 "grammar/grammar.y.pre"
                (yyval.node) = make_range_node(F_RE_RANGE, (yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].node), 0);
            }
    break;

  case 193:

    {
#line 2444 "grammar/grammar.y.pre"
                if (IS_NODE((yyvsp[(1) - (5)].node), NODE_CALL, F_AGGREGATE)
		    && (yyvsp[(4) - (5)].node)->kind == NODE_NUMBER) {
                    int i = (yyvsp[(4) - (5)].node)->v.number;
                    if (i < 1 || i > (yyvsp[(1) - (5)].node)->l.number)
                        yyerror("Illegal index to array constant.");
                    else {
                        parse_node_t *node = (yyvsp[(1) - (5)].node)->r.expr;
                        i = (yyvsp[(1) - (5)].node)->l.number - i;
                        while (i--)
                            node = node->r.expr;
                        (yyval.node) = node->v.expr;
                        break;
                    }
                }
		CREATE_BINARY_OP((yyval.node), F_RINDEX, 0, (yyvsp[(4) - (5)].node), (yyvsp[(1) - (5)].node));
                if (exact_types) {
		    switch((yyvsp[(1) - (5)].node)->type) {
		    case TYPE_MAPPING:
			yyerror("Illegal index for mapping.");
		    case TYPE_ANY:
			(yyval.node)->type = TYPE_ANY;
			break;
		    case TYPE_STRING:
		    case TYPE_BUFFER:
			(yyval.node)->type = TYPE_NUMBER;
			if (!IS_TYPE((yyvsp[(4) - (5)].node)->type,TYPE_NUMBER))
			    type_error("Bad type of index", (yyvsp[(4) - (5)].node)->type);
			break;

		    default:
			if ((yyvsp[(1) - (5)].node)->type & TYPE_MOD_ARRAY) {
			    (yyval.node)->type = (yyvsp[(1) - (5)].node)->type & ~TYPE_MOD_ARRAY;
			    if (!IS_TYPE((yyvsp[(4) - (5)].node)->type,TYPE_NUMBER))
				type_error("Bad type of index", (yyvsp[(4) - (5)].node)->type);
			} else {
			    type_error("Value indexed has a bad type ", (yyvsp[(1) - (5)].node)->type);
			    (yyval.node)->type = TYPE_ANY;
			}
		    }
		} else (yyval.node)->type = TYPE_ANY;
            }
    break;

  case 194:

    {
#line 2487 "grammar/grammar.y.pre"
		/* Something stupid like ({ 1, 2, 3 })[1]; we take the
		 * time to optimize this because people who don't understand
		 * the preprocessor often write things like:
		 *
		 * #define MY_ARRAY ({ "foo", "bar", "bazz" })
		 * ...
		 * ... MY_ARRAY[1] ...
		 *
		 * which of course expands to the above.
		 */
                if (IS_NODE((yyvsp[(1) - (4)].node), NODE_CALL, F_AGGREGATE) && (yyvsp[(3) - (4)].node)->kind == NODE_NUMBER) {
                    int i = (yyvsp[(3) - (4)].node)->v.number;
                    if (i < 0 || i >= (yyvsp[(1) - (4)].node)->l.number)
					yyerror("Illegal index to array constant.");
                    else {
                        parse_node_t *node = (yyvsp[(1) - (4)].node)->r.expr;
                        while (i--)
                            node = node->r.expr;
                        (yyval.node) = node->v.expr;
                        break;
                    }
                }
			if ((yyvsp[(1) - (4)].node)->type != TYPE_MAPPING && (yyvsp[(1) - (4)].node)->type != TYPE_ANY &&
                (yyvsp[(3) - (4)].node)->kind == NODE_NUMBER && (yyvsp[(3) - (4)].node)->v.number < 0)
					yywarn("11 A negative constant in arr[x] no longer means indexing from the end.  Use arr[<x]");
                CREATE_BINARY_OP((yyval.node), F_INDEX, 0, (yyvsp[(3) - (4)].node), (yyvsp[(1) - (4)].node));
                if (exact_types) {
		    switch((yyvsp[(1) - (4)].node)->type) {
		    case TYPE_MAPPING:
		    case TYPE_ANY:
			(yyval.node)->type = TYPE_ANY;
			break;
		    case TYPE_STRING:
		    case TYPE_BUFFER:
			(yyval.node)->type = TYPE_NUMBER;
			if (!IS_TYPE((yyvsp[(3) - (4)].node)->type,TYPE_NUMBER))
			    type_error("Bad type of index", (yyvsp[(3) - (4)].node)->type);
			break;

		    default:
			if ((yyvsp[(1) - (4)].node)->type & TYPE_MOD_ARRAY) {
			    (yyval.node)->type = (yyvsp[(1) - (4)].node)->type & ~TYPE_MOD_ARRAY;
			    if (!IS_TYPE((yyvsp[(3) - (4)].node)->type,TYPE_NUMBER))
				type_error("Bad type of index", (yyvsp[(3) - (4)].node)->type);
			} else {
			    type_error("Value indexed has a bad type ", (yyvsp[(1) - (4)].node)->type);
			    (yyval.node)->type = TYPE_ANY;
			}
                    }
                } else (yyval.node)->type = TYPE_ANY;
            }
    break;

  case 196:

    {
#line 2543 "grammar/grammar.y.pre"
		(yyval.node) = (yyvsp[(2) - (3)].node);
	    }
    break;

  case 198:

    {
#line 2551 "grammar/grammar.y.pre"
	        if ((yyvsp[(1) - (1)].type) != TYPE_FUNCTION) yyerror("Reserved type name unexpected.");
		(yyval.func_block).num_local = current_number_of_locals;
		(yyval.func_block).max_num_locals = max_num_locals;
		(yyval.func_block).context = context;
		(yyval.func_block).save_current_type = current_type;
		(yyval.func_block).save_exact_types = exact_types;
	        if (type_of_locals_ptr + max_num_locals + CFG_MAX_LOCAL_VARIABLES >= &type_of_locals[type_of_locals_size])
		    reallocate_locals();
		deactivate_current_locals();
		locals_ptr += current_number_of_locals;
		type_of_locals_ptr += max_num_locals;
		runtime_locals_ptr += current_number_of_locals;
		max_num_locals = current_number_of_locals = 0;
		push_function_context();
		current_function_context->num_parameters = -1;
		exact_types = TYPE_ANY;
		context = 0;
            }
    break;

  case 199:

    {
#line 2571 "grammar/grammar.y.pre"
		if ((yyvsp[(4) - (6)].argument).flags & ARG_IS_PROTO) {
		    yyerror("Missing name for function argument");
		}
		if ((yyvsp[(4) - (6)].argument).flags & ARG_IS_VARARGS) {
		    yyerror("Anonymous varargs functions aren't implemented");
		}
		if (!(yyvsp[(6) - (6)].decl).node) {
		    CREATE_RETURN((yyval.node), 0);
		} else if ((yyvsp[(6) - (6)].decl).node->kind != NODE_RETURN &&
			   ((yyvsp[(6) - (6)].decl).node->kind != NODE_TWO_VALUES || (yyvsp[(6) - (6)].decl).node->r.expr->kind != NODE_RETURN)) {
		    parse_node_t *replacement;
		    CREATE_STATEMENTS(replacement, (yyvsp[(6) - (6)].decl).node, 0);
		    CREATE_RETURN(replacement->r.expr, 0);
		    (yyvsp[(6) - (6)].decl).node = replacement;
		}

		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_ANON_FUNC;
		(yyval.node)->type = TYPE_FUNCTION;
		(yyval.node)->l.number = (max_num_locals - (yyvsp[(4) - (6)].argument).num_arg);
		(yyval.node)->r.expr = (yyvsp[(6) - (6)].decl).node;
		(yyval.node)->v.number = (yyvsp[(4) - (6)].argument).num_arg;
		if (current_function_context->bindable)
		    (yyval.node)->v.number |= 0x10000;
		free_all_local_names();

		current_number_of_locals = (yyvsp[(2) - (6)].func_block).num_local;
		max_num_locals = (yyvsp[(2) - (6)].func_block).max_num_locals;
		context = (yyvsp[(2) - (6)].func_block).context;
		current_type = (yyvsp[(2) - (6)].func_block).save_current_type;
		exact_types = (yyvsp[(2) - (6)].func_block).save_exact_types;
		pop_function_context();

		locals_ptr -= current_number_of_locals;
		type_of_locals_ptr -= max_num_locals;
		runtime_locals_ptr -= current_number_of_locals;
		reactivate_current_locals();
	    }
    break;

  case 200:

    {
#line 2611 "grammar/grammar.y.pre"
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_FUNCTION_CONSTRUCTOR;
		(yyval.node)->type = TYPE_FUNCTION;
		(yyval.node)->r.expr = 0;
		switch ((yyvsp[(1) - (3)].number) & 0xff) {
		case FP_L_VAR:
		    yyerror("Illegal to use local variable in a functional.");
		    CREATE_NUMBER((yyval.node)->l.expr, 0);
		    (yyval.node)->l.expr->r.expr = 0;
		    (yyval.node)->l.expr->l.expr = 0;
		    (yyval.node)->v.number = FP_FUNCTIONAL;
		    break;
		case FP_G_VAR:
		    CREATE_OPCODE_1((yyval.node)->l.expr, F_GLOBAL, 0, (yyvsp[(1) - (3)].number) >> 8);
		    (yyval.node)->v.number = FP_FUNCTIONAL | FP_NOT_BINDABLE;
		    if (VAR_TEMP((yyval.node)->l.expr->l.number)->type & NAME_HIDDEN) {
		      char buf[256];
		      char *end = EndOf(buf);
		      char *p;

		      p = strput(buf, end, "Illegal to use private variable '");
		      p = strput(p, end, VAR_TEMP((yyval.node)->l.expr->l.number)->name);
		      p = strput(p, end, "'");
		      yyerror(buf);
		    }
		    break;
		default:
		    (yyval.node)->v.number = (yyvsp[(1) - (3)].number);
		    break;
		}
	    }
    break;

  case 201:

    {
#line 2644 "grammar/grammar.y.pre"
		(yyval.node) = new_node();
		(yyval.node)->kind = NODE_FUNCTION_CONSTRUCTOR;
		(yyval.node)->type = TYPE_FUNCTION;
		(yyval.node)->v.number = (yyvsp[(1) - (5)].number);
		(yyval.node)->r.expr = (yyvsp[(3) - (5)].node);

		switch ((yyvsp[(1) - (5)].number) & 0xff) {
		case FP_EFUN: {
		    int *argp;
		    int f = (yyvsp[(1) - (5)].number) >>8;
		    int num = (yyvsp[(3) - (5)].node)->kind;
		    int max_arg = predefs[f].max_args;

		    if (num > max_arg && max_arg != -1) {
			parse_node_t *pn = (yyvsp[(3) - (5)].node);

			while (pn) {
			    if (pn->type & 1) break;
			    pn = pn->r.expr;
			}

			if (!pn) {
			    char bff[256];
			    char *end = EndOf(bff);
			    char *p;

			    p = strput(bff, end, "Too many arguments to ");
			    p = strput(p, end, predefs[f].word);
			    yyerror(bff);
			}
		    } else if (max_arg != -1 && exact_types) {
			/*
			 * Now check all types of arguments to efuns.
			 */
			int i, argn, tmp;
			parse_node_t *enode = (yyvsp[(3) - (5)].node);
			argp = &efun_arg_types[predefs[f].arg_index];

			for (argn = 0; argn < num; argn++) {
			    if (enode->type & 1) break;

			    tmp = enode->v.expr->type;
			    for (i=0; !compatible_types(tmp, argp[i])
				 && argp[i] != 0; i++)
				;
			    if (argp[i] == 0) {
				char buf[256];
				char *end = EndOf(buf);
				char *p;

				p = strput(buf, end, "Bad argument ");
				p = strput_int(p, end, argn+1);
				p = strput(p, end, " to efun ");
				p = strput(p, end, predefs[f].word);
				p = strput(p, end, "()");
				yyerror(buf);
			    } else {
				/* this little section necessary b/c in the
				   case float | int we dont want to do
				   promoting. */
				if (tmp == TYPE_NUMBER && argp[i] == TYPE_REAL) {
				    for (i++; argp[i] && argp[i] != TYPE_NUMBER; i++)
					;
				    if (!argp[i])
					enode->v.expr = promote_to_float(enode->v.expr);
				}
				if (tmp == TYPE_REAL && argp[i] == TYPE_NUMBER) {
				    for (i++; argp[i] && argp[i] != TYPE_REAL; i++)
					;
				    if (!argp[i])
					enode->v.expr = promote_to_int(enode->v.expr);
				}
			    }
			    while (argp[i] != 0)
				i++;
			    argp += i + 1;
			    enode = enode->r.expr;
			}
		    }
		    break;
		}
		case FP_L_VAR:
		case FP_G_VAR:
		    yyerror("Can't give parameters to functional.");
		    break;
		}
	    }
    break;

  case 202:

    {
#line 2733 "grammar/grammar.y.pre"
		 if (current_function_context->num_locals)
		     yyerror("Illegal to use local variable in functional.");
		 if (current_function_context->values_list->r.expr)
		     current_function_context->values_list->r.expr->kind = current_function_context->values_list->kind;

		 (yyval.node) = new_node();
		 (yyval.node)->kind = NODE_FUNCTION_CONSTRUCTOR;
		 (yyval.node)->type = TYPE_FUNCTION;
		 (yyval.node)->l.expr = (yyvsp[(2) - (4)].node);
		 if ((yyvsp[(2) - (4)].node)->kind == NODE_STRING)
			yywarn("Function pointer returning string constant is NOT a function call");
		 (yyval.node)->r.expr = current_function_context->values_list->r.expr;
		 (yyval.node)->v.number = FP_FUNCTIONAL + current_function_context->bindable
		     + (current_function_context->num_parameters << 8);
		 pop_function_context();
             }
    break;

  case 203:

    {
#line 2751 "grammar/grammar.y.pre"
		CREATE_CALL((yyval.node), F_AGGREGATE_ASSOC, TYPE_MAPPING, (yyvsp[(2) - (4)].node));
	    }
    break;

  case 204:

    {
#line 2755 "grammar/grammar.y.pre"
		CREATE_CALL((yyval.node), F_AGGREGATE, TYPE_ANY | TYPE_MOD_ARRAY, (yyvsp[(2) - (4)].node));
	    }
    break;

  case 205:

    {
#line 2762 "grammar/grammar.y.pre"
		(yyval.node) = (yyvsp[(1) - (1)].decl).node;
	    }
    break;

  case 206:

    {
#line 2766 "grammar/grammar.y.pre"
		(yyval.node) = insert_pop_value((yyvsp[(2) - (3)].node));
	    }
    break;

  case 207:

    {
#line 2773 "grammar/grammar.y.pre"
		(yyval.number) = context;
		context = SPECIAL_CONTEXT;
	    }
    break;

  case 208:

    {
#line 2778 "grammar/grammar.y.pre"
		CREATE_CATCH((yyval.node), (yyvsp[(3) - (3)].node));
		context = (yyvsp[(2) - (3)].number);
	    }
    break;

  case 209:

    {
#line 2802 "grammar/grammar.y.pre"
		int p = (yyvsp[(6) - (7)].node)->v.number;
		CREATE_LVALUE_EFUN((yyval.node), TYPE_NUMBER, (yyvsp[(6) - (7)].node));
		CREATE_BINARY_OP_1((yyval.node)->l.expr, F_SSCANF, 0, (yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), p);
	    }
    break;

  case 210:

    {
#line 2811 "grammar/grammar.y.pre"
		(yyval.number) = context;
		context = SPECIAL_CONTEXT;
	    }
    break;

  case 211:

    {
#line 2816 "grammar/grammar.y.pre"
		CREATE_TIME_EXPRESSION((yyval.node), (yyvsp[(3) - (3)].node));
		context = (yyvsp[(2) - (3)].number);
	    }
    break;

  case 212:

    {
#line 2824 "grammar/grammar.y.pre"
	        (yyval.node) = new_node_no_line();
		(yyval.node)->r.expr = 0;
	        (yyval.node)->v.number = 0;
	    }
    break;

  case 213:

    {
#line 2830 "grammar/grammar.y.pre"
		parse_node_t *insert;

		(yyval.node) = (yyvsp[(3) - (3)].node);
		insert = new_node_no_line();
		insert->r.expr = (yyvsp[(3) - (3)].node)->r.expr;
		insert->l.expr = (yyvsp[(2) - (3)].node);
		(yyvsp[(3) - (3)].node)->r.expr = insert;
		(yyval.node)->v.number++;
	    }
    break;

  case 214:

    {
#line 2844 "grammar/grammar.y.pre"
		CREATE_STRING((yyval.node), (yyvsp[(1) - (1)].string));
		scratch_free((yyvsp[(1) - (1)].string));
	    }
    break;

  case 216:

    {
#line 2853 "grammar/grammar.y.pre"
		(yyval.string) = (yyvsp[(2) - (3)].string);
	    }
    break;

  case 217:

    {
#line 2857 "grammar/grammar.y.pre"
		(yyval.string) = scratch_join((yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string));
	    }
    break;

  case 219:

    {
#line 2865 "grammar/grammar.y.pre"
		(yyval.string) = scratch_join((yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string));
	    }
    break;

  case 220:

    {
#line 2871 "grammar/grammar.y.pre"
	(yyval.node) = new_node();
	(yyval.node)->l.expr = (parse_node_t *)(yyvsp[(1) - (3)].string);
	(yyval.node)->v.expr = (yyvsp[(3) - (3)].node);
	(yyval.node)->r.expr = 0;
    }
    break;

  case 221:

    {
#line 2881 "grammar/grammar.y.pre"
	(yyval.node) = 0;
    }
    break;

  case 222:

    {
#line 2885 "grammar/grammar.y.pre"
	(yyval.node) = (yyvsp[(3) - (3)].node);
	(yyval.node)->r.expr = (yyvsp[(1) - (3)].node);
    }
    break;

  case 223:

    {
#line 2894 "grammar/grammar.y.pre"
	      (yyval.node) = validate_efun_call((yyvsp[(1) - (4)].number),(yyvsp[(3) - (4)].node));
	    }
    break;

  case 224:

    {
#line 2898 "grammar/grammar.y.pre"
		ident_hash_elem_t *ihe;

		ihe = lookup_ident("clone_object");
		(yyval.node) = validate_efun_call(ihe->dn.efun_num, (yyvsp[(3) - (4)].node));
            }
    break;

  case 225:

    {
#line 2905 "grammar/grammar.y.pre"
		parse_node_t *node;

		if ((yyvsp[(4) - (6)].ihe)->dn.class_num == -1) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;

		    p = strput(buf, end, "Undefined class '");
		    p = strput(p, end, (yyvsp[(4) - (6)].ihe)->name);
		    p = strput(p, end, "'");
		    yyerror(buf);
		    CREATE_ERROR((yyval.node));
		    node = (yyvsp[(5) - (6)].node);
		    while (node) {
			scratch_free((char *)node->l.expr);
			node = node->r.expr;
		    }
		} else {
		    int type = (yyvsp[(4) - (6)].ihe)->dn.class_num | TYPE_MOD_CLASS;

		    if ((node = (yyvsp[(5) - (6)].node))) {
			CREATE_TWO_VALUES((yyval.node), type, 0, 0);
			(yyval.node)->l.expr = reorder_class_values((yyvsp[(4) - (6)].ihe)->dn.class_num,
							node);
			CREATE_OPCODE_1((yyval.node)->r.expr, F_NEW_CLASS,
					type, (yyvsp[(4) - (6)].ihe)->dn.class_num);

		    } else {
			CREATE_OPCODE_1((yyval.node), F_NEW_EMPTY_CLASS,
					type, (yyvsp[(4) - (6)].ihe)->dn.class_num);
		    }
		}
            }
    break;

  case 226:

    {
#line 2940 "grammar/grammar.y.pre"
	      int f;

	      (yyval.node) = (yyvsp[(3) - (4)].node);
	      if ((f = (yyvsp[(1) - (4)].ihe)->dn.function_num) != -1) {
		  if (FUNCTION_FLAGS(f) & NAME_HIDDEN) {
		      char buf[256];
		      char *end = EndOf(buf);
		      char *p;

		      p = strput(buf, end, "Illegal to call private function '");
		      p = strput(p, end, (yyvsp[(1) - (4)].ihe)->name);
		      p = strput(p, end, "'");
		      yyerror(buf);
		  }
		  if (current_function_context)
		      current_function_context->bindable = FP_NOT_BINDABLE;

		  (yyval.node)->kind = NODE_CALL_1;
		  (yyval.node)->v.number = F_CALL_FUNCTION_BY_ADDRESS;
		  (yyval.node)->l.number = f;
		  (yyval.node)->type = validate_function_call(f, (yyvsp[(3) - (4)].node)->r.expr);
	      } else
	      if ((f=(yyvsp[(1) - (4)].ihe)->dn.simul_num) != -1) {
		  (yyval.node)->kind = NODE_CALL_1;
		  (yyval.node)->v.number = F_SIMUL_EFUN;
		  (yyval.node)->l.number = f;
		  (yyval.node)->type = (SIMUL(f)->type) & ~NAME_TYPE_MOD;
	      } else
	      if ((f=(yyvsp[(1) - (4)].ihe)->dn.efun_num) != -1) {
		  (yyval.node) = validate_efun_call(f, (yyvsp[(3) - (4)].node));
	      } else {
		/* This here is a really nasty case that only occurs with
		 * exact_types off.  The user has done something gross like:
		 *
		 * func() { int f; f(); } // if f was prototyped we wouldn't
		 * f() { }                // need this case
		 *
		 * Don't complain, just grok it.
		 */
		int cf, f;

		if (current_function_context)
		    current_function_context->bindable = FP_NOT_BINDABLE;

		cf = define_new_function((yyvsp[(1) - (4)].ihe)->name, 0, 0,
					 NAME_UNDEFINED | NAME_PROTOTYPE, 0);
		f = COMPILER_FUNC(cf)->runtime_index;
		(yyval.node)->kind = NODE_CALL_1;
		(yyval.node)->v.number = F_CALL_FUNCTION_BY_ADDRESS;
		(yyval.node)->l.number = f;
		(yyval.node)->type = TYPE_ANY; /* just a guess */
		if (exact_types) {
		    char buf[256];
		    char *end = EndOf(buf);
		    char *p;
		    char *n = (yyvsp[(1) - (4)].ihe)->name;
		    if (*n == ':') n++;
		    /* prevent some errors; by making it look like an
		     * inherited function we prevent redeclaration errors
		     * if it shows up later
		     */
		    FUNCTION_FLAGS(f) &= ~NAME_UNDEFINED;
		    FUNCTION_FLAGS(f) |= NAME_INHERITED;
		    COMPILER_FUNC(cf)->type |= NAME_VARARGS;
		    p = strput(buf, end, "Undefined function ");
		    p = strput(p, end, n);
		    yyerror(buf);
		}
	      }
	    }
    break;

  case 227:

    {
#line 3012 "grammar/grammar.y.pre"
	      char *name = (yyvsp[(1) - (4)].string);

	      (yyval.node) = (yyvsp[(3) - (4)].node);

	      if (current_function_context)
		  current_function_context->bindable = FP_NOT_BINDABLE;

	      if (*name == ':') {
		  int f;

		  f = arrange_call_inherited(name + 1, (yyval.node));
		      /* Can't do this; f may not be the correct function
			 entry.  It might be overloaded.

		      validate_function_call(f, $$->r.expr)
		      */
	      } else {
		  int f;
		  ident_hash_elem_t *ihe;

		  f = (ihe = lookup_ident(name)) ? ihe->dn.function_num : -1;
		  (yyval.node)->kind = NODE_CALL_1;
		  (yyval.node)->v.number = F_CALL_FUNCTION_BY_ADDRESS;
		  if (f!=-1) {
		      /* The only way this can happen is if function_name
		       * below made the function name.  The lexer would
		       * return L_DEFINED_NAME instead.
		       */
		      (yyval.node)->type = validate_function_call(f, (yyvsp[(3) - (4)].node)->r.expr);
		  } else {
		      f = define_new_function(name, 0, 0,
					      NAME_UNDEFINED | NAME_PROTOTYPE, 0);
		      f = COMPILER_FUNC(f)->runtime_index;
		  }
		  (yyval.node)->l.number = f;
		  /*
		   * Check if this function has been defined.
		   * But, don't complain yet about functions defined
		   * by inheritance.
		   */
		  if (exact_types && (FUNCTION_FLAGS(f) & NAME_UNDEFINED)) {
		      char buf[256];
		      char *end = EndOf(buf);
		      char *p;
		      char *n = (yyvsp[(1) - (4)].string);
		      if (*n == ':') n++;
		      /* prevent some errors */
		      FUNCTION_FLAGS(f) &= ~NAME_UNDEFINED;
		      FUNCTION_FLAGS(f) |= (NAME_INHERITED | NAME_VARARGS);
		      p = strput(buf, end, "Undefined function ");
		      p = strput(p, end, n);
		      yyerror(buf);
		  }
		  if (!(FUNCTION_FLAGS(f) & NAME_UNDEFINED))
		      (yyval.node)->type = FUNCTION_DEF(f)->type;
		  else
		      (yyval.node)->type = TYPE_ANY;  /* Just a guess */
	      }
	      scratch_free(name);
	  }
    break;

  case 228:

    {
#line 3074 "grammar/grammar.y.pre"
		parse_node_t *expr, *expr2;
		(yyval.node) = (yyvsp[(5) - (6)].node);
		(yyval.node)->kind = NODE_EFUN;
		(yyval.node)->l.number = (yyval.node)->v.number + 2;
		(yyval.node)->v.number = F_CALL_OTHER;
#ifdef CAST_CALL_OTHERS
		(yyval.node)->type = TYPE_UNKNOWN;
#else
                (yyval.node)->type = TYPE_ANY;
#endif
		expr = new_node_no_line();
		expr->type = 0;
		expr->v.expr = (yyvsp[(1) - (6)].node);

		expr2 = new_node_no_line();
		expr2->type = 0;
		CREATE_STRING(expr2->v.expr, (yyvsp[(3) - (6)].string));
		scratch_free((yyvsp[(3) - (6)].string));

		/* insert the two nodes */
		expr2->r.expr = (yyval.node)->r.expr;
		expr->r.expr = expr2;
		(yyval.node)->r.expr = expr;
	    }
    break;

  case 229:

    {
#line 3100 "grammar/grammar.y.pre"
	        parse_node_t *expr;

		(yyval.node) = (yyvsp[(6) - (7)].node);
		(yyval.node)->kind = NODE_EFUN;
		(yyval.node)->l.number = (yyval.node)->v.number + 1;
		(yyval.node)->v.number = F_EVALUATE;
#ifdef CAST_CALL_OTHERS
		(yyval.node)->type = TYPE_UNKNOWN;
#else
		(yyval.node)->type = TYPE_ANY;
#endif
		expr = new_node_no_line();
		expr->type = 0;
		expr->v.expr = (yyvsp[(3) - (7)].node);
		expr->r.expr = (yyval.node)->r.expr;
		(yyval.node)->r.expr = expr;
	    }
    break;

  case 230:

    {
#line 3120 "grammar/grammar.y.pre"
	//svalue_t *res;
	ident_hash_elem_t *ihe;

	(yyval.number) = (ihe = lookup_ident((yyvsp[(3) - (3)].string))) ? ihe->dn.efun_num : -1;
	if ((yyval.number) == -1) {
	    char buf[256];
	    char *end = EndOf(buf);
	    char *p;

	    p = strput(buf, end, "Unknown efun: ");
	    p = strput(p, end, (yyvsp[(3) - (3)].string));
	    yyerror(buf);
	} else {
		/* // 去掉了override关于APPLY_*的判断， -- by canoe
	    push_malloced_string(the_file_name(current_file));
	    share_and_push_string($3);
	    push_malloced_string(add_slash(main_file_name()));
	    res = safe_apply_master_ob(APPLY_VALID_OVERRIDE, 3);
	    if (!MASTER_APPROVED(res)) {
		yyerror("Invalid simulated efunction override");
		$$ = -1;
	    }
		*/
	}
	scratch_free((yyvsp[(3) - (3)].string));
      }
    break;

  case 231:

    {
#line 3147 "grammar/grammar.y.pre"
	ident_hash_elem_t *ihe;
	//svalue_t *res;

	ihe = lookup_ident("clone_object");
	/* // 去掉了override关于APPLY_*的判断， -- by canoe
	push_malloced_string(the_file_name(current_file));
	push_constant_string("clone_object");
	push_malloced_string(add_slash(main_file_name()));
	res = safe_apply_master_ob(APPLY_VALID_OVERRIDE, 3);
	if (!MASTER_APPROVED(res)) {
	    yyerror("Invalid simulated efunction override");
	    $$ = -1;
	} else
	*/
	(yyval.number) = ihe->dn.efun_num;
      }
    break;

  case 233:

    {
#line 3169 "grammar/grammar.y.pre"
		int l = strlen((yyvsp[(2) - (2)].string)) + 1;
		char *p;
		/* here we be a bit cute.  we put a : on the front so we
		 * don't have to strchr for it.  Here we do:
		 * "name" -> ":::name"
		 */
		(yyval.string) = scratch_realloc((yyvsp[(2) - (2)].string), l + 3);
		p = (yyval.string) + l;
		while (p--,l--)
		    *(p+3) = *p;
		strncpy((yyval.string), ":::", 3);
	    }
    break;

  case 234:

    {
#line 3183 "grammar/grammar.y.pre"
		int z, l = strlen((yyvsp[(3) - (3)].string)) + 1;
		char *p;
		/* <type> and "name" -> ":type::name" */
		z = strlen(compiler_type_names[(yyvsp[(1) - (3)].type)]) + 3; /* length of :type:: */
		(yyval.string) = scratch_realloc((yyvsp[(3) - (3)].string), l + z);
		p = (yyval.string) + l;
		while (p--,l--)
		    *(p+z) = *p;
		(yyval.string)[0] = ':';
		strncpy((yyval.string) + 1, compiler_type_names[(yyvsp[(1) - (3)].type)], z - 3);
		(yyval.string)[z-2] = ':';
		(yyval.string)[z-1] = ':';
	    }
    break;

  case 235:

    {
#line 3198 "grammar/grammar.y.pre"
		int l = strlen((yyvsp[(1) - (3)].string));
		/* "ob" and "name" -> ":ob::name" */
		(yyval.string) = scratch_alloc(l + strlen((yyvsp[(3) - (3)].string)) + 4);
		*((yyval.string)) = ':';
		strcpy((yyval.string) + 1, (yyvsp[(1) - (3)].string));
		strcpy((yyval.string) + l + 1, "::");
		strcpy((yyval.string) + l + 3, (yyvsp[(3) - (3)].string));
		scratch_free((yyvsp[(1) - (3)].string));
		scratch_free((yyvsp[(3) - (3)].string));
	    }
    break;

  case 236:

    {
#line 3213 "grammar/grammar.y.pre"
		/* x != 0 -> x */
		if (IS_NODE((yyvsp[(3) - (6)].node), NODE_BINARY_OP, F_NE)) {
		    if (IS_NODE((yyvsp[(3) - (6)].node)->r.expr, NODE_NUMBER, 0))
			(yyvsp[(3) - (6)].node) = (yyvsp[(3) - (6)].node)->l.expr;
		    else if (IS_NODE((yyvsp[(3) - (6)].node)->l.expr, NODE_NUMBER, 0))
			     (yyvsp[(3) - (6)].node) = (yyvsp[(3) - (6)].node)->r.expr;
		}

		/* TODO: should optimize if (0), if (1) here.
		 * Also generalize this.
		 */

		if ((yyvsp[(5) - (6)].node) == 0) {
		    if ((yyvsp[(6) - (6)].node) == 0) {
			/* if (x) ; -> x; */
			(yyval.node) = insert_pop_value((yyvsp[(3) - (6)].node));
			break;
		    } else {
			/* if (x) {} else y; -> if (!x) y; */
			parse_node_t *repl;

			CREATE_UNARY_OP(repl, F_NOT, TYPE_NUMBER, (yyvsp[(3) - (6)].node));
			(yyvsp[(3) - (6)].node) = repl;
			(yyvsp[(5) - (6)].node) = (yyvsp[(6) - (6)].node);
			(yyvsp[(6) - (6)].node) = 0;
		    }
		}
		CREATE_IF((yyval.node), (yyvsp[(3) - (6)].node), (yyvsp[(5) - (6)].node), (yyvsp[(6) - (6)].node));
	    }
    break;

  case 237:

    {
#line 3247 "grammar/grammar.y.pre"
		(yyval.node) = 0;
	    }
    break;

  case 238:

    {
#line 3251 "grammar/grammar.y.pre"
		(yyval.node) = (yyvsp[(2) - (2)].node);
            }
    break;



      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}






#line 3256 "grammar/grammar.y.pre"


