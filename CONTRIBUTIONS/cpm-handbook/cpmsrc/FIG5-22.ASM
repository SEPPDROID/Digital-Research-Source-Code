; Figure 5-22
;
;	SFA
;	Set File Attributes
;	This subroutine takes a compressed bit map of all the
;	File Attribute bits, expands them out into an existing
;	File Control Block and then requests CP/M to set the
;	Attributes in the File Directory.
;
;	Entry Parameters
;
;		DE -> File Control Block
;		HL = Bit Map. Only the most significant 11 bits
;		     are used, and correspond directly with the
;		     possible attribute bytes.
;
;	Exit Parameters
;
;		Carry Clear if operation successful (A = 0,1,2,3)
;		Carry Set if error (A = 0FFH)
;
;	Calling Sequence
;
;		LXI	D,FCB
;		LXI	H,0000$0000$1100$0000B	;Bit Map
;		CALL	SFA
;		JC	ERROR
;
;	File Attribute Equates
;
FA$F1	EQU	1000$0000$0000$0000B	;F1' )
FA$F2	EQU	0100$0000$0000$0000B	;F2' ) Available for use
FA$F3	EQU	0010$0000$0000$0000B	;F3' ) by Application Programs
FA$F4	EQU	0001$0000$0000$0000B	;F4' )
FA$F5	EQU	0000$1000$0000$0000B	;F5' ]
FA$F6	EQU	0000$0100$0000$0000B	;F6' ] Reserved for CP/M
FA$F7	EQU	0000$0010$0000$0000B	;F7' ]
FA$F8	EQU	0000$0001$0000$0000B	;F8' ]
;
FA$T1	EQU	0000$0000$1000$0000B	;T1' - Read/Only File
FA$RO	EQU	FA$T1
FA$T2	EQU	0000$0000$0100$0000B	;T2' - System Files
FA$SYS	EQU	FA$T2
FA$T3	EQU	0000$0000$0010$0000B	;T3' - Reserved for CP/M
;
;
B$SETFAT	EQU	30	;Set File Attributes
BDOS		EQU	5	;BDOS Entry Point
;
;
SFA:
	PUSH	D		;Save FCB Pointer
	INX	D		;HL -> 1st character of file name
	MVI	C,8+3		;Loop Count for File Name and Type
SFAL:				;Main Processing Loop
	XRA	A		;Clear Carry and A
	DAD	H		;Shift next MS Bit into Carry
	ACI	0		;A = 0 or 1 depending on Carry
	RRC			;Rotate LS bit of A into MS bit
	MOV	B,A		;Save result (00H or 80H)
	XCHG			;HL -> FCB character
	MOV	A,M		;Get FCB character
	ANI	7FH		;Isolate all but Attribute bit
	ORA	B		;Set Attribute with result
	MOV	M,A		;and store back into FCB
	XCHG			;DE -> FCB, HL = remaining bit map
	INX	D		;DE -> Next character in FCB
	DCR	C		;Downdate character count
	JNZ	SFAL		;Loop back for next character
	MVI	C,B$SETFAT	;Set File Attribute function code
	POP	D		;Recover FCB Pointer
	CALL	BDOS
	CPI	0FFH		;Carry set if OK, clear if error
	CMC			;Invert to use Carry set if error
	RET

	

