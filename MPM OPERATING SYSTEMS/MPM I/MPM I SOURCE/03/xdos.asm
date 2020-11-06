$title	 ('MP/M 1.1 Extended Disk Operating System')
	name	xdos
	cseg
;xdos:
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
;$include (queue.lit)
;$nolist
;$include (proces.lit)
;$nolist
;$include (memmgr.lit)
;$nolist
;$include (datapg.ext)
;$nolist
;$include (proces.ext)
;$nolist
;$include (queue.ext)
;$nolist
;$include (flag.ext)
;$nolist
;$include (memmgr.ext)
;$nolist
;$include (th.ext)
;$nolist
;  parse$filename:
	extrn	parsefilename
;    procedure (pcb$address) address external;
;      declare pcb$address address;
;    end parse$filename;

;  exitr:
	extrn	exitr
;    procedure external;
;    end exitr;

;  flgwt:
	extrn	flgwt
;    procedure (flgnmb) byte external;
;      declare flgnmb byte;
;    end flgwt;

;  flgset:
	extrn	flgset
;    procedure (flgnmb) byte external;
;      declare flgnmb byte;
;    end flgset;

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

;  dispat:
	extrn	dispat
;    procedure external;
;    end dispat;

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

;  deletq:
	extrn	deletq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end deletq;

;  readq:
	extrn	readq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end readq;

;  creadq:
	extrn	creadq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end creadq;

;  writeq:
	extrn	writeq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end writeq;

;  cwriteq:
	extrn	cwriteq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end cwriteq;

;  assign:
	extrn	assign
;    procedure (pname) byte external;
;      declare pname address;
;    end assign;

;  declare dparam address external;
	extrn	dparam

;  declare rlr address external;
	extrn	rlr

;  declare drl address external;
	extrn	drl

;  declare dlr address external;
	extrn	dlr

;  declare plr address external;
	extrn	plr

;  declare slr address external;
	extrn	slr

;  declare qlr address external;
	extrn	qlr

;  declare thrdrt address external
	extrn	thrdrt

;  declare nmbcns byte external;
	extrn	nmbcns

;  declare cnsque (1) address external;
	extrn	cnsque

;  declare tod structure (
	extrn	tod
;    day address,
;    hr byte,
;    min byte,
;    sec byte ) external;

;  declare nmbflags byte external;
	extrn	nmbflags

;  declare sysfla (1) address external;
	extrn	sysfla

;  declare sysdat address external;
	extrn	sysdat

;  declare cli$lqcb address external;
	extrn	clilqcb

	dseg
;  declare cli$uqcb userqcbhead
;    initial (.cli$lqcb,0);
cliuqcb:
	dw	clilqcb	; pointer
	dw	$-$	; msgadr

;  declare rsp$adr address;
rspadr:	ds	2

;  declare rsp$uqcb userqcb
;    initial (0,.rsp$adr,'$$$$$$$$');
rspuqcb:
	dw	$-$	; pointer
	dw	rspadr	; msgadr
	db	'$$$$$$$$'	; name

;  declare cpb$adr address;
cpbadr:	ds	2
;  declare cpb based cpb$adr structure (
;    rsp$name$adr address,
;    rsp$param address);

;  declare flag$wait$function literally '4';
;  declare flag$set$function literally '5';

;  declare (svstk,old$drl,old$thread$root) address;
svstk:	ds	2
olddrl:	ds	2
oldthreadroot:
	ds	2
	cseg

;  xdos:
xdos:
	public	xdos
;    procedure (function,parameter) address reentrant public;
;      declare function byte;
;      declare parameter address;
;      declare ret address;
;      declare pd based parameter process$descriptor;

;      ret = 0000H;
	MOV	A,C
;      function = function - 128;
	SUI	80H
;      if function > max$xdos$function then
	CPI	maxfunc
	LXI	H,0FFFFH
	RNC
;      do;
;        ret = 0FFFFH;
;      end;
;      else
;      do case function;
	CPI	4H
	JZ	@1	; SETUP RETURN TO DISPATCH
	CPI	5H	;  FOR ALL BUT FLAG OP'S
	JZ	@1
	LXI	H,DISPAT
	PUSH	H
@1:
	LXI	H,EXDOS
	PUSH	H
	MOV	B,D
	MOV	C,E
	MOV	E,A
	MVI	D,0
	LXI	H,@37
	DAD	D
	DAD	D
	MOV	E,M
	INX	H
	MOV	D,M
	LHLD	RLR
	INX	H
	INX	H	; HL = .RLR.STATUS
	XCHG
	PCHL

;        /* function = 128, Absolute Memory Request */
;        ret = abs$rq (parameter);
;	CALL	ABSRQ

;        /* function = 129, Relocatable Memory Request */
;        ret = rel$rq (parameter);
;	CALL	RELRQ

;        /* function = 130, Memory Free */
;        call mem$fr (parameter);
;	CALL	MEMFR

;        /* function = 131, Poll Device */
;        do;
@7:
;          enter$region;
	DI
;          dsptch$param = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	DPARAM
;          rlrpd.status = poll$status;
	XCHG
	MVI	M,3H
;        end;
	RET

;        /* function = 132, Flag Wait */
;        ret = flag$wait (parameter);
;	CALL	FLGWT

;        /* function = 133, Flag Set */
;        ret = flag$set (parameter);
;	CALL	FLGSET

;        /* function = 134, Make Queue */
;        ret = makeq (parameter);
;	CALL	MAKEQ

;        /* function = 135, Open Queue */
;        ret = openq (parameter);
;	CALL	OPENQ

;        /* function = 136, Delete Queue */
;        ret = deletq (parameter);
;	CALL	DELETQ

;        /* function = 137, Read Queue */
;        ret = readq (parameter);
;	CALL	READQ

;        /* function = 138, Conditional Read Queue */
;        ret = creadq (parameter);
;	CALL	CREADQ

;        /* function = 139, Write Queue */
;        ret = writeq (parameter);
;	CALL	WRITEQ

;        /* function = 140, Conditional Write Queue */
;        ret = cwriteq (parameter);
;	CALL	CWRITEQ

;        /* function = 141, Delay */
;        do;
@17:
;          enter$region;
	DI
;          dsptch$param = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	DPARAM
;          rlrpd.status = delay$status;
	XCHG
	MVI	M,5H
;        end;
	RET

;        /* function = 142, Dispatch */
;        do;
@18:
;          enter$region;
	DI
;          rlrpd.status = dispatch$status;
	XCHG
	MVI	M,9H
;        end;
	RET

;        /* function = 143, Terminate */
;        do;
@19:
;          enter$region;
	DI
;          dsptch$param = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	DPARAM
;          rlrpd.status = terminate$status;
	XCHG
	MVI	M,7H
;        end;
	RET

;        /* function = 144, Create */
;        do;
@20:
;          enter$region;
	DI
;          old$drl = drl;
	LHLD	DRL
	SHLD	OLDDRL
;          old$thread$root = thread$root;
	LHLD	THRDRT
	SHLD	OLDTHREADROOT
;          drl,
;          thread$root = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	DRL
	SHLD	THRDRT
;          do while pd.pl <> 0;
@38:
	MOV	E,M
	INX	H
	MOV	D,M
	MOV	A,E
	ORA	D
	JZ	@39
;            pd.thread = pd.pl;
	LXI	H,12H
	DAD	B
	MOV	M,E
	INX	H
	MOV	M,D
	CALL @39A
;            parameter = pd.pl;
	MOV	B,D
	MOV	C,E
	XCHG
;          end;
	JMP	@38
@39:
;          pd.pl = old$drl;
	XCHG
	LHLD	OLDDRL
	XCHG
	MOV	M,D
	DCX	H
	MOV	M,E
;          pd.thread = old$thread$root;
	LHLD	OLDTHREADROOT
	XCHG
	LXI	H,12H
	DAD	B
	MOV	M,E
	INX	H
	MOV	M,D
@39A:
;            pd.drvacc = 0;
	lxi	h,1ch
	dad	b
	xra	a
	mov	m,a
	inx	h
	mov	m,a
;        end;
	RET

;        /* function = 145, Set Priority */
;        do;
@21:
;          enter$region;
	DI
;          dsptch$param = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	DPARAM
;          rlrpd.status = set$prior$status;
	XCHG
	MVI	M,8H
;        end;
	RET

;        /* function = 146, Attach */
;        do;
@22:
;          enter$region;
	DI
;          rlrpd.status = attach$status;
	XCHG
	MVI	M,0AH
;        end;
	RET

;        /* function = 147, Detach */
;        do;
@23:
;          enter$region;
	DI
;          rlrpd.status = detach$status;
	XCHG
	MVI	M,0BH
;        end;
	RET

;        /* function = 148, Set Console */
;        do;
@24:
	lxi	h,nmbcns
	mov	a,c
	cmp	m
	mvi	a,0ffh
	rnc			;** return if bad console #
;          enter$region;
	DI
;          dsptch$param = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	DPARAM
;          rlrpd.status = set$cns$status;
	XCHG
	MVI	M,0CH
;        end;
	RET

;        /* function = 149, Assign Console */
;        ret = assign (parameter);
;	CALL	ASSIGN

;        /* function = 150, Send CLI Command */
;        do;
@26:
;          cli$uqcb.msgadr = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	CLIUQCB+2H
;          ret = writeq (.cli$uqcb);
	LXI	B,CLIUQCB
	JMP	WRITEQ
;        end;

;        /* function = 151, Call Resident System Process */
;        do;
@27:
;          enter$region;
	DI
	POP	H	; DISCARD EXDOS RETURN
;            cpb$adr = parameter;
	MOV	L,C
	MOV	H,B
	SHLD	CPBADR
;            call move (8,cpb.rsp$name$adr,.rsp$uqcb.name);
	MOV	C,M
	INX	H
	MOV	B,M
	LXI	D,RSPUQCB+4H
	MVI	L,8H
	LDAX	B
	STAX	D
	INX	B
	INX	D
	DCR	L
	JNZ	$-5H
;            /* open queue having passed procedure name */
;            if low(ret := xdos (135,.rsp$uqcb)) <> 0ffh then
	LXI	B,RSPUQCB
	CALL	OPENQ
	INR	A
	LXI	H,0001H
	JZ	@30
;            do;
;              /* read queue to get procedure entry point address */
;              ret = xdos (137,.rsp$uqcb);
;              /* execute the procedure (function) */
	LXI	B,RSPUQCB
	CALL	READQ
;              ret = xfunc (cpb.rsp$param,rsp$adr);
;              /* write queue to put message back on queue, this
;                 mechanism makes the procedure a serially
;                 resuseable resource                            */
	LHLD	CPBADR
	INX	H
	INX	H
	MOV	C,M
	INX	H
	MOV	B,M
	LHLD	RSPADR
	LXI	D,XFUNC
	PUSH	D
	PCHL
XFUNC:
;              if low(xdos (139,.rsp$uqcb)) = 0ffh
	PUSH	H
	LXI	B,RSPUQCB
	CALL	WRITEQ
	INR	A
	POP	H
	JNZ	@30
;                then ret = 0ffffh;
	LXI	H,0FFFFH
;            end;
;            else
;            do;
;              /* procedure not resident */
;              ret = 1;
;            end;
;          exit$region;
;        end;
@30:
	PUSH	H
	CALL	EXITR
	POP	H
	RET

;        /* function = 152, Parse Filename */
;        ret = parse$filename (parameter);
@31:
	POP	H	; DISCARD EXDOS RETURN
	JMP	PARSEFILENAME

;        /* function = 153, Get Console Number */
;        ret = rlrpd.console;
@32:
	LXI	B,0CH
	XCHG
	DAD	B
	MOV	A,M
	RET

;        /* function = 154, System Data Address */
;        ret = sysdat;
@33:
	POP	H	; DISCARD EXDOS RETURN
	LHLD	SYSDAT
	RET

;        /* function = 155, Get Time & Date */
;        do;
@34:
;          call move (5,.tod,parameter);
	LXI	D,TOD
	MVI	L,5H
	LDAX	D
	STAX	B
	INX	B
	INX	D
	DCR	L
	JNZ	$-5H
;        end;
	RET

;        /* function = 156, Return Process Descriptor Address */
;	 do;
rtnpdadr:
;          return rlr;
	pop	h	; discard EXDOS return
	lhld	rlr
;        end;
	ret

;        /* function = 157, Abort Specified Process */
;        do;
abort:
;
; BC -> Abort$parameter$control$block
; declare apcb structure (
;   pdadr address,
;   param address,
;   pname (8) byte );
	ldax	b
	inx	b
	mov	e,a
	ldax	b
	inx	b
	mov	d,a
	ora	e	;test for PD address present
	ldax	b
	inx	b
	mov	l,a
	ldax	b
	inx	b
	mov	h,a	;DE = apcb.pdadr, HL = apcb.param
	di
	shld	dparam
	shld	tparam
	jnz	@15	;jump if already have pdadr
;      pdadr = thread$root;
	LHLD	THRDRT
	XCHG
;      do while pdadr <> 0;
@9:
;        i = 1;
	PUSH	B
	LXI	H,6
	DAD	D
	PUSH	H
	MVI	L,9	;** this includes the pd.console byte !
;        do while (i <> 9) and (pd.name(i-1) = pname(i));
@11:
	XTHL
	LDAX	B
	sub	M
	INX	B
	INX	H
	JZ	@11A
	ani	7fh
	jz	@11a	;also don't care on high order bit
	POP	H
	POP	B
	JMP	@6
@11A:
	XTHL
	DCR	L
	JNZ	@11
;          i = i + 1;
;        end;
	pop	h
	pop	b
	jmp	@15
@6:
;        pdadr = pd.thread;
	LXI	H,12H
	DAD	D
	MOV	E,M
	INX	H
	MOV	D,M
	MOV	A,D
	ORA	E
	JNZ	@9
;      end;
exitabort:
;      return 0FFFFH;
	call	exitr
	mvi	a,0ffh
	RET
@15:
	lxi	h,6
	dad	d
	mov	a,m
	cpi	'@'
	jz	exitabort	;cannot abort @pdname
	lhld	rlr
	mov	a,e
	cmp	l
	jnz	@16
	mov	a,d
	cmp	h
	jnz	@16
;
; aborting the running process
	inx	h
	inx	h
	mvi	m,7	;set pd.status to terminate
	ret
@16:
	lxi	b,setupabort
	push	b		;setup return address
	lxi	h,2
	dad	d
	mov	c,m
	mvi	b,0
	lxi	h,abtbl
	dad	b
	dad	b
	mov	a,m
	inx	h
	mov	h,m
	mov	l,a
	pchl

abtbl:
	dw	abtrun	; 0 = Ready to run
	dw	abtque	; 1 = DQ
	dw	abtque	; 2 = NQ
	dw	abtpol	; 3 = Poll
	dw	abtflg	; 4 = Flag Wait
	dw	abtdly	; 5 = Delay
	dw	abtswp	; 6 = Swap
	dw	abtrun	; 7 = Terminate
	dw	abtrun	; 8 = Set Priority
	dw	abtrun	; 9 = Dispatch
	dw	abtcns	;10 = Attach
	dw	abtrun	;11 = Detach
	dw	abtcns	;12 = Set Console

;
; 0 Ready to run
; 7 Terminate
; 8 Set Priority
; 9 Dispatch
;11 Detach
;
abtrun:
	pop	h	;discard return addr
	jmp	procrdy	;no action simply setup abort

;
; 1 DQ
; 2 NQ
;
abtque:
	;find queue link & remove
	lxi	b,qlr
abtq0:
	ldax	b
	mov	l,a
	inx	b
	ldax	b
	mov	h,a
	ora	l
	rz		;not DQing or NQing ?
	push	h
	lxi	b,14
	dad	b
	mov	b,h
	mov	c,l
	push	b
	call	delpr
	pop	b
	pop	h
	rc		;return if DQing & removed
	push	h
	inx	b
	inx	b
	call	delpr
	pop	b
	rc		;return if NQing & removed
	jmp	abtq0

;
; 3 Poll
;
abtpol:
	;remove PD from poll list
	lxi	b,plr
	jmp	delpr
	;ret

;
; 4 Flag Wait
;
abtflg:
	;remove PD from flag
	lda	nmbflags
	mov	c,a
	inr	c
	lxi	h,sysfla-1
abtfl0:
	inx	h
	dcr	c
	rz		;not waiting for a flag ?!
	mov	a,e
	cmp	m
	inx	h
	jnz	abtfl0
	mov	a,d
	cmp	m
	jnz	abtfl0
	mvi	m,0ffh
	dcx	h
	mvi	m,0ffh
	ret

;
; 5 Delay
;
abtdly:
	;remove PD from delay list
	lxi	b,dlr
	jmp	delpr
	;ret

;
; 6 Swap
;
abtswp:
	;remove PD from swap list
	lxi	b,slr
	jmp	delpr
	;ret

;
;10 Attach
;12 Set Console
;
abtcns:
	;remove PD from console queue
	lda	nmbcns
	inr	a
	mov	l,a
	lxi	b,cnsque
abct0:
	dcr	l
	rz		;not queued for any console ?
	push	b
	push	h
	call	delpr
	pop	h
	pop	b
	inx	b
	inx	b
	jnc	abct0
	ret

setupabort:
	;put PD on dispatcher ready list
	lhld	drl
	xchg
	mov	m,e
	inx	h
	mov	m,d
	dcx	h
	shld	drl
	xchg
procrdy:
	;compute process return address location in stack
	;and fill in with address of abort code.
	lxi	h,abortcode
	shld	abtcdadr
	lxi	h,4
	dad	d	;HL = .pd.stkptr
	lxi	d,abtstack
	mov	m,e
	inx	h
	mov	m,d
	call	exitr
	xra	a
	ret

abortcode:
	di
	mvi	c,143
	lxi	d,$-$	;Note: Potential critical parameter !
tparam	equ	$-2
	jmp	xdos

	dseg
abtstack:
abtcdadr:
	dw	abortcode

	cseg

;        end;

;      end; /* case */
@37:
	DW	ABSRQ
	DW	RELRQ
	DW	MEMFR
	DW      @7
	DW	FLGWT
	DW	FLGSET
	DW	MAKEQ
	DW	OPENQ
	DW	DELETQ
	DW	READQ
	DW	CREADQ
	DW	WRITEQ
	DW	CWRITEQ
	DW      @17
	DW      @18
	DW      @19
	DW      @20
	DW      @21
	DW      @22
	DW      @23
	DW      @24
	DW	ASSIGN
	DW      @26
	DW      @27
	DW	@31
	DW      @32
	DW      @33
	DW      @34
	dw	rtnpdadr
	dw	abort
maxfunc	equ	($-@37)/2

;      if function <> flag$set$function then
;      do;
;        if function <> flag$wait$function then
;        do;
;          call dispatch;
;        end;
;      end;
;      return ret;
EXDOS:
	MVI	H,0
	MOV	L,A
	RET
;    end xdos;

delpr0:
	mov	b,h
	mov	c,l
;  delete$process:
;    procedure (nxtpdladr,pdadr) public;
delpr:
	public	delpr
;      declare (nxtpdladr,pdadr) address;
;      declare pdladr based nxtpdladr address;
	ldax	b
	mov	l,a
	inx	b
	ldax	b
	dcx	b
	mov	h,a
	ora	l
	rz		;end of list with no match
	mov	a,l
	cmp	e
	jnz	delpr0
	mov	a,h
	cmp	d
	jnz	delpr0
	mov	a,m	;found match, update pointers
	stax	b
	inx	h
	inx	b
	mov	a,m
	stax	b
	stc		;indicate success
	ret
;end xdos;
	END
