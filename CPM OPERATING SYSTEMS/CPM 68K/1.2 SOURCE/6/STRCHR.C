/*********************************************************************
*   STRCHR -  returns a pointer to first occurrence of char in string.
*	(formerly known as "index()")
*
*	BYTE *strchr(s,c)
*	BYTE *s, c;
*
*	Returns pointer to first c in s, or zero if c not in s.
*
*	Edits:
*	1/84 whf	changed from 'index()' to 'strchr()'
**********************************************************************/

#include "portab.h"

BYTE *	strchr(str,ch)				/* CLEAR FUNCTION ***********/
REG	BYTE *str, ch;
{
	for( ; ch != *str ; str++ )	/* look for c in s.		*/
		if( *str == NULL )	/* if we get to eos, we've gone */
			return(0);	/*   too far.			*/
	return(str);			/* found c. note that 'index'	*/
					/*   works to find NULL, (ie.	*/
					/*   eos), if c==NULL.		*/
}

BYTE *	index(str,ch)				/* CLEAR FUNCTION ***********/
REG	BYTE *str, ch;
{
	return strchr(str,ch);		/* index is jacket rtn for V7...    */
}
index is jacket rtn for V7...    */
}
index is jacket rtn for V7...    */
}
