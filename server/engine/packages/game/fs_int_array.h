#ifndef _FS_INT_ARRAY_H_
#define _FS_INT_ARRAY_H_

#define INT_ARRAY_INI_SIZE                8

// TODO: 内存

/* 初始化array */
#define INIT_INT_ARRAY(v, s) \
do { \
	v = (fs_int_array_t *) calloc(1, sizeof(fs_int_array_t)); \
	v->intp = (int *) calloc(s, sizeof(int)); \
	v->size = s; \
	v->n = 0; \
} while(0)

/* 重设array */
#define RESET_INT_ARRAY(v) \
do { \
	if (v) v->n = 0; \
} while(0)

/* 添加一个元素 */
#define ADD_INT_ARRAY(v, e) \
do { \
	if (v)  { \
		if (v->n >= (v->size - 1)) { \
			v->size = (v->size + INT_ARRAY_INI_SIZE); \
			v->intp = (int *) realloc(v->intp, v->size * sizeof(int)); \
		} \
		v->intp[v->n++] = e; \
		v->intp[v->n] = 0; \
	} \
} while(0)


/* 删除一个元素 */
/* 从最后一个找起，应该更容易找到 
 * 找到之后所有元素前移*/
#define SUB_INT_ARRAY(arr, val) { \
	if (arr && arr->n) { \
		int index, fix = -1; \
		for (index = arr->n - 1; index >= 0; index--) { \
			if (arr->intp[index] == val) { \
				fix = index; \
				break; \
			} \
		} \
		if (fix >= 0) { \
			for (index = fix; index <= arr->n - 1; index++) { \
				arr->intp[index] = arr->intp[index + 1]; \
			} \
			arr->n--; \
		} \
	} \
}

#define FREE_INT_ARRAY(v) { \
	if (v) { \
		free(v->intp); \
		free(v); \
	} \
}

/* 整形数组结构 */
typedef struct fs_int_array_s {
	int size;
	int n;
	int *intp;
} fs_int_array_t;

void test_array();
#endif

