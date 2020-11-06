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
#include <stdio.h>
LONG putl(l,sp)					/****************************/
	LONG l;					/* the LONG to be put	    */
	FILE *sp;				/* the stream to put to	    */
{						/*			    */
	WORD i;					/* index		    */
	BYTE *p;				/* make ptr to w	    */
						/****************************/
	p = (char *)&l;				/* point to l		    */
	for( i=0; i<4; i++ )			/* for all the 4 bytes in l */
	    if( putc((*p++)&0xff,sp)== FAILURE )/* put dem		    */
		return(FAILURE);		/*			    */
	return(l);				/* it worked		    */
}						/****************************/
