// main.c


//
string *epilog()
{
	return remove_file_comments( "/etc/preload" );
}

void preload( string sFile )
{
	if ( sizeof(sFile) > 0 ) // 支持空行
	{
		object oTarget;
		mixed Err = catch(oTarget = load_object(sFile));

		debug_message("preload : " + sFile);
		if (!Err && function_exists("Startup", oTarget) )
		{
			Err = catch(oTarget->Startup());
		}

		debug_message("preload: " + sFile + " ... " + ((Err) ? "Error!" : "Done.") ) ;
	}
}


int pre_init()
{
	"rc/globals"->__init__("/etc/globals"); // 必须存在，请不要删掉

	debug_message( sprintf("loading MASTER ... Done.") );
	return 1;
}


void main()
{
	if (!pre_init()) {
		return;
	}

	string *pre = epilog();
	if (pre) {
		foreach ( string sFile in pre)
			preload(sFile);
	}

	string domain = "wg01";
	string binddomain = "/home/zhj/work/wg/logic/tools/locale";
	mapping m = init_gettext(domain, binddomain);
	debug_message("init gettext:%O\n", m);
	//debug_message(gettext("init gettext ok"));
	//debug_message(gettext("初始化gettext成功"));
	//debug_message(gettext("test gettext str:%s,int:%d\n"), gettext("hello"), 10);
	debug_message(_("init gettext ok"));
	debug_message(_("初始化gettext成功"));
	debug_message(_("test gettext str:%s,int:%d\n"), _("hello"), 10);

	Import("SCENE")->initWorld();

	// 启动系统主循环 
	main_loop();

}

void create()
{
}

// 当编译程序发生任何错误，驱动程序都将调用该函数。
void log_error( string sFile, string sMsg )
{
	log_file( "log/compile.log", sprintf( "%s %s", short_time(), sMsg ) );
	debug_message( sprintf("compile error:%s,%s",sFile,sMsg) );
}

