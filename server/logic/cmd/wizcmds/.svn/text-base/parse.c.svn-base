


mapping getConf()
{
	return ([
		"Level" : 99,
		"HelpMsg" : "parse 1|2....",
		"Alias" : ({"ps"}),
		"Format" : "string",
		"explode": 1,
	]);
}

string parse_update_file(string updatedir, string updatefile)
{
	int iFd;
	string strLine;
	string cRes = "";

	if( (iFd = log_open(updatefile)) == -1) {
		return "没有updatefile: " + updatefile;
	}

	// 更新文件
	while (strLine = log_read(iFd)) {
		string file = updatedir + strLine;
		int ret;
		file = replace_string( file, "\n", "" );
		if (file[sizeof(file)-2..sizeof(file)] != ".c") {
			continue;
		}
		if ((catch(ret = "module/util/update.c"->DoUpdate(file)) == 0) && ret) {
			cRes += sprintf( "更新%s成功#r", file );
		} else {
			cRes += sprintf( "更新%s失败#r", file);
		}
	}

	log_close(iFd);
	return cRes;
	// rm(updatefile);
}

mapping doCmd(string parse_name)
{
	int iFd, iCurEnd, iPreEnd;
	string strLine;
	string logfile = "/log/parse_log.log";
	string updatefile = "/tmp/update_file.txt";
	string updatedir = __LIB_PATH__;
	string cRes = "";
	string cCmd;

	if (!stringp(parse_name)) {
		return ([ "IsSuccess" : 0, "Ret" : "失败" ]);
	}

	rm(updatefile);

	//cCmd = __LIB_PATH__ + " "+parse_name;
	cCmd = parse_name;
	debug_message( sprintf("docmd: /tools/parse_table.py %s", cCmd) );
	iPreEnd = file_size(logfile);
	os_command( "/tools/parse_table.py", cCmd, "python");
	iCurEnd = file_size(logfile);

	if (iCurEnd > iPreEnd) {
		string cIncContent = read_bytes(logfile, iPreEnd, iCurEnd - iPreEnd );
		cRes = replace_string(cIncContent, "\n", "#r");
	}

	cRes += parse_update_file("", updatefile);

	switch (parse_name) {
		case "mission":
			"module/util/update.c"->DoUpdate("module/mission/preload.c");
			"module/util/update.c"->DoUpdate("module/mission/main.c");
			break;
                case "drama":
			"module/util/update.c"->DoUpdate("module/client_drama.c");
        }

	return ([ "IsSuccess" : 1, "Ret" : cRes ]) ;
}
