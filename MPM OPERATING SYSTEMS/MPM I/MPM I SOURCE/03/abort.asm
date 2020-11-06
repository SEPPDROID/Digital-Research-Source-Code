$title	('MP/M 1.0 Abort Process')
	name	abort
	cseg
;abort:
;do;

;$include (copyrt.lit)
;/*
;  Copyright (C) 1979
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;*/
;$include (common.lit)
;$nolist
;$include (proces.lit)
;$nolist
;$include (queue.lit)
;$nolist
;$include (xdos.lit)
;$nolist
;$include (datapg.ext)
;$nolist
;$include (xdos.ext)
;$nolist
;$include (bdos.ext)
;$nolist

;  declare rlr address external;
	extrn	rlr

;  nfxdos:
;    procedure (func,info) external;
	extrn	nfxdos
;      declare func byte;
;      declare info address;
;    end nfxdos;

;  xdos:
;    procedure (func,info) byte external;
	extrn	xdos
;      declare func byte;
;      declare info address;
;    end xdos;

;  printb:
;    procedure (bufferadr) external;
	extrn	printb
;      declare bufferadr address;
;    end printb;

;  declare rlrpd based rlr process$descriptor;

;/*
	dseg
;  Abort Process Data Segment
;*/
;  declare abort$pd process$descriptor public
;    initial (0,rtr$status,20,.abort$stk+28,
;             'ABORT   ',0,0ffh,0);
abortpd:
	public	abortpd
	dw	0	; pl
	db	0	; status
	db	20	; priority
	dw	abortstk+38	; stkptr
	db	'ABORT   '	; name
	db	$-$	; console
	db	0ffh	; memseg (system)
	dw	$-$	; b
	dw	$-$	; thread
	ds	10
;  declare abort$stk (20) address
;    initial (restarts,.abort);
abortstk:
	dw	0c7c7h,0c7c7h,0c7c7h
	dw	0c7c7h,0c7c7h,0c7c7h
	dw	0c7c7h,0c7c7h,0c7c7h
	db	'FACBEDLHxiyifacbedlh'
	dw	abort

;  declare abort$lqcb structure (
;    lqueue,
;    buf (12) byte )
;    initial (0,'ABORT   ',10,1);
abortlqcb:
	dw	$-$	; ql
	db	'ABORT   '	; name
	dw	10	; msglen
	dw	1	; nmbmsgs
	dw	$-$	; dqph
	dw	$-$	; nqph
	dw	$-$	; mh
	dw	$-$	; mt
	dw	$-$	; bh
	ds	12	; buf (12) byte
;  declare abort$uqcb userqcbhead public
;    initial (.abort$lqcb,.apcb.param);
abortuqcb:
	dw	abortlqcb	; pointer
	dw	param		; msgadr
;  declare abort$parameter$control$block structure (
apcb:
;    pdadr address,
pdadr:	dw	0
;    param address,
param:	ds	2
;    pname (8) byte ) initial (0);
	pname:	ds	8
;  declare console byte at (.apcb.param(1));
console	equ	param+1
	cseg

abortfail:
	db	'Abort failed.'
	db	'$'


;/*
;  abort:
;          The purpose of the abort process is to abort
;        the specified process.

;  Entry Conditions:
;        None

;  Exit Conditions:
;        None

;*/

;  abort:
abort:
;    procedure public;
	public	abort
;      declare i byte;

;      call nfxdos (make$queue,.abort$lqcb);
	LXI	D,ABORTLQCB
	MVI	C,86H
;      do forever;
@4:
	CALL	NFXDOS
;        call nfxdos (read$queue,.abort$uqcb);
	LXI	D,ABORTUQCB
	MVI	C,89H
	CALL	NFXDOS
;        rlrpd.console = console;
	LXI	B,0EH
	LHLD	RLR
	DAD	B
	LDA	CONSOLE
	MOV	M,A
;        i = 0;
	MVI	C,8
	LXI	H,PNAME
;        do while i <> 8;
@6:
;          if field(i) = 0 then
	MOV	A,M
	ORA	A
	JNZ	@1
;          do while i <> 8;
@8:
;            field(i) = ' ';
	MVI	M,20H
;            i = i + 1;
	INX	H
	DCR	C
	JNZ	@8
	JMP	@7
;          end;
@1:
;          else i = i + 1;
	INX	H
	DCR	C
	JNZ	@6
;        end;
@7:
;        /* parameters to XDOS abort process are terminate
;           system or non-sytem process & release memory segment */
;	apcb.param = 00ffh;
	lxi	h,00ffh
	shld	param
;        if xdos (abort$process,.apcb) = 255 then
	LXI	D,apcb
	MVI	C,9dH
	CALL	XDOS
	INR	L
;        do;
;          call printb (.('Abort failed.','$'));
	LXI	B,abortfail
	CZ	PRINTB
;        end;
;        call nfxdos (detach,0);
	MVI	C,93H
;      end; /* forever */
	JMP	@4
;    end abort;

;end abort;
	END
