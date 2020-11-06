/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/**
 **	formated print
 **/

/*char *version "@(#)fprintf - jan 24, 1982";*/
#include "/usr/regulus/stdio.h"
#define	BUFSIZ		80

char *ftoa();
char *etoa();
char *petoa();
char *pftoa();

static char *__str;
FILE *__stream;
static char **_p;

fprintf(fp, plist)
FILE *fp;
char *plist;
{
	if (!fp->_flag & _WMODE) return;

	__stream = fp;
	_p = &plist;
	__doprintf(0);
	fflush(fp);
}

sprintf(s, plist)
char *s, *plist;
{
	__str = s;
	_p = &plist;
	__doprint(1);
	*__str = NULL;
}

__doprint(mode)
int mode;
{
	register char	*fmt, c;
	char		buf[BUFSIZ];
	extern char	*__prtshort(), *__prtld();
	register int *pi;
	int		width, prec;
	int		left, longf;
	char		padchar;
	char		*s;
	int		n;
	auto		(*fn)();
	int		len;

	fmt = *_p++;

	pi = _p;
	while (c = *fmt++)
	{
		_p = pi;
		if (c != '%')
		{
			__putch(mode, c);
			continue;
		}
		left = 0;
		if ((c = *fmt++) == '-')
		{
			c = *fmt++;
			left++;
		}
		padchar = ' ';
		if (c == '0')
		{
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
		/*
		 * we now have all the prelims out of the way;
		 *  let's see what we want to print
		 */
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
			s = *_p++;
			pi = _p;
			break;

		  case 'c':		/* character */
		  case 'C':
			n = *pi++;
			buf[0] = n;
			buf[1] = '\0';
			break;

		  case 'e':		/* exponential */
		  case 'E':
			petoa(pi, buf, prec);
			pi =+ 2;
			prec = -1;
			break;

		  case 'f':		/* floating */
		  case 'F':
			fptoa(pi, buf, prec);
			pi =+ 2;
			prec = -1;
			break;

		  case 'g':		/* e or f */
		  case 'G':
			pftoa(pi, buf, prec);
			if (strlen(buf) > (7 + prec)) /* smallest fp string */
				petoa(pi, buf, prec);
			pi =+ 2;
			prec = -1;
			break;

		  default:		/* just print the character */
			__putch(mode, c);
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
				__putch(mode, *s); s++;
			}
			while (n-- > 0)
				__putch(mode, padchar);
		}
		while (len--) {
			__putch(mode, *s); s++;
		}
		while (n-- > 0)
			__putch(mode, padchar);
	}
}


__putch(mode, c)
int	mode;
char	c;
{
	if (mode)
		*__str++ = c;
	else
		putc(c,__stream);
	return (c);
}

char *
pftoa(addr,buf,prec)
float *addr;
char *buf;
int prec;
{
	float fp;

	if (prec<0)
		prec = 6;
	fp = *addr;
	return( ftoa(fp, buf, prec) );
}

char *
petoa(addr,buf,prec)
float *addr;
char *buf;
int prec;
{
	float fp;

	if (prec<0)
		prec = 6;
	fp = *addr;
	return( etoa(fp, buf, prec) );
}
