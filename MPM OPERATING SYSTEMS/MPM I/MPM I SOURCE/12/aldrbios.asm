	TITLE	'MP/M LOADER, COPYRIGHT 1979, ALTOS COMPUTER SYSTEMS'
;-----------------------------------------------------------------------
;
;	ALTOS COMPUTER SYSTEMS
;	2338A WALSH AVENUE
;	SANTA CLARA, CA 95050
;
;	(408) 244-5766
;
;	COPYRIGHT 1979, ALTOS COMPUTER SYSTEMS
;
;	THIS PROGRAM IS INTENDED TO PERFORM THE LOADING OF THE
;	CCP AND BDOS INTO MEMORY DURING THE FIRST COLD START
;	OF THE ALTOS SYSTEM.
;	ONCE THE SYSTEM IS LOADED IT WILL LOAD THE FULL CBIOS
;	ROUTINES WHICH WILL CONTROL THE LOADING AND OPERATION
;	OF THE SYSTEM FROM THAT POINT ONWARD.
;	IF THE INITIAL LOAD OF THE SYSTEM IS UNABLE TO FIND THE
;	FULL CBIOS ON THE CURRENT DISK IT WILL CONTINUE TO 
;	OPERATE BUT IN A DEGRADED MODE.
;
;	VERSION: 2.00
;	DATE:    DECEMBER 5, 1979
;	.        INITIAL VERSION OF PROGRAM.
;
;-----------------------------------------------------------------------

	MACLIB	DISKDEF
	MACLIB	Z80S

VERSION	EQU	20		;CP/M VERSION NUMBER
ALTOSV	EQU	0		;ALTOS VERSION NUMBER
TRUE	EQU	0FFFFH		;
FALSE	EQU	00000H		;

;-----------------------------------------------------------------------
;
;	THE FOLLOWING FIELDS ARE USER MODIFIABLE
;
;-----------------------------------------------------------------------

MSIZE	EQU	32		;
DMA	EQU	TRUE		;
HARDSK	EQU	TRUE		;

;-----------------------------------------------------------------------
;
;	THE FOLLOWING CONSTANTS APPLY TO THE RELOCATION OF BIOS
;
;-----------------------------------------------------------------------

RELOC	EQU	TRUE		;

RAM$TOP	EQU	1D00H		;TOP ADDRESS+1
BIOS	EQU	RAM$TOP-0600H	;BASIC INPUT/OUTPUT
BDOS	EQU	BIOS-0E00H	;BASE OF THE BDOS

	ORG	BIOS

BUFF	EQU	080H		;DEFAULT BUFFER

RETRY	EQU	10		;MAX RETRIES ON DISK

;
;	JUMP VECTOR FOR INDIVIDUAL SUBROUTINES
;
;-----------------------------------------------------------------------

	jmp	boot		;cold start
wboote:	jmp	wboot		;warm start
	jmp	const		;console status
	jmp	conin		;console character in
	jmp	conout		;console character out
	jmp	list		;list character out
	jmp	punch		;punch character out
	jmp	reader		;reader character out
	jmp	home		;move head to home position
	jmp	seldsk		;select disk
	jmp	settrk		;set track number
	jmp	setsec		;set sector number
	jmp	setdma		;set dma address
	jmp	read		;read disk
	jmp	write		;write disk
	jmp	listst		;return list status
	jmp	sectran		;sector translate

	PAGE
;-----------------------------------------------------------------------
;
;	FIXED TABLES FOR TWO DRIVE MINIMAL STANDARD SYSTEM
;
;	THE LOCATION OF THE FOLLOWING TABLES MUST NOT BE
;	CHANGED AS EXTERNAL PROGRAMS RELY ON THEIR
;	POSITION IN THIS PROGRAM.
;
;-----------------------------------------------------------------------

TRKM:	DB	000H,000H,000H,000H	;TRACK REGISTERS
SELM:	DB	004H,008H,010H,020H	;DENSITY MASKS UPDATED BY SETUP
MODM:	DB	000H,000H,000H,000H	;MODE MASKS UPDATED BY SETUP

PTR1:	DB	047H,00DH	;9600 BAUD SETUP FOR PRINTER 1
PTR2:	DB	047H,00DH	;9600 BAUD SETUP FOR AUXILLIARY PORT

DPBASE:				;THESE BLOCKS ARE UPDATED BY SETUP
;	DISK PARAMETER HEADER FOR DISK 00 (A:)
	DW	XLT00,0000H
	DW	0000H,0000H
	DW	DIRBF,SDBLK
	DW	CHK00,ALL00

XLT00:	db	01,07,13,19,25,05,11,17,23,03,09,15,21
	db	02,08,14,20,26,06,12,18,24,04,10,16,22

XLT01:	DB	01,02,03,04,05,06,07,08,09,10,11,12
	DB	13,14,15,16,17,18,19,20,21,22,23,24
	DB	25,26,27,28,29,30,31,32,33,34,35,36
	DB	37,38,39,40,41,42,43,44,45,46,47,48

;-----------------------------------------------------------------------
;
;	DISK PARAMETER BLOCKS
;
;-----------------------------------------------------------------------

SDBLK:	;DISK PARAMETER BLOCK FOR SINGLE DENSITY
	dw	26		;sectors per track
	db	3		;block shift factor
	db	7		;block mask
	db	0		;null mask
	dw	242		;disk size-1
	dw	63		;directory max
	db	192		;alloc 0
	db	0		;alloc 1
	dw	16		;check size
	dw	2		;track offset

DDBLK:	DW	48		;SECTORS PER TRACK
	DB	4		;BLOCK SHIFT FACTOR
	DB	15		;BLOCK MASK
	DB	0		;EXTENT MASK
	DW	224		;DISK SIZE-1
	DW	95		;DIRECTORY MAX
	DB	192		;ALLOC 0
	DB	0		;ALLOC 1
	DW	24		;CHECK SIZE
	DW	2		;TRACK OFFSET
MPARMS:	DW	0		;MISC. PARAMETERS
;
;	0100H = DOUBLE HEADED FLOPPY DRIVES
;	0200H = CENTRONICS PRINTER IS LIST DEVICE
;

;-----------------------------------------------------------------------
;
;	END OF FIXED LOCATION DEPENDENT TABLES
;-----------------------------------------------------------------------

	PAGE
;	COLD BOOT ROUTINE
;
BOOT:	;simplest case is to just perform parameter initialization
;	INTERRUPT VECTOR AND ROUTINE
WBOOT:	;SIMPLEST CASE IS TO DO A COLD BOOT

gocpm:
	DI
	MVI	A,INTERUPT SHR 8
	STAI
	MVI	A,INTERUPT AND 0FFH
	OUT	00AH
	EI
	RET
;

CONST:	;console status, return 0ffh if character ready, 00h if not
	XRA	A		;CHECK STATUS
	OUT	01DH		;
	IN	01DH		;
	ANI	00000001B	;CHARACTER WAITING ??
	RZ			;NO, RETURN
	MVI	A,0FFH		;RETURN ALL FOXES
	ret

CONIN:	;console character into register a
	CALL	CONST		;CHARACTER WAITING ??
	JZ	CONIN		;NOT YET....
	IN	01CH		;READ CHARACTER IN
	ANI	07FH		;MAKE SURE ITS ASCII
	ret

CONOUT: ;console character output from register c
	MVI	A,010H		;RESET INT BIT
	OUT	01DH		;
	IN	01DH		;GET STATUS
	ANI	00001100B	;TXE AND DTR
	CPI	00001100B	;READY ??
	JNZ	CONOUT		;NO, WAIT HERE
	MOV	A,C		;Get to accumulator
	OUT	01CH		;WRITE IT ONTO CONSOLE
	ret

LIST:	;list character from register c
	MVI	A,010H		;RESET INT BIT
	OUT	01FH		;
	IN	01FH		;GET STATUS
	ANI	00001100B	;TXE AND DTR
	CPI	00001100B	;READY ??
	JNZ	LIST		;NO, WAIT HERE
	MOV	A,C		;Get to accumulator
	OUT	01EH		;WRITE IT ONTO PRINTER
	ret			;RETURN TO CALLER

LISTST:	;return list status (0 if not ready, 1 if ready)
	xra	a	;0 is always ok to return
	ret

PUNCH:	;punch character from register c
	ret		;null subroutine

READER: ;read character into register a from reader device
	mvi	a,1ah	;enter end of file for now (replace later)
	ret

	PAGE
;-----------------------------------------------------------------------
;
;	THESE ARE THE DISK I/O ROUTINES.
;	THESE ROUTINES WILL BE USED BY THE BDOS DURING
;	THE LOAD OF THE FULL CBIOS. FOLLOWING THE SUCCESSFUL
;	LOAD THEY WILL NO LONGER BE USED.
;
;-----------------------------------------------------------------------

HOME:	;move to the track 00 position of current drive
	CALL	POINT		;POINT TO TRACK REGISTER
	MVI	M,000H		;SET TO TRACK ZERO
	MVI	A,00AH		;HOME COMMAND
	OUT	004H		;DO IT
WT1:	JR	WT1		;WAIT HERE UNTIL FINISHED
	ret			;RETURN TO CALLER

SELDSK:	;select disk given by register C
	CALL	WBOOT
	lxi	h,0000h		;error return code
	mov	a,c		;
	ORA	A		;MUST BE ONLY 0
	RNZ			;INVALID FOR LOADER.

	STA	DISKNO		;UPDATE DISK NUMBER
	CALL	POINT		;
	MOV	A,M		;LOAD LAST TRACK NUMBER
	OUT	005H		;RESETUP 1791
	LXI	D,004H		;POINT TO DENSITY REGISTER
	DAD	D		;
	MOV	A,M		;PICKUP MASK
	OUT	008H		;WITHE IT OUT TO SELECT DRIVE

;	compute proper disk parameter header address

	LXI	H,DPBASE	;ONLY ONE SO ITS EASY....
	ret

SETTRK:	;set track given by register c
	CALL	POINT		;POINT TO LAST TRACK REGISTER
	mov	a,c		;
	CMP	M		;SAME TRACK AS LAST TIME ??
	RZ			;YES, NO NEED TO SEEK
	MOV	M,A		;
	OUT	007H		;
	MVI	A,01AH		;SEEK TO CORRECT TRACK
	OUT	004H		;DO IT....
STWT:	JR	STWT		;WAIT FOR SEEK TO COMPLETE
	ret

SETSEC:	;set sector given by register c
	mov	a,c		;WRITE SECTOR OUT
	OUT	006H		;
	ret

SECTRAN:
	;translate the sector given by BC using the
	;translate table given by DE
	xchg			;HL=.trans
	dad	b		;HL=.trans(sector)
	mov	l,m		;L = trans(sector)
	mvi	h,0		;HL= trans(sector)
	ret			;with value in HL

SETDMA:	;set dma address given by registers b and c
	mov	l,c		;low order address
	mov	h,b		;high order address
	shld	dmaad		;save the address
	ret

READ:	;perform read operation (usually this is similar to write
;	so we will allow space to set up read command, then use
;	common code in write)
	
	MVI	A,08CH		;READ COMMAND CODE
	MVI	B,001H		;DMA READ CODE
	MVI	C,09FH		;READ STATUS MASK
	jmp	waitio		;to perform the actual i/o

	ORG	(($+0A2H) AND 0FF00H)+5EH

INTERUPT:
	DW	IRTN

IRTN:
	POP	H		;PICKUP PC
	INX	H		;BUMP PAST JR $
	INX	H		;
	PUSH	H		;PUT IT BACK FOR RETI
	EI			;
	RETI			;

WRITE:	;perform a write operation
	MVI	A,0ACH		;WRITE COMMAND CODE
	MVI	B,005H		;DMA WRITE CODE
	MVI	C,0FFH		;WRITE STATUS MASK

waitio:	;enter here from read and write to perform the actual i/o 
;	operation.  return a 00h in register a if the operation completes
;	properly, and 01h if an error occurs during the read or write

	PUSH	PSW		;SAVE I/O COMMAND CODE
	IN	008H		;IS HEAD LOADED ??
	ANI	00000010B	;CHECK
	MVI	D,0FBH		;ASSUME YES
	JNZ	$+3+2		;VERIFY ASSUMPTION
	MVI	D,0FFH		;ITS NOT, LEAVE HEAD LOAD BIT IN COMMAND
	POP	PSW		;RESTORE COMMAND
	ANA	D		;RESET HEAD LOAD BASED ON PREVIOUS TEST

	IF	DMA		;GENERATE DMA SUPPORT
	PUSH	PSW		;SAVE COMMAND
	PUSH	B		;SAVE REST
	MOV	A,B		;
	STA	DMALST3		;SAVE READ/WRITE CODE BYTE
	LXI	B,01100H	;17 BYTES OF DATA AND PORT ZERO
	LXI	H,DMALIST	;
	OUTIR			;WRITE IT OUT
	POP	B		;
	POP	PSW		;
	ENDIF			;

	PUSH	B		;SAVE STATUS MASK

	IF	NOT DMA		;NON DMA SOFTWARE
	PUSH	PSW		;SAVE PSW
	MOV	A,B		;GET DMA I/O COMMAND (001 OR 005)
	ANI	00000100B	;REMOVE ALL BUT TEST BIT
	RRC			;
	RRC			;
	LHLD	LOWSTR		;LOW MEMORY INI/RETN
	ORA	H		;MAKE INTO AN OTI IF NECESSARY
	MOV	H,A		;
	SHLD	066H		;SAVE AT NMI LOCATION
	LHLD	LOWSTR+2	;
	SHLD	066H+2		;
	POP	PSW		;RESTORE PSW
	LHLD	DMAAD		;POINT TO RECORD AREA
	MVI	C,007H		;PORT ADDRESS FOR I/O
	ENDIF			;END NON DMA SOFTWARE

	OUT	004H		;PERFORM COMMAND
RDLP:	JR	RDLP		;WAIT HERE

	IN	004H		;READ STATUS
	POP	B		;
	ANA	C		;IS STATUS OK ??
	RZ			;YES, RETURN OK
	
	mvi	a,1		;error condition
	ret			;RETURN IN ERROR

POINT:
	LHLD	DISKNO		;CURRENT DRIVE
	MVI	H,000H		;
	LXI	D,TRKM		;
	DAD	D		;
	RET			;POINT TO CURRENT TRACK REGISTER

MSPRT:
	MOV	A,M		;
	ORA	A		;END OF MESSAGE ??
	RZ			;YES
	INX	H		;
	MOV	C,A		;
	CALL	CONOUT		;
	JMP	MSPRT		;

	PAGE
;-----------------------------------------------------------------------
;
;	CONSTANTS AND WORK AREAS
;
;-----------------------------------------------------------------------

DISKNO:	DS	1		;DISK NUMBER 0-15
TRACK:	DS	1		;TRACK NUMBER
SECTOR:	DS	1		;

	IF	DMA		;DMA SUPPORT SOFTWARE
DMALIST:DB	0C3H,07DH	;
DMAAD:	DS	2		;DIRECT MEMORY ADDRESS
	DW	128-1		;LENGTH FOR I/O
	DB	014H,028H,085H,007H,08AH,0CFH,001H,0CFH
DMALST3:DS	1		;001=READ, 005=WRITE
	DB	0CFH,087H	;
	ENDIF			;

	IF	NOT DMA		;NON DMA SUPPORT SOFTWARE
DMAAD:	DS	2		;DIRECT MEMORY ADDRESS
LOWSTR:	INI			;
	RETN			;
	ENDIF			;

MSG:	DB	00DH,00AH
	IF	RELOC
MSZ:	DB	'00'
	ENDIF
	IF	NOT RELOC
MSZ:	DB	MSIZE/10+'0',MSIZE MOD 10 + '0'
	ENDIF
	DB	00DH,00AH,000H

NOMSG:	DB	00DH,00AH
	DB	'CBIOS00.COM NOT FOUND ON DISK, CORRECT AND RETRY '
	DB	00DH,00AH,000H

LDRSIZ	EQU	$-BIOS		;LOADER SIZE IN BYTES
LDRPAG	EQU	(LDRSIZ+255)/256;LOADER SIZE IN PAGES

;-----------------------------------------------------------------------
;
;	THE remainder of the CBIOS is reserved uninitialized
;	data area, and does not need to be a part of the
;	system memory image (the space must be available,
;	however, between "begdat" and "enddat").
;
;-----------------------------------------------------------------------

begdat	equ	$	;beginning of data area
dirbf:	ds	128	;scratch directory area
all00:	ds	31	;allocation vector 0
chk00:	ds	16	;check vector 0

enddat	equ	$	;end of data area
datsiz	equ	$-begdat;size of data area
	end

