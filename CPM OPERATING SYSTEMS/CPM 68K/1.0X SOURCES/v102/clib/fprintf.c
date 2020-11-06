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
#include <stdio.h>
WORD fprintf(stream,fmt,args)
	FILE *stream;
	BYTE *fmt,
	     *args;
{
	return(_doprt(stream,fmt,&args));
}
