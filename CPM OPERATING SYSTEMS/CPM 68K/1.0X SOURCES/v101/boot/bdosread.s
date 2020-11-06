.globl _rdsec
.globl _swap
.globl _dirscan
.globl _openfile
.globl _blkindx
.text
_blkindx:
~~blkindx:
~i=R7
~blkshf=R6
~dparmp=R12
~fcbp=R13
link R14,#0
movem.l R5-R7/R12-R13,-(sp)
move.l 8(R14),R13
move.l 8+_gbls,R12
move.b 2(R12),R6
ext.w R6
move.b 12(R13),R0
ext.w R0
move.b 4(R12),R1
ext.w R1
and R1,R0
move #7,R1
sub R6,R1
asl R1,R0
move R0,R7
move R7,R0
move.b 32(R13),R1
ext.w R1
and #255,R1
move R6,R2
asr R2,R1
add R1,R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R6-R7/R12-R13
unlk R14
rts
.globl _blknum
.text
_blknum:
~~blknum:
~index=R7
~wrdfcb=14
~fcbp=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
move.l 8(R14),R13
move 12(R14),R7
tst 14(R14)
beq L3
move R7,R8
add.l R8,R8
move 16(R13,R8.l),(sp)
jsr _swap
bra L2
bra L4
L3:
move.b 16(R13,R7),R0
ext.w R0
and #255,R0
bra L2
L4:L2:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _do_io
.text
_do_io:
~~do_io:
~block=8
~dparmp=R13
~lsec=-4
~rcrd=11
link R14,#-4
movem.l R7-R7/R13-R13,-(sp)
move.l 8+_gbls,R13
clr.l R0
move 8(R14),R0
move.b 2(R13),R1
ext.w R1
ext.l R1
asl.l R1,R0
move.b 3(R13),R1
ext.w R1
move.b 11(R14),R2
ext.w R2
and R2,R1
ext.l R1
add.l R1,R0
move.l R0,-4(R14)
move.l 12+_gbls,(sp)
move.l -4(R14),-(sp)
jsr _rdsec
add #4,sp
bra L5
L5:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _new_ext
.text
_new_ext:
~~new_ext:
~mod=R7
~ext=R6
~fcbp=R13
link R14,#0
movem.l R5-R7/R13-R13,-(sp)
move.l 8(R14),R13
move.b 14(R13),R7
and.b #63,R7
move.b 12(R13),R6
add.b #1,R6
cmp.b #32,R6
blt L7
clr.b R6
add.b #1,R7
L7:
cmp.b #64,R7
blt L8
move #6,R0
bra L6
L8:
move.b R7,R0
ext.w R0
move.b 14(R13),R1
ext.w R1
and #63,R1
cmp R1,R0
bne L9
move.b R6,R0
ext.w R0
move.b 12(R13),R1
ext.w R1
eor R1,R0
move.l 8+_gbls,R9
move.b 4(R9),R1
ext.w R1
not.b R1
and R1,R0
and #31,R0
bne L10
move.b R6,12(R13)
clr R0
bra L6
L10:L9:
move.b R7,14(R13)
move.b R6,12(R13)
move.l R13,(sp)
move.l #_openfile,-(sp)
jsr _dirscan
add #4,sp
cmp #255,R0
blo L11
move #4,R0
bra L6
L11:
clr R0
bra L6
L6:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.globl _calcext
.text
_calcext:
~~calcext:
~i=R7
~p=R12
~fcbp=R13
link R14,#0
movem.l R6-R7/R12-R13,-(sp)
move.l 8(R14),R13
move #15,R7
lea 32(R13),R12
L15:
tst.b -(R12)
bne L13
sub #1,R7
L14:
tst R7
bne L15
L13:
move.l 8+_gbls,R8
cmp #255,6(R8)
bls L16
lsr R7
L16:
clr.l R0
move #7,R0
move.l 8+_gbls,R9
move.b 2(R9),R1
ext.w R1
sub R1,R0
lsr R0,R7
move R7,R0
move.l 8+_gbls,R9
move.b 4(R9),R1
ext.w R1
not.b R1
move.b 12(R13),R2
ext.w R2
and R2,R1
and #31,R1
or R1,R0
bra L12
L12:tst.l (sp)+
movem.l (sp)+,R7-R7/R12-R13
unlk R14
rts
.globl _get_rc
.text
_get_rc:
~~get_rc:
~ext=R7
~fcbp=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
move.l 8(R14),R13
move.l R13,(sp)
jsr _calcext
move R0,R7
move.b 12(R13),R0
ext.w R0
cmp R0,R7
bne L18
move.b 15(R13),R0
ext.w R0
and #255,R0
bra L17
bra L19
L18:
move.b 12(R13),R0
ext.w R0
cmp R0,R7
bls L20
move #128,R0
bra L17
bra L21
L20:
clr R0
bra L17
L21:L19:L17:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _seqread
.text
_seqread:
~~seqread:
~block=R7
~index=R6
~bigfile=R4
~fcbp=R13
~parm=R5
link R14,#0
movem.l R3-R7/R13-R13,-(sp)
move.l 8(R14),R13
move.l 8+_gbls,R8
move 6(R8),R4
and #-256,R4
cmp.b #128,32(R13)
bne L23
move.l R13,(sp)
jsr _new_ext
tst R0
beq L24
move #1,R0
bra L22
L24:
clr.b 32(R13)
L23:
move.l R13,(sp)
jsr _get_rc
move.b 32(R13),R1
ext.w R1
and #255,R1
cmp R1,R0
bhi L25
move #1,R0
bra L22
L25:
move.l R13,(sp)
jsr _blkindx
move R0,R6
move R4,(sp)
move R6,-(sp)
move.l R13,-(sp)
jsr _blknum
add #6,sp
move R0,R7
tst R7
bne L26
move #1,R0
bra L22
L26:
move.b 32(R13),R0
ext.w R0
move R0,(sp)
move R7,-(sp)
jsr _do_io
add #2,sp
add.b #1,32(R13)
bra L22
L22:tst.l (sp)+
movem.l (sp)+,R4-R7/R13-R13
unlk R14
rts
.data
