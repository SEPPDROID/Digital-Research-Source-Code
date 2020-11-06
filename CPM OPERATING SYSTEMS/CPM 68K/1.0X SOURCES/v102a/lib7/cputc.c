# include	"iodec.h"

/**
 **	put a single character
 **/

int	f_log	0;

cputc(c, fn)
char	c;
int	fn;
{
	/*if (fn == 2)	/* Standard Error */
	/*		write(fn, &c, 1);*/
	/*else*/
		putchar(c);
}
