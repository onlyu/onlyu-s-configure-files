
mapping getZhaocaiData()
{
	mapping dayData = getDayData();
	if (undefinedp(dayData[K_USER_ZHAOCAIMAO]))
	{
		dayData[K_USER_ZHAOCAIMAO] = ([]);
		dayData[K_USER_ZHAOCAIMAO][K_USER_ZHAOCAIMAO_TIMES] = 0;
		dayData[K_USER_ZHAOCAIMAO][K_USER_ZHAOCAIMAO_HINT] = 1;
	}
	return dayData[K_USER_ZHAOCAIMAO];
}

mapping getZhaocaiTimes()
{
	mapping zhaocaiData = getZhaocaiData();
	return zhaocaiData[K_USER_ZHAOCAIMAO_TIMES];
}

void addZhaocaiTimes(int times)
{
	if (times <= 0)
		return;

	mapping zhaocaiData = getZhaocaiData();
	zhaocaiData[K_USER_ZHAOCAIMAO_TIMES] += times;
}
	

void setZhaocaiNoMoreHint()
{
	mapping zhaocaiData = getZhaocaiData();
	zhaocaiData[K_USER_ZHAOCAIMAO_HINT] = 0;
}

int giveZhaocaiHintOrNot()
{
	mapping zhaocaiData = getZhaocaiData();
	return zhaocaiData[K_USER_ZHAOCAIMAO_HINT];
}





























































