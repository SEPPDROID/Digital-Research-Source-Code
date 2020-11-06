.globl __bdos
.globl _startld
.globl _bios1
.globl _move
.globl _buffer
.comm _buffer,128
.globl _cpmfcb
.data
_cpmfcb:
.dc.w 67
.dc.w 20557
.dc.w 8224
.dc.w 8224
.dc.w 8275
.dc.w 22867
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.globl _pstring
.text
_pstring:
~~pstring:
~p=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
move.l 8(R14),R13
L3:
tst.b (R13)
beq L2
move.b (R13)+,R0
ext.w R0
move R0,(sp)
move #2,-(sp)
jsr __bdos
add #2,sp
bra L3
L2:L1:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _badload
.text
_badload:
~~badload:
~i=R6
~errtype=R7
link R14,#0
movem.l R5-R7,-(sp)
move 8(R14),R7
move R7,R0
bra L6
L7:
move.l #L8,(sp)
jsr _pstring
jsr _startld
L9:
move.l #L10,(sp)
jsr _pstring
bra L5
L11:
move.l #L12,(sp)
jsr _pstring
bra L5
L6:tst R0
beq L7
cmp #1,R0
beq L9
cmp #2,R0
beq L11
bra L5
L5:
move.l #L13,(sp)
jsr _pstring
jsr _startld
L4:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _load
.text
_load:
~~load:
~p=R12
~length=R7
~hp=R11
~fcbp=R13
link R14,#0
movem.l R6-R7/R11-R13,-(sp)
move.l 8(R14),R13
move.l #_buffer,(sp)
clr -(sp)
move #26,-(sp)
jsr __bdos
add #4,sp
move.l R13,(sp)
clr -(sp)
move #15,-(sp)
jsr __bdos
add #4,sp
cmp #255,R0
blt L15
move #1,(sp)
jsr _badload
L15:
clr.b 32(R13)
move.l R13,(sp)
clr -(sp)
move #20,-(sp)
jsr __bdos
add #4,sp
tst R0
beq L16
move #1,(sp)
jsr _badload
L16:
move.l #_buffer,R11
cmp #24602,(R11)
bne L10000
tst 26(R11)
bne L17
L10000:move #2,(sp)
jsr _badload
L17:
move.l 22(R11),R12
move.l 6(R11),R7
add.l 2(R11),R7
move #100,(sp)
move.l R12,-(sp)
move.l #28+_buffer,-(sp)
jsr _move
add #8,sp
add.l #100,R12
sub.l #100,R7
L19:
cmp.l #$0,R7
ble L18
move.l R12,(sp)
clr -(sp)
move #26,-(sp)
jsr __bdos
add #4,sp
move.l R13,(sp)
clr -(sp)
move #20,-(sp)
jsr __bdos
add #4,sp
tst R0
beq L20
move #1,(sp)
jsr _badload
L20:
add.l #128,R12
sub.l #128,R7
bra L19
L18:
move.l 22(R11),R0
bra L14
L14:tst.l (sp)+
movem.l (sp)+,R7-R7/R11-R13
unlk R14
rts
.globl _ldcpm
.text
_ldcpm:
~~ldcpm:
~gocpm=-4
link R14,#-8
clr (sp)
move #14,-(sp)
jsr __bdos
add #2,sp
tst R0
beq L22
clr (sp)
jsr _badload
L22:
move.l #_cpmfcb,(sp)
jsr _load
move.l R0,-4(R14)
move.l #L23,(sp)
jsr _pstring
move.l #L24,(sp)
jsr _pstring
move.l -4(R14),R8
jsr (R8)
L21:unlk R14
rts
.data
L8:.dc.b 13,10,66,111,111,116,32,101,114,114,111,114,46,0
L10:.dc.b 10,13,79,112,101,110,32,111,114,32,82,101,97,100,32,101,114,114,111,114,32,111,110,32,0
L12:.dc.b 10,13,66,97,100,32,102,105,108,101,32,102,111,114,109,97,116,32,111,110,32,0
L13:.dc.b 67,80,77,46,83,89,83,0
L23:.dc.b 13,10,10,67,80,47,77,45,54,56,75,40,116,109,41,32,86,101,114,115,105,111,110,32,49,46,49,32,0
L24:.dc.b 10,13,67,111,112,121,114,105,103,104,116,32,40,99,41,32,49,57,56,50,32,68,105,103,105,116,97,108,32,82,101,115,101,97,114,99,104,44,32,73,110,99,46,13,10,10,0
