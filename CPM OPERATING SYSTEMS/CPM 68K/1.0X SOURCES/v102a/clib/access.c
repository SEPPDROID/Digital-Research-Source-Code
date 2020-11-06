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
*	Made O/S independent 20-Aug-83 sw
*	Added chmod & chown to this source module 16-Sep-83 whf
*
**************************************************************************/

#include "portab.h"
#include "osif.h"
#include "osiferr.h"
#include "errno.h"

WORD	access(fname,mode)			/* CLEAR FUNCTION ***********/
	BYTE *fname;
	WORD mode;
{
REG	WORD rval;

	if ((rval=open(fname,READ)) >= 0 )	/* File must be there ... */
	{
		close(rval);			/* Free up fd 		  */
		return(SUCCESS);
	}
	RETERR(FAILURE,ENOENT);
}


/***************************************/
/* chmod - change mode: NOP under CP/M */
/***************************************/
WORD	chmod(name,mode)			/* CLEAR FUNCTION ***********/
	BYTE *name;
	WORD mode;
{
	return(access(name,mode));
}


/**************************************************/
/* chown - change owner: like access() under CP/M */
/**************************************************/
WORD	chown(name,owner,group)			/* CLEAR FUNCTION ***********/
	BYTE *name;
	WORD owner;
	WORD group;
{
	return(access(name,0));
}
