
#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <var_prop.h>

#define SCHEDULE_MAIN "module/schedule"

MEMORY_VAR(allSchedules, ([]))

#define ADD_LOCAL_SCHEDULE(id) \
	if (id > 0) { \
		allSchedules[id] = 1; \
	}

#define DEL_LOCAL_SCHEDULE(id) \
	map_delete(allSchedules, id);


// @param start   排期开始的时间
// @param period  排期周期
// @param end     排期结束周期，如果永久的，传入-
int schedule(int start, int period, int end, string func, mixed* args) 
{
	int id = SCHEDULE_MAIN->schedule( start, period, end, __FILE__, func, args );
	ADD_LOCAL_SCHEDULE(id)
	return id;
}

int runAt(int start, string func, mixed* args)
{
	int id = SCHEDULE_MAIN->runAt(start, __FILE__, func, args);
	ADD_LOCAL_SCHEDULE(id)
	return id;
}

int runAtTime(string timeStr, string func, mixed *args)
{
	return runAt(string_to_time(timeStr), func, args);
}

int runEveryDay(int start, string func, mixed* args)
{
	int id = SCHEDULE_MAIN->runEveryDay( start, __FILE__, func, args );
	ADD_LOCAL_SCHEDULE(id)
	return id;
}

int runEveryHour(int start, string func, mixed* args )
{
	int id = SCHEDULE_MAIN->runEveryHour( start, __FILE__, func, args );
	ADD_LOCAL_SCHEDULE(id)
	return id;
}

int runEveryMin( int start, string func, mixed* args )
{
	int id = SCHEDULE_MAIN->runEveryMin( start, __FILE__, func, args );
	ADD_LOCAL_SCHEDULE(id)
	return id;
}


/*
int SetTimeOut(int delay, string func, mixed *args)
{
	return Schedule( time() -1, delay +1, time() + delay+1, func, args );
}

int SetInterval(int delay, int total, string func, mixed args)
{
	return Schedule(time() -1, delay + 1, time() + total + 1, func, args);
}
*/

int clearSchedule(int id)
{
	SCHEDULE_MAIN->RemoveSchedule(id);
	DEL_LOCAL_SCHEDULE(id);
}

void clearAllSchedule()
{
	foreach( int _id, mixed _ in allSchedules) {
		clearSchedule(_id);
	}
}

#endif // __SCHEDULE_H__
