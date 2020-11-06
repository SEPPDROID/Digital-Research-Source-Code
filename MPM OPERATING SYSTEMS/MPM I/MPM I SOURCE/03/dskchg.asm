$title	('MP/M 1.0 Disk Change Procedure')
	name	dskchg
	cseg
;diskchange:
;do;

;/*
;  Copyright (C) 1979
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;*/

;  declare rlrpd based rlr process$descriptor;
	extrn	rlr

;  declare nmb$cns byte external;
	extrn	nmbcns

;  declare cns$att (1) address external;
	extrn	cnsatt

;  declare cns$que (1) address external;
	extrn	cnsque

;  declare plr address external;
	extrn	plr

;  declare drl address external;
	extrn	drl

;  rawlst:
;    procedure (string$adr) external;
	extrn	rawlst
;      declare string$adr address;
;    end rawlst;

;  rawci:
;    procedure byte external;
	extrn	rawci
;    end rawci;

	dseg
;  declare nomatch boolean;
;  declare plrcontadr address;
plrcontadr:
	ds	2
;  declare plrcont based plrcontadr address;

cnsai:	ds	2
	cseg

cnfrmsg:
	db	0dh,0ah
	db	'Confirm disk system reset (Y/N)?'
	db	'$'

denied:
	db	'Denied.'
	db	0dh,0ah
	db	'$'

rspmsg:
	db	$-$
	db	0dh,0ah
	db	'$'

;/*
;  dskchg:
;*/

;  dskchg:
dskchg:
;    procedure boolean public;
	public	dskchg
	dseg
;      declare pdadr address;
pdadr:	ds	2
;      declare pd based pdadr process$descriptor;
;      declare (cur$cns,respns,i) byte;
curcns:	ds	1
respns:	ds	1
i:	ds	1
;      declare xplradr address;
xplradr:
	ds	2
;      declare xplr based xplradr process$dercriptor;
;      declare next$console$ptr address;
nextconsoleptr:
	ds	2
;      declare next$console based next$console$ptr address;
;      declare pdladr address;
pdladr:	ds	2
;      declare pdl based pdladr process$descriptor;
	cseg

;      if nmb$cns < 2
	LDA	NMBCNS
	CPI	2H
	MVI	A,0FFH
	RC
;        then return true;
;      cur$cns = rlrpd.console;
	LXI	B,0EH
	LHLD	RLR
	DAD	B
	MOV	A,M
	STA	CURCNS
;      do i = 0 to nmb$cns-1;
	LXI	H,I
	MVI	M,0FFH
@10:
	LDA	NMBCNS
	DCR	A
	LXI	H,I
	INR	M
	CMP	M
	MVI	A,0FFH
	RC
;        pdadr = console$attached(i);
;        /* pull process off poll list */
	MOV	L,M
	MVI	H,0
	LXI	B,CNSATT
	DAD	H
	DAD	B
	SHLD	CNSAI
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	SHLD	PDADR
;        xplradr = 0;
	LXI	H,0H
	SHLD	XPLRADR
;        plrcontadr = .plr;
	LXI	H,PLR
	SHLD	PLRCONTADR
;        nomatch = true;
;        do while nomatch and (plrcont <> 0);
@12:
	MOV	A,M
	MOV	E,A
	INX	H
	MOV	D,M
	ORA	D
	JZ	@13
;          if plrcont = pdadr then
	LXI	B,PDADR
	LDAX	B
	CMP	E
	JNZ	@2
	INX	B
	LDAX	B
	CMP	D
	JNZ	@2
;          do;
;            nomatch = false;
;            xplradr = plrcont;
	XCHG
	SHLD	XPLRADR
;            plrcont = xplr.pl;
	MOV	C,M
	INX	H
	MOV	B,M
	XCHG
	MOV	M,B
	DCX	H
	MOV	M,C
;          end;
	JMP	@13
@2:
;          else
;          do;
;            plrcontadr = plrcont;
	XCHG
	SHLD	PLRCONTADR
;          end;
@3:
;        end;
	JMP	@12
@13:
;        console$attached(i) = rlr;
	LHLD	RLR
	XCHG
	LHLD	CNSAI
	MOV	M,E
	INX	H
	MOV	M,D
;        rlrpd.console = i;
	LXI	B,0EH
	XCHG
	DAD	B
	LDA	I
	MOV	M,A
;        call rawlst (.(0dh,0ah,
	LXI	B,cnfrmsg
	CALL	RAWLST
;          'Confirm reset disk system (Y/N)?','$'));
;        respns = rawci;
	CALL	RAWCI
	STA	RESPNS
;        call crlf;
	LXI	B,RSPMSG
	STAX	B
	CALL	RAWLST
;        rlrpd.console = cur$cns;
;        /* assign console back to pre-disk reset state */
	LXI	B,0EH
	LHLD	RLR
	DAD	B
	LDA	CURCNS
	MOV	M,A
;        console$attached(i) = pdadr;
	LHLD	PDADR
	XCHG
	LHLD	CNSAI
	MOV	M,E
	INX	H
	MOV     M,D
;        nomatch = true;
;        next$console$ptr = .console$queue(i);
	LHLD	I
	MVI     H,0
	LXI	B,CNSQUE
	DAD	H
	DAD	B
	SHLD	NEXTCONSOLEPTR
;        do while nomatch;
@14:
;          if (pdladr := next$console) = 0 then
	MOV	E,M
	INX	H
	MOV	D,M
	XCHG
	SHLD	PDLADR
	MOV	A,H
	ORA	L
	JZ	@15
;          do;
;            nomatch = false;
;          end;
;          else
;          do;
;            if pdladr = pdadr then
	XCHG
	LHLD	PDADR
	MOV	A,L
	CMP	E
	JNZ	@6
	MOV	A,H
	CMP	D
	JNZ	@6
;            do;
;              next$console = pdl.pl;
	XCHG
;              pd.pl = drl;
	LHLD	DRL
	XCHG
	LHLD	PDADR
	MOV     M,E
	INX	H
	MOV	M,D
;              drl = pdadr;
	DCX	H
	SHLD	DRL
;              nomatch = false;
;            end;
	JMP	@15
@6:
;            else
;            do;
;              next$console$ptr = next$console;
	LHLD	NEXTCONSOLEPTR
	MOV	E,M
	INX     H
	MOV	D,M
	XCHG
	SHLD	NEXTCONSOLEPTR
;            end;
@7:
;          end;
@5:
;        end;
	JMP	@14
@15:
;        /* put process back on the poll list */
;        if xplradr <> 0 then
	LHLD	XPLRADR
	MOV	A,H
	ORA	L
	JZ	@8
;        do;
;          xplr.pl = plr;
	XCHG
	LHLD	PLR
	XCHG
	MOV	M,E
	INX	H
	MOV	M,D
;          plr = xplradr;
	DCX	H
	SHLD	PLR
;        end;
@8:
;        if not ((respns = 'y') or (respns = 'Y')) then
	LDA	RESPNS
	ANI	5FH
	CPI	59H
	JZ	@10
;        do;
;          call rawlst (.(
	LXI	B,denied
	CALL	RAWLST	; 'Disk reset denied.',0dh,0ah,'$'));
;          return false;
	XRA	A
	RET
;         end;
;      end;
;      return true;
;    end dskchg;
;end diskchange;
	END
