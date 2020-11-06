/* getpid - return process ID */
/*	returns phony number under CP/M */
#include <portab.h>
#define PHONYPID 222
WORD getpid()
{
	return(PHONYPID);
}
