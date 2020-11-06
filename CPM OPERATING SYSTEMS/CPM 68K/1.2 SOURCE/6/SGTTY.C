/*************************************************************************
*
*			stty / gtty   Functions
*			-----------------------
*	Copyright 1983 by Digital Reserach Inc.  All rights reserved.
*
*	"stty(fd,argp)" sets the bits for CON: file opened on 'fd'
*	"gtty(fd,argp)" gets the bits for CON: file opened on 'fd'
*
*	Where:
*		fd  = file descriptor returned by 'open'
*		argp pts to struct 'sgttyb' (see "sgtty.h")
*
**************************************************************************/
#include "portab.h"
#include "osif.h"
#include "sgtty.h"

VOID	stty(fd,argp)				/* CLEAR FUNCTION ***********/
    WORD fd;					/* opened file descriptor */
    struct sgttyb *argp;			/* ptr to control info */
{
	REG FD *fp; FD *_chkc();

	if( (fp=_chkc(fd)) == NULLFD  ||
	    (fp->flags & ISTTY) == 0  )
		return -1;
	blkmove(argp,&(fp->fcb),sizeof(*argp));	/* no checks: do whatever */
	return 0;
}


VOID	gtty(fd,argp)				/* CLEAR FUNCTION ***********/
    WORD fd;					/* opened file descriptor */
    struct sgttyb *argp;			/* ptr to control info */
{
	REG FD *fp; FD *_chkc();

	if( (fp=_chkc(fd)) == NULLFD  ||
	    (fp->flags & ISTTY) == 0  )
		return -1;
	blkmove(&(fp->fcb),argp,sizeof(*argp));	/* no checks: do whatever */
	return 0;
}
turn 0;
}
turn 0;
}
