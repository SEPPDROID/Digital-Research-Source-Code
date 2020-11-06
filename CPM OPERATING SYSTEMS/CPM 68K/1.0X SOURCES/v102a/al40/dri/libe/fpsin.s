* 
*	Floating Point Sine :
*		Front End to IEEE Floating Point Package.
*
*		double
*		sin(farg)
*		double farg;
*
*	Input : in radians
*	Returns : sine of Floating point number
*
.globl sin
.globl _sin
.globl iefsin
.text
sin:
_sin:
~~sin:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		iefsin
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
