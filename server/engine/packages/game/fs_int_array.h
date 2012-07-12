#ifndef _FS_INT_ARRAY_H_
#define _FS_INT_ARRAY_H_

#define INT_ARRAY_INI_SIZE                8

// TODO: �ڴ�

/* ��ʼ��array */
#define INIT_INT_ARRAY(v, s) \
do { \
	v = (fs_int_array_t *) calloc(1, sizeof(fs_int_array_t)); \
	v->intp = (int *) calloc(s, sizeof(int)); \
	v->size = s; \
	v->n = 0; \
} while(0)

/* ����array */
#define RESET_INT_ARRAY(v) \
do { \
	if (v) v->n = 0; \
} while(0)

/* ���һ��Ԫ�� */
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


/* ɾ��һ��Ԫ�� */
/* �����һ������Ӧ�ø������ҵ� 
 * �ҵ�֮������Ԫ��ǰ��*/
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

/* ��������ṹ */
typedef struct fs_int_array_s {
	int size;
	int n;
	int *intp;
} fs_int_array_t;

void test_array();
#endif

