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

BYTE *rindex(s,c)
	BYTE *s, c;
{	REG BYTE *t;

	for( t=s; *t; t++ )		/* look for eos.		*/
		;
	for( ; c != *t ; t-- )		/* look for c in s.		*/
		if( t==s )		/* if we get to start of string,*/
			return(0);	/*   too far.			*/
	return(t);			/* found c. note that 'rindex'	*/
					/*   works (ie. returns eos)	*/
					/*   if c==NULL.		*/
}
