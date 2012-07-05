#ifndef OBJECT_H
	#define OBJECT_H

	/*
	 * Definition of an object.
	 * If the object is inherited, then it must not be destructed !
	 *
	 * The reset is used as follows:
	 * 0: There is an error in the reset() in this object. Never call it again.
	 * 1: Normal state.
	 * 2 or higher: This is an interactive user, that has not given any commands
	 *		for a number of reset periods.
	 */

	#define MAX_OBJECT_NAME_SIZE 2048

	#define O_HEART_BEAT		0x01	/* Does it have an heart beat ?      */

	#define O_LISTENER              0x04    /* can hear say(), etc */
	#define O_CATCH_TELL            0x04

	#define O_CLONE			0x08	/* Is it cloned from a master copy ? */
	#define O_DESTRUCTED		0x10	/* Is it destructed ?                */
	#define O_SWAPPED		0x20	/* Is it swapped to file             */
	#define O_ONCE_INTERACTIVE	0x40	/* Has it ever been interactive ?    */
	#define O_RESET_STATE		0x80	/* Object in a 'reset':ed state ?    */
	#define O_WILL_CLEAN_UP		0x100	/* clean_up will be called next time */
	#define O_VIRTUAL		0x200	/* We're a virtual object            */
	#define O_HIDDEN		0x400	/* We're hidden from nonprived objs  */
	#define O_WILL_RESET            0x1000	/* reset will be called next time    */

	/*
	 * Note: use of more than 16 bits means extending flags to an unsigned long
	 */

	typedef struct sentence_s
	{
		struct sentence_s *next;
		struct object_s *ob;
		union string_or_func function;
		int flags;
	}

	sentence_t;

	typedef struct object_s
	{
		unsigned short ref; /* Reference count. */
		unsigned short flags; /* Bits or'ed together from above */
		#ifdef DEBUG
			unsigned int extra_ref; /* Used to check ref count. */
		#endif 
		char *name;
		struct object_s *next_hash;
		/* the fields above must match lpc_object_t */
		int load_time; /* time when this object was created */
		#ifndef NO_RESET
			int next_reset; /* Time of next reset of this object */
		#endif 
		int time_of_ref; /* Time when last referenced. Used by swap */
		long swap_num; /* Swap file offset. -1 is not swapped yet. */
		program_t *prog;
		struct object_s *next_all;
		struct interactive_s *interactive; /* Data about an interactive user */
		#ifdef PRIVS
			char *privs; /* object's privledges */
		#endif /* PRIVS */
		#ifdef PACKAGE_PARSER
			struct parse_info_s *pinfo;
		#endif 
		unsigned iType;
		//    int x;
		//    int y;
		//    int X;
		//    int Y;
		svalue_t variables[1]; /* All variables to this program */
		/* The variables MUST come last in the struct */
	} object_t;

	#ifdef DEBUG
		#define add_ref(ob, str) SAFE(\
		ob->ref++; \
		if (d_flag > 1) \
		printf("Add_ref %s (%d) from %s\n", \
		ob->name, ob->ref, str);\
		)
	#else 
		#define add_ref(ob, str) ob->ref++
	#endif 

	#define ROB_STRING_ERROR 1
	#define ROB_ARRAY_ERROR 2
	#define ROB_MAPPING_ERROR 4
	#define ROB_NUMERAL_ERROR 8
	#define ROB_GENERAL_ERROR 16
	#define ROB_CLASS_ERROR 32
	#define ROB_ERROR 63

	extern object_t *hashed_living[CFG_LIVING_HASH_SIZE];
	extern object_t *previous_ob;
	extern int tot_alloc_object;
	extern int tot_alloc_object_size;
	extern int save_svalue_depth;

	void bufcat(char **, char*);
	INLINE int svalue_save_size(svalue_t*);
	INLINE void save_svalue(svalue_t *, char **);
	INLINE int restore_svalue(char *, svalue_t*);
	int save_object(object_t *, char *, int);
	char *save_variable(svalue_t*);
	int restore_object(object_t *, char *, int);
	void restore_variable(svalue_t *, char*);
	object_t *get_empty_object(int);
	void reset_object(object_t*);
	void call_create(object_t *, int);
	void reload_object(object_t*);
	void free_object(object_t *, char*);
	void safe_load_object(object_t **obj, char *file);
	object_t *find_living_object(char *, int);
	INLINE int valid_hide(object_t*);
	INLINE int object_visible(object_t*);
	void set_living_name(object_t *, char*);
	void remove_living_name(object_t*);
	void stat_living_objects(outbuffer_t*);
	void tell_npc(object_t *, char*);
	int find_global_variable(program_t *, char *, unsigned short*);
	void dealloc_object(object_t *, char*);




	unsigned int set_object_type(object_t *obj, unsigned int iType);

	unsigned int get_object_type(object_t *obj);

	svalue_t *call_script_func(char *objectName, char *funcName, char *paramFmt, ...);
	svalue_t *call_object_func(object_t *object, char *funcName, char *paramFmt, ...);

	svalue_t *find_object_variable(object_t *object, char *name);
	char *save_variable_2(svalue_t *var, char *ret);

#endif
