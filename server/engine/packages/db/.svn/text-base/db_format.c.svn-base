
#include "fs_db_format.h"
#include "lpc_args_op.h"
#include "fs_cksum.h"
#include "fs_log.h"
#include "lpc_incl.h"

#define DB_FORMAT_VERSION 1
#define META_BEGIN "#meta="
#define META_END "#meta_end\n"
#define META_FORMAT META_BEGIN"version:%u;sum:%u;\n"META_END
#define MAX_META_LEN 1024

/*******
注意！这里的格式化函数都必须是线程安全的！因为dbd用多线程来格式化数据并存盘.

*/

inline static int format(svalue_t *value, struct evbuffer *outbuf);

inline static int format_mapping(mapping_t *map, struct evbuffer *outbuf)
{
	int i;
	mapping_node_t *elt, **table;
	int pass_first = 0;

	evbuffer_add_printf(outbuf, "{");
	for (i = 0, table = map->table; i <= map->table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			if (pass_first) {
				evbuffer_add_printf(outbuf, ",");
			} else {
				pass_first = 1;
			}
			format(&elt->values[0], outbuf);
			evbuffer_add_printf(outbuf, ":");
			format(&elt->values[1], outbuf);
		}
	}
	evbuffer_add_printf(outbuf, "}");
	return 0;
}

inline static int format_array(array_t *array, struct evbuffer *outbuf)
{
	int i;

	evbuffer_add_printf(outbuf, "[");
	for (i = 0; i < array->size; ) {
		format(&array->item[i++], outbuf);
		if (i < array->size) {
			evbuffer_add_printf(outbuf, ",");
		}
	}
	evbuffer_add_printf(outbuf, "]");
	return 0;
}

/*
inline static int format_string_v0(char *str, struct evbuffer *outbuf)
{
	// 只对 \ 和 " 做转义, 不对\n \t \b \r \f \a \v \\ \' \" \?  和 \octal \x 做特别处理
        char c;
        evbuffer_add_printf(outbuf, "\"");
        for (; (c = *str++) != '\0'; ) { 
                if (c == '\\' || c == '"') {
                        evbuffer_add_printf(outbuf, "\\");
                }   
                evbuffer_add_printf(outbuf, "%c", c); 
        }   
        evbuffer_add_printf(outbuf, "\"");
        return 0;
}
*/

inline static int format_string(char *str, struct evbuffer *outbuf)
{
	char c;
	evbuffer_add_printf(outbuf, "\"");
	for (; (c = *str++) != '\0'; ) {
		switch (c) {
		case '"': evbuffer_add_printf(outbuf, "\\\""); break;
		case '\\': evbuffer_add_printf(outbuf, "\\\\"); break;
		case '\a': evbuffer_add_printf(outbuf, "\\a"); break;
		case '\b': evbuffer_add_printf(outbuf, "\\b"); break;
		case '\f': evbuffer_add_printf(outbuf, "\\f"); break;
		case '\n': evbuffer_add_printf(outbuf, "\\n"); break;
		case '\r': evbuffer_add_printf(outbuf, "\\r"); break;
		case '\t': evbuffer_add_printf(outbuf, "\\t"); break;
		case '\v': evbuffer_add_printf(outbuf, "\\v"); break;
		default:	
			/*为了支持显示中文，所以直接输出字符值而不能做转义 
			if (isprint((unsigned char)c))
				evbuffer_add_printf(outbuf, "%c", c);
			else
				evbuffer_add_printf(outbuf, "\\x%02x", (unsigned char)c);
			*/
			evbuffer_add_printf(outbuf, "%c", c);
		}
	}
	evbuffer_add_printf(outbuf, "\"");
	return 0;
}

inline static int format_number(int n, struct evbuffer *outbuf)
{
	evbuffer_add_printf(outbuf, "%d", n);
	return 0;
}

inline static int format_real(float f, struct evbuffer *outbuf)
{
	evbuffer_add_printf(outbuf, "%f", f);
	return 0;
}

inline static int format(svalue_t *value, struct evbuffer *outbuf)
{
	int type = value->type;
	//check undefine first
	switch (type) {
		case T_MAPPING:
			return format_mapping(value->u.map, outbuf);
		case T_ARRAY:
			return format_array(value->u.arr, outbuf);
		case T_REAL:
			return format_real(value->u.real, outbuf);
		case T_NUMBER:
			return format_number(value->u.number, outbuf);
		case T_STRING:
			return format_string(value->u.string, outbuf);
		default:
			//fprintf(stderr, "error! db format unknow type:%s\n", type_name(type));
			log_error_thr_safe(MOD_DB, "error! db format unknow type:%s\n", type_name(type));
			return -1;
	}
}

inline static int oneline_primitive_first_map(char *key, mapping_t *map, struct evbuffer *outbuf)
{
	int i;
	mapping_node_t *elt, **table;
	svalue_t *k, *v;
	struct evbuffer *comp = evbuffer_new();

	format_string(key, outbuf);
	evbuffer_add_printf(outbuf, ":{");

	for (i = 0, table = map->table; i <= map->table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			struct evbuffer *buf = NULL;
			k = &elt->values[0];
			v = &elt->values[1];

			switch (v->type) {
				case T_MAPPING:
				case T_ARRAY:
					buf = comp;
					break;
				default:
					buf = outbuf;
			}

			format(k, buf);
			evbuffer_add_printf(buf, ":");
			format(v, buf);
			evbuffer_add_printf(buf, ",");
		}
	}

	evbuffer_add(outbuf, EVBUFFER_DATA(comp), EVBUFFER_LENGTH(comp));
	evbuffer_add_printf(outbuf, "},\n");
	evbuffer_free(comp);
	return 0;
}

inline static int list_map(char *key, mapping_t *map, struct evbuffer *outbuf)
{
	int i;
	mapping_node_t *elt, **table;
	svalue_t *k, *v;

	format_string(key, outbuf);
	evbuffer_add_printf(outbuf, ":{\n");

	for (i = 0, table = map->table; i <= map->table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			k = &elt->values[0];
			v = &elt->values[1];
			evbuffer_add_printf(outbuf, "\t");
			format(k, outbuf);
			evbuffer_add_printf(outbuf, ":");
			format(v, outbuf);
			evbuffer_add_printf(outbuf, ",\n");
		}
	}

	evbuffer_add_printf(outbuf, "},\n\n");
	return 0;
}

inline static int oneline_map(char *key, mapping_t *map, struct evbuffer *outbuf)
{
	format_string(key, outbuf);
	evbuffer_add_printf(outbuf, ":");
	int ret = format_mapping(map, outbuf);
	evbuffer_add_printf(outbuf, ",\n\n");
	return ret;
}

inline static int format_prop(char *key, mapping_t *prop, struct evbuffer *outbuf)
{
	return oneline_map(key, prop, outbuf);
}

inline static int format_item(char *key, mapping_t *item, struct evbuffer *outbuf)
{
	return list_map(key, item, outbuf);
}

inline static int format_mission(char *key, mapping_t *mission, struct evbuffer *outbuf)
{
	return list_map(key, mission, outbuf);
}

inline static int format_summon(char *key, mapping_t *summon, struct evbuffer *outbuf)
{
	return list_map(key, summon, outbuf);
}

static int db_format_data_thr_safe(mapping_t *map, struct evbuffer *outbuf)
{
	int i, ret;
	mapping_node_t *elt, **table;
	svalue_t *k, *v;
	char *prop = "prop";
	char *item = "item";
	char *summon = "summon";
	char *mission = "mission";
	char *npc = "npc_data";
	char *share_data = "share_data";

	evbuffer_add_printf(outbuf, "data = {\n");
	for (i = 0, table = map->table; i <= map->table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			k = &elt->values[0];
			v = &elt->values[1];
			if (k->type != T_STRING) {
				//fprintf(stderr, "db format error!bad key type expect:%s,given:%s\n",
				//	type_name(T_STRING), type_name(k->type));
				log_error_thr_safe(MOD_DB, "db format error!bad key type expect:%s,given:%s",
					type_name(T_STRING), type_name(k->type));
				return -1;
			}
			if (v->type != T_MAPPING) {
				//fprintf(stderr, "db format error!bad value type expect:%s,given:%s\n",
				//	type_name(T_MAPPING), type_name(v->type));
				log_error_thr_safe(MOD_DB, "db format error!bad value type expect:%s,given:%s",
					type_name(T_MAPPING), type_name(v->type));
				return -1;
			}
			if (0 == strcmp(k->u.string, prop)) {
				ret = format_prop(prop, v->u.map, outbuf);
			} else if (0 == strcmp(k->u.string, item)) {
				ret = format_item(item, v->u.map, outbuf);
			} else if (0 == strcmp(k->u.string, summon)) {
				ret = format_summon(summon, v->u.map, outbuf);
			} else if (0 == strcmp(k->u.string, mission)) {
				ret = format_mission(mission, v->u.map, outbuf);
			} else if (0 == strcmp(k->u.string, npc)) {
				ret = list_map(npc, v->u.map, outbuf);
			} else if (0 == strcmp(k->u.string, share_data)) {
				ret = list_map(share_data, v->u.map, outbuf);
			} else {
				ret = oneline_map(k->u.string, v->u.map, outbuf);
			}

			if (ret) return ret;
		}
	}
	evbuffer_add_printf(outbuf, "}\n");

	return 0;
}

int fs_db_format_thr_safe(svalue_t *value, struct evbuffer *outbuf)
{
	uint32_t sum = 0;
	struct evbuffer *buff = evbuffer_new();

	if (value->type != T_MAPPING) {
		//fprintf(stderr, "db format error!bad type expect:%s,given:%s\n",type_name(T_MAPPING), type_name(value->type));
		log_error_thr_safe(MOD_DB, "db format error!bad type expect:%s,given:%s",type_name(T_MAPPING), type_name(value->type));
		goto error;
	}
	if (db_format_data_thr_safe(value->u.map, buff)) {
		goto error;
	}

	//for python header
	cksum(EVBUFFER_DATA(buff), EVBUFFER_LENGTH(buff), &sum);
	evbuffer_add_printf(outbuf, "#-*- coding: utf-8 -*-\n");
	evbuffer_add_printf(outbuf, META_FORMAT, DB_FORMAT_VERSION, sum);
	evbuffer_add(outbuf, EVBUFFER_DATA(buff), EVBUFFER_LENGTH(buff));
	evbuffer_free(buff);

	return 0;
error:
	evbuffer_free(buff);
	return -1;
}

int fs_db_format_(svalue_t *value, struct evbuffer *outbuf)
{
	char end = '\0';

	if (0 == format(value, outbuf)) {
		evbuffer_add(outbuf, &end, sizeof(end));
	}
	return 0;
}


#define FS_T_ARRAY_BEG '['
#define FS_T_ARRAY_END ']'
#define FS_T_MAP_BEG '{'
#define FS_T_MAP_END '}'
#define FS_T_MAP_PAIR_SEP ':'
#define FS_T_ELEMENT_SEP ','
#define FS_T_ASSIGN 	'='


#define FS_T_ID 	'i'
#define FS_T_INT 	'n'
#define FS_T_FLOAT 	'f'
#define FS_T_STR	's'		

#define FS_T_EOF	'\0'
#define FS_T_ERR 	'e'	
#define FS_T_START	'b'	


/* lex
INT = '-'* [0-9]+
FLOAT = '-'* [0-9]+ '.' [0-9]+
STRING = '"' .* '"'
ID = ('_' |[a-zA-Z0-9]) [_a-zA-Z0-9]* 
*/


typedef union val_u {
	char *str;
	int n;
	float f;
} val_t;

typedef struct token_s {
	int type;
	val_t val;
} token_t;

typedef struct scanner_s {
	unsigned version;
	unsigned line;
	unsigned line_pos;
	unsigned line_word;
	char *input;
	unsigned pos;
	token_t token;
	struct evbuffer *buf;
} scanner_t;



void static scanner_init(scanner_t *scan, unsigned version, char *input)
{
	scan->version = version;
	scan->input = input;
	scan->line = 1;
	scan->line_pos = 1;
	scan->line_word = 1;
	scan->pos = 0;
	scan->token.type = FS_T_START;
	scan->token.val.n = 0;
	scan->buf = evbuffer_new();
}

void static scanner_deinit(scanner_t *scan)
{
	evbuffer_free(scan->buf);
}

inline static void scanner_buf_reset(scanner_t *scan)
{
	evbuffer_drain(scan->buf, EVBUFFER_LENGTH(scan->buf));
}

inline static void scanner_buf_add(scanner_t *scan, char c)
{
	evbuffer_add(scan->buf, &c, sizeof(c));
}

inline static char *scanner_buf_data(scanner_t *scan)
{
	return (char*)EVBUFFER_DATA(scan->buf);
}


/*
static void scan_str_v0(scanner_t *scan, char sep)
{
	char c;
	int in_escape = 0;
	scanner_buf_reset(scan);
	//fprintf(stderr, "scan str v0\n");
	
	for (; (c = scan->input[scan->pos]) != '\0'; scan->pos++) {
		if (!in_escape) {
			if (c == '\\') {
				in_escape = 1;
			} else if (c == sep) { //字符串结束
				scanner_buf_add(scan, '\0');
				scan->pos++;
				scan->token.type = FS_T_STR;
				scan->token.val.str = scanner_buf_data(scan);
				return ;
			} else { //非转义的正常字符
				scanner_buf_add(scan, c);
			}
		} else {
			in_escape = 0;
			switch (c) {
			case 'x': {//只简单处理  \xab的情况
				int idx;
				char tmp[3];
				for (idx = 0, scan->pos++; idx < 2; idx++, scan->pos++) {
					c = scan->input[scan->pos];
					if (!isxdigit(c)) { //include '\0'
						goto error;
					}
					tmp[idx] = c;
				}
				tmp[idx] = '\0';
				c = (char)strtol(tmp, NULL, 16);
				//fprintf(stderr, "add digital:%02x, %c\n", c, c);
				scanner_buf_add(scan, c);
				scan->pos--;
				break;
			}
			default:
				scanner_buf_add(scan, c);
			}
		}
	}

error:
	scan->token.type = FS_T_ERR;
}
*/

int isodigit(char c)
{
	return '0' <= c && c < '8';
}

void scan_str_v1(scanner_t *scan, char del)
{
	char c;

	scanner_buf_reset(scan);
	for (; (c = scan->input[scan->pos]) != '\0'; scan->pos++) {
		if (c == del) { //字符串结束
			scanner_buf_add(scan, '\0');
			scan->pos++;
			scan->token.type = FS_T_STR;
			scan->token.val.str = scanner_buf_data(scan);
			return ;
		}
		switch (c) {
		case '\n':/* go through */
		case '\r':
			goto error;
		case '\\': {
			int n = 0, idx = 0;
			char tmp[4];
			c = scan->input[++scan->pos]; /* skip the `\' and next*/
			switch (c) {
			case '\0': goto error; /* EOF */ 
			case '\n':  /* go through */
			case '\r': //fprintf(stderr, "*** skip \\nr after \\\n"); 
				continue; /*todo line++; just skip the newline as "ab\ENTERcd" = "abcd" */
			case 'a': n = '\a'; break;
			case 'b': n = '\b'; break;
			case 'f': n = '\f'; break;
			case 'n': n = '\n'; break;
			case 'r': n = '\r'; break;
			case 't': n = '\t'; break;
			case 'v': n = '\v'; break;
			case 'x': { //处理  \xab的情况
				for (idx = 0, scan->pos++; idx < 2; idx++, scan->pos++) {
					c = scan->input[scan->pos];
					if (!isxdigit(c)) //include '\0'
						goto error;
					tmp[idx] = c;
				}
				scan->pos--;
				tmp[idx] = '\0';
				n = (char)strtol(tmp, NULL, 16);
				//fprintf(stderr, "add hex:%02x, %c\n", n, n);
				break;
			} 
			default: {
				if (isodigit(c)) {/* \123 */
					idx = 0;
					do {
						tmp[idx++] = c;
						c = scan->input[++scan->pos];
					} while (idx < 3 && isodigit(c));
					scan->pos--;
					tmp[idx] = '\0';
					n = (char)strtol(tmp, NULL, 8);
					//fprintf(stderr, "add oct:%03o, %c\n", n, n);
					if (n > UCHAR_MAX) {
						goto error;
					}
				} else {  
					n = c;  /* handles \\, \", \', and \? */
				}
			}
			} //switch \c
			//fprintf(stderr, "add backslash char:%d \n", n);
			scanner_buf_add(scan, n);
			break;
		} /*case '\\' */
		default://非转义的正常字符
			//fprintf(stderr, "add nornal char %c \n", c);
			scanner_buf_add(scan, c);
		} /* switch (c) */
	} //for 
error:
	scan->token.type = FS_T_ERR;
}

void scan_str(scanner_t *scan, char del)
{
	/*
	if (scan->version == 0) {
		scan_str_v0(scan, del);
	} else {
		scan_str_v1(scan, del);
	}
	*/
	scan_str_v1(scan, del);
}

inline static int is_id_begin(int c)
{
	return isalpha(c) || c == '_';
}

static void scan_identifier(scanner_t *scan)
{
	char c;
	scanner_buf_reset(scan);
	int is_start = 1;
	
	for (; (c = scan->input[scan->pos]) != '\0'; scan->pos++, scan->line_pos++) {
		//printf("scan id:%c\n", c);
		if (is_start) {
			is_start = 0;
			if (is_id_begin(c)) {
				scanner_buf_add(scan, c);
			} else {
				scan->token.type = FS_T_ERR;
				return;
			}
		} else {
			if (c == '_' || isalnum(c)) {
				scanner_buf_add(scan, c);
			} else {
				scanner_buf_add(scan, '\0');
				scan->token.type = FS_T_ID;
				scan->token.val.str = scanner_buf_data(scan);
				return;
			}
		}
	}
}


static void scan_numeric(scanner_t *scan)
{
	char c;
	int is_float = 0;
	token_t *token = &scan->token;
	char *start = &scan->input[scan->pos - 1];

	for (; (c = scan->input[scan->pos]) != '\0'; scan->pos++, scan->line_pos++) {
		switch(c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '.':
				if (!is_float) {
					is_float = 1;
					break;
				}
				//no break here!
			default: {
				char *end = &scan->input[scan->pos];
				char *expect;
				if (is_float) {
					token->type = FS_T_FLOAT;
					token->val.f = strtof(start , &expect);
					//fprintf(stderr, "scan float:%f\n", token->val.f);
				} else {
					token->type = FS_T_INT;
					token->val.n = (int)strtol(start, &expect, 10);
					//fprintf(stderr, "scan int:%d\n", token->val.n);
				}
				if (end != expect) {
					fprintf(stderr, "scan numeric error\n");
					token->type = FS_T_ERR;
				}
				return ;
			}
		} //switch
	} // for

	scan->token.type = FS_T_ERR;
}


static void eat_comment(scanner_t *scan)
{
	char c;
	for (; (c = scan->input[scan->pos]) != '\0'; scan->pos++, scan->line_pos++) {
		if (c == '\n')
			break;
	}
}

static token_t *next_token(scanner_t *scan)
{
	char c;
	token_t *token = &scan->token;

	for (; (c = scan->input[scan->pos++]) != '\0'; ) {
		//printf("check token char:%c,pos:%d\n", c, scan->pos);
		scan->line_pos++;
		switch (c) {
			case '#':
				eat_comment(scan);
				break;
			case '\n':
				scan->line++;
				scan->line_pos = 1;
				scan->line_word = 1;
			case ' ':
			case '\t':
				scan->line_word++;
				break;
			case '{':
				token->type = FS_T_MAP_BEG;
				return token;
			case '}':
				token->type = FS_T_MAP_END;
				return token;
			
			case '[':
				token->type = FS_T_ARRAY_BEG;
				return token;
			case ']':
				token->type = FS_T_ARRAY_END;
				return token;
			case ':':
				token->type = FS_T_MAP_PAIR_SEP;
				return token;
			case ',':
				token->type = FS_T_ELEMENT_SEP;
				return token;
			case '=':
				token->type = FS_T_ASSIGN;
				return token;
			case '\'':
				scan_str(scan, '\'');
				return token;
			case '"':
				scan_str(scan, '"');
				return token;
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				scan_numeric(scan);
				return token;
			default:
				if (is_id_begin(c)) {
					scan->pos--;
					scan_identifier(scan);
					return token;
				} else {
					token->type = FS_T_ERR;
					return token;
				}
		}
	}

	token->type = FS_T_EOF;
	return token;
}


/*
// BNF grammer
start = identity '=' (map|array)

map = '{' map_pairs '}'
map_pairs = NULL|map_pair ',' map_pairs | map_pair
map_pair = primitive ':' element

array = '[' array_element ']'
array_element = NULL|element ',' array_element|element

element = primitive|map|array
primitive = number|STRING
number = INT|FLOAT
*/

inline static int is_primitive(token_t *token)
{
	return token->type == FS_T_INT || token->type == FS_T_STR || token->type == FS_T_FLOAT;
}

inline static int is_element(token_t *token) 
{
	if (is_primitive(token))
		return 1;
	return token->type == FS_T_MAP_BEG || token->type == FS_T_ARRAY_BEG;
}

static int match(int type, scanner_t *scan, char *what)
{
	if (type == scan->token.type) {
		//fprintf(stderr, "ok! match %s token:'%c'\n", what, type);
		if (type != FS_T_EOF) {
			next_token(scan);
		}
		return 0;
	} else {
		if (what) {
			fprintf(stderr, "error! %s line:%d,word:%d,expect:'%c',given:'%c'\n", 
				what, scan->line, scan->line_word, type, scan->token.type);
		}
		return -1;
	}
}

static int gramm_primitive(scanner_t *scan, svalue_t *value)
{
	token_t *token = &scan->token;
	switch (token->type) {
		case FS_T_INT:
			set_svalue_number(value, token->val.n);
			match(FS_T_INT, scan, "int");
			return 0;
		case FS_T_FLOAT:
			set_svalue_real(value, token->val.f);
			match(FS_T_FLOAT, scan, "float");
			return 0;
		case FS_T_STR: {
			char * str = string_copy(token->val.str, "unform string");
			set_svalue_malloced_string(value, str);
			match(FS_T_STR, scan, "string");
			return 0;
		}
		default:
			fprintf(stderr, "error unknown primitive:'%c'\n", scan->token.type);
			return -1;
	}
}


int static gramm_array(scanner_t *scan, svalue_t *value);
static int gramm_map(scanner_t *scan, svalue_t *value);

static int gramm_element(scanner_t *scan, svalue_t *value)
{
	token_t *token = &scan->token;
	switch (token->type) {
		case FS_T_ARRAY_BEG:
			return gramm_array(scan, value);
		case FS_T_MAP_BEG:
			return gramm_map(scan, value);
		default:
			return gramm_primitive(scan, value);
	}
}

int static gramm_map_pairs(scanner_t *scan, mapping_t *map)
{
	svalue_t k, v;

	while (1) {
		if (is_primitive(&scan->token)) {
			if (gramm_primitive(scan, &k)) {
				return -1;
			}

			if (match(FS_T_MAP_PAIR_SEP, scan, "map pair seperator")) {
				free_svalue(&k, "gramm map key of pair\n");
				return -1;
			}

			if (gramm_element(scan, &v)) {
				free_svalue(&k, "gramm map key of pair\n");
				return -1;
			}

			if (match(FS_T_ELEMENT_SEP, scan, NULL)) {
				if (scan->token.type !=  FS_T_MAP_END) {
					free_svalue(&k, "gramm map key of pair\n");
					free_svalue(&v, "gramm map key of pair\n");
					return -1;
				}
			}

			svalue_t *iv = find_for_insert(map, &k, 0);
			assign_svalue(iv, &v);
			free_svalue(&k, "gramm map free");
			free_svalue(&v, "gramm map free");
		} else {
			return 0;
		}
	}
}


static int gramm_map(scanner_t *scan, svalue_t *value)
{

	if (match(FS_T_MAP_BEG, scan, "map begin")) {
		return -1;
	}

	mapping_t *map = allocate_mapping(0);
	if (gramm_map_pairs(scan, map)) {
		free_mapping(map);
		return -1;
	}

	if (match(FS_T_MAP_END, scan, "map end")) {
		free_mapping(map);
		return -1;
	}

	set_svalue_mapping(value, map);
	return 0;
}


static svalue_t *gramm_array_element(scanner_t *scan, size_t *size)
{
	size_t alloc_size = 4;
	*size = 0;
	svalue_t *elements = malloc(sizeof(svalue_t) * alloc_size);

	while (1) {
		if (is_element(&scan->token)) {
			//handle element
			if (gramm_element(scan, &elements[*size])) {
				//todo for each svalue free
				free(elements);
				return NULL;
			}

			(*size)++;
			if (*size >= alloc_size) {
				alloc_size *= 2;
				elements = realloc(elements, sizeof(svalue_t) * alloc_size);
				
			}

			//if (match(FS_T_ELEMENT_SEP, scan, "array element seperator")) {
			if (match(FS_T_ELEMENT_SEP, scan, NULL)) {
				if (FS_T_ARRAY_END == scan->token.type) {
					return elements;
				} else {
					//todo for each svalue free
					free(elements);
					return NULL;
				}
			}
		} else {
			return elements;
		}
	}
}

int static gramm_array(scanner_t *scan, svalue_t *value)
{
	svalue_t *elements;
	size_t size;

	if (match(FS_T_ARRAY_BEG, scan, "array begin")) {
		return -1;
	} 

	if (NULL == (elements = gramm_array_element(scan, &size))) {
		return -1;
	}

	if (match(FS_T_ARRAY_END, scan, "array end")) {
		free(elements);
		return -1;
	} 

	array_t *arr = allocate_empty_array(size);
	memcpy(arr->item, elements, size * sizeof(svalue_t));
	set_svalue_array(value, arr);


	free(elements);
	return 0;

}


static int gramm_start(scanner_t *scan, svalue_t *value)
{
	//data  
	if (match(FS_T_ID, scan, "start identifier")) {
		return -1;
	}

	// =
	if (match(FS_T_ASSIGN, scan, "start id assign")) {
		return -1;
	}
	//fprintf(stderr, "get assign:%c\n", token->type);

	switch (scan->token.type) {
		case FS_T_ARRAY_BEG:
			if (gramm_array(scan, value)) {
				return -1;
			}
			break;
		case FS_T_MAP_BEG: 
			if (gramm_map(scan, value)) {
				return -1;
			}
			break;
		default:
			fprintf(stderr, "gramm start unknow type:%c\n", scan->token.type);
			return -1;
	}

	if (match(FS_T_EOF, scan, "finish EOF")) {
		return -1;
	}

	return 0;
}

int fs_db_unformat(char *pathname, char *source, size_t len, svalue_t *value)
{
	scanner_t scan;

	unsigned version = 0;
	unsigned sum = 0,savesum = 0;
	char *meta_begin = NULL;
	char *meta_end = NULL;

	if (source == NULL || len == 0) {
		return -1;
	}

	meta_begin = strnstr(source, META_BEGIN, MAX_META_LEN);
	if (meta_begin) {
		meta_end = strnstr(meta_begin, META_END, MAX_META_LEN);
	}
	if (meta_begin && meta_end) {
		if (2 == sscanf(meta_begin, META_FORMAT, &version, &savesum)) {
			char *data = meta_end + strlen(META_END);
			cksum((unsigned char*)data, len - (data - source), &sum);
			if (sum != savesum) {
				log_error(MOD_DB, "unformat meta bad cksum version=%u,sum=%u,savesum=%u,pathname=%s", 
					version, sum, savesum, pathname);
			} else {
				//log_error(MOD_DB, "unformat meta ok version=%u,sum=%u,savesum=%u,pathname=%s", 
				//	version, sum, savesum, pathname);
			}
		} else {
			log_error(MOD_DB, "unformat malformed meta pathname=%s", pathname);
		}
	} else {
		log_warning(MOD_DB, "unformat no meta pathname=%s", pathname);
	}

	scanner_init(&scan, version, source);
	next_token(&scan);
	if (gramm_start(&scan, value)) {
		scanner_deinit(&scan);
		return -1;
	}

	scanner_deinit(&scan);
	return 0;
}

static int pystr_start(scanner_t *scan, svalue_t *value)
{
	switch (scan->token.type) {
		case FS_T_ARRAY_BEG:
			if (gramm_array(scan, value)) {
				return -1;
			}
			break;
		case FS_T_MAP_BEG: 
			if (gramm_map(scan, value)) {
				return -1;
			}
			break;
		default:
			fprintf(stderr, "gramm start unknow type:%c\n", scan->token.type);
			return -1;
	}

	if (match(FS_T_EOF, scan, "finish EOF")) {
		return -1;
	}

	return 0;
}


int fs_pystr2lpc(char *source, svalue_t *value)
{
	scanner_t scan;

	scanner_init(&scan, DB_FORMAT_VERSION, source);
	next_token(&scan);
	if (pystr_start(&scan, value)) {
		scanner_deinit(&scan);
		return -1;
	}

	scanner_deinit(&scan);
	return 0;
}


void f_pystr2lpc(void)
{
	int ret;
	svalue_t value;
	
	char *source = sp->u.string;
	ret = fs_pystr2lpc(source, &value);
	*sp = ret == 0 ? value : const0u;
}

void f_lpc2pystr(void)
{
	int ret;
	SET_ALL_ARGS;
	struct evbuffer *buf = evbuffer_new();
	ret = format(ARGS(0), buf);
	POP_ALL_ARGS;
	if (ret == 0) {
		char end = '\0';
		evbuffer_add(buf, &end, sizeof(end));
		char *p = string_copy((char*)EVBUFFER_DATA(buf), "db format");
		push_malloced_string(p);
	} else {
		push_undefined();
	}
	evbuffer_free(buf);
}

