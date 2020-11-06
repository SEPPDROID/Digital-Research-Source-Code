/*************************************************************************
*
*			a c c e s s   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"access" returns 0 if access to a file is allowed, -1 o.w.
*	Under CP/M, this just tests for existence.
*
*	Calling sequence:
*		ret = access(fname,mode)
*	Where:
*		ret = 0 if accessable, -1 o.w.
*		fname -> file's name, NULL terminated string
*		mode = test for read, write, exec, dir path access
*			(ignored by CP/M)
*
**************************************************************************/
#include <portab.h>
#include <cpm.h>
#include <errno.h>
WORD access(fname,mode)
REG	BYTE *fname;
REG	WORD mode;
{
REG	WORD ich;
REG	WORD rval;
REG	FD *fp;

	rval = FAILURE;
	if( (ich=allocc()) < 0)
		return(FAILURE);		/* can't access anything    */
	__chinit(ich);
	fp = _getccb(ich);
	if(__open(ich,fname,OPEN) == SUCCESS )
	{
		rval = SUCCESS;
		__BDOS(CLOSE,&(fp->fcb));
	}
	freec(ich);
	if( rval == SUCCESS )
		return(SUCCESS);
	else	RETERR(FAILURE,ENOENT);
}
