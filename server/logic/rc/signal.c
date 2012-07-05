

void gamed_on_signal(int sig)
{
	debug_message( "gamed_on_signal " + sig );
	// SIGINT

	// logout可能逻辑很长，先把脏数据刷过去
	"rc/db"->take_snapshot();

	if (sig == 15) {
		catch("rc/main"->AllLogout());
	}
}

void gamed_on_shutdown()
{
	debug_message( "gamed_on_shutdown ");
	Import("DB")->save_all(1);
	//gamed发给dbd的最后一条指令：告诉dbd gamed退出了。
	fs_db_gamed_exit();

	//"module/user_cache"->DoSave();
	//"module/cache/user_base_attr.c"->DoSave();
}
