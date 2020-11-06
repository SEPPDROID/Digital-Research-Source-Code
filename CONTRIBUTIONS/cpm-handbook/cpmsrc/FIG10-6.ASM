;	Figure 10-6
;
;	Testbed for Disk I/O drivers in the BIOS.
;
;	The complete source file consists of three components:
;
;		1. The testbed code shown here.
;		2. The Disk I/O drivers destined for the BIOS.
;		3. The debug package shown in Figure 10-2.
;
TRUE	EQU	0FFFFH
FALSE	EQU	NOT TRUE

DEBUG	EQU	TRUE		;For conditional assembly of RST
				;  instructions in place of IN and
				;  OUT instructions in the drivers.
	ORG	100H
START:
	LXI	SP,Test$Stack	;Use a local stack
	CALL	DB$Init		;Initialize the debug package
;
;	Make calls to SELDSK, SETTRK, SETSEC and SETDMA,
;	then either a Read or Write routine.
;
Testbed$Loop:
	LXI	SP,Test$Stack	;Use local stack

	LDA	Logical$Disk	;Setup for SELDSK call
	MOV	C,A
	CALL	SELDSK

	CALL	DB$Display	;Display return value in HL
	DB	DB$HL
	DB	'SELDSK returned',0

	SHLD	DPH$Start	;Setup to display Disk Parameter Header
	LXI	D,16		;Compute end address
	DAD	D
	SHLD	DPH$End		;Store into debug call

	CALL	DB$Display	;Display DPH
	DB	DB$M		;Memory
DPH$Start:
	DW	0
DPH$End:
	DW	0
	DB	'Selected DPH',0

	LHLD	Track		;Call SETTRK
	PUSH	H
	POP	B		;SETTRK needs track in BC
	CALL	SETTRK

	LDA	Sector		;Call SETSEC
	MOV	C,A		;SETSEC need sector in C
	CALL	SETSEC

	LXI	B,Test$Buffer	;Set DMA address
	CALL	SETDMA

	LDA	Write$Disk	;Check if reading or writing
	ORA	A
	JNZ	Test$Write

	CALL	Read$No$Deblock	;*** or Read$Physical depending on which
				;*** drivers you are testing.
	CALL	DB$Display	;Display return code
	DB	DB$A
	DB	'Test Read returned',0

	CALL	Check$Ripple	;Check if ripple pattern in buffer
	JZ	Testbed$Loop	;Yes, it is correct

	CALL	DB$MSGI		;Indicate problem
	DB	DB$HL		;Display HL (points to offending byte)
	DB	'Ripple pattern incorrect. HL -> failure.',0

	CALL	DB$Display	;Display test buffer
	CALL	DB$M		;Memory
	DW	Test$Buffer
	DW	Test$Buffer$Size
	DB	'Contents of Test$Buffer',0

	JMP	Testbed$Loop

Test$Write:
	CALL	Fill$Ripple	;Fill the test buffer with ripple pattern
	CALL	Write$No$Deblock;*** or Write$Physical depending on which
				;*** drivers you are testing.

	CALL	DB$Display	;Display return code
	DB	DB$A
	DB	'Test Write returned',0

	JMP	Testbed$Loop
	
Fill$Ripple:			;Fills the Test$Buffer with a pattern
				;  formed by putting into each byte, the
				;  least significant 8-bits of the byte's
				;  address.
	LXI	B,Test$Buffer$Size
	LXI	H,Test$Buffer
FR$Loop:
	MOV	M,L		;Set pattern value into buffer
	INX	H		;Update buffer pointer
	DCX	B		;Down date count
	MOV	A,C		;Check if count zero
	ORA	B
	JNZ	FR$Loop		;Repeat until zero
	RET
;
Check$Ripple:			;Check that the buffer is filled with the
				;  correct ripple pattern.
				;  Returns with Zero status if this is true,
				;  non-zero status if the ripple is not 
				;  correct. HL point to the offending byte
				;  (which should  = L)
	LXI	B,Test$Buffer$Size
	LXI	H,Test$Buffer
CR$Loop:
	MOV	A,L		;Get correct value
	CMP	M		;Compare to that in the buffer
	RNZ			;Mismatch - Non-zero already indicated
	INX	H		;Update buffer pointer
	DCX	B		;Down date count
	MOV	A,C		;Check count zero
	ORA	B
	JNZ	CR$Loop		;Repeat until zero
	RET			;Zero flag will already be set
;
;	Testbed variables
;
Logical$Disk:	DB	0	;A = 0, B = 1,...
Track:		DW	0	;Disk track number
Sector:		DB	0	;Disk sector number
Write$Disk:	DB	0	;NZ to write to disk
;
Test$Buffer$Size	EQU	512	;<=== Alter as required
Test$Buffer:	DS	Test$Buffer$Size
;
	DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
	DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
	DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
Test$Stack:
;
;	Dummy routines for those shown in other figures
;
;	BIOS routines (Figure 8-10)
;
SELDSK:			;Select logical disk
SETTRK:			;Set track number
SETSEC:			;Set sector number
SETDMA:			;Set DMA address
Read$No$Deblock:	;Driver read routines
Read$Physical:
Write$No$Deblock:	;Driver write routines
Write$Physical:

;
;	Debug routines (Figure 10-2)
;
DB$Init:		;Debug initialization
DB$MSGI:		;Display message in-line
DB$Display:		;Main debug display routine
DB$A	EQU	02	;Display codes for DB$Display
DB$HL	EQU	20
DB$M	EQU	24
