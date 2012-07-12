#ifndef __SJCG_TIME__
#define __SJCG_TIME__
#include <localtime.h>
#define END  0
#define READY 1 
#define CAN_ENTER 2
#define START 3 
#define START_HOUR 21

#define START_TIME ({21,0,0})
#define READY_TIME ({20,50,0})
#define CAN_ENTER_TIME ({21,59,0})
#define END_TIME ({21,30,0})

static int getDaySeconds(int* timeTuples)
{
	int hour = timeTuples[0];
	int minute = timeTuples[1];
	int second = timeTuples[2];
	return hour * 3600 + minute * 60 + second;
}


static int getSecondBeforeReady()
{
	return 10;
}
static int getNowDaySeconds()
{
	int now = time();
	int* nowTuples = localtime(now);
	int second = nowTuples[0];
	int minute = nowTuples[1];
	int hour = nowTuples[2];
	int nowDaySeconds = hour * 3600 + minute * 60 + second;
}
static int START_TIME_SECONDS = getDaySeconds(START_TIME);
static int READY_TIME_SECONDS = getDaySeconds(READY_TIME);
static int CAN_ENTER_TIME_SECONDS = getDaySeconds(CAN_ENTER_TIME);
static int END_TIME_SECONDS = getDaySeconds(END_TIME);

int getPeriodByTime()
{
	int nowDaySeconds = getNowDaySeconds();
	if (nowDaySeconds > END_TIME_SECONDS || nowDaySeconds < READY_TIME_SECONDS)
		return END;

	else if (nowDaySeconds >= READY_TIME_SECONDS && nowDaySeconds < CAN_ENTER_TIME_SECONDS)
		return READY;
	else if (nowDaySeconds >= CAN_ENTER_TIME_SECONDS && nowDaySeconds < START_TIME_SECONDS)
		return CAN_ENTER;
	else
		return START;
}

#endif 
