/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) libput.c - Sep 12, 1983  REGULUS 4.1
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */


#include "order.h"
#include <stdio.h>

#define doputc(byte,fp) putc(byte,fp)

lputl(lp,f)			/* returns 0 for success, -1 for failure */
long *lp;			/* 32 bits */
FILE *f;
{
	doputc(lp->b1,f);
	doputc(lp->b2,f);
	doputc(lp->b3,f);
	doputc(lp->b4,f);
	return((ferror(f)) ? ERROR : SUCCESS);
}

lputw(lp,f)			/* returns 0 for success, -1 for failure */
short *lp;			/* 16 bits */
FILE *f;
{
	doputc(lp->wb1,f);
	doputc(lp->wb2,f);
	return((ferror(f)) ? ERROR : SUCCESS);
}

