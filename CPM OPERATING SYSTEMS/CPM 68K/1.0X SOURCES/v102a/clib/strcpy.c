/**********************************************************************
*   STRCPY  -  copies from one string to another
*
*	BYTE *strcpy(s1,s2)
*	BYTE *s1, *s2;
*
*	Copies bytes from s2 to s1, stopping after null has been moved.
*	Returns s1.
*	No check for overflow of s1.
***********************************************************************/

#include <portab.h>

BYTE *	strcpy(s1,s2)				/* CLEAR FUNCTION ***********/
	BYTE *s1;
REG	BYTE *s2;
{	REG BYTE *cp;

	cp = s1;			/* save for return.		*/
	while( *cp++ = *s2++ ) 		/* isn't C fun?			*/
		;
	return(s1);
}

