/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) libget.c - Sep 12, 1983  REGULUS 4.1
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */

#include "order.h"

#include "iobuf.h"

#define dogetc(byte,i,fp) if((i=getc(fp))==-1) return(ERROR); else byte=(char)i

lgetl(lp,f)		/* returns -1 for failure, 0 for success */
long *lp;		/* 32 bits */
FILE *f;
{
	register int i;

	dogetc(lp->b1,i,f);
	dogetc(lp->b2,i,f);
	dogetc(lp->b3,i,f);
	dogetc(lp->b4,i,f);
	return(SUCCESS);
}

lgetw(lp,f)		/* returns -1 for failure, 0 for success */
short *lp;		/* 16 bits */
FILE *f;
{
	register int i;

	dogetc(lp->wb1,i,f);
	dogetc(lp->wb2,i,f);
	*lp &= 0xffff;
	return(SUCCESS);
}

