/*************************************************************************
*
*		_ _ p r t l d   F u n c t i o n
*		-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"__prtld" prints LONGs, for use with "_doprt".
*
**************************************************************************/
#include <stdio.h>
#include <cpm.h>	/* used for hardware dependencies: byte store order */

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
	extern long uldiv();
	extern long uldivr;
	REG WORD	i;

	p = digs;
	b = base;
	n = *pobj;
	if(base == 16) {	/* special because of negatives */
		i = 8;
		while(n && i) {
			*p++ = n & 0xf;
			n =>> 4;
			i--;
		}
	}
	else if(base == 8) {
		i = 11;
		while(n && i) {
			*p++ = n & 7;
			n =>> 3;
			i--;
		}
		if(i==0) {
			*(p-1) =& 3;	/* only 2 bits in upper octal digit */
		}
	}
	else {
		if (signed && n < 0) {
			*(*pbuf)++ = '-';
			n = -n;
		}
		while(n) {
			n = uldiv(n,b); /* n>0 (or unsigned), do unsigned div*/
			*p++ = uldivr.lblolo;
		}
	}
	return (p);
}
