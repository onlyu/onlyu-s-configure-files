#include "std.h"
#include "config.h"
#include "lpc_incl.h"
#include "main.h"


#define MAX_LINE_LENGTH 120

char *config_str[NUM_CONFIG_STRS];
int config_int[NUM_CONFIG_INTS];

static void config_init(void); /* don't ask */
static void config_init()
{
	int i;

	for (i = 0; i < NUM_CONFIG_INTS; i++)
	{
		config_int[i] = 0;
	}
	for (i = 0; i < NUM_CONFIG_STRS; i++)
	{
		config_str[i] = 0;
	}

}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

char *process_config_string(char *str)
{
	char *p = str;
	char *q;
	int n;

	while (*p && isspace(*p) )
	{
		p++;
	}
	n = strlen(p);
	if (!n)
	{
		return alloc_cstring("", "config_file: blank string");
	}

	q = p + n - 1;
	while (q > p && isspace(*q) )
	{
		q--;
	}
	q[1] = 0;
	return alloc_cstring(p, "process_config_string()");
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void set_defaults(char *filename)
{
	// ≥ı ºªØ≈‰÷√
	config_init();
}
