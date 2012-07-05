/*
math.c: this file contains the math efunctions called from
inside eval_instruction() in interpret.c.
-- coded by Truilkan 93/02/21
 */

#include <math.h>
#ifdef LATTICE
	#include "/lpc_incl.h"
#else 
	#include "include/lpc_incl.h"
#endif 

#ifdef F_COS
	void f_cos(void)
	{
		sp->u.real = cos(sp->u.real);
	}
#endif 

#ifdef F_SIN
	void f_sin(void)
	{
		sp->u.real = sin(sp->u.real);
	}
#endif 
#ifdef F_TAN
	void f_tan(void)
	{
		/*
		 * maybe should try to check that tan won't blow up (x != (Pi/2 + N*Pi))
		 */
		sp->u.real = tan(sp->u.real);
	}
#endif 

#ifdef F_ASIN
	void f_asin(void)
	{
		if (sp->u.real <  - 1.0)
		{
			error("math: asin(x) with (x < -1.0)\n");
			return ;
		}
		else if (sp->u.real > 1.0)
		{
			error("math: asin(x) with (x > 1.0)\n");
			return ;
		}
		sp->u.real = asin(sp->u.real);
	}
#endif 

#ifdef F_ACOS
	void f_acos(void)
	{
		if (sp->u.real <  - 1.0)
		{
			error("math: acos(x) with (x < -1.0)\n");
			return ;
		}
		else if (sp->u.real > 1.0)
		{
			error("math: acos(x) with (x > 1.0)\n");
			return ;
		}
		sp->u.real = acos(sp->u.real);
	}
#endif 

#ifdef F_ATAN
	void f_atan(void)
	{
		sp->u.real = atan(sp->u.real);
	}
#endif 

#ifdef F_SQRT
	void f_sqrt(void)
	{
		if (sp->u.real < 0.0)
		{
			error("math: sqrt(x) with (x < 0.0)\n");
			return ;
		}
		sp->u.real = sqrt(sp->u.real);
	}
#endif 

#ifdef F_LOG
	void f_log(void)
	{
		if (sp->u.real <= 0.0)
		{
			error("math: log(x) with (x <= 0.0)\n");
			return ;
		}
		sp->u.real = log(sp->u.real);
	}
#endif 

#ifdef F_POW
	void f_pow(void)
	{
		(sp - 1)->u.real = pow( (sp - 1)->u.real, sp->u.real);
		sp--;
	}
#endif 

#ifdef F_EXP
	void f_exp(void)
	{
		sp->u.real = exp(sp->u.real);
	}
#endif 

#ifdef F_FLOOR
	void f_floor(void)
	{
		sp->u.real = floor(sp->u.real);
	}
#endif 

#ifdef F_CEIL
	void f_ceil(void)
	{
		sp->u.real = ceil(sp->u.real);
	}
#endif 


#ifdef F_MAX

	void f_max()
	{
		svalue_t *sarr = sp - st_num_arg + 1;
		array_t *arr = sarr->u.arr;
		int max_index = 0;
		int i;

		if (!arr->size)
		{
			error("Can't find max of an empty array.\n");
		}

		if (arr->item->type != T_NUMBER && arr->item->type != T_REAL && arr->item->type != T_STRING)
		{
			error("Array must consist of ints, floats or strings.\n");
		}

		for (i = 1; i < arr->size; i++)
		{
			// Check the type of this element.
			switch (arr->item[i].type)
			{
				case T_NUMBER:
					switch (arr->item[max_index].type)
					{
					case T_NUMBER:
						if (arr->item[i].u.number > arr->item[max_index].u.number)
						{
							max_index = i;
						}
						break;
					case T_REAL:
						if (arr->item[i].u.number > arr->item[max_index].u.real)
						{
							max_index = i;
						}
						break;
					default:
						error("Inhomogeneous array.\n");
					}
					break;
				case T_REAL:
					switch (arr->item[max_index].type)
					{
					case T_NUMBER:
						if (arr->item[i].u.real > arr->item[max_index].u.number)
						{
							max_index = i;
						}
						break;
					case T_REAL:
						if (arr->item[i].u.real > arr->item[max_index].u.real)
						{
							max_index = i;
						}
						break;
					default:
						error("Inhomogeneous array.\n");
					}
					break;
				case T_STRING:
					if (arr->item[max_index].type != T_STRING)
					{
						error("Inhomogeneous array.\n");
					}
					if (strcmp(arr->item[i].u.string, arr->item[max_index].u.string) > 0)
					{
						max_index = i;
					}
					break;
				default:
					error("Array must consist of ints, floats or strings.\n");
			}
		}

		if (st_num_arg == 2)
		{
			if (sp->u.number != 0)
			{
				pop_n_elems(2);
				push_number(max_index);
				return ;
			}

			pop_stack();
		}

		assign_svalue_no_free(sp, &arr->item[max_index]);
		free_array(arr);
	}

#endif 

#ifdef F_MIN

	void f_min()
	{
		svalue_t *sarr = sp - st_num_arg + 1;
		array_t *arr = sarr->u.arr;
		int min_index = 0;
		int i;

		if (!arr->size)
		{
			error("Can't find min of an empty array.\n");
		}

		if (arr->item->type != T_NUMBER && arr->item->type != T_REAL && arr->item->type != T_STRING)
		{
			error("Array must consist of ints, floats or strings.\n");
		}

		for (i = 1; i < arr->size; i++)
		{
			// Check the type of this element.
			switch (arr->item[i].type)
			{
				case T_NUMBER:
					switch (arr->item[min_index].type)
					{
					case T_NUMBER:
						if (arr->item[i].u.number < arr->item[min_index].u.number)
						{
							min_index = i;
						}
						break;
					case T_REAL:
						if (arr->item[i].u.number < arr->item[min_index].u.real)
						{
							min_index = i;
						}
						break;
					default:
						error("Inhomogeneous array.\n");
					}
					break;
				case T_REAL:
					switch (arr->item[min_index].type)
					{
					case T_NUMBER:
						if (arr->item[i].u.real < arr->item[min_index].u.number)
						{
							min_index = i;
						}
						break;
					case T_REAL:
						if (arr->item[i].u.real < arr->item[min_index].u.real)
						{
							min_index = i;
						}
						break;
					default:
						error("Inhomogeneous array.\n");
					}
					break;
				case T_STRING:
					if (arr->item[min_index].type != T_STRING)
					{
						error("Inhomogeneous array.\n");
					}
					if (strcmp(arr->item[i].u.string, arr->item[min_index].u.string) < 0)
					{
						min_index = i;
					}
					break;
				default:
					error("Array must consist of ints, floats or strings.\n");
			}
		}

		if (st_num_arg == 2)
		{
			if (sp->u.number != 0)
			{
				pop_n_elems(2);
				push_number(min_index);
				return ;
			}

			pop_stack();
		}

		assign_svalue_no_free(sp, &arr->item[min_index]);
		free_array(arr);
	}
#endif
