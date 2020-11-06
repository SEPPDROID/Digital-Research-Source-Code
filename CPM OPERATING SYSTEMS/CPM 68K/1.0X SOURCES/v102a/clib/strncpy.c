/**********************************************************************
*   STRNCPY  -  copies at most n chars from one string to another
*
*	BYTE *strcpy(s1,s2,n)
*	BYTE *s1, *s2;
*	UWORD n;
*
*	Copies at most n bytes from s2 to s1, stopping after null 
*	  has been moved.
*	Truncates or null-pads s2, depending on n.
*	Returns s1.
*	No check for overflow of s1.
***********************************************************************/

#include <portab.h>

BYTE *	strncpy(s1,s2,num)			/* CLEAR FUNCTION ***********/
	BYTE *s1;
REG	BYTE *s2;
	WORD num;
{	REG BYTE *cp;

	for( cp=s1; num-- > 0 && (*cp = *s2); cp++,s2++ ) /* isn't C fun?   */
		;
	num++;				/* bump n back up.		*/
	while( num-- )			/* while #chars != 0		*/
		*cp++ = NULL;		/*   null pad.			*/
	return(s1);
}
