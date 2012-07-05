
#include "fs_lpc_ext.h"

funptr_t *fs_make_lfun_funp(object_t *obj, char *function_name, svalue_t *args)
{
	funptr_t *fp;
	char *name = function_name;
	int flags, index, fio, vio;

	program_t *prog = find_function_by_name2(obj, &name, &index, &fio, &vio);
	if (prog == NULL) 
		return NULL;

	compiler_function_t *funp = &prog->function_table[index];
	//runtime_defined_t *fundefp = &(FIND_FUNC_ENTRY(prog, funp->runtime_index)->def);
	//flags = ob->prog->function_flags[funp->runtime_index + fio];
	flags = funp->runtime_index + fio;

	fp = (funptr_t*)DXALLOC(sizeof(funptr_hdr_t) + sizeof(local_ptr_t), TAG_FUNP, "make_local_funp");
	fp->hdr.owner = obj;;
	add_ref(obj, "make_local_funp");
	fp->hdr.type = FP_LOCAL | FP_NOT_BINDABLE;

	//funflags
	fp->f.local.index = flags;

	if (args != NULL && args->type == T_ARRAY) {
		fp->hdr.args = args->u.arr;
		args->u.arr->ref++;
	} else {
		fp->hdr.args = NULL;
	}

	fp->hdr.ref = 1;

	return fp;
}
