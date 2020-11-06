/**************************************************************************
*
*			g e t l   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"getl" gets an LONG (4 byte) value from the input stream.
*	Note that what's written (binary) by a PDP 11 (UNIX: lo byte, hi byte)
*	system will NOT be compatible with what's written by a
*	68K (NUXI: hi byte, lo byte) system.
*
*	Calling sequence:
*		l = getl(stream)
*	Where:
*		l     = a 32-bit long value
*		stream-> a (FILE *) input stream
*
*****************************************************************************/

#include "stdio.h"

LONG	getl(sp)				/* CLEAR FUNCTION ***********/
REG	FILE *sp;				/* the stream to get from   */
{						/*			    */
	LONG l;					/* place to get to	    */
REG	BYTE *p;				/* make ptr to l	    */
						/****************************/
	p = (char *)&l;				/* point to l		    */
	*p++ = getc(sp);			/* no way to distinguish    */
	*p++ = getc(sp);			/*  twixt EOF & -1	    */
	*p++ = getc(sp);			/*			    */
	*p++ = getc(sp);			/*			    */
	return(l);				/* there it is		    */
}						/****************************/
 */
}						/****************************/
 */
}						/****************************/
