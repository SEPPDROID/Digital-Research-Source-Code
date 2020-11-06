/**********************************************************************
*    STRLEN  -  finds the number of non-null characters in s.
*
*	WORD strlen(s)
*	BYTE *s;
**********************************************************************/

#include <portab.h>

WORD strlen(s)
REG	BYTE *s;
{
REG	BYTE *p;
	for( p = s; *p; p++ )		/* advance *p until NULL.	*/
		;
	return(p-s);
}

