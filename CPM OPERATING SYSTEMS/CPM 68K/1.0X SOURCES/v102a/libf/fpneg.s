* 
*	Floating Point Negation :
*		Front End to FFP Floating Point Package.
*
*		double
*		fpneg(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
.globl fpneg
.globl _fpneg
.globl ffpneg
.text
fpneg:
_fpneg:
~~fpneg:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpneg
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts

