#ifndef __LPC_STACK_H__

#define __LPC_STACK_H__

#define MAX_STACK 20480
#define POP_STR_LEN 1024

typedef struct stack_string
{
	int(*push) (struct stack_string *stackp, char *value);
	int(*pop) (struct stack_string *stackp, char **value);
	char *store[MAX_STACK];
	int index;
} ST_S;

int __push_string(ST_S *stackp, char *value);
int __pop_string(ST_S *stackp, char **value);
int init_stack_string(ST_S **stackpp);
void free_stack_string(ST_S *stackp);

typedef struct stack_int
{
	int(*push) (struct stack_int *stackp, int num);
	int(*pop) (struct stack_int *stackp, int *num);
	int store[MAX_STACK];
	int index;
} ST_I;

int __push_int(ST_I *stackp, int num);
int __pop_int(ST_I *stackp, int *num);
int init_stack_int(ST_I **stackpp);
void free_stack_int(ST_I *stackp);

#endif // __LPC_STACK_H__
