* 
*	Floating Point Power :
*		Front End to FFP Floating Point Package.
*
*		double
*		pow(x,y)
*		double x, y;
*
*	Returns : x ^ y
*
.globl pow
.globl _pow
.globl ffppwr
.text
pow:
_pow:
~~pow:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		ffppwr
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
