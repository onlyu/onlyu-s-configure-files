#ifndef __MUTEX_MD_H__
	#define __MUTEX_MD_H__

	#include <pthread.h>

	typedef pthread_mutex_t mutex_t;

	int mutexInit(mutex_t *m);
	#define mutexDestroy(m)	pthread_mutex_destroy(m)
	#define mutexLock(m)	pthread_mutex_lock(m)
	#define mutexUnlock(m)	pthread_mutex_unlock(m)

#endif // __MUTEX_MD_H__
