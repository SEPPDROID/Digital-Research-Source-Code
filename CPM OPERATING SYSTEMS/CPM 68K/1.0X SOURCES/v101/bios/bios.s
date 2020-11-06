.globl _copyrigh
.data
_copyrigh:
.dc.b 67,111,112,121,114,105,103,104,116,32,49,57,56,51,44,32,68,105,103,105,116,97,108,32,82,101,115,101,97,114,99,104,0
.even
.globl _dirbuf
.comm _dirbuf,128
.globl _csv0
.comm _csv0,16
.globl _csv1
.comm _csv1,16
.globl _csv2
.comm _csv2,256
.globl _csv3
.comm _csv3,256
.globl _csv4
.comm _csv4,16
.globl _alv0
.comm _alv0,32
.globl _alv1
.comm _alv1,32
.globl _alv2
.comm _alv2,412
.globl _alv3
.comm _alv3,412
.globl _alv4
.comm _alv4,48
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
.globl _dpb2
.data
_dpb2:
.dc.w 32
.dc.w 1311
.dc.w 256
.dc.w 3288
.dc.w 1023
.dc.w 65280
.dc.w 256
.dc.w 4
.globl _dpb3
.data
_dpb3:
.dc.w 32
.dc.w 1039
.dc.w 0
.dc.w 191
.dc.w 63
.dc.w 0
.dc.w 16
.dc.w 0
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
*line 247
.dc.l _xlt
.dc.w 0
.dc.w 0
.dc.w 0
*line 247
.dc.l _dirbuf
*line 247
.dc.l _dpb0
*line 247
.dc.l _csv0
*line 247
.dc.l _alv0
*line 248
.dc.l _xlt
.dc.w 0
.dc.w 0
.dc.w 0
*line 248
.dc.l _dirbuf
*line 248
.dc.l _dpb0
*line 248
.dc.l _csv1
*line 248
.dc.l _alv1
.dc.w 0,0
.dc.w 0
.dc.w 0
.dc.w 0
*line 250
.dc.l _dirbuf
*line 250
.dc.l _dpb2
*line 250
.dc.l _csv2
*line 250
.dc.l _alv2
.dc.w 0,0
.dc.w 0
.dc.w 0
.dc.w 0
*line 251
.dc.l _dirbuf
*line 251
.dc.l _dpb2
*line 251
.dc.l _csv3
*line 251
.dc.l _alv3
.dc.w 0,0
.dc.w 0
.dc.w 0
.dc.w 0
*line 255
.dc.l _dirbuf
*line 255
.dc.l _dpb3
*line 255
.dc.l _csv4
*line 255
.dc.l _alv4
.globl _memtab
.data
_memtab:
.dc.w 1
.dc.w 0,1024
.dc.w 1,18432
.globl _memdsk
.data
_memdsk:
.dc.w 2,0
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
.globl _firstbuf
.comm _firstbuf,4
.globl _lastbuf
.comm _lastbuf,4
.globl _tbuf
.comm _tbuf,16424
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
.globl _fmtpack
.data
_fmtpack:
.dc.w 512
.dc.w 1792
.dc.w 16386
.dc.w 768
.globl _cnvdsk
.data
_cnvdsk:
.dc.b 4
.dc.b 5
.dc.b 0
.dc.b 1
.ds.b 1
.even
.globl _rcnvdsk
.data
_rcnvdsk:
.dc.b 2
.dc.b 3
.dc.b 0
.dc.b 0
.dc.b 0
.dc.b 1
.globl _ipcstate
.comm _ipcstate,2
.globl _actvdsk
.comm _actvdsk,2
.globl _intcount
.comm _intcount,4
.globl _dskstate
.comm _dskstate,20
.globl _portinit
.text
_portinit:
~~portinit:
~port=R13
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
*line 460
move.l 8(R14),R13
*line 460
move.b #3,(R13)
*line 461
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
*line 474
move.l 8(R14),R13
*line 474
btst #0,(R13)
beq L4
*line 474
move #255,R0
bra L3
bra L5
L4:
*line 475
clr R0
bra L3
L5:L3:tst.l (sp)+
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
*line 486
move.l 8(R14),R13
L8:
*line 486
move.l R13,(sp)
jsr _portstat
tst R0
bne L7
bra L8
L7:
*line 487
move.b 2(R13),R0
ext.w R0
bra L6
L6:tst.l (sp)+
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
*line 499
move.l 8(R14),R13
*line 499
move.b 13(R14),R7
L11:
*line 499
btst #1,(R13)
bne L10
bra L11
L10:
*line 500
move.b R7,2(R13)
L9:tst.l (sp)+
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
*line 513
move.l 8(R14),R13
*line 513
move.l #L13,(sp)
jsr _printstr
*line 514
move.l R13,(sp)
jsr _printstr
*line 515
move.l #L14,(sp)
jsr _printstr
L12:tst.l (sp)+
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
*line 521
move.l 8(R14),R13
L17:
*line 521
tst.b (R13)
beq L16
*line 521
move.b (R13),R0
ext.w R0
move R0,(sp)
move.l #$ffee011,-(sp)
jsr _portout
addq.l #4,sp
*line 521
add.l #1,R13
bra L17
L16:L15:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _dskic
.text
_dskic:
~~dskic:
~stpkt=-16
~workbyte=R7
link R14,#-16
movem.l R6-R7,-(sp)
*line 548
move.b $ff0183,R7
*line 550
cmp.b #6,R7
beq L10000
cmp.b #21,R7
bne L19
L10000:*line 551
cmp #1,_ipcstate
bne L20
*line 551
add.l #1,_intcount
bra L21
L20:
*line 552
clr.b $ff0183
L21:L19:
*line 555
move.b $ff0181,R7
*line 557
btst #7,R7
beq L22
*line 558
move.l R14,(sp)
add.l #-16,(sp)
jsr _getstpkt
*line 561
cmp.b #255,-15(R14)
bne L23
*line 564
move.l R14,(sp)
add.l #-16,(sp)
jsr _unsolst
*line 565
jsr _sendack
bra L24
L23:
*line 571
cmp #1,_ipcstate
bne L25
*line 571
add.l #1,_intcount
bra L26
L25:
*line 572
jsr _sendack
L26:L24:L22:L18:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _getstpkt
.text
_getstpkt:
~~getstpkt:
~i=R7
~p=R12
~q=R11
~stpktp=R13
link R14,#0
movem.l R6-R7/R11-R13,-(sp)
*line 589
move.l 8(R14),R13
*line 589
move.l R13,R12
*line 590
move.l #$ff0185,R11
*line 592
move #15,R7
L29:
*line 592
tst R7
beq L28
*line 594
move.b (R11),(R12)
*line 595
add.l #1,R12
*line 596
add.l #2,R11
L30:
*line 592
sub #1,R7
bra L29
L28:L27:tst.l (sp)+
movem.l (sp)+,R7-R7/R11-R13
unlk R14
rts
.globl _unsolst
.text
_unsolst:
~~unsolst:
~ready=R6
~stpktp=R13
~dev=R7
~dsp=R12
link R14,#0
movem.l R5-R7/R12-R13,-(sp)
*line 611
move.l 8(R14),R13
*line 611
move.b 3(R13),R0
ext.w R0
move R0,R8
move.l #_rcnvdsk,R9
move.b 0(R8,R9.l),R0
ext.w R0
move R0,R7
*line 612
btst #7,8(R13)
beq L10001
clr R0
bra L10002
L10001:move #1,R0
L10002:move R0,R6
*line 613
move R7,R0
asl #2,R0
ext.l R0
move.l R0,R12
add.l #_dskstate,R12
*line 615
tst R6
beq L10004
tst.b 2(R12)
beq L10003
L10004:tst R6
bne L32
tst.b 2(R12)
beq L32
L10003:*line 615
move.b #1,3(R12)
L32:
*line 616
move.b R6,2(R12)
*line 618
tst R6
bne L33
*line 618
move R7,(sp)
jsr _setinvld
L33:L31:tst.l (sp)+
movem.l (sp)+,R6-R7/R12-R13
unlk R14
rts
.globl _setinvld
.text
_setinvld:
~~setinvld:
~dsk=R7
~tbp=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
*line 634
move 8(R14),R7
*line 634
move.l _firstbuf,R13
L36:
*line 635
move.l R13,R0
beq L35
*line 637
cmp 4100(R13),R7
bne L37
*line 637
clr.b 4104(R13)
L37:
*line 638
move.l (R13),R13
bra L36
L35:L34:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _waitack
.text
_waitack:
~~waitack:
~imsave=R7
~work=R6
link R14,#0
movem.l R5-R7,-(sp)
L40:
*line 653
L42:
*line 655
tst.l _intcount
bne L41
bra L42
L41:
*line 657
move #7,(sp)
jsr _setimask
move R0,R7
*line 658
sub.l #1,_intcount
*line 659
move.b $ff0183,R6
*line 661
cmp.b #6,R6
beq L10005
cmp.b #21,R6
bne L43
L10005:*line 662
clr.b $ff0183
*line 663
move R7,(sp)
jsr _setimask
*line 664
cmp.b #6,R6
beq L10006
clr R0
bra L10007
L10006:move #1,R0
L10007:bra L38
L43:
*line 666
move R7,(sp)
jsr _setimask
bra L40
L39:L38:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _sendack
.text
_sendack:
~~sendack:
link R14,#-4
*line 678
clr.b $ff0181
*line 679
move.b #6,$ff0103
*line 680
clr.b $ff000d
L44:unlk R14
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
*line 695
move.l 8(R14),R13
*line 695
move 12(R14),R7
L47:
*line 695
tst.b $ff0101
beq L46
bra L47
L46:
*line 696
clr.b $ff0183
*line 697
clr.b $ff0181
*line 698
move.l #$ff0105,R12
L50:
*line 699
move.b (R13)+,(R12)
*line 699
add.l #2,R12
*line 699
sub #1,R7
L49:
*line 699
tst R7
bne L50
L48:
*line 700
move.b #128,$ff0101
*line 701
move #7,(sp)
jsr _setimask
move R0,R6
*line 702
move _actvdsk,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
move #1,(R8)
*line 703
move #1,_ipcstate
*line 704
move.l #$0,_intcount
*line 705
clr.b $ff000d
*line 706
move R6,(sp)
jsr _setimask
*line 707
jsr _waitack
L45:tst.l (sp)+
movem.l (sp)+,R6-R7/R12-R13
unlk R14
rts
.globl _dskwait
.text
_dskwait:
~~dskwait:
~stcom=11
~stval=12
~stpkt=-16
~imsave=R6
~dsk=R7
link R14,#-16
movem.l R5-R7,-(sp)
*line 722
move 8(R14),R7
*line 722
move #7,(sp)
jsr _setimask
move R0,R6
L53:
*line 724
tst.l _intcount
bne L52
move R7,R8
add.l R8,R8
add.l R8,R8
move.l #_dskstate,R9
tst.b 2(R8,R9.l)
beq L52
move R7,R8
add.l R8,R8
add.l R8,R8
move.l #_dskstate,R9
tst.b 3(R8,R9.l)
bne L52
*line 726
move R6,(sp)
jsr _setimask
*line 726
move #7,(sp)
jsr _setimask
move R0,R6
bra L53
L52:
*line 730
tst.l _intcount
beq L54
*line 731
sub.l #1,_intcount
*line 733
move.b $ff0181,R0
ext.w R0
and #128,R0
beq L55
*line 734
move.l R14,(sp)
add.l #-16,(sp)
jsr _getstpkt
*line 735
move R6,(sp)
jsr _setimask
*line 737
move.b -11(R14),R0
ext.w R0
cmp.b 11(R14),R0
bne L56
move -10(R14),R0
cmp 12(R14),R0
bne L56
*line 737
move #1,R0
bra L51
bra L57
L56:
*line 738
clr R0
bra L51
L57:L55:L54:
*line 741
move R6,(sp)
jsr _setimask
*line 742
clr R0
bra L51
L51:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _dskxfer
.text
_dskxfer:
~~dskxfer:
~result=R3
~sectcnt=R4
~cmd=R5
~dsk=R7
~trk=R6
~bufp=R13
link R14,#0
movem.l R2-R7/R13-R13,-(sp)
*line 763
move 8(R14),R7
*line 763
move 10(R14),R6
*line 763
move.l 12(R14),R13
*line 763
move 16(R14),R5
*line 763
move R7,R8
add.l #_cnvdsk,R8
move.b (R8),3+_rwpack
*line 764
move.l R13,10+_rwpack
*line 765
move R7,R0
muls #26,R0
move.l R0,R8
move.l #_dphtab,R9
move.l 14(R8,R9.l),R8
move (R8),R4
*line 766
move R6,R0
move R4,R1
asr #1,R1
muls R1,R0
move.l R0,16+_rwpack
*line 767
move.b R5,4+_rwpack
*line 768
move R4,R0
asr #1,R0
move R0,6+_rwpack
*line 777
move R7,_actvdsk
*line 778
move R7,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr.b 3(R8)
*line 779
move #21,(sp)
move.l #_rwpack,-(sp)
jsr _sendpkt
addq.l #4,sp
*line 780
clr (sp)
move #112,-(sp)
move R7,-(sp)
jsr _dskwait
addq.l #4,sp
move R0,R3
*line 781
jsr _sendack
*line 782
move R7,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr (R8)
*line 783
clr _ipcstate
*line 784
move R3,R0
bra L58
L58:tst.l (sp)+
movem.l (sp)+,R3-R7/R13-R13
unlk R14
rts
.globl _flush1
.text
_flush1:
~~flush1:
~ok=R7
~tbp=8
link R14,#0
movem.l R6-R7,-(sp)
*line 800
move.l 8(R14),R8
tst.b 4104(R8)
beq L60
move.l 8(R14),R8
tst.b 4105(R8)
beq L60
*line 800
move #32,(sp)
move.l 8(R14),-(sp)
add.l #4,(sp)
move.l 8(R14),R8
move 4102(R8),-(sp)
move.l 8(R14),R8
move 4100(R8),-(sp)
jsr _dskxfer
addq.l #8,sp
move R0,R7
bra L61
L60:
*line 801
move #1,R7
L61:
*line 803
move.l 8(R14),R8
clr.b 4105(R8)
*line 804
move R7,-(sp)
move.l 8(R14),R8
move.b 4104(R8),R0
and (sp)+,R0
move.b R0,4104(R8)
*line 806
move R7,R0
bra L59
L59:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _flush
.text
_flush:
~~flush:
~ok=R7
~tbp=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
*line 818
move #1,R7
*line 819
move.l _firstbuf,R13
L64:
*line 820
move.l R13,R0
beq L63
*line 822
move.l R13,(sp)
jsr _flush1
tst R0
bne L65
*line 822
clr R7
L65:
*line 823
move.l (R13),R13
bra L64
L63:
*line 825
move R7,R0
bra L62
L62:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _fill
.text
_fill:
~~fill:
~ok=R7
~tbp=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
*line 839
move.l 8(R14),R13
*line 839
tst.b 4104(R13)
beq L67
tst.b 4105(R13)
beq L67
*line 839
move.l R13,(sp)
jsr _flush1
move R0,R7
bra L68
L67:
*line 840
move #1,R7
L68:
*line 842
tst R7
beq L69
*line 842
move #16,(sp)
move.l R13,-(sp)
add.l #4,(sp)
move _settrk,-(sp)
move _setdsk,-(sp)
jsr _dskxfer
addq.l #8,sp
move R0,R7
L69:
*line 844
move.b R7,4104(R13)
*line 845
clr.b 4105(R13)
*line 846
move _settrk,4102(R13)
*line 847
move _setdsk,4100(R13)
*line 849
move R7,R0
bra L66
L66:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _gettrk
.text
_gettrk:
~~gettrk:
~imsave=R7
~tbp=R13
~ltbp=R12
~mtbp=R11
link R14,#0
movem.l R6-R7/R11-R13,-(sp)
*line 867
move #7,(sp)
jsr _setimask
move R0,R7
*line 869
move _setdsk,R8
add.l R8,R8
add.l R8,R8
move.l #_dskstate,R9
tst.b 2(R8,R9.l)
bne L71
*line 870
move R7,(sp)
jsr _setimask
*line 871
move.l #$0,R13
*line 872
move.l R13,R0
bra L70
L71:
*line 878
move.l _firstbuf,R13
*line 879
clr.l R12
*line 880
clr.l R11
L73:
*line 882
move.l R13,R0
beq L72
*line 886
tst.b 4104(R13)
beq L74
move 4100(R13),R0
cmp _setdsk,R0
bne L74
move 4102(R13),R0
cmp _settrk,R0
bne L74
*line 888
move.l R12,R0
beq L75
*line 889
move.l (R13),(R12)
*line 890
move.l _firstbuf,(R13)
*line 891
move.l R13,_firstbuf
L75:
*line 893
move R7,(sp)
jsr _setimask
*line 894
move.l R13,R0
bra L70
bra L76
L74:
*line 898
move.l R12,R11
*line 899
move.l R13,R12
*line 900
move.l (R13),R13
L76:bra L73
L72:
*line 907
move.l R11,R0
beq L77
*line 907
clr.l (R11)
L77:
*line 908
move.l _firstbuf,(R12)
*line 909
move.l R12,_firstbuf
*line 910
move R7,(sp)
jsr _setimask
*line 911
move.l R12,(sp)
jsr _flush1
tst R0
beq L78
move.l R12,(sp)
jsr _fill
tst R0
beq L78
*line 911
move.l R12,R11
bra L79
L78:
*line 912
move.l #$0,R11
L79:
*line 913
move.l R11,R0
bra L70
L70:tst.l (sp)+
movem.l (sp)+,R7-R7/R11-R13
unlk R14
rts
.globl _read
.text
_read:
~~read:
~i=R7
~p=R13
~q=R12
~tbp=R11
link R14,#0
movem.l R6-R7/R11-R13,-(sp)
*line 931
cmp #5,_setdsk
beq L81
*line 933
jsr _gettrk
move.l R0,R11
*line 935
move.l R11,R0
bne L82
*line 935
move #1,R0
bra L80
L82:
*line 939
move.l R11,R13
move _setsec,R0
asl #7,R0
ext.l R0
add.l R0,R13
add.l #4,R13
bra L83
L81:
*line 943
move _settrk,R0
ext.l R0
move.l #$c,R1
asl.l R1,R0
move.l R0,R13
move _setsec,R0
ext.l R0
move.l #$7,R1
asl.l R1,R0
add.l R0,R13
add.l _memdsk,R13
L83:
*line 945
move.l _setdma,R12
*line 946
move #128,R7
L86:
*line 947
move.b (R13)+,(R12)+
*line 947
sub #1,R7
L85:
*line 947
tst R7
bne L86
L84:
*line 948
clr R0
bra L80
L80:tst.l (sp)+
movem.l (sp)+,R7-R7/R11-R13
unlk R14
rts
.globl _write
.text
_write:
~~write:
~i=R7
~p=R13
~q=R12
~tbp=R11
~mode=9
link R14,#0
movem.l R6-R7/R11-R13,-(sp)
*line 967
cmp #5,_setdsk
beq L88
*line 969
jsr _gettrk
move.l R0,R11
*line 970
move.l R11,R0
bne L89
*line 970
move #1,R0
bra L87
L89:
*line 974
move.l R11,R13
move _setsec,R0
asl #7,R0
ext.l R0
add.l R0,R13
add.l #4,R13
bra L90
L88:
*line 978
move _settrk,R0
ext.l R0
move.l #$c,R1
asl.l R1,R0
move.l R0,R13
move _setsec,R0
ext.l R0
move.l #$7,R1
asl.l R1,R0
add.l R0,R13
add.l _memdsk,R13
*line 979
move.l _setdma,R12
*line 980
move #128,R7
L93:
*line 981
move.b (R12)+,(R13)+
*line 981
sub #1,R7
L92:
*line 981
tst R7
bne L93
L91:
*line 982
clr R0
bra L87
L90:
*line 985
move.l _setdma,R12
*line 986
move #128,R7
L96:
*line 987
move.b (R12)+,(R13)+
*line 987
sub #1,R7
L95:
*line 987
tst R7
bne L96
L94:
*line 989
move.b #1,4105(R11)
*line 993
cmp.b #1,9(R14)
bne L97
*line 993
move.l R11,(sp)
jsr _flush1
tst R0
beq L98
*line 993
clr R0
bra L87
bra L99
L98:
*line 993
move #1,R0
bra L87
L99:bra L100
L97:
*line 994
clr R0
bra L87
L100:L87:tst.l (sp)+
movem.l (sp)+,R7-R7/R11-R13
unlk R14
rts
.globl _sectran
.text
_sectran:
~~sectran:
~s=R7
~xp=R13
link R14,#0
movem.l R6-R7/R13-R13,-(sp)
*line 1031
move 8(R14),R7
*line 1031
move.l 10(R14),R13
*line 1031
move.l R13,R0
beq L102
*line 1031
move.b 0(R13,R7),R0
ext.w R0
bra L101
bra L103
L102:
*line 1032
move R7,R0
add #1,R0
bra L101
L103:L101:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
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
*line 1047
move 8(R14),R0
ext.l R0
asl.l #2,R0
move.l R0,R13
*line 1048
move.l (R13),R7
*line 1049
move.l 10(R14),(R13)
*line 1051
move.l R7,R0
bra L104
L104:tst.l (sp)+
movem.l (sp)+,R7-R7/R13-R13
unlk R14
rts
.globl _slctdsk
.text
_slctdsk:
~~slctdsk:
~stpkt=-16
~logged=11
~st1=R6
~st2=R5
~dsk=R7
~dphp=R13
link R14,#-16
movem.l R4-R7/R13-R13,-(sp)
*line 1068
move.b 9(R14),R7
*line 1068
move.b R7,R0
ext.w R0
move R0,_setdsk
*line 1077
cmp.b #4,R7
bgt L10008
cmp.b #2,R7
bne L106
L10008:*line 1078
move.l #L107,(sp)
jsr _printstr
*line 1079
move.b R7,R0
ext.w R0
move R0,(sp)
add #65,(sp)
move.l #$ffee011,-(sp)
jsr _portout
addq.l #4,sp
*line 1080
move.l #L108,(sp)
jsr _printstr
*line 1081
move.l #$0,R0
bra L105
L106:
*line 1085
move.b R7,R0
ext.w R0
muls #26,R0
move.l R0,R13
add.l #_dphtab,R13
*line 1089
cmp #5,_setdsk
bne L109
*line 1089
move.l R13,R0
bra L105
L109:
*line 1093
btst #0,11(R14)
bne L110
*line 1095
move.l #_cnvdsk,R8
move _setdsk,R9
add.l R9,R8
move.b (R8),3+_hmpack
*line 1096
move.b #48,4+_hmpack
*line 1097
move.b #2,5+_hmpack
*line 1098
move.b R7,R0
ext.w R0
move R0,_actvdsk
*line 1099
move.b R7,R0
ext.w R0
move R0,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr.b 3(R8)
*line 1100
move #7,(sp)
move.l #_hmpack,-(sp)
jsr _sendpkt
addq.l #4,sp
*line 1102
clr (sp)
move #114,-(sp)
move.b R7,R0
ext.w R0
move R0,-(sp)
jsr _dskwait
addq.l #4,sp
tst R0
bne L111
*line 1103
jsr _sendack
*line 1104
clr _ipcstate
*line 1105
move.l #$0,R0
bra L105
L111:
*line 1107
move.l R14,(sp)
add.l #-16,(sp)
jsr _getstpkt
*line 1108
jsr _sendack
*line 1109
clr _ipcstate
*line 1110
move.b -8(R14),R6
*line 1111
move.b -7(R14),R5
*line 1114
btst #7,R6
beq L112
*line 1115
move.b R7,R0
ext.w R0
move R0,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr.b 2(R8)
*line 1116
move.l #$0,R0
bra L105
bra L113
L112:
*line 1119
move.b R7,R0
ext.w R0
move R0,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
move.b #1,2(R8)
L113:
*line 1121
move.b R6,R0
ext.w R0
and #7,R0
bra L115
L116:
*line 1125
move.l #_dpb0,14(R13)
bra L114
L117:
*line 1131
move.l #_dpb2,14(R13)
bra L114
L118:
*line 1135
move.l #L119,(sp)
jsr _bioserr
*line 1136
move.l #$0,R13
bra L114
bra L114
L115:cmp #1,R0
beq L116
cmp #2,R0
beq L117
bra L118
L114:L110:
*line 1140
move.l R13,R0
bra L105
L105:tst.l (sp)+
movem.l (sp)+,R5-R7/R13-R13
unlk R14
rts
.globl _format
.text
_format:
~~format:
~retval=R6
~dsk=R7
link R14,#0
movem.l R5-R7,-(sp)
*line 1161
move 8(R14),R7
*line 1161
move.b #1,R0
ext.w R0
move R0,(sp)
move.b R7,R0
ext.w R0
move R0,-(sp)
jsr _slctdsk
addq.l #2,sp
tst.l R0
beq L120
*line 1164
cmp #5,_setdsk
beq L120
*line 1167
move.l #_cnvdsk,R8
move _setdsk,R9
add.l R9,R8
move.b (R8),3+_fmtpack
*line 1168
move _setdsk,_actvdsk
*line 1169
move _setdsk,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr.b 3(R8)
*line 1170
move #7,(sp)
move.l #_fmtpack,-(sp)
jsr _sendpkt
addq.l #4,sp
*line 1171
clr (sp)
move #112,-(sp)
move _setdsk,-(sp)
jsr _dskwait
addq.l #4,sp
tst R0
bne L121
*line 1171
clr R6
bra L122
L121:
*line 1172
move #1,R6
L122:
*line 1173
jsr _sendack
*line 1174
clr _ipcstate
*line 1175
move R6,R0
bra L120
L120:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _biosinit
.text
_biosinit:
~~biosinit:
link R14,#-4
*line 1189
jsr _initprts
*line 1190
jsr _initdsks
L123:unlk R14
rts
.globl _initprts
.text
_initprts:
~~initprts:
link R14,#-4
*line 1195
move.l #$ffee011,(sp)
jsr _portinit
*line 1196
move.l #$ffee015,(sp)
jsr _portinit
L124:unlk R14
rts
.globl _initdsks
.text
_initdsks:
~~initdsks:
~i=R7
~imsave=R6
link R14,#0
movem.l R5-R7,-(sp)
*line 1205
clr R7
L127:
*line 1205
cmp #4,R7
bge L126
*line 1207
move R7,R0
muls #4106,R0
add.l #_tbuf,R0
move.l R0,R8
clr.b 4104(R8)
*line 1208
move R7,R0
muls #4106,R0
add.l #_tbuf,R0
move.l R0,R8
clr.b 4105(R8)
*line 1209
move R7,R0
add #1,R0
cmp #4,R0
bge L129
*line 1209
move R7,R0
add #1,R0
muls #4106,R0
add.l #_tbuf,R0
move R7,R1
muls #4106,R1
add.l #_tbuf,R1
move.l R1,R9
move.l R0,(R9)
bra L130
L129:
*line 1210
move R7,R0
muls #4106,R0
add.l #_tbuf,R0
move.l R0,R8
move.l #0,(R8)
L130:L128:
*line 1205
add #1,R7
bra L127
L126:
*line 1212
move.l #_tbuf,_firstbuf
*line 1213
move.l #12318+_tbuf,_lastbuf
*line 1218
clr R7
L132:
*line 1218
cmp #4,R7
bgt L131
*line 1220
move R7,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr (R8)
*line 1221
move R7,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
move.b #1,2(R8)
*line 1222
move R7,R8
add.l R8,R8
add.l R8,R8
add.l #_dskstate,R8
clr.b 3(R8)
L133:
*line 1218
add #1,R7
bra L132
L131:
*line 1225
move #7,(sp)
jsr _setimask
move R0,R6
*line 1226
clr.l _intcount
*line 1227
clr _ipcstate
*line 1228
move R6,(sp)
jsr _setimask
L125:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _cbios
.text
_cbios:
~~cbios:
~d0=R7
~d1=R6
~d2=R5
link R14,#0
movem.l R4-R7,-(sp)
*line 1244
move 8(R14),R7
*line 1244
move.l 10(R14),R6
*line 1244
move.l 14(R14),R5
*line 1244
move R7,R0
bra L136
L137:
*line 1246
jsr _biosinit
bra L135
L138:
*line 1250
jsr _flush
*line 1251
jsr _initdsks
*line 1252
jsr _wboot
L139:
*line 1255
move.l #$ffee011,(sp)
jsr _portstat
ext.l R0
bra L134
L140:
*line 1258
move.l #$ffee011,(sp)
jsr _portin
ext.l R0
bra L134
L141:
*line 1261
move.l R6,R0
move R0,(sp)
move.l #$ffee011,-(sp)
jsr _portout
addq.l #4,sp
bra L135
L142:L143:
*line 1265
move.l R6,R0
move R0,(sp)
move.l #$ffee015,-(sp)
jsr _portout
addq.l #4,sp
bra L135
L144:
*line 1268
move.l #$ffee015,(sp)
jsr _portin
ext.l R0
bra L134
L145:
*line 1271
clr _settrk
bra L135
L146:
*line 1278
move.l R5,R0
move R0,(sp)
move.l R6,R0
move R0,-(sp)
jsr _slctdsk
addq.l #2,sp
bra L134
L147:
*line 1281
move R6,_settrk
bra L135
L148:
*line 1284
move.l R6,R0
sub.l #1,R0
move R0,_setsec
bra L135
L149:
*line 1287
move.l R6,_setdma
bra L135
L150:
*line 1290
jsr _read
ext.l R0
bra L134
L151:
*line 1293
move.l R6,R0
move R0,(sp)
jsr _write
ext.l R0
bra L134
L152:
*line 1297
move.b $ffee015,R0
ext.w R0
and #2,R0
beq L153
*line 1297
move.l #255,R0
bra L134
bra L154
L153:
*line 1298
clr.l R0
bra L134
L154:L155:
*line 1302
move.l R5,(sp)
move.l R6,R0
move R0,-(sp)
jsr _sectran
addq.l #2,sp
ext.l R0
bra L134
L156:
*line 1305
move.l #_memtab,R0
bra L134
L157:
*line 1308
move _iobyte,R0
ext.l R0
bra L134
L158:
*line 1311
move R6,_iobyte
bra L135
L159:
*line 1314
jsr _flush
tst R0
beq L160
*line 1314
move.l #$0,R0
bra L134
bra L161
L160:
*line 1315
move.l #$ffff,R0
bra L134
L161:L162:
*line 1318
move.l R5,(sp)
move.l R6,R0
move R0,-(sp)
jsr _setxvect
addq.l #2,sp
bra L134
L163:
*line 1328
move.l R6,R0
move R0,(sp)
jsr _format
tst R0
beq L10009
clr R0
bra L10010
L10009:move #1,R0
L10010:ext.l R0
bra L134
L164:
*line 1332
move.l #$0,R0
bra L134
bra L135
bra L135
L136:cmp #63,R0
bhi L164
asl #2,R0
move R0,R8
add.l #L165,R8
move.l (R8),R8
jmp (R8)
.data
L165:.dc.l L137
.dc.l L138
.dc.l L139
.dc.l L140
.dc.l L141
.dc.l L142
.dc.l L143
.dc.l L144
.dc.l L145
.dc.l L146
.dc.l L147
.dc.l L148
.dc.l L149
.dc.l L150
.dc.l L151
.dc.l L152
.dc.l L155
.dc.l L164
.dc.l L156
.dc.l L157
.dc.l L158
.dc.l L159
.dc.l L162
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L164
.dc.l L163
.text
L135:L134:tst.l (sp)+
movem.l (sp)+,R5-R7
unlk R14
rts
.data
L13:.dc.b 10,13,66,73,79,83,32,69,82,82,79,82,32,45,45,32,0
L14:.dc.b 46,10,13,0
L107:.dc.b 10,13,66,73,79,83,32,69,82,82,79,82,32,45,45,32,68,73,83,75,32,0
L108:.dc.b 32,78,79,84,32,83,85,80,80,79,82,84,69,68,10,13,0
L119:.dc.b 73,110,118,97,108,105,100,32,68,105,115,107,32,83,116,97,116,117,115,0
