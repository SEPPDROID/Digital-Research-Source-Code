* 
*	Floating Point Addition :
*		Front End to FFP Floating Point Package.
*
*		double
*		fpadd(addend,adder)
*		double addend, adder;
*
*	Returns : Sum of two floating point numbers
*
.globl fpadd
.globl _fpadd
.globl ffpadd
.text
fpadd:
_fpadd:
~~fpadd:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		ffpadd
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
