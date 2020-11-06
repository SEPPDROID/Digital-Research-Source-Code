* 
*	Floating Point Logarithm :
*		Front End to FFP Floating Point Package.
*
*		double
*		log(x)
*		double x;
*
*	Returns : the floating point logarithm
*
.globl log
.globl _log
.globl ffplog
.text
log:
_log:
~~log:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		ffplog
move.l	r7,r0
movem.l	(sp)+,d7
unlk	r14
rts
