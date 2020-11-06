.globl _do_phio
.globl _rdsec
.text
_rdsec:
~~rdsec:
~rwpkt=-18
~secnum=8
~dma=12
link R14,#-22
move.b #1,-18(R14)
clr.b -17(R14)
move.l 8(R14),-14(R14)
move.l 12(R14),-8(R14)
move.l _gbls,-4(R14)
move.l R14,(sp)
add.l #-18,(sp)
jsr _do_phio
bra L1
L1:unlk R14
rts
.globl _dir_rd
.text
_dir_rd:
~~dir_rd:
~secnum=8
link R14,#-4
move.l 4+_gbls,(sp)
move 8(R14),R8
move.l R8,-(sp)
jsr _rdsec
add #4,sp
bra L2
L2:unlk R14
rts
.globl _dirscan
.text
_dirscan:
~~dirscan:
~funcp=8
~i=R7
~dirsec=R6
~dparmp=R12
~fcbp=R13
link R14,#0
movem.l R5-R7/R12-R13,-(sp)
move.l 12(R14),R13
move.l 8+_gbls,R12
clr R7
L5:
cmp 8(R12),R7
bhi L4
move R7,R0
and #3,R0
bne L7
move R7,R6
lsr #2,R6
move R6,(sp)
jsr _dir_rd
tst R0
beq L8
move #65535,R0
bra L3
L8:L7:
move R7,-(sp)
move R7,R0
and #3,R0
lsl #5,R0
swap R0
clr R0
swap R0
add.l 4+_gbls,R0
move.l R0,-(sp)
move.l R13,-(sp)
move.l 8(R14),R8
jsr (R8)
add #10,sp
tst R0
beq L9
move R7,R0
and #3,R0
bra L3
L9:L6:
add #1,R7
bra L5
L4:
move #255,R0
bra L3
L3:tst.l (sp)+
movem.l (sp)+,R6-R7/R12-R13
unlk R14
rts
.data
