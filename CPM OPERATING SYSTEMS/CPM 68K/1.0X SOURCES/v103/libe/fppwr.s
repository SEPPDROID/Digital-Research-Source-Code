* 
*	Floating Point Power :
*		Front End to IEEE Floating Point Package.
*
*		double
*		pow(x,y)
*		double x, y;
*
*	Returns : x ^ y
*
.globl pow
.globl _pow
.globl iefpwr
.text
pow:
_pow:
~~pow:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		iefpwr
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
