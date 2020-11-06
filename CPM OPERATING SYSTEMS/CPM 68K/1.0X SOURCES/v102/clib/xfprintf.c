#define	BUFSIZ		80

char *ftoa();
char *etoa();
char *petoa();
char *pftoa();
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
			pftoa(pi, buf, prec);
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
