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

#include "stdio.h"

WORD	printf(fmt,args)			/* CLEAR FUNCTION ***********/
	BYTE *fmt,
	     *args;
{
	return(_doprt(stdout,fmt,&args));
}


/************************************************************************
*
*			f p r i n t f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fprintf" prints args specified in format string to a stream file.
*
*	Calling sequence:
*		nchrs = fprintf(fmt,arg1,arg2,...argn);
*	Where:
*		nchrs = number of chars output
*		fmt -> a string specifying how arg1-n are to be printed.
*
**************************************************************************/
WORD	fprintf(sp,fmt,args)			/* CLEAR FUNCTION ***********/
	FILE *sp;
	BYTE *fmt,
	     *args;
{
	return(_doprt(sp,fmt,&args));
}
