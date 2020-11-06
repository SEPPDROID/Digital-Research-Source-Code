/* chmod - change mode: NOP under CP/M */
#include <portab.h>
WORD chmod(name,mode)
	BYTE *name;
	WORD mode;
{
	return(access(name,mode));
}
