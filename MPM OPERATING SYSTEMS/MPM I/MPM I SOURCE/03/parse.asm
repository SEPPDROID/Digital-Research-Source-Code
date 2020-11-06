$title	('MP/M 1.1 Parse Filename')
	name	parse
	cseg
;parse:
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
;$include (queue.lit)
;$nolist
;$include (xdos.lit)
;$nolist
;$include (xdos.ext)
;$nolist

;  readq:
	extrn	readq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end readq;

;  writeq:
	extrn	writeq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end writeq;

;  xdos:
	extrn	xdos
;    procedure (func,info) address external;
;      declare func byte;
;      declare info address;
;    end xdos;

	dseg
;/*
;  Parse Queue Data Segment
;*/
;  declare parse$cqcb structure (cqueue,pdadr address) public
;    initial (0,'MXParse ',0,1);
	public	parsecqcb
parsecqcb:
	dw	$-$	; ql
	db	'MXParse '	; name
	dw	0	; msglen
	dw	1	; nmbmsgs
	dw	$-$	; dqph
	dw	$-$	; nqph
	dw	$-$	; msgin
	dw	$-$	; msgout
	dw	$-$	; msgcnt

	dw	$-$	; pdadr

;  declare parse$mutex address public
;    initial (.parse$cqcb);
parsemutex:
	public	parsemutex
	dw	parsecqcb	; pointer
	cseg

del:
	db	0dh,' =.:<>_[],'

;  /*
;    parse:
;  */

parse:
;  parse:
;    procedure (pcb$adr) address;
	dseg
;    declare pcb$adr address;
pcbadr:	ds	2
;    declare pcb based pcb$adr structure (
;      filename$adr address,
;      fcb$adr address      );
;    declare pcb$filename$adr address;
pcbfilenameadr:
	ds	2
;    declare pcb$fcb$adr address;
pcbfcbadr:
	ds	2
;    declare filename based pcb$filename$adr (1) byte;
filename	equ	pcbfilenameadr+1
;    declare fcb based pcb$fcb$adr (1) byte;
fcb	equ	pcbfcbadr+1

;    declare
;      /* return conditions */
;      endline literally '00000H',
;      badfile literally '0FFFFH',
;      /* useful literals */
;      disk    literally 'fcb(0)',
;      fcbname literally '8',  /* end of name */
;      fcbtype literally '11', /* end of type field */
;      fcbsize literally '16'; /* partial size of fcb */

;    declare char byte; /* global temp for current char */
char:	ds	1
;    declare fnp  byte; /* index into file name buffer */
fnp:	ds	1
;    declare fnlen byte;
fnlen:	ds	1
	cseg

;    /* initialize local bases */
;    pcb$filename$adr = pcb.filename$adr;
	LDAX	B
	MOV	L,A
	INX	B
	LDAX	B
	MOV	H,A
	SHLD	PCBFILENAMEADR
;    pcb$fcb$adr      = pcb.fcb$adr;
;    /* initialize file control block to empty */
	INX	B
	LDAX	B
	MOV	L,A
	INX	B
	LDAX	B
	MOV	H,A
	SHLD	PCBFCBADR
;    char  = ' ';
	LXI	H,CHAR
	MVI	M,20H
;    fnlen  = 0;
	LXI	H,FNLEN
	MVI	M,0H
;    fnp   = -1;
	DCX	H
	MVI	M,0FFH
;    do while fnlen < fcbsize-1;
@24:
	LDA	FNLEN
	CPI	0FH
	JNC	@25
;      if fnlen = fcbtype then char = 0;
	SUI	0BH
	JNZ	@4
	LXI	H,CHAR
	MOV	M,A
@4:
;      call putchar;
	CALL	PUTCHAR
;    end;
	JMP	@24
@25:
;    disk = 0;
;    /* scan next name */
	LHLD	PCBFCBADR
	MVI	M,0H
;    do forever;
@26:
;    /* deblank command buffer */
;      call gnc;
	CALL	GNC	; char left in A
;      do while char = ' ';
@28:
	CPI	20H
	JZ	@26
;        call gnc;
;      end;
;      if delimiter then return badfile;
	CALL	DELIMITER
	RAR
	JC	@8A
;      fnlen = 0;
	LXI	H,FNLEN
	MVI	M,0H
;      do while not delimiter;
@30:
	CALL	DELIMITER
	RAR
	JC	@31
;        if fnlen >= fcbname then /* error, file name too long */
	LDA	FNLEN
	CPI	8H
	JC	@6
;          return badfile;
@8A:
	LXI	H,0FFFFH
	RET

@6:
;        if char = '*' then call fillq(fcbname); else call putchar;
	LDA	CHAR
	CPI	2AH
	JNZ	@7
	MVI	C,8H
	CALL	FILLQ
	JMP	@8
@7:
	CALL	PUTCHAR
@8:
;        call gnc;
	CALL	GNC
;      end;
	JMP	@30
@31:

;      /* check for disk name */
;      if char = ':' then
	LDA	CHAR
	CPI	3AH
	JNZ	@9
;      do;
;        if not (disk = 0 and fnlen = 1) then
	LHLD	PCBFCBADR
	MOV	A,M
	ORA	A
	JNZ	@8A
	LDA	FNLEN
	DCR	A
	JNZ	@8A
;          return badfile;
;        /* must be a disk name */
;        if (disk := fcb(1) - 'A' + 1) > 26
;          /* invalid disk name */
	LHLD	PCBFCBADR
	INX	H
	MOV	A,M
	SUI	41H
	INR	A
	DCX	H
	MOV	M,A
	CPI	27
	JNC	@8A
;          then return badfile;
;          /* valid disk name replace space in name */
;          else fcb(fnlen) = ' ';
	LHLD	FNLEN
	MVI	H,0
	XCHG
	LHLD	PCBFCBADR
	DAD	D
	MVI	M,20H
;      end;
	JMP	@26
@9:
;      else
;      do;
;        /* char is not ':', so file name is set. scan remainder */
;        /* at least one char scanned */
;        fnlen = fcbname;
	LXI	H,FNLEN
	MVI	M,8H
;        if char = '.' then /* scan file type */
	CPI	2EH
	JNZ	@14
;        do;
;          call gnc;
	CALL	GNC
;          do while not delimiter;
@32:
	CALL	DELIMITER
	RAR
	JC	@33
;            if fnlen >= fcbtype then
	LDA	FNLEN
	CPI	0BH
	JC	@15
;              /* error, type field too long */
;              return badfile;
	LXI	H,0FFFFH
	RET
@15:
;            if char = '*'
	LDA	CHAR
	CPI	2AH
	JNZ	@16
;              then call fillq(fcbtype);
	MVI	C,0BH
	CALL	FILLQ
	JMP	@17
@16:
;              else call putchar;
	CALL	PUTCHAR
@17:
;              call gnc;
	CALL	GNC
;          end;
	JMP	@32
@33:
	LDA	CHAR
;        end;
@14:

;        if char = 0dh
	CPI	0DH
	LXI	H,0000H
;          then return endline;
	RZ
;          else return .filename(fnp);
	LHLD	FNP
	MVI	H,0
	XCHG
	LHLD	PCBFILENAMEADR
	DAD	D
	RET
;      end;
;    end; /* of forever */
;  end parse;

;    gnctran:
gnctran:
;      procedure(b) byte;
;        declare b byte;

;        if b < ' ' then return 0dh; /* all non-graphics */
	MOV	A,C
	CPI	20H
	MVI	A,0DH
	RC
;        /* translate alpha to upper case */
;        if b >= 'a' and b <= 'z' then
	MOV	A,C
	CPI	'a'
	RC
	CPI	'z'+1
	RNC
	ANI	5FH
	RET
;          b = b and 101$1111b; /* upper case */
;        return b;
;      end gnctran;

;    gnc:
gnc:
;      procedure;

;        char = gnctran(filename(fnp := fnp + 1));
	LXI	H,FNP
	INR	M
	MOV	C,M
	MVI	B,0
	LHLD	PCBFILENAMEADR
	DAD	B
	MOV	C,M
	CALL	GNCTRAN
	STA	CHAR
;      end gnc;
	RET

;    delimiter:
delimiter:
;      procedure byte;
;        declare i byte;
;        declare del(*) byte data
;          (0dh,' =.:<>_[],');

;        do i = 0 to last(del);
	MVI	C,11
	LXI	D,DEL
	LXI	H,CHAR
@20:
;          if char = del(i) then return true;
	LDAX	D
	SUB	M
	MVI	A,0FFH
	RZ
	INX	D
	DCR	C
	JNZ	@20
;        end;
;        return false;
	XRA	A
	RET
;      end delimiter;

;    putchar:
putchar:
;      procedure;

;        fcb(fnlen:=fnlen+1) = char;
;        /* can check here for ambig ref's "char = '?'" */
	LXI	H,FNLEN
	INR	M
	MOV	C,M
	MVI	B,0
	LHLD	PCBFCBADR
	DAD	B
	LDA	CHAR
	MOV	M,A
;      end putchar;
	RET

;    fillq:
fillq:
;      procedure(len);
;      /* fill current name or type with question marks */

;        declare len byte;

;        char = '?'; /* question mark */
	LXI	H,CHAR
	MVI	M,3FH
;        do while fnlen < len;
@22:
	LDA	FNLEN
	CMP	C
	RNC
;          call putchar;
	PUSH	B
	CALL	PUTCHAR
	POP	B
;        end;
	JMP	@22
;      end fillq;

;/*
;  parse$filename:
;*/

;  parse$filename:
parsefilename:
	PUSH	B
;    procedure (pcb$adr) address reentrant public;
	public	parsefilename
;      declare pcb$adr address;
;      declare nxt$chr$adr address;
;      declare ret byte;

;      ret = xdos (read$queue,.parse$mutex);
	LXI	B,PARSEMUTEX
	CALL	READQ
;      nxt$chr$adr = parse (pcb$adr);
	POP	B
	CALL	PARSE
	PUSH	H
;      ret = xdos (write$queue,.parse$mutex);
	LXI	B,PARSEMUTEX
	CALL	WRITEQ
;      return nxt$chr$adr;
	POP	H
	RET
;    end parse$filename;

;end parse;
	END
