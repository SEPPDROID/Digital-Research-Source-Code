* 
*	Floating Point Exponent :
*		Front End to FFP Floating Point Package.
*
*		double
*		exp(x)
*		double x;
*
*	Returns : e ^ x (where e = 2.718...)
*
.globl exp
.globl _exp
.globl ffpexp
.text
exp:
_exp:
~~exp:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		ffpexp
move.l	r7,r0
movem.l	(sp)+,d7
unlk	r14
rts
