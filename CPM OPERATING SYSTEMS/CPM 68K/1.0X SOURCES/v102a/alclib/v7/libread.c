
/*
	Copyright 1983
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
	register char *p;
	char junk[2];

	if (read(fd,junk,2) != 2)
		return(ERROR);
	p = junk;
	str->wb1 = *p++;
	str->wb2 = *p;
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
