#include <stdlib.h>
#include <stdio.h>
#include "lassert.h"
#include "lexcept.h"

lexcept_frame_t *except_stack = NULL;

void lexcept_raise( const lexcept_t *e, const char *file, int line )
{
	lexcept_frame_t *frame = except_stack;

	lassert( e );

	if( frame == NULL ){
		fprintf( stderr, "Uncaught exception" );		
		if ( e->reason )
			fprintf( stderr, " %s", e->reason );
		else
			fprintf( stderr, " at 0x%p", e );
		if ( file && line > 0 )
			fprintf( stderr, " raised at %s:%d\n", file, line );
		fprintf( stderr, "aborting...\n" );
		fflush( stderr );
		abort();
	}
	frame->exception = e;
	frame->file = file;
	frame->line = line;
	except_stack = except_stack->prev;
	longjmp( frame->env, lexcept_raised );
}
