#ifndef __CACHE_H__
#define __CACHE_H__

#include <var_prop.h>

#define CACHE_DIR "dat/"
#define CACHE_EXT ".py"
// 持久化到DB的变量
static string * _cacheArray = ({});


//会持久化到磁盘的变量
#define CACHE_VAR(var, def) \
	mapping var = ( insertArry(_cacheArray, "##var" ), def); \
	mapping Get##var() { return var; } \
	void Set##var(mixed value) { var=value; } \
	void Init##var() { var=def; } 

private static string _saveName = "";
private static int _initFlag = 0;

string getSaveName()
{
	return _saveName;
}

int setSaveName(string saveName)
{
	if(_initFlag)
		return 0;
	_saveName = CACHE_DIR + saveName + CACHE_EXT;
	return 1;
}

int doSave()
{
	mapping data = ([]);
	string toSaveName;
	int ret;
	foreach( string var in _cacheArray )
	{
		data[var] = call_other(this_object(), "Get"+var);
	} 

	toSaveName = getSaveName();
	ret = fs_save_to_file(toSaveName, data );
	debug_message( "save ret:" + ret );
	return 1;
}

// 会自动设置可存盘
// return 99 初始化一个新数据 
//         1 获取数据成功
int doRestore()
{
	mapping data = fs_restore_from_file(getSaveName());
	mixed value;

	if (undefinedp(data)) 
	{
		data = ([]);

		debug_message( "Init[" + getSaveName() + "]" );
		// 初始化数据
		foreach(string var in _cacheArray)
		{
			debug_message( "Init[" + var + "]" );
			call_other(this_object(), "Init" + var);
		}
		doSave();
		return 99;
	}

	foreach(string var in _cacheArray)
	{
		value =  data[var];
		debug_message( "restore[" + var + "]" + sizeof(value));
		call_other(this_object(), "Set" + var, value);
	}
	return 1;
}
#endif
