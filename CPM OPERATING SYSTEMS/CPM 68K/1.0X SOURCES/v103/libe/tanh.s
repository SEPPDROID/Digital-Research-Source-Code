
* 
*	Floating Point Hyperbolic tangent:
*		Front End to FFP Floating Point Package.
*
*		double
*		tanh(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
.globl _tanh
.globl ieftanh
.text
fptanh:
_tanh:
~~tanh:
link	r14,#-4
move.l	d7,-(sp)
move.l	8(r14),r7
jsr		ieftanh
move.l	r7,r0
move.l	(sp)+,d7
unlk	r14
rts
