; Figure 5-15
;
;	CDISK
;	Change Disk
;	This subroutine displays a message requesting the
;	to change the specified logical disk, then waits for
;	a Carriage Return to be pressed. It then issues
;	a Disk Reset and returns to the caller.
;
;	Entry Parameters
;
;		A = Logical disk to be changed (A = 0, B = 1)
;
;	Exit Parameters
;
;		None
;
;	Calling Sequence
;
;		MVI	A,0		;Change drive A:
;		CALL	CDISK
;
;
B$DSKRESET	EQU	13		;Disk Reset function code
B$PRINTS	EQU	9		;Print $-terminated string
B$CONIN		EQU	1		;Get console input
BDOS		EQU	5		;BDOS Entry Point
;
CR		EQU	0DH
LF		EQU	0AH
;
CDISKM:		DB	CR,LF,'Change logical disk '
CDISKD:		DB	0
		DB	': and press Carriage Return to continue$'
;
CDISK:
	ADI	'A'-1		;Convert to Letter
	STA	CDISKD		;Store into message
	MVI	C,B$PRINTS	;Display Message
	LXI	D,CDISKM
	CALL	BDOS
CDISKW:
	MVI	C,B$CONIN	;Get keyboard character
	CALL	BDOS
	CPI	CR
	JNZ	CDISKW
	MVI	C,B$DSKRESET	;Now reset disk system
	CALL	BDOS
	RET
