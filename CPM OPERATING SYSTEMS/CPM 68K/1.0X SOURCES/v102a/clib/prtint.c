
#include "portab.h"

BYTE *__prtint(pobj, buf, base, signed, f)
WORD	*pobj;
BYTE	*buf;
WORD	base;
WORD	signed;
BYTE	*(*f)();
{
	BYTE		digs[15];
	REG BYTE	*dp;
	REG WORD	k;
	REG BYTE	*p;

	dp = (*f)(pobj, &buf, base, signed, digs);
 
	if (dp == digs)
		*dp++ = 0;
	p = buf;
	while (dp != digs)
	{
		k = *--dp + '0';		/* HY */
		if (k > '9')
			k += 'A'-10-'0';

		*p++ = k;
	}
	*p = 0;
	return (p);
}

