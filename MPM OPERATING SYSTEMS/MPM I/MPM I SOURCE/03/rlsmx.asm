$title	('MP/M 1.1 Release MX queues Procedure')
	name	rlsmx
	cseg
;release$MX:
;do;

;/*
;  Copyright (C) 1979, 1980
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950

;  Revised:
;    23 Jan 80  by Thomas Rolander
;*/

;  writeq:
	extrn	writeq
;    procedure (uqcbadr) byte external;
;      declare uqcbadr address;
;    end writeq;

;  declare qlr address external;
	extrn	qlr


;/*
;  rlsMX:

;        Note- this procedure must be called from within a
;              critical region.
;*/

;  rlsMX:
rlsmx:
	public	rlsmx
;    procedure (rlr$pdadr) public;
	dseg
;      declare rls$pdadr address;
;      declare ret byte;
;      decrare xqlradr address;
xqlradr:
	ds	2
;      declare xqlr based xqlradr address;
;      declare que  based xqlradr structure (
;        cqueue,pdadr address);
	cseg

;      xqlradr = qlr;
	LHLD	QLR
	XCHG			; DE = xqlradr
;      do while xqlradr <> 0;
@5:
	MOV	A,D
	ORA	E
	RZ
;        if que.name(0) = 'M' then
	LXI	H,2
	DAD	D
	MOV	A,M
	CPI	4DH
	JNZ	@1
;        do;
;          if que.name(1) = 'X' then
	INX	H
	MOV	A,M
	CPI	58H
	JNZ	@1
;          do;
;            if que.msgcnt = 0 then
	LXI	H,16H
	DAD	D
	MOV	A,M
	INX	H
	ORA	M
	JNZ	@1
;            do;
;              if que.pdadr = rls$pdadr then
	INX	H
	MOV	A,M
	CMP	C
	JNZ	@1
	INX	H
	MOV	A,M
	CMP	B
	JNZ	@1
;              do;
;                ret = writeq (.xqlradr);
	PUSH	B
	PUSH	D
	xchg
	shld	xqlradr
	LXI     B,XQLRADR
	CALL	WRITEQ
	POP	D
	POP	B
;              end;
;            end;
;          end;
;        end;
@1:
;        xqlradr = xqlr;
	XCHG
	MOV	E,M
	INX	H
	MOV	D,M
;      end;
	JMP	@5
;    end rlsMX;
;end release$MX;
	END
