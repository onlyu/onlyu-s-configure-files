
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mysql.h"

#include "lpc.h"
#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "fs_comm.h"

typedef struct {
	unsigned int id;
	MYSQL     *my_conn;
} fs_mysql_conn_t;


typedef struct {
	unsigned int id;
	int        conn_id;               /* reference to connection */
	unsigned numcols;
	array_t *coltypes;
	array_t *colnames;
	array_t *fetch_tmp;
	MYSQL_RES *my_res;
} fs_mysql_cursor_t;

static fs_apr_hash_t conn_base = {NULL, NULL};
static fs_apr_hash_t cursor_base = {NULL, NULL};

static unsigned int s_conn_id = 1;
static unsigned int s_cursor_id = 1;

void fs_mysql_module_init()
{
	fs_apr_hash_construct(&conn_base);
	fs_apr_hash_construct(&cursor_base);
}

static fs_mysql_conn_t *create_conn(MYSQL *my_conn)
{
	unsigned int cid = s_conn_id++;
	fs_mysql_conn_t *conn = calloc(1, sizeof(*conn));
	conn->id = cid;
	conn->my_conn = my_conn;
	apr_hash_set(conn_base.hash, &conn->id, sizeof(conn->id), conn);
	return conn;
}

static void free_conn(fs_mysql_conn_t *conn)
{
	mysql_close(conn->my_conn);
	apr_hash_set(conn_base.hash, &conn->id, sizeof(conn->id), NULL);
	free(conn);
}

static fs_mysql_cursor_t * create_cursor(fs_mysql_conn_t *conn, MYSQL_RES *result, unsigned numcols)
{
	fs_mysql_cursor_t *cur = calloc(1, sizeof(*cur));

	cur->id = s_cursor_id++;
	cur->conn_id = conn->id;
	cur->my_res = result;
	cur->numcols = numcols;
	cur->coltypes = NULL;
	cur->colnames = NULL;;
	cur->fetch_tmp = NULL;
	apr_hash_set(cursor_base.hash, &cur->id, sizeof(cur->id), cur);

	return cur;
}

static void free_cursor(fs_mysql_cursor_t *cur)
{
	mysql_free_result(cur->my_res);
	apr_hash_set(cursor_base.hash, &cur->id, sizeof(cur->id), NULL);
	if (cur->coltypes != NULL) {
		free_array(cur->coltypes);
	}
	if (cur->colnames != NULL) {
		free_array(cur->colnames);
	}
	if (cur->fetch_tmp != NULL) {
		free_array(cur->fetch_tmp);
	}
	free(cur);
}

inline static void cursor_set_colnames(fs_mysql_cursor_t *cur, array_t *colnames)
{
	cur->colnames = colnames;
	add_ref(colnames, "fs mysql");
}

inline static void cursor_set_coltypes(fs_mysql_cursor_t *cur, array_t *coltypes)
{
	cur->coltypes = coltypes;
	add_ref(coltypes, "fs mysql");
}

/*
** Get the internal database type of the given column.
*/
static char *getcolumntype (enum enum_field_types type) 
{
	switch (type) {
		case MYSQL_TYPE_VAR_STRING: case MYSQL_TYPE_STRING:
			return "string";
		case MYSQL_TYPE_TINY: 
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_INT24: 
		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_LONGLONG:
		case MYSQL_TYPE_YEAR: 
			return "int";
		case MYSQL_TYPE_DECIMAL: 
		case MYSQL_TYPE_FLOAT: 
		case MYSQL_TYPE_DOUBLE: 
			return "float";
		case MYSQL_TYPE_TINY_BLOB: 
		case MYSQL_TYPE_MEDIUM_BLOB:
		case MYSQL_TYPE_LONG_BLOB: 
		case MYSQL_TYPE_BLOB:
			return "binary";
		case MYSQL_TYPE_DATE: 
		case MYSQL_TYPE_NEWDATE:
			return "date";
		case MYSQL_TYPE_DATETIME:
			return "datetime";
		case MYSQL_TYPE_TIME:
			return "time";
		case MYSQL_TYPE_TIMESTAMP:
			return "timestamp";
		case MYSQL_TYPE_ENUM: 
		case MYSQL_TYPE_SET:
			return "set";
		case MYSQL_TYPE_NULL:
			return "null";
		default:
			return "undefined";
	}
}


/*
** Connects to a data source.
**     param: one string for each connection parameter, said
**     datasource, username, password, host and port.
*/
void f_mysql_connect() 
{
	char *host;
	char *username;
	char *password;
	char *dbname;
	int port;
	mapping_t *mapconn = allocate_mapping(0);

	SET_ALL_ARGS;

	array_t *arr = ARGS(0)->u.arr;
	svalue_t *args = arr->item;
	if (arr->size != 5) {
		goto error;
	}

	if (args[0].type != T_STRING) {
		goto error;
	}
	host = args[0].u.string;

	if (args[1].type != T_STRING) {
		goto error;
	}
	username = args[1].u.string;

	if (args[2].type != T_STRING) {
		goto error;
	}
	password = args[2].u.string;

	if (args[3].type != T_STRING) {
		goto error;
	}
	dbname = args[3].u.string;

	if (args[4].type != T_NUMBER) {
		goto error;
	}
	port = args[4].u.number;

	MYSQL *my_conn = mysql_init(NULL);
	if (!mysql_real_connect(my_conn, host, username, password, 
		dbname, port, NULL, 0))
	{
		fprintf(stderr, "error connecting to database.host:%s,db:%s,user:%s,port:%d,errstr:%s\n",
			host, dbname, username, port, mysql_error(my_conn));
		mysql_close (my_conn); /* Close conn if connect failed */
		goto error;
	}

	POP_ALL_ARGS;

	fs_mysql_conn_t *conn = create_conn(my_conn);
	add_mapping_string(mapconn, "host", host);
	add_mapping_string(mapconn, "username", username);
	add_mapping_string(mapconn, "password", password);
	add_mapping_string(mapconn, "database", dbname);
	add_mapping_pair(mapconn, "port", port);
	add_mapping_pair(mapconn, "id", conn->id);
	push_refed_mapping(mapconn);
	return;

error:
	POP_ALL_ARGS;
	free_mapping(mapconn);
	push_number(0);

}

void f_mysql_close()
 {
	SET_ALL_ARGS;
	unsigned int cid = ARGS(0)->u.number;
	fs_mysql_conn_t *conn = apr_hash_get(conn_base.hash, &cid, sizeof(cid));
	if (conn == NULL) {
		put_number(-1);
		return;
	}

	free_conn(conn);
	put_number(0);
}

/*
** Execute an SQL statement.
** Return a Cursor object if the statement is a query, otherwise
** return the number of tuples affected by the statement.
*/
void f_mysql_execute() 
{
	SET_ALL_ARGS;
	unsigned int cid = ARGS(0)->u.number;
	char *statement = ARGS(1)->u.string;
	unsigned int st_len = SVALUE_STRLEN(ARGS(1));

	fs_mysql_conn_t *conn = apr_hash_get(conn_base.hash, &cid, sizeof(cid));
	if (conn == NULL) {
		fprintf(stderr, "error no such connection:%d.statement:%s\n", cid, statement);
		goto error;
	}
	
	if (mysql_real_query(conn->my_conn, statement, st_len)) {
		fprintf(stderr, "error executing query.statement:%s,errstr:%s\n", statement, mysql_error(conn->my_conn));
		goto error;
	} else {
		MYSQL_RES *res = mysql_store_result(conn->my_conn);
		unsigned int num_cols = mysql_field_count(conn->my_conn);

		if (res) { /* tuples returned */
			fs_mysql_cursor_t *cur = create_cursor(conn, res, num_cols);
			POP_ALL_ARGS;

			mapping_t *cursor = allocate_mapping(0);
			add_mapping_pair(cursor, "id", cur->id);
			add_mapping_pair(cursor, "numcols", num_cols);
			add_mapping_pair(cursor, "numrows", mysql_num_rows(cur->my_res));
			array_t *coltypes = allocate_array(num_cols);
			array_t *colnames = allocate_array(num_cols);
			MYSQL_FIELD *fields = mysql_fetch_fields(cur->my_res);
			int i;
			for (i = 0; i < cur->numcols; ++i) {
				set_svalue_shared_string(&colnames->item[i], fields[i].name);
				set_svalue_shared_string(&coltypes->item[i], getcolumntype(fields[i].type));
			}
			svalue_t *v = map_insert_str_key(cursor, "coltypes");
			set_svalue_array(v, coltypes);
			v = map_insert_str_key(cursor, "colnames");
			set_svalue_array(v, colnames);
			push_refed_mapping(cursor);
			return;
		} else { /* mysql_use_result() returned nothing; should it have? */
			if(num_cols == 0) { /* no tuples returned */
            	/* query does not return data (it was not a SELECT) */
				//lua_pushnumber(L, mysql_affected_rows(conn->my_conn));
				POP_ALL_ARGS;
				push_number(mysql_affected_rows(conn->my_conn));
				return;
			} else {/* mysql_use_result() should have returned data */
				fprintf(stderr, "error retrieving result.statement:%s,errstr:%s\n", 
					statement, mysql_error(conn->my_conn));
				goto error;
			}
		}
	}

error:
	POP_ALL_ARGS;
	push_number(-1);
}

void f_cursor_close() 
{
	SET_ALL_ARGS;
	unsigned int cur_id = ARGS(0)->u.number;
	fs_mysql_cursor_t *cur = apr_hash_get(cursor_base.hash, &cur_id, sizeof(cur_id));
	if (cur == NULL) {
		goto error;
	}

	free_cursor(cur);
	put_number(0);

error:
	put_number(-1);
}


/*
** Get another row of the given cursor.
*/
void f_cursor_fetch() 
{
	SET_ALL_ARGS;
	unsigned int cur_id = ARGS(0)->u.number;
	POP_ALL_ARGS;

	fs_mysql_cursor_t *cur = apr_hash_get(cursor_base.hash, &cur_id, sizeof(cur_id));
	if (cur == NULL) {
		push_number(-1);
		return;
	}

	MYSQL_RES *res = cur->my_res;
	//unsigned long *lengths;
	MYSQL_ROW row = mysql_fetch_row(res);
	if (row == NULL) {
		/* no more results */
		push_number(0);
		return;
	}

	int i;
	unsigned long *lengths = mysql_fetch_lengths(res);
	array_t *arr = cur->fetch_tmp;
	if (arr == NULL) {
		arr = allocate_array(cur->numcols);
		cur->fetch_tmp = arr;

		for (i = 0; i < cur->numcols; ++i) {
			buffer_t *buf = allocate_buffer(lengths[i]);
			write_buffer(buf, 0, row[i], lengths[i]);
			set_svalue_buffer(&arr->item[i], buf);
		}
	} else {
		for (i = 0; i < cur->numcols; ++i) {
			buffer_t *buf = arr->item[i].u.buf;
			if (buf->size == lengths[i]) {
				write_buffer(buf, 0, row[i], lengths[i]);
			} else {
				free_buffer(buf);
				buffer_t *newbuf = allocate_buffer(lengths[i]);
				write_buffer(newbuf, 0, row[i], lengths[i]);
				set_svalue_buffer(&arr->item[i], newbuf);
			}
		}
	}

	push_array(arr);
}


/*

void f_cursor_colnames() 
{
	SET_ALL_ARGS;
	unsigned int cur_id = ARGS(0)->u.number;
	POP_ALL_ARGS;

	fs_mysql_cursor_t *cur = apr_hash_get(cursor_base.hash, &cur_id, sizeof(cur_id));
	if (cur == NULL) {
		push_number(-1);
		return;
	}

	push_array(cur->colnames);
}

*/
