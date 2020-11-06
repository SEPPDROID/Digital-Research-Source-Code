*#include <portab.h>
*long uldivr = 0;
*
*
*long uldiv(al1,al2)	/* unsigned long divide */
*long al1,al2;
*{
*
*#ifdef ALCYON
*	register long l1,l2;	
*	/* you must modify comparisons in the assembler file
*	 * for this routine to work.
*	 */
*#else
*	register unsigned long l1, l2;
*#endif
*	register long q, b;
*
*	l1 = al1;
*	l2 = al2;
*	if(l2==0) {
*		uldivr = 0x80000000;
*		return(0x80000000);
*	}
*	if(l2 > l1) {				/** comparison (ble->bls) **/
*		uldivr = l1;
*		return(0);
*	}
*	if(l1==l2) {
*		q = 1;
*		l1 = 0;
*		goto doret;
*	}
*	b = 1;					/* bit value */
*	while(l1>=l2) {				/** comparison (blt->blo) **/
*		if( l2 > (l2<<1) )		/** comparison (bgt->bhi) **/
*			break;			/* detect overflow */
*		l2 =<< 1;
*		b =<< 1;
*	}
*	q = 0;
*	while(b) {		/* now do shifts and subtracts */
*		if(l1>=l2) {			/** comparison (blt->blo) **/
*			q =| b;
*			l1 =- l2;
*		}
*		b =>> 1;
*		l2 =>> 1;
*	}
*doret:
*	uldivr = l1;
*	return(q);
*}
*
*
.globl _uldivr
.data
_uldivr:
.dc.w 0,0
.globl _uldiv
.text
_uldiv:
~~uldiv:
~b=R4
~q=R5
~l1=R7
~l2=R6
~al1=8
~al2=12
link R14,#0
movem.l R3-R7,-(sp)
*line 20
move.l 8(R14),R7
*line 21
move.l 12(R14),R6
*line 22
tst.l R6
bne L2
*line 23
move.l #$80000000,_uldivr
*line 24
move.l #$80000000,R0
bra L1
L2:
*line 26
cmp.l R7,R6
bls L3			*<<<<< ble
*line 27
move.l R7,_uldivr
*line 28
clr.l R0
bra L1
L3:
*line 30
cmp.l R6,R7
bne L4
*line 31
move.l #1,R5
*line 32
clr.l R7
bra L5
L4:
*line 35
move.l #1,R4
L7:
*line 36
cmp.l R6,R7
blo L6			*<<<< blt
*line 38
move.l R6,R0
asl.l #1,R0
cmp.l R0,R6
bhi L6			*<<<< bgt
*line 39
asl.l #1,R6
*line 40
asl.l #1,R4
bra L7
L6:
*line 42
clr.l R5
L9:
*line 43
tst.l R4
beq L8
*line 44
cmp.l R6,R7
blo L10			*<<<< blt
*line 45
or.l R4,R5
*line 46
sub.l R6,R7
L10:
*line 48
lsr.l #1,R4		*<<<< asr
*line 49
lsr.l #1,R6		*<<<< asr
bra L9
L8:L5:
*line 52
move.l R7,_uldivr
*line 53
move.l R5,R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R4-R7
unlk R14
rts
.data
