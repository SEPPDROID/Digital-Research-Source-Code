* 
*	Floating Point Square Root :
*		Front End to FFP Floating Point Package.
*
*		double
*		sqrt(farg)
*		double farg;
*
*	Input : in radians
*	Returns : square root of Floating point number
*
.globl sqrt
.globl _sqrt
.globl ffpsqrt
.text
sqrt:
_sqrt:
~~sqrt:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpsqrt
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts

