
/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#) libread.c - Sep 12, 1983  REGULUS 4.1
*/

#include "order.h"

lreadl(fd,str)
int fd;
long *str;
{
	register char *p;
	char junk[4];

	if (read(fd,junk,4) != 4)
		return(ERROR);
	p = junk;
	str->b1 = *p++;
	str->b2 = *p++;
	str->b3 = *p++;
	str->b4 = *p;
	return(SUCCESS);
}

lreadw(fd,str)
int fd;
short *str;
{
	char junk[2];

	if (read(fd,junk,2) != 2)
		return(ERROR);
	str->wb1 = junk[0];
	str->wb2 = junk[1];
	return(SUCCESS);
}

lreadc(fd,str)
int fd;
char *str;
{
	char junk[1];

	if (read(fd,&junk,1) != 1)
		return(ERROR);
	*str = junk[0];
	return(SUCCESS);
}
