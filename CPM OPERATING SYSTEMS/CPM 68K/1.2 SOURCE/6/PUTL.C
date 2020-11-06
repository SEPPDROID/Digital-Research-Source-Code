/**************************************************************************
*
*			p u t l   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"putl" puts a LONG (4 byte) value into the output stream.
*	Note that what's written (binary) by a UNIX (lo byte, hi byte)
*	system (PDP 11) will NOT be compatible with what's written by a
*	NUXI (hi byte, lo byte) system (68K).
*
*	Calling sequence:
*		ret = putl(l, stream)
*	Where:
*		ret   = l or FAILURE on error
*		l     = an LONG value
*		stream-> a (FILE *) output stream
*
*****************************************************************************/

#include "stdio.h"

LONG	putl(lnum,sp)				/* CLEAR FUNCTION ***********/
	LONG lnum;				/* the LONG to be put	    */
	FILE *sp;				/* the stream to put to	    */
{						/*			    */
REG	WORD i;					/* index		    */
REG	BYTE *p;				/* make ptr to w	    */
						/****************************/
	p = (char *)&lnum;			/* point to l		    */
	for( i=0; i<4; i++ )			/* for all the 4 bytes in l */
	    if( putc(*p++,sp)== FAILURE )	/* put dem		    */
		return(FAILURE);		/*			    */
	return(lnum);				/* it worked		    */
}						/****************************/
FAILURE);		/*			    */
	return(lnum);				/* it worked		    */
}						/****************************/
FAILURE);		/*			    */
	return(lnum);				/* it worked		    */
}						/****************************/
