; Figure 5-11
;
;	This example shows how to use the Get and Set IOBYTE
;	functions to implement a simple Terminal emulator. 
;	For this example to work, the BIOS must detect the
;	Console Value being set to 3 (IO$CUC1) and connect
;	Console Status, Input and Output functions to the
;	Communications line.
;
B$DIRCONIO	EQU	6	;Direct Console Input/Output
B$GETIO		EQU	7	;Get IOBYTE
B$SETIO		EQU	8	;Set IOBYTE
B$CONST		EQU	11	;Get Console Status (sneak preview)
BDOS		EQU	5	;BDOS entry point
;
IO$CONM	EQU	0000$0011B	;Console Mask for IOBYTE
IO$CCRT	EQU	1		;Console -> CRT:
IO$CUC1	EQU	3		;Console -> User Console #1
;
TERM:
	CALL	SETCRT		;Connect Console -> CRT:
TERM$CKS:
	CALL	CONST		;Get CRT status
	JZ	TERM$NOKI	;No console input
	CALL	CONIN		;Get keyboard character
	CALL	SETCOMM		;Connect Console -> Comm. Line
	CALL	CONOUT		;Output to Comm. Line
TERM$CCS:			;Check Comm. Status
	CALL	CONST		;Get "Console" status
	JZ	TERM		;No incoming Comm. character
	CALL	CONIN		;Get incoming Comm. character
	CALL	SETCRT		;Connect Console -> CRT:
	CALL	CONOUT		;Output to CRT
	JMP	TERM$CKS	;Loop back to check keyboard status
TERM$NOKI:
	CALL	SETCOMM		;Connect Console -> Comm. Line
	JMP	TERM$CCS	;Loop back to check Comm. status
;
SETCRT:				;Connect Console -> CRT:
	PUSH	PSW		;Save possible data character
	MVI	B,IO$CCRT	;Connect Console -> CRT:
	JMP	SETCON		;Common Code

SETCOMM:			;Connect Console -> Comm. Line
	PUSH	PSW		;Save possible data character
	MVI	B,IO$CUC1	;Connect Console -> Comm. Line
				;Drop into SETCON

SETCON:			;Set Console Device
			;New code in B (in bits 1,0)
	PUSH	B		;Save code
	MVI	C,B$GETIO	;Get current IOBYTE
	CALL	BDOS
	ANI	(NOT IO$CONM) AND 0FFH	;Preserve all but console
	POP	B		;Recover required code
	ORA	B		;OR in new bits
	MOV	E,A		;Ready for setting
	MVI	C,B$SETIO	;Function code
	CALL	BDOS
	POP	PSW		;Recover possible data character
	RET
CONOUT:
	MOV	E,A		;Get data byte for output
	MVI	C,B$DIRCONIO	;Function code
	JMP	BDOS		;BDOS returns to CONOUT's caller
CONIN:
	MVI	C,B$DIRCONIO	;Function Code
	MVI	E,0FFH		;Indicate Console Input
	JMP	BDOS		;BDOS returns to CONIN's caller
CONST:
	MVI	C,B$CONST	;Function Code
	CALL	BDOS
	ORA	A		;Set Z-flag to result
	RET
