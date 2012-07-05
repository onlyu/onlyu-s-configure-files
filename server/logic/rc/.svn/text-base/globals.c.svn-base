// 全局的object总控文件
//
// 同一个物品Import两次，应该不会有任何影响。
// Import过来就直接使用，不用变量缓存之。etc: Import("ErrorName")->XXX(), 即使写错了，系统也会报对NullObject操作的错 Bad argument 1 to call_other
//
// TODO: 更新了/etc/globals，如何在线更新？
// 	方法1、改编译脚本和本文件，自动调用XImport？
// 	方法2、显式调用XImport
//
#include <debug.h>

mapping mpGlobal = ([]);
static int __inited__ = 0;

// 将object置入mpGlobal中去
varargs object XImport(string cKey, string cFile, int bOverload)
{
	object oObj ;
	cKey = upper_case(cKey);
	
	if (!bOverload) assert(!mpGlobal[cKey]);

	oObj = load_object(cFile);
	debug_message( sprintf("Import(%s) = %s", cKey, cFile) );
	mpGlobal[cKey] = ([ ]);
	mpGlobal[cKey]["obj"] = oObj;
	mpGlobal[cKey]["filename"] = TrueName(cFile);

	return oObj;
}

object XLoad(string cFile)
{
	cFile = TrueName(cFile);
	foreach( string cKey, mapping mpFile in mpGlobal)
	{
		if (cFile != mpFile["filename"])
			continue;

		return XImport(cKey, cFile, 1);
	}

	return load_object(cFile);
}

void CreateGlobalDefines(string cCfgFile)
{
	foreach ( string cFiles in remove_file_comments(cCfgFile) )
	{
		string * Arrs = atom(cFiles);
		if (sizeof(Arrs) < 2) {
			continue;
		}
		XImport(Arrs[0], Arrs[1], 1);
	}
}

object Import(string cKey)
{
	// assert( definep(cName) && sizeof(cName) );
	return mpGlobal[cKey]["obj"];
}

void __init__(string cCfgFile)
{
	if (!__inited__)
	{
		CreateGlobalDefines(cCfgFile);
		__inited__ = 1;
	}	
}


void create()
{
	//__init__();
}
