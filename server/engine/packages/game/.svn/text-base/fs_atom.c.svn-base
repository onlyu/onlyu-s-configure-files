// Purpose  : 场景原子变量机制
// Created  : 2010-04-30
// By       : Mouse
// History  : 2010-04-30  Mouse create
//            2010-05-26  支持动态扩充原子表, 支持原子反查询字串
//
// 说明: 将一个字串存于池中，并返回一个原子编号，这个编号保持运行时唯一.
// 注意: 
//       1. 如果加入某一个类型的字串进来，需要考虑存储池是否足够.
//       2. 目前存储池没有做动态追加内存
//       3. 如果需要某一专属的原子串管理，如:gd跟db的通讯。需要在外部模块定义fs_atom_table_t变量.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "fs_atom.h"

// 字串的平均长度, 假定16个字节
#define STR_SIZE                16
#define ATOM_MAX_NUM            4800
#define ATOM_REALLOC_NUM        1024
#define ATOM_BUF_REALLOC_SIZE   (STR_SIZE * 1024)


fs_atom_table_t gSysAtomTable;

// s->string
#define StrHash(s)     (whashstr((s), 20))

// x->fs_atom_node_t
#define STR_ATOM(x)    (x->atom)
#define STR_POS(x)     (x->strpos)
#define STRING_LEN(x)  (x->strlen)
#define NEXT_NODE(x)   (x->next)

extern void debug_message (char * fmt, ...);

inline static fs_atom_node_t *fs_new_atom_node()
{
	return (fs_atom_node_t *) malloc(sizeof(fs_atom_node_t));
}

inline static void fs_free_atom_node(fs_atom_node_t *node)
{
	free(node);
}

inline static int fs_init_atom_table(fs_atom_table_t *at, int atom_num, int strsize)
{
	int bufsize = atom_num * strsize;
	int hashsize, i;

	hashsize = 1;
	for (; hashsize < atom_num; hashsize *= 2) ;

	at->atombuf = (char *) malloc(bufsize);
	at->bufsize = bufsize;
	at->bufpos = 0;
	at->atomid = 0;

	memset(at->atombuf, 0, bufsize);

	// hash init!
	at->hashtbl = (fs_atom_node_t **)malloc(hashsize * sizeof(fs_atom_node_t *));
	if (!at->hashtbl) {
		debug_message("\natom table can't malloc, Check it!!!");
		free(at->atombuf);
		return 0;
	}
	at->hashsize = hashsize;
	for (i = 0; i < hashsize; i++) {
		at->hashtbl[i] = NULL;
	}

	// nodes init!
	at->nodes = (fs_atom_node_t **)malloc(atom_num * sizeof(fs_atom_node_t *));

	at->atomnum = atom_num;
	for (i = 0; i < atom_num; i++) {
		at->nodes[i] = NULL;
	}

	at->inited = 1;
	return 1;
}

int fs_default_init_atom_table()
{
	return fs_init_atom_table(&gSysAtomTable, ATOM_MAX_NUM, STR_SIZE);
}

static fs_atom_node_t *_find_atom_node(fs_atom_table_t *at, char *s, int h, int badd)
{
	fs_atom_node_t *cur, *nn;

	if (!s) return NULL;

	h = h & (at->hashsize - 1);

	cur = at->hashtbl[h];

	while (cur) {
		if (strlen(s) == STRING_LEN(cur) \
				&& !strncmp(at->atombuf + STR_POS(cur), s, STRING_LEN(cur))) {
			return cur;
		}
		cur = NEXT_NODE(cur);
	}

	// not find! 
	// 添加
	if (badd) {
		if (strlen(s) >= (at->bufsize - at->bufpos) ) {
			//warning TODO
			//debug_message( "atom buff is used up, Check it!!");
			//return NULL;
			at->bufsize += ATOM_BUF_REALLOC_SIZE;
			at->atombuf = (char *)realloc(at->atombuf, at->bufsize);
		}

		nn = fs_new_atom_node();
		nn->strlen = strlen(s);
		nn->atom = ++(at->atomid);
		nn->strpos = at->bufpos;
		nn->next = NULL;
		strncpy(at->atombuf + at->bufpos, s, strlen(s));
		at->atombuf[at->bufpos + strlen(s)] = '\0';
		at->bufpos += strlen(s) + 1;

		if (!at->hashtbl[h]) {
			at->hashtbl[h] = nn;
		} else {
			nn->next = at->hashtbl[h];
			at->hashtbl[h] = nn;
		}

		if (at->atomid >= at->atomnum) {
			// realloc, warning
			at->atomnum += ATOM_REALLOC_NUM;
			at->nodes = (fs_atom_node_t **)realloc(at->nodes, sizeof(fs_atom_node_t *) * at->atomnum);
		}

		at->nodes[at->atomid-1] = nn;

		return nn;
	}
	return NULL;
}

int fs_atom_get_string_atom(char *str)
{
	fs_atom_node_t *nn = _find_atom_node(&gSysAtomTable, str, StrHash(str), 1);
	if (nn) {
		return STR_ATOM(nn);
	}
	return 0;
}

int fs_atom_find_string_atom(char *str)
{
	fs_atom_node_t *nn = _find_atom_node(&gSysAtomTable, str, StrHash(str), 0);
	if (nn) {
		return STR_ATOM(nn);
	}
	return 0;
}

char *fs_atom_get_string_by_atom(int atom)
{
	fs_atom_table_t *at = &gSysAtomTable;
	int pos;

	if (atom <= 0 || atom > at->atomnum) {
		// warning
		return "";
	}
	if (!at->nodes[atom-1]) {
		return "";
	}

	pos = at->nodes[atom-1]->strpos;

	return at->atombuf + pos;
}

// ==========================================
// test
// test cmd: gcc fs_atom.c ../src/hash.c -I../include/

#if 0
char *gen_str(int i)
{
#define BUF_LEN 32
	char *buf;
	buf = (char *) malloc(BUF_LEN);
	snprintf(buf, BUF_LEN, "test%d", i);
	return buf;
}

#define STR_REAL_NUM   15600
#define STR_INIT_NUM   128


int main()
{
	char *str;
	int i, atom;

	// fs_default_init_atom_table();

	fs_init_atom_table(&gSysAtomTable, STR_INIT_NUM, STR_SIZE);

	i = 1;

	printf( "insert atom:\n" );
	while( i <= STR_REAL_NUM ) {
		str = gen_str(i);
		atom = fs_atom_get_string_atom(str);
		printf( "%s:%d,", str, atom );
		i++;
	}

	printf( "find atom:\n" );

	i = 0;
	while( i <= STR_REAL_NUM ) {
		str = fs_atom_get_string_by_atom(i);
		printf( "%s:%d,", str, i);
		i++;
	}
	return 0;
}
#endif
