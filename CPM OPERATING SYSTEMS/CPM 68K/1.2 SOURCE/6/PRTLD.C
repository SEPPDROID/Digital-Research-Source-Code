/*************************************************************************
*
*		_ _ p r t l d   F u n c t i o n
*		-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"__prtld" prints LONGs, for use with "_doprt".
*
**************************************************************************/
#include "portab.h"
#include "osif.h"	/* used for hardware dependencies: byte store order */

BYTE *__prtld(pobj, pbuf, base, signed, digs)
LONG	*pobj;
BYTE	**pbuf;
WORD	base;
WORD	signed;
BYTE	*digs;
{
	REG LONG	n;
	REG LONG	b;
	REG BYTE	*p;
	REG WORD	ii;
	extern long uldiv();

	extern long uldivr;
	struct long_struct *p_uldivr;

	p_uldivr = &uldivr;
	p = digs;
	b = base;
	n = *pobj;
	if (base == 16) {	/* special because of negatives */
		ii = 8;
		while (n && ii) {
			*p++ = (int)n & 0xf;	/* HY  */
			n >>= 4;		/* MGL */
			ii--;
		}
	}
	else if (base == 8) {
		ii = 11;
		while (n && ii) {
			*p++ = (int)n & 7;	/* HY  */
			n >>= 3;		/* MGL */
			ii--;
		}
		if (ii==0) {
			*(p-1) &= 3;	/* only 2 bits in upper octal digit */
			/* MGL */
		}
	}
	else {
		if (signed && n < 0) {
			*(*pbuf)++ = '-';
			n = -n;
		}
		while (n) {
			n = uldiv(n,b); /* n>0 (or unsigned), do unsigned div*/
			*p++ = p_uldivr->lblolo;
		}
	}
	return (p);
}
 unsigned div*/
			*p++ = p_uldivr->lblolo;
		}
	}
	return (p);
}
 unsigned div*/
			*p++ = p_uldivr->lblolo;
		}
	}
	return (p);
}
