; Figure 5-3
;
;	Useful subroutines using the Write Console Byte
;	BDOS Function.
;
;	MSGOUT
;	Output null-byte terminated message
;
;	Calling sequence
;
;	MESSAGE:	DB	'Message',0
;		:
;		LXI	H,MESSAGE
;		CALL	MSGOUT
;
;	Exit Parameters
;		HL -> Null byte terminator
;
B$CONOUT	EQU	2	;Write console byte
BDOS		EQU	5	;BDOS Entry Point
;
MSGOUT:
	MOV	A,M	;Get next byte for output
	ORA	A
	RZ		;Return when null-byte
	INX	H	;Update message pointer
	PUSH	H	;Save updated pointer
	MOV	E,A	;Ready for BDOS
	MVI	C,B$CONOUT	;Function code
	CALL	BDOS
	POP	H	;Recover message pointer
	JMP	MSGOUT	;Go back for next character

