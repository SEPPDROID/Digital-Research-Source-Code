.globl	_ldiv
.globl	_ldivr
.comm	_ldivr,4
.globl	_lrem
.globl	lrem
.text
_lrem:
lrem:
~~lrem:
~l2=12
~al1=8
link	R14,#-2
move.l	12(R14),-(sp)
move.l	8(R14),-(sp)
jsr	_ldiv
cmpm.l	(sp)+,(sp)+
move.l	_ldivr,R0
unlk	R14
rts


