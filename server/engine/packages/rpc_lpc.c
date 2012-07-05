
#include "fs_mbuf.h"
#include "fs_rpc_lpc.h"
#include "fs_rpc_c.h"
#include "lpc_incl.h"
#include "interpret.h"
#include "fs_ipc.h"
#include "fs_comm.h"
#include "fs_lpc_op.h"
#include "fs_lpc_ext.h"
#include "lpc_args_op.h"
#include "game/fs_user.h"
#include "fs_rpc_id.h"

#include <assert.h>

extern int eval_cost;

static rpc_lpc_func_t g_rpc_call_imp;
static fs_rpc_table_t *g_rpc_table;


fs_rpc_lpc_t *fs_rpc_lpc_create(const char *cfg, rpc_lpc_func_t imp)
{
	fs_rpc_lpc_t * rpc_lpc = calloc(1, sizeof(fs_rpc_lpc_t));
	rpc_lpc->rpc_call_imp = imp;
	fs_mbuf_init(&rpc_lpc->c_unpack_mbuf, 0);
	fs_rpc_table_create(&rpc_lpc->rpc_table, cfg);
	rpc_lpc->function_table = rpc_lpc->rpc_table.function_table;

	g_rpc_table = &rpc_lpc->rpc_table;
	//***
	g_rpc_call_imp = imp;
	return rpc_lpc;
}

static void dump_field_error(svalue_t *sv, fs_rpc_field_t *field)
{
	if (IS_LPC_UNDEF(sv)) {
		fprintf(stderr, "error! pack undefined:%s->%s,expected:%s\n", 
			field->parent->name, field->name, fs_rpc_field_type_name(field));
	} else {
		fprintf(stderr, "error! pack wrong type:%s->%s,expected:%s,given:%s\n", 
			field->parent->name, field->name, fs_rpc_field_type_name(field), type_name(sv->type));
	}
}

#define PACK_FIELD_ERROR(sv, field) do {\
	dump_field_error((sv), (field));\
	return -1;\
} while (0)

#define PACK_ARRAY_ERROR(sv, field) do {\
	fprintf(stderr, "error! pack wrong array:%s->%s,expected:array of %s,given:%s\n", \
			(field)->parent->name, (field)->name, fs_rpc_field_type_name(field), type_name((sv)->type));\
	return -1;\
} while (0)


#define PACK_CHECK_TYPE(sv, lpctype) do {\
	if (!((sv)->type & lpctype)) {	\
		goto error;		\
	}				\
} while(0)

inline static int pack(fs_rpc_field_t *field, svalue_t *value, int field_count, fs_mbuf_t *mbuf);

inline int static pack_field(fs_rpc_field_t *field, svalue_t *value, fs_mbuf_t *mbuf)
{
	switch (field->type) {
	case FS_RPC_INT: {
		PACK_CHECK_TYPE(value, T_NUMBER);
		fs_rpc_int_t v = value->u.number;
		FS_MBUF_ENQ_TYPE(mbuf, &v, fs_rpc_int_t);
		//printf("pack field int.name:%s,i:%d, len:%ld\n", field->name, v, sizeof(v));
		break;
	}
	case FS_RPC_STRING: {
		PACK_CHECK_TYPE(value, T_STRING);
		fs_rpc_size_t len = SVALUE_STRLEN(value);
		FS_MBUF_ENQ_TYPE(mbuf, &len, fs_rpc_size_t);
		FS_MBUF_ENQ(mbuf, value->u.string, len);
		//printf("pack field string.name:%s, str:%s,len:%d\n", field->name, value->u.string, len);
		break;
	}
	case FS_RPC_BUFFER: {
		PACK_CHECK_TYPE(value, T_BUFFER);
		fs_rpc_size_t len = value->u.buf->size;
		FS_MBUF_ENQ_TYPE(mbuf, &len, fs_rpc_size_t);
		FS_MBUF_ENQ(mbuf, value->u.buf->item, len);
		break;
	}
	case FS_RPC_CLASS: {
		PACK_CHECK_TYPE(value, T_CLASS);
		assert(value->u.arr->size == field->class_ptr->field_count);
		//printf("unfold class elements:name:%s,%d\n", field->name, value->u.arr->size);
		if (pack(field->class_ptr->field, value->u.arr->item, field->class_ptr->field_count, mbuf)) {
			return -1;
		}
		break;
	}
	default:
		fprintf(stderr, "error! unknown lpc rpc type.rpc_type=%d,lpctype=%s\n", field->type, type_name(value->type));
		return -1;
	}

	return 0;

error:
	PACK_FIELD_ERROR(value, field);
}

inline static int pack(fs_rpc_field_t *field, svalue_t *value, int field_count, fs_mbuf_t *mbuf)
{
	int c, i;
	for (c = 0; c < field_count; ++c, ++field, ++value) {
		if (field->array == FS_RPC_ARRAY_VAR) {
			if (value->type & T_ARRAY) {
				fs_rpc_size_t len = value->u.arr->size;
				FS_MBUF_ENQ_TYPE(mbuf, &len, fs_rpc_size_t);
				//printf("pack array name:%s, len:%d\n", field->name, len);
				for (i = 0; i < len; ++i) {
					if (pack_field(field, &value->u.arr->item[i], mbuf))
						return -1;
				}
			} else {
				PACK_ARRAY_ERROR(value, field);
			}
		} else {
			if (pack_field(field, value, mbuf))
				return -1;
		}
	}

	return 0;
}

//注意，这里要去掉第一个参数
int fs_rpc_lpc_pack(fs_rpc_function_t *function, svalue_t *value, int value_count, fs_mbuf_t *mbuf)
{
	fs_rpc_class_t *arg = &function->arg_class;
	//去掉第一个参数
	if (value_count == arg->field_count - 1)  {
		if (pack(arg->field + 1, value, value_count, mbuf)) {
			fprintf(stderr, "lpc pack error! function:%s\n", arg->name);
			//assert(0);
			return -1;
		}
	} else {
		fprintf(stderr, "lpc pack error! arguments dismatch.function:%s,expect:%d,given:%d\n", 
			arg->name, arg->field_count - 1, value_count);
		//assert(0);
		return -1;
	}

	//log pack info
	log_pack(function, mbuf);

	return 0;
}

inline static int unpack(fs_rpc_field_t *field, svalue_t *value, int field_count, fs_inbuf_t* inbuf);

inline int static unpack_field(fs_rpc_field_t *field, svalue_t *value, fs_inbuf_t *inbuf)
{
	switch (field->type) {
	case FS_RPC_INT: {
		//fprintf(stderr, "unpack int\n");
		fs_rpc_int_t v;
		FS_INBUF_GET_TYPE(inbuf, &v, fs_rpc_int_t);
		set_svalue_number(value, v);
		break;
	}
	case FS_RPC_STRING: {
		fs_rpc_size_t len = 0;
		//fprintf(stderr, "unpack string buf_len:%u\n", FS_INBUF_LEN(inbuf));
		FS_INBUF_GET_TYPE(inbuf, &len, fs_rpc_size_t);
		//fprintf(stderr, "unpack string type_len:%lu,str_len:%u\n", sizeof(fs_rpc_size_t), len);
		FS_INBUF_CHECK(inbuf, len);
		char *p = new_string(len, "fs rpc:pack");
		FS_INBUF_GET_NO_CHECK(inbuf, p, len);
		p[len] = '\0';
		set_svalue_malloced_string(value, p);
		break;
	}
	case FS_RPC_BUFFER: {
		fs_rpc_size_t len = 0;
		FS_INBUF_GET_TYPE(inbuf, &len, fs_rpc_size_t);
		//fprintf(stderr, "unpack string type_len:%lu,str_len:%u\n", sizeof(fs_rpc_size_t), len);
		FS_INBUF_CHECK(inbuf, len);
		buffer_t *buf = allocate_buffer(len);
		if (buf == NULL) {
			fprintf(stderr, "fail to allocate buffer when unpacking type=%d,len=%u\n", field->type, len);
			return -1;
		}
		FS_INBUF_GET_NO_CHECK(inbuf, buf->item, len);
		set_svalue_buffer(value, buf);
		break;
	}
	case FS_RPC_CLASS: {
		//fprintf(stderr, "unpack class\n");
		array_t *cls = allocate_class_by_size(field->class_ptr->field_count);
		set_svalue_class(value, cls);

		//assert(value->u.arr->size == field->class_ptr->field_count);
		if (unpack(field->class_ptr->field, cls->item, field->class_ptr->field_count, inbuf))
			return -1;
		break;
	}
	default:
		fprintf(stderr, "unpack unknown type=%d\n", field->type);
		return -1;
	}
	return 0;
}


inline static int unpack(fs_rpc_field_t *field, svalue_t *value, int field_count, fs_inbuf_t* inbuf)
{
	int c, i;

	//fprintf(stderr, "unpack client network data len:%u\n", FS_INBUF_LEN(inbuf));
	for (c = 0; c < field_count; ++c, ++field, ++value) {
		if (field->array == FS_RPC_ARRAY_VAR) {
			//fprintf(stderr, "unpack array\n");
			fs_rpc_size_t len = 0;
			FS_INBUF_GET_TYPE(inbuf, &len, fs_rpc_size_t);
			//toco : check the array size
			FS_INBUF_CHECK(inbuf, len);
			array_t *arr = allocate_array(len);
			set_svalue_array(value, arr);
			for (i = 0; i < len; ++i) {
				if (unpack_field(field, &arr->item[i], inbuf))
					return -1;
			}
		} else {
			if (unpack_field(field, value, inbuf))
				return -1;
		}
	}

	return 0;
}


typedef struct rpc_lpc_udata_s {
	funptr_t *rpcfun;
} rpc_lpc_udata_t;

#define UDATA(udata) ((rpc_lpc_udata_t*)(udata))

inline static funptr_t *get_module_function(char *module, char *funname)
{
	object_t *mod = find_object(module); 
	if (mod == NULL) {
		fprintf(stderr, "no lpc rpc module object:%s\n", module);
		return NULL;
	}
	
	funptr_t *funptr = fs_make_lfun_funp(mod, funname, NULL);
	if (funptr == NULL) {
		fprintf(stderr, "no lpc rpc module object function:%s,%s\n", module, funname);
		return NULL;
	}

	return funptr;
}

inline static funptr_t *get_lpc_rpc_function(fs_rpc_function_t *function)
{
	if (function->udata != NULL) {
		funptr_t *funp = UDATA(function->udata)->rpcfun;
		if ((funp->hdr.owner->flags &O_DESTRUCTED)) {
			free_funp(funp);
			free(function->udata);	
			function->udata = NULL;
		} else {
			return funp;
		}
	}

	char *funname = function->arg_class.name;
	funptr_t *funptr = get_module_function(function->module, funname);
	if (funptr == NULL) {
		fprintf(stderr, "no lpc rpc module object function:%s,%s\n", function->module, funname);
		return NULL;
	}

	function->udata = calloc(1, sizeof(rpc_lpc_udata_t));
	UDATA(function->udata)->rpcfun = funptr;
	fprintf(stderr, "install lpc module object function:%s,%s\n", function->module, funname);

	return UDATA(function->udata)->rpcfun;
}

static int rpc_lpc_module_update(fs_rpc_function_t *function, void *data)
{
	object_t *obj = data;
	char *funname = function->arg_class.name;
	if (strcmp(obj->name, function->module) == 0) {
		if (function->udata != NULL) {
			funptr_t *funptr = fs_make_lfun_funp(obj, funname, NULL);
			if (funptr == NULL) {
				fprintf(stderr, "update error.no rpc function:%s,%s\n", function->module, funname);
			} else {
				free_funp(UDATA(function->udata)->rpcfun);
				UDATA(function->udata)->rpcfun = funptr;
				fprintf(stderr, "update rpc function:%s,%s\n", function->module, funname);
			}
		} else {
			fprintf(stderr, "skip update rpc function:%s,%s\n", function->module, funname);
		}
	}
	return 1;
}

void fs_rpc_lpc_module_update(object_t *obj)
{
	fprintf(stderr, "try to update rpc module object:%s\n", obj->name);
	fs_rpc_foreach_function(g_rpc_table->function_table, rpc_lpc_module_update, obj);
}

//todo : 这里出现vfd和uid，需要分开
object_t* GetUserLpcObj( int usernum );
int fs_rpc_lpc_called(int id, fs_rpc_function_t *function, fs_inbuf_t *inbuf)
{
	fs_protocol_id_t pid = function->function_id;
	funptr_t *func = get_lpc_rpc_function(function);
	if (func == NULL) {
		fprintf(stderr, "error miss rpc called id:%d,moduel:%s,function:%s,pid:%d\n", 
			id, function->module, function->arg_class.name, pid);
		return -1;
	}

	//skip the first arg id
	//printf("rpc lpc called moduel:%s,function:%s\n", function->module, function->arg_class.name);
	int argc = function->arg_class.field_count - 1;
	switch (function->arg_class.field[0].type) {
		case FS_RPC_OBJECT: {
			object_t * user_obj = GetUserLpcObj(id);
			if (user_obj == NULL) {
				fprintf(stderr, "error rpc lpc not user object id:%d,moduel:%s,function:%s\n", 
					id, function->module, function->arg_class.name);
				return -1;
			}
			push_object(user_obj);
			break;
		}
		case FS_RPC_INT:
		default:
			push_number(id);
			break;
	}
	push_undefineds(argc);
	if (0 == unpack(function->arg_class.field + 1, sp - argc + 1, argc , inbuf)) {

		// 设置eval_cost
		eval_cost = max_cost;

		//log_unpack(function, inbuf);
		//return safe_apply(function->arg_class.name, object, function->arg_class.field_count, ORIGIN_DRIVER);
		if (NULL == safe_call_function_pointer(func, function->arg_class.field_count)) {
			fprintf(stderr, "error rpc lpc called id:%d,moduel:%s,function:%s\n", 
				id, function->module, function->arg_class.name);
			return -1;
		}
	} else {
		pop_n_elems(function->arg_class.field_count);
		fprintf(stderr, "error rpc lpc unpack id:%d,moduel:%s,function:%s\n", 
			id, function->module, function->arg_class.name);
		return -1;
	}

	return 0;
}


int fs_rpc_dispatch(fs_rpc_lpc_t *rpc_lpc, int id, fs_inbuf_t *inbuf)
{
	fs_protocol_id_t pid = 0;
	FS_INBUF_GET_TYPE(inbuf, &pid, fs_protocol_id_t);

	fs_rpc_function_t *func = fs_rpc_function_at(rpc_lpc->function_table, pid);
	if (func == NULL) {
		fprintf(stderr, "error no function for rpc id:%d,pid:%u\n", id, pid);
		return -1;
	}

	if (func->arg_class.c_imp) {
		if (func->c_function == NULL) {
			fprintf(stderr, "error no c function for rpc id:%d,pid:%u\n", id, pid);
			return -1;
		}

		fs_mbuf_reset(&rpc_lpc->c_unpack_mbuf);
		void *data = fs_rpc_c_unpack(func, &rpc_lpc->c_unpack_mbuf, inbuf);
		if (data == NULL) {
			fprintf(stderr, "error unpack c network data id:%d,pid:%u\n", id, pid);
			return -1;
		}

		func->c_function(id, data);
		return 0;
	} else {
		return fs_rpc_lpc_called(id, func, inbuf);
	}
}

inline static void check_specific_pto_stats(fs_user_t *user, fs_rpc_function_t *func, int pid)
{
	if (!TEST_RPC_EXCLUDED_PTO(func->flag)) {
		if (user->specific_pto_start) {
			user->specific_pto_count++;
			/*
			fprintf(stderr, "inc pto stats uid=%d,count=%d,threshold=%d,step=%d,mod=%d,errorcount=%d,funname=%s\n"
				, uid, user->specific_pto_count, user->specific_pto_threshold, user->specific_pto_step, 
				user->specific_pto_mod, user->specific_pto_error_count, func->arg_class.name);
			*/
			if (user->specific_pto_count == user->specific_pto_threshold) {
				int next;
				//int isok = (pid == RPC_SERVER_PTO_STATS_THRESHOLD);
				int isok = 1;
				if (!isok) {
					user->specific_pto_error_count++;
				}

				//排除 0 和 rpc_server_pto_stats_start
				next = 2 + ((user->specific_pto_threshold + user->specific_pto_step) % user->specific_pto_mod);
				user->specific_pto_step = user->specific_pto_threshold;
				user->specific_pto_threshold = next;
				user->specific_pto_count = 0;

				//防止c和脚本穿插调用，所以这个逻辑放在最后
				call_script_func("module/validate.c", "spcecific_pto_stats_alarm", "oii", 
					user->user_obj, isok,  user->specific_pto_error_count);
			}
		} else {
			/*
			fprintf(stderr, "inc pto no start uid=%d,threshold=%d,step=%d,mod=%d,funname=%s\n", 
				uid, user->specific_pto_threshold, user->specific_pto_step, 
				user->specific_pto_mod, func->arg_class.name);
			*/
		}
	} else {
		/*
		fprintf(stderr, "skip pto stats threshold=%d,step=%d,mod=%d,funname=%s\n", 
			user->specific_pto_threshold, user->specific_pto_step, user->specific_pto_mod, func->arg_class.name);
		*/
	}
}

extern time_t last_pto_timestamp_cache;
inline static void on_user_recv_pto(fs_rpc_function_t *func, int uid, int pid)
{
	fs_user_t *user = FindUserOnline(uid);
	if (user == NULL) { 
		fprintf(stderr, "error no userobj for rpc uid:%d,pid:%u\n", uid, pid);
		return; 
	}
	user->last_pto_timestamp = last_pto_timestamp_cache;
	check_specific_pto_stats(user, func, pid);
}


//客户端输入专用
int fs_rpc_user_dispatch(fs_rpc_lpc_t *rpc_lpc, int uid, fs_inbuf_t *inbuf)
{
	int ret;
	fs_protocol_id_t pid = 0;
	FS_INBUF_GET_TYPE(inbuf, &pid, fs_protocol_id_t);

	TIME_TAG_BEGIN(g_log_pto_enable, rpc_called)
	fs_rpc_function_t *func = fs_rpc_function_at(rpc_lpc->function_table, pid);
	if (func == NULL) {
		fprintf(stderr, "error no function for rpc uid:%d,pid:%u\n", uid, pid);
		return -1;
	}

	if (func->arg_class.c_imp) {
		if (func->c_function == NULL) {
			fprintf(stderr, "error no c function for rpc uid:%d,pid:%u\n", uid, pid);
			return -1;
		}

		fs_mbuf_reset(&rpc_lpc->c_unpack_mbuf);
		void *data = fs_rpc_c_unpack(func, &rpc_lpc->c_unpack_mbuf, inbuf);
		if (data == NULL) {
			fprintf(stderr, "error unpack c network data uid:%d,pid:%u\n", uid, pid);
			return -1;
		}

		func->c_function(uid, data);
		ret = 0;
	} else {
		ret = fs_rpc_lpc_called(uid, func, inbuf);
	}

	//on_user_recv_pto(func, uid, pid);
	TIME_TAG_END(g_log_pto_enable, rpc_called,"fun_name=%s,data_len=%u,uid=%d", func->arg_class.name, inbuf->size, uid);

	return ret;
}

int fs_rpc_lpc_pack_arg(int argc, svalue_t *argv, fs_rpc_lpc_t *rpc_lpc, fs_net_mbuf_t *netbuf, int *rid)
{
	//commond id: uid, vfd or hid
	int  id = argv[0].u.number;
	fs_protocol_id_t pid = argv[1].u.number;

	fs_rpc_function_t *function = fs_rpc_function_at(rpc_lpc->function_table, pid);
	if (function == NULL) {
		fprintf(stderr, "error! no such rpc function to call.pid:%d\n", pid);
		return -1;
	}

	fs_net_mbuf_reset(netbuf);
	FS_MBUF_ENQ_TYPE(&netbuf->mbuf, &pid, fs_protocol_id_t);
	// skip the vfd arg for arg_class field, and skip the pid arg.
	if (fs_rpc_lpc_pack(function, &argv[2], argc - 2, &netbuf->mbuf)) {
		return -1;
	}

	*rid = id;

	return 0;
} 

//////////////////////////////

#define SET_NO_CLASS_FIELD_TYPE(field, t) do {\
	(field)->type = (t);\
	(field)->class_index = FS_RPC_CLASS_INDEX_NOT;\
} while(0)

int static init_field(fs_rpc_field_t *field, int field_type, fs_rpc_class_table_t *class_table, program_t *prog, int index)
{
	if ((field_type & TYPE_MOD_ARRAY) == TYPE_MOD_ARRAY) {
		field->array = FS_RPC_ARRAY_VAR;
		field_type &= ~TYPE_MOD_ARRAY;
	} else
		field->array = FS_RPC_ARRAY_NOT;

	if (field_type == TYPE_ANY) {
		if (index == 0) {
			SET_NO_CLASS_FIELD_TYPE(field, FS_RPC_MIXED); 
		} else {
			fprintf(stderr, "mixed type must be the first arg\n");
			return -1;
		}
	} else if (field_type == TYPE_STRING) {
		SET_NO_CLASS_FIELD_TYPE(field, FS_RPC_STRING); 
	} else if (field_type == TYPE_NUMBER) {
		SET_NO_CLASS_FIELD_TYPE(field, FS_RPC_INT); 
	} else if (field_type == TYPE_BUFFER) {
		SET_NO_CLASS_FIELD_TYPE(field, FS_RPC_BUFFER); 
	}  else if (field_type == TYPE_OBJECT) {
		if (index == 0) {
			SET_NO_CLASS_FIELD_TYPE(field, FS_RPC_OBJECT); 
		} else {
			fprintf(stderr, "object type must be the first arg\n");
			return -1;
		}
	} else if (IS_CLASS(field_type)) {
		int prog_class_idx = CLASS_IDX(field_type);
		class_def_t *cd = &prog->classes[prog_class_idx];
		int cidx = fs_rpc_class_index(class_table, prog->strings[cd->name]); 
		assert(cidx >= 0);
		field->type = FS_RPC_CLASS;
		field->class_index = cidx;
	} else {
		return -1;
	}

	return 0;
}

int has_same_class(fs_rpc_class_table_t * class_table, fs_rpc_class_t *cls)
{
	int index = fs_rpc_class_index(class_table, cls->name);
	fs_rpc_class_t * exist_class = fs_rpc_class(class_table, index);
	return fs_rpc_class_same(cls, exist_class);
}

static int expand_class_table(fs_rpc_table_t *table, program_t *prog, int cimp)
{
	int i, j;
	fs_rpc_class_table_t *tbl = NULL;
	int add_class = prog->num_classes;
	/* 一个program最好不要声明32个以上class, 函数参数只能索引32个class,请看 CLASS_IDX */
	assert(prog->num_classes <= 32); 

	if (table->class_table == NULL) {
		tbl = calloc(1, sizeof(fs_rpc_class_table_t) + sizeof(fs_rpc_class_t) * add_class);
	} else {
		tbl = realloc(table->class_table, 
			sizeof(fs_rpc_class_table_t) + sizeof(fs_rpc_class_t) * (table->class_table->elts + add_class));
	}
	tbl->rpc_class = (fs_rpc_class_t*)((char*)tbl + sizeof(fs_rpc_class_table_t));
	memset(&tbl->rpc_class[tbl->elts], 0, sizeof(fs_rpc_class_t) * add_class);


	for(i = 0; i < add_class; ++i) {
		class_def_t *cd = &prog->classes[i];
		fs_rpc_class_t *class = &tbl->rpc_class[tbl->elts]; 

		class->name = strdup(prog->strings[cd->name]);
		assert(class->name);
		class->field_count = cd->size;
		class->field = calloc(class->field_count, sizeof(fs_rpc_field_t));
		class->c_imp = cimp;


		for (j = 0; j < class->field_count; ++j) {
			class_member_entry_t *cm = &prog->class_members[cd->index + j];  
			fs_rpc_field_t *field = &class->field[j];
			field->name = strdup(prog->strings[cm->name]);
			assert(field->name);
			if (0 > init_field(field, cm->type, tbl, prog, j)) {
				fprintf(stderr, "error:rpc reject the field.class:%s,field:%s,type=%d\n",
					class->name, field->name, cm->type);
				goto error;
			}

		} /* for class->field_count */

		if (has_same_class(tbl, class)) {
			fprintf(stderr, "warn:rpc already has the same class:%s\n", class->name);
			class_free(class);
			continue;
		}
		/* only same class to be skip */
		if (fs_rpc_class_index(tbl, class->name) >= 0) {
			fprintf(stderr, "error:rpc has same name class:%s\n", class->name);
			goto error;
		}
		tbl->elts++;
		
	} /* for add_class */


	table->class_table = tbl;
	return 0;	

	error:
		fs_rpc_table_destroy(table);
		return -1;
}

static char *new_module_name(char *name)
{
	char *ret;
	unsigned len;
	char *n = strdup(name);

	if (n[0] == '/') 
		n++;
	len = strlen(n);
	if (len >= 2) {
		if (n[len-2] == '.' && n[len-1] == 'c')
			n[len - 2] = '\0';
	}
	assert(strlen(n) > 0);
	ret = strdup(n);
	free(n);
	return ret;
}

typedef struct {
	int idx;
	char *name;
} sort_function_t;

static int sort_function(const void *a, const void *b)
{
	const sort_function_t *fa = a;
	const sort_function_t *fb = b;
	return strcmp(fa->name, fb->name);
}

static int expand_function_table(fs_rpc_table_t *table, program_t *prog, int cimp, char *module, mapping_t *static_pids)
{
	int i, j, old_function;
	sort_function_t *sort;
	fs_rpc_function_table_t *tbl = NULL;
	int add_function = prog->num_functions_defined;
	/* 一个program最好不要声明32个以上class, 函数参数只能索引32个class,请看 CLASS_IDX */
	assert(prog->num_classes <= 32); 

	static int lpc_pid = FS_RPC_LPC_PID_START;
	//static int c_pid = FS_RPC_C_PID_START;

	if (table->function_table == NULL) {
		old_function = 0;
		tbl = calloc(1, sizeof(fs_rpc_function_table_t) + sizeof(fs_rpc_function_t) * add_function);
		tbl->rpc_function = (fs_rpc_function_t*)((char*)tbl + sizeof(fs_rpc_function_table_t));
	} else {
		old_function = table->function_table->elts;
		tbl = realloc(table->function_table, 
			sizeof(fs_rpc_function_table_t)+sizeof(fs_rpc_function_t)*(old_function + add_function));
		tbl->rpc_function = (fs_rpc_function_t*)((char*)tbl + sizeof(fs_rpc_function_table_t));
		memset(&tbl->rpc_function[tbl->elts], 0, sizeof(fs_rpc_function_t) * add_function);
	}

	sort = calloc(add_function, sizeof(*sort));
	for(i = 0; i < add_function; ++i) {
		compiler_function_t *cf = &prog->function_table[i];
		sort[i].idx = i;
		sort[i].name = cf->name;
	}
	qsort(sort, add_function, sizeof(*sort), sort_function);

	for(i = 0; i < add_function; ++i, tbl->elts++) {
		compiler_function_t *cf = &prog->function_table[sort[i].idx];
		//compiler_function_t *cf = &prog->function_table[i];
		runtime_defined_t *rd = &prog->function_offsets[cf->runtime_index].def;

		//分配协议id
		int index = old_function + i;
		fs_rpc_function_t *function = &tbl->rpc_function[index];

		if (static_pids != NULL) {
			svalue_t *v = find_string_in_mapping(static_pids, cf->name);
			/*
			if (!(v->type == T_NUMBER && v != &const0u && v != &const0)) {
				fprintf(stderr, "error must set aside the static protocol id for function:%s\n", cf->name);
				assert(0);
			}
			*/
			if (v->type == T_NUMBER && v != &const0u && v != &const0) {
				if (v->u.number >= FS_RPC_LPC_PID_START) {
					fprintf(stderr, "static protocol id must lesss than %d\n", FS_RPC_LPC_PID_START);
					assert(0);
				}
				function->function_id = v->u.number;
			} else {
				//function->function_id = cimp ? c_pid++ : lpc_pid++;
				function->function_id = lpc_pid++;
			}
		} else {
			//function->function_id = cimp ? c_pid++ : lpc_pid++;
				function->function_id = lpc_pid++;
		}

		if (module == NULL) {
			function->module = strdup("no_such_module");
		} else {
			function->module = new_module_name(module);
		}

		fs_rpc_class_t *arg_class = &function->arg_class;
		arg_class->name = strdup(cf->name); //函数的名字
		arg_class->field_count = rd->num_arg;
		arg_class->field = calloc(rd->num_arg, sizeof(fs_rpc_field_t));
		arg_class->c_imp = cimp;

		if (fs_rpc_function_find(tbl, arg_class->name)) {
			fprintf(stderr, "error:rpc has same name function:%s\n", arg_class->name);
			goto error;
		}

		int arg_type_start = prog->type_start[rd->f_index];	
		for (j = 0; j < rd->num_arg; ++j) {
			int arg_type = prog->argument_types[arg_type_start + j];

			fs_rpc_field_t *field = &arg_class->field[j];
			if (0 > init_field(field, arg_type, table->class_table, prog, j)) {
				fprintf(stderr, "error:rpc reject the arg.function:%s,field type=%d\n",
					arg_class->name, arg_type);
				goto error;
			}
		}

	}	


	table->function_table = tbl;
	free(sort);
	return 0;	

	error:
		free(sort);
		fs_rpc_table_destroy(table);
		return -1;
}



/*
static int set_function_id(mapping_t *m, mapping_node_t *node, void *table)
{
	fs_rpc_table_t *t = table;
	fs_rpc_function_set_id(t->function_table, (const char**)&node->values[0].u.string, &node->values[1].u.number, 1); 
	return 1;
}
*/

void f_fs_rpc_make_cfg(void)
{
	int i;
	array_t *decl = (sp-2)->u.arr;
	//mapping_t *id = (sp-1)->u.map;
	const char *rpc_id = (sp - 1)->u.string;
	const char *rpc_cfg = sp->u.string;

	fs_rpc_table_t  table;
	object_t *hdrobj = NULL, *impobj = NULL;
	memset(&table, 0, sizeof(table));

	for (i = 0; i < decl->size; ++i) {
		mapping_t *module = decl->item[i].u.map;
		svalue_t *header = find_string_in_mapping(module, "declare");
		svalue_t *imp = find_string_in_mapping(module, "implement");
		svalue_t *cimp = find_string_in_mapping(module, "c_implement");
		svalue_t *static_ptoid = find_string_in_mapping(module, "static_ptoid");
		mapping_t *static_pids = (static_ptoid->type == T_MAPPING) ? static_ptoid->u.map : NULL;

		if (header == NULL || imp == NULL|| header->u.string == NULL) {
			continue;
		}
		int iscimp = cimp->u.string != NULL;

		//printf("try to read header file:%s,cimp:%d\n", header->u.string, iscimp);
		hdrobj = find_object(header->u.string);
		if (hdrobj == NULL) {
			fprintf(stderr, "rpc cfg error:no such header:%s\n", header->u.string);
			continue;
		}
	
		(void)impobj;
		/*
		printf("try to read implement file:%s\n", imp->u.string);
		impobj = find_object(imp->u.string);
		if (impobj == NULL) {
			fprintf(stderr, "rpc cfg warn:no such implement:%s\n", imp->u.string);
		} else {
			printf("parse implement file:%s\n", impobj->prog->name);
		}
		*/

		if (0 > expand_class_table(&table, hdrobj->prog, iscimp)) 
			goto error;
		if (0 > expand_function_table(&table, hdrobj->prog, iscimp, imp->u.string, static_pids))
			goto error;
	}

	//mapTraverse(id, set_function_id, &table);

	if (table.function_table != NULL) {
		fs_rpc_function_qsort(table.function_table);
	}
	fs_rpc_dump_cfg(&table, rpc_cfg);
	fs_rpc_dump_c(&table, "../engine/packages/fs_rpc_c.h", "../engine/packages/rpc_c.c");
	fs_rpc_dump_function_id(table.function_table, rpc_id);
	fs_rpc_dump_function_c_id(table.function_table, "../engine/packages/fs_rpc_id.h");
error:
	pop_n_elems(st_num_arg);
	fs_rpc_table_destroy(&table);
}

void f_fs_rpc_call(void)
{
	SET_ALL_ARGS
	g_rpc_call_imp(st_num_arg, ARGS(0));
	POP_ALL_ARGS;
} 

void f_fs_validation_start(void)
{
	SET_ALL_ARGS
	int uid = ARGS(0)->u.number;
	fs_user_t *user = FindUserOnline(uid);
	if (user != NULL) {
		specific_pto_stats_start(user);
	}
	POP_ALL_ARGS;
} 

void f_fs_set_validation(void)
{
	SET_ALL_ARGS
	int uid = ARGS(0)->u.number;
	int threshold = ARGS(1)->u.number;
	int step = ARGS(2)->u.number;
	int mod = ARGS(3)->u.number;
	fs_user_t *user = FindUserOnline(uid);
	if (user != NULL) {
		set_specific_pto_stats(user, threshold, step, mod);
	}
	POP_ALL_ARGS;
} 

void f_fs_last_pto_timestamp(void)
{
	time_t stamp = 0;
	SET_ALL_ARGS
	int uid = ARGS(0)->u.number;
	fs_user_t *user = FindUserOnline(uid);
	if (user != NULL) {
		stamp = user->last_pto_timestamp;
	}
	POP_ALL_ARGS;
	push_number(stamp);
}

void f_fs_exclude_stats_pto(void)
{
	SET_ALL_ARGS
	svalue_t *sv;
	array_t *arr = ARGS(0)->u.arr;
	int i;
	int pid;
	fs_rpc_function_t *func;

	for (i = 0; i < arr->size; ++i) {
		sv = &arr->item[i];
		if (sv->type != T_NUMBER) {
			fprintf(stderr, "error bad type for exluding pto stats\n");
			continue;
		}
		pid = sv->u.number;
		func = fs_rpc_function_at(g_rpc_table->function_table, pid);
		if (func != NULL) {
			SET_RPC_EXCLUDED_PTO(func->flag);
			//fprintf(stderr, "ok exlude pto stats pid:%d\n", pid);
		} else {
			fprintf(stderr, "error no pto to exlude stats pid:%d\n", pid);
		}
	}
	
	POP_ALL_ARGS;
}

extern fs_rpc_lpc_t *gamed_rpc_lpc;
static int to_loop(int argc, svalue_t *argv)
{
	fs_uid_t uid = argv[0].u.number;
	fs_protocol_id_t pid = argv[1].u.number;

	
	fs_mbuf_t mbufvar;
	fs_mbuf_t *mbuf = &mbufvar;
	fs_mbuf_init(mbuf, 0);

	fs_rpc_function_t *function = fs_rpc_function_at(g_rpc_table->function_table, pid);
	if (function == NULL)  {
		fprintf(stderr, "no such rpc function to call.pid:%d\n", pid);
		return -1;
	}

	// add the pid
	FS_MBUF_ENQ_TYPE(mbuf, &pid, fs_protocol_id_t);

	// skip the uid arg for arg_class field, and skip the pid arg.
	if (fs_rpc_lpc_pack(function, &argv[2], argc - 2, mbuf)) {
		fprintf(stderr, "rpc call loop pack error\n");
		return -1;
	}


	{
		char *rawbuf = malloc(mbuf->data_size);
		int off = 0;
		fs_mbuf_blk_t *blk;
		for (blk = mbuf->head; blk != NULL; blk = blk->next) {
			unsigned len = FS_MBUF_BLK_DATA_LEN(blk);
			if (len > 0) {
				//bufferevent_write(bufev, blk->head, len);
				memcpy(rawbuf + off, blk->head, len);
				off += len;
			}
		}

		fs_inbuf_t inbuf;	
		FS_INBUF_INIT(&inbuf, rawbuf, mbuf->data_size);
		fs_rpc_dispatch(gamed_rpc_lpc, uid, &inbuf);
		free(rawbuf);
	}

	fs_mbuf_destruct(mbuf);

	return 0;
}

void f_fs_rpc_call_loop(void)
{
	SET_ALL_ARGS
	to_loop(st_num_arg, ARGS(0));
	POP_ALL_ARGS;
}


////////test
/*
#define TYPE_UNKNOWN	0	
#define TYPE_ANY        1	
#define TYPE_NOVALUE    2
#define TYPE_VOID       3
#define TYPE_NUMBER     4
#define TYPE_STRING     5
#define TYPE_OBJECT     6
#define TYPE_MAPPING    7
#define TYPE_FUNCTION   8
#define TYPE_REAL       9
#define TYPE_BUFFER     10
*/
char *type_name_tbl[] = {
"TYPE_UNKNOWN",
"TYPE_ANY",	
"TYPE_NOVALUE",
"TYPE_VOID",
"TYPE_NUMBER",
"TYPE_STRING",
"TYPE_OBJECT",
"TYPE_MAPPING",
"TYPE_FUNCTION",
"TYPE_REAL",
"TYPE_BUFFER",
};

int is_array(int type) 
{
	return type & TYPE_MOD_ARRAY;
}

char * my_type_name(int type)
{
	if (type >= 0 && type < sizeof(type_name_tbl)/sizeof(char*)) {
		return type_name_tbl[type];
	} else {
		printf("bad type:%d\n", type);
		printf("is class:%d\n", IS_CLASS(type));
		printf("class index:%d\n", CLASS_IDX(type));
	
		return NULL;
	}
}

void pack_test()
{
	object_t *obj;
	program_t *prog;
	int i,j;
	
	char *file = "/rc/pack";
	obj = find_object(file);
	prog = obj->prog;
	printf("num class:%d\n", prog->num_classes);
	for(i = 0; i < prog->num_classes; ++i) {
		class_def_t * cd = &prog->classes[i];
		printf("class name:%s,type:%s,size:%d,index:%d\n", prog->strings[cd->name], 
			my_type_name(cd->type), cd->size, cd->index);
		for (j = 0; j < cd->size; ++j) {
			class_member_entry_t *cm = &prog->class_members[cd->index + j];  
			printf("member name:%s,type:%s\n", prog->strings[cm->name], my_type_name(cm->type));
		}
		
	}

	printf("num defined function:%d\n", prog->num_functions_defined);
	for(i = 0; i < prog->num_functions_defined; ++i) {
		compiler_function_t *cf = &prog->function_table[i];
		printf("defined function name=%s, return type=%s\n", cf->name, my_type_name(cf->type));
		runtime_defined_t *rd = &prog->function_offsets[cf->runtime_index].def;
		printf("runtime def num arg=%d, num local=%d, index=%d, index=%d\n", 
				rd->num_arg, rd->num_local, rd->f_index,cf->runtime_index);
		unsigned short type_start = prog->type_start[rd->f_index];	
		printf("type start index:%d\n", type_start);
		for (j = 0; j < rd->num_arg; ++j) {
			printf("arg type:%s\n",	my_type_name(prog->argument_types[type_start + j]));
		}
	}	
}

/////////////////////////////////////////////////////
