/*
this file contains the efunctions called from inside eval_instruction()
in interpret.c.
Note: if you are adding local efunctions that are specific to your
driver, you would be better off adding them to a separate source file.
Doing so will make it much easier for you to upgrade (won't have to
patch this file).
Be sure to #include "lpc_incl.h" in that separate source file.
 */

#include <sys/types.h>
#include <regex.h>
#include "std.h"
#include "lpc_incl.h"
#include "file_incl.h"
#include "malloc_incl.h"
#include "file.h"
#include "comm.h"
#include "sprintf.h"
#include "backend.h"
#include "port.h"
#include "swap.h"
#include "strstr.h"
#include "otable.h"
#include "eoperators.h"
#include "reclaim.h"
#include "dumpstat.h"
#include "efuns_main.h"
#include "debug.h"
#include "md.h"

#include "crypt.h"

/* This shouldn't be here.  See binaries.c */
#ifdef WIN32
	#include <direct.h>
#endif 

int call_origin = 0;

static object_t *ob;

int data_size(object_t *ob);
void reload_object(object_t *obj);


extern char game_lib_path[];

////////////////////////////////////////////////////////////////

#define _debug(fmt, args...) 

/* 在Interpret.H中提供了一些宏来设置EFUN的返回值，应尽量使用。
EFUN只能接受0-3个参数, 如果多于3个, 则需要用可变参数的形式.
#define push_svalue(x) assign_svalue_no_free(++sp, x)
*/

#ifdef F_GET_CURRENT_SP
	void f_get_current_sp(void)
	{
		//printf("sp=%X,end_sp=%X\n",sp,end_of_stack);
		put_number( (int)sp);
	}
#endif 

#ifdef F_ALLOCATE
	void f_allocate(void)
	{
		sp->u.arr = allocate_array(sp->u.number);
		sp->type = T_ARRAY;
	}
#endif 


#ifdef F_ALLOCATE_MAPPING
	void f_allocate_mapping(void)
	{
		sp->type = T_MAPPING;
		sp->u.map = allocate_mapping(sp->u.number);
	}
#endif 

#ifdef F_CACHE_STATS
	static void print_cache_stats(outbuffer_t *ob)
	{
		outbuf_add(ob, "QTZ_ServerEngine:Function cache information\n");
		outbuf_add(ob, "****Copy Right By QTZ*****\n");
		outbuf_addv(ob, "%% cache hits:    %10.2f\n", 100 *( (double)apply_low_cache_hits / apply_low_call_others) );
		outbuf_addv(ob, "call_others:     %10lu\n", apply_low_call_others);
		outbuf_addv(ob, "cache hits:      %10lu\n", apply_low_cache_hits);
		outbuf_addv(ob, "cache size:      %10lu\n", APPLY_CACHE_SIZE);
		outbuf_addv(ob, "slots used:      %10lu\n", apply_low_slots_used);
		outbuf_addv(ob, "%% slots used:    %10.2f\n", 100 *( (double)apply_low_slots_used / APPLY_CACHE_SIZE) );
		outbuf_addv(ob, "collisions:      %10lu\n", apply_low_collisions);
		outbuf_addv(ob, "%% collisions:    %10.2f\n", 100 *( (double)apply_low_collisions / apply_low_call_others) );
	}

	void f_cache_stats(void)
	{
		outbuffer_t ob;

		outbuf_zero(&ob);
		print_cache_stats(&ob);
		outbuf_push(&ob);
	}
#endif 

#ifdef F_CALL_OTHER
	/* enhanced call_other written 930314 by Luke Mewburn <zak@rmit.edu.au> */
	void f_call_other(void)
	{
		svalue_t *arg;
		char *funcname;
		int i;
		int num_arg = st_num_arg;


		if (current_object->flags &O_DESTRUCTED)
		{
			 /* No external calls allowed */
			pop_n_elems(num_arg);
			push_undefined();
			return ;
		}
		arg = sp - num_arg + 1;
		if (arg[1].type == T_STRING)
		{
			funcname = arg[1].u.string;
		}
		else
		{
			 /* must be T_ARRAY then */
			array_t *v = arg[1].u.arr;
			svalue_t *sv = 0;

			check_for_destr(v);
			if ( ( (i = v->size) < 1) || !( (sv = v->item)->type == T_STRING) )
			{
				error("QTZ_ServerEngine:\n Call_other: 1st elem of array for arg 2 must be a string\n");
			}
			funcname = sv->u.string;
			num_arg = 2+merge_arg_lists(num_arg - 2, v, 1);
		}

		if (arg[0].type == T_OBJECT)
		{
			ob = arg[0].u.ob;
		}
		else if (arg[0].type == T_ARRAY)
		{
			array_t *ret;

			ret = call_all_other(arg[0].u.arr, funcname, num_arg - 2);
			pop_stack();
			free_array(arg->u.arr);
			sp->u.arr = ret;
			return ;
		}
		else
		{
			object_t *old_ob;
			ob = find_object(arg[0].u.string);
			if (!(old_ob = ob) || !object_visible(ob) )
			{
				debug_message("[%s]Can't find object", arg[0].u.string);
				error("Can't find object\n");
			}
			ob = old_ob;
		}
		/* Send the remaining arguments to the function. */
		call_origin = ORIGIN_CALL_OTHER;
		if (apply_low(funcname, ob, num_arg - 2) == 0)
		{
			 /* Function not found */
			if (strcmp(funcname, "???") != 0)
			{
				char *file;
				int line;
				get_line_number_info(&file, &line);
				debug_message("\n%s %d : object[%s] func[%s] not exist!\n", file, line, ob->name, funcname);
			}
			pop_2_elems();
			push_undefined();
			return ;
		}
		/*
		 * The result of the function call is on the stack.  So is the function
		 * name and object that was called, though. These have to be removed.
		 */
		free_svalue(--sp, "f_call_other:1");
		free_svalue(--sp, "f_call_other:2");
		*sp = *(sp + 2);
		return ;
	}
#endif 


#if defined(F_CALL_STACK) || defined(F_ORIGIN)
	char *origin_name(int orig)
	{
		/* FIXME: this should use ffs() if available (BSD) */
		int i = 0;
		static char *origins[] = 
		{
			"driver", "local", "call_other", "simul", "call_out", "efun", "function pointer", "functional"
		};
		while (orig >>= 1)
		{
			i++;
		}
		return origins[i];
	}
#endif 

#ifdef F_CALL_STACK
	void f_call_stack(void)
	{
		int i, n = csp - &control_stack[0] + 1;
		array_t *ret;

		if (sp->u.number < 0 || sp->u.number > 3)
		{
			error("QTZ_ServerEngine:First argument of call_stack() is wrong.\n");
		}

		ret = allocate_empty_array(n);

		switch (sp->u.number)
		{
			case 0:
				ret->item[0].type = T_STRING;
				ret->item[0].subtype = STRING_MALLOC;
				ret->item[0].u.string = add_slash(current_prog->name);
				for (i = 1; i < n; i++)
				{
					ret->item[i].type = T_STRING;
					ret->item[i].subtype = STRING_MALLOC;
					ret->item[i].u.string = add_slash( (csp - i + 1)->prog->name);
				}
				break;
			case 1:
				ret->item[0].type = T_OBJECT;
				ret->item[0].u.ob = current_object;
				add_ref(current_object, "f_call_stack: curr");
				for (i = 1; i < n; i++)
				{
					ret->item[i].type = T_OBJECT;
					ret->item[i].u.ob = (csp - i + 1)->ob;
					add_ref( (csp - i + 1)->ob, "f_call_stack");
				}
				break;
			case 2:
				for (i = 0; i < n; i++)
				{
					ret->item[i].type = T_STRING;
					if ( ( (csp - i)->framekind &FRAME_MASK) == FRAME_FUNCTION)
					{
						program_t *prog = (i ? (csp - i + 1)->prog: current_prog);
						int index = (csp - i)->fr.table_index;
						compiler_function_t *cfp = &prog->function_table[index];

						ret->item[i].subtype = STRING_SHARED;
						ret->item[i].u.string = cfp->name;
						ref_string(cfp->name);
					}
					else
					{
						ret->item[i].subtype = STRING_CONSTANT;
						ret->item[i].u.string = ( ( (csp - i)->framekind &FRAME_MASK) == FRAME_CATCH) ? "CATCH" : "<function>";
					}
				}
				break;
			case 3:
				ret->item[0].type = T_STRING;
				ret->item[0].subtype = STRING_CONSTANT;
				ret->item[0].u.string = origin_name(caller_type);

				for (i = 1; i < n; i++)
				{
					ret->item[i].type = T_STRING;
					ret->item[i].subtype = STRING_CONSTANT;
					ret->item[i].u.string = origin_name( (csp - i + 1)->caller_type);
				}
				break;
		}
		put_array(ret);
	}
#endif 

#ifdef F_CAPITALIZE
	void f_capitalize(void)
	{
		if (islower(sp->u.string[0]) )
		{
			unlink_string_svalue(sp);
			sp->u.string[0] += 'A' - 'a';
		}
	}
#endif 

#ifdef F_CHILDREN
	void f_children(void)
	{
		array_t *vec;

		vec = children(sp->u.string);
		free_string_svalue(sp);
		put_array(vec);
	}
#endif 

#ifdef F_CLASSP
	void f_classp(void)
	{
		if (sp->type == T_CLASS)
		{
			free_class(sp->u.arr);
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_classp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_CLEAR_BIT
	void f_clear_bit(void)
	{
		char *str;
		int len, ind, bit;

		if (sp->u.number > MAX_BITS)
		{
			error("QTZ_ServerEngine:Function requested : %d > maximum bits: %d\n", sp->u.number, MAX_BITS);
		}
		bit = (sp--)->u.number;
		if (bit < 0)
		{
			error("QTZ_ServerEngine:Bad argument 2 function.\n");
		}
		ind = bit / 6;
		bit %= 6;
		len = SVALUE_STRLEN(sp);
		if (ind >= len)
		{
			return ;
		}
		 /* return first arg unmodified */
		unlink_string_svalue(sp);
		str = sp->u.string;

		if (str[ind] > 0x3f + ' ' || str[ind] < ' ')
		{
			error("QTZ_ServerEngine:Wrong bit pattern in function character %d\n", ind);
		}
		str[ind] = ( (str[ind] - ' ') &~(1 << bit) ) + ' ';
	}
#endif 

#ifdef F_CLONEP
	void f_clonep(void)
	{
		if ( (sp->type == T_OBJECT) && (sp->u.ob->flags &O_CLONE) )
		{
			free_object(sp->u.ob, "f_clonep");
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_clonep");
			*sp = const0;
		}
	}
#endif 

#ifdef F_CLONE_OBJECT
	void f_clone_object(void)
	{
		svalue_t *arg = sp - st_num_arg + 1;

		ob = clone_object(arg->u.string, st_num_arg - 1);
		free_string_svalue(sp);
		if (ob)
		{
			put_unrefed_undested_object(ob, "f_clone_object");
		}
		else
		{
			 *sp = const0;
		}
	}
#endif 

#ifdef F_CP
	void f_cp(void)
	{
		int i;

		i = copy_file(sp[ - 1].u.string, sp[0].u.string);
		free_string_svalue(sp--);
		free_string_svalue(sp);
		put_number(i);
	}
#endif 

#ifdef F_CTIME
	void f_ctime(void)
	{
		char *cp,  *nl,  *p;
		int l;

		cp = time_string( (void*) &(sp->u.number) );

		if ( (nl = strchr(cp, '\n') ) )
		{
			l = nl - cp;
		}
		else
		{
			l = strlen(cp);
		}

		p = new_string(l, "f_ctime");
		strncpy(p, cp, l);
		p[l] = '\0';
		put_malloced_string(p);
	}
#endif 

#ifdef F_DEEP_INHERIT_LIST
	void f_deep_inherit_list(void)
	{
		array_t *vec;

#ifdef _USE_SWAP_H_
		if (!(sp->u.ob->flags &O_SWAPPED) )
		{
			vec = deep_inherit_list(sp->u.ob);
		}
		else
		{
			vec = &the_null_array;
		}
#else
		vec = deep_inherit_list(sp->u.ob);
#endif
		free_object(sp->u.ob, "f_deep_inherit_list");
		put_array(vec);
	}
#endif 

#ifdef DEBUG_MACRO
	#ifdef F_SET_DEBUG_LEVEL
		void f_set_debug_level(void)
		{
			debug_level = (sp--)->u.number;
		}
	#endif 
#endif 


#ifdef F_DESTRUCT
	void f_destruct(void)
	{
		destruct_object(sp->u.ob);
		sp--; /* Ok since the object was removed from the stack */
	}
#endif 

#ifdef F_DUMPALLOBJ
	void f_dumpallobj(void)
	{
		if (st_num_arg)
		{
			dumpstat(sp->u.string);
			free_string_svalue(sp--);
		}
		else
		{
			dumpstat("/OBJ_DUMP");
		}
	}
#endif 

#ifdef F_ERROR
	void f_error(void)
	{
		int l = SVALUE_STRLEN(sp);
		char err_buf[2048];

		if (sp->u.string[l - 1] == '\n')
		{
			l--;
		}
		if (l > 2045)
		{
			l = 2045;
		}

		err_buf[0] = '*';
		strncpy(err_buf + 1, sp->u.string, l);
		err_buf[l + 1] = '\n';
		err_buf[l + 2] = 0;

		error_handler(err_buf);
	}
#endif 


#ifdef F_EXPLODE
	void f_explode(void)
	{
		array_t *vec;
		int src_len = SVALUE_STRLEN(sp - 1);
		int sep_len = SVALUE_STRLEN(sp);

		vec = explode_string( (sp - 1)->u.string, src_len, sp->u.string, sep_len);
		free_string_svalue(sp--);
		free_string_svalue(sp);
		put_array(vec);
	}
#endif 

#ifdef F_FILE_NAME
	void f_file_name(void)
	{
		char *res;

		/* This function now returns a leading '/' */
		res = (char*)add_slash(sp->u.ob->name);
		free_object(sp->u.ob, "f_file_name");
		put_malloced_string(res);
	}
#endif 

#ifdef F_FILE_SIZE
	void f_file_size(void)
	{
		int i = file_size(sp->u.string);
		free_string_svalue(sp);
		put_number(i);
	}
#endif 

#ifdef F_FILTER
	void f_filter(void)
	{
		svalue_t *arg = sp - st_num_arg + 1;

		if (arg->type == T_MAPPING)
		{
			filter_mapping(arg, st_num_arg);
		}
		else
		{
			filter_array(arg, st_num_arg);
		}
	}
#endif 

#ifdef F_FIND_OBJECT
	void f_find_object(void)
	{
		if ( (sp--)->u.number)
		{
			ob = find_object(sp->u.string);
		}
		else
		{
			ob = find_object2(sp->u.string);
		}
		free_string_svalue(sp);
		if (ob)
		{
			object_t *old_ob = ob;
			/* object_visible might change ob, a global - Sym */
			if (object_visible(ob) )
			{
				/* find_object only returns undested objects */
				put_unrefed_undested_object(old_ob, "find_object");
			}
			else
			{
				 *sp = const0;
			}
		}
		else
		{
			 *sp = const0;
		}
	}
#endif 

#ifdef F_FUNCTION_PROFILE
	// f_function_profile: John Garnett, 1993/05/31, 0.9.17.3
	void f_function_profile(void)
	{
		array_t *vec;
		mapping_t *map;
		program_t *prog;
		int nf, j;

		ob = sp->u.ob;

#ifdef _USE_SWAP_H_
		if (ob->flags &O_SWAPPED)
		{
			load_ob_from_swap(ob);
		}
#endif		
		prog = ob->prog;
		nf = prog->num_functions_defined;
		vec = allocate_empty_array(nf);
		for (j = 0; j < nf; j++)
		{
			map = allocate_mapping(3);
			add_mapping_pair(map, "calls", prog->function_table[j].calls);
			add_mapping_pair(map, "self", prog->function_table[j].self - prog->function_table[j].children);
			add_mapping_pair(map, "children", prog->function_table[j].children);
			add_mapping_shared_string(map, "name", prog->function_table[j].name);
			vec->item[j].type = T_MAPPING;
			vec->item[j].u.map = map;
		}
		free_object(ob, "f_function_profile");
		put_array(vec);
	}
#endif 

#ifdef F_GET_DIR
	void f_get_dir(void)
	{
		array_t *vec;

		vec = get_dir( (sp - 1)->u.string, sp->u.number);
		free_string_svalue(--sp);
		if (vec)
		{
			put_array(vec);
		}
		else
		{
			 *sp = const0;
		}
	}
#endif 

#ifdef F_IMPLODE
	void f_implode(void)
	{
		array_t *arr;
		int flag;
		svalue_t *args;

		if (st_num_arg == 3)
		{
			args = (sp - 2);
			if (args[1].type == T_STRING)
			{
				error("Third argument to implode() is illegal with implode(array, string)\n");
			}
			flag = 1;
		}
		else
		{
			args = (sp - 1);
			flag = 0;
		}
		arr = args->u.arr;
		check_for_destr(arr);

		if (args[1].type == T_STRING)
		{
			/* st_num_arg == 2 here */
			char *str;

			str = implode_string(arr, sp->u.string, SVALUE_STRLEN(sp) );
			free_string_svalue(sp--);
			free_array(arr);
			put_malloced_string(str);
		}
		else
		{
			 /* function */
			funptr_t *funp = args[1].u.fp;

			/* this pulls the extra arg off the stack if it exists */
			implode_array(funp, arr, args, flag);
			sp--;
			free_funp(funp);
			free_array(arr);
		}
	}
#endif 

#ifdef F_INHERITS
	int inherits(program_t *prog, program_t *thep)
	{
		int j, k = prog->num_inherited;
		program_t *pg;

		for (j = 0; j < k; j++)
		{
			if ( (pg = prog->inherit[j].prog) == thep)
			{
				return 1;
			}
			if (!strcmp(pg->name, thep->name) )
			{
				return 2;
			}
			if (inherits(pg, thep) )
			{
				return 1;
			}
		}
		return 0;
	}

	void f_inherits(void)
	{
		object_t *ob,  *base;
		int i;

		base = (sp--)->u.ob;
		ob = find_object2(sp->u.string);

#ifdef _USE_SWAP_H_
		if (!ob || (ob->flags &O_SWAPPED) )
#else
		if (!ob)
#endif			
		{
			free_object(base, "f_inherits");
			assign_svalue(sp, &const0);
			return ;
		}

#ifdef _USE_SWAP_H_
		if (base->flags &O_SWAPPED)
		{
			load_ob_from_swap(base);
		}
#endif		
		i = inherits(base->prog, ob->prog);
		free_object(base, "f_inherits");
		free_string_svalue(sp);
		put_number(i);
	}
#endif 

#ifdef F_SHALLOW_INHERIT_LIST
	void f_shallow_inherit_list(void)
	{
		array_t *vec;

#ifdef _USE_SWAP_H_		
		if (!(sp->u.ob->flags &O_SWAPPED) )
		{
			vec = inherit_list(sp->u.ob);
		}
		else
		{
			vec = &the_null_array;
		}
#else
		vec = inherit_list(sp->u.ob);
#endif		
		free_object(sp->u.ob, "f_inherit_list");
		put_array(vec);
	}
#endif 

#ifdef F_INTP
	void f_intp(void)
	{
		if (sp->type == T_NUMBER)
		{
			sp->u.number = 1;
		}
		else
		{
			free_svalue(sp, "f_intp");
			put_number(0);
		}
	}
#endif 

#ifdef F_FUNCTIONP
	void f_functionp(void)
	{
		int i;

		if (sp->type == T_FUNCTION)
		{
			i = sp->u.fp->hdr.type;
			if (sp->u.fp->hdr.args)
			{
				i |= FP_HAS_ARGUMENTS;
			}
			if (sp->u.fp->hdr.owner->flags &O_DESTRUCTED)
			{
				i |= FP_OWNER_DESTED;
			}
			free_funp(sp->u.fp);
			put_number(i);
			return ;
		}
		assign_svalue(sp, &const0);
	}
#endif 

#ifdef F_KEYS
	void f_keys(void)
	{
		array_t *vec;

		vec = mapping_indices(sp->u.map);
		free_mapping(sp->u.map);
		put_array(vec);
	}
#endif 

#ifdef F_VALUES
	void f_values(void)
	{
		array_t *vec;

		vec = mapping_values(sp->u.map);
		free_mapping(sp->u.map);
		put_array(vec);
	}
#endif 

#ifdef F_LOWER_CASE
	void f_lower_case(void)
	{
		register char *str;

		str = sp->u.string;
		/* find first upper case letter, if any */
		for (;  *str; str++)
		{
			if (isupper(*str) )
			{
				int l = str - sp->u.string;
				unlink_string_svalue(sp);
				str = sp->u.string + l;
				*str += 'a' - 'A';
				for (str++;  *str; str++)
				{
					if (isupper(*str) )
					{
						*str += 'a' - 'A';
					}
				}
				return ;
			}
		}
	}
#endif 

#ifdef F_LPC_INFO
	void f_lpc_info(void)
	{
		outbuffer_t out;

		interface_t **p = if_t_array;
		object_t *ob;

		outbuf_zero(&out);
		outbuf_addv(&out, "%30s  Loaded  Using compiled program\n", "Program");
		while (*p)
		{
			outbuf_addv(&out, "%30s: ", (*p)->fname);
			ob = lookup_object_hash( (*p)->fname);
			if (ob)
			{
				if (ob->flags &O_COMPILED_PROGRAM)
				{
					outbuf_add(&out, " No\n");
				}
				else if (ob->flags &O_SWAPPED)
				{
					outbuf_add(&out, " Yes      Swapped\n");
				}
				else if (ob->prog->program_size == 0)
				{
					outbuf_add(&out, " Yes      Yes\n");
				}
				else
				{
					outbuf_add(&out, " Yes      No\n");
				}
			}
			else
			{
				outbuf_add(&out, "Something REALLY wierd happened; no record of the object.\n");
			}
			p++;
		}
		outbuf_push(&out);
	}
#endif 

#ifdef F_MALLOC_STATUS
	void f_malloc_status(void)
	{
		outbuffer_t ob;

		outbuf_zero(&ob);

		#ifdef BSDMALLOC
			outbuf_add(&ob, "Using BSD malloc");
		#endif 
		#ifdef SMALLOC
			outbuf_add(&ob, "Using Smalloc");
		#endif 
		#ifdef SYSMALLOC
			outbuf_add(&ob, "Using system malloc");
		#endif 
		#ifdef DEBUGMALLOC
			outbuf_add(&ob, ", wrapped with debugmalloc");
		#endif 
		#ifdef WRAPPEDMALLOC
			outbuf_add(&ob, ", wrapped with wrappedmalloc");
		#endif 
		outbuf_add(&ob, ".\n");
		#ifdef DO_MSTATS
			show_mstats(&ob, "malloc_status()");
		#endif 
		#if (defined(WRAPPEDMALLOC) || defined(DEBUGMALLOC))
			dump_malloc_data(&ob);
		#endif 
		outbuf_push(&ob);
	}
#endif 

#ifdef F_MAP_DELETE
	void f_map_delete(void)
	{
		mapping_delete( (sp - 1)->u.map, sp);
		pop_stack();
		#ifndef COMPAT_32
			free_mapping( (sp--)->u.map);
		#endif 
	}
#endif 

#ifdef F_MAPP
	void f_mapp(void)
	{
		if (sp->type == T_MAPPING)
		{
			free_mapping(sp->u.map);
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_mapp");
			*sp = const0;
		}
	}
#endif 


#ifdef F_MAP
	void f_map(void)
	{
		svalue_t *arg = sp - st_num_arg + 1;

		if (arg->type == T_MAPPING)
		{
			map_mapping(arg, st_num_arg);
		}
		else if (arg->type == T_ARRAY)
		{
			map_array(arg, st_num_arg);
		}
		else
		{
			map_string(arg, st_num_arg);
		}
	}
#endif 

#ifdef F_FUNCTION_EXISTS
void f_function_exists (void)
{
    char *str, *res;
    int l;
    object_t *ob;
    int flag = 0;
    
    if (st_num_arg > 1) {
	if (st_num_arg > 2)
	    flag = (sp--)->u.number;
	ob = (sp--)->u.ob;
	free_object(ob, "f_function_exists");
    } else {
	if (current_object->flags & O_DESTRUCTED) {
	    free_string_svalue(sp);
	    *sp = const0;
	    return;
	}
	ob = current_object;
    }

    str = function_exists(sp->u.string, ob, flag);
    free_string_svalue(sp);
    if (str) {
	l = SHARED_STRLEN(str) - 2; /* no .c */
	res = new_string(l + 1, "function_exists");
	res[0] = '/';
	strncpy(res + 1, str, l);
	res[l + 1] = 0;

	sp->subtype = STRING_MALLOC;
        sp->u.string = res;
    } else *sp = const0;
}
#endif

/*
This efun searches a mapping for a path.  Each key is assumed to be a
string.  The value is completely arbitrary.  The efun finds the largest
matching path in the mapping.  Keys ended in '/' are assumed to match
paths with character that follow the '/', i.e. / is a wildcard for anything
below this directory.  DO NOT CHANGE THIS EFUN TIL YOU UNDERSTAND IT.  It
catches folks by suprise at first, but it is coded the way it is for a reason.
It effectively implements the search loop in TMI's access object as a single
efun.

Cygnus
 */
#ifdef F_MEMBER_ARRAY
	void f_member_array(void)
	{
		array_t *v;
		int i;

		if (st_num_arg > 2)
		{
			i = (sp--)->u.number;
			if (i < 0)
			{
				bad_arg(3, F_MEMBER_ARRAY);
			}
		}
		else
		{
			i = 0;
		}

		if (sp->type == T_STRING)
		{
			char *res;
			CHECK_TYPES(sp - 1, T_NUMBER, 1, F_MEMBER_ARRAY);
			if (i > (int)SVALUE_STRLEN(sp) )
			{
				error("QTZ_ServerEngine:Index to start search is > string length.\n");
			}
			if ( (res = strchr(sp->u.string + i, (sp - 1)->u.number) ) )
			{
				i = res - sp->u.string;
			}
			else
			{
				i =  - 1;
			}
			free_string_svalue(sp--);
		}
		else
		{
			int size = (v = sp->u.arr)->size;
			svalue_t *sv;
			svalue_t *find;
			int flen = 0;

			find = (sp - 1);
			/* optimize a bit */
			if (find->type == T_STRING)
			{
				/* *not* COUNTED_STRLEN() which can do a (costly) strlen() call */
				if (find->subtype &STRING_COUNTED)
				{
					flen = MSTR_SIZE(find->u.string);
				}
				else
				{
					flen = 0;
				}
			}

			for (; i < size; i++)
			{
				switch (find->type | (sv = v->item + i)->type)
				{
					case T_STRING:
						if (flen && (sv->subtype &STRING_COUNTED) && flen != MSTR_SIZE(sv->u.string) )
						{
							continue;
						}
						if (strcmp(find->u.string, sv->u.string) )
						{
							continue;
						}
						break;
					case T_NUMBER:
						if (find->u.number == sv->u.number)
						{
							break;
						}
						continue;
					case T_REAL:
						if (find->u.real == sv->u.real)
						{
							break;
						}
						continue;
					case T_ARRAY:
						if (find->u.arr == sv->u.arr)
						{
							break;
						}
						continue;
					case T_OBJECT:
						{
							if (sv->u.ob->flags &O_DESTRUCTED)
							{
								assign_svalue(sv, &const0);
								continue;
							}
							if (find->u.ob == sv->u.ob)
							{
								break;
							}
							continue;
						}
					case T_MAPPING:
						if (find->u.map == sv->u.map)
						{
							break;
						}
						continue;
					case T_FUNCTION:
						if (find->u.fp == sv->u.fp)
						{
							break;
						}
						continue;
					case T_BUFFER:
						if (find->u.buf == sv->u.buf)
						{
							break;
						}
						continue;
					default:
						if (sv->type == T_OBJECT && sv->u.ob->flags &O_DESTRUCTED)
						{
							assign_svalue(sv, &const0);
							if (find->type == T_NUMBER && !find->u.number)
							{
								break;
							}
						}
						continue;
				}
				break;
			}
			if (i == size)
			{
				i =  - 1;
			}
			 /* Return -1 for failure */
			free_array(v);
			free_svalue(find, "f_member_array");
			sp--;
		}
		put_number(i);
	}
#endif 

#ifdef F_MKDIR
	void f_mkdir(void)
	{
		char *path;

		path = check_valid_path(sp->u.string, current_object, "mkdir", 1);
		if (!path || OS_mkdir(path, 0770) ==  - 1)
		{
			free_string_svalue(sp);
			*sp = const0;
		}
		else
		{
			free_string_svalue(sp);
			*sp = const1;
		}
	}
#endif 

#ifdef F_MOVE_OBJECT
	void f_move_object(void)
	{
		object_t *o1,  *o2;

		/* get destination */
		if (sp->type == T_OBJECT)
		{
			o2 = sp->u.ob;
		}
		else
		{
			if (!(o2 = find_object(sp->u.string) ) || !object_visible(o2) )
			{
				error("QTZ_ServerEngine: could not find destination\n");
			}
		}

		if ( (o1 = current_object)->flags &O_DESTRUCTED)
		{
			error("QTZ_ServerEngine:can't move a destructed object\n");
		}

		move_object(o1, o2);
		pop_stack();
	}
#endif 

#ifdef F_OBJECTP
	void f_objectp(void)
	{
		if (sp->type == T_OBJECT)
		{
			free_object(sp->u.ob, "f_objectp");
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_objectp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_OPCPROF
	void f_opcprof(void)
	{
		if (st_num_arg == 1)
		{
			opcdump(sp->u.string);
			free_string_svalue(sp--);
		}
		else
		{
			opcdump("/OPCPROF");
		}
	}
#endif 

#ifdef F_ORIGIN
	void f_origin(void)
	{
		push_constant_string(origin_name(caller_type) );
	}
#endif 

#ifdef F_POINTERP
	void f_pointerp(void)
	{
		if (sp->type == T_ARRAY)
		{
			free_array(sp->u.arr);
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_pointerp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_PREVIOUS_OBJECT
	void f_previous_object(void)
	{
		control_stack_t *p;
		int i;

		if ( (i = sp->u.number) > 0)
		{
			if (i >= CFG_MAX_CALL_DEPTH)
			{
				sp->u.number = 0;
				return ;
			}
			ob = 0;
			p = csp;
			do
			{
				if ( (p->framekind &FRAME_OB_CHANGE) && !(--i) )
				{
					ob = p->prev_ob;
					break;
				}
			}

			while (--p >= control_stack);
		}
		else if (i ==  - 1)
		{
			array_t *v;

			i = previous_ob ? 1 : 0;
			p = csp;
			do
			{
				if ( (p->framekind &FRAME_OB_CHANGE) && p->prev_ob)
				{
					i++;
				}
			}

			while (--p >= control_stack);
			v = allocate_empty_array(i);
			p = csp;
			if (previous_ob)
			{
				if (!(previous_ob->flags &O_DESTRUCTED) )
				{
					v->item[0].type = T_OBJECT;
					v->item[0].u.ob = previous_ob;
					add_ref(previous_ob, "previous_object(-1)");
				}
				else
				{
					v->item[0] = const0;
				}
				i = 1;
			}
			else
			{
				i = 0;
			}
			do
			{
				if ( (p->framekind &FRAME_OB_CHANGE) && (ob = p->prev_ob) )
				{
					if (!(ob->flags &O_DESTRUCTED) )
					{
						v->item[i].type = T_OBJECT;
						v->item[i].u.ob = ob;
						add_ref(ob, "previous_object(-1)");
					}
					else
					{
						v->item[i] = const0;
					}
					i++;
				}
			}

			while (--p >= control_stack);
			put_array(v);
			return ;
		}
		else if (i < 0)
		{
			error("QTZ_ServerEngine:Bad negative argument to previous_object()\n");
		}
		else
		{
			ob = previous_ob;
		}
		if (!ob || (ob->flags &O_DESTRUCTED) )
		{
			sp->u.number = 0;
		}
		else
		{
			put_unrefed_undested_object(ob, "previous_object()");
		}
	}
#endif 

#ifdef F_PRINTF
	void f_printf(void)
	{
		if (sp->type == T_STRING)
		{
			printf("%s", sp->u.string);
			free_string_svalue(sp);
			sp = sp - 1;
		}
	}
#endif 

#ifdef F_QUERY_LOAD_AVERAGE
	void f_query_load_average(void)
	{
		copy_and_push_string(query_load_av() );
	}
#endif 

#ifdef F_GET_VERB
	void f_get_verb(void)
	{
		if (!last_verb)
		{
			push_number(0);
			return ;
		}
		share_and_push_string(last_verb);
	}
#endif 

#ifdef F_RANDOM
	void f_random(void)
	{
		if (sp->u.number <= 0)
		{
			sp->u.number = 0;
			return ;
		}
		sp->u.number = random_number(sp->u.number);
	}
#endif 

#ifdef F_READ_BYTES
	void f_read_bytes(void)
	{
		char *str;
		int start = 0, len = 0, rlen = 0, num_arg = st_num_arg;
		svalue_t *arg;

		arg = sp - num_arg + 1;
		if (num_arg > 1)
		{
			start = arg[1].u.number;
		}
		if (num_arg == 3)
		{
			len = arg[2].u.number;
		}
		str = read_bytes(arg[0].u.string, start, len, &rlen);
		pop_n_elems(num_arg);
		if (str == 0)
		{
			push_number(0);
		}
		else
		{
			push_malloced_string(str);
		}
	}
#endif 

#ifdef F_READ_FILE
	void f_read_file(void)
	{
		char *str;
		int start, len;

		if (st_num_arg == 3)
		{
			len = (sp--)->u.number;
		}
		else
		{
			len = 0;
		}
		if (st_num_arg > 1)
		{
			start = (sp--)->u.number;
		}
		else
		{
			start = 0;
		}

		str = read_file(sp->u.string, start, len);
		free_string_svalue(sp);
		if (!str)
		{
			 *sp = const0;
		}
		else
		{
			sp->subtype = STRING_MALLOC;
			sp->u.string = str;
		}
	}
#endif 

#ifdef F_REGEXP
	void f_regexp(void)
	{
		array_t *v;
		int flag;

		if (st_num_arg > 2)
		{
			if (!(sp->type == T_NUMBER) )
			{
				error("QTZ_ServerEngine:Bad 3rd argument to fucnciton\n");
			}
			if (sp[ - 2].type == T_STRING)
			{
				error("QTZ_ServerEngine:Bad 3rd argument for function\n");
			}
			flag = (sp--)->u.number;
		}
		else
		{
			flag = 0;
		}
		if (sp[ - 1].type == T_STRING)
		{
			flag = match_single_regexp( (sp - 1)->u.string, sp->u.string);
			free_string_svalue(sp--);
			free_string_svalue(sp);
			put_number(flag);
		}
		else
		{
			v = match_regexp( (sp - 1)->u.arr, sp->u.string, flag);
			free_string_svalue(sp--);
			free_array(sp->u.arr);
			sp->u.arr = v;
		}
	}
#endif 

#ifdef F_RENAME
	void f_rename(void)
	{
		int i;

		i = do_rename( (sp - 1)->u.string, sp->u.string, F_RENAME);
		free_string_svalue(sp--);
		free_string_svalue(sp);
		put_number(i);
	}
#endif /* F_RENAME */


#ifdef F_REPLACE_STRING
	/*
	syntax for replace_string is now:
	string replace_string(src, pat, rep);   // or
	string replace_string(src, pat, rep, max);  // or
	string replace_string(src, pat, rep, first, last);

	The 4th/5th args are optional (to retain backward compatibility).
	- src, pat, and rep are all strings.
	- max is an integer. It will replace all occurances up to max
	matches (starting as 1 as the first), with a value of 0 meaning
	'replace all')
	- first and last are just a range to replace between, with
	the following constraints
	first < 1: change all from start
	last == 0 || last > max matches:    change all to end
	first > last: return unmodified array.
	(i.e, with 4 args, it's like calling it with:
	replace_string(src, pat, rep, 0, max);
	)
	 */
	void f_replace_string(void)
	{
		int plen, rlen, dlen, slen, first, last, cur, j;

		char *pattern;
		char *replace;
		register char *src,  *dst1,  *dst2;
		svalue_t *arg;
		int skip_table[256];
		char *slimit = 0;
		char *flimit = 0;
		char *climit = 0;
		int probe = 0;
		int skip = 0;

		max_string_length = 200000; //MAX_STRING_LENGTH;

		if (st_num_arg > 5)
		{
			error("QTZ_ServerEngine:Too many args to function.\n");
			pop_n_elems(st_num_arg);
			return ;
		}

		arg = sp - st_num_arg + 1;
		src = arg->u.string;
		first = 0;
		last = 0;

		if (st_num_arg >= 4)
		{
			CHECK_TYPES( (arg + 3), T_NUMBER, 4, F_REPLACE_STRING);
			first = (arg + 3)->u.number;

			if (st_num_arg == 4)
			{
				last = first;
				first = 0;
			}
			else if (st_num_arg == 5)
			{
				CHECK_TYPES(sp, T_NUMBER, 5, F_REPLACE_STRING);
				/* first set above. */
				last = sp->u.number;
			}
		}

		if (!last)
		{
			last = max_string_length;
		}

		if (first > last)
		{
			 /* just return it */
			pop_n_elems(st_num_arg - 1);
			return ;
		}
		pattern = (arg + 1)->u.string;
		plen = SVALUE_STRLEN(arg + 1);
		if (!plen)
		{
			pop_n_elems(st_num_arg - 1); /* just return it */

			return ;
		}
		replace = (arg + 2)->u.string;
		rlen = SVALUE_STRLEN(arg + 2);
		dlen = 0;
		cur = 0;

		if (rlen <= plen)
		{
			/* we're going to do in string replacement */
			unlink_string_svalue(arg);
			src = arg->u.string;
		}



		if (plen > 1)
		{
			/* build skip table */
			for (j = 0; j < 256; j++)
			{
				skip_table[j] = plen;
			}
			for (j = 0; j < plen; j++)
			{
				skip_table[(unsigned char)pattern[j]] = plen - j - 1;
			}
			slen = SVALUE_STRLEN(arg);
			slimit = src + slen;
			flimit = slimit - plen + 1;
			probe = plen - 1;
		}

		if (rlen <= plen)
		{
			/* in string replacement */
			dst2 = dst1 = arg->u.string;

			if (plen > 1)
			{
				 /* pattern length > 1, jump table most efficient */
				while (src < flimit)
				{
					if ( (skip = skip_table[(unsigned char)src[probe]]) )
					{
						for (climit = dst2 + skip; dst2 < climit;  *dst2++ =  *src++)
							;
					}
					else if (memcmp(src, pattern, plen) == 0)
					{
						cur++;
						if ( (cur >= first) && (cur <= last) )
						{
							if (rlen)
							{
								memcpy(dst2, replace, rlen);
								dst2 += rlen;
							}
							src += plen;
							if (cur == last)
							{
								break;
							}
						}
						else
						{
							memcpy(dst2, src, plen);
							dst2 += plen;
							src += plen;
						}
					}
					else
					{
						*dst2++ =  *src++;
					}
				}
				memcpy(dst2, src, slimit - src);
				dst2 += (slimit - src);
				*dst2 = 0;
				arg->u.string = extend_string(dst1, dst2 - dst1);
			}
			else
			{
				 /* pattern length <= 1, brute force most efficient */
				/* Beek - if it was zero, we already returned, so plen == 1 */
				/* assume source string is a string < maximum string length */
				if (rlen)
				{
					while (*src)
					{
						if (*src ==  *pattern)
						{
							cur++;

							if (cur >= first && cur <= last)
							{
								*src =  *replace;
							}
						}
						src++;
					}
				}
				else
				{
					 /* rlen is zero */
					while (*src)
					{
						if (*src++ ==  *pattern)
						{
							cur++;
							if (cur >= first)
							{
								dst2 = src - 1;
								while (*src)
								{
									if (*src ==  *pattern)
									{
										cur++;
										if (cur <= last)
										{
											src++;
											continue;
										}
										else
										{
											while (*src)
											{
												*dst2++ =  *src++;
											}
											break;
										}
									}
									*dst2++ =  *src++;
								}
								*dst2 = 0;
								arg->u.string = extend_string(dst1, dst2 - dst1);
								break;
							}
						}
					}
				}
			}
			pop_n_elems(st_num_arg - 1);
		}
		else
		{

			dst2 = dst1 = new_string(max_string_length, "f_replace_string: 2");

			if (plen > 1)
			{
				while (src < flimit)
				{
					if ( (skip = skip_table[(unsigned char)src[probe]]) )
					{
						for (climit = dst2 + skip; dst2 < climit;  *dst2++ =  *src++)
							;

					}
					else if (memcmp(src, pattern, plen) == 0)
					{
						cur++;
						if ( (cur >= first) && (cur <= last) )
						{
							if (max_string_length - dlen <= rlen)
							{
								pop_n_elems(st_num_arg);
								push_svalue(&const0u);
								FREE_MSTR(dst1);
								return ;
							}
							memcpy(dst2, replace, rlen);
							dst2 += rlen;
							dlen += rlen;
							src += plen;
							if (cur == last)
							{
								break;
							}
						}
						else
						{
							dlen += plen;
							if (max_string_length - dlen <= 0)
							{
								pop_n_elems(st_num_arg);
								push_svalue(&const0u);

								FREE_MSTR(dst1);
								return ;
							}
							memcpy(dst2, src, plen);
							dst2 += plen;
							src += plen;
						}
					}
					else
					{
						if (max_string_length - dlen <= 1)
						{
							pop_n_elems(st_num_arg);
							push_svalue(&const0u);

							FREE_MSTR(dst1);
							return ;
						}
						*dst2++ =  *src++;
						dlen++;
					}
				}
				if (max_string_length - dlen <= (slimit - src) )
				{
					pop_n_elems(st_num_arg);
					push_svalue(&const0u);
					FREE_MSTR(dst1);
					return ;
				}
				memcpy(dst2, src, slimit - src);
				dst2 += (slimit - src);
			}
			else
			{
				 /* plen <= 1 */
				/* Beek: plen == 1 */
				while (*src != '\0')
				{
					if (*src ==  *pattern)
					{
						cur++;
						if (cur >= first && cur <= last)
						{
							if (rlen != 0)
							{
								if (max_string_length - dlen <= rlen)
								{
									pop_n_elems(st_num_arg);
									push_svalue(&const0u);
									FREE_MSTR(dst1);
									return ;
								}
								strncpy(dst2, replace, rlen);
								dst2 += rlen;
								dlen += rlen;
							}
							src++;
							continue;
						}
					}
					if (max_string_length - dlen <= 1)
					{
						pop_n_elems(st_num_arg);
						push_svalue(&const0u);
						FREE_MSTR(dst1);
						return ;
					}
					*dst2++ =  *src++;
					dlen++;
				}
			}
			*dst2 = '\0';

			pop_n_elems(st_num_arg);
			/*
			 * shrink block or make a copy of exact size
			 */
			push_malloced_string(extend_string(dst1, dst2 - dst1) );
		}
	}
#endif 

#define ERR_STR_LEN 256
static char *_Dofilt(char *msg, char *pattern, char *replace)
{
	regex_t r;
	regmatch_t match;
	char result[1024];
	char *p,  *t, errbuf1[ERR_STR_LEN], errbuf2[ERR_STR_LEN];
	size_t pos = 0, len = strlen(replace);
	int err;

	t = msg;

	memset(result, 0, sizeof(result) );

	err = regcomp(&r, pattern, REG_EXTENDED);

	if (err)
	{
		regerror(err, &r, errbuf1, sizeof(errbuf1) );
		snprintf(errbuf2, ERR_STR_LEN, "QTZ_ServerEngine:reg exp comile error:|%s|\n", pattern);
		error(errbuf2);
		return msg;
	}

	while (regexec(&r, t, 1, &match, 0) == 0)
	{
		//printf("start:%lld,end:%lld\n",t-msg+match.rm_so,t-msg+match.rm_eo);//absolute position in string s
		memcpy(result + pos, t, match.rm_so); //first copy the string that doesn't match
		pos += match.rm_so; //change pos
		memcpy(result + pos, replace, len); //then replace 
		pos += len;
		t += match.rm_eo; //for another match
	}
	//printf("String: %s Size: %d \n", t, strlen(t));

	memcpy(result + pos, t, strlen(t) ); //don't forget the last unmatch string
	//末尾加上\0
	len = pos + strlen(t); //strlen(result);
	//printf("len: %d \n", len);
	result[len] = '\0';

	p = new_string(len, "_Dofilt");
	strncpy(p, result, len);
	p[len] = '\0';
	//printf("p: %s \n", p);

	regfree(&r);
	return p;
}

// 只支持3个参数(str, pattern, replace);将str中的，全部符合pattern的字符串，转成replace
void f_replace_string_reg(void)
{
	char *str = NULL,  *pattern = NULL,  *replace = NULL;
	char *result;

	replace = sp->u.string;
	pattern = (sp - 1)->u.string;
	str = (sp - 2)->u.string;

	result = _Dofilt(str, pattern, replace);

	free_string_svalue(sp--);
	free_string_svalue(sp--);
	free_string_svalue(sp);

	put_malloced_string(result);
}

#ifdef F_RESTORE_OBJECT
	void f_restore_object(void)
	{
		int flag;

		flag = (st_num_arg > 1) ? (sp--)->u.number: 0;
		flag = restore_object(current_object, sp->u.string, flag);
		free_string_svalue(sp);
		put_number(flag);
	}
#endif 


#ifdef F_RESTORE_VARIABLE
	void f_restore_variable(void)
	{
		svalue_t v;

		unlink_string_svalue(sp);
		v.type = T_NUMBER;

		restore_variable(&v, sp->u.string);
		FREE_MSTR(sp->u.string);
		*sp = v;
	}
#endif 

#ifdef F_RM
	void f_rm(void)
	{
		int i;

		i = remove_file(sp->u.string);
		free_string_svalue(sp);
		put_number(i);
	}
#endif 

#ifdef F_RMDIR
	void f_rmdir(void)
	{
		char *path;

		path = check_valid_path(sp->u.string, current_object, "rmdir", 1);
		if (!path || rmdir(path) ==  - 1)
		{
			free_string_svalue(sp);
			*sp = const0;
		}
		else
		{
			free_string_svalue(sp);
			*sp = const1;
		}
	}
#endif 

#ifdef F_SAVE_OBJECT
	void f_save_object(void)
	{
		int flag;

		flag = (st_num_arg == 2) && (sp--)->u.number;
		flag = save_object(current_object, sp->u.string, flag);
		free_string_svalue(sp);
		put_number(flag);
	}
#endif 

#ifdef F_SAVE_VARIABLE
	void f_save_variable(void)
	{
		char *p;

		p = save_variable(sp);
		pop_stack();
		push_malloced_string(p);
	}
#endif 

#ifdef F_SET_EVAL_LIMIT
	/* warning: do not enable this without using valid_override() in the master
	object and a set_eval_limit() simul_efun to restrict access.
	 */
	void f_set_eval_limit(void)
	{
		switch (sp->u.number)
		{
			case 0:
				sp->u.number = eval_cost = max_cost;
				break;
			case - 1: 
				sp->u.number = eval_cost;
				break;
			case 1:
				sp->u.number = max_cost;
				break;
			default:
				max_cost = sp->u.number;
				break;
		}
	}
#endif 

#ifdef F_SET_BIT
	void f_set_bit(void)
	{
		char *str;
		int len, old_len, ind, bit;

		if (sp->u.number > MAX_BITS)

		{
			error("QTZ_ServerEngine:function bit requested: %d > maximum bits: %d\n", sp->u.number, MAX_BITS);
		}
		bit = (sp--)->u.number;
		if (bit < 0)
		{
			error("QTZ_ServerEngine:Bad 3rd argument to function.\n");
		}
		ind = bit / 6;
		bit %= 6;
		old_len = len = SVALUE_STRLEN(sp);
		if (ind >= len)
		{
			len = ind + 1;
		}
		if (ind < old_len)
		{
			unlink_string_svalue(sp);
			str = sp->u.string;
		}
		else
		{
			str = new_string(len, "f_set_bit: str");
			str[len] = '\0';
			if (old_len)
			{
				memcpy(str, sp->u.string, old_len);
			}
			if (len > old_len)
			{
				memset(str + old_len, ' ', len - old_len);
			}
			free_string_svalue(sp);
			sp->subtype = STRING_MALLOC;
			sp->u.string = str;
		}

		if (str[ind] > 0x3f + ' ' || str[ind] < ' ')
		{
			error("QTZ_ServerEngine:Bad bit pattern in that fucntion character %d\n", ind);
		}
		str[ind] = ( (str[ind] - ' ') | (1 << bit) ) + ' ';
	}
#endif 

#ifdef F_SET_HEART_BEAT
	void f_set_heart_beat(void)
	{
		set_heart_beat(current_object, (sp--)->u.number);
	}
#endif 
#ifdef F_GET_HEART_BEAT
	void  //f_query_heart_beat (void)
	f_get_heart_beat(void)
	{
		free_object(ob = sp->u.ob, "f_get_heart_beat");
		put_number(query_heart_beat(ob) );
	}
#endif 

#ifdef F_SHUTDOWN
	void f_shutdown(void)
	{
		shutdownOS(st_num_arg ? sp->u.number: (*++sp = const0, 0) );
	}
#endif 

#ifdef F_SIZEOF
	void f_sizeof(void)
	{
		int i;

		switch (sp->type)
		{
			case T_CLASS:
				i = sp->u.arr->size;
				free_class(sp->u.arr);
				break;
			case T_ARRAY:
				i = sp->u.arr->size;
				free_array(sp->u.arr);
				break;
			case T_MAPPING:
				i = sp->u.map->count;
				free_mapping(sp->u.map);
				break;
			case T_BUFFER:
				i = sp->u.buf->size;
				free_buffer(sp->u.buf);
				break;
			case T_STRING:
				i = SVALUE_STRLEN(sp);
				free_string_svalue(sp);
				break;
			default:
				i = 0;
				free_svalue(sp, "f_sizeof");
		}
		sp->type = T_NUMBER;
		sp->u.number = i;
	}
#endif 

#ifdef F_SPRINTF
	void f_sprintf(void)
	{
		char *s;
		int num_arg = st_num_arg;

		s = string_print_formatted( (sp - num_arg + 1)->u.string, num_arg - 1, sp - num_arg + 2);
		pop_n_elems(num_arg);

		(++sp)->type = T_STRING;
		if (!s)
		{
			sp->subtype = STRING_CONSTANT;
			sp->u.string = "";
		}
		else
		{
			sp->subtype = STRING_MALLOC;
			sp->u.string = s;
		}
	}
#endif 


#ifdef F_STAT
	void f_stat(void)
	{
		struct stat buf;
		char *path;
		array_t *v;

		path = check_valid_path( (--sp)->u.string, current_object, "stat", 0);
		if (!path)
		{
			free_string_svalue(sp);
			*sp = const0;
			return ;
		} if (stat(path, &buf) !=  - 1)
		{
			if (buf.st_mode &S_IFREG)
			{
				 /* if a regular file */
				v = allocate_empty_array(3);
				v->item[0].type = T_NUMBER;
				v->item[0].u.number = buf.st_size;
				v->item[1].type = T_NUMBER;
				v->item[1].u.number = buf.st_mtime;
				v->item[2].type = T_NUMBER;
				ob = find_object2(path);
				if (ob && !object_visible(ob) )
				{
					ob = 0;
				}
				if (ob)
				{
					v->item[2].u.number = ob->load_time;
				}
				else
				{
					v->item[2].u.number = 0;
				}
				free_string_svalue(sp);
				put_array(v);
				return ;
			}
		}
		v = get_dir(sp->u.string, (sp + 1)->u.number);
		free_string_svalue(sp);
		if (v)
		{
			put_array(v);
		}
		else
		{
			 *sp = const0;
		}
	}
#endif 

#ifdef F_SUBSTR
	void f_substr(void)
	{
		register char *big,  *little,  *pos,  *ch;
		static char buf[2];
		register int i, j, k;
		int blen, llen;

		sp--;
		big = (sp - 1)->u.string;
		blen = SVALUE_STRLEN(sp - 1);
		if (sp->type == T_NUMBER)
		{
			little = buf;
			if ( (buf[0] = (char)sp->u.number) )
			{
				llen = 1;
			}
			else
			{
				llen = 0;
			}
		}
		else
		{
			little = sp->u.string;
			llen = SVALUE_STRLEN(sp);
		}

		if (!llen || blen < llen)
		{
			pos = NULL;
		}
		/* start at left */
		else if (!( (sp + 1)->u.number) )
		{
			if (!little[1])
			 /* 1 char srch pattern */
			{
				pos = strchr(big, (int)little[0]);
			}
			else
			{
				for (pos = big;  *pos != 0; pos++)
				{
					for (ch = pos, j = 0;  *ch != 0 && j < llen; ch++, j++)
					{
						if (*ch != little[j])
						{
							break;
						}
					}
					if (j == llen)
					{
						break;
					}
					if ( (unsigned char) *pos > 128 && (unsigned char) *pos < 0XFE)
					//for unicode
					{
						pos++;
					}
				}
				if (*pos == 0)
				{
					pos = NULL;
				}
			}
			/* start at right */
		}
		else
		{
			 /* maybe test for -1 */
			if (!little[1])
			 /* 1 char srch pattern */
			{
				pos = strrchr(big, (int)little[0]);
			}
			else
			{
				pos = NULL;
				for (i = blen; i >= llen; i--)
				 /* find from end */
				{
					for (j = i - llen, k = 0; j != i && k < llen; j++, k++)
					{
						if (big[j] != little[k])
						{
							break;
						}
					}
					if (k == llen)
					{
						pos = big + i - llen;
						break;
					}
					if ( (unsigned char)big[i - 1] > 128 && (unsigned char)big[i - 1] < 0XFE)
					{
						i--;
					}
				}
			}
		}

		if (!pos)
		{
			i =  - 1;
		}
		else
		{
			i = (int) (pos - big);
		}
		if (sp->type == T_STRING)
		{
			free_string_svalue(sp);
		}
		free_string_svalue(--sp);
		put_number(i);
	} /* substr */
#endif 

#ifdef F_STRSRCH

	/*
	 * int strsrch(string big, string little, [ int flag ])
	 * - search for little in big, starting at right if flag is set
	 *   return int offset of little, -1 if not found
	 *
	 * Written 930706 by Luke Mewburn <zak@rmit.edu.au>
	 *
	 */

	void f_strsrch()
	{
		register char *big,  *little,  *pos;
		static char buf[2];
		int i, blen, llen;

		sp--;
		big = (sp - 1)->u.string;
		blen = SVALUE_STRLEN(sp - 1);
		if (sp->type == T_NUMBER)
		{
			little = buf;
			if ( (buf[0] = (char)sp->u.number) )
			{
				llen = 1;
			}
			else
			{
				llen = 0;
			}
		}
		else
		{
			little = sp->u.string;
			llen = SVALUE_STRLEN(sp);
		}

		if (!llen || blen < llen)
		{
			pos = NULL;


		}
		else if (!( (sp + 1)->u.number) )
		{
			if (!little[1])
			{
				pos = strchr(big, (int)little[0]);
			}
			else
			{
				pos = (char*)_strstr(big, little);
			}

		}
		else
		{
			if (!little[1])
			{
				pos = strrchr(big, (int)little[0]);
			}
			else
			{
				char c =  *little;

				pos = big + blen;
				pos -= llen;
				do
				{
					do
					{
						if (*pos == c)
						{
							break;
						}
					}

					while (--pos >= big);
					if (*pos != c)
					{
						pos = NULL;
						break;
					}
					for (i = 1; little[i] && (pos[i] == little[i]); i++)
						;
					if (!little[i])
					{
						break;
					}
				}

				while (--pos >= big)
					;
			}
		}

		if (!pos)
		{
			i =  - 1;
		}
		else
		{
			i = (int) (pos - big);
		}
		if (sp->type == T_STRING)
		{
			free_string_svalue(sp);
		}
		free_string_svalue(--sp);
		put_number(i);
	}

#endif 

#ifdef F_STRCMP
	void f_strcmp(void)
	{
		int i;

		i = strcmp( (sp - 1)->u.string, sp->u.string);
		free_string_svalue(sp--);
		free_string_svalue(sp);
		put_number(i);
	}
#endif 

#ifdef F_STRINGP
	void f_stringp(void)
	{
		if (sp->type == T_STRING)
		{
			free_string_svalue(sp);
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_stringp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_BUFFERP
	void f_bufferp(void)
	{
		if (sp->type == T_BUFFER)
		{
			free_buffer(sp->u.buf);
			*sp = const1;
		}
		else
		{
			free_svalue(sp, "f_bufferp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_SWAP
	void f_swap(void)
	{
		object_t *ob = sp->u.ob;
		control_stack_t *p;

		/* a few sanity checks */
		if (!(ob->flags &O_SWAPPED) && (ob != current_object) )
		{
			for (p = csp; p >= control_stack; p--)
			if (ob == csp->ob)
			{
				pop_stack();
				return ;
			}
			(void)swap(sp->u.ob);
		}

		pop_stack();
	}
#endif 

#ifdef F_TELL_OBJECT
	void f_tell_object(void)
	{
		tell_object( (sp - 1)->u.ob, sp->u.string);
		free_string_svalue(sp--);
		pop_stack();
	}
#endif 

#ifdef F_TELL_ROOM
	void f_tell_room(void)
	{
		array_t *avoid;
		int num_arg = st_num_arg;
		svalue_t *arg = sp - num_arg + 1;

		if (arg->type == T_OBJECT)
		{
			ob = arg[0].u.ob;
		}
		else
		{
			 /* must be a string... */
			ob = find_object(arg[0].u.string);
			if (!ob || !object_visible(ob) )
			{
				error("QTZ_ServerEngine:Bad 1st argument to function\n");
			}
		}

		if (num_arg == 2)
		{
			avoid = &the_null_array;
		}
		else
		{
			avoid = arg[2].u.arr;
		}

		tell_room(ob, &arg[1], avoid);
		free_array(avoid);
		free_svalue(arg + 1, "f_tell_room");
		free_svalue(arg, "f_tell_room");
		sp = arg - 1;
	}
#endif 

#ifdef F_TEST_BIT
	void f_test_bit(void)
	{
		int ind = (sp--)->u.number;

		if (ind / 6 >= (int)SVALUE_STRLEN(sp) )
		{
			free_string_svalue(sp);
			*sp = const0;
			return ;
		}
		if (ind < 0)
		{
			error("Bad 2nd argument to function.\n");
		}
		if ( (sp->u.string[ind / 6] - ' ') &(1 << (ind % 6) ) )
		{
			free_string_svalue(sp);
			*sp = const1;
		}
		else
		{
			free_string_svalue(sp);
			*sp = const0;
		}
	}
#endif 

#ifdef F_NEXT_BIT
	void f_next_bit(void)
	{
		int start = (sp--)->u.number;
		int len = SVALUE_STRLEN(sp);
		int which = 0;
		int bit = 0;
		int value = 0;

		if (!len || start / 6 >= len)
		{
			free_string_svalue(sp);
			put_number( - 1);
			return ;
		}
		/* Find the next bit AFTER start */
		if (start > 0)
		{
			if (start % 6 == 5)
			{
				which = (start / 6) + 1;
				value = sp->u.string[which] - ' ';
			}
			else
			{
				/* we have a partial byte to check */
				which = start / 6;
				bit = 0x3f - ( (1 << ( (start % 6) + 1) ) - 1);
				value = (sp->u.string[which] - ' ') &bit;
			}
		}
		else
		{
			which = 0;
			value =  *sp->u.string - ' ';
		}

		while (1)
		{
			if (value)
			{
				if (value &0x07)
				{
					if (value &0x01)
					{
						bit = which * 6;
					}
					else if (value &0x02)
					{
						bit = which * 6+1;
					}
					else if (value &0x04)
					{
						bit = which * 6+2;
					}
					break;
				}
				else if (value &0x38)
				{
					if (value &0x08)
					{
						bit = which * 6+3;
					}
					else if (value &0x10)
					{
						bit = which * 6+4;
					}
					else if (value &0x20)
					{
						bit = which * 6+5;
					}
					break;
				}
			}
			which++;
			if (which == len)
			{
				bit =  - 1;
				break;
			}
			value = sp->u.string[which] - ' ';
		}

		free_string_svalue(sp);
		put_number(bit);
	}
#endif 

#ifdef F_THIS_OBJECT
	void f_this_object(void)
	{
		if (current_object->flags &O_DESTRUCTED)
		 /* Fixed from 3.1.1 */
		{
			*++sp = const0;
		}
		else
		{
			push_object(current_object);
		}
	}
#endif 

#ifdef F_THROW
	void f_throw(void)
	{
		free_svalue(&catch_value, "f_throw");
		catch_value =  *sp--;
		throw_error(); /* do the longjump, with extra checks... */
	}
#endif 

#ifdef F_TIME
	void f_time(void)
	{
		push_number(current_time);
	}
#endif 

#ifdef F_TO_FLOAT
	void f_to_float(void)
	{
		double temp = 0;

		switch (sp->type)
		{
			case T_NUMBER:
				sp->type = T_REAL;
				sp->u.real = (double)sp->u.number;
				break;
			case T_STRING:
				sscanf(sp->u.string, "%lf", &temp);
				free_string_svalue(sp);
				sp->type = T_REAL;
				sp->u.real = temp;
		}
	}
#endif 

#ifdef F_TO_INT
	void f_to_int(void)
	{
		switch (sp->type)
		{
			case T_REAL:
				sp->type = T_NUMBER;
				sp->u.number = (int)sp->u.real;
				break;
			case T_STRING:
				{
					int temp;

					temp = atoi(sp->u.string);
					free_string_svalue(sp);
					sp->u.number = temp;
					sp->type = T_NUMBER;
					break;
				}
			case T_BUFFER:
				if (sp->u.buf->size < sizeof(int) )
				{
					free_buffer(sp->u.buf);
					*sp = const0;
				}
				else
				{
					int hostint, netint;

					memcpy( (char*) &netint, sp->u.buf->item, sizeof(int) );
					hostint = ntohl(netint);
					free_buffer(sp->u.buf);
					put_number(hostint);
				}
		}
	}
#endif 

#ifdef F_TYPEOF
	void f_typeof(void)
	{
		char *t = type_name(sp->type);

		free_svalue(sp, "f_typeof");
		put_constant_string(t);
	}
#endif 

#ifdef F_UNDEFINEDP
	void f_undefinedp(void)
	{
		if (sp->type == T_NUMBER)
		{
			if (!sp->u.number && (sp->subtype == T_UNDEFINED) )
			{
				*sp = const1;
			}
			else
			{
				 *sp = const0;
			}
		}
		else
		{
			free_svalue(sp, "f_undefinedp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_UPTIME
	void f_uptime(void)
	{
		push_number(current_time - boot_time);
	}
#endif 

#ifdef F_VIRTUALP
	void f_virtualp(void)
	{
		int i;

		i = (int)sp->u.ob->flags &O_VIRTUAL;
		free_object(sp->u.ob, "f_virtualp");
		put_number(i != 0);
	}
#endif 

#ifdef F_WRITE_BYTES
	void f_write_bytes(void)
	{
		int i = 0;

		switch (sp->type)
		{
			case T_NUMBER:
				{
					int netint;
					char *netbuf;

					if (!sp->u.number)
					{
						bad_arg(3, F_WRITE_BYTES);
					}
					netint = htonl(sp->u.number); /* convert to network
					 * byte-order */
					netbuf = (char*) &netint;
					i = write_bytes( (sp - 2)->u.string, (sp - 1)->u.number, netbuf, sizeof(int) );
					break;
				}

			case T_BUFFER:
				{
					i = write_bytes( (sp - 2)->u.string, (sp - 1)->u.number, (char*)sp->u.buf->item, sp->u.buf->size);
					break;
				}

			case T_STRING:
				{
					i = write_bytes( (sp - 2)->u.string, (sp - 1)->u.number, sp->u.string, SVALUE_STRLEN(sp) );
					break;
				}

			default:
				{
					bad_argument(sp, T_BUFFER | T_STRING | T_NUMBER, 3, F_WRITE_BYTES);
				}
		}
		free_svalue(sp--, "f_write_bytes");
		free_string_svalue(--sp);
		put_number(i);
	}
#endif 

#ifdef F_WRITE_FILE
	void f_write_file(void)
	{
		int flags = 0;

		if (st_num_arg == 3)
		{
			flags = (sp--)->u.number;
		}
		flags = write_file( (sp - 1)->u.string, sp->u.string, flags);
		free_string_svalue(sp--);
		free_string_svalue(sp);
		put_number(flags);
	}
#endif 

#ifdef F_DUMP_FILE_DESCRIPTORS
	void f_dump_file_descriptors(void)
	{
		outbuffer_t out;

		outbuf_zero(&out);
		dump_file_descriptors(&out);
		outbuf_push(&out);
	}
#endif 

#ifdef F_RECLAIM_OBJECTS
	void f_reclaim_objects(void)
	{
		push_number(reclaim_objects() );
	}
#endif 

#ifdef F_MEMORY_INFO
	void f_memory_info(void)
	{
		int mem;

		if (st_num_arg == 0)
		{
			int res, tot;

			if (reserved_area)
			{
				res = RESERVED_SIZE;
			}
			else
			{
				res = 0;
			}
			tot = total_prog_block_size + 
			#ifdef ARRAY_STATS
				total_array_size + 
			#endif 
			total_mapping_size + tot_alloc_object_size + tot_alloc_sentence *sizeof(sentence_t) + total_users *sizeof(interactive_t) + show_otable_status(0,  - 1) + heart_beat_status(0,  - 1) + add_string_status(0,  - 1) + res;
			push_number(tot);
			return ;
		}
		if (sp->type != T_OBJECT)
		{
			bad_argument(sp, T_OBJECT, 1, F_MEMORY_INFO);
		}
		ob = sp->u.ob;
		if (ob->prog && (ob->prog->ref == 1 || !(ob->flags &O_CLONE) ) )
		{
			mem = ob->prog->total_size;
		}
		else
		{
			mem = 0;
		}
		mem += (data_size(ob) + sizeof(object_t) );
		free_object(ob, "f_memory_info");
		put_number(mem);
	}
#endif 

#ifdef F_FLOATP
	void f_floatp(void)
	{
		if (sp->type == T_REAL)
		{
			sp->type = T_NUMBER;
			sp->u.number = 1;
		}
		else
		{
			free_svalue(sp, "f_floatp");
			*sp = const0;
		}
	}
#endif 

#ifdef F_SAVE_FUNC
	void f_save_func(void)
	{
		if (FP != NULL)
		{
			free_funp(FP);
		}

		FP = (sp)->u.fp; //不free
		//FP->hdr.ref++;//??
		put_number(FP->hdr.ref);
	}

#endif 

#ifdef F_RUN_FUNC
	void f_run_func(void)
	{
		svalue_t *v;

		if (FP == NULL)
		{
			pop_n_elems(st_num_arg);
			sp++;
			*sp = const0;
			return ;
		}

		v = safe_call_function_pointer(FP, st_num_arg);
		sp++;
		if (v == NULL)
		{
			*sp = const0;
		}
		else
		{
			*sp =  *v;
		}
	}
#endif 

#ifdef F_ALLOCATE_BUFFER
	void f_allocate_buffer(void)
	{
		buffer_t *buf;

		sp--; // dumb arg, useless
		buf = allocate_buffer(sp->u.number);
		if (buf)
		{
			pop_stack();
			//	push_buffer(buf);
			push_refed_buffer(buf);
		}
		else
		{
			assign_svalue(sp, &const0);
		}
	}
#endif 


#ifdef F_READ_BUFFER
	void f_read_buffer(void)
	{
		char *str;
		int start = 0, len = 0, rlen = 0, num_arg = st_num_arg;
		int from_file = 0; // new line
		svalue_t *arg = sp - num_arg + 1;

		if (num_arg > 1)
		{
			start = arg[1].u.number;
			if (num_arg == 3)
			{
				len = arg[2].u.number;
			}
		}
		if (arg[0].type == T_STRING)
		{
			from_file = 1; // new line
			str = read_bytes(arg[0].u.string, start, len, &rlen);
		}
		else
		{
			// T_BUFFER
			str = read_buffer(arg[0].u.buf, start, len, &rlen);
		}
		pop_n_elems(num_arg);
		if (str == 0)
		{
			push_number(0);
		}
		else if (from_file)
		{
			// changed
			buffer_t *buf;

			buf = allocate_buffer(rlen);
			memcpy(buf->item, str, rlen);
			(++sp)->u.buf = buf;
			sp->type = T_BUFFER;
			FREE_MSTR(str);
		}
		else
		{
			// T_BUFFER
			push_malloced_string(str);
		}
	}
#endif 

#ifdef F_WRITE_BUFFER
	void f_write_buffer(void)
	{
		int i = 0;

		if ( (sp - 2)->type == T_STRING)
		{
			f_write_bytes();
			return ;
		}

		switch (sp->type)
		{
			case T_NUMBER:
				{
					int netint;
					char *netbuf;

					netint = htonl(sp->u.number); // convert to network
					// byte-order
					netbuf = (char*) &netint;
					i = write_buffer( (sp - 2)->u.buf, (sp - 1)->u.number, netbuf, sizeof(int) );
					break;
				}

			case T_BUFFER:
				{
					i = write_buffer( (sp - 2)->u.buf, (sp - 1)->u.number, (char*)sp->u.buf->item, sp->u.buf->size);
					break;
				}

			case T_STRING:
				{
					i = write_buffer( (sp - 2)->u.buf, (sp - 1)->u.number, sp->u.string, SVALUE_STRLEN(sp) );
					break;
				}

			default:
				{
					bad_argument(sp, T_BUFFER | T_STRING | T_NUMBER, 3, F_WRITE_BUFFER);
				}
		}
		free_svalue(sp--, "f_write_buffer");
		free_svalue(--sp, "f_write_buffer");
		//free_string_svalue(--sp);
		put_number(i);
	}
#endif 


//返回hex的字符串，32byte
void f_md5_hexdigest(void)
{
	char *input_str = 0,  *p = 0;
	unsigned char buf[17];

	input_str = sp->u.string;
	//会输出一个16byte的二进制串, 空串也不要紧
	md5_digest(buf, input_str, strlen(input_str) );
	//printf("input str is %s, and len is %d\n", input_str, strlen(input_str));
	buf[16] = 0;
	p = new_string(32, "f_md5_hexdigest");
	//result = hexdecode(buf, p, 16);
	hexencode(buf, 16, p);
	p[32] = '\0';
	//printf("input str is %s, buf is %s, and the p is %s\n", input_str, buf, p);
	put_malloced_string(p);
}

#ifdef F_KEYS_BY_VALUE
void f_keys_by_value(void)
{
	int i,j,m,n;
	svalue_t sv_Tmp,*sv_K,*sv_V;
	array_t *vec_K,*vec_V;

	vec_K = mapping_indices(sp->u.map);
	vec_V = mapping_values(sp->u.map);
	free_mapping(sp->u.map);

	m = vec_K->size;
	n = vec_V->size;
	if ( m != n )
	{
		put_number(0);
		return;
	}

	sv_K = vec_K->item;
	sv_V = vec_V->item;
	for(i=n-1;i>0;i--)
	{
		for(j=0;j<i;j++)
		{
			if ( sv_V[j].u.number > sv_V[j+1].u.number )
			{
				sv_Tmp = sv_V[j];
				sv_V[j] = sv_V[j+1];
				sv_V[j+1] = sv_Tmp;

				sv_Tmp = sv_K[j];
				sv_K[j] = sv_K[j+1];
				sv_K[j+1] = sv_Tmp;
			}
		}
	}

	put_array(vec_K);
}
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void f_os_command(void)
{
	//int pid;
	char *cmd = (sp--)->u.string;
	char *param = (sp--)->u.string;
	char *script = (sp--)->u.string;
	char full_path[256];

	if (strcmp(cmd, "") == 0) {
		snprintf(full_path, 256, "%s%s %s", game_lib_path, script, param);
	} else {
		snprintf(full_path, 256, "%s %s%s %s", cmd, game_lib_path, script, param);
	}

	debug_message("excute cmd: %s\n", full_path);

	system(full_path);
	/*
	if( (pid = fork()) < 0 )
	{
	//这里不能return,需要最后的free_string_svalue
	}
	else if( pid == 0 ) //子进程中执行脚本,不做waitpid
	{
	execl( full_path, script, param, (char *) 0 );
	exit(127);
	}
	 */
	free_string_svalue(sp + 3);
	free_string_svalue(sp + 2);
	free_string_svalue(sp + 1);
}

extern char* config_file;
void f_get_config_file(void)
{
	if ( config_file == NULL)
	{
		copy_and_push_string( "" );
	}
	else
	{
		copy_and_push_string( config_file );
	}
}

extern int auth_flag;
void f_is_auth_server(void)
{
	push_number(auth_flag);
}

#ifdef F_LPC_GC
void remove_destructed_objects();
extern object_t *obj_list_destruct;
void f_lpc_gc()
{
	int param = (sp--)->u.number;

	// 保留参数暂时无效
	(void)param;

	if ( obj_list_destruct )
	{
		remove_destructed_objects();
	}
}
#endif 


void f_fs_memory_info(void)
{
	outbuffer_t out;
	int res, tot = 0;
	int tmp;

	outbuf_zero(&out);


	if (reserved_area)
	{
		res = RESERVED_SIZE;
	}
	else
	{
		res = 0;
	}
	tmp = total_prog_block_size;

	outbuf_addv(&out, "total_prog_block_size = %d\n", tmp);

	tot += tmp;
#ifdef ARRAY_STATS
	tmp = total_array_size; 
	outbuf_addv(&out, "total_array_size = %d\n", tmp);
	tot += tmp;
#endif 
	tmp = total_mapping_size ; 
	outbuf_addv(&out, "total_mapping_size = %d\n", tmp);
	tot += tmp;

	tmp = tot_alloc_object_size; 
	outbuf_addv(&out, "tot_alloc_object_size = %d\n", tmp);
	tot += tmp;

	tmp = tot_alloc_sentence *sizeof(sentence_t) ; 
	outbuf_addv(&out, "tot_alloc_sentence *sizeof(sentence_t) = %d\n", tmp);
	tot += tmp;

	tmp = total_users *sizeof(interactive_t); 
	outbuf_addv(&out, "total_users *sizeof(interactive_t) = %d\n", tmp);
	tot += tmp;

	tmp = show_otable_status(&out, 1) ; 
	outbuf_addv(&out, "show_otable_status(&out, 1) = %d\n", tmp);
	tot += tmp;

	tmp = heart_beat_status(&out,  1) ; 
	outbuf_addv(&out, "heart_beat_status(&out,  1) = %d\n", tmp);
	tot += tmp;

	tmp = add_string_status(&out,  1) ; 
	outbuf_addv(&out, "add_string_status(&out,  1) = %d\n", tmp);
	tot += tmp;

	tmp = res;
	outbuf_addv(&out, "res = %d\n", tmp);
	tot += tmp;

	outbuf_addv(&out, "================================================================================\n");

	outbuf_addv(&out, "tot = %d\n", tot);

	outbuf_push(&out);

	return ;
}

