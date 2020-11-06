#include <portab.h>

BYTE *__prtint(pobj, buf, base, signed, f)
WORD	*pobj;
BYTE	*buf;
WORD	base;
WORD	signed;
BYTE	*(*f)();
{
	MLOCAL BYTE	digs[15];
	REG BYTE	*dp;
	REG WORD	k;
	REG BYTE	*p;

	dp = (*f)(pobj, &buf, base, signed, digs);

	if (dp == digs)
		*dp++ = 0;
	p = buf;
	while (dp != digs)
	{
		k = *--dp;
		k += ( (k < 10) ? '0' : 'A'-10 );
		*p++ = k;
	}
	*p = 0;
	return (p);
}

