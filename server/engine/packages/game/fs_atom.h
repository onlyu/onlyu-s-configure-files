#ifndef _FS_ATOM_H_
#define _FS_ATOM_H_


#include "hash.h"
#include <assert.h>

// hashtable
typedef struct fs_atom_node_s {
	int atom;
	int strpos;
	int strlen;
	struct fs_atom_node_s *next;
} fs_atom_node_t;

typedef struct fs_atom_table_s {

	char *atombuf;						/*ԭ�Ӵ�������*/
	int bufsize;						/*�����С*/
	int bufpos;							/*��ʹ�õĻ���λ��*/
	int atomid;							/*��ʹ�õ�ID*/

	fs_atom_node_t **hashtbl;
	fs_atom_node_t **nodes;             /*ԭ�Ӵ�ָ������*/
	int hashsize;
	int atomnum;
	int inited;

} fs_atom_table_t;

int fs_default_init_atom_table();
int fs_atom_get_string_atom(char *str);
int fs_atom_find_string_atom(char *str);
char *fs_atom_get_string_by_atom(int atom);

#endif
