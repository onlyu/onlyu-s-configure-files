typedef struct list_int
{
	int(*add) (struct list_int *listp, int num);
	int(*get) (struct list_int *listp, int *num);

	int *_store;
	int _max; 
	int _head;
	int _tail;
	int _used; 
} LIST_I;

int _add_int(LIST_I *listp, int num);
int _get_int(LIST_I *listp, int *num);
int init_list_int(LIST_I **listpp, int max_list);
void free_list_int(LIST_I *listp);
