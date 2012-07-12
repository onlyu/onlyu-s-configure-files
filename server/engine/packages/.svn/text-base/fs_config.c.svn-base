// 系統配置管理文件

#include "fs_config.h"
#include "fs_rpc_lpc.h"

#include <assert.h>

// static mapping_t *config_map = NULL;

inline int get_int_from_map( mapping_t * map, char* key)
{
	svalue_t key_sv, *val_sv;
	
	key_sv.type = T_STRING;
	key_sv.subtype = STRING_CONSTANT;
	key_sv.u.string = make_shared_string(key);

	val_sv = find_in_mapping( map, &key_sv); 

	printf( "[%s]type = [%d] value = [%d]\n", key, val_sv->type, val_sv->u.number );

	if ( !IS_LPC_INT(val_sv) )
	{
		fprintf(stderr, "OS error: The config var [%s]'s type isn't INT .\n", key);
		return 0;
	}

	return val_sv->u.number;
}

inline char * get_string_from_map( mapping_t * map, char* key)
{
	svalue_t key_sv, *val_sv;
	
	key_sv.type = T_STRING;
	key_sv.subtype = STRING_CONSTANT;
	key_sv.u.string = make_shared_string(key);

	val_sv = find_in_mapping( map, &key_sv); 

	if ( !IS_LPC_STR(val_sv) )
	{
		fprintf(stderr, "OS error: The config var [%s]'s type isn't String .\n", key);
		return NULL;
	}

	return val_sv->u.string;
}

svalue_t *get_config_svalue()
{
	object_t *config_ob;
	int config_index;
	unsigned short type;
	svalue_t * config_sv = NULL; 

	config_ob = find_object(CONFIG_OBJECT);
	if (config_ob == NULL) 
	{
		fprintf(stderr, "OS error: The config file %s was not loaded.\n", CONFIG_OBJECT);
		return NULL;
	}

	// 取出config变量 
	config_index = find_global_variable(config_ob->prog, "config", &type);

	if ( config_index == -1)
	{
		fprintf(stderr, "OS error: The config file %s was loaded, but hasn't config var.\n", CONFIG_OBJECT);
		return NULL;
	}

	config_sv = &(config_ob->variables[config_index]);
	
	// 校验config类型
	if ( (config_sv->type & T_MAPPING) == 0)
	{
		fprintf(stderr, "OS error: The config file %s was loaded, but config var's type is error.\n", CONFIG_OBJECT);
		return NULL;
	}

	return config_sv;
}

#if 0
#define MAX_LINE_LENGTH 120

inline static int isnum(char *str)
{
	char c;
	int len;
	int i;
	
	len = strlen(str);

	i = 0;
	while (i < len) {
		c = str[i];
		if (c < '0' || c > '9') {
			return 0;
		}
		i++;
	}
	return 1;
}

void load_config_file(char *filename)
{
    char str[MAX_LINE_LENGTH * 4];
    int len, i, j;
    char key[MAX_LINE_LENGTH];
    char val[MAX_LINE_LENGTH];
	char *fmt = "%[^=]=%[^;]";
    char tmp[MAX_LINE_LENGTH];
	svalue_t *psval, skey;
    FILE *file;

    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: couldn't find or open config file: '%s'\n", filename);
        exit(-1);
    }

	config_map = allocate_mapping(0);

    while (1) {
        if (fgets(str, sizeof(str), file) == NULL)
            break;

		// 去除注释
        if (str[0] == '#' || str[0] == '/' || str[0] == '\n' || str[0] == ' ') {
			continue;
		}

        len = strlen(str); /* ACK! */

        if (len > MAX_LINE_LENGTH) {
            fprintf(stderr, "Error in config file: line too long:\n%s...\n", str);
            exit(-1);
        }

		memset(tmp, '\0', sizeof(tmp));
		// 去除空格
		i = 0;
		j = 0;
		while (i < len) {
			if (str[i] != ' ') {
				tmp[j++] = str[i];
			}
			i++;
		}

		if (sscanf(tmp, fmt, &key, &val) == 2) {

			TO_LPC_STR(skey, key); 
			// 字串
			if (val[0] == '"' && val[strlen(val) - 1] == '"') {
				val[strlen(val) - 1] = '\0';
				psval = find_for_insert(config_map, &skey, 1);
				set_svalue_shared_string(psval, val + 1);
			} else {
				// 数字
				if (isnum(val)) {
					psval = find_for_insert(config_map, &skey, 1);
					set_svalue_number(psval, atoi(val));
				} else {
					fprintf(stderr, "Error in config file: val not valid:%s, %s...\n", key, val);
				}
			}
		} else {
			fprintf(stderr, "Error in config file: line not valid: %s...\n", str);
		}
	}
    fclose(file);
}
#endif

char * fs_config_get_string(char *key)
{
	svalue_t * config_sv = get_config_svalue(); 
	if (config_sv == NULL) {
		return NULL;
	}
	return get_string_from_map(config_sv->u.map, key);
}

int fs_config_get_int(char *key)
{
	svalue_t * config_sv = get_config_svalue(); 
	if (config_sv == NULL) {
		return 0;
	}
	return get_int_from_map(config_sv->u.map, key);
}

int fs_config_get_host_hop(int hid, char **ip, unsigned *port) 
{
	svalue_t ini_key, hid_key, ip_key, hop_port_key;
	svalue_t * config_sv = get_config_svalue(); 

	set_svalue_shared_string(&ini_key, SERVER_INI); 
	svalue_t *ini = find_in_mapping(config_sv->u.map, &ini_key); 
	if (ini->type != T_MAPPING) {
		free_svalue(&ini_key, "server ini");
		fprintf(stderr, "error! no server ini\n");
		goto error;
	}
	free_svalue(&ini_key, "server ini");

	set_svalue_number(&hid_key, hid);
	svalue_t *host =  find_in_mapping(ini->u.map, &hid_key);
	if (host->type != T_MAPPING) {
		fprintf(stderr, "error! no server host.hid:%d\n", hid);
		goto error;
	}

	set_svalue_shared_string(&ip_key, "ip"); 
	set_svalue_shared_string(&hop_port_key, "hop_port"); 
	svalue_t *sip = find_in_mapping(host->u.map, &ip_key);
	svalue_t *sport = find_in_mapping(host->u.map, &hop_port_key);
	if (IS_LPC_UNDEF(sip) || IS_LPC_UNDEF(sport) || sip->type != T_STRING || sport->type != T_NUMBER) {
		fprintf(stderr, "error! host no ip or port.hid:%d\n", hid);
		free_svalue(&ip_key, "host ip");
		free_svalue(&hop_port_key, "host port");
		goto error;
	}

	*ip = sip->u.string;
	*port = sport->u.number;
	free_svalue(&ip_key, "host hop");
	free_svalue(&hop_port_key, "host hop");
	return 0;
error:
	*ip = NULL;
	*port = 0;
	return -1;
	
}
