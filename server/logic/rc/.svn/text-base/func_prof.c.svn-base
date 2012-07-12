#define PROF_FUNC_TIMEOUT (60 * 60)

/*
void create()
{
	call_out("prof_func", PROF_FUNC_TIMEOUT);
}
*/

/*
void prof_func()
{
	mapping *mpObjInfo;
	string cFile;
	int calls;
	float fSelf, fTotal, fSelfPer, fTotalPer;
	string cMsg;
	int i = 0;

	cMsg = sprintf("[%s] Function Profiling...\n", short_time());
	cMsg += "     self       total                 self       total\n";
	cMsg += "    seconds    seconds      calls    ms/call    ms/call  name\n";

	foreach (object obj in objects())
	{
		mpObjInfo = function_profile(obj);
		cFile = file_name(obj);
		if (strsrch(cFile, "#") != -1)
		{
			continue;
		}

		foreach (mapping mpInfo in mpObjInfo)
		{
			calls = mpInfo["calls"];

			if (calls <= 0)
			{
				continue;
			}

			fSelf = mpInfo["self"] / 1000000.0;
			fTotal = (mpInfo["self"] + mpInfo["children"]) / 1000000.0;
			fSelfPer = mpInfo["self"] / 1000.0 / calls;
			fTotalPer = (mpInfo["self"] + mpInfo["children"]) / 1000.0 / calls;

			cMsg += sprintf("%9.2f%9.2f%9d%9.2f%9.2f  %s\n",
					fSelf,
					fTotal,
					calls,
					fSelfPer,
					fTotalPer,
					cFile + "->" + mpInfo["name"]);
			if (i++ == 100)
			{
				i = 0;
				log_file("sys/func_prof", cMsg);
				cMsg = "";
			}
		}
	}
	log_file("sys/func_prof", cMsg);
}
*/
