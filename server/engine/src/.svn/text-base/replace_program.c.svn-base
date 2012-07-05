#include "std.h"
#include "lpc_incl.h"
#include "replace_program.h"
#include "simul_efun.h"
#include "swap.h"

/*
 * replace_program.c
 * replaces the program in a running object with one of the programs
 * it inherits, in order to save memory.
 * Ported from Amylaars LP 3.2 driver
 */

replace_ob_t *obj_list_replace = 0;

static program_t *search_inherited(char *, program_t *, int*);
static replace_ob_t *retrieve_replace_program_entry(void);

void replace_programs()
{
	replace_ob_t *r_ob,  *r_next;
	int i, num_fewer, offset;
	svalue_t *svp;

	#ifdef DEBUG
		if (d_flag)
		{
			debug_message("start of replace_programs\n");
		}
	#endif 
	for (r_ob = obj_list_replace; r_ob; r_ob = r_next)
	{
		program_t *old_prog;

#ifdef _USE_SWAP_H_
		if (r_ob->ob->flags &O_SWAPPED)
		{
			load_ob_from_swap(r_ob->ob);
		}
#endif		
		num_fewer = r_ob->ob->prog->num_variables_total - r_ob->new_prog->num_variables_total;
		#ifdef DEBUG
			if (d_flag)
			{
				debug_message("%d less variables\n", num_fewer);
			}
		#endif 
		tot_alloc_object_size -= num_fewer * sizeof(svalue_t[1]);
		if ( (offset = r_ob->var_offset) )
		{
			svp = r_ob->ob->variables;
			/* move our variables up to the top */
			for (i = 0; i < r_ob->new_prog->num_variables_total; i++)
			{
				free_svalue(svp, "replace_programs");
				*svp = *(svp + offset);
				*(svp + offset) = const0u;
				svp++;
			}
			/* free the rest */
			for (i = 0; i < num_fewer; i++)
			{
				free_svalue(svp, "replace_programs");
				*svp++ = const0u;
			}
		}
		else
		{
			/* We just need to remove the last num_fewer variables */
			svp = &r_ob->ob->variables[r_ob->new_prog->num_variables_total];
			for (i = 0; i < num_fewer; i++)
			{
				free_svalue(svp, "replace_programs");
				*svp++ = const0u;
			}
		}

		r_ob->new_prog->ref++;
		old_prog = r_ob->ob->prog;
		r_ob->ob->prog = r_ob->new_prog;
		r_next = r_ob->next;
		free_prog(old_prog, 1);
		#ifdef DEBUG
			if (d_flag)
			{
				debug_message("program freed.\n");
			}
		#endif 
		FREE( (char*)r_ob);
	}
	obj_list_replace = (replace_ob_t*)0;
	#ifdef DEBUG
		if (d_flag)
		{
			debug_message("OS debug: end of replace_programs\n");
		}
	#endif 
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef F_REPLACE_PROGRAM
	static program_t *search_inherited(char *str, program_t *prg, int *offpnt)
	{
		program_t *tmp;
		int i;

		#ifdef DEBUG
			if (d_flag)
			{
				debug_message("OS debug: search_inherited started\n");
				debug_message("OS debug: searching for PRG(/%s) in PRG(/%s)\n", str, prg->name);
				debug_message("OS debug:num_inherited=%d\n", prg->num_inherited);
			}
		#endif 
		for (i = 0; i < (int)prg->num_inherited; i++)
		{
			#ifdef DEBUG
				if (d_flag)
				{
					debug_message("OS debug: index %d:\n", i);
					debug_message("OS debug: checking PRG(/%s)\n", prg->inherit[i].prog->name);
				}
			#endif 
			if (strcmp(str, prg->inherit[i].prog->name) == 0)
			{
				#ifdef DEBUG
					if (d_flag)
					{
						debug_message("OS debug: match found\n");
					}
				#endif 
				*offpnt = prg->inherit[i].variable_index_offset;
				return prg->inherit[i].prog;
			}
			else if ( (tmp = search_inherited(str, prg->inherit[i].prog, offpnt) ) )
			{
				#ifdef DEBUG
					if (d_flag)
					{
						debug_message("OS debug: deferred match found\n");
					}
				#endif 
				*offpnt += prg->inherit[i].variable_index_offset;
				return tmp;
			}
		}
		#ifdef DEBUG
			if (d_flag)
			{
				debug_message("OS debug: search_inherited failed\n");
			}
		#endif 
		return (program_t*)0;
	}

	static replace_ob_t *retrieve_replace_program_entry()
	{
		replace_ob_t *r_ob;

		for (r_ob = obj_list_replace; r_ob; r_ob = r_ob->next)
		{
			if (r_ob->ob == current_object)
			{
				return r_ob;
			}
		}
		return 0;
	}

	void f_replace_program(int num_arg, int instruction)
	{
		replace_ob_t *tmp;
		int name_len;
		char *name,  *xname;
		program_t *new_prog;
		int var_offset;

		(void)num_arg;
		if (sp->type != T_STRING)
		{
			bad_arg(1, instruction);
		}
		#ifdef DEBUG
			if (d_flag)
			{
				debug_message("OS debug: replace_program called\n");
			}
		#endif 
		if (!current_object)
		{
			error("OS error: replace_program called with no current object\n");
		}
		if (current_object == simul_efun_ob)
		{
			error("OS error: replace_program on simul_efun object\n");
		}

		if (current_object->prog->func_ref)
		{
			error("OS error: cannot replace a program with function references.\n");
		}

		name_len = strlen(sp->u.string);
		name = (char*)DMALLOC(name_len + 3, TAG_TEMPORARY, "replace_program");
		xname = name;
		strcpy(name, sp->u.string);
		if (name[name_len - 2] != '.' || name[name_len - 1] != 'c')
		{
			strcat(name, ".c");
		}
		if (*name == '/')
		{
			name++;
		}
		new_prog = search_inherited(name, current_object->prog, &var_offset);
		FREE(xname);
		if (!new_prog)
		{
			error("OS error: program to replace the current with has to be inherited\n");
		}
		if (!(tmp = retrieve_replace_program_entry() ) )
		{
			tmp = ALLOCATE(replace_ob_t, TAG_TEMPORARY, "replace_program");
			tmp->ob = current_object;
			tmp->next = obj_list_replace;
			obj_list_replace = tmp;
		}
		tmp->new_prog = new_prog;
		tmp->var_offset = var_offset;
		#ifdef DEBUG
			if (d_flag)
			{
				debug_message("OS debug: replace_program finished\n");
			}
		#endif 
		free_string_svalue(sp--);
	}

#endif
