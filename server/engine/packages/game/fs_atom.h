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

	char *atombuf;						/*原子串缓冲区*/
	int bufsize;						/*缓冲大小*/
	int bufpos;							/*以使用的缓冲位置*/
	int atomid;							/*已使用的ID*/

	fs_atom_node_t **hashtbl;
	fs_atom_node_t **nodes;             /*原子串指针数组*/
	int hashsize;
	int atomnum;
	int inited;

} fs_atom_table_t;

int fs_default_init_atom_table();
int fs_atom_get_string_atom(char *str);
int fs_atom_find_string_atom(char *str);
char *fs_atom_get_string_by_atom(int atom);

#endif
