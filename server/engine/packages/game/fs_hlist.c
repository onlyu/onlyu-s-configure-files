// ==========================================
// Purpose  : ��ϣ�������
// Created  : 2010-04-30
// By       : Mouse
// History  : 2010-04-30  Mouse create
// ==========================================
//
// hashlist�Ĺ���:
// 1.ͨ��id,�ܹ��ܿ���ҵ���Ӧ�Ķ���
// 2.˫���������
// 3.��Ҫ���Ƶط�: 
// 	1) hash������ý��,�˷ѿռ�
// 	2) hash���hash����,�ִ���int�����ֲ�ͬ��hash�㷨
// ==========================================

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include "fs_mem_pool.h"
#include "fs_hlist.h"
#include "hash.h"

extern void debug_message (char *, ...);

static int inited = 0;
static fs_memory_pool_t *gpHlistNodePool;					/*����������*/

// �޸�32λʱ��Ҫ����hash�㷨
#define POINTER_INT int64_t

#define COMBINE_INT(kt, kv) (((POINTER_INT)kt << 32) | (POINTER_INT)(kv))
// int
#define MAP_POINTER_HASH(x) (((POINTER_INT)(x) >> 4) ^ ((POINTER_INT)(x) & 0xFFFFFFF))

#define KEYHASH(kt, kv, m) (MAP_POINTER_HASH(COMBINE_INT(kt,kv)) & (m->hashsize -1))

#define _equal_hkey(k, t, v) (k->type== t && k->val == v)

inline static void fs_init_hlist_pool()
{
	gpHlistNodePool = fs_create_memory_pool(sizeof(fs_ht_node_t), MAX_HLIST_NODE_NUM);
	inited = 1;
}

inline static fs_ht_node_t *NEW_HT_NODE()
{
	if (!inited)  {
		fs_init_hlist_pool();
	}
	return fs_malloc_node(gpHlistNodePool);
}

inline static void FREE_HT_NODE(fs_ht_node_t *node)
{
	fs_free_node(gpHlistNodePool, node);
}

void fs_dump_hlist_node()
{
	fs_dump_mem_pool_desc(gpHlistNodePool, "hash table node");
}

inline static void _insert_list_node(fs_obj_hash_t *obh, fs_ht_node_t *node)
{
	// insert to list
	if (!obh->head) {
		node->lprev = &(obh->head);

		node->lnext = NULL;
		obh->head = node;
		obh->tail = node;

	} else {
		node->lnext = obh->head;
		node->lprev = &(obh->head);

		obh->head->lprev = &(node->lnext);
		obh->head = node;
	}
}

inline static void _remove_list_node(fs_obj_hash_t *obh, fs_ht_node_t *node)
{
	// ����ǽ�β��, ��lnext��Ϊ��
	if (obh->tail != node) {
		*(node->lprev) = node->lnext;
		node->lnext->lprev = node->lprev;
	} else {
		*(node->lprev) = NULL;
		// �����ͷ�ڵ�
		if (node->lprev == &(obh->head)) {
			obh->tail = NULL;
		} else {
			obh->tail = (fs_ht_node_t *)((void *)(node->lprev) - (void *)(&((fs_ht_node_t *)0)->lnext));
		}
	}
}


// ����key�ҵ�����
inline fs_ht_node_t * fs_ht_find_node(fs_obj_hash_t *obh, int h, int kt, int kv)
{
	fs_ht_node_t* p = obh->node_hash[h];
	fs_ht_key_t *pk;

	while (p!=NULL) {
		pk = &p->hkey;
		if (_equal_hkey(pk, kt, kv)) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}


// �������ob
inline static int fs_ht_insert_node(fs_obj_hash_t *obh, int h, int kt, int kv, void *ob)
{
	if ( !obh || fs_ht_find_node(obh, h, kt, kv) != NULL ) {
		// TODO: warning
		return 0;
	}

	fs_ht_node_t* p = NEW_HT_NODE();
	assert(p!=NULL);
	p->hkey.type = kt;
	p->hkey.val = kv;
	p->next = obh->node_hash[h] ;

	p->ob = ob ;
	obh->node_hash[h] = p ;
	obh->num ++;

	_insert_list_node(obh, p);

	return 1;
}

// ɾ������ڵ�
inline static void fs_ht_remove_node(fs_obj_hash_t *obh, int h, int kt, int kv)
{
	fs_ht_node_t *pre, *cur;
	fs_ht_key_t *pk;

	cur = obh->node_hash[h];
	pre = cur ;
	while(cur!=NULL){
		pk = &cur->hkey;
		if (_equal_hkey(pk, kt, kv)) {
			if ( cur == obh->node_hash[h] ) {
				obh->node_hash[h] = cur->next ;
			}else {
				pre->next = cur->next ;
			}

			obh->num--;
			_remove_list_node(obh, cur);
			FREE_HT_NODE(cur);
			break ;
		}else {
			pre = cur ;
			cur = cur->next;
		}
	}
}

// ����һ��hash��
fs_obj_hash_t *fs_ht_create_hash_table(int tblsize)
{
	int x, obj_hash_size;
	fs_obj_hash_t *obh;

	/* ensure that otable_size is a power of 2 */
	obj_hash_size = 1 ;
	for (; obj_hash_size < tblsize; obj_hash_size *= 2 ) ;

	obh = (fs_obj_hash_t *) malloc(sizeof(fs_obj_hash_t));
	obh->hashsize = obj_hash_size;
	obh->node_hash = (fs_ht_node_t**) malloc(obj_hash_size * sizeof(fs_ht_node_t *));
	if (!obh->node_hash) {
		debug_message( "\nerror ----------------, ht can't malloc");
	}

	obh->head = NULL;
	obh->tail = NULL;
	obh->blru = 0;
	obh->num = 0;
	for (x = 0; x < obj_hash_size; x++) {
		obh->node_hash[x] = NULL;
	}

	return obh;
}

// ����hlistΪһ�����ʹ�õ�����, ��ÿ����1��Ԫ�أ������Ԫ����������ͷ
// ���Ľ���ͻ��ǽ���ʹ�õ���������β
void fs_ht_set_lru_hlist(fs_obj_hash_t *obh)
{
	obh->blru = 1;
}

// �ͷ�һ��hash��
void fs_ht_free_hash_table(fs_obj_hash_t *obh)
{
	fs_ht_node_t* p ;
	fs_ht_node_t* cur;
	int i = 0 ;
	for (i=0; i<obh->hashsize; ++i){
		p = obh->node_hash[i] ;
		while (p!=NULL){
			cur = p ;
			p = p->next; 
			FREE_HT_NODE(cur);
		}
	}
	free(obh);
	free(obh->node_hash);
}

// ��һ��hash���ӡ����
void fs_ht_dump_hash_table(fs_obj_hash_t *obh)
{
	fs_ht_node_t* p ;
	fs_ht_node_t* cur;
	int i = 0 ;
	debug_message("\n ========================\n");
	debug_message("\n hash tbl: %d\n", obh->hashsize);
	for (i=0; i<obh->hashsize; ++i){
		p = obh->node_hash[i] ;
		if (p) debug_message("\n---%d :", i);
		while (p!=NULL){
			cur = p ;
			p = p->next; 
			debug_message( "\n------(%d,%d)", cur->hkey.type, cur->hkey.val);
		}
	}
	debug_message("\n ========================\n");
}

void *fs_ht_find_key(fs_obj_hash_t *obh, int kt, int kv)
{
	fs_ht_node_t *n = fs_ht_find_node(obh, KEYHASH(kt, kv, obh), kt, kv);

	if (n) {
		if (obh->blru) {
			_remove_list_node(obh, n);
			_insert_list_node(obh, n);
		}
		return n->ob;
	}

	return NULL;
}

int fs_ht_insert_key(fs_obj_hash_t *obh, int kt, int kv, void *ob)
{
	return fs_ht_insert_node(obh, KEYHASH(kt, kv, obh), kt, kv, ob);
}

void fs_ht_remove_key(fs_obj_hash_t *obh, int kt, int kv)
{
	return fs_ht_remove_node(obh, KEYHASH(kt, kv, obh), kt, kv);
}

// ===================================================
// ���·�����������lru������ʵ�hash�Ż�
//
// 1. ��hash����������ʱ,�����Ƴ�hash����������Ԫ��
// 2. 1��ǰ���Ǳ�֤ÿ�η���hash��֮��, ���ڵ��ƶ���ͷ��

void *fs_ht_tail_node_ob(fs_obj_hash_t *obh)
{
	if (obh->tail) {
		return obh->tail->ob;
	}
	return NULL;
}

// �Ƴ�����һ��Ԫ��
void fs_ht_remove_tail_node(fs_obj_hash_t *obh)
{
	int kt, kv;
	if (obh->tail) {
		kt = obh->tail->hkey.type;
		kv = obh->tail->hkey.val;
		fs_ht_remove_node(obh, KEYHASH(kt, kv, obh), kt, kv);
	}
}

// ��hash�������һ������ƶ���ͷ
// ���Բ��ö�hash��
void fs_ht_node_move_2_head(fs_obj_hash_t *obh, fs_ht_node_t *node)
{

}
