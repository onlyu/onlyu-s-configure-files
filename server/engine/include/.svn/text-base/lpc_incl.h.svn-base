#ifndef LPC_INCL_H
	#define LPC_INCL_H

	#include "std.h"
	#include "origin.h"
	#include "applies.h"

	#include "config.h"
	#include "program.h"
	#include "lpc.h"
	#include "function.h"
	#include "buffer.h"
	#include "object.h"
	#include "dirty.h"
	#include "array.h"
	#include "mapping.h"
	#include "class.h"
	#include "eoperators.h"

	/* main.c has xalloc() and fatal() */
	#include "main.h"
	#include "stralloc.h"
	#include "simulate.h"
	#include "interpret.h"
	#include "rc.h"

	#define FP_LOCAL              2
	#define FP_EFUN               3
	#define FP_SIMUL              4
	#define FP_FUNCTIONAL         5

	/* internal use */
	#define FP_G_VAR              6
	#define FP_L_VAR              7
	#define FP_ANONYMOUS          8
	#define FP_THIS_OBJECT      0x10

	/* additional flags */
	#define FP_MASK             0x0f
	#define FP_HAS_ARGUMENTS    0x10
	#define FP_OWNER_DESTED     0x20
	#define FP_NOT_BINDABLE     0x40

	#define SETJMP(x) setjmp(x)
	#define LONGJMP(x,y) longjmp(x,y)

#endif
