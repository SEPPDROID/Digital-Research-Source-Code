.globl	_ldiv
.globl	_aldiv
.globl	aldiv
.text
_aldiv:
aldiv:
~~aldiv:
~l2=12
~al1=8
link	R14,#-2
move.l	12(R14),-(sp)
move.l	8(R14),R8
move.l	(R8),-(sp)
jsr	_ldiv
cmpm.l	(sp)+,(sp)+
move.l	8(R14),R9
move.l	R0,(R9)
L1:
unlk	R14
rts
