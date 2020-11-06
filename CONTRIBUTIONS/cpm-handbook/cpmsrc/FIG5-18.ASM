; Figure 5-18
;
;	GETC
;	This subroutine gets the next character from a
;	Sequential disk file. It assumes that the file has
;	already been opened.
;
;>>>	Note : this subroutine changes CP/M's DMA Address.
;
;	Entry Parameters
;
;		DE -> File Control Block.
;
;	Exit Parameters
;
;		A = next character from file.
;		    (= 0FFH on physical End of File).
;		    Note : 1AH is normal EOF character for
;			   ASCII Files.
;
;	Calling sequence
;
;		LXI	DE,FCB
;		CALL	GETC
;		CPI	1AH
;		JZ	EOFCHAR
;		CPI	0FFH
;		JZ	ACTUALEOF
;
B$READSEQ	EQU	20	;Read Sequential
B$SETDMA	EQU	26	;Set DMA Address
BDOS		EQU	5	;BDOS Entry Point
;
GETCBS	EQU	128		;Buffer Size
GETCBF:	DS	GETCBS		;Declare buffer
GETCCC:	DB	0		;Char. count (initially 'empty')
;
GETC:
	LDA	GETCCC		;Check if buffer is empty
	ORA	A
	JZ	GETCFB		;Yes, fill buffer
GETCRE:				;Re-entry point after buffer filled
	DCR	A		;No, downdate count
	STA	GETCCC		;Save downdated count

	MOV	B,A		;Compute offset of next character
	MVI	A,GETCBS-1	;By subtracting
	SUB	B		;(buffer size - downdated count)
	MOV	E,A		;Make result into word value
	MVI	D,0
	LXI	H,GETCBF	;HL -> Base of Buffer
	DAD	D		;HL -> Next character in buffer
	MOV	A,M		;Get next character
	RET
;
GETCFB:				;Fill Buffer
	PUSH	D		;Save FCB Pointer
	LXI	D,GETCBF	;Set DMA Address to Buffer
	MVI	C,B$SETDMA	;Function Code
	CALL	BDOS
	POP	D		;Recover FCB Pointer
	MVI	C,B$READSEQ	;Read Sequential 'record' (sector)
	CALL	BDOS
	ORA	A		;Check if read unsuccessful (A = NZ)
	JNZ	GETCX		;Yes
	MVI	A,GETCBS	;Reset count
	STA	GETCCC
	JMP	GETCRE		;Re-enter subroutine
;
GETCX:				;Physical end of file
	MVI	A,0FFH		;Indicate such
	RET
