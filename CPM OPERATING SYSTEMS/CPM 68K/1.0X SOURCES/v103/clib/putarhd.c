/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) putarhd.c - Sep 12, 1983  REGULUS 4.1
*/


/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */

#include "ar68.h"
#include "iobuf.h"

/*
 * putarhd - fills the buffer from the archive header structure in
 *		the byte orientation of the target machine (68000).
 */
int
putarhd(fp,arptr)		/* returns 0 for success, -1 for error */
FILE *fp;
struct libhdr *arptr;
{
	register int i;
	register char *p, *lp;

	for (i=0, lp = arptr->lfname; i<LIBNSIZE; i++, lp++)
		if (putc(*lp,fp) == -1)
			return(-1);
	if (lputl(&arptr->lmodti,fp) == -1)
		return(-1);
	if (putc(arptr->luserid,fp) == -1)
		return(-1);
	if (putc(arptr->lgid,fp) == -1)
		return(-1);
	if (lputw(&arptr->lfimode,fp) == -1)
		return(-1);
	if (lputl(&arptr->lfsize,fp) == -1)
		return(-1);
	return(0);
}

