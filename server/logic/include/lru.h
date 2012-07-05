#ifndef _LRU_H_
#define _LRU_H_
#include <cache.h>

#define  K_LRU_DATA            0
#define  K_LRU_PREV            1
#define  K_LRU_NEXT            2
#define  INIT_LRU_TRIPLE(x, prev, next)    ({x, prev, next,})

#define  LRU_NULL          (-1)

MEMORY_VAR(lruTail, LRU_NULL)
MEMORY_VAR(lruHead, LRU_NULL)
MEMORY_VAR(lruSize, 0)
MEMORY_VAR(lruQueue, ([]))
MEMORY_VAR(lruInit, 0)

MEMORY_VAR(lruRemoveKeyCallBackFunc, "")

CACHE_VAR(lruData, ([]))

void setLruData(mixed key, mixed data);

#define LRU_SIZE           sizeof(lruQueue)
#define LRU_TRIPLE(k)      (lruQueue[k])
#define LRU_DATA(k)        (lruQueue[k][K_LRU_DATA])
#define LRU_PREV(k)        (lruQueue[k][K_LRU_PREV])
#define LRU_NEXT(k)        (lruQueue[k][K_LRU_NEXT])

void initLRU(int lruCacheSize)
{
	if (!lruInit) {
		lruSize = lruCacheSize;
		lruTail = LRU_NULL;
		lruHead = LRU_NULL;
		lruQueue = ([]);
		lruInit = 1;
	}
	if (sizeof(lruData)) {
		foreach (mixed key, mapping _data in lruData) {
			setLruData(key, _data);
		}
	}
}

void clearLru()
{
	lruSize = 0;
	lruTail = LRU_NULL;
	lruHead = LRU_NULL;
	lruQueue = ([]);
	lruInit = 0;

	foreach (mixed key in keys(lruData)) {
		map_delete(lruData, key);
	}
}

// 将key从队列中取出
void lruPopKey(mixed key)
{
	mixed *tripleList;
	tripleList = LRU_TRIPLE(key);
	if (LRU_SIZE == 1) {
		lruTail = LRU_NULL;
		lruHead = LRU_NULL;
	} else if (key == lruTail) {
		tripleList[K_LRU_NEXT] = LRU_NULL;
		lruTail = tripleList[K_LRU_PREV];
	} else if (key == lruHead) {
		tripleList[K_LRU_PREV] = LRU_NULL;
		lruHead = tripleList[K_LRU_NEXT];
	} else {
		LRU_NEXT(tripleList[K_LRU_PREV]) = tripleList[K_LRU_NEXT];
		LRU_PREV(tripleList[K_LRU_NEXT]) = tripleList[K_LRU_PREV];
	}
	tripleList[K_LRU_NEXT] = LRU_NULL;
	tripleList[K_LRU_PREV] = LRU_NULL;
}

void lruRemoveKey(mixed key)
{
	lruPopKey(key);
	mixed data = LRU_DATA(key);
	map_delete(lruQueue, key);
	map_delete(lruData, key);
	if( sizeof(lruRemoveKeyCallBackFunc)) {
		call_other( this_object(), ({ lruRemoveKeyCallBackFunc, key, data }) );
	}	
}

// 当达到上限时,需要去除结尾的数据
void lruCheck()
{
	int i;
	mixed key, pre_key;

	if (LRU_SIZE >= lruSize) {
		i = lruSize / 10;
		key = lruTail;
		while (i) {
			pre_key = LRU_PREV(key);
			lruRemoveKey(key);
			key = pre_key;
			i--;
		}
	}
}

// 将key插入到队列前面
void lruPushHead(mixed key)
{
	if (lruHead != LRU_NULL) {
		LRU_PREV(lruHead) = key;
	}
	LRU_NEXT(key) = lruHead;
	lruHead = key;
	if (LRU_SIZE == 1) {
		lruTail = key;
	}
	lruCheck();
}

mixed getLruData(mixed key)
{
	if (key == LRU_NULL) {
		debug_message(sprintf("lru can't support key[%d]", LRU_NULL));
		return 0;
	}
	if (undefinedp(LRU_TRIPLE(key))) {
		return 0;
	} else {
		if (lruHead == key) {
			return LRU_DATA(key);
		}
		lruPopKey(key);
		lruPushHead(key);
	}
	return LRU_DATA(key);
}

void setLruData(mixed key, mixed data)
{
	if (key == LRU_NULL) {
		debug_message(sprintf("lru can't support key[%d]", LRU_NULL));
		return;
	}
	if (undefinedp(LRU_TRIPLE(key))) {
		LRU_TRIPLE(key) = INIT_LRU_TRIPLE(data, LRU_NULL, LRU_NULL);
	} else {
		lruPopKey(key);
		LRU_DATA(key) = data;
	}
	lruPushHead(key);
	lruData[key] = data;
}

void removeLruData(mixed key)
{
	if (!undefinedp(LRU_TRIPLE(key))) {
		lruRemoveKey(key);
	}
}

mixed *getLruKeys()
{
	return keys(lruQueue);
}

void printLruInfo()
{
	debug_message(sprintf("lruQueue: %O\nlruTail: %O\nlruHead: %OlruSize: %O", lruQueue, lruTail, lruHead, lruSize));
}

#endif


