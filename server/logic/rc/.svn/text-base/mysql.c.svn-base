
// return mapping: ok
// rturn 0: error

mixed connect(string host, string user, string passwd, string db, int port)
{
	mixed *conn_arg = ({host, user, passwd, db, port});
	return mysql_connect(conn_arg);
}

void conn_destroy(mapping conn)
{
	mysql_close(conn["id"]);
}

// return mapping:
//		select : cursor
// return int:
//		< 0 : error
//		>= 0: affect rows
// 
mixed execute(mapping conn, string sql)
{
	mixed ret = mysql_execute(conn["id"], sql);
	return ret;
}

void cursor_destroy(mapping cursor)
{
	cursor_close(cursor["id"]);
}


static mixed conver(string type, buffer buf)
{
	string str = read_buffer(buf, 0);
	switch (type) {
	case "int":
		return to_int(str);
	case "float":
		return to_float(str);
	default:
		return str;
		
	}
}

mapping cursor_fetch_map(mapping cursor)
{
	int i;
	int numcols;
	mapping m;
	string *colnames;
	string *coltypes;

	buffer *res = cursor_fetch(cursor["id"]);
	if (res == 0) {
		return 0;
	}

	m = ([]);
	colnames = cursor["colnames"];
	coltypes = cursor["coltypes"];
	numcols = cursor["numcols"];

	for (i = 0; i < numcols; i++) {
		m[colnames[i]] = conver(coltypes[i], res[i]);
	}
	return m;
}

mixed *cursor_fetch_array(mapping cursor)
{
	int i;
	int numcols;
	mixed *arr;
	string *colnames;
	string *coltypes;

	buffer *res = cursor_fetch(cursor["id"]);
	if (res == 0) {
		return 0;
	}

	colnames = cursor["colnames"];
	coltypes = cursor["coltypes"];
	numcols = cursor["numcols"];
	arr = allocate(numcols);

	for (i = 0; i < numcols; i++) {
		arr[i] = conver(coltypes[i], res[i]);
	}

	return arr;
}

