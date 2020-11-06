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

#include "stdio.h"
#define CMASK 0xFF

WORD	fgetc(sp)				/* CLEAR FUNCTION ***********/
REG	FILE *sp;				/* stream pointer	    */
{						/****************************/
	EXTERN WORD _filbuf();			/* use this when we run shrt*/
	if( --sp->_cnt >= 0 )			/* any chars left in buf?   */
	    return(((WORD) *sp->_ptr++) & CMASK);/* return & advance ptr ifso*/
	else					/*			    */
	    return( _filbuf(sp) );		/* o.w. whatever happens    */
}						/****************************/
p) );		/* o.w. whatever happens    */
}						/****************************/
p) );		/* o.w. whatever happens    */
}						/****************************/
