* 
*	Floating Point Negation :
*		Front End to IEEE Floating Point Package.
*
*		double
*		fpneg(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
.globl fpneg
.globl _fpneg
.globl iefneg
.text
fpneg:
_fpneg:
~~fpneg:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefneg
move.l	r7,r0
movem.l	(sp)+,d7
unlk	r14
rts
