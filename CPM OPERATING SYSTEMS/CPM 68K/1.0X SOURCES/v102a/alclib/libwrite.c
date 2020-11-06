
/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#) libwrite.c - Sep 12, 1983  REGULUS 4.1
*/

#include "order.h"

lwritel(fd,str)
int fd;
long *str;
{
	register char *p;
	char junk[4];

	p = junk;
	*p++ = str->b1;
	*p++ = str->b2;
	*p++ = str->b3;
	*p   = str->b4;
	if (write(fd,junk,4) == 4)
		return(SUCCESS);
	else
		return(ERROR);
}

lwritew(fd,str)
int fd;
short *str;
{
	register char *p;
	char junk[2];

	p = junk;
	*p++ = str->wb1;
	*p   = str->wb2;
	if (write(fd,junk,2) == 2)
		return(SUCCESS);
	else
		return(ERROR);
}

lwritec(fd,str)
int fd;
char *str;
{
	if (write(fd,str,1) != 1)
		return(ERROR);
	return(SUCCESS);
}
