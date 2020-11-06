/*********************************************************************
*   INDEX  -  returns a pointer to first occurrence of char in string.
*
*	BYTE *index(s,c)
*	BYTE *s, c;
*
*	Returns pointer to first c in s, or zero if c not in s.
**********************************************************************/

#include <portab.h>

BYTE *	index(str,ch)				/* CLEAR FUNCTION ***********/
REG	BYTE *str, ch;
{
	for( ; ch != *str ; str++ )	/* look for c in s.		*/
		if( *str == NULL )	/* if we get to eos, we've gone */
			return(0);	/*   too far.			*/
	return(str);			/* found c. note that 'index'	*/
					/*   works to find NULL, (ie.	*/
					/*   eos), if c==NULL.		*/
}
ULL, (ie.	*/
					/*   eos), if c==NULL.		*/
}
ULL, (ie.	*/
					/*   eos), if c==NULL.		*/
}
