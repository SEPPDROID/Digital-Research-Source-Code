; Figure 5-7
;
;	WL$LST
;	Write Line to List device. Output terminates
;	when a 00H byte is encountered.
;	A Carriage Return is output when a Line Feed is
;	encountered.
;
;	Calling sequence
;
;		LXI	H,BUFFER
;		CALL	WL$LST
;
;	Exit Parameters
;
;		HL -> 00H byte terminator
;
B$LSTOUT	EQU	5
BDOS		EQU	5
;
CR		EQU	0DH	;Carriage Return
LF		EQU	0AH	;Line Feed
;
WL$LST:
	PUSH	H		;Save Buffer pointer
	MOV	A,M		;Get next character
	ORA	A		;Check if 00H
	JZ	WL$LSTX		;Yes, exit
	CPI	LF		;Check if Line Feed
	CZ	WL$LSTLF	;Yes, O/P CR
	MOV	E,A		;Character to be output
	MVI	C,B$LSTOUT	;Function Code
	CALL	BDOS		;Output character
	POP	H		;Recover Buffer pointer
	INX	H		;Update to next char.
	JMP	WL$LST		;Output next char
WL$LSTLF:			;Line Feed encountered
	MVI	C,B$LSTOUT	;Function Code
	MVI	E,CR		;Output a CR
	CALL	BDOS
	MVI	A,LF		;Recreate Line Feed
	RET			;Output LF
WL$LSTX:			;Exit
	POP	H		;Balance the stack
	RET
