; Figure 5-6
;
;	WL$PUN
;	Write Line to Punch device. Output terminates
;	when a 00H byte is encountered.
;	A Carriage Return is output when a Line Feed is
;	encountered.
;
;	Calling sequence
;
;		LXI	H,BUFFER
;		CALL	WL$PUN
;
;	Exit Parameters
;
;		HL -> 00H byte terminator
;
B$PUNOUT	EQU	4
BDOS		EQU	5
;
CR		EQU	0DH	;Carriage Return
LF		EQU	0AH	;Line Feed
;
WL$PUN:
	PUSH	H		;Save Buffer pointer
	MOV	A,M		;Get next character
	ORA	A		;Check if 00H
	JZ	WL$PUNX		;Yes, exit
	CPI	LF		;Check if Line Feed
	CZ	WL$PUNLF	;Yes, O/P CR
	MOV	E,A		;Character to be output
	MVI	C,B$PUNOUT	;Function Code
	CALL	BDOS		;Output character
	POP	H		;Recover Buffer pointer
	INX	H		;Update to next char.
	JMP	WL$PUN		;Output next char
WL$PUNLF:			;Line Feed encountered
	MVI	C,B$PUNOUT	;Function Code
	MVI	E,CR		;Output a CR
	CALL	BDOS
	MVI	A,LF		;Recreate Line Feed
	RET			;Output LF
WL$PUNX:			;Exit
	POP	H		;Balance the stack
	RET
