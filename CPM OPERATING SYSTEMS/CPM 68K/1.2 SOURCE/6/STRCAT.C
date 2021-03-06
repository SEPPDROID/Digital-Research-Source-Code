/****************************************************************************
*    STRCAT  -  concatenate strings
*
*	BYTE *strcat(s1,s2)	copies s2 to end of s1
*	BYTE *s1, *s2;
*
*	Assumes null terminated strings. No check is made for string area
*		overflow.
****************************************************************************/

#include <portab.h>

BYTE *	strcat(s1,s2)				/* CLEAR FUNCTION ***********/
	BYTE *s1;
REG	BYTE *s2;
{
REG	BYTE *cp;

	for( cp=s1; *cp; cp++ )		/* save s1 for return.		*/
		;
	while( (*cp++ = *s2++) )	/* copy until eos(s2).		*/
		;
	return(s1);
}

for return.		*/
		;
	while( (*cp++ = *s2++) )	/* copy until eos(s2).		*/
		;
	return(s1);
}

for return.		*/
		;
	while( (*cp++ = *s2++) )	/* copy until eos(s2).		*/
		;
	return(s1);
}

