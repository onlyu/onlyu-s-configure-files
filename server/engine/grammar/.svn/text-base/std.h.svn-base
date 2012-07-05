#ifndef STD_H
	#define STD_H

	/* the definition of ARCH */
	#include "../include/arch.h"

	#ifdef EDIT_SOURCE
		#define CONST
		#define INLINE
	#else 
		/* all options and configuration */
		#include "options_incl.h"
		#include "configure.h"

		#ifdef PEDANTIC
			#undef INLINE
			#define INLINE
		#endif 
	#endif 

	#include "../include/portability.h"
	#include "../include/macros.h"
	#ifndef _FUNC_SPEC_
		#include "std_incl.h"
		#include "malloc_incl.h"
		#ifndef NO_OPCODES
			#include "opcodes.h"
		#endif 
	#endif 

	#ifdef WIN32
		#define outp outptr
	#endif 

#endif
