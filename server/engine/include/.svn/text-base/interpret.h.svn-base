#ifndef INTERPRET_H
	#define INTERPRET_H

	#define PUSH_STRING    (0 << 6)
	#define PUSH_NUMBER    (1 << 6)
	#define PUSH_GLOBAL    (2 << 6)
	#define PUSH_LOCAL     (3 << 6)

	#define PUSH_WHAT      (3 << 6)
	#define PUSH_MASK      (0xff ^ (PUSH_WHAT))

	#define SWITCH_CASE_SIZE ((int)(2 + sizeof(char *)))

	#define EXTRACT_UCHAR(p) (*(unsigned char *)(p))

	/*
	 * Control stack element.
	 * 'prog' is usually same as 'ob->prog' (current_object), except when
	 * when the current function is defined by inheritance.
	 * The pointer, csp, will point to the values that will be used at return.
	 */
	#define FRAME_FUNCTION     0
	#define FRAME_FUNP         1
	#define FRAME_CATCH        2
	#define FRAME_FAKE         3
	#define FRAME_MASK         3

	#define FRAME_OB_CHANGE    4
	#define FRAME_EXTERNAL     8

	typedef struct
	{
		#ifdef PROFILE_FUNCTIONS
			unsigned long entry_secs, entry_usecs;
		#endif 
		short framekind;
		union
		{
			int table_index;
			funptr_t *funp;
		} fr;
		object_t *ob; /* Current object */
		object_t *prev_ob; /* Save previous object */
		program_t *prog; /* Current program */
		int num_local_variables; /* Local + arguments */
		char *pc;
		svalue_t *fp;
		int function_index_offset; /* Used when executing functions in inherited * programs */
		int variable_index_offset; /* Same */
		short caller_type; /* was this a locally called function? */
	}

	control_stack_t;

	typedef struct
	{
		object_t *ob;
		union
		{
			funptr_t *fp;
			char *str;
		} f;
		int narg;
		svalue_t *args;
	}

	function_to_call_t;

	typedef struct error_context_s
	{
		jmp_buf context;
		control_stack_t *save_csp;
		object_t *save_command_giver;
		svalue_t *save_sp;
		struct error_context_s *save_context;
	} error_context_t;

	/* for apply_master_ob */
	#define MASTER_APPROVED(x) (((x)==(svalue_t *)-1) || ((x) && (((x)->type != T_NUMBER) || (x)->u.number))) 

	#define IS_ZERO(x) (!(x) || (((x)->type == T_NUMBER) && ((x)->u.number == 0)))
	#define IS_UNDEFINED(x) (!(x) || (((x)->type == T_NUMBER) && \
	((x)->subtype == T_UNDEFINED) && ((x)->u.number == 0)))

	#define CHECK_TYPES(val, t, arg, inst) \
	if (!((val)->type & (t))) bad_argument(val, t, arg, inst);

	/* Beek - add some sanity to joining strings */
	/* add to an svalue */
	#define EXTEND_SVALUE_STRING(x, y, z) \
	SAFE( char *ess_res; \
	int ess_len; \
	int ess_r; \
	ess_len = (ess_r = SVALUE_STRLEN(x)) + strlen(y); \
	if ((x)->subtype == STRING_MALLOC && MSTR_REF((x)->u.string) == 1) { \
	ess_res = (char *) extend_string((x)->u.string, ess_len); \
	if (!ess_res) fatal("QTZ_ServerEngine:Out of memory!\n"); \
	strcpy(ess_res + ess_r, (y)); \
	} else { \
	ess_res = new_string(ess_len, z); \
	strcpy(ess_res, (x)->u.string); \
	strcpy(ess_res + ess_r, (y)); \
	free_string_svalue(x); \
	(x)->subtype = STRING_MALLOC; \
	} \
	(x)->u.string = ess_res;  )

	/* <something that needs no free> + string svalue */
	#define SVALUE_STRING_ADD_LEFT(y, z) \
	SAFE( char *pss_res; int pss_r; int pss_len; \
	pss_len = SVALUE_STRLEN(sp) + (pss_r = strlen(y)); \
	pss_res = new_string(pss_len, z); \
	strcpy(pss_res, y); \
	strcpy(pss_res + pss_r, sp->u.string); \
	free_string_svalue(sp--); \
	sp->type = T_STRING; \
	sp->u.string = pss_res; \
	sp->subtype = STRING_MALLOC; \
	)

	/* basically, string + string; faster than using extend b/c of SVALUE_STRLEN */
	#define SVALUE_STRING_JOIN(x, y, z) \
	SAFE( char *ssj_res; int ssj_r; int ssj_len; \
	ssj_r = SVALUE_STRLEN(x); \
	ssj_len = ssj_r + SVALUE_STRLEN(y); \
	if ((x)->subtype == STRING_MALLOC && MSTR_REF((x)->u.string) == 1) { \
	ssj_res = (char *) extend_string((x)->u.string, ssj_len); \
	if (!ssj_res) fatal("QTZ_ServerEngine:Out of memory!\n"); \
	(void) strcpy(ssj_res + ssj_r, (y)->u.string); \
	free_string_svalue(y); \
	} else { \
	ssj_res = (char *) new_string(ssj_len, z); \
	strcpy(ssj_res, (x)->u.string); \
	strcpy(ssj_res + ssj_r, (y)->u.string); \
	free_string_svalue(y); \
	free_string_svalue(x); \
	(x)->subtype = STRING_MALLOC; \
	} \
	(x)->u.string = ssj_res; \
	)

	/* macro calls */
	#define call_program(prog, offset) \
	eval_instruction(prog->program + offset)

	#ifdef DEBUG
		#define free_svalue(x,y) int_free_svalue(x,y)
	#else 
		#define free_svalue(x,y) int_free_svalue(x)
	#endif 

	#define push_svalue(x) assign_svalue_no_free(++sp, x)
	#define put_number(x) SAFE( \
	sp->type = T_NUMBER;\
	sp->subtype = 0;\
	sp->u.number = (x);\
	)
	#define set_svalue_number(s, x) SAFE( \
	(s)->type = T_NUMBER;\
	(s)->subtype = 0;\
	(s)->u.number = (x);\
	)
	#define set_svalue_real(s, x) SAFE( \
	(s)->type = T_REAL;\
	(s)->u.real = (x);\
	)

	#define set_svalue_object(s, x) SAFE( \
	(s)->type = T_OBJECT;\
	(s)->u.ob = (x);\
	)
	#define set_svalue_function(s, x) SAFE( \
	(s)->type = T_FUNCTION;\
	(s)->u.fp = (x);\
	)

	#define put_buffer(x) SAFE( \
	sp->type = T_BUFFER;\
	sp->u.buf = (x);\
	)
	#define set_svalue_buffer(s, x) SAFE( \
	(s)->type = T_BUFFER;\
	(s)->u.buf = (x);\
	)
	#define put_undested_object(x) SAFE(\
	sp->type = T_OBJECT;\
	sp->u.ob = (x);\
	)
	#define put_object(x) SAFE(\
	if ((x)->flags & O_DESTRUCTED) put_number(0); \
	else put_undested_object(x);\
	)
	#define put_unrefed_undested_object(x, y) SAFE(\
	sp->type = T_OBJECT;\
	sp->u.ob = (x);\
	add_ref((x), y);\
	)
	#define put_unrefed_object(x,y) SAFE(\
	if ((x)->flags & O_DESTRUCTED)\
	put_number(0);\
	else put_unrefed_undested_object(x,y);\
	)
	/* see comments on push_constant_string */
	#define put_constant_string(x) SAFE(\
	sp->type = T_STRING;\
	sp->subtype = STRING_SHARED;\
	sp->u.string = make_shared_string(x);\
	)
	#define put_malloced_string(x) SAFE(\
	sp->type = T_STRING;\
	sp->subtype = STRING_MALLOC;\
	sp->u.string = (x);\
	)
	#define set_svalue_malloced_string(s, x) SAFE(\
	(s)->type = T_STRING;\
	(s)->subtype = STRING_MALLOC;\
	(s)->u.string = (x);\
	)
	#define set_svalue_shared_string(s, x) SAFE(\
	(s)->type = T_STRING;\
	(s)->subtype = STRING_SHARED;\
	(s)->u.string = make_shared_string(x);\
	)
	#define put_array(x) SAFE(\
	sp->type = T_ARRAY;\
	sp->u.arr = (x);\
	)
	#define set_svalue_array(s, x) SAFE(\
	(s)->type = T_ARRAY;\
	(s)->u.arr = (x);\
	)
	#define set_svalue_mapping(s, x) SAFE(\
	(s)->type = T_MAPPING;\
	(s)->u.map = (x);\
	)
	#define set_svalue_class(s, x) SAFE(\
	(s)->type = T_CLASS;\
	(s)->u.arr = (x);\
	)
	#define put_shared_string(x) SAFE(\
	sp->type = T_STRING;\
	sp->subtype = STRING_SHARED;\
	sp->u.string = (x);\
	)

	#define set_svalue_undefined(s) SAFE( \
	*(s) = const0u;			\
	)

	#define svalue_add_ref(s) SAFE(\
	if ((s)->type == T_STRING) { \
		if ((s)->subtype &STRING_COUNTED) \
		{ \
			INC_COUNTED_REF((s)->u.string); \
			ADD_STRING(MSTR_SIZE((s)->u.string) ); \
			NDBG(BLOCK((s)->u.string)); \
		} \
	} \
	if ((s)->type &T_REFED) (s)->u.refed->ref++;\
	)	

	#define efun_arg(n) (sp - st_num_arg +(n))

	extern program_t *current_prog;
	extern short caller_type;
	extern char *pc;
	extern svalue_t *sp;
	extern svalue_t *end_of_stack;
	extern svalue_t *fp;
	extern svalue_t catch_value;
	extern control_stack_t control_stack[CFG_MAX_CALL_DEPTH];
	extern control_stack_t *csp;
	extern int too_deep_error;
	extern int max_eval_error;
	extern int function_index_offset;
	extern int variable_index_offset;
	extern unsigned int apply_low_call_others;
	extern unsigned int apply_low_cache_hits;
	extern unsigned int apply_low_slots_used;
	extern unsigned int apply_low_collisions;
	extern int function_index_offset;
	extern int simul_efun_is_loading;
	extern program_t fake_prog;
	extern svalue_t global_lvalue_byte;
	extern int num_varargs;

	void eval_instruction(char *p);
	INLINE void assign_svalue(svalue_t *, svalue_t*);
	INLINE void assign_svalue_no_free(svalue_t *, svalue_t*);
	INLINE void copy_some_svalues(svalue_t *, svalue_t *, int);
	INLINE void transfer_push_some_svalues(svalue_t *, int);
	INLINE void push_some_svalues(svalue_t *, int);
	#ifdef DEBUG
		INLINE void int_free_svalue(svalue_t *, char*);
	#else 
		INLINE void int_free_svalue(svalue_t*);
	#endif 
	INLINE void free_string_svalue(svalue_t*);
	INLINE void free_some_svalues(svalue_t *, int);
	INLINE void push_object(object_t*);
	INLINE void push_number(int);
	INLINE void push_real(double);
	INLINE void push_undefined(void);
	INLINE void push_undefineds(int n);
	INLINE void copy_and_push_string(char*);
	INLINE void share_and_push_string(char*);
	INLINE void push_array(array_t*);
	INLINE void push_refed_array(array_t*);
	INLINE void push_buffer(buffer_t*);
	INLINE void push_refed_buffer(buffer_t*);
	INLINE void push_mapping(mapping_t*);
	INLINE void push_refed_mapping(mapping_t*);
	INLINE void push_class(array_t*);
	INLINE void push_refed_class(array_t *, unsigned char);
	INLINE void push_malloced_string(char*);
	INLINE void push_shared_string(char*);
	INLINE void push_constant_string(char*);
	INLINE void pop_stack(void);
	INLINE void pop_n_elems(int);
	INLINE void pop_2_elems(void);
	INLINE void pop_3_elems(void);
	INLINE compiler_function_t *setup_inherited_frame(int);
	char *function_name(program_t *, int);
	void remove_object_from_stack(object_t*);
	void setup_fake_frame(funptr_t*);
	void remove_fake_frame(void);
	void push_indexed_lvalue(int);

	void process_efun_callback(int, function_to_call_t *, int);
	svalue_t *call_efun_callback(function_to_call_t *, int);
	char *type_name(int c);
	void bad_arg(int, int);
	void bad_argument(svalue_t *, int, int, int);
	void check_for_destr(array_t*);
	int is_static(char *, object_t*);
	int apply_low(char *, object_t *, int);
	svalue_t *apply(char *, object_t *, int, int);
	svalue_t *call_function_pointer(funptr_t *, int);
	svalue_t *safe_call_function_pointer(funptr_t *, int);
	svalue_t *safe_apply(char *, object_t *, int, int);
	void call___INIT(object_t*);
	array_t *call_all_other(array_t *, char *, int);
	char *function_exists(char *, object_t *, int);
	void call_function(program_t *, int);
	svalue_t *apply_master_ob(char *, int);
	svalue_t *safe_apply_master_ob(char *, int);
	void init_master(char*);
	void mark_apply_low_cache(void);
	void translate_absolute_line(int, unsigned short *, int *, int*);
	char *add_slash(char*);
	int strpref(char *, char*);
	array_t *get_svalue_trace(void);
	void do_trace(char *, char *, char*);
	char *dump_trace(int);
	void opcdump(char*);
	int inter_sscanf(svalue_t *, svalue_t *, svalue_t *, int);
	char *get_line_number_if_any(void);
	char *get_line_number(char *, program_t*);
	void get_line_number_info(char **, int*);
	void get_version(char*);
	void reset_machine(int);
	void unlink_string_svalue(svalue_t*);
	void copy_lvalue_range(svalue_t*);
	void assign_lvalue_range(svalue_t*);

	void pop_context(error_context_t*);
	void restore_context(error_context_t*);
	int save_context(error_context_t*);

	void pop_control_stack(void);
	INLINE compiler_function_t *setup_new_frame(int);
	INLINE void push_control_stack(int);
	program_t *find_function_by_name2(object_t *ob, char **name, int *index, int *fio, int *vio);

	void break_point(void);

	#ifdef DEBUGMALLOC_EXTENSIONS
		void mark_svalue(svalue_t*);
		void mark_stack(void);
	#endif 

#endif /* _INTERPRET_H */
