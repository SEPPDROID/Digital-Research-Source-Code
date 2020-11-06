
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) printf.c - Sep 12, 1983  REGULUS 4.1
*/

# include	"iodec.h"
# define	BUFSIZ		80

/**
 **	formated print
 **/

printf(parlist)
char	*parlist;
{
	register char	*fmt, c;
	char		buf[BUFSIZ];
	extern char	*__prtshort(), *__prtld(), *__prtld();
	int		mode;
	char		*fd;
	register char 	**p;
	register int *pi;
	int		width, prec;
	int		left, longf;
	char		padchar;
	char		*s;
	int		n;
	auto		(*fn)();
	int		len;

	p = &parlist;
	fd = 0;
	mode = 0;		/* mode zero, putchar */
	if (parlist + 1 < MAXFILES + 1) {
		mode++;		/* mode one, cputc */
		fd = *p++;
	}
	if (fd == -1) {
		mode++;		/* mode two, string */
		fd = *p++;
	}
	fmt = *p++;

	pi = p;
	while (c = *fmt++)
	{
		p = pi;
		if (c != '%') {
			__putch(mode, &fd, c);
			continue;
		}
		left = 0;
		if ((c = *fmt++) == '-') {
			c = *fmt++;
			left++;
		}
		padchar = ' ';
		if (c == '0') {
			padchar = c;
			c = *fmt++;
		}
		width = -1;
		while (c >= '0' && c <= '9')
		{
			if (width < 0)
				width = 0;
			width = width * 10 + (c - '0');
			c = *fmt++;
		}
		prec = -1;
		if (c == '.')
		{
			prec = 0;
			c = *fmt++;
		}
		while (c >= '0' && c <= '9')
		{
			prec = prec * 10 + (c - '0');
			c = *fmt++;
		}
		longf = 0;
		if (c == 'l')
		{
			longf++;
			c = *fmt++;
		}
		/* we now have all the prelims out of the way;
		   let's see what we want to print */

		s = buf;
		switch (c)
		{

		  case 'd':		/* decimal signed */
		  case 'D':
			if (longf)
				fn = __prtld;
			else
				fn = __prtshort;
			__prtint(pi++, buf, 10, 1, fn, 0);
			if (longf)
				pi++;
			break;

		  case 'u':		/* decimal unsigned */
		  case 'U':
			__prtint(pi++, buf, 10, 0, __prtshort, 0);
			break;

		  case 'o':		/* octal unsigned */
		  case 'O':
			if (longf)
				fn = __prtld;
			else
				fn = __prtshort;
			__prtint(pi++, buf, 8, 0, fn, 0);
			if (longf)
				pi++;
			break;

		  case 'x':		/* hexadecimal unsigned */
		  case 'X':
			if (longf)
				fn = __prtld;
			else
				fn = __prtshort;
			__prtint(pi++, buf, 16, 0, fn, c == 'X');
			if (longf)
				pi++;
			break;

		  case 's':		/* string */
		  case 'S':
			s = *p++;
			pi = p;
			break;

		  case 'c':		/* character */
		  case 'C':
			n = *pi++;
			buf[0] = n;
			buf[1] = '\0';
			break;


		  default:		/* just print the character */
			__putch(mode, &fd, c);
			continue;

		}
		len = __length(s);
		if (prec < len && prec >= 0)
			len = prec;
		n = width - len;
		if (!left)
		{
			if (padchar != ' ' && *s == '-')
			{
				len--;
				__putch(mode, &fd, *s++);
			}
			while (n-- > 0)
				__putch(mode, &fd, padchar);
		}
		while (len--)
			__putch(mode, &fd, *s++);
		while (n-- > 0)
			__putch(mode, &fd, padchar);
	}
	if (mode == 2)
		*fd = '\0';
}


__putch(mode, pfd, c)
int	mode;
char	c;
char	**pfd;
{
	switch (mode)
	{

	  case 0:
		putchar(c);
		break;

	  case 1:
		cputc(c, *pfd);
		break;

	  case 2:
		*(*pfd)++ = c;
		break;

	}
	return (c);
}


char *__prtld(pobj, pbuf, base, signed, digs)
long	*pobj;
char	**pbuf;
int	base;
int	signed;
char	*digs;
{
	register long n;
	register long b;
	register char	*p;
	struct {
		char cbyte0;
		char cbyte1;
		char cbyte2;
		char cbyte3;
	};
	register i;
	struct {
		int wd1;
		int wd2;
	};

	p = digs;
	b = base;
	n = *pobj;
	if(base == 16) {	/* special because of negatives */
		i = 8;
		while(n && i) {
			*p++ = n & 0xf;
			n =>> 4;
			i--;
		}
	}
	else if(base == 8) {
		i = 11;
		while(n && i) {
			*p++ = n & 7;
			n =>> 3;
			i--;
		}
		if(i==0) {
			*(p-1) =& 3;	/* only 2 bits in upper octal digit */
		}
	}
	else {
		if (signed && n < 0) {
			*(*pbuf)++ = '-';
			n = -n;
		}
		while(n) {
			*p++ = n%b;
			n /= b;
		}
	}
	return (p);
}
