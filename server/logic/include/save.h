
#ifndef __SAVE_H__

#define __SAVE_H__

#include <var_prop.h>

#define SAVE_DIR "/dat/"

// 持久化到DB的变量
static string * _GDBArray = ({});

//会持久化到磁盘的变量
#define DB_VAR(var, def) \
	varargs mapping var = ( insertArry(_GDBArray, "##var" ), def); \
	mapping get##var() { return var; }

private static string _SaveName = "";

string getSaveName() 
{
	/*
	if ( sizeof(_SaveName) < 1 )
	{
		_SaveName = "D_" + __FILE__[0..<3];
		_SaveName = replace_string( _SaveName, "/", "_");
		assure_file(_SaveName);
	}		
	*/
	return _SaveName; 
}


// 存盘一次，并注册可存盘
// create里只能调用load_dat_sync.
int regSave()
{
	return Import("DB")->new_dat(this_object());
}

// 存盘一次，并反注册存盘，此时object可以删除了
// 此函数名千万不能改动，改动则update逻辑出错
int unRegSave()
{
	return Import("DB")->unload_dat(this_object());
}

int syncUnRegSave()
{
	return Import("DB")->unload_dat(this_object(), 1);
}

string setSaveName(string cSaveName)
{
	//_SaveName = SAVE_DIR + cSaveName;
	_SaveName = cSaveName;
}

int doSave()
{
	//save_to_db(GetSaveName());
	return Import("DB")->save_dat(this_object()); 
}


// 会自动设置可存盘
int doRestore()
{
	//restore_from_db(GetSaveName());
	return Import("DB")->load_dat_sync(this_object());
}

void doRestoreAsync( function callback, mixed* args... )
{
	Import("DB")->load_dat_async(this_object(), callback, args...);
}

int rename(string cNewSaveName)
{
	return Import("DB")->rename(this_object(), cNewSaveName);
}

#endif // __SAVE_H__
