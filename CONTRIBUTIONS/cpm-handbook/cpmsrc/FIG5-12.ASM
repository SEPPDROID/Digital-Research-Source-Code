; Figure 5-12
;
;	OM
;	Output Message
;	This subroutine selects one of several messages based on
;	the contents of the A register on entry. It then displays
;	this message on the console.
;
;	Each message is declared with a '$' as its last character.
;	If the A register contains a value larger than the number
;	of messages declared, OM will output "Unknown Message".
;
;	As an option, OM can output Carriage Return / Line Feed
;	prior to outputting the message text.
;
;	Entry Parameters
;
;		HL -> Message Table
;			This has the form :
;				DB	3	;Number of Messages in table
;				DW	MSG0	;Address of text (A = 0)
;				DW	MSG1	;(A = 1)
;				DW	MSG2	;(A = 2)
;
;			MSG0:	DB	'Message text$'
;				...etc.
;
;		A = Message Code (from 0 on up)
;		B = Output CR/LF if Non-zero
;
;	Calling sequence
;
;		LXI	H,MSG$TABLE
;		LDA	MSGCODE
;		MVI	B,0		;Suppress CR/LF
;		CALL	OM
;
B$PRINTS	EQU	9	;Print $-terminated string
BDOS		EQU	5	;BDOS Entry Point
;
CR		EQU	0DH	;Carriage Return
LF		EQU	0AH	;Line Feed
;
OM$CRLF:	DB	CR,LF,'$'
OM$UM:		DB	'Unknown Message$'
;
;
OM:
	PUSH	PSW		;Save message code
	PUSH	H		;Save message table pointer
	MOV	A,B		;Check if CR/LF required
	ORA	A
	JZ	OM$NOCR		;No
	LXI	D,OM$CRLF	;Output CR/LF
	MVI	C,B$PRINTS
	CALL	BDOS
OM$NOCR:
	POP	H		;Recover message table pointer
	POP	PSW		;Recover message code
	CMP	M		;Compare message to max. value
	JNC	OM$ERR		;Error - code not <= max.
	INX	H		;Bypass max. value in table
	ADD	A		;Message Code * 2
	MOV	E,A		;Make Code * 2 a word value
	MVI	D,0
	DAD	D		;HL -> Address of message text
	MOV	E,M		;Get LS Byte
	INX	H		;HL -> MS Byte
	MOV	D,M		;Get MS Byte
				;DE -> Message text itself
OM$PS:			;Print string entry point
	MVI	C,B$PRINTS	;Function Code
	CALL	BDOS
	RET			;Return to caller
;
OM$ERR:			;Error
	LXI	D,OM$UM		;Point to "Unknown Message"
	JMP	OM$PS		;Print string
