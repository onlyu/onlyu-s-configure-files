#define SUPPRESS_COMPILER_INLINES
#include "std.h"
#include "lpc_incl.h"
#include "file_incl.h"
#include "backend.h"
#include "simul_efun.h"
#include "compiler.h"
#include "otable.h"
#include "comm.h"
#include "lex.h"
#include "binaries.h"
#include "swap.h"
#include "md.h"
#include "eoperators.h"
#include "file.h"
#include "object.h"

/*
 * 'inherit_file' is used as a flag. If it is set to a string
 * after yyparse(), this string should be loaded as an object,
 * and the original object must be loaded again.
 */

char *inherit_file;

/* prevents infinite inherit loops.
No, mark-and-sweep solution won't work.  Exercise for reader.  */
static int num_objects_this_thread = 0;

static object_t *restrict_destruct;

char *last_verb = 0;

int illegal_sentence_action;
object_t *obj_list,  *obj_list_destruct = NULL;
object_t *current_object; /* The object interpreting a function. */
object_t *command_giver; /* Where the current command came from. */
object_t *current_interactive; /* The user who caused this execution */

#ifdef PRIVS
	static void init_privs_for_object(object_t*);
#endif 
static svalue_t *load_virtual_object(char*);
static char *make_new_name(char*);

INLINE void check_legal_string(char *s)
{
	if (strlen(s) > LARGEST_PRINTABLE_STRING)
	{
		error("Printable strings limited to length of %d.\n", LARGEST_PRINTABLE_STRING);
	}
}

/* equivalent to strcpy(x, y); return x + strlen(y), but faster and safer */
/* Code like:
 *
 * char buf[256];
 * strcpy(buf, ...);
 * strcat(buf, ...);
 * strcat(buf, ...);
 *
 * Should be replaced with:
 *
 * char buf[256];
 * char *p, *end = EndOf(buf);
 * p = strput(buf, end, ...);
 * p = strput(p, end, ...);
 * p = strput(p, end, ...);
 */
char *strput(char *x, char *limit, char *y)
{
	while ( (*x++ =  *y++) )
	{
		if (x == limit)
		{
			*(x - 1) = 0;
			break;
		}
	}
	return x - 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

char *strput_int(char *x, char *limit, int num)
{
	char buf[20];
	sprintf(buf, "%d", num);
	return strput(x, limit, buf);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef PRIVS
	static char *privs_file_fname = (char*)0;

	static void init_privs_for_object(object_t *ob)
	{
		object_t *tmp_ob;
		svalue_t *value;

		if (!master_ob)
		{
			tmp_ob = ob;
		}
		else
		{
			tmp_ob = master_ob;
		}

		if (!current_object)
		{
			ob->privs = NULL;
			return ;
		}
		push_malloced_string(add_slash(ob->name) );
		if (!privs_file_fname)
		{
			privs_file_fname = make_shared_string(APPLY_PRIVS_FILE);
		}
		value = apply(privs_file_fname, tmp_ob, 1, ORIGIN_DRIVER);
		if (value == NULL || value->type != T_STRING)
		{
			ob->privs = NULL;
		}
		else
		{
			ob->privs = make_shared_string(value->u.string);
		}
	}
#endif /* PRIVS */


static svalue_t *load_virtual_object(char *name)
{
	svalue_t *v;

	if (!master_ob)
	{
		return 0;
	}
	push_malloced_string(add_slash(name) );
	v = apply_master_ob(APPLY_COMPILE_OBJECT, 1);
	if (!v || (v->type != T_OBJECT) )
	{
		return 0;
	}
	return v;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void set_master(object_t *ob)
{

	master_ob = ob;
	/* Make sure master_ob is never made a dangling pointer. */
	add_ref(master_ob, "set_master");
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

char *check_name(char *src)
{
	char *p;
	while (*src == '/')
	{
		src++;
	}
	p = src;
	while (*p)
	{
		if (*p == '/' && *(p + 1) == '/')
		{
			return 0;
		}
		p++;
	}
	return src;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int strip_name(char *src, char *dest, int size)
{
	char last_c = 0;
	char *p = dest;
	char *end = dest + size - 1;

	while (*src == '/')
	{
		src++;
	}

	while (*src && p < end)
	{
		if (last_c == '/' &&  *src == '/')
		{
			return 0;
		}
		last_c = (*p++ =  *src++);
	}

	/* In some cases, (for example, object loading) this currently gets
	 * run twice, once in find_object, and once in load object.  The
	 * net effect of this is:
	 * /foo.c -> /foo [no such exists, try to load] -> /foo created
	 * /foo.c.c -> /foo.c [no such exists, try to load] -> /foo created
	 *
	 * causing a duplicate object crash.  There are two ways to fix this:
	 * (1) strip multiple .c's so that the output of this routine is something
	 *     that doesn't change if this is run again.
	 * (2) make sure this routine is only called once on any name.
	 *
	 * The first solution is the one currently in use.
	 */
	while (p - dest > 2 && p[ - 1] == 'c' && p[ - 2] == '.')
	{
		p -= 2;
	}

	*p = 0;
	return 1;
}

/*
 * Load an object definition from file. If the object wants to inherit
 * from an object that is not loaded, discard all, load the inherited object,
 * and reload again.
 *
 * In QTZ_ServerEnginelib3.0 when loading inherited objects, their reset() is not called.
 * - why is this??  it makes no sense and causes a problem when a developer
 * inherits code from a real used item.  Say a room for example.  In this case
 * the room is loaded but is never set up properly, so when someone enters it
 * it's all messed up.  Realistically, I know that it's pretty bad style to
 * inherit from an object that's actually being used and isn't just a building
 * block, but I see no reason for this limitation.  It happens, and when it
 * does occur, produces mysterious results than can be hard to track down.
 * for now, I've reenabled resetting.  We'll see if anything breaks. -WF
 *
 * Save the command_giver, because reset() in the new object might change
 * it.
 *
 */
object_t *int_load_object(char *lname)
{
	int f;
	program_t *prog;
	object_t *ob,  *save_command_giver = command_giver;
	struct stat c_st;
	char real_name[200], name[200];


	if (++num_objects_this_thread > INHERIT_CHAIN_SIZE)
	{
		error("OS error: Inherit chain too deep: > %d when trying to load '%s'.\n", INHERIT_CHAIN_SIZE, lname);
	}
	if (!strip_name(lname, name, sizeof name) )
	{
		error("OS error: Filenames with consecutive /'s in them aren't allowed (%s).\n", lname);
	}

	/*
	 * First check that the c-file exists.
	 */
	(void)strcpy(real_name, name);
	(void)strcat(real_name, ".c");

	//debug_message("loading (%d) %s\n",comp_flag++, real_name);
	{
		char bname[256];
		char *pbname = bname;
		sprintf(bname, "%s/%s.b", SAVE_BINARIES, name);
		if (bname[0] == '/')
		{
			pbname++;
		}

		if (stat(pbname, &c_st) ==  - 1 && stat(real_name, &c_st) ==  - 1)
		{
			debug_message("checking file %s\n", pbname);
			num_objects_this_thread--;
			return 0;
		} 
	}

	/*
	 * Check if it's a legal name.
	 */
	if (!legal_path(real_name) )
	{
		debug_message("OS debug: Illegal pathname: /%s\n", real_name);
		error("OS error: Illegal path name '/%s'.\n", real_name);
		return 0;
	}
#ifdef BINARIES
	if (!(prog = load_binary(real_name, lpc_obj) ) && !inherit_file)
	{
#endif 
		/* maybe move this section into compile_file? */
		if (comp_flag)
		{
			debug_message("OS debug: compiling /%s ...", real_name);
			#ifdef LATTICE
				fflush(stderr);
			#endif 
		}
		f = open(real_name, O_RDONLY);
		if (f ==  - 1)
		{
			debug_perror("OS debug: compile_file", real_name);
			error("OS error: Could not read the file '/%s'.\n", real_name);
		}

		prog = compile_file(f, real_name);

		if (comp_flag)
		{
			debug_message(" done\n");
		}
		update_compile_av(total_lines);
		total_lines = 0;
		close(f);
#ifdef BINARIES
	}
#endif 

	/* Sorry, can't handle objects without programs yet. */
	if (inherit_file == 0 && (num_parse_error > 0 || prog == 0) )
	{
		if (prog)
		{
			free_prog(prog, 1);
		}
		if (num_parse_error == 0 && prog == 0)
		{
			error("OS error: No program in object '/%s'!\n", name);
		}
		error("OS error: %d Error in loading object '/%s',\n", num_parse_error, name);
	}



	/*
	 * This is an iterative process. If this object wants to inherit an
	 * unloaded object, then discard current object, load the object to be
	 * inherited and reload the current object again. The global variable
	 * "inherit_file" will be set by lang.y to point to a file name.
	 */
	if (inherit_file)
	{
		object_t *inh_obj;
		char inhbuf[MAX_OBJECT_NAME_SIZE];


		if (!strip_name(inherit_file, inhbuf, sizeof inhbuf) )
		{
			strcpy(inhbuf, inherit_file);
		}
		FREE(inherit_file);
		inherit_file = 0;


		if (prog)
		{
			free_prog(prog, 1);
			prog = 0;
		}
		if (strcmp(inhbuf, name) == 0)
		{
			error("OS error: Illegal to inherit self.\n");
		}

		if ( (inh_obj = lookup_object_hash(inhbuf) ) )
		{
			IF_DEBUG(fatal("OS debug:Inherited object is already loaded!") );
		}
		else
		{
			inh_obj = load_object(inhbuf, 0);
		}
		if (!inh_obj)
		{
			error("OS error: Inherited file '/%s' does not exist!\n", inhbuf);
		}

		/*
		 * Yes, the following is necessary.  It is possible that when we
		 * loaded the inherited object, it loaded this object from it's
		 * create function. Without this check, that would crash the driver.
		 * -Beek
		 */
		if (!(ob = lookup_object_hash(name) ) )
		{
			ob = load_object(name, 0);
			/* sigh, loading the inherited file removed us */
			if (!ob)
			{
				num_objects_this_thread--;
				return 0;
			}
			ob->load_time = current_time;
		}
		num_objects_this_thread--;
		return ob;
	}



	ob = get_empty_object(prog->num_variables_total);
	/* Shared string is no good here */
	ob->name = alloc_cstring(name, "load_object");
	SET_TAG(ob->name, TAG_OBJ_NAME);
	ob->prog = prog;
	ob->flags |= O_WILL_RESET; /* must be before reset is first called */
	ob->next_all = obj_list;
	obj_list = ob;
	enter_object_hash(ob); /* add name to fast object lookup table */

	call_create(ob, 0);

	command_giver = save_command_giver;
	#ifdef DEBUG
		if (d_flag > 1 && ob)
		{
			debug_message("--/%s loaded\n", ob->name);
		}
	#endif 
	ob->load_time = current_time;
	num_objects_this_thread--;
	return ob;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

static char *make_new_name(char *str)
{
	static int i;
	char *p = DXALLOC(strlen(str) + 10, TAG_OBJ_NAME, "make_new_name");

	(void)sprintf(p, "%s#%d", str, i);
	i++;
	return p;
}


/*
 * Save the command_giver, because reset() in the new object might change
 * it.
 */
object_t *clone_object(char *str1, int num_arg)
{
	object_t *ob,  *new_ob;
	object_t *save_command_giver = command_giver;

	num_objects_this_thread = 0;
	ob = find_object(str1);
	if (ob && !object_visible(ob) )
	{
		ob = 0;
	}
	/*
	 * If the object self-destructed...
	 */
	if (ob == 0)
	{
		 /* fix from 3.1.1 */
		pop_n_elems(num_arg);
		return (0);
	}
	if (ob->flags &O_CLONE)
	{
		if (!(ob->flags &O_VIRTUAL) || strrchr(str1, '#') )
		{
			error("OS error: Cannot clone from a clone\n");
		}
		else
		{
			/*
			 * well... it's a virtual object.  So now we're going to "clone"
			 * it.
			 */
			svalue_t *v;

			pop_n_elems(num_arg); /* possibly this should be smarter */
			/* but then, this whole section is a
			kludge and should be looked at.

			Note that create() never gets called
			in clones of virtual objects.
			-Beek */

			if (!(str1 = check_name(str1) ) )
			{
				error("OS error: Filenames with consecutive /'s in them aren't allowed (%s).\n", str1);
			}

			if (ob->ref == 1)
			{
				/*
				 * ob unused so reuse it instead to save space. (possibly
				 * loaded just for cloning)
				 */
				new_ob = ob;
			}
			else
			{
				/* can't reuse, so load another */
				if (!(v = load_virtual_object(str1) ) )
				{
					return 0;
				}
				new_ob = v->u.ob;
			}
			remove_object_hash(new_ob);
			if (new_ob->name)
			{
				FREE(new_ob->name);
			}
			/* Now set the file name of the specified object correctly... */
			new_ob->name = make_new_name(str1);
			enter_object_hash(new_ob);
			new_ob->flags |= O_VIRTUAL;
			new_ob->load_time = current_time;
			command_giver = save_command_giver;
			return (new_ob);
			/*
			 * we can skip all of the stuff below since we were already
			 * cloned once to have gotten to this stage.
			 */
		}
	}
	/* We do not want the heart beat to be running for unused copied objects */
	if (ob->flags &O_HEART_BEAT)
	{
		(void)set_heart_beat(ob, 0);
	}
	new_ob = get_empty_object(ob->prog->num_variables_total);
	new_ob->name = make_new_name(ob->name);
	new_ob->flags |= (O_CLONE | (ob->flags &(O_WILL_CLEAN_UP | O_WILL_RESET) ) );
	new_ob->load_time = ob->load_time;
	new_ob->prog = ob->prog;
	reference_prog(ob->prog, "clone_object");
	//    DEBUG_CHECK(!current_object, "clone_object() from no current_object !\n");

	new_ob->next_all = obj_list;
	obj_list = new_ob;
	enter_object_hash(new_ob); /* Add name to fast object lookup table */
	call_create(new_ob, num_arg);
	command_giver = save_command_giver;
	/* Never know what can happen ! :-( */
	if (new_ob->flags &O_DESTRUCTED)
	{
		return (0);
	}
	return (new_ob);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void init_master(char *file)
{
	char buf[512];
	object_t *new_ob;

	if (!file || !file[0])
	{
		fprintf(stderr, "OS error: No master object specified in config file\n");
		exit( - 1);
	}

	if (!strip_name(file, buf, sizeof buf) )
	{
		error("OS error: Illegal master file name '%s'\n", file);
	}


	if (file[strlen(file) - 2] != '.')
	{
		strcat(buf, ".c");
	}

	new_ob = load_object(file, compiled_version);
	if (new_ob == 0)
	{
		fprintf(stderr, "OS error: The master file %s was not loaded.\n", file);
		exit( - 1);
	}
	set_master(new_ob);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

static char *saved_master_name;
static char *saved_simul_name;

static void fix_object_names()
{
	master_ob->name = saved_master_name;
	simul_efun_ob->name = saved_simul_name;
}

/*
 * Remove an object. It is first moved into the destruct list, and
 * not really destructed until later. (see destruct2()).
 */
void destruct_object(object_t *ob)
{
	object_t **pp;
	int removed;

	if (restrict_destruct && restrict_destruct != ob)
	{
		error("OS error: Only this_object() can be destructed from move_or_destruct.\n");
	}

	#ifdef PACKAGE_PARSER
		if (ob->pinfo)
		{
			parse_free(ob->pinfo);
			ob->pinfo = 0;
		}
	#endif 

	if (ob->flags &O_DESTRUCTED)
	{
		return ;
	}
#ifdef _USE_SWAP_H_	
	if (ob->flags &O_SWAPPED)
	{
		load_ob_from_swap(ob);
	}
#endif	
	remove_object_from_stack(ob);

	#ifdef DEBUG
		if (d_flag > 1)
		{
			debug_message("OS debug:Deobject_t /%s (ref %d)\n", ob->name, ob->ref);
		}
	#endif 


	/* At this point, we can still back out, but this is the very last
	 * minute we can do so.  Make sure we have a new object to replace
	 * us if this is a vital object.
	 */
	if (ob == master_ob || ob == simul_efun_ob)
	{
		object_t *new_ob;
		char *tmp = ob->name;

		(++sp)->type = T_ERROR_HANDLER;
		sp->u.error_handler = fix_object_names;
		saved_master_name = master_ob->name;
		saved_simul_name = simul_efun_ob->name;

		/* hack to make sure we don't find ourselves at several points
		in the following process */
		ob->name = "";

		/* handle these two carefully, since they are rather vital */
		new_ob = load_object(tmp, compiled_version);
		if (!new_ob)
		{
			ob->name = tmp;
			sp--;
			error("OS error: Destruct on vital object failed: new copy failed to reload.");
		}

		free_object(ob, "vital object reference");
		if (ob == master_ob)
		{
			set_master(new_ob);
		}
		if (ob == simul_efun_ob)
		{
			set_simul_efun(new_ob);
		}

		/* Set the name back so we can remove it from the hash table.
		Also be careful not to remove the new object, which has
		the same name. */
		sp--; /* error handler */
		ob->name = tmp;
		tmp = new_ob->name;
		new_ob->name = "";
		remove_object_hash(ob);
		new_ob->name = tmp;
	}
	else
	{
		remove_object_hash(ob);
	}

	/*
	 * Now remove us out of the list of all objects. This must be done last,
	 * because an error in the above code would halt execution.
	 */
	removed = 0;
	for (pp = &obj_list;  *pp; pp = &(*pp)->next_all)
	{
		if (*pp != ob)
		{
			continue;
		}
		*pp = (*pp)->next_all;
		removed = 1;
		break;
	}
	DEBUG_CHECK(!removed, "Failed to delete object.\n");


	ob->next_all = obj_list_destruct;
	obj_list_destruct = ob;

	set_heart_beat(ob, 0);
	ob->flags |= O_DESTRUCTED;
}

/*
 * This one is called when no program is executing from the main loop.
 */
void destruct2(object_t *ob)
{
	int data_size(object_t *ob);
	// debug_message("OS debug:Destruct-2 object /%s (ref %d, size %d)\n", ob->name, ob->ref, data_size(ob));
	#ifdef DEBUG
		if (d_flag > 1)
		{
			debug_message("OS debug:Destruct-2 object /%s (ref %d)\n", ob->name, ob->ref);
		}
	#endif 
	/*
	 * We must deallocate variables here, not in 'free_object()'. That is
	 * because one of the local variables may point to this object, and
	 * deallocation of this pointer will also decrease the reference count of
	 * this object. Otherwise, an object with a variable pointing to itself,
	 * would never be freed. Just in case the program in this object would
	 * continue to execute, change string and object variables into the
	 * number 0.
	 */
	if (ob->prog->num_variables_total > 0)
	{
		/*
		 * Deallocate variables in this object. The space of the variables
		 * are not deallocated until the object structure is freed in
		 * free_object().
		 */
		int i;

		for (i = 0; i < (int)ob->prog->num_variables_total; i++)
		{
#if 0
// ¼ì²â
			svalue_t *v = &ob->variables[i];
			if (v->type &T_REFED) {
				debug_message("OS debug:Destruct-2 object value /%s (index %d, ref %d)\n", ob->name, i, v->u.refed->ref);
				if (v->u.refed->ref > 1) {
					int size;
					char buf[10240 * 4];
					size = svalue_save_size(v) + 1;
					char *save_variable_2(svalue_t *var, char *ret);
					save_variable_2(v, buf); 
					debug_message("OS debug:Destruct-2 object value /%s: %s", ob->name, buf);
				}
			}
#endif
			free_svalue(&ob->variables[i], "destruct2");
			ob->variables[i] = const0u;
		}
	}
	free_object(ob, "destruct_object");
}



/* Find an object. If not loaded, load it !
 * The object may selfdestruct, which is the only case when 0 will be
 * returned.
 */

object_t *find_object(char *str)
{
	object_t *ob;
	char tmpbuf[MAX_OBJECT_NAME_SIZE];


	if (!strip_name(str, tmpbuf, sizeof tmpbuf) )
	{
		printf("strip_name faint.\n");
		return 0;
	}

	if ( (ob = lookup_object_hash(tmpbuf) ) )
	{

#ifdef _USE_SWAP_H_	
		if (ob->flags &O_SWAPPED)
		{
			load_ob_from_swap(ob);
		}
#endif		
		return ob;
	}

	ob = load_object(tmpbuf, 0);
	if (!ob || (ob->flags &O_DESTRUCTED) )
	{
		printf("%s O_DESTRUCTED 2 faint.\n", tmpbuf);
		return 0;
	}
	
#ifdef _USE_SWAP_H_	
	if (ob && ob->flags &O_SWAPPED)
	{
		load_ob_from_swap(ob);
	}
#endif	
	return ob;
}

/* Look for a loaded object. Return 0 if non found. */
object_t *find_object2(char *str)
{
	register object_t *ob;
	char p[MAX_OBJECT_NAME_SIZE];

	if (!strip_name(str, p, sizeof p) )
	{
		return 0;
	}

	if ( (ob = lookup_object_hash(p) ) )
	{
		
#ifdef _USE_SWAP_H_	
		if (ob->flags &O_SWAPPED)
		{
			load_ob_from_swap(ob);
		}
#endif		
		return ob;
	}
	return 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

static sentence_t *sent_free = 0;
int tot_alloc_sentence;

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef DEBUGMALLOC_EXTENSIONS
	void mark_free_sentences()
	{
		sentence_t *sent = sent_free;

		#ifdef PRIVS
			if (privs_file_fname)
			{
				EXTRA_REF(BLOCK(privs_file_fname) )++;
			}
		#endif 

		while (sent)
		{
			DO_MARK(sent, TAG_SENTENCE);
			/* Freed sentences should have been freed.  right?
			if (sent->function)
			EXTRA_REF(BLOCK(sent->function))++;
			if (sent->verb)
			EXTRA_REF(BLOCK(sent->verb))++;
			 */
			sent = sent->next;
		}
	}
#endif 

void free_sentence(sentence_t *p)
{
	if (p->flags &V_FUNCTION)
	{
		if (p->function.f)
		{
			free_funp(p->function.f);
		}
		else
		{
			p->function.f = 0;
		}
	}
	else
	{
		if (p->function.s)
		{
			free_string(p->function.s);
		}
		else
		{
			p->function.s = 0;
		}
	}
	p->next = sent_free;
	sent_free = p;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void fatal(char *fmt, ...)
{
	static int in_fatal = 0;
	char msg_buf[2049];
	va_list args;
	V_DCL(char *fmt);

	if (in_fatal)
	{
		debug_message("OS debug:Fatal error while shutting down.  Aborting.\n");
	}
	else
	{
		in_fatal = 1;
		V_START(args, fmt);
		V_VAR(char *, fmt, args);
		vsprintf(msg_buf, fmt, args);
		va_end(args);
		debug_message("OS debug:******** FATAL ERROR: %s\nQTZ_ServerEngine driver attempting to exit gracefully.\n", msg_buf);
		if (current_file)
		{
			debug_message("OS debug:(occured during compilation of %s at line %d)\n", current_file, current_line);
		}
		if (current_object)
		{
			debug_message("OS debug:(current object was /%s)\n", current_object->name);
		}

		dump_trace(1);

		copy_and_push_string(msg_buf);
		if (command_giver)
		{
			push_object(command_giver);
		}
		else
		{
			push_undefined();
		}
		if (current_object)
		{
			push_object(current_object);
		}
		else
		{
			push_undefined();
		}
		apply_master_ob(APPLY_CRASH, 3);
		debug_message("OS debug:crash() in master called successfully.  Aborting.\n");
	}
	/* Make sure we don't trap our abort() */
	#ifdef SIGABRT
		signal(SIGABRT, SIG_DFL);
	#endif 
	#ifdef SIGILL
		signal(SIGILL, SIG_DFL);
	#endif 
	#ifdef SIGIOT
		signal(SIGIOT, SIG_DFL);
	#endif 

	#if !defined(DEBUG_NON_FATAL) || !defined(DEBUG)
		abort();
	#endif 
	in_fatal = 0;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

static int num_error = 0;

#ifdef QTZ_ServerEngineLIB_ERROR_HANDLER
	static int num_QTZ_ServerEnginelib_error = 0;
#endif 

/*
 * Error() has been "fixed" so that users can catch and throw them.
 * To catch them nicely, we really have to provide decent error information.
 * Hence, all errors that are to be caught
 * (error_recovery_context_exists == 2) construct a string containing
 * the error message, which is returned as the
 * thrown value.  Users can throw their own error values however they choose.
 */

/*
 * This is here because throw constructs its own return value; we dont
 * want to replace it with the system's error string.
 */

void throw_error()
{
	if ( ( (current_error_context->save_csp + 1)->framekind &FRAME_MASK) == FRAME_CATCH)
	{
		LONGJMP(current_error_context->context, 1);
		fatal("OS error: Throw_error failed!");
	}
	error("OS error: Throw with no catch.\n");
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

static void debug_message_with_location(char *err)
{
	if (current_object && current_prog)
	{
		debug_message("OS debug:%sprogram: /%s, object: /%s, file: %s\n", err, current_prog->name, current_object->name, get_line_number(pc, current_prog) );
	}
	else if (current_object)
	{
		debug_message("OS debug:%sprogram: (none), object: /%s, file: (none)\n", err, current_object->name);
	}
	else
	{
		debug_message("OS debug:%sprogram: (none), object: (none), file: (none)\n", err);
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


#ifdef QTZ_ServerEngineLIB_ERROR_HANDLER
	catch )
	{
		mapping_t *m;
		char *file;
		int line;
		svalue_t *mret;

		m = allocate_mapping(6);
		add_mapping_string(m, "error", err);
		if (current_prog)
		{
			add_mapping_string(m, "program", current_prog->name);
		}
		if (current_object)
		{
			add_mapping_object(m, "object", current_object);
		}
		add_mapping_array(m, "trace", get_svalue_trace() );
		get_line_number_info(&file, &line);
		add_mapping_string(m, "file", file);
		add_mapping_pair(m, "line", line);

		push_refed_mapping(m);
		catch )
		{
			*(++sp) = const1;
			mret = apply_master_ob(APPLY_ERROR_HANDLER, 2);
		}
		else
		{
			mret = apply_master_ob(APPLY_ERROR_HANDLER, 1);
		}
		if ( (mret == (svalue_t*) - 1) || !mret)
		{
			debug_message("OS debug:No error handler for error: ");
			debug_message_with_location(err);
			dump_trace(0);
		}
		else if (mret->type == T_STRING)
		{
			debug_message("%s", mret->u.string);
		}
	}
#endif 

void error_handler(char *err)
{
	#ifndef QTZ_ServerEngineLIB_ERROR_HANDLER
		char *object_name;
	#endif 

	/* in case we're going to jump out of load_object */
	restrict_destruct = 0;
	num_objects_this_thread = 0; /* reset the count */
	if (!current_error_context)
	{
		debug_message("OS debug:current_error_context is null\n");
		if (current_object)
		{
			debug_message("current_object:%s\n", current_object->name);
		}
		shutdownOS(163);
		return ;
	}
	if ( ( (current_error_context->save_csp + 1)->framekind &FRAME_MASK) == FRAME_CATCH)
	{
		/* user catches this error */
		#ifdef LOG_CATCHES
			/* This is added so that catches generate messages in the log file. */
			#ifdef QTZ_ServerEngineLIB_ERROR_HANDLER
				if (num_QTZ_ServerEnginelib_error)
				{
					debug_message("OS debug:Error in error handler: ");
				#endif 
				debug_message_with_location(err);
				(void)dump_trace(0);
				#ifdef QTZ_ServerEngineLIB_ERROR_HANDLER
					num_QTZ_ServerEnginelib_error = 0;
				}
				else
				{
					num_QTZ_ServerEnginelib_error++;
					QTZ_ServerEnginelib_error_handler(err, 1);
					num_QTZ_ServerEnginelib_error--;
				}
			#endif 
		#endif 
		free_svalue(&catch_value, "caught error");
		catch_value.type = T_STRING;
		catch_value.subtype = STRING_MALLOC;
		catch_value.u.string = string_copy(err, "caught error");
		LONGJMP(current_error_context->context, 1);
		fatal("Catch() longjump failed");
	}
	too_deep_error = max_eval_error = 0;
	if (num_error > 0)
	{
		/* This can happen via errors in the object_name() apply. */
		debug_message("OS debug:Error '%s' while trying to print error trace -- trace suppressed.\n", err);
		if (current_error_context)
		{
			LONGJMP(current_error_context->context, 1);
		}
		fatal("OS error: LONGJMP failed or no error context for error.\n");
	}
	num_error++;
	#ifdef QTZ_ServerEngineLIB_ERROR_HANDLER
		if (num_QTZ_ServerEnginelib_error)
		{
			debug_message("Error in error handler: ");
			debug_message_with_location(err);
			(void)dump_trace(0);
			num_QTZ_ServerEnginelib_error = 0;
		}
		else
		{
			num_QTZ_ServerEnginelib_error++;
			num_error--;
			QTZ_ServerEnginelib_error_handler(err, 0);
			num_QTZ_ServerEnginelib_error--;
			num_error++;
		}
		if (current_heart_beat)
		{
			set_heart_beat(current_heart_beat, 0);
			debug_message("OS debug:Heart beat in %s turned off.\n", current_heart_beat->name);

			current_heart_beat = 0;
		}
	#else 
		debug_message_with_location(err + 1);
		#if defined(DEBUG) && defined(TRACE_CODE)
			object_name = dump_trace(1);
		#else 
			object_name = dump_trace(0);
		#endif 
		if (object_name)
		{
			object_t *ob;

			ob = find_object2(object_name);
			if (!ob)
			{
				debug_message("meet error when OS executing program in destroyed object /%s\n", object_name);
			}
		}
		if (current_heart_beat)
		{
			set_heart_beat(current_heart_beat, 0);
			debug_message("OS debug:Heart beat in /%s turned off.\n", current_heart_beat->name);

			current_heart_beat = 0;
		}
	#endif 
	num_error--;
	if (current_error_context)
	{
		LONGJMP(current_error_context->context, 1);
	}
	fatal("LONGJMP failed or no error context for error.\n");
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void error_needs_free(char *s)
{
	char err_buf[2048];
	strncpy(err_buf + 1, s, 2047);
	err_buf[0] = '*'; /* all system errors get a * at the start */
	err_buf[2047] = '\0';
	FREE_MSTR(s);

	error_handler(err_buf);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void error(char *fmt, ...)
{
	char err_buf[20480];
	va_list args;
	V_DCL(char *fmt);

	V_START(args, fmt);
	V_VAR(char *, fmt, args);
	vsprintf(err_buf + 1, fmt, args);
	va_end(args);
	err_buf[0] = '*'; /* all system errors get a * at the start */

	error_handler(err_buf);
}

/*
 * This one is called from HUP.
 */
int OS_is_being_shut_down;

#ifdef SIGNAL_FUNC_TAKES_INT
	void startshutdownOS(int sig)
#else 
	void startshutdownOS()
#endif 
{
	#ifdef SIGNAL_FUNC_TAKES_INT
		(void)sig;
	#endif 
	OS_is_being_shut_down = 1;
}


void pre_shutdown()
{
	//call_script_func(char* objectName, char* funcName, char* paramFmt, ... )
}

/*
 * This one is called from the command "shutdown".
 * We don't call it directly from HUP, because it is dangerous when being
 * in an interrupt.
 */

int ShutDownTryTimes = 0;
void shutdownOS(int exit_code)
{
	if (ShutDownTryTimes > 0)
	{
		exit(exit_code);
	}
	ShutDownTryTimes++;

	pre_shutdown();

	#ifdef LATTICE
		signal(SIGUSR1, SIG_IGN);
		signal(SIGTERM, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
		signal(SIGALRM, SIG_IGN);
	#endif 
	unlink_swap_file();
	#ifdef PROFILING
		monitor(0, 0, 0, 0, 0); /* cause gmon.out to be written */
	#endif 

	exit(exit_code);
}

/*
 * Call this one when there is only little memory left. It will start
 * Armageddon.
 */
void slow_shut_down(int minutes)
{
	/*
	 * Swap out objects, and free some memory.
	 */
	svalue_t *amo;

	push_number(minutes);
	amo = apply_master_ob(APPLY_SLOW_SHUTDOWN, 1);
	/* in this case, approved means the QTZ_ServerEnginelib will handle it */
	if (!MASTER_APPROVED(amo) )
	{
		object_t *save_current = current_object,  *save_command = command_giver;

		command_giver = 0;
		current_object = 0;
		command_giver = save_command;
		current_object = save_current;
		#ifdef SIGNAL_FUNC_TAKES_INT
			startshutdownOS(1);
		#else 
			startshutdownOS();
		#endif 
		return ;
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void do_message(svalue_t *class , svalue_t *msg, array_t *scope, array_t *exclude, int recurse)
{
	/*
		int i, j, valid;
		object_t *ob;

		for (i = 0; i < scope->size; i++)
		{
				switch (scope->item[i].type)
				{
						case T_STRING:
							ob = find_object(scope->item[i].u.string);
							if (!ob || !object_visible(ob) )
							{
								continue;
							}
							break;
						case T_OBJECT:
							ob = scope->item[i].u.ob;
							break;
						default:
							continue;
				}
				if (ob->flags &O_LISTENER || ob->interactive)
				{
						for (valid = 1, j = 0; j < exclude->size; j++)
						{
								if (exclude->item[j].type != T_OBJECT)
								{
									continue;
								}
								if (exclude->item[j].u.ob == ob)
								{
										valid = 0;
										break;
								}
						}
						if (valid)
						{
								push_svalue(class );
								push_svalue(msg);
								apply(APPLY_RECEIVE_MESSAGE, ob, 2, ORIGIN_DRIVER);
						}
				}
		}
		*/
}
