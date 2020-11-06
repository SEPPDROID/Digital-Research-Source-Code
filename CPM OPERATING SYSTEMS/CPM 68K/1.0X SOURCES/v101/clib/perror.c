/***********************************************************************
*
*			p e r r o r   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"perror" produces a short error message on stderr describing
*	the last error encountered during a call to the system.
*	It assumes the variable "errno" contains this error.
*	Note: "errno" is set when errors occur, but is not cleared.
*	Note also that some of the UNIX errors are meaningless under 
*	CP/M, and will print an undefined message (defined below).
*	Finally, note that the defines in """errno.h""" will index 
*	appropriately into the sys_errlist table, below.
*
*	Calling sequence:
*		ret = perror(s)
*	Where:
*		ret = errno
*		s   -> a string containing a message printed with
*			the explanatory error message.
*
**************************************************************************/
#include <stdio.h>
#include <cpm.h>

#define NUMERRS 36
#define STDERR 2
MLOCAL BYTE _undeferr[] = "Error undefined on CP/M" ;
WORD sys_nerr = NUMERRS;

perror(s)
	BYTE *s;
{
	BYTE *err;
	BYTE lbuf[20], *buf, *_itoa();
	EXTERN BYTE *sys_errlist[];

	if( errno<0 || errno>=sys_nerr )
		err = _undeferr;
	else	err = sys_errlist[errno];
	write(STDERR,s,strlen(s));
	write(STDERR,": ",2);
	write(STDERR,err,strlen(err));
	write(STDERR," (",2);
	buf = _itoa(errno,lbuf);
	if( _errcpm != 0 )		/* assume this had something to do */
	{
		*buf++ = '.';
		buf = _itoa(_errcpm,buf);
	}
	*buf++ = ')'; *buf++ = '\n'; *buf = ZERO;
	write(STDERR,lbuf,strlen(lbuf));
	return(errno);
}

MLOCAL BYTE *_itoa(nm,bp)
	WORD nm;
	BYTE *bp;
{
	/* temporary... */
	sprintf(bp,"%d",nm);
	while(*bp) bp++;
	return(bp);
}


BYTE *sys_errlist[NUMERRS] = {
	_undeferr,			/* 0 */
	_undeferr,			/* 1 */
	"ENOENT No such file",		/* 2 */
	_undeferr,			/* 3 */
	_undeferr,			/* 4 */
	"EIO I/O error",		/* 5 */
	_undeferr,			/* 6 */
	"E2BIG Arg list too long",	/* 7 */
	_undeferr,			/* 8 */
	"EBADF Bad file number",	/* 9 */
	_undeferr,			/* 10 */
	_undeferr,			/* 11 */
	"ENOMEM Not enough core",	/* 12 */
	"EACCES Permission denied",	/* 13 */
	_undeferr,			/* 14 */
	_undeferr,			/* 15 */
	_undeferr,			/* 16 */
	_undeferr,			/* 17 */
	_undeferr,			/* 18 */
	_undeferr,			/* 19 */
	_undeferr,			/* 20 */
	_undeferr,			/* 21 */
	"EINVAL Invalid argument",	/* 22 */
	"ENFILE File table overflow",	/* 23 */
	"EMFILE Too many open files",	/* 24 */
	"ENOTTY Not a typewriter",	/* 25 */
	_undeferr,			/* 26 */
	"EFBIG File too big",		/* 27 */
	"ENOSPC No space left on device",/* 28 */
	_undeferr,			/* 29 */
	"EROFS Read-only file system",	/* 30 */
	_undeferr,			/* 31 */
	_undeferr,			/* 32 */
	_undeferr,			/* 33 */
	_undeferr,			/* 34 */
/* end of UNIX v7, start of CPM specific      */
	"ENODSPC No directory space"	/* 35 */
};
