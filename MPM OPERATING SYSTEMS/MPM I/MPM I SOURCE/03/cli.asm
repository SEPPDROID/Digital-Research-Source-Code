$title	('MP/M 1.1 Command Line Interpreter')
	name	cli
	cseg
;cli:
;do;

;/*
;  Copyright (C) 1979,1980
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;
;  Revised:
;    18 Jan 80 by Thomas Rolander
;*/

;$include (common.lit)
;$nolist
;$include (proces.lit)
;$nolist
;$include (queue.lit)
;$nolist
;$include (fcb.lit)
;$nolist
;$include (xdos.lit)
;$nolist
;$include (memmgr.lit)
;$nolist
;$include (memmgr.ext)
;$nolist
;$include (xdos.ext)
;$nolist
;$include (bdos.ext)
;$nolist
;$include (bdosi.ext)
;$nolist
;$include (datapg.ext)
;$nolist

;  declare stktbl (1) structure (loc (10) address) external;
	extrn	stktbl

;  declare pdtbl (1) structure (process$descriptor) external;
	extrn	pdtbl

;  declare console$attached (1) address external;
	extrn	cnsatt

;  declare rlr address external;
	extrn	rlr

;  declare rlrpd based rlr process$descriptor;

;  assign:
	extrn	assign
;    procedure (nameadr) byte external;
;      declare nameadr address;
;    end assign;

;  dispatch:
	extrn	dispatch
;    procedure external;
;    end dispatch;

;  parsefilename:
	extrn	parsefilename
;    procedure (pcb$address) address external;
;      declare pcb$address address;
;    end paresefilename;

;  makeq:
	extrn	makeq
;    procedure (qcbadr) byte external;
;      declare qcbadr address;
;    end makeq;

;  openq:
	extrn	openq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end openq;

;  readq:
	extrn	readq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end readq;

;  cwriteq:
	extrn	cwriteq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end cwriteq;

;  detach:
	extrn	detach
;    procedure external;
;    end detach;

;  xbdos:
	extrn	xbdos
;    procedure (func,info) address external;
;      declare func byte;
;      declare info address;
;    end xbdos;

;  xdos:
	extrn	xdos
;    procedure (func,info) address external;
;      declare func byte;
;      declare info address;
;    end xdos;

;  endp:
	extrn	endp
;    procedure external;
;    end endp;

;  absrq:
	extrn	absrq
;    procedure (mdadr) byte external;
;      declare mdadr address;
;    end absrq;

;  relrq:
	extrn	relrq
;    procedure (mdadr) byte external;
;      declare mdadr address;
;    end relrq;

;  memfr:
	extrn	memfr
;    procedure (mdadr) byte external;
;      declare mdadr address;
;    end memfr;

;  printb:
	extrn	printb
;    procedure (msgadr);
;      declare msgadr address;
;    end printb;

;  open:
	extrn	open
;    procedure (fcbadr) byte;
;      declare fcbadr address;
;    end open;

;  readbf:
	extrn	readbf
;    procedure (fcbadr) byte;
;      declare fcbadr address;
;    end readbf;

;  setdma:
	extrn	setdma
;    procedure (dmaadr) external;
;      declare dmaadr address;
;    end setdma;

	dseg
;  declare reserved$for$disk (3) byte;
	ds	3
;  declare buffer (128) byte;
buffer:	ds	128

;  declare pname (10) byte initial (
;    0,'        ',0);
pname:
	db	0
	db	'        '
	db	0

;/*
;  CLI Process Data Segment
;*/
;  declare cli$pd process$descriptor public
;    initial (0,rtr$status,199,.cli$entrypt,
;             'c'+80h,'li     ',0,0ffh,0,0,.buffer,0);
clipd:
	public	clipd
	extrn	attchpd
	dw	attchpd	; pl
	db	0	; status
	db	199	; priority
	dw	clientrypt	; stkptr
	db	'c'+80H,'li     '	; name
	db	$-$	; console
	db	0ffh	; memseg (system)
	dw	$-$	; b
	dw	$-$	; thread
	dw	buffer	; disk set DMA
	db	$-$	; disk select / user code
	dw	$-$	; dcnt
	db	$-$	; searchl
	dw	$-$	; searcha
	ds	2	; drvact
	ds	20	; registers
	ds	2	; scratch

;  declare cli$stk (13) address
;    initial (restarts,.cli);
clistk:
	dw	0c7c7h,0c7c7h,0c7c7h
	dw	0c7c7h,0c7c7h,0c7c7h
	dw	0c7c7h,0c7c7h,0c7c7h
	dw	0c7c7h,0c7c7h,0c7c7h
clientrypt:
	dw	cli

;  declare cli$lqcb
;    structure (lqueue,
;               buf (131) byte) public
;    initial (0,'CliQ    ',129,1);
clilqcb:
	public	clilqcb
	dw	$-$	; ql
	db	'CliQ    '	; name
	dw	129	; msglen
	dw	1	; nmbmsgs
	dw	$-$	; dqph
	dw	$-$	; nqph
	dw	$-$	; mh
	dw	$-$	; mt
	dw	$-$	; bh
	ds	131	; buf (131) byte

;  declare CLIQ userqcbhead
;    initial (.cli$lqcb,.field);
cliq:
	dw	clilqcb	; pointer
	dw	field	; msgadr

;  declare pcb structure (
;    field$adr address,
;    fcb$adr address );
pcb:
	ds	2	; fieldadr
	ds	2	; fcbadr

;  declare field (129) byte;
field:	ds	129
;  declare disk$select byte at (.field);
diskselect	equ	field
;  declare console byte at (.field(1));
console	equ	field+1
;  declare command$tail (1) byte at (.field(2));
commandtail	equ	field+2

;  declare fcb fcb$descriptor;
fcb:
	db	$-$	; et
	db	'        '	; fn
	db	'   '	; ft
	db	$-$	; ex
	dw	$-$	; nu
	db	$-$	; rc
	ds	16	; dm
	db	$-$	; nr

;  declare cusp$uqcb userqcb initial (
;    0,.field,'$$$$$$$$');
cuspuqcb:
	dw	$-$	; pointer
	dw	field	; msgadr
	db	'$$$$$$$$'	; name

;  declare nxt$chr$adr address;
nxtchradr:
	ds	2

;  declare ret byte;

;  declare md memory$descriptor;
md:
	db	$-$	; base
	db	$-$	; size
	db	$-$	; attrib
	db	$-$	; bank

;  declare pdadr address;
pdadr:	ds	2
;  declare pd based pdadr process$descriptor;

;  declare (base,top) address;
base:	ds	2
top:	ds	2

;  declare (i,j) address;
i:	ds	2
j:	ds	2

;  declare (mask,prl,ok,notdone) byte;
mask:	ds	1
prl:	ds	1
ok:	ds	1
notdone:	ds	1

;  declare sector$size literally '0080H';

;  declare user$priority literally '200';

;  declare segment$bottom address;
segmentbottom:
	ds	2

;  declare offset address;
offset:	ds	2

;  declare data$size address;
datasize:
	ds	2

;  declare mem$pointer address;
mempointer:
	ds	2
;  declare instr based mem$pointer byte;
;  declare location based mem$pointer address;
;  declare array based mem$pointer (1) byte;

;  declare loc3 byte at (0003H);
loc3	equ	0003h

;  declare bitmap$adr address;
bitmapadr:
	ds	2
;  declare bitmap based bitmap$adr (1) byte;

;  declare prl$code$adr address;
prlcodeadr:
	ds	2
;  declare prl$code based prl$code$adr (1) byte;

;  declare prlen address;
prlen:
	ds	2

	cseg
;  declare tfcb$default (38) byte data (
tfcbdefault:
	db	0,'        ','   '
	db	0,0,0,0
	db	0,'        ','   '
	db	0,0,0,0
	db	0,0,0,0,0,0
;      /* setup tfcb:    005CH - 005CH = 0
;                        005DH - 0067H = ' '
;                        0068H - 006BH = 0
;               tfcb+16: 006CH - 006CH = 0
;                        006DH - 0077H = ' '
;                        0078H - 007BH = 0
;                        007CH - 007FH = 0
;               tbuff:   0080H - 0081H = 0  */

plderr:
	db	'Prg ld err'
	db	'$'

abstpana:
	db	'Abs TPA not free'
	db	'$'

insufrm:
	db	'Reloc seg not free'
	db	'$'

badprlhr:
	db	'Bad PRL hdr rec'
	db	'$'

prltype:
	db	'PRL'

comtype:
	db	'COM'

fltypblnk:
	db	'Blnk file type rqd'
	db	'$'

quefull:
	db	'Queue full'
	db	'$'

illegal:
	db	'Bad entry'
	db	'$'

;  pmove:
pmove:
	; BC = COUNT, DE = SOURCE ADR, HL = DEST ADR
;    procedure (n,s$adr,d$adr);
;      declare (n,s$adr,d$adr) address;
;      declare s based s$adr byte;
;      declare d based d$adr byte;

;      n = n + 1;
;      do while (n := n - 1) <> 0;
@38:
	MOV	A,B
	ORA	C
	RZ
;        if s >= 'a' and s <= 'z'
	LDAX	D
	MOV	M,A
	CPI	'a'
	JC	@2
	CPI	'z'+1
	JNC	@2
;          then d = s and 101$1111b;  /* force upper case */
	ANI	5FH
	MOV	M,A
;          else d = s;
@2:
;        s$adr = s$adr + 1;
	INX	D
;        d$adr = d$adr + 1;
	INX	H
;      end;
	DCX	B
	JMP	@38
;    end pmove;

;  setup$base$page:
setupbasepage:
;    procedure;

;      /* place a jump to xdos in the top three bytes
;         of the memory segment                       */
;      base,
;      mem$pointer = top - 3;
	LHLD	TOP
	DCX	H
	DCX	H
	DCX	H
	SHLD	BASE
;      instr = 0C3H;
	MVI	M,0C3H
;      mem$pointer = mem$pointer + 1;
	INX	H
;      location = .xbdos;

;      /* place a jump to the termination procedure (ENDP)
;         at the first three bytes of the memory segment  */
	LXI	B,XBDOS
	MOV	M,C
	INX	H
	MOV	M,B
;      if (mem$pointer := segment$bottom) <> 0000H then
	LHLD	SEGMENTBOTTOM
	MOV	A,H
	ORA	L
	JZ	@3
;      do;
;        instr = 0C3H;
	MVI	M,0C3H
;        mem$pointer = mem$pointer + 1;
	INX	H
;        location = .endp;
	LXI	B,ENDP
	MOV	M,C
	INX	H
	MOV	M,B
;      end;

;      /* place a jump to the mem segment top - 3 into
@3:
;         the normal bdos jump at mem segment 0005H    */
;      mem$pointer = segment$bottom + 5;
	LXI	D,5H
	LHLD	SEGMENTBOTTOM
	DAD	D
;      instr = 0C3H;
	MVI	M,0C3H
;      mem$pointer = mem$pointer + 1;
	INX	H
;      location = base;
	XCHG
	LHLD	BASE
	XCHG
	MOV	M,E
	INX	H
	MOV	M,D
;    end setup$base$page;
	RET

;  parse$command$tail:
parsecommandtail:
;    procedure;

;      call pmove (128-(nxt$chr$adr-.command$tail),nxt$chr$adr,
	LHLD	NXTCHRADR
	XCHG
	MOV	A,E
	SUI	LOW(COMMANDTAIL)
	MOV	B,A
	MVI	A,80H
	SUB	B
	LXI	B,0014H
	LHLD	PDADR
	DAD	B
	MOV	C,M
	INX	H
	MOV	H,M
	MOV	L,C
	PUSH	H
	INX	H
	MOV	C,A
	MVI	B,0
	CALL	PMOVE
	POP	H
	PUSH	H
;                 (mem$pointer := pd.disk$set$dma+1));
;      j = 0;
	MVI	B,0FFH
;      do while instr <> 0;
@40:
	INX	H
	INR	B
	MOV	A,M
	ORA	A
	JNZ	@40
;        mem$pointer = mem$pointer + 1;
;        j = j + 1;
;      end;
;      mem$pointer = pd.disk$set$dma;
	POP	H
;      instr = j;
	MOV	M,B
;      pcb.field$adr = nxt$chr$adr;
	LHLD	NXTCHRADR
	SHLD	PCB
;      pcb.fcb$adr = segment$bottom + 5CH;
	LXI	D,5CH
	LHLD	SEGMENTBOTTOM
	DAD	D
	SHLD	PCB+2H
;      if (nxt$chr$adr := xdos (parse$fname,.pcb)) <> 0FFFFH then
	LXI	B,PCB
	CALL	PARSEFILENAME
	SHLD	NXTCHRADR
	INX	H
	MOV	A,H
	ORA	L
	RZ
;      /* valid first file name in command tail */
;      do;
;        if nxt$chr$adr <> 0 then
	DCX	H
	MOV	A,H
	ORA	L
	RZ
;        /* parse second file name in command tail */
;        do;
;          pcb.field$adr = nxt$chr$adr + 1;
	INX	H
	SHLD	PCB
;          pcb.fcb$adr = segment$bottom + 6CH;
	LXI	D,6CH
	LHLD	SEGMENTBOTTOM
	DAD	D
	SHLD	PCB+2H
;          nxt$chr$adr = xdos (parse$fname,.pcb);
	LXI	B,PCB
	CALL	PARSEFILENAME
	SHLD	NXTCHRADR
;        end;
;      end;
;    end parse$command$tail;
	RET

;  relocate:
relocate:
;    procedure;

;      /* offset by base of reloc memseg */
;      offset = md.base;
;      /* bitmap directly follows last byte of code */
	LDA	MD
	MOV	B,A
;      bitmap$adr = .prl$code + prlen;
	LHLD	PRLEN
	XCHG
	LHLD	PRLCODEADR
	PUSH	H
	DAD	D
;      prlen = prlen - 1;
;      j = 0;
;      mask = 80H;
;      /* loop through entire bit map */
	MVI	C,80H
;      do i = 0 to prlen;
	; B = OFFSET, C = MASK, DE = PRLEN, HL = BITMAPADR
	; TOS = PRLCODEADR
@42:
;        if (bitmap(j) and mask) <> 0 then
	MOV	A,M
	ANA	C
	XTHL
	JZ	@6
;        /* offset the byte where a bitmap bit is on */
;        do;
;          prl$code(i) = prl$code(i) + offset;
	MOV	A,M
	ADD	B
	MOV	M,A
;        end;
@6:
	INX	H
	XTHL
;        /* move mask bit one position to the right */
;        if (mask := shr(mask,1)) = 0 then
	MOV	A,C
	RAR
	MOV	C,A
	JNC	@7
;        /* re-initialize mask and get next bitmap byte */
;        do;
;          mask = 80H;
	MVI	C,80H
;          j = j + 1;
	INX	H
;        end;
@7:
;      end;
	DCX	D
	MOV	A,D
	ORA	E
	JNZ	@42
;    end relocate;
	POP	H
	RET

;  pd$init:
pdinit:
;    procedure;

;      pd.pl = 0;
	LHLD	PDADR
	XRA	A
	MOV	M,A
	INX	H
	MOV	M,A
;      pd.status = rtr$status;
	INX	H
	MOV	M,A
;      pd.priority = user$priority;
	INX	H
	MVI	M,0C8H
	XCHG
;      pd.stkptr = .stktbl(rlrpd.memseg).loc(18);
	LXI	B,000FH
	LHLD	RLR
	DAD	B
	MOV	A,M
	LXI	B,0014H
	LXI	H,STKTBL-4
	INR	A
@MPM0:
	DAD	B
	DCR	A
	JNZ	@MPM0
	XCHG
	INX	H
	MOV	M,E
	INX	H
	MOV	M,D
;      call pmove (8,.fcb.fn,.pd.name);
	LXI	B,8H
	INX	H
	LXI	D,FCB+1H
	CALL	PMOVE
;      pd.console = rlrpd.console;
	XCHG
	LXI	B,000EH
	LHLD	RLR
	DAD	B
	MOV	A,M
	STAX	D
;      pd.memseg = rlrpd.memseg;
	INX	D
	INX	H
	MOV	A,M
	STAX	D
;      segment$bottom = shl(double(md.base),8);
	LDA	MD
	MOV	H,A
	MVI	L,0
	SHLD	SEGMENTBOTTOM
;      pd.disk$set$dma = segment$bottom + 0080H;
	LXI	D,0080H
	DAD	D
	XCHG
	LXI	B,0014H
	LHLD	PDADR
	DAD	B
	MOV	M,E
	INX	H
	MOV	M,D
;      pd.disk$slct = rlrpd.disk$slct;
	INX	H
	XCHG
	LXI	B,0016H
	LHLD	RLR
	DAD	B
	MOV	A,M
	STAX	D
;    end pd$init;
	RET

;  load:
load:
;    procedure;

;      /* obtain proc dscrptr adr from memsegtbl index */
;      pdadr = .pdtbl(rlrpd.memseg);

;      /* make dispatch call to force memory selection */
	LXI	B,0FH
	LHLD	RLR
	DAD	B
	MOV	A,M
	LXI	H,PDTBL-34H
	LXI	B,0034H
	INR	A
@MPM1:
	DAD	B
	DCR	A
	JNZ	@MPM1
	SHLD	PDADR
;      ret = xdos (dispatch,0);
	CALL	DISPATCH

;      /* initialize process descriptor */
;      call pd$init;

	CALL	PDINIT
;      base = segment$bottom + 0100H;
	LXI	D,100H
	LHLD	SEGMENTBOTTOM
	DAD	D
	SHLD	BASE
;      prl$code$adr = base;

;      /* setup stack */
	SHLD	PRLCODEADR
;      stktbl(pd.memseg).loc(19) = .endp;
	LXI	B,0FH
	LHLD	PDADR
	DAD	B
	MOV	A,M
	LXI	B,0014H
	LXI	H,STKTBL-2
	INR	A
@MPM2:
	DAD	B
	DCR	A
	JNZ	@MPM2
	LXI	B,ENDP
	MOV	M,C
	INX	H
	MOV	M,B
;      stktbl(pd.memseg).loc(18) = base;
	DCX	H
	DCX	H
	XCHG
	LHLD	BASE
	XCHG
	MOV	M,D
	DCX	H
	MOV	M,E
;      do i = 0 to 8;
	MVI	B,16
@44:
;        stktbl(pd.memseg).loc(i) = 0C7C7H;
	DCX	H
	MVI	M,0C7H
;      end;
	DCR	B
	JNZ	@44

;      top = segment$bottom + shl(double(md.size),8);
	LDA	MD+1H
	MOV	D,A
	MVI	E,0
	LHLD	SEGMENTBOTTOM
	DAD	D
	SHLD	TOP
;      ok = false;
;      notdone = true;

;      /* read COM or PRL+bitmap file into memory */
;      do while notdone;
@46:
;        if base = top then
	LHLD	BASE
	XCHG
	LHLD	TOP
	MOV	A,E
	CMP	L
	JNZ	@8
	MOV	A,D
	CMP	H
	JNZ	@8
;        do;
;          notdone = false;
;          if prl then ok = true;
	LDA	PRL
	RAR
	JC	@47
;          else
;          do;
;            call set$dma (.buffer);
	LXI	B,BUFFER
	CALL	SETDMA
;            if readbf (.fcb) = 1 then ok = true;
	LXI	B,FCB
	CALL	READBF
	DCR	A
	JZ	@47
	JMP	@15
;          end;
;        end;
@8:
;        else
;        do;
;          call set$dma (base);
	LHLD	BASE
	MOV	B,H
	MOV	C,L
	LXI	D,80H
	DAD	D
	SHLD	BASE
	CALL	SETDMA
;          base = base + sector$size;
;          if (ret := readbf (.fcb)) <> 0 then
	LXI	B,FCB
	CALL	READBF
	ORA	A
	JZ	@46
;          do;
;            notdone = false;
;            if ret = 1 then ok = true;
	DCR	A
	JNZ	@15
;          end;
;        end;
;      end;
@47:

;      /* free drives */
;      ret = xdos (39,0ffffh);
	mvi	c,39
	lxi	d,0ffffh
	call	xbdos
;      if ok then
;      /* file read with no errors */
;      do;
;        if prl then
	LDA	PRL
	RAR
;        /* page relocatable, do the relocation */
;        do;
;          call relocate;
	CC	RELOCATE
;        end;

;        call pmove (38,.tfcb$default,segment$bottom+5CH);
	LXI	B,26H
	LXI	D,5CH
	LHLD	SEGMENTBOTTOM
	DAD	D
	LXI	D,TFCBDEFAULT
	CALL	PMOVE
;        if nxt$chr$adr <> 0 then
	LHLD	NXTCHRADR
	MOV	A,H
	ORA	L
;        /* parse the command tail */
;        do;
;           call parse$command$tail;
	CNZ	PARSECOMMANDTAIL
;        end;

;        /* setup base page of memory segment */
;        call setup$base$page;

	CALL	SETUPBASEPAGE
;        /* attach the console to the process to be created */
;        console$attached(pd.console) = pdadr;

;        /* create - start the process */
	LHLD	PDADR
	XCHG
	LXI	H,000EH
	DAD	D
	MOV	C,M
	MVI	B,0
	LXI	H,cnsatt	; CONSOLEATTACHED
	DAD	B
	DAD	B
	MOV	M,E
	INX	H
	MOV	M,D
;        rlrpd.memseg = 0ffh; /* set clipd.memseg back to system */
	LXI	B,0FH
	LHLD	RLR
	DAD	B
	MVI	M,0FFH
;        ret = xdos (create,pdadr);

	MVI	C,90H
	JMP	XDOS
;      end; /* of successful file read */
@15:
;      else
;      /* file read errors */
;      do;
;        /* free the allocated memory segment */
;        call mem$fr (.md);
	LXI	B,MD
	CALL	MEMFR
;        rlrpd.memseg = 0ffh; /* set clipd.memseg back to system */
	LXI	B,0FH
	LHLD	RLR
	DAD	B
	MVI	M,0FFH
;        call print$b (.(
	LXI	B,plderr
	JMP	PRINTB
;          'Program load error.','$'));
;      end;
;    end load;

;  load$COM:
loadcom:
;    procedure;

;      md.base = 00H;
;      /* make absolute memory request */
	LXI	B,MD
	XRA	A
	STAX	B
;      if abs$rq (.md) = 0 then
	CALL	ABSRQ
	ORA	A
;      /* successful memory request */
;      do;
;        /* load and create process */
;        call load;
	JZ	LOAD
;      end;
;      else
;      /* unsuccessful memory request */
;      do;
;        call print$b (.('Absolute ',
	LXI	B,abstpana
	JMP	PRINTB
;          'TPA is not currently available.','$'));
;      end;
;    end load$COM;

;  load$PRL:
loadprl:
;    procedure;

;      ok = false;
;      /* read in first record, contains code size
;         and data size information                */
;      if readbf (.fcb) = 0 then
	LXI	B,FCB
	CALL	READBF
	ORA	A
	JNZ	@21
;      do;
;        /* obtain code length */
;        mem$pointer = .buffer(1);
	LXI	H,BUFFER+1H
;        prlen = location;

;        /* obtain data length */
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	SHLD	PRLEN
;        mem$pointer = mem$pointer + 3;
	XCHG
	INX	H
	INX	H
;        data$size = location;

;        /* compute size of memory segment needed */
	MOV	E,M
	INX	H
	MOV	D,M
;        md.size = high(prlen+0FFH)
;                 + high(data$size+0FFH)
;                 + shr(prlen,11)
;                 + 1;

	LXI	B,0FFH
	LHLD	PRLEN
	DAD	B
	MOV	A,H
	XCHG
	DAD	B
	ADD	H
	INR	A
	MOV	B,A
	LDA	PRLEN+1
	ANI	0F8H
	RRC
	RRC
	RRC
	ADD	B
	STA	MD+1H
;        /* ignore next sector */
;        if readbf (.fcb) = 0 then
	LXI	B,FCB
	CALL	READBF
	ORA	A
	JNZ	@22
;        do;
;          /* make relocatable memory request */
;          if rel$rq(.md) = 0 then
	LXI	B,MD
	CALL	RELRQ
	ORA	A
;          /* successful memory request */
;          do;
;            /* load and create process */
;            call load;
	JZ	LOAD
;            return;
;          end;
;          else
;          /* unsuccessful memory request */
;          do;
;            call print$b (.(
;              'Insufficient relocatable memory to',
	LXI	B,insufrm
	JMP	PRINTB
;              ' load program.','$'));
;            return;
;          end;
@24:
;        end; /* of successful ignore record read */
@22:
;      end; /* of successful header record read */
@21:
;      call print$b (.(
	LXI	B,badprlhr
	JMP	PRINTB
;        'Bad PRL header record.','$'));
;    end load$PRL;

;  file$load$execute:
fileloadexecute:
;    procedure;

;      call set$dma (.buffer);
	LXI	B,BUFFER
	CALL	SETDMA
;      prl = true;
	LXI	H,PRL
	MVI	M,0FFH
;      if fcb.ft(0) = ' ' then
	LXI	H,FCB+9H
	MOV	A,M
	CPI	20H
	JNZ	@25
;      /* type must be left blank */
;      do;
;        call pmove (3,.('PRL'),.fcb.ft);
	MVI	M,'P'
	INX	H
	MVI	M,'R'
	INX	H
	MVI	M,'L'

;        /* first try for PRL file */
;        if open (.fcb) = 0FFH then
	LXI	B,FCB
	CALL	OPEN
	INR	A
	JNZ	LOADPRL
;        /* PRL file not found, try COM file */
;        do;
;          prl = false;
	LXI	H,PRL
	MVI	M,0H
;          call pmove (3,.('COM'),.fcb.ft);
	LXI	H,FCB+9H
	MVI	M,'C'
	INX	H
	MVI	M,'O'
	INX	H
	MVI	M,'M'
;          if open (.fcb) = 0FFH then
	LXI	B,FCB
	CALL	OPEN
	INR	A
	JNZ	LOADCOM
;          /* unsuccessful file open */
;          do;
;            call print$b (.(
	LXI	H,FCB+9H
@23:
	DCX	H
	MOV	A,M
	CPI	' '
	JZ	@23
	INX	H
	MVI	M,'?'
	INX	H
	MVI	M,'$'
	LXI	B,FCB+1H
	JMP	PRINTB
;              'No such file.','$'));
;            return;
;          end;
;        end;

;        /* successful file open */
;        if prl then
;        /* relocatable load */
;        do;
;          call load$PRL;
;        end;
;        else
;        /* COM file load */
;        do;
;          call load$COM;
;        end;
;      end; /* of blank file type */
@25:
;      else
;      do;
;      /* non-blank file type */
;        call print$b (.(
	LXI	B,fltypblnk
	JMP	PRINTB
;          'File type must not be specified.','$'));
;      end;
;    end file$load$execute;

;  queue$message:
queuemessage:
;    procedure boolean;

;      call pmove (8,.fcb.fn,.cusp$uqcb.name);
	LXI	B,8H
	LXI	H,CUSPUQCB+4H
	LXI	D,FCB+1H
	CALL	PMOVE
;      if xdos (open$queue,.cusp$uqcb) = 0 then
	LXI	B,CUSPUQCB
	CALL	openq
	ORA	A
	MVI	A,0
	RNZ
;      /* queue exists */
;      do;
;        call pmove (8,.fcb.fn,.pname(1));
	LXI	B,8H
	LXI	H,PNAME+1H
	LXI	D,FCB+1H
	CALL	PMOVE
;        /* assign the console to the process, if any,
;           associated with the queue. a console is
;           associated with a process if there is a
;           process with the same name as the queue.   */
;        ret = assign (.pname);

	LXI	B,PNAME
	CALL	ASSIGN
;        if nxt$chr$adr <> 0 then
	LHLD	NXTCHRADR
	MOV	A,H
	ORA	L
	JZ	@32
;        /* copy the command tail */
;        do;
;          call pmove (128-(nxt$chr$adr-.command$tail),
	LHLD	NXTCHRADR
	XCHG
	MOV	A,E
	INX	D
	SUI	LOW(COMMANDTAIL)
	MOV	B,A
	MVI	A,80H
	SUB	B
	MOV	C,A
	MVI	B,0
	LXI	H,FIELD+2H
	CALL	PMOVE
;            nxt$chr$adr+1,.field(2));
;        end;
	JMP	@33
@32:
;        else
;        /* put a <cr> in first field position */
;        do;
;          field(2) = 0dh;
	LXI	H,FIELD+2H
	MVI	M,0DH
;        end;
@33:

;        /* conditionally write the message to the queue */
;        if xdos (cond$write$queue,.cusp$uqcb) <> 0 then
	LXI	B,CUSPUQCB
	CALL	cwriteq
	ORA	A
;        /* write failed, buffer not available */
;        do;
;          call print$b (.(
	LXI	B,quefull
	CNZ	PRINTB
;            'Queue full.','$'));
;        end;
@34:
;        return true;
	MVI	A,0FFH
	RET
;      end; /* of successful queue open */
;      /* queue open failed */
;      return false;
;    end queue$message;


;/*
;  cli:
;*/

;  cli:
cli:
;    procedure;

;      ret = xdos (make$queue,.cli$lqcb);

	LXI	B,CLILQCB
	CALL	makeq
;      do forever;
@48:
;        ret = xdos (read$queue,.CLIQ);
	LXI	B,CLIQ
	CALL	readq
;        pname(0),
;        rlrpd.console = console;
	LDA	CONSOLE
	STA	PNAME
	LHLD	RLR
	XCHG
	LXI	H,000EH
	DAD	D
	MOV	M,A
;        rlrpd.disk$slct = disk$select;
	LXI	H,16H
	DAD	D
	LDA	DISKSELECT
	MOV	M,A
;        pcb.field$adr = .command$tail;
	LXI	H,COMMANDTAIL
	SHLD	PCB
;        pcb.fcb$adr = .fcb;
	LXI	H,FCB
	SHLD	PCB+2H
;        if (nxt$chr$adr := xdos (parse$fname,.pcb)) <> 0FFFFH then
	LXI	B,PCB
	CALL	parsefilename
	SHLD	NXTCHRADR
	INX	H
	MOV	A,H
	ORA	L
	JZ	@35
;        /* legitimate queue or file name entered */
;        do;
;          fcb.nr = 0;
;          /* test for message to be queued */
	LXI	H,FCB+20H
	MVI	M,0H
;          if not queue$message
	CALL	QUEUEMESSAGE
	RAR
	JC	@37
;          then
;          /* file is to be loaded and executed */
;          do;
;            call file$load$execute;
	CALL	FILELOADEXECUTE
;          end;
;        end;
	JMP	@37
@35:
;        else
;        /* illegitimate queue or file name */
;        do;
;          call print$b (.(
	LXI	B,illegal
	CALL	PRINTB
;            'Illegal entry.','$'));
;        end;
@37:

;        /* detach the console from CLI, if still attached */
;        ret = xdos (detach,0);
	LXI	B,CLIPD
	CALL	DETACH
;        /* free drives, if still accessed */
;        ret = xdos (39,0ffffh);
	mvi	c,39
	lxi	d,0ffffh
	call	xbdos
;      end; /* of forever */
	JMP	@48

;    end cli; /* procedure */

;end cli;
	END
