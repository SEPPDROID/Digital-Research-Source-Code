/*************************************************************************
*
*			Channel Info Functions
*			----------------------
*	Copyright 1982,83 by Digital Reserach Inc.  All rights reserved.
*
*	"isatty(fd)" == TRUE iff the file on channel fd is a 'CON:' device.
*	"isdev(fd)" == TRUE iff fd is 'CON:', 'LST:', 'AUX:'
*
*	Where:
*		fd  = file descriptor returned by 'open'
*
**************************************************************************/
#include "portab.h"
#include "osif.h"

BOOLEAN	isatty(fd)				/* CLEAR FUNCTION ***********/
	WORD fd;				/* file des returned by open*/
{						/****************************/
	REG FD *fp;				/* ptr to ccb		    */
	FD *_chkc();				/* validates fd, cvt to fp  */
						/*			    */
	if((fp=_chkc(fd)) == NULLFD)		/* make sure its open  MGL  */
		return(FALSE);			/*   isno TTY ifnot open    */
	return( (fp->flags & ISTTY) != 0 );	/* test this flag	    */
}						/****************************/

BOOLEAN	isdev(fd)				/* CLEAR FUNCTION ***********/
	WORD fd;				/* file des returned by open*/
{						/****************************/
	REG FD *fp;				/* ptr to ccb		    */
	FD *_chkc();				/* validates fd, cvt to fp  */
						/*			    */
	if((fp=_chkc(fd)) == NULLFD)		/* make sure its open  MGL  */
		return(FALSE);			/*   isno TTY ifnot open    */
	return(fp->flags&(ISTTY|ISLPT|ISAUX));	/* test these flags	    */
}						/****************************/


/* ttyname - find name of a terminal */
/*	returns "CON:" if isatty, NULLPTR o.w., under CP/M */

EXTERN	BYTE	__tname[];			/* CON: String		*/
BYTE *	ttyname(fd)				/* CLEAR FUNCTION ***********/
	WORD fd;
{
	if( isatty(fd) )
		return(__tname);
	return(NULLPTR);
}
satty(fd) )
		return(__tname);
	return(NULLPTR);
}
satty(fd) )
		return(__tname);
	return(NULLPTR);
}
