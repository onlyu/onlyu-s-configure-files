#ifndef LEXCEPT_H
#define LEXCEPT_H

#include <setjmp.h>

typedef struct lexcept_t{
	const char *reason;
} lexcept_t;

typedef struct lexcept_frame_t lexcept_frame_t;

struct lexcept_frame_t {
	lexcept_frame_t *prev;
	jmp_buf env;
	const char *file;
	int line;
	const lexcept_t *exception;
};

enum { 
	lexcept_entered=0,
	lexcept_raised,
	lexcept_handled,
	lexcept_finalized,
};

extern lexcept_frame_t *except_stack;

void lexcept_raise(const lexcept_t *e, const char *file, int line);

#define L_TRY do {\
	volatile int execpt_flag;\
	lexcept_frame_t except_frame;\
	except_frame.prev = except_stack;\
	except_stack = &except_frame;\
	except_flag = setjmp(except_frame.env);\
	if(except_flag == lexcept_entered) {

#define L_EXCEPT(e)\
		if (except_flag == lexcept_entered )\
			except_stack = except_stack->prev;\
	}else if(except_frame.exception == &(e)){\
		except_flag = lexcept_handled;

#define L_ELSE \
		if ( except_flag == lexcept_entered )\
			except_stack = except_stack->prev;\
	} else { \
		except_flag = except_handled;

#define L_FINALLY \
		if ( except_flag == lexcept_entered )\
			except_stack = except_stack->prev;\
	} { \
		if ( except_flag == lexcept_entered ) \
			except_flag = lexcept_finalized;

#define L_END_TRY \
		if ( except_flag == lexcept_entered )\
			except_stack = except_stack->prev;\
	} if ( except_flag == lexcept_raised ){ L_RERAISE; }\
} while (0);

// raise a exception
#define L_RAISE(e) lexcept_raise(&(e), __FILE__, __LINE__)
#define L_RERAISE(e) lexcept_raise(except_frame.exception, except_frame.file, except_frame.line)

#endif

