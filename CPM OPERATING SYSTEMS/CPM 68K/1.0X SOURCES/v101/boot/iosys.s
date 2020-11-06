.globl _bios1
.globl _bios2
.globl _bios3
.globl _bios4
.globl _bios5
.globl _udiv
.globl _do_phio
.text
_do_phio:
~~do_phio:
~dparmp=R11
~iopdiv=-4
~iop=R13
~hdrp=R12
link R14,#-4
movem.l R7-R7/R11-R13,-(sp)
move.l 8(R14),R13
move.b (R13),R0
ext.w R0
bra L3
L4:
move.b 1(R13),R0
ext.w R0
move R0,(sp)
move.b 3(R13),R0
ext.w R0
move R0,-(sp)
move #9,-(sp)
jsr _bios4
add #4,sp
move.l R0,14(R13)
bra L2
L5:
move.l 14(R13),R12
move.l 14(R12),R11
move.l R14,(sp)
sub.l #4,(sp)
move (R11),-(sp)
move.l 4(R13),-(sp)
jsr _udiv
add #6,sp
move -4(R14),(sp)
move 14(R11),R0
add R0,(sp)
move #10,-(sp)
jsr _bios2
add #2,sp
move.l (R12),(sp)
move -2(R14),-(sp)
move #16,-(sp)
jsr _bios5
add #4,sp
move R0,(sp)
move #11,-(sp)
jsr _bios2
add #2,sp
move.l 10(R13),(sp)
move #12,-(sp)
jsr _bios3
add #2,sp
move #13,(sp)
jsr _bios1
bra L1
bra L2
L3:tst R0
beq L4
cmp #1,R0
beq L5
bra L2
L2:
clr R0
bra L1
L1:tst.l (sp)+
movem.l (sp)+,R11-R13
unlk R14
rts
.data
