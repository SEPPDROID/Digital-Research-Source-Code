* 
*	Floating Point Subtraction :
*		Front End to IEEE Floating Point Package.
*
*		double
*		fpsub(subtrahend,minuend)
*		double subtrahend, minuend;
*
*	Returns : Floating point subtraction result
*
.globl fpsub
.globl _fpsub
.globl iefsub
.text
fpsub:
_fpsub:
~~fpsub:
link	r14,#-4
movem.l	d6-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		iefsub
move.l	r7,r0
movem.l	(sp)+,d6-d7
unlk	r14
rts
