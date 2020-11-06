.globl	_ldiv
.globl	_ldivr
.comm	_ldivr,4
.globl	_alrem
.globl  alrem
.text
_alrem:
alrem:
~~alrem:
~l2=12
~al1=8
link	R14,#-2
move.l	12(R14),-(sp)
move.l	8(R14),R8
move.l	(R8),-(sp)
jsr	_ldiv
cmpm.l	(sp)+,(sp)+
move.l	_ldivr,R0
move.l	8(R14),R9
move.l	R0,(R9)
unlk	R14
rts
(R14),R8
move.l	(R8),-(sp)
jsr	_ldiv
cmpm.l	(sp)+,(sp)+
move.l	_ldivr,R0
move.l	8(14),R9
move.l	R0,(R9)
unlk	R14
rts
(R14),R8
move.l	(R8),-(sp)
jsr	_ldiv
cmpm.l	(sp)+,(sp)+
move.l	_ldivr,R0
move.l	8(14),R9
move.l	R0,(R9)
unlk	R14
rts
(R14),R8
move.l	(R8),-(sp)
jsr	_ldiv
cmpm.l	(sp)+,(sp)+
move.l	_ldivr,R0
move.l	8(