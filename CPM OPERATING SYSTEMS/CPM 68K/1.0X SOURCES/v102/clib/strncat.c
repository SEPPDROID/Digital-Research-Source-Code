/****************************************************************************
*  STRNCAT  -  concatenate strings (limited)
*
*	BYTE *strncat(s1,s2,n)
*	BYTE *s1, *s2; WORD n;
*
*	'strncat' copies at most n bytes of s2 onto the end of s1.
*
*	Assumes null terminated strings. No check is made for string area
*		overflow.
****************************************************************************/

#include <portab.h>

BYTE *strncat(s1,s2,n)			/* copies at most n chars.	*/
REG	BYTE *s1, *s2;			/* 'n' unsigned: if n<0, we'll	*/
REG	WORD n;				/*  still decrement to 0.	*/
{
	REG BYTE *cp;

	for( cp=s1; *cp; cp++)		/* save s1 for return.		*/
		;
	while( *s2 && n-- > 0 )		/* copy until eos(s2) or n==0:	*/
		*cp++ = *s2++;		/*  no copy if n==0.		*/
	*cp = ZERO;			/* make sure string gets eos.	*/
	return(s1);
}
