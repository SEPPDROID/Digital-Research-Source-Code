/*********************************************************************
*   RINDEX  -  returns a pointer to last occurrence of char in string.
*
*	BYTE *rindex(s,c)
*	BYTE *s, c;
*
*	Like 'index', only returns pointer to last c in s (instead of first),
*	  or zero if c not in s.
**********************************************************************/

#include <portab.h>

BYTE *	rindex(str,ch)				/* CLEAR FUNCTION ***********/
REG	BYTE *str, ch;
{
	REG BYTE *t;

	for( t=str; *t; t++ )		/* look for eos.		*/
		;
	for( ; ch != *t ; t-- )		/* look for c in s.		*/
		if( t==str )		/* if we get to start of string,*/
			return(NULL);	/*   too far.			*/
	return(t);			/* found c. note that 'rindex'	*/
					/*   works (ie. returns eos)	*/
					/*   if c==NULL.		*/
}
 if c==NULL.		*/
}
 if c==NULL.		*/
}
