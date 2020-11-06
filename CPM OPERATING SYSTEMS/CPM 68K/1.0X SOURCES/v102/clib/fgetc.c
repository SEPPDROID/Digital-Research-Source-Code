/**************************************************************************
*
*			f g e t c   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Function "fgetc" does the yanking of a char out of the stream.
*
*	Calling sequence:
*		ch = fgetc(s)
*	Where:
*		ch = a (WORD) character (-1 on EOF)
*		s -> a stream file (FILE *)
*
*****************************************************************************/
#include <stdio.h>
WORD fgetc(s)					/****************************/
REG	FILE *s;				/* stream pointer	    */
{						/****************************/
	EXTERN WORD _filbuf();			/* use this when we run shrt*/
	if( --s->_cnt >= 0 )			/* any chars left in buf?   */
	    return(((WORD) *s->_ptr++) & 0377);	/* return & advance ptr ifso*/
	else					/*			    */
	    return( _filbuf(s) );		/* o.w. whatever happens    */
}						/****************************/
