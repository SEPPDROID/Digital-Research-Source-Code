.globl _do_phio
.globl _swap
.globl _seldisk
.text
_seldisk:
~~seldisk:
~i=R6
~p=R13
~dsknum=R7
~selpkt=-18
link R14,#-18
movem.l R5-R7/R13-R13,-(sp)
move.b 9(R14),R7
move.b R7,R0
ext.w R0
and #255,R0
cmp #15,R0
ble L2
move #65535,R0
bra L1
L2:
clr.b -18(R14)
move.b R7,-15(R14)
clr.b -17(R14)
move.l R14,(sp)
add.l #-18,(sp)
jsr _do_phio
move.l -4(R14),_gbls
bne L3
move #65535,R0
bra L1
L3:
move.l _gbls,R8
move.l 10(R8),4+_gbls
move.l _gbls,R8
move.l 14(R8),8+_gbls
clr R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.globl _move
.text
_move:
~~move:
~i=R7
~p1=R13
~p2=R12
link R14,#0
movem.l R6-R7/R12-R13,-(sp)
move.l 8(R14),R13
move.l 12(R14),R12
move 16(R14),R7
L6:
move R7,R0
sub #1,R7
tst R0
beq L5
move.b (R13)+,(R12)+
bra L6
L5:L4:tst.l (sp)+
movem.l (sp)+,R7-R7/R12-R13
unlk R14
rts
.globl _match
.text
_match:
~~match:
~i=R7
~p1=R13
~p2=R12
~chk_ext=17
~temp=R6
link R14,#0
movem.l R5-R7/R12-R13,-(sp)
move.l 8(R14),R13
move.l 12(R14),R12
move #12,R7
L10:
move.b (R13),R6
eor.b #63,R6
move.b (R13)+,R0
ext.w R0
move.b (R12)+,R1
ext.w R1
eor R1,R0
and #127,R0
beq L11
tst.b R6
beq L11
clr R0
bra L7
L11:
sub #1,R7
L9:
tst R7
bne L10
L8:
tst.b 17(R14)
beq L12
cmp.b #63,(R13)
beq L13
move.b (R13),R0
ext.w R0
move.b (R12),R1
ext.w R1
eor R1,R0
move.l 8+_gbls,R9
move.b 4(R9),R1
ext.w R1
not.b R1
and R1,R0
beq L13
clr R0
bra L7
L13:
add.l #2,R13
add.l #2,R12
move.b (R13),R0
ext.w R0
move.b (R12),R1
ext.w R1
eor R1,R0
and #63,R0
beq L14
clr R0
bra L7
L14:L12:
move #1,R0
bra L7
L7:tst.l (sp)+
movem.l (sp)+,R6-R7/R12-R13
unlk R14
rts
.globl _openfile
.text
_openfile:
~~openfile:
~fcb_ext=R7
~dirindx=16
~rtn=R6
~fcbp=R13
~dirp=12
link R14,#0
movem.l R5-R7/R13-R13,-(sp)
move.l 8(R14),R13
move #1,(sp)
move.l 12(R14),-(sp)
move.l R13,-(sp)
jsr _match
add #8,sp
move.b R0,R6
beq L16
move.b 12(R13),R7
move #32,(sp)
move.l R13,-(sp)
move.l 12(R14),-(sp)
jsr _move
add #8,sp
move.b R7,12(R13)
L16:
move.b R6,R0
ext.w R0
bra L15
L15:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.data
