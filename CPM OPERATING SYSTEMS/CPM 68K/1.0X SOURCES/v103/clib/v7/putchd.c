/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) putchd.c - Sep 12, 1983  REGULUS 4.1
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */

#include <stdio.h>

#ifndef MC68000
#	include <c68/cout.h>
#else
#	include <cout.h>
#endif

/*
 * putchd - fills the buffer from the c.out header structure in
 *		the byte orientation of the target machine (68000).
 */
int
putchd(fp,arptr)		/* returns 0 for success, -1 for failure */
FILE *fp;
struct hdr2 *arptr;
{
	if (lputw(&arptr->ch_magic,fp) == -1)
		return(-1);
	if (lputl(&arptr->ch_tsize,fp) == -1)
		return(-1);
	if (lputl(&arptr->ch_dsize,fp) == -1)
		return(-1);
	if (lputl(&arptr->ch_bsize,fp) == -1)
		return(-1);
	if (lputl(&arptr->ch_ssize,fp) == -1)
		return(-1);
	if (lputl(&arptr->ch_stksize,fp) == -1)
		return(-1);
	if (lputl(&arptr->ch_entry,fp) == -1)
		return(-1);
	if (lputw(&arptr->ch_rlbflg,fp) == -1)
		return(-1);
	if (arptr->ch_magic == EX_ABMAGIC) {
		if (lputl(&arptr->ch_dstart,fp) == -1)
			return(-1);
		if (lputl(&arptr->ch_bstart,fp) == -1)
			return(-1);
	}
	return(0);
}

