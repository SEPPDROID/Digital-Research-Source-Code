/* swab - swap (hi/lo) bytes in an area of memory */
/*	constructed to allow in-place swabs (fr==to) */
#include <portab.h>

WORD 	swab(fr,to,num)				/* CLEAR FUNCTION ***********/
	REG BYTE *fr;
	REG BYTE *to;
	REG WORD num;
{
	REG BYTE t;
	for( ; num>0; num -= 2, fr += 2, to += 2) {
		t = fr[0];
		to[0] = fr[1];
		to[1] = t;
	}
	return(0);
}
