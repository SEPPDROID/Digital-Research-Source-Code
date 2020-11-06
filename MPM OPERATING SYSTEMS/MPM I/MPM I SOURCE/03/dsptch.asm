$title('MP/M 1.1 Dispatcher')
	name	dsptch
;dsptch:
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

;/*
;    Common Literals
;*/

;  declare true literally '0FFFFH';
;  declare false literally '0';
;  declare forever literally 'while true';
;  declare boolean literally 'byte';

;  declare enter$region literally
;    'disable';

;/*
;    Proces Literals
;*/

;  declare process$header literally
;    'structure (pl address,
;                status byte,
;                priority byte,
;                stkptr address';
;  declare bdos$save literally
;               'disk$set$dma address,
;                disk$slct byte,
;                dcnt byte,
;                searchl byte,
;                searcha address,
;                thread address,
;                scratch (14) byte)';
;  declare process$descriptor literally
;               'process$header,
;                name (8) byte,
;                console byte,
;                memseg byte,
;                 b address,
;                bdos$save';

;  declare rtr$status       literally '0',
;          dq$status        literally '1',
;          nq$status        literally '2',
;          poll$status      literally '3',
;          FlgWt$status     literally '4',
;          Delay$status     literally '5',
;          Swap$status      literally '6',
;          Terminate$status literally '7',
;          Set$Prior$status literally '8',
;          Dispatch$status  literally '9',
;          Attach$status    literally '10',
;          Detach$status    literally '11',
;          Set$cns$status   literally '12';

;/*
;    Memmgr Literals
;*/

;  declare memory$descriptor literally
;    'structure (base byte,
;                size byte,
;                attrib byte)';

;/*
;    Th Literals
;*/

;  declare nmb$consoles literally '2';
	extrn	maxcns

;/*
;    Bdosi Externals
;*/

;  xios$pl:
	extrn	xiospl
;    procedure (device) byte external;
;      declare device byte;
;    end xios$pl;

;  xios$ms:
	extrn	xiosms
;    procedure (memsegadr) external;
;      declare memsegadr address;
;    end xios$ms;

;  str$clk:
	extrn	strclk
;    procedure external;
;    end str$clk;

;/*
;    Flag Externals
;*/

;  declare sys$flag (1) address external;
	extrn	sysflag

;/*
;    Memmgr Externals
;*/

;  declare memsegtbl (1) memory$descriptor external;
	extrn	memsegtbl

;  user$process:
	extrn	userprocess
;    procedure (pdadr) byte external;
;      declare pdadr$address;
;    end user$process;

;  mem$fr:
	extrn	memfr
;    procedure (mdadr) external;
;      declare mdadr address;
;    end mem$fr;

;/*
;    Th Externals
;*/

;  attach:
	extrn	attach
;    procedure (pdadr) external;
;      declare pdadr address;
;     end attach;

;  detach:
	extrn	detach
;    procedure (pdadr) external;
;      declare pdadr address;
;    end detach;


	dseg

;  declare rlr address external;
	extrn	rlr

;  declare rlrpd based rlr process$descriptor;

;  declare thread$root address external;
	extrn	thrdrt

;  declare drl address external;
	extrn	drl

;  declare drlpd based drl process$descriptor;

;  declare plr address external;
	extrn	plr

;  declare dlr address external;
	extrn	dlr

;  declare slr address external;
	extrn	slr

;  declare indisp byte public initial (0);
	public	indisp
indisp:	db	0

;  declare dsptch$param address public;
	public dparam
dparam:
dsptchparam:
	dw	0

;  declare pdadr address;
pdadr:
	dw	0

;  declare pd based pdadr process$descriptor;

;  declare pollpointer address;
pollpointer:
	dw	0

;  declare oldpollptr address;
oldpollptr:
nextthreadptr:
	dw	0

;  declare oldpoll based oldpollptr address;

;  declare next$thread$ptr address at (.oldpollptr);

;  declare next$thread based next$thread$ptr address;

;  declare pdladr address;
pdladr:
	dw	0

;  declare pdl based pdladr process$descriptor;

;  declare first$time boolean;


;/*
;  delay$list$insert:
;          The purpose of the delay list insert procedure is to
;        place a process descriptor on the delay list.  The delay
;        list is maintained in such a manner that the tick counts
;        in the process descriptor represent the value required to
;        produce the specified delay, by adding together all the
;        tick counts of the preceding processes on the delay list.

;  Entry Conditions:
;        Dispatch$param = ticks

;  Exit Conditions:
;        Note

;        *** NOTE *** this procedure assumes that
;                      a critical region has been entered.
;*/

	cseg

;  delay$list$insert:
delaylistinsert:
;    procedure;

	dseg
;      declare accum address;
accum:
	dw	0
;      declare nxtpdladr address;
nxtpdladr:
	dw	0
;      declare nxtpdl based nxtpdladr address;
;

	cseg
;      dsptch$param = dsptch$param+1;
	lxi	h,DSPTCHPARAM
	inr	m
;        if dlr = 0 then call strclk;
	LHLD	DLR
	MOV	A,H
	ORA	L
	CZ	STRCLK
;      accum = 0;
	LXI	H,0H
	SHLD	ACCUM
;      nxtpdladr = .dlr;
	LXI	H,DLR
	SHLD	NXTPDLADR
;        do forever;
;
@25:
;          pdladr = nxtpdl;
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	SHLD	PDLADR
;          if (nxtpdl = 0) or
	XCHG
	DCX	H
	MOV	A,M
	INX	H
	ORA	M
	JZ	@25A
	LXI	H,10H
	DAD	D
	XCHG
	LHLD	ACCUM
	CALL	PADD
	LXI	D,DSPTCHPARAM
	CALL	PSUB
	JNC	@3
;             ((accum + pdl.b) > dsptch$param) then
;          do;
@25A:
;            pd.pl = nxtpdl;
	LHLD	NXTPDLADR
	MOV	C,M
	INX	H
	MOV	B,M
	DCX	H
	XCHG
	LHLD	PDADR
	MOV	M,C
	INX	H
	MOV	M,B
;            nxtpdl = pdadr;
	DCX	H
	XCHG
	MOV	M,E
	INX	H
	MOV	M,D
;            accum = dsptch$param - accum;
	LHLD	ACCUM
	LXI	D,DSPTCHPARAM
	CALL	PADD
	SHLD	ACCUM
;            pd.b = accum;
	XCHG
	LXI	B,10H
	LHLD	PDADR
	DAD	B
	PUSH	H
	MOV	M,E
	INX	H
	MOV	M,D
;            if pd.pl <> 0 then pdl.b = pdl.b - accum;
	LHLD	PDADR
	MOV	A,M
	INX	H
	ORA	M
	POP	D
	RZ
	LHLD	ACCUM
	CALL	PSUB
	XCHG
	DCX	H
	MOV	M,E
	INX	H
	MOV	M,D
;            return;
	RET
;          end;
@3:
;          accum = accum + pdl.b;
	LXI	B,10H
	LHLD	PDLADR
	DAD	B
	XCHG
	LHLD	ACCUM
	CALL	PADD
	SHLD	ACCUM
;          nxtpdladr = nxtpdl;
	LHLD	NXTPDLADR
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	SHLD	NXTPDLADR
;        end;
	JMP	@25
;      end;
;    end delay$list$insert;

PADD:
	; HL = [DE] + HL
	LDAX	D
	ADD	L
	MOV	L,A
	INX	D
	LDAX	D
	ADC	H
	MOV	H,A
	RET

PSUB:
	; HL = [DE] - HL
	LDAX	D
	SUB	L
	MOV	L,A
	INX	D
	LDAX	D
	SBB	H
	MOV	H,A
	RET

;/*
;  insert$process:
;          The purpose of the insert process procedure is to
;        place a process desciptor into a list pointed to by a
;        passed parameter in priority order.  The algorithm
;        searches for a process in the list with a lower priority
;        (lower priorities have high priority numbers) to make
;        the insertion.  This ensures that processes of the same
;        priority are round-robin scheduled.

;  Entry Conditions:
;        BC = list root
;        DE = process descriptor

;        *** NOTE *** this procedure assumes that
;                      a critical region has been entered.

;  Exit Conditions:
;        None
;*/

	cseg
@5:
	MOV	B,H
	MOV	C,L
;  insert$process:
inspr:
insertprocess:
;    procedure (nxtpdladr,pdadr) public;
	public	inspr

;      declare (nxtpdladr,pdadr) address;
;      declare nxtpdl based nxtpdladr address;
;      declare pd based pdadr process$descriptor;


;      do forever;
;        pdladr = nxtpdl;
	; BC = NXTPDLADR, DE = PDADR
@27:
	LDAX	B
	MOV	L,A
	INX	B
	LDAX	B
	MOV	H,A
;        if (nxtpdl = 0) or
	ORA	L
	INX	D
	INX	D
	INX	D
	LDAX	D
	DCX	D
	DCX	D
	DCX	D
	JZ	@27A
	; HL = PDLADR
	INX	H
	INX	H
	INX	H
	SUB	M
	DCX	H
	DCX	H
	DCX	H
	JNC	@5
;           (pd.priority < pdl.priority) then
;        do;
;          pd.pl = nxtpdl;
@27A:
	MOV	A,L
	STAX	D
	INX	D
	MOV	A,H
	STAX	D
	DCX	D
;          nxtpdl = pdadr;
	MOV	A,D
	STAX	B
	DCX	B
	MOV	A,E
	STAX B
;          return;
	RET
;        end;
;        nxtpdladr = nxtpdl;
;      end;
;    end insert$process;

;/*
;  dispatch:
;          The purpose of the dispatch procedure is to perform the
;        action required by the status of the current running
;        process, to determine the highest priority ready process,
;        and then to give it the processor resource.  The procedure
;        takes all the processes linked to the dispatcher
;        ready list and inserts them into the ready list before
;        executing the highest priority ready process.

;  Entry Conditions:
;        None

;  Exit Conditions:
;        None
;*/

	dseg
svhl:	ds	2	; saved HL
svde:	ds	2	; saved DE
svret:	ds	2	; saved return
svsp:	ds	2	; saved SP

dspstk:
	dw	0c7c7h,0c7c7h,0c7c7h,0c7c7h,0c7c7h,0c7c7h
	dw	0c7c7h,0c7c7h,0c7c7h,0c7c7h,0c7c7h,0c7c7h

	cseg
;  dispatch:
pdisp:
	public	pdisp
;    Note:  This entry point should only be used from an
;           interrupt handler, i.e. interrupts disabled.
;    rlrpd.status = dispatch
	di
	shld	svhl
	lhld	rlr
	inx	h
	inx	h
	mvi	m,9
	lhld	svhl

dispatch:
;    procedure public;
	public	dispatch

;      enter$region;
	DI
;
; save registers in the Process Desciptor
;
	shld	svhl
	pop	h
	shld	svret
	push	psw
	lxi	h,0
	dad	sp
	shld	svsp
	xchg
	shld	svde
	lhld	rlr
	lxi	d,50
	dad	d
	xchg
	pop	psw
	lhld	svret
	push	h
	xchg
	sphl
	lhld	svde
	xchg
	lhld	svhl

	PUSH	PSW
	PUSH	B
	PUSH	D
	PUSH	H
;        call svz80;

	public	dspsvz80
dspsvz80:
	jmp	noz80save
	db	0ddh,0e5h	;Push IX
	db	0fdh,0e5h	;Push IY
	db	008h		;Ex AF,AF'
	db	0d9h		;Exx
	push	psw
	push	b
	push	d
	push	h
noz80save:

	mvi	a,0ffh
	sta	indisp		;set in dipatcher flag

;        pdadr = stackptr;
	lhld	svsp
	XCHG
;        rlrpd.stkptr = pdadr;
	LHLD	RLR
;        pdadr = rlr;
	SHLD	PDADR
	MOV	B,H
	MOV	C,L
	inx	h
	inx	h
	inx	h
	inx	h
	MOV	M,E
	INX	H
	MOV	M,D
;
	LXI	SP,DSPSTK+24
;        if rlrpd.status <> rtr$status then rlr = rlrpd.pl;
	dcx	h
	dcx	h
	dcx	h
	MOV	A,M
	ORA	A
	JZ	@7
	DCX	H
	DCX	H
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	SHLD	RLR
;        do case pd.status;
	MOV	E,A
	MVI	D,0
	LXI	H,@29
	DAD	D
	DAD	D
	MOV	E,M
	INX	H
	MOV	D,M
	LHLD	DSPTCHPARAM
	XCHG
	PCHL

;          /* status = 0, Ready to Run */
;           do;
;          end;

;          /* status = 1, DQ */
;          do;
@9:
;            call insert$process (dsptch$param,pdadr);
;          end;

;          /* status = 2, NQ */
;          do;
@10:
;            call insert$process (dsptch$param,pdadr);
	MOV	H,B
	MOV	L,C
	MOV	B,D
	MOV	C,E
	XCHG
;          end;
	JMP	@6

;          /* status = 3, Poll */
;          do;
@11:
;            pd.b = dsptch$param;
	LXI	H,10H
	DAD	B
	MOV	M,E
	INX	H
	MOV	M,D
;            pd.pl = plr;
	LHLD	PLR
	XCHG
	LHLD	PDADR
	MOV	M,E
	INX	H
	MOV	M,D
;            plr = pdadr;
	DCX	H
	SHLD	PLR
;          end;
	JMP	@7

;          /* status = 4, Flag Wait */
;          do;
@12:
;            sys$flag(dsptch$param) = pdadr;
	LXI	H,SYSFLAG
	DAD	D
	DAD	D
	MOV	M,C
	INX	H
	MOV	M,B
;            pd.pl = 0;
	XRA	A
	STAX	B
	INX	B
	STAX	B
;          end;
	JMP	@7

;          /* status = 5, Delay */
;          do;
@13:
;            call delay$list$insert;
	CALL	DELAYLISTINSERT
;          end;
	JMP	@7

;          /* status = 6, Swap */
;          do;
@14:
;            call insert$process (.slr,pdadr);
	MOV	D,B
	MOV	E,C
	LXI	B,SLR
;          end;
	JMP	@6

;          /* status = 7, Terminate */
;          do;
@15:
;             if dsptch$param = 0ffh or /* sys pr. term */
;               user$process (pdadr) then
	PUSH	D
	CALL	USERPROCESS
	POP	D
	RAR
	JC	@15A
	INR	E
	JZ	@15C
	JMP	@16
;            do;
@15A:
;              call mem$fr (.memsegtbl(pd.memseg));
;	free memory segment only if BDOS system reset call
	LHLD	DSPTCHPARAM
	INR	H
	JZ	@15C
	LXI	B,0FH
	LHLD	PDADR
	DAD	B
	MOV	A,M
	LXI	H,MEMSEGTBL
	ADD	A
	ADD	A
	MOV	E,A
	MVI	D,0
	DAD	D
	MOV	B,H
	MOV	C,L
	CALL	MEMFR
@15c:
;              call rlsmx (pdadr);
	lhld	pdadr
	mov	b,h
	mov	c,l
	extrn	rlsmx
	call	rlsmx	; release all owned MX queues
;              call detach (pdadr);
	LHLD	PDADR
	MOV	B,H
	MOV	C,L
	CALL	DETACH
;              next$thread$ptr = .thread$root;
	LHLD	PDADR
	MOV	B,H
	MOV	C,L
	LXI	H,thrdrt
@30:
	; BC = PDADR, HL = NEXTTHREADPTR
;              do while (pdladr := next$thread) <> pdadr;
	MOV	E,M
	INX	H
	MOV	D,M
	; DE = PDLADR, NEXTTHREAD
	MOV	A,E
	CMP	C
	JNZ	@30A
	MOV	A,D
	CMP	B
	JZ	@31
@30A:
;                next$thread$ptr = .pdl.thread;
	LXI	H,12H
	DAD	D
;              end;
	JMP	@30
@31:
;              next$thread = pdl.thread;
	DCX	H
	XCHG
	LXI	B,12H
	DAD	B
	MOV	A,M
	STAX	D
	INX	D
	INX	H
	MOV	A,M
	STAX	D
;            end;

	JMP	@7
@16:
;            else
;            do;
;              pd.status = rtr$status;
	LHLD	PDADR
	INX	H
	INX	H
	MVI	M,0H
;              pd.pl = drl;
;              drl = pdadr;
;            end;
;          end;
	JMP	@19

;          /* status = 8, Set Priority */
;          do;
@18:
;            pd.priority = dsptch$param;
	INX	B
	INX	B
	INX	B
	MOV	A,L
	STAX	B
;            pd.pl = drl;
;            drl = pdadr;
;          end;
;	JMP	@19

;          /* status = 9, Dispatch */
;          do;
@19:
;            pd.pl = drl;
	LHLD	DRL
	XCHG
	LHLD	PDADR
	MOV	M,E
	INX	H
	MOV	M,D
;            drl = pdadr;
	DCX	H
	SHLD	DRL
;          end;
	JMP	@7

;          /* status = 10, Attach */
;          do;
;@20:  (see end of set console)
;            call attach (pdadr);
;          end;

;          /* status = 11, Detach */
;          do;
@21:
;            call detach (pdadr);
	CALL	DETACH
;            pd.pl = drl;
;            drl = pdadr;
;          end;
	JMP	@19

;          /* status = 12, Set Console */
;          do;
@22:
;            call detach (pdadr);
	CALL	DETACH
;            if dsptch$param < nmb$consoles then
	CALL	MAXCNS
	MOV	B,A
	LDA	DSPTCHPARAM
	CMP	B
	JNC	@7
;            do;
;              pd.console = dsptch$param;
	LXI	B,0EH
	LHLD	PDADR
	DAD	B
	MOV	M,A
;              call attach (pdadr);
@20:
	LHLD	PDADR
	MOV	B,H
	MOV	C,L
	CALL	ATTACH
;            end;
;          end;
	JMP	@7

;        end; /* case */
@29:
	DW	@7
	DW	@9
	DW	@10
	DW	@11
	DW	@12
	DW	@13
	DW	@14
	DW	@15
	DW	@18
	DW	@19
	DW	@20
	DW	@21
	DW	@22
@6:
	CALL	INSERTPROCESS
@7:

;        /* Put ready processes on ready list and poll
;           devices required -> at least once then until
;           there is a process ready to run              */
;        do while first$time or (rlr = 0);
@32:
;          /* insert all processes on ready list from
;             the dispatcher ready list               */
;          do while drl <> 0;
@34:
	LHLD	DRL
@34A:
	MOV	A,H
	ORA	L
	JZ	@35
;            pdadr = drlpd.pl;
	MOV	E,M
	INX	H
	MOV	D,M
	PUSH	D
;            drlpd.status = rtr$status;
	INX	H
	MVI	M,0H
;            call insert$process (.rlr,drl);
	DCX	H
	DCX	H
	XCHG
	LXI	B,RLR
	CALL	INSERTPROCESS
;            drl = pdadr;
	POP	H
	SHLD	DRL
;          end;
	JMP	@34A
@35:

;          /* poll all required devices and place any
;             readied processes on the ready list      */
;          pdadr = .plr;
	LXI	D,PLR
;          pollpointer = plr;
	LHLD	PLR
;          do while pollpointer <> 0;
@36:	; BC = oldpollptr, DE = pdadr, HL = pollpointer
	MOV	A,H
	ORA	L
	JZ	@37
;            oldpollptr = pdadr;
	MOV	B,D
	MOV	C,E
;            pdadr = pollpointer;
	MOV	D,H
	MOV	E,L
;            pollpointer = pd.pl;
	MOV	A,M
	INX	H
	MOV	H,M
	MOV	L,A
;            if xios$pl (pd.b) then
	PUSH	B
	PUSH	D
	PUSH	H
	LXI	H,10H
	DAD	D
	MOV	C,M
	CALL	XIOSPL
	POP	H
	POP	D
	POP	B
	RAR
	JNC	@36
;            do;
;              oldpoll = pollpointer;
	MOV	A,L
	STAX	B
	INX	B
	MOV	A,H
	STAX	B
	DCX	B
	PUSH	B
	PUSH	H
;              pd.status = rtr$status;
	MOV	H,D
	MOV	L,E
	INX	H
	INX	H
	MVI	M,0
;              call insert$process (.rlr,pdadr);
	LXI	B,RLR
	CALL	INSERTPROCESS
	POP	H
	POP	B
;              pdadr = oldpollptr;
	MOV	D,B
	MOV	E,C
;            end;
;          end;
	JMP	@36
@37:
;          first$time = false;
;        end;

;        call xios$ms (.memsegtbl(rlrpd.memseg));
	lhld	rlr
	LXI	B,0FH
	DAD	B
	MOV	A,M
	add	a
	jm	@38	;system process, no xiosms
	add	a
	LXI	H,MEMSEGTBL
	mov	e,a
	mvi	d,0
	dad	d
	MOV	B,H
	MOV	C,L
	CALL	XIOSMS

;        stackptr = rlrpd.stkptr;
@38:
	LHLD	RLR
	LXI	B,4H
	DAD	B
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	shld	svsp

	xra	a
	sta	indisp	;clear in dispatcher flag

	lhld	rlr
	lxi	d,50-8
	dad	d
	sphl

	public	dsprsz80
dsprsz80:
	jmp	noz80restore

	lxi	d,-12
	dad	d
	sphl

	pop	h
	pop	d
	pop	b
	pop	psw
	db	0d9h		;Exx
	db	008h		;Ex AF,AF'
	db	0fdh,0e1h	;Pop IY
	db	0ddh,0e1h	;Pop IX
noz80restore:

	POP	H
	POP	D
	POP	B
	POP	PSW
;      exit$region;

	shld	svhl
	lhld	svsp
	sphl
	lhld	svhl

	EI
	RET
;    end dispatch;

;end dsptch;

	end
