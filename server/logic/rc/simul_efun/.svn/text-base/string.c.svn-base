
int is_number(string sNum)
{
	int i ;

	for ( i = 0; i < sizeof(sNum); i++ )
	{
		if ( (sNum[i] < 48 || sNum[i] > 57) )
		{
			return 0 ;
		}
	}
	return 1 ;
}

int is_sign_number(string sNum)
{
	int i ;

	for ( i = 0; i < sizeof(sNum); i++ )
	{
		if ( (sNum[i] < 48 || sNum[i] > 57) && sNum[i] != 45 )
		{
			return 0 ;
		}
	}
	return 1 ;
}

int atoi(string sNumStr)
{
	int iNum;

	if ( !stringp(sNumStr) ) return 0;

	if( !sscanf(sNumStr, "%d", iNum) ) 
		return 0;

	return iNum;
}

string itoa(int iNum) 
{ 
	return sprintf("%d", iNum); 
}

mixed left_trim( mixed mArray )
{
	int i, iSize;

	if( stringp(mArray) )
	{
		iSize = sizeof(mArray);
		for( i = 0; i < iSize && mArray[i] == ' '; i ++ ) ;
		return mArray[ i..<1 ];
	}
	else if( pointerp(mArray) )
	{
		iSize = sizeof(mArray);
		for( i = 0; i < iSize && mArray[i] == ""; i ++ ) ;
		return mArray[ i..<1 ];
	}

	return mArray;
}

mixed right_trim( mixed mArray )
{
	int iSize;

	if( stringp(mArray) )
	{
		iSize = sizeof(mArray);
		while( iSize -- ) if( mArray[iSize] != ' ' ) break;
		return mArray[ 0..iSize ];
	}
	else if( pointerp(mArray) )
	{
		iSize = sizeof(mArray);
		while( iSize -- ) if( mArray[iSize] != "" ) break;
		return mArray[ 0..iSize ];
	}
	else return mArray;
}


mixed all_trim( mixed mArray )
{
	return left_trim( right_trim( mArray ) );
}


mixed atom(string sArg) 
{ 
	mixed *m, m1, m2;
	object o;
	string s, s1, s2;
	int i, iSize;

	if( ! ( sArg = all_trim(sArg) ) ) 
		return sArg;

	if( sscanf( sArg, "%d", i ) && is_sign_number(sArg) ) 
		return i;

	if( sscanf( sArg, "\"%s\"", s ) ) 
		return s;

	if( o = find_object( sArg ) ) 
		return o;

	if( sscanf( sArg, "(:%s,%s:)", s1, s2 ) == 2 )
	{
		m1 = atom( s1 );
		m2 = atom( s2 );
	}	

	if( sscanf( sArg, "({%s})", s ) )
	{
		m = explode( s, "," );
		iSize = sizeof( m );

		if( iSize && !m[0] ) 
			return ({ });

		for( i = 0; i < iSize; i ++ ) 
			m[i] = atom( m[i] );

		return m;
	}

	if( sscanf( sArg, "([%s])", s ) )
	{
		mapping mp = ([]);
		m = explode( s, "," );
		iSize = sizeof( m );

		if( iSize && !m[0] ) 
			return ([ ]);

		for( i = 0; i < iSize; i ++ )
			if( sscanf( m[i], "%s:%s", s1, s2 ) == 2 ) 
				mp[ atom(s1) ] = atom(s2);

		return mp;
	}

	return sArg;	
}	


mixed random_one( mixed *sArray ) 
{ 
	return sArray[ random( sizeof(sArray) ) ]; 
}

string string_join(mixed * arr, string spe)
{		
	string str = "";
	mixed s;
	if (!arrayp(arr)) return "";		
	if (sizeof(arr) < 1) return "";

	foreach(s in arr)	
	{
		str += sprintf("%O%s", s, spe);
	}
	return str[0..(sizeof(str)-1-sizeof(spe))];
}

/*
varargs mixed mid( mixed mArray, int iStart, int iNum )
{
	int iSize;

	if( stringp(mArray) || pointerp(mArray) )
	{
		iSize = sizeof(mArray);
		if( iStart < 1 ) iStart = 1;
		if( nullp(iNum) ) return mArray[iStart - 1 .. iSize - 1];
		if( iNum < 1 || iStart > iSize ) return mArray[ iSize..iSize ];
		return mArray[iStart - 1 .. iStart + iNum - 2];
	}
	else return mArray;
}
*/

/*
varargs mixed exclude( mixed mArray, int iStart, int iNum )
{
	mixed mLeft, mRight;
	int iSize;

	if( stringp(mArray) || pointerp(mArray) )
	{
		iSize = sizeof(mArray);
		if( iStart < 1 ) iStart = 1;
		if( nullp(iNum) )
		{
			if( iStart == 1 ) return mArray[iSize .. iSize];
			else return mArray[0 .. iStart - 2];
		}
		if( iNum <= 0 || iStart > iSize ) return mArray;

		if( iStart >= 2 ) mLeft = mArray[0 .. iStart - 2];
		else mLeft = mArray[ iSize..iSize ];

		if( iStart + iNum - 1 < iSize )
			mRight = mArray[iStart + iNum - 1 .. sizeof(mArray) - 1];
		else
			mRight = mArray[ iSize..iSize ];
		return mLeft + mRight;
	}
	else return mArray;
}
*/

string base_name( mixed val ) {
	string name, base;

	if( stringp(val) )
		name = val;
	else
		name = file_name(val);

	if( sscanf( name, "%s#%*d", base ) == 2 )
		return base;
	else
		return name;
} /* base_name() */

string TrueName(string cName)
{
	sscanf( cName, "/%s", cName);
	sscanf( cName, "%s.c", cName);

	return "/"+cName+".c";
}

mixed asort( mixed strlist ) {
	    return sort_array( strlist, (: strcmp( $1, $2 ) :) );
} /* asort() */

#define CLEAR_CONTINUE() {iContinue = 0; cLine = "";}

mapping File2Map( string cFile)
{
	int len, iContinue ;

	string content, line, cLine, cKey, cValue, * tmp;
	mapping mpResult = ([]);

	content = read_file(cFile);
	if (!stringp(content))
	{	
		debug_message(sprintf("WARNING: read config file error %s? \n", cFile) );
		return 0;
	}

	content = replace_string(content, "\r", "");

	CLEAR_CONTINUE()
	tmp = explode(content, "\n");

	foreach (line in tmp)
	{
		line = all_trim(line);
		if (line[0] == '#'
			|| ( line[0] == '/' && line[1] == '/')	)
			continue ;

		len = strsrch(line, '#');
		if (len != -1)
		{
			line = line[0..len-1];
		}

		line = replace_string( line, " ", "");
		line = replace_string( line, "\t", "");
		line = all_trim(line);

		if (! strlen(line))
			continue;

		if ( iContinue )
		{
			if ( line[<1] != ';' )
			{
				cLine += line;
				continue;
			}

			if (sscanf(line, "%s=%s", cKey, cValue) == 2)
			{
				debug_message(sprintf("config [%s]syntax error: %s \n", cFile, cLine) );
				CLEAR_CONTINUE()
				continue;
			}

			line = cLine + line;
		}

		if (sscanf(line, "%s=%s", cKey, cValue) != 2)
		{
			debug_message( sprintf("config [%s]syntax error: %s \n", cFile, line) );
			CLEAR_CONTINUE()
			continue;
		}

		cValue = all_trim(cValue);

		if ( cValue[<1] != ';')
		{
			iContinue = 1;
			cLine += line;
			continue ;
		}

		CLEAR_CONTINUE()
		cValue = cValue[0..<2];

		cKey = all_trim(cKey);
		cValue = all_trim(cValue);

		mpResult[atom(cKey)] = atom(cValue);
	}

	return mpResult ;
}

// 
// 函数：将字串转换成时间
// 注释：当 iFlag 缺省或者值为零时，
//          将时间转换成“YYYY-MM-DD HH:MM:SS”格式的时间字符串，
//          否则将时间转换成“YYMMDDHHMMSS”格式的时间字符串。
varargs int string2time(string time, int iFlag )
{
	int year, month, day, hour, min, sec;

	if (iFlag)
	{
		if (sscanf(time, "%4d%2d%2d%2d%2d%2d", year, month, day, hour, min, sec)  != 6 )
		{
			return 0;
		}
	}
	else
	{
		if (sscanf(time, "%d-%d-%d %d:%d:%d", year, month, day, hour, min, sec)  != 6 )
		{
			return 0;
		}
	}

	return mktime(year, month, day, hour, min, sec);
}


