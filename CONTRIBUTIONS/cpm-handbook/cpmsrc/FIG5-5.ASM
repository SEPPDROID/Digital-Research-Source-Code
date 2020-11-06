; Figure 5-5
;
;	RL$RDR
;	Read Line from Reader Device.
;	Carriage Returns are ignored, and input terminates
;	when specified number of characters have been read
;	or a Line Feed is input.
;
;	Note : Potential weakness is that there is no
;	timeout in this subroutine. It will wait forever
;	if no more characters arrive at the Reader device.
;
;	Calling sequence
;
;		LXI	H,BUFFER
;		LXI	B,MAXCOUNT
;		CALL	RL$RDR
;
;	Exit Parameters
;
;		HL -> 00H byte terminating string
;		BC = Residual Count (0 if Max. chars read)
;		E = Last Character Read
;
B$READIN	EQU	3	;Reader Input
BDOS		EQU	5	;BDOS Entry Point
;
CR		EQU	0DH	;Carriage Return
LF		EQU	0AH	;Line Feed (Terminator)
;
RL$RDR:
	MOV	A,C	;Check if count 0
	ORA	B	;If count 0 on entry,
	MOV	E,A	;  fake last char. read (00H)
	JZ	RL$RDRX	;Yes, exit
	PUSH	B	;Save Max. Chars count
	PUSH	H	;Save Buffer Pointer
RL$RDRI:		;Loop back here to ignore char.
	MVI	C,B$READIN
	CALL	BDOS	;A = Character input
	MOV	E,A	;Preserve copy of characters
	CPI	CR	;Check if Carriage Return
	JZ	RL$RDRI	;Yes, ignore it
	POP	H	;Recover Buffer pointer
	POP	B	;Recover Max. Count
	CPI	LF	;Check if Line Feed
	JZ	RL$RDRX	;Yes, exit
	MOV	M,A	;No, store character in buffer
	INX	H	;Update Buffer pointer
	DCX	B	;Downdate count
	JMP	RL$RDR	;Loop back for next char.
RL$RDRX:
	MVI	M,0	;Null byte terminate buffer
	RET
