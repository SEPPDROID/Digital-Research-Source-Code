/***********************************************************************
*
*			g e t s   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"gets" reads a string from the standard input, and returns
*	its argument (NULL on end of file).  The newline at the
*	end of the string is replaced by a NULL.
*
*	Calling sequence:
*		addr = gets(saddr)
*	Where:
*		saddr -> where the string is to go (no bounds check)
*		addr  = saddr if all ok, NULLPTR o.w.
*
*****************************************************************************/

#include "stdio.h"

BYTE *	gets(str)				/* CLEAR FUNCTION ***********/
REG	BYTE *str;				/* string save area	    */
{						/****************************/
	REG WORD c;				/* char to test for eof	    */
	BYTE *sav;				/* sav pointer for return   */
	sav = str;				/* remember this	    */
	while( (c=getc(stdin)) != FAILURE  &&	/* while everything ok	    */
		c != '\n' )			/*   and not end of line    */
	    *str++ = c;				/*     store it		    */
	*str = NULL;				/* clean up string	    */
	if( c == FAILURE )			/* if not cool		    */
		return(NULLPTR);		/*   then tell them so	    */
	return(sav);				/* tell them is cool	    */
}						/****************************/
************/
************/
