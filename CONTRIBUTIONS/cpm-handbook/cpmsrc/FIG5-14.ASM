; Figure 5-14
;
;	CCPM
;	Check if CP/M
;	This subroutine determines the version number of the
;	Operating System and, if not CP/M version 2, displays
;	an error message and executes a warm boot.
;
;	Entry and Exit Parameters
;
;		None
;
;	Calling sequence
;
;		CALL	CCPM	;Warm boots if not CP/M 2
;
B$PRINTS	EQU	9	;Display $-terminated string
B$GETVER	EQU	12	;Get version number
BDOS		EQU	5	;BDOS Entry Point
;
CR		EQU	0DH	;Carriage Return
LF		EQU	0AH	;Line Feed
;
CCPMM:	DB	CR,LF
	DB	'This program can only run under CP/M version 2.'
	DB	CR,LF,'$'
;
;
CCPM:
	MVI	C,B$GETVER	;Get version number
	CALL	BDOS
	MOV	A,H		;H must be 0 for CP/M
	ORA	A
	JNZ	CCPME		;Must be MP/M
	MOV	A,L		;L = version number of CP/M
	ANI	0F0H		;Version number in MS Nibble
	CPI	20H		;Check if version 2
	JNZ	CCPME		;Must be an earlier version
	RET			;Yes, we under CP/M version 2
;
CCPME:				;Error
	MVI	C,B$PRINTS	;Display error message
	LXI	D,CCPMM
	CALL	BDOS
	JMP	0		;Warm Boot
