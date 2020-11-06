
#include "portab.h"
#include "osif.h"


BYTE *__prtshort(pobj, pbuf, base, signed, digs)
WORD	*pobj;
BYTE	**pbuf;
WORD	base;
WORD	signed;

BYTE	*digs;
{
  /* 05/27/83 - changed to do divide in short (HY) */
  /* 08/11/83 - changed to handle 68K arg passing (as longs) (WHF) */

	REG UWORD	n;
	REG BYTE	*p;
	REG WORD	b;
#if HILO					/*--------------------------*/
	    LONG	ln;			/* declare a placeholder    */
						/*			    */
	ln = *pobj;				/* assign arg to long	    */
	n = ln & 0xFFFFL;			/* then to WORD		    */
#else						/*--------------------------*/
	n = *pobj;				/* assign arg to WORD	    */
#endif						/*--------------------------*/
	p = digs;
	b = base;
	if (signed)
	    if ((int)n < 0) {
		n = -n;
		*(*pbuf)++ = '-';
	    }
	while (n != 0)
	{
		*p++ = n % b;
		n = n / b;
	}
	return (p);
}
	    }
	while (n != 0)
	{
		*p++ = n % b;
		n = n / b;
	}
	return (p);
}
	    }
	while (n != 0)
	{
		*p++ = n % b;
		n = n / b;
	}
	return (p);
}
