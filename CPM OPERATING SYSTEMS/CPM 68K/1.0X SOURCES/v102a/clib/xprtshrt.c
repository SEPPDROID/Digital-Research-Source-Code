#include <portab.h>


BYTE *__prtshort(pobj, pbuf, base, signed, digs)
WORD	*pobj;
BYTE	**pbuf;
WORD	base;
WORD	signed;
BYTE	*digs;
{
	extern long	uldivr;
	extern long	uldiv();
	REG LONG	n;
	REG BYTE	*p;
	REG LONG	b;

	p = digs;
	b = base;
	n = *pobj;
	if (signed && n < 0)
	{
		n = -n;
		*(*pbuf)++ = '-';
	}
	else {
		n =& 0xffffL;	/* clear upper half */
	}
	while (n != 0)
	{
		n = uldiv(n,b);
		*p++ = uldivr;
	}
	return (p);
}
