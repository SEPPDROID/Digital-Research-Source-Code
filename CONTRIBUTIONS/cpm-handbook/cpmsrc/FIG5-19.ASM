; Figure 5-19
;
;	PUTC
;	This subroutine either puts the next chararacter out
;	to a sequential file, writing out completed 'records'
;	(128-byte sectors) or, if requested to, will fill the
;	remainder of the current 'record' with 1AH's to indicate
;	End of File to CP/M.
;
;	Entry Parameters
;
;		DE -> File Control Block
;		B = 0, A = next data character to be output
;		B /= 0, fill the current 'record' with 1AH's
;
;	Exit Parameters
;
;		None.
;
;	Calling sequence
;
;		LXI	D,FCB
;		MVI	B,0	;Not end of File
;		LDA	CHAR
;		CALL	PUTC
;	   ..or
;		LXI	D,FCB
;		MVI	B,1	;Indicate end of file
;		CALL	PUTC
;
B$WRITESEQ	EQU	21	;Write Sequential
B$SETDMA	EQU	26	;Set DMA Address
BDOS		EQU	5	;BDOS Entry Point
;
PUTCBS	EQU	128		;Buffer Size
PUTCBF:	DS	PUTCBS		;Declare buffer
PUTCCC:	DB	0		;Char. count (initially 'empty')
;
PUTC:
	PUSH	D		;Save FCB Address
	PUSH	PSW		;Save data character
	MOV	A,B		;Check if end of file requested
	ORA	A
	JNZ	PUTCEF		;Yes
	CALL	PUTCGA		;No, get address of next free byte
				;HL -> next free byte
				;E = Current Char. count (as well as A)
	POP	PSW		;Recover data character
	MOV	M,A		;Save in buffer
	MOV	A,E		;Get current character count
	INR	A		;Update character count
	CPI	PUTCBS		;Check if buffer full
	JZ	PUTCWB		;Yes, write buffer
	STA	PUTCCC		;No, save updated count
	POP	D		;Dump FCB Address for return
	RET
;
PUTCEF:				;End of file
	POP	PSW		;Dump data character
	CALL	PUTCGA		;HL -> next free byte
				;A = Current character count
PUTCCE:				;Copy EOF character
	CPI	PUTCBS		;Check for end of buffer
	JZ	PUTCWB		;Yes, write out the buffer
	MVI	M,1AH		;No, store EOF in buffer
	INR	A		;Update count
	INX	H		;Update buffer pointer
	JMP	PUTCCE		;Continue until end of buffer
;
PUTCWB:				;Write buffer
	XRA	A		;Reset character count to 0
	STA	PUTCCC
	LXI	D,PUTCBF	;DE -> Buffer
	MVI	C,B$SETDMA	;Set DMA Address -> Buffer
	CALL	BDOS
	POP	D		;Recover FCB Address
	MVI	C,B$WRITESEQ	;Write Sequential Record
	CALL	BDOS
	ORA	A		;Check if error
	JNZ	PUTCX		;Yes if A = NZ
	RET			;No, return to caller
;
PUTCX:				;Error exit
	MVI	A,0FFH		;Indicate such
	RET
;
PUTCGA:				;Return with HL -> next free char.
				;and A = current char. count
	LDA	PUTCCC		;Get current character count
	MOV	E,A		;Make word value in DE
	MVI	D,0
	LXI	H,PUTCBF	;HL -> Base of buffer
	DAD	D		;HL -> next free character
	RET
