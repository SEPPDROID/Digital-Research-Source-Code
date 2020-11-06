/* ttyname - find name of a terminal */
/*	returns "CON:" if isatty, NULL o.w., under CP/M */
#include <portab.h>
#include <cpm.h>
EXTERN	BYTE	__tname[];		/* CON: String		*/
BYTE *ttyname(fd)
	WORD fd;
{
	if( isatty(fd) )
		return(__tname);
	else	return((BYTE *)NULL);
}
