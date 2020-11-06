.globl _copyrigh
.data
_copyrigh:
.dc.b 67,111,112,121,114,105,103,104,116,32,49,57,56,50,44,32,68,105,103,105,116,97,108,32,82,101,115,101,97,114,99,104,0
.even
.globl _dirbuf
.comm _dirbuf,128
.globl _csv0
.comm _csv0,16
.globl _csv1
.comm _csv1,16
.globl _csv2
.comm _csv2,16
.globl _alv0
.comm _alv0,32
.globl _alv1
.comm _alv1,32
.globl _alv2
.comm _alv2,2002
.globl _dpb0
.data
_dpb0:
.dc.w 26
.dc.w 775
.dc.w 0
.dc.w 242
.dc.w 63
.dc.w -16384
.dc.w 16
.dc.w 2
.globl _dpb1
.data
_dpb1:
.dc.w 26
.dc.w 1039
.dc.w 0
.dc.w 242
.dc.w 63
.dc.w -16384
.dc.w 16
.dc.w 2
.globl _dpb2
.data
_dpb2:
.dc.w 26
.dc.w 775
.dc.w 0
.dc.w 16008
.dc.w 63
.dc.w -16384
.dc.w 16
.dc.w 2
.globl _xlt
.data
_xlt:
.dc.b 1
.dc.b 7
.dc.b 13
.dc.b 19
.dc.b 25
.dc.b 5
.dc.b 11
.dc.b 17
.dc.b 23
.dc.b 3
.dc.b 9
.dc.b 15
.dc.b 21
.dc.b 2
.dc.b 8
.dc.b 14
.dc.b 20
.dc.b 26
.dc.b 6
.dc.b 12
.dc.b 18
.dc.b 24
.dc.b 4
.dc.b 10
.dc.b 16
.dc.b 22
.globl _dphtab
.data
_dphtab:
.dc.l _xlt
.dc.w 0
.dc.w 0
.dc.w 0
.dc.l _dirbuf
.dc.l _dpb0
.dc.l _csv0
.dc.l _alv0
.dc.l _xlt
.dc.w 0
.dc.w 0
.dc.w 0
.dc.l _dirbuf
.dc.l _dpb0
.dc.l _csv1
.dc.l _alv1
.dc.w 0,0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.l _dirbuf
.dc.l _dpb2
.dc.l _csv2
.dc.l _alv2
.dc.w 0,0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.l _dirbuf
.dc.l _dpb2
.dc.l _csv2
.dc.l _alv2
.globl _memtab
.data
_memtab:
.dc.w 1
.dc.w 0
.dc.w 1024
.dc.w 1
.dc.w 31744
.globl _iobyte
.comm _iobyte,2
.globl _settrk
.comm _settrk,2
.globl _setsec
.comm _setsec,2
.globl _setdsk
.comm _setdsk,2
.globl _setdma
.comm _setdma,4
.globl _trkbuf
.comm _trkbuf,3328
.globl _tbvalid
.data
_tbvalid:
.dc.w 0
.globl _tbdirty
.data
_tbdirty:
.dc.w 0
.globl _tbtrk
.comm _tbtrk,2
.globl _tbdsk
.comm _tbdsk,2
.globl _hmpack
.data
_hmpack:
.dc.w 512
.dc.w 1792
.dc.w 0
.dc.w 768
.globl _rwpack
.data
_rwpack:
.dc.w 512
.dc.w 5376
.dc.w 4097
.dc.w 13
.dc.w 256
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 0
.dc.w 768
.globl _cnvdsk
.data
_cnvdsk:
.dc.b 4
.dc.b 5
.dc.b 0
.dc.b 1
.globl _dskstate
.data
_dskstate:
.dc.w 0
.globl _portinit
.text
_portinit:
~~portinit:
~port=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
move.l 8(R14),R13
move.b #3,(R13)
move.b #17,(R13)
L2:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _portstat
.text
_portstat:
~~portstat:
~port=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
move.l 8(R14),R13
btst #0,(R13)
bne L10000
clr R0
bra L10001
L10000:move #1,R0
L10001:bra L3
L3:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _portin
.text
_portin:
~~portin:
~port=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
move.l 8(R14),R13
L6:
move.l R13,(sp)
jsr _portstat
tst R0
bne L5
bra L6
L5:
move.b 2(R13),R0
ext.w R0
bra L4
L4:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _portout
.text
_portout:
~~portout:
~ch=R7
~port=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
move.l 8(R14),R13
move.b 13(R14),R7
L9:
btst #1,(R13)
bne L8
bra L9
L8:
move.b R7,2(R13)
L7:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _bioserr
.text
_bioserr:
~~bioserr:
~errmsg=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
move.l 8(R14),R13
move.l #L11,(sp)
jsr _printstr
move.l R13,(sp)
jsr _printstr
move.l #L12,(sp)
jsr _printstr
L14:
bra L14
L13:L10:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _printstr
.text
_printstr:
~~printstr:
~s=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
move.l 8(R14),R13
L17:
tst.b (R13)
beq L16
move.b (R13),R0
ext.w R0
move R0,(sp)
move.l #$ffee011,-(sp)
jsr _portout
add #4,sp
add.l #1,R13
bra L17
L16:L15:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _dskia
.globl _setimask
.globl _dskic
.text
_dskic:
~~dskic:
link R14,#-4
move _dskstate,R0
bra L20
L21:L22:L23:
cmp.b #6,$ff0183
beq L10002
cmp.b #21,$ff0183
bne L24
L10002:clr.b $ff0183
L24:
cmp.b #128,$ff0181
bne L25
move.b #6,$ff0103
clr.b $ff0181
clr.b $ff000d
L25:bra L19
L26:
cmp.b #6,$ff0183
bne L27
clr.b $ff0183
move #3,_dskstate
bra L28
L27:
cmp.b #21,$ff0183
bne L29
clr.b $ff0183
move #2,_dskstate
bra L30
L29:
move.l #L31,(sp)
jsr _bioserr
L30:L28:
cmp.b #128,$ff0181
bne L32
move #4,_dskstate
L32:bra L19
L33:
cmp.b #128,$ff0181
beq L34
move.l #L35,(sp)
jsr _bioserr
L34:
move #4,_dskstate
clr.b $ff0181
move.b #6,$ff0103
clr.b $ff000d
bra L19
bra L19
L20:cmp #4,R0
bhi L19
asl #2,R0
move R0,R8
add.l #L36,R8
move.l (R8),R8
jmp (R8)
.data
L36:.dc.l L21
.dc.l L26
.dc.l L23
.dc.l L33
.dc.l L22
.text
L19:L18:unlk R14
rts
.globl _sendpkt
.text
_sendpkt:
~~sendpkt:
~imsave=R6
~pktadr=R13
~iopackp=R12
~pktsize=R7
link R14,#0
movem.l R5-R7/R12-R13,-(sp)
move.l 8(R14),R13
move 12(R14),R7
L39:
tst.b $ff0101
beq L38
bra L39
L38:
cmp.b #21,$ff0183
bne L40
move.l #L41,(sp)
jsr _bioserr
L40:
clr.b $ff0183
clr.b $ff0181
move.l #$ff0105,R12
L44:
move.b (R13)+,(R12)
add.l #2,R12
sub #1,R7
L43:
tst R7
bne L44
L42:
move.b #128,$ff0101
move #7,(sp)
jsr _setimask
move R0,R6
move #1,_dskstate
clr.b $ff000d
move R6,(sp)
jsr _setimask
L37:tst.l (sp)+
movem.l (sp)+,R6-R7/R12-R13
unlk R14
rts
.globl _dskxfer
.text
_dskxfer:
~~dskxfer:
~cmd=R5
~dsk=R7
~trk=R6
~bufp=R13
link R14,#0
movem.l R4-R7/R13-R13,-(sp)
move 8(R14),R7
move 10(R14),R6
move.l 12(R14),R13
move 16(R14),R5
move R7,R0
ext.l R0
add.l #_cnvdsk,R0
move.l R0,R8
move.b (R8),3+_rwpack
move.l R13,10+_rwpack
move R6,R0
muls #13,R0
move.l R0,16+_rwpack
move.b R5,4+_rwpack
move #21,(sp)
move.l #_rwpack,-(sp)
jsr _sendpkt
add #4,sp
L47:
cmp #1,_dskstate
bne L46
bra L47
L46:
cmp #2,_dskstate
bne L48
move.l #L49,(sp)
jsr _bioserr
L48:L51:
cmp #3,_dskstate
bne L50
bra L51
L50:L45:tst.l (sp)+
movem.l (sp)+,R5-R7/R13-R13
unlk R14
rts
.globl _flush
.text
_flush:
~~flush:
link R14,#-4
tst _tbvalid
beq L53
move #32,(sp)
move.l #_trkbuf,-(sp)
move _tbtrk,-(sp)
move _tbdsk,-(sp)
jsr _dskxfer
add #8,sp
L53:
clr _tbdirty
clr R0
bra L52
L52:unlk R14
rts
.globl _filltb
.text
_filltb:
~~filltb:
link R14,#-4
tst _tbvalid
beq L55
tst _tbdirty
beq L55
jsr _flush
L55:
move #16,(sp)
move.l #_trkbuf,-(sp)
move _settrk,-(sp)
move _setdsk,-(sp)
jsr _dskxfer
add #8,sp
move #1,_tbvalid
clr _tbdirty
move _settrk,_tbtrk
move _setdsk,_tbdsk
L54:unlk R14
rts
.globl _read
.text
_read:
~~read:
~i=R7
~p=R13
~q=R12
link R14,#0
movem.l R6-R7/R12-R13,-(sp)
tst _tbvalid
beq L10003
move _tbtrk,R0
cmp _settrk,R0
bne L10003
move _tbdsk,R0
cmp _setdsk,R0
beq L57
L10003:jsr _filltb
L57:
move _setsec,R0
sub #1,R0
asl #7,R0
ext.l R0
move.l R0,R13
add.l #_trkbuf,R13
move.l _setdma,R12
move #128,R7
L60:
move.b (R13)+,(R12)+
sub #1,R7
L59:
tst R7
bne L60
L58:
clr R0
bra L56
L56:tst.l (sp)+
movem.l (sp)+,R7-R7/R12-R13
unlk R14
rts
.globl _write
.text
_write:
~~write:
~i=R7
~p=R13
~q=R12
~mode=9
link R14,#0
movem.l R6-R7/R12-R13,-(sp)
tst _tbvalid
beq L10004
move _tbtrk,R0
cmp _settrk,R0
bne L10004
move _tbdsk,R0
cmp _setdsk,R0
beq L62
L10004:jsr _filltb
L62:
move _setsec,R0
sub #1,R0
asl #7,R0
ext.l R0
move.l R0,R13
add.l #_trkbuf,R13
move.l _setdma,R12
move #128,R7
L65:
move.b (R12)+,(R13)+
sub #1,R7
L64:
tst R7
bne L65
L63:
move #1,_tbdirty
cmp.b #1,9(R14)
bne L66
jsr _flush
L66:
clr R0
bra L61
L61:tst.l (sp)+
movem.l (sp)+,R7-R7/R12-R13
unlk R14
rts
.globl _sectran
.text
_sectran:
~~sectran:
~s=8
~xp=10
link R14,#-4
move 8(R14),R8
move.l 10(R14),R9
move.b 0(R8,R9.l),R0
ext.w R0
bra L67
L67:unlk R14
rts
.globl _setxvect
.text
_setxvect:
~~setxvect:
~oldval=R7
~vloc=R13
~vval=10
~vnum=8
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
move 8(R14),R0
ext.l R0
asl.l #2,R0
move.l R0,R13
move.l (R13),R7
move.l 10(R14),(R13)
move.l R7,R0
bra L68
L68:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _seldsk
.text
_seldsk:
~~seldsk:
~logged=11
~dsk=R7
~dphp=R13
~check=R6
link R14,#0
movem.l R5-R7/R13-R13,-(sp)
move.b 9(R14),R7
cmp.b #3,R7
ble L70
move.l #$0,R0
bra L69
L70:
move.b R7,R0
ext.w R0
move R0,_setdsk
move.b R7,R0
ext.w R0
muls #26,R0
move.l R0,R13
add.l #_dphtab,R13
tst.b 11(R14)
bne L71
move.l #_cnvdsk,R0
move _setdsk,R1
ext.l R1
add.l R1,R0
move.l R0,R8
move.b (R8),3+_hmpack
move.b #48,4+_hmpack
move.b #2,5+_hmpack
move.b #6,$ff0183
tst.b $ff0101
beq L72
move.l #L73,(sp)
jsr _bioserr
bra L74
L72:
move #7,(sp)
move.l #_hmpack,-(sp)
jsr _sendpkt
add #4,sp
L74:L76:
cmp #1,_dskstate
bne L75
bra L76
L75:
cmp #2,_dskstate
bne L77
move.l #L78,(sp)
jsr _bioserr
L77:L80:
cmp #3,_dskstate
bne L79
bra L80
L79:
move.b $ff019d,R6
move.b R6,R0
ext.w R0
bra L82
L83:
move.l #_dpb0,14(R13)
bra L81
L84:
move.l #_dpb1,14(R13)
bra L81
L85:
move.l #L86,(sp)
jsr _bioserr
bra L81
bra L81
L82:cmp #3,R0
beq L83
cmp #7,R0
beq L84
bra L85
L81:L71:
move.l R13,R0
bra L69
L69:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.globl _biosinit
.text
_biosinit:
~~biosinit:
link R14,#-4
move.l #$ffee011,(sp)
jsr _portinit
move.l #$ffee015,(sp)
jsr _portinit
L87:unlk R14
rts
.globl _bios
.text
_bios:
~~bios:
~d0=8
~d1=10
~d2=14
link R14,#-4
move 8(R14),R0
bra L90
L91:
jsr _init
bra L89
L92:
jsr _wboot
bra L89
L93:
move.l #$ffee011,(sp)
jsr _portstat
ext.l R0
bra L88
bra L89
L94:
move.l #$ffee011,(sp)
jsr _portin
ext.l R0
bra L88
bra L89
L95:
move.l 10(R14),R0
move R0,(sp)
move.l #$ffee011,-(sp)
jsr _portout
add #4,sp
bra L89
L96:L97:
move.l 10(R14),R0
move R0,(sp)
move.l #$ffee015,-(sp)
jsr _portout
add #4,sp
bra L89
L98:
move.l #$ffee015,(sp)
jsr _portin
ext.l R0
bra L88
bra L89
L99:
clr _settrk
bra L89
L100:
move.l 14(R14),R0
move R0,(sp)
move.l 10(R14),R0
move R0,-(sp)
jsr _seldsk
add #2,sp
bra L88
bra L89
L101:
move.l 10(R14),R0
move R0,_settrk
bra L89
L102:
move.l 10(R14),R0
move R0,_setsec
bra L89
L103:
move.l 10(R14),_setdma
bra L89
L104:
jsr _read
ext.l R0
bra L88
bra L89
L105:
move.l 10(R14),R0
move R0,(sp)
jsr _write
ext.l R0
bra L88
bra L89
L106:
move.l #$ffee015,(sp)
jsr _portstat
ext.l R0
bra L88
bra L89
L107:
move.l 14(R14),(sp)
move.l 10(R14),R0
move R0,-(sp)
jsr _sectran
add #2,sp
ext.l R0
bra L88
bra L89
L108:
move.l #_memtab,R0
bra L88
bra L89
L109:
move _iobyte,R0
ext.l R0
bra L88
bra L89
L110:
move.l 10(R14),R0
move R0,_iobyte
bra L89
L111:
jsr _flush
ext.l R0
bra L88
bra L89
L112:
move.l 14(R14),(sp)
move.l 10(R14),R0
move R0,-(sp)
jsr _setxvect
add #2,sp
bra L88
bra L89
bra L89
L90:cmp #22,R0
bhi L89
asl #2,R0
move R0,R8
add.l #L113,R8
move.l (R8),R8
jmp (R8)
.data
L113:.dc.l L91
.dc.l L92
.dc.l L93
.dc.l L94
.dc.l L95
.dc.l L96
.dc.l L97
.dc.l L98
.dc.l L99
.dc.l L100
.dc.l L101
.dc.l L102
.dc.l L103
.dc.l L104
.dc.l L105
.dc.l L106
.dc.l L107
.dc.l L89
.dc.l L108
.dc.l L109
.dc.l L110
.dc.l L111
.dc.l L112
.text
L89:L88:unlk R14
rts
.data
L11:.dc.b 10,13,66,73,79,83,32,69,82,82,79,82,32,45,45,32,0
L12:.dc.b 46,10,13,0
L31:.dc.b 69,120,112,101,99,116,101,100,32,65,67,75,32,111,114,32,78,65,75,0
L35:.dc.b 69,120,112,101,99,116,101,100,32,115,116,97,116,117,115,32,112,97,99,107,101,116,32,102,114,111,109,32,73,80,67,0
L41:.dc.b 78,65,75,32,102,114,111,109,32,73,80,67,0
L49:.dc.b 78,65,75,32,102,114,111,109,32,73,80,67,0
L73:.dc.b 69,82,82,32,54,0
L78:.dc.b 78,65,75,32,102,114,111,109,32,73,80,67,0
L86:.dc.b 69,82,82,32,55,0
