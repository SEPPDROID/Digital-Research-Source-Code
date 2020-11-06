$title	('MP/M 1.1 Terminal Message Processor')
	name	tmp
	cseg
;tmp:
;do;

;$include (copyrt.lit)
;/*
;  Copyright (C) 1979, 1980
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;
;  Revised:
;    18 Jan 80  by Thomas Rolander
;*/
;$include (common.lit)
;$nolist
;$include (proces.lit)
;$nolist
;$include (queue.lit)
;$nolist
;$include (xdos.lit)
;$nolist
;$include (fcb.lit)
;$nolist
;$include (xdos.ext)
;$nolist
;$include (bdos.ext)
;$nolist
;$include (datapg.ext)
;$nolist

;  xdos:
	extrn	xdos
;    procedure (func,info) address external;
;      declare func byte;
;      declare info address;
;    end xdos;

;  mon1:
	extrn	mon1
;    procedure (func,info) external;
;      declare func byte;
;      declare info address;
;    end mon1;

;  mon2:
	extrn	mon2
;    procedure (func,info) byte external;
;      declare func byte;
;      declare info address;
;    end mon2;

;  open:
	extrn	open
;    procedure (fcbadr) byte external;
;      declare fcbadr address;
;    end open;

;  close:
	extrn	close
;    procedure (fcbadr) byte external;
;      declare fcbadr address;
;    end close;

;  readbf:
	extrn	readbf
;    procedure (fcbadr) byte external;
;      declare fcbadr address;
;    end readbf;

;  setdma:
	extrn	setdma
;    procedure (dmaadr) external;
;      declare dmaadr address;
;    end setdma;

;  co:
	extrn	co
;    procedure (char) external;
;      declare char byte;
;    end co;

;  ci:
	extrn	ci
;    procedure byte external;
;    end ci;

;  printb:
	extrn	printb
;    procedure (msgadr) external;
;      declare msgadr address;
;    end printb;

;  readbu:
	extrn	readbu
;    procedure (bufferadr) external;
;      declare bufferadr address;
;    end readbu;

;  crlf:
	extrn	crlf
;    procedure external;
;    end crlf;

;  assign:
	extrn	assign
;    procedure (name$adr) byte external;
;      declare name$adr address;
;    end assign;


	dseg
;/*
;  TMP Data Segment
;*/

;  declare rlr address external;
	extrn	rlr

;  declare rlrpd based rlr process$descriptor;

;  declare ver (1) byte external;
	extrn	ver

;  declare cli$lqcb queuehead external;
	extrn	clilqcb

;  declare subflgadr address;
subflgadr:
	ds	2
;  declare subflg based subflgadr (1) byte;

	cseg
;  declare cli$name (8) byte data ('c'+80h,'li     ');
cliname:
	db	'c'+80h,'li     '

;  declare submit$fcb (16) byte data (1,'$$$     SUB',
;    0,0,0,0);
submitfcb:
	db	1
	db	'$$$     SUB'
	db	0,0,0,0

dskerr:
	db	'Disk error during submit file read.'
	db	'$'

;/*
;  tmp:
;*/

;  tmp:
tmp:
	public	tmp
	LXI	H,0FF4CH
	DAD	SP
	SPHL
;    procedure reentrant public;
;      declare buf(129) byte;
;      declare fcb fcb$descriptor;
;      declare submit$flag byte;
;      declare console byte;
;      declare i byte;
;      declare ret byte;
;      declare CLIQ (2) address;
;      declare pname (10) byte;

;      console = rlrpd.console;
	LXI	B,0EH
	LHLD	RLR
	DAD	B
	MOV	A,M
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	MOV	M,A
;      subflgadr = xdos (system$data$adr,0) + 128;
	MVI	C,9AH
	CALL	XDOS
	LXI	D,80H
	DAD	D
	SHLD	SUBFLGADR
;      subflg(console) = false;
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	MOV	C,M
	MVI	B,0
	LHLD	SUBFLGADR
	DAD	B
	MVI	M,0H
;      CLIQ(0) = .cli$lqcb;
	LXI	B,CLILQCB
	LXI	H,0A6H  ; CLIQ
	DAD	SP
	MOV	M,C
	INX	H
	MOV	M,B
;      CLIQ(1) = .buf;
	LXI	H,0H    ; BUF
	DAD	SP
	XCHG
	LXI	H,0A8H  ; CLIQ+2H
	DAD	SP
	MOV	M,E
	INX	H
	MOV	M,D
;      submit$flag = false;
	LXI	H,0A2H  ; SUBMITFLAG
	DAD	SP
	MVI	M,0H
;      pname(0) = console;
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	MOV	A,M
	LXI	H,0AAH  ; PNAME
	DAD	SP
	MOV	M,A
;      call move (8,.cli$name,.pname(1));
	INX	H
	XCHG
	LXI	B,CLINAME
	MVI	L,8H
	LDAX	B
	STAX	D
	INX	B
	INX	D
	DCR	L
	JNZ	$-5H
;      pname(9) = 0;
	XCHG
	MOV	M,E
;      call set$dma (.buf(1));
	LXI	H,1H    ; BUF+1H
	DAD	SP
	MOV	B,H
	MOV	C,L

;
;  Temporarily swap stack pointers to avoid TMP process
; descriptor destruction.
;
	lxi	h,00a2h
	dad	sp
	sphl

	CALL	SETDMA
;      ret = xdos (attach,0);
	MVI	C,92H
	CALL	XDOS
;      call print$b (.ver);
	LXI	B,VER
	CALL	PRINTB
;      ret = xdos (detach,0);

	MVI	C,93H
	CALL	XDOS

	lxi	h,-00a2h
	dad	sp
	sphl

;      do forever;
@17:
;        ret = xdos (attach,0);
	MVI	C,92H
	CALL	XDOS
;        call crlf;
	CALL	CRLF
;        i = rlrpd.disk$slct and 0fh;
	LXI	B,16H
	LHLD	RLR
	DAD	B
	MOV	A,M
	PUSH	PSW
	ANI	0FH
;        if (i:=i-10) < 15 then
	SUI	10
	JC	@TMP0
;          call co ('1');
	PUSH	PSW
	MVI	C,'1'
	CALL	CO
	POP	PSW
	SUI	10
;        call co (i + 10 + '0');
@TMP0:
	ADI	10+'0'
	MOV	C,A
	CALL	CO
;        call co (shr(rlrpd.disk$slct,4) + 'A');
	POP	PSW
	ANI	0f8h
	RAR
	RAR
	RAR
	RAR
	ADI	41H
	MOV	C,A
	CALL	CO
;        call co ('>');
	MVI	C,3EH
	CALL	CO
;        buf(0) = 100;
	LXI	H,0H    ; BUF
	DAD	SP
	MVI	M,100
;        if not submit$flag then
	LXI	H,0A2H  ; SUBMITFLAG
	DAD	SP
	MOV	A,M
	CMA
	RAR
	JNC	@1
;        do;
;          if subflg(console) then
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	MOV	C,M
	MVI	B,0
	LHLD	SUBFLGADR
	DAD	B
	MOV	A,M
	RAR
	JNC	@2
;          do;
;            call move (16,.submit$fcb,.fcb.et);
	LXI	H,81H   ; FCB
	DAD	SP
	XCHG
	LXI	B,SUBMITFCB
	MVI	L,10H
	LDAX	B
	STAX	D
	INX	B
	INX	D
	DCR	L
	JNZ	$-5H
;            fcb.fn(1) = console + '0';
	MVI	A,30H
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	ADD 	M
	LXI	H,83H   ; FCB+2H
	DAD	SP
	MOV	M,A
;            if open (.fcb) <> 0ffh then
	LXI	H,81H   ; FCB
	DAD	SP
	MOV	B,H
	MOV	C,L
	CALL	OPEN
	INR	A
	JZ	@3
;            do;
;              submit$flag = true;
	LXI	H,0A2H  ; SUBMITFLAG
	DAD	SP
	MVI	M,0FFH
;            end;
@3:
;            subflg(console) = false;
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	MOV	C,M
	MVI	B,0
	LHLD	SUBFLGADR
	DAD	B
	MOV	M,B
;          end;
@2:
;        end;
;        if submit$flag then
	LXI	H,0A2H  ; SUBMITFLAG
	DAD	SP
	MOV	A,M
	RAR
	JNC	@6
;        do;
@1:
; use buffer area as temporary stack
	lxi	h,0081h
	dad	sp
	sphl
;          if mon2 (11,0) then
	MVI	C,0BH
	CALL	MON2
	RAR
	JNC	@5
;          do;
;            ret = ci;
	CALL	CI
;            call mon1 (19,.fcb);
;            submit$flag = false;
;          end;
	lxi	h,-0081h
	dad	sp
	sphl
	JMP	@10A
@5:
	lxi	h,-0081h
	dad	sp
	sphl
;          else
;          do;
;            fcb.nr = fcb.rc - 1;
	LXI	H,90H   ; FCB+0FH
	DAD	SP
	MOV	A,M
	DCR	A
	LXI	H,0A1H  ; FCB+20H
	DAD	SP
	MOV	M,A
;            if readbf (.fcb) = 0ffh then
	LXI	H,81H   ; FCB
	DAD	SP
	MOV	B,H
	MOV	C,L
	CALL	READBF
	INR	A
	JNZ	@7
;            do;
;              submit$flag = false;
	call	endsubmit
;              call mon1 (19,.fcb); /* delete file */
	LXI	H,81H   ; FCB
	DAD	SP
	XCHG
	MVI	C,13H
	CALL	MON1
;              call print$b (.(
	LXI	B,dskerr
	CALL	PRINTB
;                'Disk error during submit file read.','$'));
;              call crlf;
	CALL	CRLF
;            end;
	JMP	@6
@7:
;            else
;            do;
;              i = 2;
	LXI	H,2H    ; BUF+2H
	DAD	SP
;              do while buf(i) <> 0;
@7A:
	mov	a,m
	ora	a
	jz	@7B
	mov	c,a
;                call co (.buf(i));
	push	h
	CALL	CO
	pop	h
;                i = i + 1;
	inx	h
;              end;
	jmp	@7A
;              call co (0dh);
@7B:
	mvi	c,0dh
	call	co
	JMP	@9
;            end;
;          end;
;        end;
;        if not submit$flag then
;        do;
@6:
;          call read$bu (.buf);
	LXI	H,0H    ; BUF
	DAD	SP
	MOV	B,H
	MOV	C,L
;
;  The following stack swap is done to prevent destruction
; of the TMP process descriptor by the stack.  The stack used
; during read$bu overlays the TMP fcb and the end of the
; line buffer.  Note that the line buffer length is reduced
; from 128 to 100 bytes.
;
	lxi	h,00a2h
	dad	sp
	sphl
	CALL	READBU
	lxi	h,-00a2h
	dad	sp
	sphl

;        end;
@9:
;        if (buf(1) <> 0) and
	LXI	H,1H    ; BUF+1H
	DAD	SP
	MOV	A,M
	ORA	A
	JZ	@10
	MOV	B,A
	INX	H
	MOV	A,M
	CPI	';'
	JZ	@10
	MOV	C,A
;           (buf(2) <> ';') then
;        do;
;          if (buf(1) = 2) and (buf(3) = ':') then
	MOV	A,B
	CPI	2
	JNZ	@11
	INX	H
	MOV	A,M
	CPI	':'
	JNZ	@11
;          do;
;            i = (buf(2) and 101$1111b) - 'A';
	MVI	A,5FH
	ANA	C
	SUI	'A'
;            if i < 16
	CPI	10H
	JNC	@13
;              then call mon1 (14,i);
	MOV	E,A
	MVI	C,0EH
	CALL	MON1
;          end;
	JMP	@13
@11:
;          else
;          do;
;            buf(buf(1)+2) = 0;
	LXI	H,1H    ; BUF+1H
	DAD	SP
	MOV	C,M
	MVI	B,0
	LXI	H,2H    ; BUF+2H
	DAD	SP
	DAD	B
	MVI	M,0H
;            call co (0ah);
	MVI	C,0AH
	CALL	CO
;            buf(0) = rlrpd.disk$slct;
	LXI	B,16H
	LHLD	RLR
	DAD	B
	MOV	A,M
	LXI	H,0H    ; BUF
	DAD	SP
	MOV	M,A
;            buf(1) = console;
	XCHG
	LXI	H,0A3H  ; CONSOLE
	DAD	SP
	MOV	A,M
	INX	D
	STAX	D
;            ret = assign (.pname);
	LXI	H,0AAH  ; PNAME
	DAD	SP
	MOV	B,H
	MOV	C,L
	CALL	ASSIGN
;            ret = xdos (write$queue,.CLIQ);
	LXI	H,0A6H  ; CLIQ
	DAD	SP
	XCHG
	MVI	C,8BH
	CALL	XDOS
;          end;
@13:
;        end;
@10:
;        if submit$flag then
	LXI	H,0A2H  ; SUBMITFLAG
	DAD	SP
	MOV	A,M
	RAR
	JNC	@17
;        do;
;          if fcb.nr = 1 then
	dcx	h
	MOV	A,M
	DCR	A
	JNZ	@15
@10A:
;          do;
;            submit$flag = false;
;            call mon1 (19,.fcb); /* delete file */
	LXI	H,81H   ; FCB
	DAD	SP
	XCHG
	MVI	C,13H
	CALL	MON1
	call	endsubmit
;          end;
	JMP	@17
@15:
;          else
;          do;
;            fcb.rc = fcb.rc - 1;
	LXI	H,90H   ; FCB+0FH
	DAD	SP
	DCR	M
;            call close (.fcb);
	LXI	H,81H   ; FCB
	DAD	SP
	MOV	B,H
	MOV	C,L
	CALL	CLOSE
;          end;
;        end;
;      end;
	JMP	@17
;    end tmp;
;end tmp;

endsubmit:
;              submit$flag = false;
	LXI	H,0A2H+2  ; SUBMITFLAG
	DAD	SP
	mvi	m,0
;              /* free drive */
;              call mon1 (39,0ffffh);
	mvi	c,39
	lxi	d,0ffffh
	jmp	mon1

	END
