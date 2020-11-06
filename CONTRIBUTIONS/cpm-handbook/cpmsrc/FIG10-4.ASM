;	Figure 10-4
;
;	Testbed for Character I/O drivers in the BIOS.
;
;	The complete source file consists of three components:
;
;		1. The testbed code shown here.
;		2. The Character I/O drivers destined for the BIOS.
;		3. The debug package shown in Figure 10-2.
;
TRUE	EQU	0FFFFH
FALSE	EQU	NOT TRUE

DEBUG	EQU	TRUE		;For conditional assembly of RST
				;  instructions in place of IN and
				;  OUT instructions in the drivers.
RST6	EQU	30H		;Use RST 6 for fake incoming character
				;  interrupt.
	ORG	100H
START:
	LXI	SP,Test$Stack	;Use a local stack
	CALL	DB$Init		;Initialize the debug package
	MVI	A,JMP		;Setup RST 6 with JMP opcode
	STA	RST6
	LXI	H,Character$Interrupt	;Setup RST 6 JMP address
	SHLD	RST6 + 1
;
;	Make repeated entry to Character Interrupt routine
;	to ensure that characters can be captured and stored in
;	an input buffer
;
Testbed$Loop:
	MVI	A,0AAH		;Set registers to known pattern
	LXI	B,0BBCCH
	LXI	D,0DDEEH
	LXI	H,0FF11H
	RST	6		;Fake interrupt for incoming character

	CALL	DB$MSGI		;Display in-line message
	DB	0DH,0AH,'Enter I to Input Char., O to Output, D to enter '
	DB	'DDT : ',0

	CALL	DB$CONINU	;Get upper case character
	CPI	'I'		;CONIN?
	JZ	Go$CONIN
	CPI	'D'		;DDT?		
	JZ	Go$DDT
	CPI	'O'		;CONOUT?
	JZ	Go$CONOUT
	JMP	Testbed$Loop	;Loop back to interrupt again
Go$DDT:
	RST	7		;Enter DDT (RST 7 setup by DDT)
	JMP	Testbed$Loop
Go$CONIN:
	CALL	CONST		;Get Console Status
	JZ	Testbed$Loop	;No Data waiting
	CALL	CONIN		;Get data from buffer

	CALL	DB$Display	;Display character returned
	DB	DB$A		;  in A register
	DB	'CONIN returned',0

	JMP	Go$CONIN	;Repeat CONIN loop until no chars.
				;  waiting.
;
Go$CONOUT:
	CALL	CONST		;Get Console Status
	JZ	Testbed$Loop	;No data waiting
	CALL	CONIN
	MOV	C,A		;Ready for output
	CALL	CONOUT		;Output to Console
	JMP	Go$CONOUT	;Repeat while there is still data
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
CONST:			;BIOS Console Status
CONIN:			;BIOS Console Input
CONOUT:			;BIOS Console Output;
Character$Interrupt:	;Interrupt service routine for incoming chars.
;
;	Debug routines (Figure 10-2)
;
DB$Init:		;Debug initialization
DB$MSGI:		;Display message in-line
DB$CONINU:		;Get upper case character from keyboard
DB$Display:		;Main debug display routine
DB$A	EQU	02	;Display code for DB$Display
