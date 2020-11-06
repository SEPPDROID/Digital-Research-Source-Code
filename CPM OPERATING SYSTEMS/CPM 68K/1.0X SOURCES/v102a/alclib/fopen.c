/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)fopen.c	1.2    10/19/83
*/

#include <iobuf.h>

fopen(fname,ibuf)
char *fname;
register FILE *ibuf;
{
	ibuf->nunused = 0;	//no chars
	return(ibuf->fildes = open(fname,0));
}
