/***********************************************************************
*
*			f g e t s   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fgets" reads a string from a stream file (up to n-1 chars), and 
*	returns pointer s (NULL on end of file).  The newline at the
*	end of line is included, NOT replaced, and the string is terminated
*	by a NULL.
*
*	Calling sequence:
*		addr = fgets(saddr,n,stream)
*	Where:
*		saddr -> where the string is to go (no bounds check)
*		n     = max # chars (inc. NULL) for saddr (assumed > 0)
*		stream-> where to get from
*		addr  = saddr if all ok, NULL o.w.
*
*****************************************************************************/
#include <stdio.h>
BYTE *fgets(s,n,f)				/****************************/
REG	BYTE *s;				/* string save area	    */
REG	WORD n;					/* max size of string	    */
REG	FILE *f;				/* where to get from	    */
{						/****************************/
	REG WORD c;				/* char to test for eof	    */
	REG BYTE *sav;				/* sav pointer for return   */
	sav = s;				/* remember this	    */
	while( --n > 0  &&			/* while there's still room */
						/*   for getc and NULL	    */
		(c=getc(f)) != FAILURE )	/*   and read_char ok	    */
	{					/*			    */
	    *s++ = c;				/*   store it		    */
	    if( c == '\n' )			/*   if end of line	    */
		break;				/*     stop the presses	    */
	}					/*			    */
	*s = ZERO;				/* clean up string	    */
	if( c== FAILURE )			/* if not cool		    */
	        return(NULL);			/*   then tell them so	    */
	else	return(sav);			/* tell them is cool	    */
}						/****************************/
