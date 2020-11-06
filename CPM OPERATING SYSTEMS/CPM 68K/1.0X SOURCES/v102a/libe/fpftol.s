* 
*	Floating Point Float to Long Routine :
*		Front End to IEEE Floating Point Package.
*
*		long
*		fpftol(fparg)
*		double fparg;
*
*	Condition Codes : V bit signifies Overflow
*
*	Return : Fixed Point representation of Floating Point Number
*
.globl fpftol
.globl _fpftol
.globl ieffpi
.text
fpftol:
_fpftol:
~~fpftol:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		ieffpi
move.l	r7,r0
movem.l	(sp)+,d7
unlk	r14
rts
