
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) cputc.c - Sep 12, 1983  REGULUS 4.1
*/

# include	"iodec.h"

/**
 **	put a single character
 **/

int	f_log	0;

cputc(c, fn)
char	c;
int	fn;
{
	if (fn == 2)	/* Standard Error */
		write(fn, &c, 1);
	else
		putchar(c);
}
