/**************************************************************************
*
*			p u t w   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"putw" puts a word (2 byte) value into the output stream.
*	Note that what's written (binary) by a UNIX (lo byte, hi byte)
*	system (PDP 11) will NOT be compatible with what's written by a
*	NUXI (hi byte, lo byte) system (68K).
*
*	Calling sequence:
*		ret = putw(w, stream)
*	Where:
*		ret   = w or FAILURE on error
*		w     = an int word value
*		stream-> a (FILE *) output stream
*
******************************************************************************/

#include "stdio.h"

WORD	putw(wrd,sp)				/* CLEAR FUNCTION ***********/
	int wrd;				/* the word to be put	    */
	FILE *sp;				/* the stream to put to	    */
{						/*			    */
REG	WORD i;					/* index		    */
REG	BYTE *p;				/* make ptr to w	    */
						/****************************/
	p = (char *)&wrd;			/* point to w		    */
	for( i=0; i<2; i++ )			/* for all the 2 bytes in w */
	    if( putc(*p++,sp)==FAILURE )	/* put dem		    */
		return(FAILURE);		/*			    */
	return(wrd);				/* it worked		    */
}						/****************************/
