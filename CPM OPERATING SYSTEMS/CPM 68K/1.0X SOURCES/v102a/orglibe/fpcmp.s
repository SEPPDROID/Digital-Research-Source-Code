* 
*	Floating Point Compare :
*		Front End to IEEE Floating Point Package.
*
*		fpcmp(source,dest)
*		double source, dest;
*
*	Returns : Condition codes based on Floating Point Compare
*			  in the Condition code register
*
.globl fpcmp
.globl _fpcmp
.globl iefcmp
.text
fpcmp:
_fpcmp:
~~fpcmp:
link	r14,#-4
movem.l	d6-d7,-(sp)
move.l	8(r14),r7
move.l	12(r14),r6
jsr		iefcmp
movem.l	(sp)+,d6-d7
unlk	r14
rts
