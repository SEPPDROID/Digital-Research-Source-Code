/**********************************************************************
*    STRLEN  -  finds the number of non-null characters in s.
*
*	WORD strlen(s)
*	BYTE *s;
**********************************************************************/

#include "portab.h"

WORD	strlen(str)				/* CLEAR FUNCTION ***********/
REG	BYTE *str;
{
REG	BYTE *p;
	for( p = str; *p; p++ )		/* advance *p until NULL.	*/
		;
	return((WORD)p-str);
}

nce *p until NULL.	*/
		;
	return((WORD)p-str);
}

nce *p until NULL.	*/
		;
	return((WORD)p-str);
}

