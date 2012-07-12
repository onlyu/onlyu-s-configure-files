#ifndef __CONDVAR_MD_H__
	#define __CONDVAR_MD_H__

	#include "threads_md.h"
	#include "mutex_md.h"
	#include <pthread.h>

	typedef struct condvar
	{
		pthread_cond_t cond;
		unsigned int counter;
	} condvar_t;

	int condvarInit(condvar_t*);
	int condvarDestroy(condvar_t*);
	int condvarWait(condvar_t *, mutex_t*);
	int condvarTimedWait(condvar_t *, mutex_t *, long millis);
	int condvarSignal(condvar_t*);
	int condvarBroadcast(condvar_t*);

#endif // __CONDVAR_MD_H__
