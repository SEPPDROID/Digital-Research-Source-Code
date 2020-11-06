/*****************************************************************************
*
*			f p u t c   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fputc" does the inserting of a char into a stream buffer,
*	calling _flsbuf when the buffer fills up.
*	Returns the character put, or FAILURE (-1) if any errors.
*
*	Calling sequence:
*		ret=fputc(ch,s)
*	Where:
*		ch = the char to put
*		s -> the stream (FILE *)
*		ret= ch or FAILURE
*
*****************************************************************************/

#include "stdio.h"

WORD	fputc(ch,sp)				/* CLEAR FUNCTION ***********/
REG	BYTE ch;				/* char to put		    */
REG	FILE *sp;				/* stream to put it to	    */
{						/****************************/
	if( --(sp->_cnt) >= 0 )			/* if there's room in buf   */
		return(((WORD)(*sp->_ptr++ = ch)) & 0377);/*   put it!	    */
	else	return(_flsbuf(ch,sp));		/* o.w. flush & put	    */
						/****************************/
}


