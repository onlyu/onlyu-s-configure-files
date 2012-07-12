#define SUPRESS_COMPILER_INLINES
#ifdef LATTICE
	#include "/lpc_incl.h"
	#include "/comm.h"
	#include "/file_incl.h"
	#include "/file.h"
	#include "/backend.h"
	#include "/swap.h"
	#include "/compiler.h"
	#include "/main.h"
	#include "/eoperators.h"
#else 
	#include "include/lpc_incl.h"
	#include "include/comm.h"
	#include "include/file_incl.h"
	#include "include/file.h"
	#include "include/backend.h"
	#include "include/swap.h"
	#include "include/compiler.h"
	#include "include/main.h"
	#include "include/eoperators.h"
#endif 

/* should be done in configure */
#ifdef WIN32
	#define strcasecmp(X, Y) stricmp(X, Y)
#endif 

/* Beek again */
#ifdef F_STORE_VARIABLE
	void f_store_variable(void)
	{
		int idx;
		svalue_t *sv;
		unsigned short type;

		idx = find_global_variable(current_object->prog, (sp - 1)->u.string, &type);
		if (idx ==  - 1)
		{
			error("No variable named '%s'!\n", (sp - 1)->u.string);
		}
		sv = &current_object->variables[idx];
		free_svalue(sv, "f_store_variable");
		*sv =  *sp--;
		free_string_svalue(sp--);
	}
#endif 

#ifdef F_FETCH_VARIABLE
	void f_fetch_variable(void)
	{
		int idx;
		svalue_t *sv;
		unsigned short type;

		idx = find_global_variable(current_object->prog, sp->u.string, &type);
		if (idx ==  - 1)
		{
			error("No variable named '%s'!\n", sp->u.string);
		}
		sv = &current_object->variables[idx];
		free_string_svalue(sp--);
		push_svalue(sv);
	}
#endif 

/* Gudu@VR wrote copy_array() and copy_mapping() which this is heavily
 * based on.  I made it into a general copy() efun which incorporates
 * both. -Beek
 */
#ifdef F_COPY
	static int depth;

	void deep_copy_svalue(svalue_t *, svalue_t*);

	array_t *deep_copy_array(array_t *arg)
	{
		array_t *vec;
		int i;

		vec = allocate_empty_array(arg->size);
		for (i = 0; i < arg->size; i++)
		{
			deep_copy_svalue(&arg->item[i], &vec->item[i]);
		}
		return vec;
	}

	int doCopy(mapping_t *map, mapping_node_t *elt, mapping_t *dest)
	{
		svalue_t *sp;

		sp = find_for_insert(dest, &elt->values[0], 1);
		if (!sp)
		{
			mapping_too_large();
			return 1;
		}

		(void)map;
		deep_copy_svalue(&elt->values[1], sp);
		return 0;
	}

	mapping_t *deep_copy_mapping(mapping_t *arg)
	{
		mapping_t *map;

		map = allocate_mapping(0); /* this should be fixed.  -Beek */
		mapTraverse(arg, (int(*) () )doCopy, map);
		return map;
	}

	void deep_copy_svalue(svalue_t *from, svalue_t *to)
	{
		switch (from->type)
		{
			case T_ARRAY:
			case T_CLASS:
				depth++;
				if (depth > MAX_SAVE_SVALUE_DEPTH)
				{
					depth = 0;
					error("Mappings, arrays and/or classes nested too deep (%d) for copy()\n", MAX_SAVE_SVALUE_DEPTH);
				}
				*to =  *from;
				to->u.arr = deep_copy_array(from->u.arr);
				depth--;
				break;
			case T_MAPPING:
				depth++;
				if (depth > MAX_SAVE_SVALUE_DEPTH)
				{
					depth = 0;
					error("Mappings, arrays and/or classes nested too deep (%d) for copy()\n", MAX_SAVE_SVALUE_DEPTH);
				}
				*to =  *from;
				to->u.map = deep_copy_mapping(from->u.map);
				depth--;
				break;
			default:
				assign_svalue_no_free(to, from);
		}
	}

	void f_copy(void)
	{
		svalue_t ret;

		depth = 0;
		deep_copy_svalue(sp, &ret);
		free_svalue(sp, "f_copy");
		*sp = ret;
	}
#endif 

/* Gudu@VR */
/* flag and extra info by Beek */
#ifdef F_FUNCTIONS
	void f_functions(void)
	{
		int i, j, num, index;
		array_t *vec,  *subvec;
		runtime_function_u *func_entry;
		compiler_function_t *funp;
		program_t *prog;
		int flag = (sp--)->u.number;
		unsigned short *types;
		char buf[256];
		char *end = EndOf(buf);
		program_t *progp;

	#ifdef _USE_SWAP_H_
		if (sp->u.ob->flags &O_SWAPPED)
		{
			load_ob_from_swap(sp->u.ob);
		}
	#endif	

		progp = sp->u.ob->prog;
		num = progp->num_functions_total;
		if (num && progp->function_table[progp->num_functions_defined - 1].name[0] == APPLY___INIT_SPECIAL_CHAR)
		{
			num--;
		}

		vec = allocate_empty_array(num);
		i = num;

		while (i--)
		{
			prog = sp->u.ob->prog;
			index = i;
			func_entry = FIND_FUNC_ENTRY(prog, index);

			/* Walk up the inheritance tree to the real definition */
			while (prog->function_flags[index] &NAME_INHERITED)
			{
				prog = prog->inherit[func_entry->inh.offset].prog;
				index = func_entry->inh.function_index_offset;
				func_entry = FIND_FUNC_ENTRY(prog, index);
			}

			funp = prog->function_table + func_entry->def.f_index;

			if (flag)
			{
				if (prog->type_start && prog->type_start[index] != INDEX_START_NONE)
				{
					types = &prog->argument_types[prog->type_start[index]];
				}
				else
				{
					types = 0;
				}

				vec->item[i].type = T_ARRAY;
				subvec = vec->item[i].u.arr = allocate_empty_array(3+func_entry->def.num_arg);

				subvec->item[0].type = T_STRING;
				subvec->item[0].subtype = STRING_SHARED;
				subvec->item[0].u.string = ref_string(funp->name);

				subvec->item[1].type = T_NUMBER;
				subvec->item[1].subtype = 0;
				subvec->item[1].u.number = func_entry->def.num_arg;

				get_type_name(buf, end, funp->type);
				subvec->item[2].type = T_STRING;
				subvec->item[2].subtype = STRING_SHARED;
				subvec->item[2].u.string = make_shared_string(buf);

				for (j = 0; j < func_entry->def.num_arg; j++)
				{
					if (types)
					{
						get_type_name(buf, end, types[j]);
						subvec->item[3+j].type = T_STRING;
						subvec->item[3+j].subtype = STRING_SHARED;
						subvec->item[3+j].u.string = make_shared_string(buf);
					}
					else
					{
						subvec->item[3+j].type = T_NUMBER;
						subvec->item[3+j].u.number = 0;
					}
				}
			}
			else
			{
				vec->item[i].type = T_STRING;
				vec->item[i].subtype = STRING_SHARED;
				vec->item[i].u.string = ref_string(funp->name);
			}
		}

		pop_stack();
		push_refed_array(vec);
	}
#endif 

/* Beek */
#ifdef F_VARIABLES
	static void fv_recurse(array_t *arr, int *idx, program_t *prog, int type, int flag)
	{
		int i;
		array_t *subarr;
		char buf[256];
		char *end = EndOf(buf);

		for (i = 0; i < prog->num_inherited; i++)
		{
			fv_recurse(arr, idx, prog->inherit[i].prog, type | prog->inherit[i].type_mod, flag);
		}
		for (i = 0; i < prog->num_variables_defined; i++)
		{
			if (flag)
			{
				arr->item[ *idx + i].type = T_ARRAY;
				subarr = arr->item[ *idx + i].u.arr = allocate_empty_array(2);
				subarr->item[0].type = T_STRING;
				subarr->item[0].subtype = STRING_SHARED;
				subarr->item[0].u.string = ref_string(prog->variable_table[i]);
				get_type_name(buf, end, prog->variable_types[i]);
				subarr->item[1].type = T_STRING;
				subarr->item[1].subtype = STRING_SHARED;
				subarr->item[1].u.string = make_shared_string(buf);
			}
			else
			{
				arr->item[ *idx + i].type = T_STRING;
				arr->item[ *idx + i].subtype = STRING_SHARED;
				arr->item[ *idx + i].u.string = ref_string(prog->variable_table[i]);
			}
		}
		*idx += prog->num_variables_defined;
	}

	void f_variables(void)
	{
		int idx = 0;
		array_t *arr;
		int flag = (sp--)->u.number;
		program_t *prog = sp->u.ob->prog;

		if (sp->u.ob->flags &O_SWAPPED)
		{
			load_ob_from_swap(sp->u.ob);
		}

		arr = allocate_empty_array(prog->num_variables_total);
		fv_recurse(arr, &idx, prog, 0, flag);

		pop_stack();
		push_refed_array(arr);
	}
#endif 

#ifdef F_FILE_LENGTH
	/*
	 * file_length() efun, returns the number of lines in a file.
	 * Returns -1 if no privs or file doesn't exist.
	 */
	int file_length(char *file)
	{
		struct stat st;
		FILE *f;
		int ret = 0;
		int num;
		char buf[2049];
		char *p,  *newp;

		file = check_valid_path(file, current_object, "file_size", 0);

		if (!file)
		{
			return  - 1;
		}
		if (stat(file, &st) ==  - 1)
		{
			return  - 1;
		}
		if (st.st_mode &S_IFDIR)
		{
			return  - 2;
		}
		if (!(f = fopen(file, "r") ) )
		{
			return  - 1;
		}

		do
		{
			num = fread(buf, 1, 2048, f);
			p = buf - 1;
			while ( (newp = memchr(p + 1, '\n', num) ) )
			{
				num -= (newp - p);
				p = newp;
				ret++;
			} 
		}

		while (!feof(f) )
			;

		fclose(f);
		return ret;
	} /* end of file_length() */

	void f_file_length(void)
	{
		int l;

		l = file_length(sp->u.string);
		pop_stack();
		push_number(l);
	}
#endif 

#ifdef F_UPPER_CASE
	void f_upper_case(void)
	{
		register char *str;

		str = sp->u.string;
		/* find first upper case letter, if any */
		for (;  *str; str++)
		{
			if (islower(*str) )
			{
				int l = str - sp->u.string;
				unlink_string_svalue(sp);
				str = sp->u.string + l;
				*str -= 'a' - 'A';
				for (str++;  *str; str++)
				{
					if (islower(*str) )
					{
						*str -= 'a' - 'A';
					}
				}
				return ;
			}
		}
	}
#endif 

#ifdef F_REPLACEABLE
	void f_replaceable(void)
	{
		program_t *prog;
		int i, j, num, numignore;
		char **ignore;

		if (st_num_arg == 2)
		{
			numignore = sp->u.arr->size;
			if (numignore)
			{
				ignore = CALLOCATE(numignore, char *, TAG_TEMPORARY, "replaceable");
			}
			else
			{
				ignore = 0;
			}
			for (i = 0; i < numignore; i++)
			{
				if (sp->u.arr->item[i].type == T_STRING)
				{
					ignore[i] = findstring(sp->u.arr->item[i].u.string);
				}
				else
				{
					ignore[i] = 0;
				}
			}
			prog = (sp - 1)->u.ob->prog;
		}
		else
		{
			numignore = 1;
			ignore = CALLOCATE(1, char *, TAG_TEMPORARY, "replaceable");
			ignore[0] = findstring(APPLY_CREATE);
			prog = sp->u.ob->prog;
		}

		num = prog->num_functions_total - 1; /* ignore #global_init# */

		for (i = 0; i < num; i++)
		{
			if (prog->function_flags[i] &(NAME_INHERITED | NAME_NO_CODE) )
			{
				continue;
			}
			for (j = 0; j < numignore; j++)
				if (ignore[j] == prog->function_table[FIND_FUNC_ENTRY(prog, i)->def.f_index].name)
			    {
					break;
			    }
			if (j == numignore)
			{
				break;
			}
		}
		if (st_num_arg == 2)
		{
			free_array( (sp--)->u.arr);
		}
		FREE(ignore);
		free_svalue(sp, "f_replaceable");
		put_number(i == num);
	}
#endif 

#ifdef F_PROGRAM_INFO
	void f_program_info(void)
	{
		int func_size = 0;
		int string_size = 0;
		int var_size = 0;
		int inherit_size = 0;
		int prog_size = 0;
		int hdr_size = 0;
		int class_size = 0;
		int type_size = 0;
		int total_size = 0;
		object_t *ob;
		program_t *prog;
		outbuffer_t out;
		int i, n;

		if (st_num_arg == 1)
		{
			ob = sp->u.ob;
			prog = ob->prog;
			if (!(ob->flags &(O_CLONE | O_SWAPPED) ) )
			{
				hdr_size += sizeof(program_t);
				prog_size += prog->program_size;
				func_size += 2 * prog->num_functions_total; /* function flags */
				#ifdef COMPRESS_FUNCTION_TABLES
					/* compressed table header */
					func_size += sizeof(compressed_offset_table_t) - 1;
					/* it's entries */
					func_size += (prog->function_compressed->first_defined - prog->function_compressed->num_compressed);
					/* offset table */
					func_size += sizeof(runtime_function_u)*(prog->num_functions_total - prog->function_compressed->num_deleted);
				#else 
					/* offset table */
					func_size += prog->num_functions_total *sizeof(runtime_function_u);
				#endif 
				/* definitions */
				func_size += prog->num_functions_defined *sizeof(compiler_function_t);
				string_size += prog->num_strings *sizeof(char*);
				var_size += prog->num_variables_defined *(sizeof(char*) + sizeof(unsigned short) );
				inherit_size += prog->num_inherited *sizeof(inherit_t);
				if (prog->num_classes)
				{
					class_size += prog->num_classes *sizeof(class_def_t) + (prog->classes[prog->num_classes - 1].index + prog->classes[prog->num_classes - 1].size) *sizeof(class_member_entry_t);
				}
				type_size += prog->num_functions_defined *sizeof(short);
				n = 0;
				for (i = 0; i < prog->num_functions_defined; i++)
				{
					int start;
					unsigned short *ts = prog->type_start;
					int ri;

					if (!ts)
					{
						continue;
					}
					start = ts[i];
					if (start == INDEX_START_NONE)
					{
						continue;
					}
					ri = prog->function_table[i].runtime_index;
					start += FIND_FUNC_ENTRY(prog, ri)->def.num_arg;
					if (start > n)
					{
						n = start;
					}
				}
				type_size += n * sizeof(short);
				total_size += prog->total_size;
			}
			pop_stack();
		}
		else
		{
			for (ob = obj_list; ob; ob = ob->next_all)
			{
				if (ob->flags &(O_CLONE | O_SWAPPED) )
				{
					continue;
				}
				prog = ob->prog;
				hdr_size += sizeof(program_t);
				prog_size += prog->program_size;
				func_size += prog->num_functions_total; /* function flags */
				#ifdef COMPRESS_FUNCTION_TABLES
					/* compressed table header */
					func_size += sizeof(compressed_offset_table_t) - 1;
					/* it's entries */
					func_size += (prog->function_compressed->first_defined - prog->function_compressed->num_compressed);
					/* offset table */
					func_size += sizeof(runtime_function_u)*(prog->num_functions_total - prog->function_compressed->num_deleted);
				#else 
					/* offset table */
					func_size += prog->num_functions_total *sizeof(runtime_function_u);
				#endif 
				/* definitions */
				func_size += prog->num_functions_defined *sizeof(compiler_function_t);
				string_size += prog->num_strings *sizeof(char*);
				var_size += prog->num_variables_defined *(sizeof(char*) + sizeof(unsigned short) );
				inherit_size += prog->num_inherited *sizeof(inherit_t);
				if (prog->num_classes)
				{
					class_size += prog->num_classes *sizeof(class_def_t) + (prog->classes[prog->num_classes - 1].index + prog->classes[prog->num_classes - 1].size) *sizeof(class_member_entry_t);
				}
				type_size += prog->num_functions_defined *sizeof(short);
				n = 0;
				for (i = 0; i < prog->num_functions_defined; i++)
				{
					int start;
					int ri;

					unsigned short *ts = prog->type_start;
					if (!ts)
					{
						continue;
					}
					start = ts[i];
					if (start == INDEX_START_NONE)
					{
						continue;
					}
					ri = prog->function_table[i].runtime_index;
					start += FIND_FUNC_ENTRY(prog, ri)->def.num_arg;
					if (start > n)
					{
						n = start;
					}
				}
				type_size += n * sizeof(short);
				total_size += prog->total_size;
			}
		}

		outbuf_zero(&out);

		outbuf_addv(&out, "\nheader size: %i\n", hdr_size);
		outbuf_addv(&out, "code size: %i\n", prog_size);
		outbuf_addv(&out, "function size: %i\n", func_size);
		outbuf_addv(&out, "string size: %i\n", string_size);
		outbuf_addv(&out, "var size: %i\n", var_size);
		outbuf_addv(&out, "class size: %i\n", class_size);
		outbuf_addv(&out, "inherit size: %i\n", inherit_size);
		outbuf_addv(&out, "saved type size: %i\n\n", type_size);

		outbuf_addv(&out, "total size: %i\n", total_size);

		outbuf_push(&out);
	}
#endif 

#ifdef F_DEBUG_MESSAGE
	void f_debug_message(void)
	{
		char *file;
		int line;
		char *s = NULL;
		int num_arg = st_num_arg;
		get_line_number_info(&file, &line);

		char *string_print_formatted(char *format_str, int argc, svalue_t *argv);

		if (num_arg > 1) {
			s = string_print_formatted( (sp - num_arg + 1)->u.string, num_arg - 1, sp - num_arg + 2);
			debug_message("%s %d : %s\n", file, line, s);
			FREE_MSTR(s);
		} else {
			debug_message("%s %d : %s\n", file, line, (sp - num_arg + 1)->u.string);
		}
		pop_n_elems(num_arg);
		// free_string_svalue(sp--);
	}
#endif 

#ifdef F_FUNCTION_OWNER
	void f_function_owner(void)
	{
		object_t *owner = sp->u.fp->hdr.owner;

		free_funp(sp->u.fp);
		put_unrefed_object(owner, "f_function_owner");
	}
#endif 

#ifdef F_REPEAT_STRING
	void f_repeat_string(void)
	{
		char *str;
		int repeat, len;
		char *ret,  *p;
		int i;

		repeat = (sp--)->u.number;
		if (repeat <= 0)
		{
			free_string_svalue(sp);
			sp->type = T_STRING;
			sp->subtype = STRING_CONSTANT;
			sp->u.string = "";
		}
		else if (repeat != 1)
		{
			str = sp->u.string;
			len = SVALUE_STRLEN(sp);
			p = ret = new_string(len *repeat, "f_repeat_string");
			for (i = 0; i < repeat; i++)
			{
				memcpy(p, str, len);
				p += len;
			}
			*p = 0;
			free_string_svalue(sp);
			sp->type = T_STRING;
			sp->subtype = STRING_MALLOC;
			sp->u.string = ret;
		}
	}
#endif
