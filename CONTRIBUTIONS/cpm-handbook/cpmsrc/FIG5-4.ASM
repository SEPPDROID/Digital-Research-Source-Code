; Figure 5-4
;
;	Useful subroutines using the Write Console Byte
;	BDOS Function.
;
;	MSGOUTI (Message Out In-line)
;	Output null-byte terminated message that follows the
;	CALL to MSGOUTI.
;
;	Calling sequence
;
;		CALL	MSGOUTI
;		DB	'Message',0
;		... next instruction
;
;	Exit Parameters
;		HL -> instruction following message
;
B$CONOUT	EQU	2	;Write console byte
BDOS		EQU	5	;BDOS Entry Point
;
MSGOUTI:
	POP	H	;HL -> Message
	MOV	A,M	;Get next data byte
	INX	H	;Update message pointer
	ORA	A	;Check if null byte
	JNZ	MSGOUTIC	;No, continue
	PCHL		;Yes, return to next instruction
			;after in-line message
MSGOUTIC:
	PUSH	H	;Save message pointer
	MOV	E,A	;Ready for BDOS
	MVI	C,B$CONOUT	;Function code
	CALL	BDOS
	JMP	MSGOUTI	;Go back for next char.

