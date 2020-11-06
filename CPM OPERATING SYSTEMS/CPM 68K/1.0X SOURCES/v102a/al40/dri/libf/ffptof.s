.globl _ffptof
.text
_ffptof:
~~ffptof:
~fsign=R5
~count=R6
~f=-4
~lf=8
~exp=R7
link R14,#-4
movem.l R4-R7,-(sp)
*line 31
tst.l 8(R14)
bne L2
*line 31
move.l #$0,R0
bra L1
L2:
*line 32
move.l 8(R14),R0
and.l #128,R0
move R0,R5
*line 33
move.l 8(R14),R0
and.l #127,R0
add.l #-64,R0
move R0,R7
*line 34
move.l 8(R14),R0
asr.l #8,R0
and.l #$ffffff,R0
move.l R0,8(R14)
*line 35
move.l 8(R14),-(sp)
jsr _fpltof
addq.l #4,sp
move.l R0,-4(R14)
*line 36
move.l #$80000059,-(sp)
move.l -4(R14),-(sp)
jsr _fpdiv
addq.l #8,sp
move.l R0,-4(R14)
L4:
*line 37
tst R7
bge L3
*line 38
move.l #$80000042,-(sp)
move.l -4(R14),-(sp)
jsr _fpdiv
addq.l #8,sp
move.l R0,-4(R14)
*line 39
add #1,R7
bra L4
L3:L6:
*line 41
tst R7
ble L5
*line 42
move.l #$80000042,-(sp)
move.l -4(R14),-(sp)
jsr _fpmult
addq.l #8,sp
move.l R0,-4(R14)
*line 43
sub #1,R7
bra L6
L5:
*line 46
tst R5
beq L7
*line 46
move.l -4(R14),-(sp)
jsr _fpneg
addq.l #4,sp
move.l R0,-4(R14)
L7:
*line 47
move.l -4(R14),R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R5-R7
unlk R14
rts
.data
