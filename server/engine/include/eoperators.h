/* this file was manually generated -- don't delete it */

INLINE void f_ge(void);
INLINE void f_le(void);
INLINE void f_lt(void);
INLINE void f_gt(void);
INLINE void f_and(void);
INLINE void f_and_eq(void);
INLINE void f_div_eq(void);
INLINE void f_eq(void);
INLINE void f_lsh(void);
INLINE void f_lsh_eq(void);
INLINE void f_mod_eq(void);
INLINE void f_mult_eq(void);
INLINE void f_ne(void);
INLINE void f_or(void);
INLINE void f_or_eq(void);
INLINE void f_parse_command(void);
INLINE void f_range(int);
INLINE void f_extract_range(int);
INLINE void f_rsh(void);
INLINE void f_rsh_eq(void);
INLINE void f_simul_efun(void);
INLINE void f_sub_eq(void);
INLINE void f_switch(void);
INLINE void f_xor(void);
INLINE void f_xor_eq(void);
INLINE void f_function_constructor(void);
INLINE void f_evaluate(void);
INLINE void f_sscanf(void);

/*
 * eoperators.c
 */
INLINE funptr_t *make_funp(svalue_t *, svalue_t*);
INLINE void push_funp(funptr_t*);
INLINE void free_funp(funptr_t*);
int merge_arg_lists(int, array_t *, int);
void call_simul_efun(unsigned short, int);

INLINE funptr_t *make_efun_funp(int, svalue_t*);
INLINE funptr_t *make_lfun_funp(int, svalue_t*);
INLINE funptr_t *make_simul_funp(int, svalue_t*);
