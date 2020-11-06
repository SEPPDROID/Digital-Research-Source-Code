.globl _bios1
.globl _bios2
.globl _bios3
.globl _bios4
.globl _bios5
.globl _copyrt
.data
_copyrt:
.dc.l L1
.globl _serial
.data
_serial:
.dc.l L2
.globl _seldisk
.globl _openfile
.globl _dirscan
.globl _seqread
.globl _gbls
.comm _gbls,16
.globl __bdos
.text
__bdos:
~~_bdos:
~infop=R13
~info=R6
~func=R7
link R14,#0
movem.l R5-R7/R13-R13,-(sp)
move 8(R14),R7
move 10(R14),R6
move.l 12(R14),R13
move R7,R0
bra L5
L6:
move.b R6,R0
ext.w R0
move R0,(sp)
move #4,-(sp)
jsr _bios2
add #2,sp
bra L4
L7:
move R6,(sp)
jsr _seldisk
bra L3
L8:
clr.b (R13)
clr.b 12(R13)
clr.b 14(R13)
move.l R13,(sp)
move.l #_openfile,-(sp)
jsr _dirscan
add #4,sp
bra L3
L9:
clr.b (R13)
move.l R13,(sp)
jsr _seqread
bra L3
L10:
move.l R13,12+_gbls
bra L4
L11:
move #65535,R0
bra L3
bra L4
L5:ext.l R0
move.l #L12,R8
move #5,R1
L13:cmp.l (R8)+,R0
dbeq R1,L13
move.l 20(R8),R8
jmp (R8)
.data
L12:.dc.l 2
.dc.l 14
.dc.l 15
.dc.l 20
.dc.l 26
.dc.l 0
.dc.l L6
.dc.l L7
.dc.l L8
.dc.l L9
.dc.l L10
.dc.l L11
.text
L4:
clr R0
bra L3
L3:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.data
L1:.dc.b 67,80,47,77,45,54,56,75,40,116,109,41,44,32,86,101,114,115,105,111,110,32,49,46,49,44,32,67,111,112,121,114,105,103,104,116,32,40,99,41,32,49,57,56,51,44,32,68,105,103,105,116,97,108,32,82,101,115,101,97,114,99,104,0
L2:.dc.b 88,88,88,88,45,48,48,48,48,45,54,53,52,51,50,49,0
