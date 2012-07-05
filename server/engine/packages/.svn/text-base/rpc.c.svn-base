

#include "fs_rpc.h"
#include "fs_rpc_c.h"
#include "fs_mbuf.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define CHECK_BUFF(buf, len) do { \
	if (EVBUFFER_LENGTH((buf)) < ((unsigned)(len))) return -1; \
} while (0)

int g_log_pto_enable = 0;

static char * type_name[] = {"rpc_int", "rpc_string", "rpc_class", "rpc_object",};

char *fs_rpc_type_name(int type)
{
	if (type >= 0 && type < sizeof(type_name)/sizeof(char*)) {
		return type_name[type];
	}

	return "rpc_unknow_type";
}

char *fs_rpc_field_type_name(fs_rpc_field_t *field)
{
	switch (field->type) {
		case FS_RPC_CLASS:
			return field->class_ptr->name;
		default:
			return fs_rpc_type_name(field->type);
	}
}

static void fs_rpc_set_class_ptr(fs_rpc_table_t *table)
{
	int i, j;
	fs_rpc_class_table_t *class_table = table->class_table;
	fs_rpc_function_table_t *function_table = table->function_table;

	if (class_table == NULL) return;

	for (i = 0; i < class_table->elts; ++i) {
		fs_rpc_class_t *cls = &class_table->rpc_class[i];
		for (j = 0; j < cls->field_count; ++j) {
			fs_rpc_field_t *field = &cls->field[j];
			if (field->class_index >= 0) 
				field->class_ptr = &class_table->rpc_class[field->class_index];
		}
	}

	for (i = 0; i < function_table->elts; ++i) {
		fs_rpc_function_t *function = &function_table->rpc_function[i];
		fs_rpc_class_t *arg_cls = &function->arg_class; 
		for (j = 0; j < arg_cls->field_count; ++j) {
			fs_rpc_field_t *field = &arg_cls->field[j];
			if (field->class_index >= 0) 
				field->class_ptr = &class_table->rpc_class[field->class_index];
		}
	}
}

void class_free(fs_rpc_class_t *cls)
{
	if (cls != NULL) {
		int i;
		for(i = 0; i < cls->field_count; ++i) {
			fs_rpc_field_t *field = &cls->field[i];
			free(field->name);
		}
		free(cls->field);
		free(cls->name);
	}
}

static void free_function(fs_rpc_function_t *function)
{
	if (function != NULL) {
		free(function->module);
		class_free(&function->arg_class);
	}
}

void fs_rpc_table_destroy(fs_rpc_table_t *table)
{
	int i;
	fs_rpc_class_table_t *class_table = table->class_table;
	fs_rpc_function_table_t *function_table = table->function_table;

	if (class_table != NULL) {
		for (i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			class_free(cls);
		}
		free(class_table);
	}

	if (function_table != NULL) {
		for (i = 0; i < function_table->elts; ++i) {
			fs_rpc_function_t *function = &function_table->rpc_function[i];
			free_function(function);
		}

		free(function_table);
	}

	memset(table, 0, sizeof(*table));
}

void fs_rpc_foreach_function(fs_rpc_function_table_t *table, int (*action)(fs_rpc_function_t *fun, void *udata), void *udata)
{
	int i;
	for(i = 0; i < table->elts; ++i) {
		fs_rpc_function_t *function = &table->rpc_function[i];
		if(action(function, udata) == 0)
			break;
	}
}

static char * strupper(char *low)
{
	char *p;
	for(p = low; *p != '\0'; p++) {
		*p = toupper(*p);
	}
	return low;
}

int fs_rpc_dump_function_c_id(fs_rpc_function_table_t *table, const char *file)
{
	FILE *f ;
	int close, i;

	if (file == NULL) {
		f = stdout;
		close = 0;
	} else {
		f = fopen(file, "w");
		close = 1;
	}

	if (f == NULL) {
		perror("fail to open file for dumping");
		fprintf(stderr, "fail file:%s\n", file);
		return -1;
	}
	
	if (table != NULL) {
		fprintf(f, "#ifndef _FS_RPC_ID_\n");
		fprintf(f, "#define _FS_RPC_ID_\n\n");

		for(i = 0; i < table->elts; ++i) {
			fs_rpc_function_t *function = &table->rpc_function[i];
			fs_rpc_class_t *arg_cls = &function->arg_class;
			if (function->function_id < FS_RPC_LPC_PID_START ) {
				fprintf(f, "#define %s %d\n", strupper(arg_cls->name), function->function_id);
			}
		}

		fprintf(f, "\n#endif /*_FS_RPC_ID_*/\n");
	}

	if (close) {
		fclose(f);
	}

	return 0;
}

int fs_rpc_dump_function_id(fs_rpc_function_table_t *table, const char *file)
{
	FILE *f ;
	int close, i;

	if (file == NULL) {
		f = stdout;
		close = 0;
	} else {
		f = fopen(file, "w");
		close = 1;
	}

	if (f == NULL) {
		perror("fail to open file for dumping");
		fprintf(stderr, "fail file:%s\n", file);
		return -1;
	}
	
	if (table != NULL) {
		fprintf(f, "#ifndef _FS_RPC_ID_\n");
		fprintf(f, "#define _FS_RPC_ID_\n\n");

		for(i = 0; i < table->elts; ++i) {
			fs_rpc_function_t *function = &table->rpc_function[i];
			fs_rpc_class_t *arg_cls = &function->arg_class;
			fprintf(f, "#define %s %d\n", strupper(arg_cls->name), function->function_id);
		}

		fprintf(f, "\n#endif /*_FS_RPC_ID_*/\n");
	}

	if (close) {
		fclose(f);
	}

	return 0;
}

int fs_rpc_dump_cfg(fs_rpc_table_t *table, const char *file)
{
	FILE *f ;
	int close, i, j;
	fs_rpc_class_table_t *class_table = table->class_table;
	fs_rpc_function_table_t *function_table = table->function_table;

	if (file == NULL) {
		f = stdout;
		close = 0;
	} else {
		f = fopen(file, "w");
		close = 1;
	}

	if (f == NULL) {
		perror("fail to open file for dumping");
		fprintf(stderr, "fail file:%s\n", file);
		return -1;
	}
	
	if (class_table != NULL) {
		fprintf(f, "class_table_num:%d\n", class_table->elts);
		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			fprintf(f, "field_count:%d,c_imp:%d,class_name=%s\n", cls->field_count, cls->c_imp, cls->name);
			for(j = 0; j < cls->field_count; ++j) {
				fs_rpc_field_t *field = &cls->field[j];
				fprintf(f, "field_type:%d,class_index:%d,array:%d,field_name:%s\n", 
					field->type, field->class_index, field->array, field->name);
			}
		}
	}

	if (function_table != NULL) {
		fprintf(f, "function_table_num:%d\n", function_table->elts);
		for(i = 0; i < function_table->elts; ++i) {
			fs_rpc_function_t *function = &function_table->rpc_function[i];
			fs_rpc_class_t *arg_cls = &function->arg_class;
			fprintf(f, "function_id:%d,c_imp:%d,arg_count:%d,module:%s,function_name:%s\n", 	
				function->function_id, arg_cls->c_imp, arg_cls->field_count, function->module, arg_cls->name);
			for(j = 0; j < arg_cls->field_count; ++j) {
				fs_rpc_field_t *field = &arg_cls->field[j];
				fprintf(f, "arg_type:%d,class_index:%d,array:%d\n", 
					field->type, field->class_index, field->array);
			}
		}
	}

	if (close) {
		fclose(f);
	}

	return 0;
}

static void dump_c_declaration(fs_rpc_function_table_t *table, char *kind, FILE *f)
{
	//rpc c function
	int i;
	for (i = 0; i < table->elts; ++i) {
		fs_rpc_function_t *function = &table->rpc_function[i];
		fs_rpc_class_t *arg_cls = &function->arg_class;
		if (!arg_cls->c_imp) 
			continue;
		// only accept class arg
		if (strstr(arg_cls->name, kind)) {
			if (FS_RPC_CHECK_C_DECL(arg_cls)) 
				fprintf(f, "void %s(int, %s *);\n", arg_cls->name, arg_cls->field[1].class_ptr->name);
			else 
				fprintf(stderr, "error:%s c rpc only accept two args\n", arg_cls->name);
		}
	}
}

static int fs_rpc_dump_c_h(fs_rpc_table_t *table, const char *file)
{
	FILE *f ;
	int close, i, j;
	fs_rpc_class_table_t *class_table = table->class_table;
	fs_rpc_function_table_t *function_table = table->function_table;

	if (file == NULL) {
		f = stdout;
		close = 0;
	} else {
		f = fopen(file, "w");
		close = 1;
	}

	if (f == NULL) {
		perror("fail to open file for dumping rpc c header");
		fprintf(stderr, "fail file:%s\n", file);
		return -1;
	}
	
	fprintf(f, "\n#ifndef _FS_RPC_C_H\n");
	fprintf(f, "#define _FS_RPC_C_H\n");

	fprintf(f, "\n#include \"fs_rpc.h\"");
	fprintf(f, "\n#include \"fs_rpc_id.h\"\n\n");
	fprintf(f, "extern fs_rpc_class_meta_t fs_rpc_class_metas[];\n");
	fprintf(f, "extern fs_rpc_function_meta_t fs_rpc_function_metas[];\n");
	fprintf(f, "\n");
	
	if (class_table != NULL) {
		// typedef struct
		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			if (!cls->c_imp) continue;
			fprintf(f, "typedef struct %s %s;\n", cls->name, cls->name);
		}

		// array union 
		fprintf(f, "\n#define FS_RPC_C_TYPE \\\n");
		fprintf(f, "\tvoid *data;\t\\\n");
		fprintf(f, "\tfs_rpc_int_t *intp;\t\\\n");
		fprintf(f, "\tfs_rpc_string_t *stringp;\t\\\n");

		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			if (!cls->c_imp) continue;
			if (i < class_table->elts - 1) {
				fprintf(f, "\t%s *%sp;\t\\\n", cls->name, cls->name);
			} else {
				fprintf(f, "\t%s *%sp;\n", cls->name, cls->name);
			}
		}

		fprintf(f, "\ntypedef struct fs_rpc_array_s {;\n");
		fprintf(f, "\tfs_rpc_size_t n;\n");
		fprintf(f, "\tunion {\n");
		fprintf(f, "\tFS_RPC_C_TYPE\n");
		fprintf(f, "\t}u;\n");
		fprintf(f, "} fs_rpc_array_t;\n\n");

		// c struct define
		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			if (!cls->c_imp) continue;
			fprintf(f, "struct %s {\n", cls->name);
			for(j = 0; j < cls->field_count; ++j) {
				fs_rpc_field_t *field = &cls->field[j];
				if (field->array == FS_RPC_ARRAY_VAR) {
					switch (field->type) {
					case FS_RPC_INT:
						fprintf(f, "\tfs_rpc_array_t %s;\n", field->name);
						break;
					case FS_RPC_STRING:
						fprintf(f, "\tfs_rpc_array_t %s;\n", field->name);
						break;
					case FS_RPC_CLASS:
						fprintf(f, "\tfs_rpc_array_t %s;\n", field->name);
						break;
					}
				} else {
					switch (field->type) {
					case FS_RPC_INT:
						fprintf(f, "\tint %s;\n", field->name);
						break;
					case FS_RPC_STRING:
						fprintf(f, "\tfs_rpc_string_t %s;\n", field->name);
						break;
					case FS_RPC_CLASS:
						fprintf(f, "\t%s %s;\n", field->class_ptr->name, field->name);
						break;
					}
				}

			}
			fprintf(f, "};\n\n");
		}
	}


	if (function_table != NULL) {
		fprintf(f, "#ifdef __FreeBSD__\n");
		dump_c_declaration(function_table, FS_RPC_SERVER_PREFIX, f);
		fprintf(f, "#elif MS_WINDOWS\n");
		dump_c_declaration(function_table, FS_RPC_CLIENT_PREFIX, f);
		fprintf(f, "#else\n");
		fprintf(f, "#error \"not support platform\"\n");
		fprintf(f, "#endif\n");
	}

	fprintf(f, "\n#endif /*_FS_RPC_C_H*/\n");

	if (close) {
		fclose(f);
	}

	return 0;
}


static void dump_c_function_meta(fs_rpc_function_table_t *table, char *kind, FILE *f)
{
	//rpc c function
	int i;

	fprintf(f, "fs_rpc_function_meta_t fs_rpc_function_metas[] = {\n");
	for (i = 0; i < table->elts; ++i) {
		fs_rpc_function_t *function = &table->rpc_function[i];
		fs_rpc_class_t *arg_cls = &function->arg_class;
		if (!arg_cls->c_imp) 
			continue;
		// only accept class arg
		assert(FS_RPC_CHECK_C_DECL(arg_cls));
		if (strstr(arg_cls->name, kind)) {
			fprintf(f, "\t{\"%s\", (fs_rpc_c_function_t)%s,},\n", arg_cls->name, arg_cls->name);
		}
	}
	fprintf(f, "\t{NULL, NULL,},\n");
	fprintf(f, "};\n");
}

static int fs_rpc_dump_meta(fs_rpc_table_t *table, const char *file)
{
	FILE *f ;
	int close, i, j;
	fs_rpc_class_table_t *class_table = table->class_table;
	fs_rpc_function_table_t *function_table = table->function_table;

	if (file == NULL) {
		f = stdout;
		close = 0;
	} else {
		f = fopen(file, "w");
		close = 1;
	}

	if (f == NULL) {
		perror("fail to open file for dumping");
		fprintf(stderr, "fail file:%s\n", file);
		return -1;
	}
	

	fprintf(f, "\n#include \"fs_rpc_c.h\"\n\n");
	fprintf(f, "\n");

	if (class_table != NULL) {
		//field_meta, class defined 
		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			if (!cls->c_imp) 
				continue;
		
			//gen the field_meta
			fprintf(f, "static fs_rpc_field_meta_t %s_fields[] = {\n", cls->name);
			for(j = 0; j < cls->field_count; ++j) {
				fs_rpc_field_t *field = &cls->field[j];
				//fprintf(f, "\t{\"%s\", offsetof(struct %s, %s),},\n", field->name, cls->name, field->name);
				fprintf(f, "\t{\"%s\", offsetof(%s, %s),},\n", field->name, cls->name, field->name);
			}
			fprintf(f, "};\n\n"); 
		}

		//class meta
		fprintf(f, "fs_rpc_class_meta_t fs_rpc_class_metas[] = {\n");
		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			if (!cls->c_imp) 
				continue;
			char *clsname = cls->name;
			//gen name, c_size, field_count, field_meta
			//fprintf(f, "\t{\"%s\", sizeof(struct %s), %d, %s_fields,},\n", clsname, clsname, cls->field_count, clsname);
			fprintf(f, "\t{\"%s\", sizeof(%s), %d, %s_fields,},\n", clsname, clsname, cls->field_count, clsname);
		}
		fprintf(f, "\t{NULL, 0, 0, NULL,},\n");
		fprintf(f, "}; \n\n"); //class_meta_end

	}

	if (function_table != NULL) {
		//function meta
		fprintf(f, "#ifdef __FreeBSD__\n");
		dump_c_function_meta(function_table, FS_RPC_SERVER_PREFIX, f);
		fprintf(f, "#elif MS_WINDOWS\n");
		dump_c_function_meta(function_table, FS_RPC_CLIENT_PREFIX, f);
		fprintf(f, "#else\n");
		fprintf(f, "#error \"not support platform\"\n");
		fprintf(f, "#endif\n");
	}

	if (close) {
		fclose(f);
	}

	return 0;
}

int fs_rpc_dump_c(fs_rpc_table_t *table, const char *hfile, const char* cfile)
{
 	fs_rpc_set_class_ptr(table);
	fs_rpc_dump_c_h(table, hfile);
	fs_rpc_dump_meta(table, cfile);
	return 0;
}

static fs_rpc_class_meta_t *find_class_meta(char *clsname)
{
	fs_rpc_class_meta_t *meta = NULL;

	for (meta = fs_rpc_class_metas; meta->name != NULL; ++meta) {
		if (0 == strcmp(clsname, meta->name)) {
			return meta;
		}
	}

	return NULL;
}

static fs_rpc_function_meta_t *find_function_meta(char *funname)
{
	fs_rpc_function_meta_t *meta = NULL;

	for (meta = fs_rpc_function_metas; meta->name != NULL; ++meta) {
		if (0 == strcmp(funname, meta->name)) {
			return meta;
		}
	}

	return NULL;
}

static void fill_meta(fs_rpc_table_t *table)
{
	//rpc c function
	int i, j;
	fs_rpc_class_meta_t *class_meta;
	fs_rpc_function_meta_t *function_meta;
	fs_rpc_field_meta_t *field_meta;
	fs_rpc_class_t *cls;

	fs_rpc_function_table_t *function_table = table->function_table;
	fs_rpc_class_table_t *class_table = table->class_table;
	
	if (class_table != NULL) {
		/* 由于 c 的rpc 函数只接收结构体作为参数，所以就不用对函数的arg_cls进行c_size和c_offset的填充.  
		* arg_cls 本身也是一个cls，但是它们存在function_table里而不是存在class_table里.
		*/
		for(i = 0; i < class_table->elts; ++i) {
			cls = &class_table->rpc_class[i];
			if (!cls->c_imp) 
				continue;
			class_meta = find_class_meta(cls->name);
			if (class_meta == NULL) {
				fprintf(stderr, "error:no metaclass for:%s\n", cls->name);
			}
			assert(class_meta);
			//printf("fill meta cls:%s,%d,%s,%d\n", cls->name, cls->c_size, class_meta->name, class_meta->c_size);
			cls->c_size = class_meta->c_size;
			field_meta = class_meta->field_meta;
			for (j = 0; j < cls->field_count; ++j) {
				cls->field[j].c_offset = field_meta[j].c_offset;
			}
		}
	}

	if (function_table != NULL) {
		for(i = 0; i < function_table->elts; ++i) {
			fs_rpc_function_t *function = &function_table->rpc_function[i];
			fs_rpc_class_t *arg_cls = &function->arg_class;
			if (!arg_cls->c_imp) 
				continue;
			//fill the c function meta
			function_meta = find_function_meta(arg_cls->name);
			if (function_meta != NULL) //分服务端和客户端，只实现一端的声明函数。
				function->c_function = function_meta->c_function;
		}
	}
}

static void check_duplicated_name(fs_rpc_table_t *table)
{
	int i, j;
	fs_rpc_function_table_t *function_table = table->function_table;
	fs_rpc_class_table_t *class_table = table->class_table;
	unsigned c;

	if (class_table != NULL) {
		for(i = 0; i < class_table->elts; ++i) {
			fs_rpc_class_t *cls = &class_table->rpc_class[i];
			for(j = 0, c = 0; j < class_table->elts; ++j) {
				if (!strcmp(cls->name, class_table->rpc_class[j].name))
					c++;
			}
			if (c > 1) {
				fprintf(stderr, "error!rpc declarationhas duplicated class:%s\n", cls->name);
				assert(0);
			}
		}
	}

	if (function_table != NULL) {
		for(i = 0; i < function_table->elts; ++i) {
			fs_rpc_function_t *function = &function_table->rpc_function[i];
			fs_rpc_class_t *arg_cls = &function->arg_class;
			for(j = 0, c = 0; j < function_table->elts; ++j) {
				if (!strcmp(arg_cls->name, function_table->rpc_function[j].arg_class.name))
					c++;
			}
			if (c > 1) {
				fprintf(stderr, "error!rpc declarationhas duplicated function:%s\n", arg_cls->name);
				assert(0);
			}
		}
	}
}

static void check_rpc_table(fs_rpc_table_t *table)
{
	check_duplicated_name(table);
	//rpc c function
	int i, j;
	fs_rpc_class_meta_t *class_meta;
	fs_rpc_function_meta_t *function_meta;
	fs_rpc_field_meta_t *field_meta;
	fs_rpc_class_t *cls;

	fs_rpc_function_table_t *function_table = table->function_table;
	fs_rpc_class_table_t *class_table = table->class_table;
	
	if (class_table != NULL) {
		for(i = 0; i < class_table->elts; ++i) {
			cls = &class_table->rpc_class[i];
			if (!cls->c_imp) 
				continue;
			class_meta = find_class_meta(cls->name);
			assert(class_meta);
			assert(0 == strcmp(class_meta->name, cls->name));
			assert(class_meta->field_count == cls->field_count);

			assert(cls->c_size == class_meta->c_size);
			field_meta = class_meta->field_meta;
			for (j = 0; j < cls->field_count; ++j) {
				assert(0 == strcmp(cls->field[j].name, field_meta[j].name));
				assert(cls->field[j].c_offset == field_meta[j].c_offset);
			}
		}
	}

	if (function_table != NULL) {
		for(i = 0; i < function_table->elts; ++i) {
			fs_rpc_function_t *function = &function_table->rpc_function[i];
			fs_rpc_class_t *arg_cls = &function->arg_class;
			if (!arg_cls->c_imp) 
				continue;
			//fill the c function meta
			function_meta = find_function_meta(arg_cls->name);
			if (function_meta == NULL) 
				continue;
			assert(function->c_function == function_meta->c_function);

			//assert the arg_cls meta info
			//only accept one struct argument for c rpc, except the uid
			assert(FS_RPC_CHECK_C_DECL(arg_cls));
			cls = FS_RPC_STRUCT_ARG(arg_cls);
			class_meta = find_class_meta(cls->name);
			//c implemented rpc function must have the class meta info
			//printf("check arg cls:%s,%d,%s,%d\n", cls->name, cls->c_size, class_meta->name, class_meta->c_size);
			assert(class_meta);
			assert(cls->c_size == class_meta->c_size);
			assert(0 == strcmp(class_meta->name, cls->name));
			assert(class_meta->field_count == cls->field_count);

			field_meta = class_meta->field_meta;
			for (j = 0; j < cls->field_count; ++j) {
				assert(0 == strcmp(cls->field[j].name, field_meta[j].name));
				assert(cls->field[j].c_offset == field_meta[j].c_offset);
			}

		}
	}
}

int fs_rpc_table_create(fs_rpc_table_t *table, const char *file)
{
	FILE *f;
	int i, j;
	int tmp;
	fs_rpc_class_table_t *ctbl;
	fs_rpc_function_table_t *ftbl;
	int max = 2048;
	char buf[max];
	char buf2[max];
	memset(buf, 0, max);
	memset(buf2, 0, max);

	f = fopen(file, "r");
	if (f == NULL) {
		perror("fail to open file for reading.");
		fprintf(stderr, "fail file:%s\n", file);
		return -1;
	}
	
	fscanf(f, "class_table_num:%d\n", &tmp);
	ctbl = (fs_rpc_class_table_t*)calloc(1, sizeof(fs_rpc_class_table_t) + sizeof(fs_rpc_class_t) * tmp);
	ctbl->elts = tmp;
	ctbl->rpc_class = (fs_rpc_class_t*)((char*)ctbl + sizeof(fs_rpc_class_table_t));

	for (i = 0; i < ctbl->elts; ++i) {
		fs_rpc_class_t *cls = &ctbl->rpc_class[i];
		fscanf(f, "field_count:%d,c_imp:%d,class_name=%s\n", &cls->field_count, &cls->c_imp, buf);
		cls->name = strdup(buf);
		cls->field = (fs_rpc_field_t*)calloc(cls->field_count, sizeof(fs_rpc_field_t));

		for(j = 0; j < cls->field_count; ++j) {
			fs_rpc_field_t *field = &cls->field[j];
			field->parent = cls;
			memset(buf, 0, max);
			fscanf(f, "field_type:%d,class_index:%d,array:%d,field_name:%s\n", 
				&field->type, &field->class_index, &field->array, buf);
			field->name = strdup(buf);
		}
	}

	fscanf(f, "function_table_num:%d\n", &tmp);
	ftbl = (fs_rpc_function_table_t*)calloc(1, sizeof(fs_rpc_function_table_t) + sizeof(fs_rpc_function_t) * tmp);
	ftbl->elts = tmp;
	ftbl->rpc_function = (fs_rpc_function_t*)((char*)ftbl + sizeof(fs_rpc_function_t));

	for(i = 0; i < ftbl->elts; ++i) {
		fs_rpc_function_t *function = &ftbl->rpc_function[i];
		fs_rpc_class_t *arg_cls = &function->arg_class;
		memset(buf, 0, max);
		memset(buf2, 0, max);
		fscanf(f, "function_id:%d,c_imp:%d,arg_count:%d,module:%[^,]%*cfunction_name:%s\n", 	
				 &function->function_id, &arg_cls->c_imp, &arg_cls->field_count, buf, buf2);
		function->module = strdup(buf);
		arg_cls->name = strdup(buf2);
		arg_cls->field = (fs_rpc_field_t*)calloc(arg_cls->field_count, sizeof(fs_rpc_field_t));

		for(j = 0; j < arg_cls->field_count; ++j) {
			fs_rpc_field_t *field = &arg_cls->field[j];
			field->parent = arg_cls;
			fscanf(f, "arg_type:%d,class_index:%d,array:%d\n", &field->type, &field->class_index, &field->array);
		}
	}

	table->class_table = ctbl;
	table->function_table = ftbl;
	fs_rpc_set_class_ptr(table);
	fill_meta(table);
	check_rpc_table(table);
	fs_rpc_function_sort(table->function_table);

	fclose(f);

	return 0;
}

fs_rpc_class_t *fs_rpc_class(fs_rpc_class_table_t *table, int index)
{
	return (0 <= index && index < table->elts) ? 
		&table->rpc_class[index] : NULL;
}

fs_rpc_function_t *fs_rpc_function(fs_rpc_function_table_t *table, int index)
{
	return (0 <= index && index < table->elts) ? 
		&table->rpc_function[index] : NULL;
}


int fs_rpc_class_index(fs_rpc_class_table_t *table, const char *name)
{
	int i;
	if (name != NULL) {
		for (i = 0; i < table->elts; ++i) {
			fs_rpc_class_t *cls = &table->rpc_class[i];
			if (0 == strcmp(cls->name, name)) {
				return i;
			}
		}
	}

	return -1;
}


fs_rpc_function_t *fs_rpc_function_find(fs_rpc_function_table_t *table, const char *name)
{
	int i;
	if (name != NULL) {
		for (i = 0; i < table->elts; ++i) {
			fs_rpc_function_t *function = &table->rpc_function[i];
			if (0 == strcmp(function->arg_class.name, name)) {
				return  function;
			}
		}
	}

	return NULL;
}


int fs_rpc_function_set_id(fs_rpc_function_table_t *function_table, const char **names, const int *ids, size_t len)
{
	size_t i;
	for (i = 0; i < len; ++i) {
		fs_rpc_function_t *function = fs_rpc_function_find(function_table, names[i]);
		if (function == NULL) 
			return -1;
		function->function_id = ids[i];
	}
	return 0;
}

static int sort_function(const void *a, const void *b)
{
	return ((fs_rpc_function_t*)a)->function_id - ((fs_rpc_function_t*)b)->function_id;
}

void fs_rpc_function_qsort(fs_rpc_function_table_t *function_table)
{
	qsort(function_table->rpc_function, function_table->elts, sizeof(fs_rpc_function_t), sort_function);
}

void fs_rpc_function_sort(fs_rpc_function_table_t *function_table)
{
	fs_rpc_function_qsort(function_table);
	if (function_table->elts == 0) {
		function_table->sparse_elts = 0;
		function_table->sparse_rpc_function = NULL;
	} else {
		fs_rpc_function_t *last = &function_table->rpc_function[function_table->elts - 1];
		function_table->sparse_elts = last->function_id + 1;
		function_table->sparse_rpc_function = calloc(function_table->sparse_elts, sizeof(fs_rpc_function_t));

		int i;
		for(i = 0; i < function_table->elts; ++i) {
			fs_rpc_function_t *function = &function_table->rpc_function[i];
			function_table->sparse_rpc_function[function->function_id] = function;
		}
	}
}

static int search_function(const void *function_id, const void *function)
{
	return *(int*)function_id - ((fs_rpc_function_t*)function)->function_id;
}

fs_rpc_function_t *fs_rpc_function_bsearch(fs_rpc_function_table_t *function_table, fs_protocol_id_t function_id)
{
	return	(fs_rpc_function_t*)bsearch(&function_id, function_table->rpc_function, 
			function_table->elts, sizeof(fs_rpc_function_t), search_function);
}

fs_rpc_function_t *fs_rpc_function_at(fs_rpc_function_table_t *function_table, fs_protocol_id_t function_id)
{
	if (function_id >= 0 && function_id < function_table->sparse_elts) {
		return function_table->sparse_rpc_function[function_id];
	}
	return NULL;
}

//////////////////////////
// 不能是定长数组 即不能[const]; 不能直接用 c 的原始指针 即 * ;  只支持一维数组,数组用fs_rpc_array_t 表示.


inline static int get_field_size(fs_rpc_field_t *field)
{
	switch (field->type) {
	case FS_RPC_INT: {
		return sizeof(fs_rpc_int_t);
	}
	case FS_RPC_STRING: {
		return sizeof(fs_rpc_string_t);
	}
	case FS_RPC_CLASS: {
		return field->class_ptr->c_size;
	}
	default:
		assert(0);
		return -1;
	}
}

inline static int unpack_struct(fs_rpc_field_t *field, int field_count, fs_mbuf_t *mbuf, char *struct_value, fs_inbuf_t*inbuf);

inline int static unpack_field(fs_rpc_field_t *field, fs_mbuf_t *mbuf, char *value, fs_inbuf_t *inbuf)
{
	switch (field->type) {
	case FS_RPC_INT: {
		fs_rpc_int_t *v = (fs_rpc_int_t*)value;
		FS_INBUF_GET_TYPE(inbuf, v, fs_rpc_int_t);
		break;
	}
	case FS_RPC_STRING: {
		fs_rpc_string_t *ptr = (fs_rpc_string_t*)value;
		FS_INBUF_GET_TYPE(inbuf, &ptr->n, fs_rpc_size_t);
		if (ptr->n <= 0) { 
			ptr->str = NULL;
		} else {
			//ptr->str = (char*)calloc(ptr->n + sizeof(*ptr->str), sizeof(*ptr->str)); //append the extra '\0'
			//ptr->str = fs_mbuf_enq(mbuf, NULL, (ptr->n + 1)*sizeof(*ptr->str)); //append the extra '\0'
			FS_INBUF_CHECK(inbuf, ptr->n);
			ptr->str = FS_MBUF_ALLOC(mbuf, (ptr->n + 1)*sizeof(*ptr->str)); //append the extra '\0'
			FS_INBUF_GET_NO_CHECK(inbuf, ptr->str, ptr->n);
			ptr->str[ptr->n] = '\0';
		}
		break;
	}
	case FS_RPC_CLASS: {
		fs_rpc_class_t *cls = field->class_ptr;
		if (unpack_struct(cls->field, cls->field_count, mbuf, value, inbuf))
			return -1;
		break;
	}
	default:
		return -1;
	}
	return 0;
}

inline static int unpack_struct(fs_rpc_field_t *field, int field_count, fs_mbuf_t *mbuf, char *struct_value, fs_inbuf_t *inbuf)
{
	unsigned i;
	int c;
	char *field_value;
	for (c = 0; c < field_count; ++c, ++field) {
		field_value = struct_value + field->c_offset;
		if (field->array == FS_RPC_ARRAY_VAR) {
			// pointer field of the struct
			fs_rpc_array_t *ptr = (fs_rpc_array_t*)field_value;
			FS_INBUF_GET_TYPE(inbuf, &ptr->n, fs_rpc_size_t);

			if (ptr->n <= 0) {
				ptr->u.data = NULL;
			} else {
				char *data;
				//todo: nned to check the real element size?
				FS_INBUF_CHECK(inbuf, ptr->n);
				size_t fsz = get_field_size(field);
				//ptr->u.data = calloc(ptr->n, fsz);
				//ptr->u.data = fs_mbuf_enq(mbuf, NULL, ptr->n * fsz);
				ptr->u.data = FS_MBUF_ALLOC(mbuf, ptr->n * fsz);
				for (i = 0, data = (char*)ptr->u.data; i < ptr->n; ++i, data += fsz) {
					if (unpack_field(field, mbuf, data, inbuf))
						return -1;
				}
			}

		} else {
			if (unpack_field(field, mbuf, field_value, inbuf))
				return -1;
		}
	}
	return 0;
}


void * fs_rpc_c_unpack(fs_rpc_function_t *function, fs_mbuf_t *mbuf, fs_inbuf_t *inbuf)
{
	//skip the vfd, get the struct arg from the arg list
	fs_rpc_class_t *cls = FS_RPC_STRUCT_ARG(&function->arg_class);
	//void *struct_value = fs_mbuf_enq(mbuf, NULL, cls->c_size);
	//printf("c unpack %s.%s %d\n", function->arg_class.name, cls->name, cls->c_size);
	void *struct_value = FS_MBUF_ALLOC(mbuf, cls->c_size);
	if(unpack_struct(cls->field, cls->field_count, mbuf, struct_value, inbuf))
		return NULL;

	//log_unpack(function, inbuf);
	return struct_value;
}


////////////////////
inline static int pack_struct(fs_rpc_field_t *field, int field_count, void *struct_value, fs_mbuf_t *mbuf);

inline static int pack_field(fs_rpc_field_t *field, void *value, fs_mbuf_t *mbuf)
{
	switch (field->type) {
	case FS_RPC_INT: {
		fs_rpc_int_t *v = (fs_rpc_int_t*)value;
		FS_MBUF_ENQ_TYPE(mbuf, v, fs_rpc_int_t);
		//printf("c pack int:%d\n", *v);
		break;
	}
	case FS_RPC_STRING: {
		fs_rpc_string_t *ptr = (fs_rpc_string_t*)value;
		FS_MBUF_ENQ_TYPE(mbuf, &ptr->n, fs_rpc_size_t);
		FS_MBUF_ENQ(mbuf, ptr->str, ptr->n);
		//printf("c pack string:%s\n", ptr->str);
		break;
	}
	case FS_RPC_CLASS: {
		fs_rpc_class_t *cls = field->class_ptr;
		// printf("____c pack class:%s\n", cls->name);
		if (pack_struct(cls->field, cls->field_count, value, mbuf)) {
			return -1;
		}
		break;
	}
	default:
		assert(0);
		return -1;
	}
	return 0;
}

inline static int pack_struct(fs_rpc_field_t *field, int field_count, void *struct_value, fs_mbuf_t *mbuf)
{
	int c;
	unsigned i;
	char *field_value;
	for (c = 0; c < field_count; ++c, ++field) {
		field_value = (char*)struct_value + field->c_offset;
		//printf("pack struct filed name:%s,type:%d,offset:%d\n", field->name, field->type, field->c_offset);
		if (field->array == FS_RPC_ARRAY_VAR) {
			size_t fsz;
			char *data;
			fs_rpc_array_t *ptr = (fs_rpc_array_t*)field_value;
			FS_MBUF_ENQ_TYPE(mbuf, &ptr->n, fs_rpc_size_t);
			fsz = get_field_size(field);
			for (i = 0, data = (char*)ptr->u.data; i < ptr->n; ++i, data += fsz) {
				if (pack_field(field, data, mbuf)) {
					return -1;
				}
			}
		} else {
			if (pack_field(field, field_value, mbuf)) {
				return -1;
			}
		}
	}
	return 0;
}

void log_pack(fs_rpc_function_t *function, fs_mbuf_t *mbuf)
{
	if (g_log_pto_enable) {
		fprintf(stderr, "rpc pack info pid=%d,pname=%s,len=%u\n", 
			function->function_id, function->arg_class.name, mbuf->data_size);
	} 
}

void log_unpack(fs_rpc_function_t *function, fs_inbuf_t *inbuf)
{
	if (g_log_pto_enable) {
		fprintf(stderr, "rpc unpack info pid=%d,pname=%s,len=%u\n", 
			function->function_id, function->arg_class.name, inbuf->size);
	}
}

void enable_log_pto(int enable)
{
	g_log_pto_enable = enable;
}

int fs_rpc_c_pack(fs_rpc_function_t *function, void *struct_value, fs_mbuf_t *mbuf)
{
	fs_rpc_class_t *cls = &function->arg_class;
	//skip the vfd arg
	int ret = pack_struct(cls->field + 1, cls->field_count - 1, struct_value, mbuf);
	log_pack(function, mbuf);
	return ret;
	//printf("c pack:%s\n", cls->name);
}

////test /////


/*
void rpc_server_test(int vfd, test_user_t *arg) {};
void rpc_c_test()
{
	fs_protocol_id_t pid = 4;
	fs_rpc_function_t *fun = fs_rpc_function_bsearch(&rpc_c_function_table, pid);
	assert(fun);


	user_info_t infos[2];
	infos[0].name.n = strlen("hello");
	infos[0].name.str = "hello";
	infos[1].name.n = strlen("qtz fs!");
	infos[1].name.str = "qtz fs!";

	test_user_t user;
	user.id = 1024;
	user.info.n = 2;
	user.info.u.data = infos;

	struct evbuffer *buf = evbuffer_new();
	fs_rpc_c_pack(fun, &user, buf);


	test_user_t *unpk = fs_rpc_c_unpack(fun, buf);

	printf("c id:%d,n:%d\n", unpk->id, unpk->info.n);
	int i;
	for (i = 0; i < unpk->info.n; ++i) {
		printf("c name:%s\n", unpk->info.u.user_info_tp[i].name.str);
	}

}
*/
