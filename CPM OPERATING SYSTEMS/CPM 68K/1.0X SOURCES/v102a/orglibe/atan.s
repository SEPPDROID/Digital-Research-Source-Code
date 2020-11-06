
* 
*	Floating Point Arctangen:
*		Front End to FFP Floating Point Package.
*
*		double
*		atan(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
.globl _atan
.globl iefatan
.text
fpatan:
_atan:
~~atan:
link	r14,#-4
move.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefatan
move.l	r7,r0
move.l	(sp)+,d7
unlk	r14
rts
