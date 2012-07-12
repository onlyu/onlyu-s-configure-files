#ifndef COMMAND_H
	#define COMMAND_H

	#include "lpc_incl.h"

	typedef char BYTE;
	typedef short WORD;
	typedef int DWORD;

	void vlog_file_timer_f(const char *filename, int log_time, const char *fmt, ...);

	#define event_debug(fmt, args...) vlog_file_timer_f("oslog/event_log.dat", 1, fmt" errno: %d\n", args, errno)
#endif
