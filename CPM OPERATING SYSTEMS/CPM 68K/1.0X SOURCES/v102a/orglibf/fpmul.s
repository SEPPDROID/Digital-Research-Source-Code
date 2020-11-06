* 
*	Floating Point Multiplication :
*		Front End to FFP Floating Point Package.
*
*		double
*		fpmul(multiplier,multiplicand)
*		double multiplier, multiplicand;
*
*	Return : Result of Floating Point Multiply
*
.globl fpmul
.globl _fpmul
.globl fpmult
.globl _fpmult
.globl ffpmul2
.text
fpmult:
_fpmult:
fpmul:
_fpmul:
~~fpmul:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		ffpmul2
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
