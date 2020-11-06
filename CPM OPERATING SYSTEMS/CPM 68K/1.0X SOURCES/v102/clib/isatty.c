/*************************************************************************
*
*	i s a t t y   F u n c t i o n
*	-----------------------------
*	Copyright 1982 by Digital Reserach Inc.  All rights reserved.
*
*	"isatty" returns TRUE iff the file on channel fd is a 'CON:'
*	device.
*	Calling sequence:
*		ret = isatty(fd)
*	Where:
*		fd  = file descriptor returned by 'open'
*		ret = TRUE iff the file is a 'CON:'
*
**************************************************************************/
#include <portab.h>
#include <cpm.h>
BOOLEAN isatty(fd)				/****************************/
	WORD fd;				/* file des returned by open*/
{						/****************************/
	REG FD *fp;				/* ptr to ccb		    */
	FD *_chkc();				/* validates fd, cvt to fp  */
	if((fp=_chkc(fd)) == NULL)		/* make sure its open  MGL  */
		return(FALSE);			/*   isno TTY ifnot open    */
	return( (fp->flags & ISTTY) != 0 );	/* test this flag	    */
}						/****************************/
