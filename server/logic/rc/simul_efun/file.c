
int last_modifed( string cFile )
{
	mixed st = stat(cFile);

	 if (! arrayp(st) || sizeof(st) < 2)
	 	return -1 ;

	return st[1];
}

int file_exist( string sFile ) { return file_size(sFile) > 0; }

string user_dbpath(int id) { return sprintf("/db/%02d/%d.dat", id%100, id); }

int user_exist(int id) { return file_exist(user_dbpath(id)); }

int dir_exist( string sPath ) { return file_size(sPath) == -2; }

void assure_file( string sFile ) 
{ 
        string *sDir, sPath = "/";
        int i, iSize;

        if( file_exist(sFile) ) return;

        sDir = explode( sFile, "/" );
        sDir = sDir[ 0..<2 ];
        for( i = 0, iSize = sizeof(sDir); i < iSize; i ++ ) 
        {
                mkdir( sPath += sDir[i] );
                sPath += "/";
        }
}

void mkdirs( string sPath )
{
	if( sPath[<1] == '/' ) 
		assure_file( sPath + "file" );
	else 
		assure_file( sPath + "/file" );
}

#define LOG_DIR "/log/"

int log_file( string sFile, string format, mixed *args... )
{
	sFile = LOG_DIR + sFile + ".log";
	assure_file(sFile);
	string sMsg = sprintf(format, args...);
	return write_file(sFile, sMsg ) ? 1 : 0;
}

string *remove_file_comments( string sFile )
{
	string *sList, *aRes = ({});
	int i, iSize;

	if( !file_exist(sFile) ) return 0;
	if( !(sFile = read_file(sFile)) )  return 0;

	sList = explode( sFile, "\n" );

	for( i = 0, iSize = sizeof(sList); i < iSize; i++ ) 
		if ( sList[i][0] != '#') 
		{	
			aRes += ({ sList[i] });
		}

	return aRes;
}
