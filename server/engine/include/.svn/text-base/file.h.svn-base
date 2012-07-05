#ifndef FILE_H
	#define FILE_H
	#include "marsh.h"
	#include "object.h"
	#include <limits.h>
	/*
	 * file.c
	 */

	typedef struct file_node_s
	{
		struct file_node_s *next;
		struct file_node_s *prev;
		int touch_count;
		char name[128];
		FILE *fp;
	} file_node;

	void debug_perror(char *, char*);

	int legal_path(char*);
	char *check_valid_path(char *, object_t *, char *, int);
	void smart_log(char *, int, char *, int);
	void dump_file_descriptors(outbuffer_t*);

	char *read_file(char *, int, int);
	char *read_bytes(char *, int, int, int*);
	int write_file(char *, char *, int);
	int write_bytes(char *, int, char *, int);
	array_t *get_dir(char *, int);
	int tail(char*);
	int file_size(char*);
	int copy_file(char *, char*);
	int do_rename(char *, char *, int);
	int remove_file(char*);

	#define PRI_LIMIT 10
	file_node *new_fp_node();
	FILE *get_fp_by_name(char *name, int flags);
	void close_all_file();
	void print_list();

	///////////////////////////////////////////////////////////////////////////////
	//以下为写文件操作线程相关
	typedef struct
	{
		FILE *fp;
		short cmd;
		char tmp_name[PATH_MAX];
		char file_name[PATH_MAX];
		m_buffer_t data;
	} FileData;

	int write_init_thread();

	void push_write_packet(FILE *fp, short cmd, const char *tmp_name, const char *file_name, const m_buffer_t *data);
	int read_file_in_queue(const char *file, char **buff);
	int file_size_in_queue(const char *file);

	void write_shut_down();

#endif
