	page	0
	title	'Type File on Console'
;	File TYPE program, reads an input file and prints
;	  it on the console
;
;	Copyright (C) 1979, 1980
;	Digital Research
;	P.O. Box 579
;	Pacific Grove, CA  93950
;
	ORG	0000H		; standard rsp start

CTLZ	EQU	1AH		; control-Z used for EOF

CONOUT	EQU	2		; bdos conout function #
PRINTF	EQU	9		;  ""  print buffer
READF	EQU	20		; read next record
OPENF	EQU	15		; open fcb
PARSEFN	EQU	152		; parse file name
MKQUE	EQU	134		; make queue
RDQUE	EQU	137		; read queue
STPRIOR	EQU	145		; set priority
DETACH	EQU	147		; detach console

;
; BDOS entry point address
bdosadr:
	dw	$-$		; ldr will fill this in
;
; Type process descriptor
;
typepd:
	dw	0		; link
	db	0		; status
	db	10		; priority (initial)
	dw	stack+38	; stack pointer
	db	'TYPE    '	; name in upper case
pdconsole:
	ds	1		; console
	db	0ffh		; memseg
	ds	2		; b
	ds	2		; thread
	dw	buff		; disk set DMA address
	ds	1		; user code & disk select
	ds	2		; dcnt
	ds	1		; searchl
	ds	2		; searcha
	ds	2		; active drives
	ds	20		; register save area
	ds	2		; scratch

;
; Type linked queue control block
;
typelqcb:
	dw	0		; link
	db	'TYPE    '	; name in upper case
	dw	72		; msglen
	dw	1		; nmbmsgs
	ds	2		; dqph
	ds	2		; nqph
	ds	2		; mh
	ds	2		; mt
	ds	2		; bh
	ds	74		; buf (72 + 2 byte link)

;
; Type user queue control block
;
typeuserqcb:
	dw	typelqcb	; pointer
	dw	field		; msgadr

;
; Field for message read from Type linked qcb
;
field:
	ds	1		; disk select
console:
	ds	1		; console
filename:
	ds	72		; message body

;
; Parse file name control block
;
pcb:
	dw	filename	; file name address
	dw	fcb		; file control block address

;
; Type Stack & other local data structures
;
stack:
	ds	38		; 20 level stack
	dw	type		; process entry point

fcb:	ds	36		; file control block

buff:	ds	128		; file buffer

;
; BDOS call procedure
;
bdos:
	lhld	bdosadr		; HL = BDOS address
	pchl

;
; Type main program
;
type:
	mvi	c,mkque
	lxi	d,typelqcb
	call	bdos		; make typelqcb
	mvi	c,stprior
	lxi	d,200
	call	bdos		; set priority to 200

forever:
	mvi	c,rdque
	lxi	d,typeuserqcb
	call	bdos		; read from type queue
	mvi	c,parsefn
	lxi	d,pcb
	call	bdos		; parse the file name
	inx	h
	mov	a,h
	ora	l		; test for 0FFFFH
	jz	error
	lda	console
	sta	pdconsole	; typepd.console = console

	MVI	C,OPENF
	LXI	D,FCB
	CALL	BDOS		; open file
	INR	A		; test return code
	JZ	ERROR		; if it was 0ffh, no file
	XRA	A		; else,
	STA	FCB+32		;   set next record to zero
NEW$SECTOR:
	MVI	C,READF
	LXI	D,FCB
	CALL	BDOS		; read next record
	ORA	A
	JNZ	DONE		; exit if eof or error

	LXI	H,BUFF		; point to data sector
	MVI	C,128		; get byte count
NEXT$BYTE:
	MOV	A,M		; get the byte
	MOV	E,A		; save in E
	CPI	CTLZ
	JZ	DONE		; exit if eof
	PUSH	B		; save byte counter
	PUSH	H		; save address register
	MVI	C,CONOUT
	CALL	BDOS		; write console
	POP	H		; restore pointer
	POP	B		; and counter
	INX	H		; bump pointer
	DCR	C		; dcr byte counter
	JNZ	NEXT$BYTE	; more in this sector
	JMP	NEW$SECTOR	; else, we need a new one

ERROR:
	LXI	D,ERR$MSG	; point to error message
	MVI	C,PRINTF	; get function code to print
	CALL	BDOS

done:
	mvi	c,detach
	call	bdos		; detach the console
	jmp	forever

ERR$MSG:
	DB	0DH,0AH,'File Not Found or Bad File Name$'

	END

