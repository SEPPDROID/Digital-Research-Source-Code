char *__prtint(pobj, buf, base, signed, f, upper)
int	*pobj;
char	*buf;
int	base;
int	signed;
int	upper;
char	*(*f)();
{
	char		digs[15];
	register char	*dp;
	register int	k;
	register char	*p;

	dp = (*f)(pobj, &buf, base, signed, digs);

	if (dp == digs)
		*dp++ = 0;
	p = buf;
	while (dp != digs)
	{
		k = *--dp;
		if (k < 10)
			k =+ '0';
		else
			k =+ upper ? 'A'-10 : 'a'-10;
		*p++ = k;
	}
	*p = 0;
	return (p);
}


char *__prtshort(pobj, pbuf, base, signed, digs)
int	*pobj;
char	**pbuf;
int	base;
int	signed;
char	*digs;
{
		long 	x;
	register long	n;
	register char	*p;
	register long b;

	p = digs;
	b = base;
	n = *pobj;
	if (signed && n < 0)
	{
		n = -n;
		*(*pbuf)++ = '-';
	}
	else {
		n =& 0xffffL;	/*clear upper half*/
	}
	while (n != 0)
	{
		*p++ = n % b;
		n = n / b;
	}
	return (p);
}
