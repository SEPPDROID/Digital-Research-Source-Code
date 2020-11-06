/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) getarhd.c - Sep 12, 1983  REGULUS 4.1
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */

#include <stdio.h>

#ifndef MC68000
#	include <c68/ar68.h>
#else
#	include <ar68.h>
#endif

/*
 * getarhd - fills the archive header structure from the buffer int
 *		the manner which will be understood on the current machine.
 */
int
getarhd(fp,arptr)				/* returns -1 for failure, 0 for success */
FILE *fp;
struct libhdr *arptr;
{
	register int i;
	register char *p, *lp;

	for (i = 0, lp = arptr->lfname; i < LIBNSIZE; i++)
		if ((*lp++ = getc(fp)) == EOF)
			return(-1);
	if ((lgetl(&arptr->lmodti,fp)) == -1)
		return(-1);
	if ((arptr->luserid = getc(fp)) == EOF)
		return(-1);
	if ((arptr->lgid = getc(fp)) == EOF)
		return(-1);
	if ((lgetw(&arptr->lfimode,fp)) == -1)
		return(-1);
	if ((lgetl(&arptr->lfsize,fp)) == -1)
		return(-1);
	return(0);
}

