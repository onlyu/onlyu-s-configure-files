#include <debug.h>

void addUserExp(object user, int exp)
{
	xassert( (exp >= 0), exp );
	user->addExp(exp);
	user->tellMe("你获得了%d经验", exp);
}

int addCash(object user, int cash)
{
	xassert( (cash >= 0), cash );
	int ret = user->addCash(cash);
	user->tellMe("你获得了%d金钱", cash);
	return ret;
}

