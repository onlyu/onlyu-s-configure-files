
#ifndef __DEBUG_H__
#define __DEBUG_H__

void __debug(string input)
{
	debug_message(input);
	log_file("debug", input);
}

#define assert(x) { if (x) {} else { error(sprintf("%s:%d: assert error:##x", __FILE__, __LINE__) ); } }

#define xassert(x, y) { if (x) {} else { __debug(sprintf("##y==%O",y) ); error(sprintf("%s: assert error:##x", __FILE__) ); } }

#define warning(x) { if (x) {} else { __debug(sprintf("%s: warning:##x", __FILE__) ); } }

#define p_debug(x) __debug( sprintf("%s:##x:%O", __FILE__, x) );


#endif
