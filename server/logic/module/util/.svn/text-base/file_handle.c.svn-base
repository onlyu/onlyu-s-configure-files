int g_iFileSize = -1;
int g_iStart;
int g_iFileEnd;
int g_iReadPtr;
int g_iReadCount;

int READ_LINE_NUM=4096;			//一次读入的字节数

string g_sFileName, *g_ReadLines;

//按回车解析成多行
string * my_explode(string sInput)
{
	int i, iSize, iStart;
	string * sLines = ({});
	
	iSize  = sizeof(sInput);
	iStart = 0;

	for ( i = 0; i < iSize; i++ )
	{		
		if ( sInput[i] == '\n' )
		{
			if ( iStart < i )
				sLines += ({ sInput[iStart..i] });

			iStart = i + 1;
		}
	}
	
	if ( iStart < iSize )
		sLines += ({ sInput[iStart..iSize-1] });
		
	return sLines;
}

int register_file(string sName)
{
	g_iFileSize = file_size(sName);
	if (g_iFileSize < 1) return -1;

	g_sFileName = sName;
	g_iStart = 0;
	g_iFileEnd = 0;
	g_iReadCount = 0;
	g_iReadPtr = 0;	
	g_ReadLines = ({});

	return g_iFileSize;
}

//读入下一段 返回读入的实际行数
int read_lines()
{
	int iCount;
	string sResult, sLastLine="";
	
	if ( g_iStart < g_iFileSize )
	{				
		iCount = READ_LINE_NUM;
		if ( g_iStart + iCount > g_iFileSize )
		{
			iCount = g_iFileSize - g_iStart;
			g_iFileEnd = 1;
		}
		
		if ( g_iReadCount ) 
			sLastLine = g_ReadLines[g_iReadCount-1];
		
		sResult = sLastLine + read_bytes(g_sFileName, g_iStart, iCount);		
		g_iStart += iCount;
		g_ReadLines = my_explode(sResult);
		g_iReadCount = sizeof(g_ReadLines);		
		return g_iReadCount;
	}	

	g_iFileEnd = 1;
	return 0;
}

string seek_line()
{
	string sLastLine = "";
	
	if ( g_iReadPtr < g_iReadCount-1 )
		return g_ReadLines[g_iReadPtr++];
	
	if (g_iFileEnd)
	{
		if (g_iReadPtr < g_iReadCount)
			return g_ReadLines[g_iReadPtr++];
	}
	else		
	{
		if ( !read_lines() ) return 0;
		g_iReadPtr = 0;		
		return seek_line();
	}
	return 0;
}
