* 
*	Floating Point Hyperbolic sine:
*		Front End to FFP Floating Point Package.
*
*		double
*		sinh(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
.globl _sinh
.globl ffpsinh
.text
fpsinh:
_sinh:
~~sinh:
link	r14,#-4
move.l	d7,-(sp)
move.l	8(r14),r7
jsr		ffpsinh
move.l	r7,r0
move.l	(sp)+,d7
unlk	r14
rts

