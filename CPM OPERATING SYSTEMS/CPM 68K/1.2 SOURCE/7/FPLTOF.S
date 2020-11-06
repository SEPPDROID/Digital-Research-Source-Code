* 
*	Floating Point Long to Float Routine :
*		Front End to FFP Floating Point Package.
*
*		double
*		fpltof(larg)
*		long larg;
*
*	Return : Floating Point representation of Long Fixed point integer
*
.globl fpltof
.globl _fpltof
.globl ffpifp
.text
fpltof:
_fpltof:
~~fpltof:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpifp
move.l	r7,r0
movem.l	(sp)+,d3-d7
unlk	r14
rts
~~fpltof:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpifp
move.l	r7,r0
movem.l	sp)+,d3-d7
unlk	r14
rts
~~fpltof:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpifp
move.l	r7,r0
movem.l	sp)+,d3-d7
unlk	r14
rts
~~fpltof:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpifp
move.l	r7,r0
movem.l	sp)+,d3-d7
unlk	r14
rts
~~fpltof:
link	r14,#-4
movem.l	d3-d7,-(sp)
move.l	8(r14),r7
jsr		ffpifp
move.l	r7,r0
movem.l	