/*********************************************************************
*   STRRCHAR -  returns a pointer to last occurrence of char in string.
*
*	BYTE *strrchr(s,c)
*	BYTE *s, c;
*
*	Like 'strchr', only returns pointer to last c in s (instead of first),
*	  or zero if c not in s.
*
*	Edits:
*	1/84 whf	change from 'rindex()' to 'strchr()'
**********************************************************************/

#include "portab.h"

BYTE *	strrchr(str,ch)				/* CLEAR FUNCTION ***********/
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

BYTE *	rindex(str,ch)				/* CLEAR FUNCTION ***********/
REG	BYTE *str, ch;
{
	return strrchr(str,ch);		/* 'jacket' rtn for V7 compatability*/
}
