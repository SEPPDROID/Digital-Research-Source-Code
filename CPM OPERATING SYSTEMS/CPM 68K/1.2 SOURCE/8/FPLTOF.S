* 
*	Floating Point Long to Float Routine :
*		Front End to IEEE Floating Point Package.
*
*		double
*		fpltof(larg)
*		long larg;
*
*	Return : Floating Point representation of Long Fixed point integer
*
.globl fpltof
.globl _fpltof
.globl iefifp
.text
fpltof:
_fpltof:
~~fpltof:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefifp
move.l	r7,r0
movem.l	(sp)+,d7
unlk	r14
rts
tof:
~~fpltof:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefifp
move.l	r7,r0
movem.l	(s)+,d7
unlk	r14
rts
tof:
~~fpltof:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefifp
move.l	r7,r0
movem.l	(s)+,d7
unlk	r14
rts
tof:
~~fpltof:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefifp
move.l	r7,r0
movem.l	(s)+,d7
unlk	r14
rts
tof:
~~fpltof:
link	r14,#-4
movem.l	d7,-(sp)
move.l	8(r14),r7
jsr		iefifp
move.l	r7,r0
movem.l	(s