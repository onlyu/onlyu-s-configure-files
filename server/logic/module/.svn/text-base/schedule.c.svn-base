// 时间排期模块
// -----------------------------------------
// mapping gScheduleTbl = ([
// 		1:([
// 			1:({ startTime, period, endTime, times, file, func, ({ arg1, arg2, }) }),
// 			11:({ startTime, period, endTime,  times, file, func, ({ arg1, arg2, }) }),
// 			21:({ startTime, period, endTime, times, file, func, ({ arg1, arg2, }) }),
//		])
// ]);
// mapping gRunningTbl = ([
// 		60: ([
// 			1322363060:([1:1,100:1,1000:1]),
// 		]),
// ]);

#include <var_prop.h>
#include <debug.h>
#include <log.h>

// 心跳最小间隔
#define SCHEDULE_PERIOD (2)

// 索引 
#define INDEX_START          (0)
#define INDEX_PERIOD         (1)
#define INDEX_END            (2)
#define INDEX_TIMES          (3)
#define INDEX_FILE           (4)
#define INDEX_FUNC           (5)
#define INDEX_ARGS           (6)
#define SCHEDULE_SIZE        (7)

#define MAX_TIME             (0x7FFFFFFF)

// 全局的排期索引
MEMORY_VAR(gIncSCHIndex, 0)

	// 最后执行时间
MEMORY_VAR(gLastRunTime, 0)

	// 排期表 
MEMORY_VAR(gScheduleTbl, ([]))

	// 运行辅助表
MEMORY_VAR(gRunningTbl, ([]))

static string log_name = "schedule"; 
static object logger;

void removeSchedule(int index);

void createLogger()
{
	logger = Import("LOG")->New(log_name);
}

int getScheduleKey(int index)
{
	return index%100;
}

int getRuningKey( int time )
{
	return time%100;
}

static int __insertRuning(int time, int index)
{
	int key = getRuningKey(time);

	logger->Log("[%d]__insertRuning[%d]", index, time);

	if (undefinedp(gRunningTbl[key])) {
		gRunningTbl[key] = ([]);
	}

	if (undefinedp(gRunningTbl[key][time])) {
		gRunningTbl[key][time] = ([]);
	}

	gRunningTbl[key][time][index] = sizeof(gRunningTbl[key][time])+1;

	return gRunningTbl[key][time][index];
}

static int __initSchedule(int start, int period, int end, int times, string file, string func, mixed* args)
{
	int index, indexKey;
	mixed* data;

	int now = time();

	if (!gLastRunTime) gLastRunTime = now - 1;
	if (start == now && gLastRunTime == now) {
		// 说明本模块的DoLoop已经执行过了当前秒，让他补跳一次
		gLastRunTime--;	
	}
	// period >0 
	assert(period > 0);

	index = (++gIncSCHIndex);

	mapping schHashData = getOrSetMapping(gScheduleTbl, getScheduleKey(index));

	data = allocate(SCHEDULE_SIZE);	

	data[INDEX_START] = start;
	data[INDEX_PERIOD] = period;
	data[INDEX_END] = end;
	data[INDEX_TIMES] = times;
	data[INDEX_FILE] = file;
	data[INDEX_FUNC] = func;
	data[INDEX_ARGS] = args;

	logger->Debug("__initSchedule[%d]", index);
	schHashData[index] = data;

	return index;
}

// return -1 目标排期被删除
//        -2 目标排期已经执行完毕
// 		  -3 目标排期已经完结
//		  >0 被排到的时间
//        -99 意外返回值 
static int __schedule(int index, int careNow)
{
	int now, indexKey;
	int start, end, times, nextTime, period;
	string file, func;
	mixed* scheduleData;

	indexKey = getScheduleKey(index);

	scheduleData = getmappingdepth(gScheduleTbl, ({ indexKey, index}));

	if (!sizeof(scheduleData)) return -1;

	times = scheduleData[INDEX_TIMES];
	if (times == 0) {
		removeSchedule(index);
		return -2;
	}

	start = scheduleData[INDEX_START];
	end = scheduleData[INDEX_END];

	// 找到下次执行的时间塞到RUNING里面去
	now = time();

	if (now < start) {
		// 直接把start插入runing	
		__insertRuning(start, index);
		return start;
	}

	if (end > 0) { 
		if (now >= end) {
			removeSchedule(index);
			return -3;
		} else {
			period= scheduleData[INDEX_PERIOD];	
			// 找到下次执行时间
			nextTime = (now + (end - now)%period);	
			if (careNow && nextTime == now) {
				nextTime += period;
			}
			if (nextTime <= end) {
				__insertRuning(nextTime, index);	
				return nextTime;
			} else {
				removeSchedule(index);
				return -3;
			}
		}
	} else {
		period= scheduleData[INDEX_PERIOD];	
		// 找到下次执行时间
		nextTime = (now + period - (now - start) % period);	
		__insertRuning(nextTime, index);
		return nextTime;
	}

	// 正常逻辑到不了这里
	//map_delete(gScheduleTbl, index);
	removeSchedule(index);
	return -99;
}

static void loop()
{
	int now = time();
	int key, start, end, times, indexKey;
	string file, func;
	mapping runKeyTbl, mpTmp;
	mixed* scheduleData, *args;

	call_out("loop", SCHEDULE_PERIOD);

	// 当没有初始化时，说明并没有schedule在列表中
	if (!gLastRunTime) {
		return;
	}

	for (int runTime = gLastRunTime + 1; runTime <= now; runTime++) {
		// 设定最后执行时间
		gLastRunTime = runTime;
		key = getRuningKey( runTime );	
		runKeyTbl = gRunningTbl[key];
		if (sizeof(runKeyTbl) == 0) continue;

		mpTmp = runKeyTbl[runTime];
		map_delete(runKeyTbl, runTime);
		while (sizeof(mpTmp)) {

			int* allSchedule = keys(mpTmp);	
			foreach (int scheduleIndex in allSchedule) {
				map_delete(mpTmp, scheduleIndex);

				indexKey = getScheduleKey(scheduleIndex);
				if (sizeof(gScheduleTbl[indexKey]) == 0) continue;

				scheduleData = gScheduleTbl[indexKey][scheduleIndex];
				if (undefinedp(scheduleData)) continue;

				// TORUN
				times = scheduleData[INDEX_TIMES];	
				if (times > 0) {
					scheduleData[INDEX_TIMES]--;
					times = scheduleData[INDEX_TIMES];
				}
				file = scheduleData[INDEX_FILE];
				func = scheduleData[INDEX_FUNC];
				args = scheduleData[INDEX_ARGS];
				logger->Log("[%d]run[%s]->[%s]", scheduleIndex, file, func);
				if (arrayp(args)) {
					call_other(file, ({ func, })  + args);
				} else {
					call_other(file, ({ func, }));
				}

				// reschedule 
				int ret = __schedule(scheduleIndex, 1);
				logger->Log( "[%d]schedule ret[%d]", scheduleIndex, ret );
			}
			// 重新取runTime
			mpTmp = runKeyTbl[runTime];
			map_delete(runKeyTbl, runTime);
		}
	}
}

void init()
{	
	loop();
}

void create()
{
	createLogger();
	call_out("init", 0);
}

// -----------------------------------------------------------------------------
// 对外接口
// -----------------------------------------------------------------------------

void removeSchedule( int index )
{
	int key = getScheduleKey(index);

	if(undefinedp(gScheduleTbl[key])) return;

	logger->Log("remove [%d][%s]", index, serial_array(keys(gScheduleTbl[key][index]), ","));

	map_delete(gScheduleTbl[key], index);
}


// 返回排期序号 
int runAt(int start, string file, string func, mixed* args)
{
	int now = time();
	int index = __initSchedule( start, MAX_TIME, start, 1, file, func, args );
	int ret = __schedule(index, 0);

	logger->Log("[%d]run at:[%d][%s]->[%s]", index, ret, file, func);
	if (ret < 0) {
		return -1;
	}
	return index;
}

// start 从0点开始的秒数，0表示每天的00:00:00,60表示01:00:00
// 返回派期序号
int runEveryDay(int start, string file, string func, mixed* args)
{
	string day = short_time()[0 .. 10] + " 00:00:00";
	int index = __initSchedule(string2time(day) + start, 86400, -1, -1, file, func, args);
	int ret = __schedule(index, 0);

	logger->Log("[%d]run every day:[%d][%s]->[%s]", index, ret, file, func );
	if (ret < 0) {
		return -1;
	}
	return index;
}

int runEveryHour(int start, string file, string func, mixed* args )
{
	string hour = short_time()[0 .. 13] + ":00:00";
	int index = __initSchedule(string2time(hour) + start, 3600, -1, -1, file, func, args);
	int ret = __schedule(index, 0 );

	logger->Log("[%d]run every hour:[%d][%s]->[%s]", index, ret, file, func );
	if (ret < 0) {
		return -1;
	}
	return index;
}

int runEveryMin(int start, string file, string func, mixed* args)
{
	string min = short_time()[0 .. 16] + ":00";
	int index = __initSchedule(string2time(min) + start, 60, -1, -1, file, func, args);

	int ret = __schedule(index, 0);
	logger->Log("[%d]RunEveryMin:[%d][%s]->[%s]", index, ret, file, func );
	if (ret < 0) {
		return -1;
	}
	return index;
}

// return -1 不需要再执行了
//        >0 执行索引
int schedule(int start, int period, int end, string file, string func, mixed* args)
{
	int runCnt;
	int index, ret;
	int now;

	// 
	assert(end <= 0 || (end > 0 && end > start));
	assert(period > 0);

	now = time();

	// 计算出执行次数	
	if (end <= 0) {
		runCnt = -1;
	} else {
		if (now <= start) {
			runCnt = (end - start) / period + 1;
		} else {
			// 对齐应该执行的时间点
			start = (now + period - (now - start)%period);	
			if (end >= start) {
				runCnt = (end - start) / period + 1;
			} else {
				runCnt = 0;
			}
		}
	}
	logger->Log("Schedule:[%d][%s]->[%s]", runCnt, file, func );
	if (runCnt == 0) return -1;
	// 修正END值
	if (runCnt > 0) {
		end = start + (runCnt - 1) * period ;
	}

	index = __initSchedule(start, period, end, runCnt, file, func, args);
	ret = __schedule(index, 0);
	logger->Log("Schedule:[%d][%s]->[%s]", ret, file, func );
	if (ret < 0) {
		return -1;
	}

	return index;
}

void dumpAllSchedule()
{
	mapping mpKey;
	logger->Log( "Begin Dump ===============================" );	
	for ( int key = 0; key < 100; key++ ) {
		mpKey = gScheduleTbl[key];	

		if (undefinedp(mpKey)) continue;

		foreach (int index, mixed* data in mpKey) {
			int startTime = data[INDEX_START]; 
			int period = data[INDEX_PERIOD]; 
			int endTime = data[INDEX_END]; 
			int times = data[INDEX_TIMES];
			string file = data[INDEX_FILE];
			string func = data[INDEX_FUNC];
			mixed* args = data[INDEX_ARGS];
			// 1:({ startTime, period, endTime, times, file, func, ({ arg1, arg2, }) }),
			logger->Log("[%d]SCHEDULE[%s][%s][%s]-[%s]/[%d]", index, file, func, short_time(startTime), short_time(endTime), period);
		}
	}
	for (int key = 0; key < 100; key++) {
		mpKey = gRunningTbl[key];    
		if (undefinedp(mpKey)) continue;
		foreach (int time, mapping mpData in mpKey) {
			logger->Log("TO RUN[%s] %s", short_time(time), serial_array(keys(mpData), ","));
		} 
	}
	logger->Log( "End Dump ===============================" );	
}
