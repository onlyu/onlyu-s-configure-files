#define	LOG_HANDLE		"module/util/file_handle.c"

//文件句炳管理
object * FileReaders = ({0,0,0,0,0,0,0,0,0,0,});

//返回一个文件句柄
int log_open(string sFileName)
{
	object ob;

	ob = clone_object(LOG_HANDLE);
	if ( ob->register_file(sFileName) == -1 )
	{
		destruct(ob);
		return -1;
	}

	for ( int i = 0; i < sizeof(FileReaders); i++ )
	{
		if ( !objectp(FileReaders[i]) )
		{
			FileReaders[i] = ob;
			return i;
		}
	}	

	destruct(ob);
	return -1;
}

//读文件，读出一行 如果读到文件尾了，则返回0
string log_read(int iFd)
{
	if ( !objectp(FileReaders[iFd]) ) 
		return 0;
	return 
		FileReaders[iFd]->seek_line();
}


//释放文件句柄
void log_close(int iFd)
{
	if ( iFd < 0 || iFd >= sizeof(FileReaders) ) return;
	if ( objectp(FileReaders[iFd]) )
		destruct(FileReaders[iFd]);
	FileReaders[iFd] = 0;
}

/*
void process_log_data(string cFile )
{
	int iFd1 ;
	string sLine ;

	if( (iFd1 = log_open(cFile) ) == -1)
	{
		_debug( sprintf("can not open %s.\n", cFile ) );
		return;
	}
	
	while(sLine = log_read(iFd1))
	{
		// _debug(sLine ); // 读了哪行就记录下来
		handle_line_speedup(sLine);
	}
	log_close(iFd1);
}	
*/

void __test__()
{

}
