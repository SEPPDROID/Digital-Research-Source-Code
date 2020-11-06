/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

//char *version "@(#)printf - jan 24, 1982";

/**
 **	formated print
 **/

#define BUFSIZ	80
#define MAXFILES 15

char *etoa();
char *ftoa();
char *petoa();
char *pftoa();

printf(arg1,arg2,arg3)
char *arg1, *arg2, *arg3;
{
	register char	*fmt, c;
	char		buf[BUFSIZ];
	extern char	*__prtshort(), *__pprtld(), *__prtld();
	int		mode;
	char	*fd;
	register char 	**p;
	register int *pi;
	int		width, prec;
	int		left, longf;
	char	padchar;
	char	*s;
	int		n;
	auto	(*fn)();
	int		len;

	if( arg1 == -1 ) {
		mode = 2;
		fd = arg2;
		p = &arg3;
	}
	else if( arg1 < MAXFILES ) {
		mode = 1;		/* printf(fd,fmt,...) or printf(-1,str,fmt,...) */
		fd = arg1;
		p = &arg2;
	}
	else {
		mode = 0;
		fd = 0;
		p = &arg1;
	}
	fmt = *p++;

	pi = p;
	while (c = *fmt++)
	{
		p = pi;
		if (c != '%')
		{
			__pputch(mode, &fd, c);
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

		  case 'e':		/* exponential */
		  case 'E':
			petoa(pi, buf, prec);
			pi =+ 2;
			prec = -1;
			break;

		  case 'f':		/* floating */
		  case 'F':
			pftoa(pi, buf,  prec);
			pi =+ 2;
			prec = -1;
			break;

		  case 'g':		/* e or f */
		  case 'G':
			pftoa(pi, buf, prec);
			if (strlen(buf) > (7 + prec) )	/* smallest fp string */
				petoa(pi, buf, prec);
			pi =+ 2;
			prec = -1;
			break;

		  default:		/* just print the character */
			__pputch(mode, &fd, c);
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
				__pputch(mode, &fd, *s++);
			}
			while (n-- > 0)
				__pputch(mode, &fd, padchar);
		}
		while (len--)
			__pputch(mode, &fd, *s++);
		while (n-- > 0)
			__pputch(mode, &fd, padchar);
	}
	if (mode == 2)
		*fd = '\0';
}


__pputch(mode, pfd, c)
int	mode;
char	c;
char	**pfd;
{
	register long fd;

	switch (mode)
	{

	  case 0:
		if (write(1,&c,1) != 1) return(-1);
		break;

	  case 1:
		fd = *pfd;
		if (write((int)fd,&c,1) != 1) return(-1);
		break;

	  case 2:
		*(*pfd)++ = c;
		break;

	}
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
