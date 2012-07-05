

#include <log.h>

static string log_file_;
static int level_;
static mapping db_table_;

class table_info_t {
	string table_format;
	string table_file;
}

void log_(int level, string arg_format, mixed *args...)
{
	if (level >= level_) {
		string format;
		switch (level) {
			case LOG_LEVEL_DEBUG:
				format = sprintf("[%%s][debug] %s\n", arg_format);
				break;
			case LOG_LEVEL_WARN:
				format = sprintf("[%%s][warn] %s\n", arg_format);
				break;
			case LOG_LEVEL_ERROR:
				format = sprintf("[%%s][error] %s\n", arg_format);
				break;
			case LOG_LEVEL_INFO:
			default:
				format = sprintf("[%%s] %s\n", arg_format);
		}
		log_file(log_file_, sprintf(format, short_time(), args...));
	}
}


varargs void SetDBFormat(string table, string table_format, string file)
{
	string format = sprintf("[%%s][%%s]%s\n", table_format);
	class table_info_t table_info = new(class table_info_t);
	table_info->table_format = format;
	table_info->table_file = file == 0 ? LOG_DB_FILE : file;
	db_table_[table] = table_info;
}


int DB(string table, mixed *args...)
{
	class table_info_t info  = db_table_[table];
	if (info) {
		log_file(info->table_file, sprintf(info->table_format, short_time(), table, args...));
		return 1;
	}
	return 0;
}


void Error(string arg_format, mixed *args...)
{
	log_(LOG_LEVEL_ERROR, arg_format, args...);
}

void Warn(string arg_format, mixed *args...)
{
	log_(LOG_LEVEL_WARN, arg_format, args...);
}

void Info(string arg_format, mixed *args...)
{
	log_(LOG_LEVEL_INFO, arg_format, args...);
}

void Log(string arg_format, mixed *args...)
{
	log_(LOG_LEVEL_INFO, arg_format, args...);
}

void Debug(string arg_format, mixed *args...)
{
	log_(LOG_LEVEL_DEBUG, arg_format, args...);
}


void create(string file, int level)
{
	log_file_ = file;
	level_ = level;
	db_table_ = ([]);
}

object New(string file, int level)
{
	object obj = new("/module/log", file, level);
	return obj;
}

