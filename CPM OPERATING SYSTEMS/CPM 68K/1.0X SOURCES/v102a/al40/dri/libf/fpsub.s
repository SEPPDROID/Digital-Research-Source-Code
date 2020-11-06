* 
*	Floating Point Subtraction :
*		Front End to FFP Floating Point Package.
*
*		double
*		fpsub(subtrahend,minuend)
*		double subtrahend, minuend;
*
*	Returns : Floating point subtraction result
*
.globl fpsub
.globl _fpsub
.globl ffpsub
.text
fpsub:
_fpsub:
~~fpsub:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		ffpsub
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
