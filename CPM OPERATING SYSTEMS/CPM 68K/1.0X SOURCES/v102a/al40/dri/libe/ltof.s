.globl _fpltof
.text
_fpltof:
~~fpltof:
~l=8
~exp=R7
~sign=R6
link R14,#0
movem.l R5-R7,-(sp)
*line 30
tst.l 8(R14)
bge L2
*line 31
move #1,R6
*line 32
move.l 8(R14),R0
neg.l R0
move.l R0,8(R14)
bra L3
L2:
*line 35
clr R6
L3:
*line 37
tst.l 8(R14)
bne L4
*line 37
clr.l R0
bra L1
L4:
*line 38
move.l #$17,R7
L6:
*line 39
move.l 8(R14),R0
and.l #$7f000000,R0
beq L5
*line 40
move.l 8(R14),R0
asr.l #1,R0
move.l R0,8(R14)
L7:
*line 39
add.l #1,R7
bra L6
L5:L9:
*line 41
move.l 8(R14),R0
and.l #$800000,R0
bne L8
*line 42
move.l 8(R14),R0
asl.l #1,R0
move.l R0,8(R14)
L10:
*line 41
sub.l #1,R7
bra L9
L8:
*line 43
move.l #$7fffff,R0
and.l R0,8(R14)
*line 45
tst R6
beq L11
*line 45
move.l #$80000000,R0
or.l R0,8(R14)
L11:
*line 46
add.l #$7f,R7
clr.l R0
move #23,R0
asl.l R0,R7
*line 47
or.l R7,8(R14)
*line 48
move.l 8(R14),R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.data
