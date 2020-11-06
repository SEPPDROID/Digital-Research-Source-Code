* 
*	Floating Point Compare :
*		Front End to FFP Floating Point Package.
*
*		int
*		fpcmp(source,dest)
*		double source, dest;
*
*	Returns : Condition codes based on Floating Point Compare
*
.globl fpcmp
.globl _fpcmp
.globl ffpcmp
.text
fpcmp:
_fpcmp:
~~fpcmp:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		ffpcmp
movem.l	(sp)+,d3-d7
unlk	r14
rts
