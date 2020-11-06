/************************************************************************
*
*			p r i n t f   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"printf" prints args specified in format string to stdout.
*
*	Calling sequence:
*		nchrs = printf(fmt,arg1,arg2,...argn);
*	Where:
*		nchrs = # chars output by printf
*		fmt -> a string specifying how arg1-n are to be printed.
*
**************************************************************************/
#include <stdio.h>
WORD printf(fmt,args)
	BYTE *fmt,
	     *args;
{
	return(_doprt(stdout,fmt,&args));
}


