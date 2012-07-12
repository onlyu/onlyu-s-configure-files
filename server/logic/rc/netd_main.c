

#include <system_config.h>

void main()
{
	"rc/globals"->__init__("/etc/netd_globals"); // 必须存在，请不要删掉
	debug_message( sprintf("loading netd MASTER ... Done.") );
}

void create()
{
}

