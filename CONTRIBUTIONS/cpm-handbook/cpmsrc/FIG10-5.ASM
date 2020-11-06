;	Figure 10-5
;
;	Testbed for Real Time Clock driver in the BIOS.
;
;	The complete source file consists of three components:
;
;		1. The testbed code shown here.
;		2. The Real Time Clock driver destined for the BIOS.
;		3. The debug package shown in Figure 10-2.
;
TRUE	EQU	0FFFFH
FALSE	EQU	NOT TRUE

DEBUG	EQU	TRUE		;For conditional assembly of RST
				;  instructions in place of IN and
				;  OUT instructions in the drivers.
RST6	EQU	30H		;Use RST 6 for fake clock tick.

	ORG	100H
START:
	LXI	SP,Test$Stack	;Use local stack
	CALL	DB$Init		;Initialize the debug package
	MVI	A,JMP		;Setup RST 6 with JMP opcode
	STA	RST6
	LXI	H,RTC$Interrupt	;Setup RST 6 JMP address
	SHLD	RST6 + 1

	JMP	Testbed$Loop	;<=== REMOVE THIS JMP WHEN READY TO
				;     TEST WATCHDOG ROUTINES

	LXI	B,50		;50 ticks before timeout
	LXI	H,WD$Timeout	;Address to transfer to
	CALL	Set$Watchdog	;Set the watchdog timer
;
;
;	Make repeated entry to RTC Interrupt routine
;	to ensure that clock is correctly updated.
;
Testbed$Loop:
	MVI	A,0AAH		;Set registers to known pattern
	LXI	B,0BBCCH
	LXI	D,0DDEEH
	LXI	H,0FF11H
	RST	6		;Fake interrupt clock

	CALL	DB$MSGI		;Display in-line message
	DB	'Clock =',0

	LXI	H,Time$In$ASCII	;Get address of clock in driver
	CALL	DB$MSG		;Display current clock value
				; (Note : Time$In$ASCII already has
				;  a Line Feed character in it)
	CALL	DB$MSGI		;Display in-line message
	DB	0DH,0		;Carriage Return

	JMP	Testbed$Loop
;
;	Control arrives here when the watchdog timer times
;	out.
WD$Timeout:
	CALL	DB$MSGI
	DB	0DH,0AH,'Watchdog timed out',0
	RET			;Return to watchdog routine
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
RTC$Interrupt:		;Interrupt service routine for clock tick
Set$Watchdog:		;Set Watchdog timer
Time$In$ASCII:		;ASCII string of HH:MM:SS, LF, 0
;
;	Debug routines (Figure 10-2)
;
DB$Init:		;Debug initialization
DB$MSGI:		;Display message in-line
DB$MSG:			;Display message
