.globl _atof
.text
_atof:
~~atof:
~esign=-32
~isign=-34
~places=-38
~dp=-30
~ep=R12
~fp=-46
~ip=R13
~ffp=-28
~buf=8
~ebin=-36
~ibin=-42
~ebuf=-24
~ibuf=-20
link R14,#-46
movem.l R7-R7/R12-R13,-(sp)
*line 41
lea -20(R14),R13
*line 41
lea -24(R14),R12
*line 41
clr -30(R14)
*line 41
clr -38(R14)
L3:
*line 42
move.l 8(R14),R8
cmp.b #32,(R8)
beq L10000
move.l 8(R14),R8
cmp.b #9,(R8)
bne L2
L10000:*line 43
add.l #1,8(R14)
bra L3
L2:
*line 44
move.l 8(R14),R8
cmp.b #45,(R8)
beq L10001
clr R0
bra L10002
L10001:move #1,R0
L10002:move R0,-34(R14)
*line 46
move.l 8(R14),R8
cmp.b #45,(R8)
beq L10003
move.l 8(R14),R8
cmp.b #43,(R8)
bne L4
L10003:*line 46
add.l #1,8(R14)
L4:L6:
*line 47
move.l 8(R14),R8
tst.b (R8)
beq L5
move.l 8(R14),R8
cmp.b #101,(R8)
beq L5
move.l 8(R14),R8
cmp.b #69,(R8)
beq L5
*line 49
move.l 8(R14),R8
cmp.b #46,(R8)
bne L7
*line 49
add #1,-30(R14)
bra L8
L7:
*line 51
move.l 8(R14),R8
move.b (R8),(R13)+
*line 53
tst -30(R14)
beq L9
*line 53
add #1,-38(R14)
L9:L8:
*line 55
add.l #1,8(R14)
bra L6
L5:
*line 57
clr.b (R13)
*line 58
move.l 8(R14),R8
cmp.b #101,(R8)
beq L10004
move.l 8(R14),R8
cmp.b #69,(R8)
bne L10
L10004:*line 59
add.l #1,8(R14)
*line 60
move.l 8(R14),R8
cmp.b #45,(R8)
beq L10005
clr R0
bra L10006
L10005:move #1,R0
L10006:move R0,-32(R14)
*line 62
move.l 8(R14),R8
cmp.b #45,(R8)
beq L10007
move.l 8(R14),R8
cmp.b #43,(R8)
bne L11
L10007:*line 62
add.l #1,8(R14)
L11:L13:
*line 63
move.l 8(R14),R8
tst.b (R8)
beq L12
*line 64
move.l 8(R14),R8
move.b (R8),(R12)+
add.l #1,8(R14)
bra L13
L12:L10:
*line 66
clr.b (R12)
*line 67
move.l R14,(sp)
add.l #-20,(sp)
jsr _strbin
move.l R0,-42(R14)
*line 68
move.l R14,(sp)
add.l #-24,(sp)
jsr _atoi
move R0,-36(R14)
*line 69
tst -32(R14)
beq L10008
move -36(R14),R0
neg R0
sub -38(R14),R0
bra L10010
L10008:move -36(R14),R0
sub -38(R14),R0
L10010:move R0,-38(R14)
*line 70
move.l -42(R14),-(sp)
move -38(R14),-(sp)
jsr _power10
addq.l #2,sp
move.l R0,-(sp)
jsr _fpmult
addq.l #8,sp
move.l R0,-46(R14)
*line 71
move.l -46(R14),(sp)
jsr _fptoffp
move.l R0,-28(R14)
*line 73
tst -34(R14)
beq L14
*line 73
or.l #128,-28(R14)
L14:
*line 74
move.l -28(R14),R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R12-R13
unlk R14
rts
.globl _power10
.text
_power10:
~~power10:
~f=-4
~pwr=8
link R14,#-8
*line 84
tst 8(R14)
bge L16
*line 84
move.l #$80000041,-4(R14)
L18:
*line 84
tst 8(R14)
bge L17
*line 85
move.l #$a0000044,-(sp)
move.l -4(R14),-(sp)
jsr _fpdiv
addq.l #8,sp
move.l R0,-4(R14)
L19:
*line 84
add #1,8(R14)
bra L18
L17:bra L20
L16:
*line 87
move.l #$80000041,-4(R14)
L22:
*line 87
tst 8(R14)
ble L21
*line 88
move.l #$a0000044,-(sp)
move.l -4(R14),-(sp)
jsr _fpmult
addq.l #8,sp
move.l R0,-4(R14)
L23:
*line 87
sub #1,8(R14)
bra L22
L21:L20:
*line 89
move.l -4(R14),R0
bra L15
L15:unlk R14
rts
.globl _strbin
.text
_strbin:
~~strbin:
~f=-4
~p=8
link R14,#-8
*line 98
move.l #$0,-4(R14)
L26:
*line 98
move.l 8(R14),R8
cmp.b #48,(R8)
blt L25
move.l 8(R14),R8
cmp.b #57,(R8)
bgt L25
*line 99
move.l #$a0000044,-(sp)
move.l -4(R14),-(sp)
jsr _fpmult
addq.l #8,sp
move.l R0,-4(R14)
*line 100
move.l -4(R14),-(sp)
move.l 8(R14),R8
move.b (R8),R0
ext.w R0
add #-48,R0
ext.l R0
move.l R0,-(sp)
jsr _fpltof
addq.l #4,sp
move.l R0,-(sp)
jsr _fpadd
addq.l #8,sp
move.l R0,-4(R14)
L27:
*line 98
add.l #1,8(R14)
bra L26
L25:
*line 102
move.l -4(R14),R0
bra L24
L24:unlk R14
rts
.data
