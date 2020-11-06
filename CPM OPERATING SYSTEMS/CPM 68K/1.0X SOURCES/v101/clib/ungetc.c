/**************************************************************************
*
*			u n g e t c   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"ungetc" puts a char back into its (read only) stream.  It's
*	guaranteed to work for at least one char, provided the
*	stream is buffered and something has been read.
*
*	Calling sequence:
*		ret = ungetc(c,stream)
*	Where:
*		ret = c if it worked, FAILURE o.w.
*		c   = char to push back
*		stream -> (FILE *) place to push back to
*
*****************************************************************************/
#include <stdio.h>
WORD ungetc(c,sp)				/****************************/
REG	WORD c;					/* char to unget	    */
REG	FILE *sp;				/* stream to unget to	    */
{						/****************************/
	if( c == EOF )				/* no allowed if EOF	    */
		return(FAILURE);		/*			    */
	if( sp->_flag & _IOREAD			/* if file is read able	    */
	    && sp->_base != NULL  		/*   and something's read   */
	    && sp->_ptr > sp->_base )		/*   and buffer not empty   */
	{					/* then...		    */
		*--sp->_ptr = c;		/* back up buffer & store   */
		sp->_cnt++;			/* there's one more byte    */
		return(c);			/* it worked!		    */
	}					/****************************/
	return(FAILURE);			/* it didn't work...	    */
}						/****************************/
