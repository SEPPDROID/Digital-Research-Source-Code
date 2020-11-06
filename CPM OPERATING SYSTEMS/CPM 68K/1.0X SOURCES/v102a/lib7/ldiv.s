.globl _ldivr
.comm _ldivr,4
.globl _ldiv
.globl ldiv
.text
_ldiv:
ldiv:
~~ldiv:
~b=R4
~q=R5
~l1=R7
~l2=R6
~al1=8
~al2=12
~sign=R3
link R14,#-2
movem.l R2-R7,-(sp)
clr R3
clr.l R5
move.l 8(R14),R7
move.l 12(R14),R6
bne L2
move.l #$80000000,_ldivr
move.l #$80000000,R0
bra L1
L2:
bge L3
neg.l R6
add #1,R3
L3:
tst.l R7
bge L4
neg.l R7
add #1,R3
L4:
cmp.l R7,R6
bgt L6
bne L7
move.l #1,R5
clr.l R7
bra L6
L7:
cmp.l #$10000,R7
bge L9
divu R6,R7
move R7,R5
swap R7
ext.l R7
bra L6
L9:
move.l #1,R4
L12:
cmp.l R6,R7
blo L11
asl.l #1,R6
asl.l #1,R4
bra L12
L11:
tst.l R4
beq L6
cmp.l R6,R7
blo L15
or.l R4,R5
sub.l R6,R7
L15:
lsr.l #1,R4
lsr.l #1,R6
bra L11
L6:
cmp #1,R3
bne L16
neg.l R7
move.l R7,_ldivr
move.l R5,R0
neg.l R0
bra L1
L16:
move.l R7,_ldivr
move.l R5,R0
L1:
tst.l (sp)+
movem.l (sp)+,R3-R7
unlk R14
rts
