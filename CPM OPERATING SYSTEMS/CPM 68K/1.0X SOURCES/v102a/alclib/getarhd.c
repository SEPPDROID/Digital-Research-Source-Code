/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */


/** mra-- put in code for our VAX */

#ifndef MC68000
#	include <c68/ar68.h>
#else
#	ifndef vax11c
#		include <ar68.h>
#	else
#		include "ar68.h"
#		define VAX11

		struct iob {
			int fd;
			int cc;
			char *cp;
			char cbuf[512];
		};
#	endif
#endif

/*
 * getarhd - fills the archive header structure from the buffer int
 *		the manner which will be understood on the current machine.
 */
int
getarhd(fp,arptr)				/* returns -1 for failure, 0 for success */
register struct iob *fp;
struct libhdr *arptr;
{
	register int i,j;
	char *lp;

	for (i = 0, lp = &(arptr->lfname); i < LNAMLEN; i++)
		if ((lp[i]=getc(fp)) == -1 )

			return(-1);
	if ((lgetl(&(arptr->lmodti),fp)) == -1)
		return(-1);
	if ((lp[(i++)+4] = getc(fp)) == -1 )
		return(-1);
	if ((lp[(i++)+4] = getc(fp)) == -1 )
		return(-1);
	if ((lgetw(&(arptr->lfimode),fp)) == -1)
		return(-1);
	if ((lgetl(&(arptr->lfsize),fp)) == -1)
		return(-1);
	return(0);
}
