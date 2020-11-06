/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)printf.c	1.2    10/19/83
*/

#include <stdio.h>

struct {
	int word0;
	int word1;
};

#define	BUFSIZ		80

FILE *__stream;
static char *__str;
static char **_p;

extern char	*__prtshort(), *__prtld();

char *etoa();
char *ftoa();
char *petoa();
char *pftoa();

printf(arg1,arg2,arg3)
char *arg1, *arg2, *arg3;
{
	FILE *fp;

	if( arg1 == -1 ) {
/*
 *	 printf( ((char *)-1), buf, fmt...) is sprintf(buf, fmt...)
 */
		__str = arg2;
		_p = &arg3;
		*__str = NULL;
		__doprint(1);
		*__str = NULL;
	}
	else if( arg1 < _NUFILE ) {
/*
 *	printf( ((char *)fd), fmt...) is fprintf( fd to fp, fmt,...)
 */
		fp = fdopen( (arg1).word1 , "w");
		__stream = fp;
		_p = &arg2;
		__doprintf(0);
		fflush(fp);
		fp->_flag = 0;
		fp->_nextp = fp->_base = NULL;
	}
	else {
		__stream = stdout;
		_p = &arg1;
		__doprintf(0);
	}
}

fprintf(fp, plist)
FILE *fp;
char *plist;
{
	if( (fp->_flag&(_WMODE|_UPDATE|_APPEND)) == 0 )
		return;

	__stream = fp;
	_p = &plist;
	__doprintf(0);
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
	register char *fmt, c;
	register int *pi;
	char buf[BUFSIZ];
	int width, prec, left, longf, n, len, prepend;
	char padchar;
	char *s;
	auto (*fn)();

	fmt = *_p++;

	pi = _p;
	while( c = *fmt++ ) {
		_p = pi;
		if( c != '%' ) {
			__putch(mode, c);
			continue;
		}
		prepend = left = 0;
		if( (c = *fmt++) == '-' ) {
			c = *fmt++;
			left++;
		}
		if (c == '#') {	/* [vlh] 26 jul 83 */
			c = *fmt++;
			prepend++;
		}
		padchar = ' ';
		if( c == '0' ) {
			padchar = c;
			c = *fmt++;
		}
		width = -1;
		while( c >= '0' && c <= '9' ) {
			if( width < 0 )
				width = 0;
			width = width * 10 + (c - '0');
			c = *fmt++;
		}
		if (c == '*') {	/* [vlh] 26 jul 83 */
			c = *fmt++;
			width = *pi++;
		}
		prec = -1;
		if( c == '.' ) {
			prec = 0;
			c = *fmt++;
		}
		while( c >= '0' && c <= '9' ) {
			prec = prec * 10 + (c - '0');
			c = *fmt++;
		}
		if (c == '*') {	/* [vlh] 26 jul 83 */
			c = *fmt++;
			prec = *pi++;
		}
		longf = 0;
		if( c == 'l' ) {
			longf++;
			c = *fmt++;
		}
		/*
		 * we now have all the prelims out of the way;
		 * let's see what we want to print
		 */
		s = buf;
		switch (c) {

		 case 'd':		/* decimal signed */
		 case 'D':
			if( longf )
				fn = __prtld;
			else
				fn = __prtshort;
			__prtint(pi++, buf, 10, 1, fn, 0);
			if( longf )
				pi++;
			break;

		 case 'u':		/* decimal unsigned */
		 case 'U':
			__prtint(pi++, buf, 10, 0, __prtshort, 0);
			break;

		 case 'o':		/* octal unsigned */
		 case 'O':
			if (prepend)	/* [vlh] 26 jul 83 */
				__putch(mode, '0');
			if( longf )
				fn = __prtld;
			else
				fn = __prtshort;
			__prtint(pi++, buf, 8, 0, fn, 0);
			if( longf )
				pi++;
			break;

		 case 'x':		/* hexadecimal unsigned */
		 case 'X':
			if (prepend) {	/* [vlh] 26 jul 83 */
				__putch(mode, '0');
				__putch(mode, 'x');
			}
			if( longf )
				fn = __prtld;
			else
				fn = __prtshort;
			__prtint(pi++, buf, 16, 0, fn, c == 'X');
			if( longf )
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
			pi += 2;
			prec = -1;
			break;

		  case 'f':		/* floating */
		  case 'F':
			pftoa(pi, buf,  prec);
			pi += 2;
			prec = -1;
			break;

		  case 'g':		/* e or f */
		  case 'G':
			pftoa(pi, buf, prec);
			if (strlen(buf) > (7 + prec) )	/* smallest fp string */
				petoa(pi, buf, prec);
			pi += 2;
			prec = -1;
			break;

		 default:		/* just print the character */
			__putch(mode, c);
			continue;

		}
		len = __length(s);
		if( prec < len && prec >= 0 )
			len = prec;
		n = width - len;
		if( !left ) {
			if( padchar != ' ' && *s == '-' ) {
				len--;
				__putch(mode, *s++);
			}
			while( n-- > 0 )
				__putch(mode, padchar);
		}
		while( len-- )
			__putch(mode, *s++);
		
		while( n-- > 0 )
			__putch(mode, padchar);
	}
	if( mode == 0 && (__stream->_flag&_UNBUF) )
		fflush(__stream);
}

__putch(mode, c)
int	mode;
char	c;
{
	if( mode )
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

