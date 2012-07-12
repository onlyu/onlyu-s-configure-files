#include "base.h"
int validateFubenId(object user, int fubenId)
{
	if (!fubenId || fubenId != Import("FUBEN_BASE")->getFubenId(user))
		return 0;
	return 1;
}

void rpc_server_fuben_fight(object user, int fubenId, int trapId)
{
	debug_message("fuben server fight fubenId:%d trapId:%d", fubenId, trapId);
	if(!validateFubenId(user, fubenId))
		return;
	class MingLei mingLei = Import("FUBEN_BASE")->getNextMingLei(user);			
	if (!mingLei || mingLei->id != trapId)
		return;
	Import("FUBEN_BASE")->fight(user, mingLei);
}

void rpc_server_fuben_exit(object user, int fubenId)
{
	if(!validateFubenId(user, fubenId))
		return;
	Import("FUBEN_SYS")->exitFuben(user);	
}

void rpc_server_fuben_continue(object user)
{
	Import("FUBEN_SYS")->fightEndContinue(user);
}

void rpc_server_transport(object user, int transId, int fubenId)
{
	debug_message("new fuben transport transId:%d,fubenId:%d", transId, fubenId);
	Import("FUBEN_SYS")->enterFuben(user, fubenId);	
}

void rpc_server_reenter(object user)
{
	Import("FUBEN_SYS")->reEnter(user);
}
