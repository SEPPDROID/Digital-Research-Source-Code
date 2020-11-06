
* 
*	Floating Point Hyperbolic cosine:
*		Front End to FFP Floating Point Package.
*
*		double
*		cosh(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
.globl _cosh
.globl ffpcosh
.text
fpcosh:
_cosh:
~~cosh:
link	r14,#-4
move.l	d7,-(sp)
move.l	8(r14),r7
jsr		ffpcosh
move.l	r7,r0
move.l	(sp)+,d7
unlk	r14
rts
