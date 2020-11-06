; Figure 5-13
;
;	RSA
;	Return Subprocessor Address
;	This subroutine returns one of several addresses selected
;	from a table by matching keyboard input against
;	specified strings. It is normally used to switch control
;	to a particular subprocessor according to an option entered
;	by the operator from the keyboard.
;
;	Character string comparisons are performed with case-folding,
;	that is lower-case letters are converted to upper-case.
;
;	If the operator input fails to match any of the specified
;	strings, then the Carry flag is set, otherwise it is cleared.
;
;	Entry Parameters
;
;		HL -> Subprocessor Select Table
;			This has the form :
;			DW	TEXT0,SUBPROC0
;			DW	TEXT1,SUBPROC1
;			DW	0	;Terminator
;
;		TEXT0:	DB	'add',0		;00H-byte terminated
;		TEXT1:	DB	'subtract',0
;
;		SUBPROC0:
;			Code for processing ADD function.
;		SUBPROC1:
;			Code for processing SUBTRACT function.
;
;	Exit Parameters
;
;		DE -> operator input string (00H-terminated input string).
;		Carry Clear, HL -> Subprocessor.
;		Carry Set, HL = 0000H.
;
;	Calling Sequence
;
;		LXI	H,SUBPROCTAB	;Subprocessor Table
;		CALL	RSA
;		JC	ERROR		;Carry set only on error
;		LXI	D,RETURN	;Fake CALL instruction
;		PUSH	D		;Push Return address on stack
;		PCHL			;"CALL" to Subprocessor
;	RETURN:
;
B$READCONS	EQU	10	;Read Console String into Buffer
BDOS		EQU	5	;BDOS Entry Point
;
RSA$BL		EQU	80	;Buffer Length
RSA$BUF:	DB	RSA$BL	;Max. no. of characters
RSA$ACTC:	DB	0	;Actual no. of characters
RSA$BUFC:	DS	RSA$BL	;Buffer characters
		DB	0	;Safety terminator
;
;
RSA:
	DCX	H		;Adjust Subproc. pointer for code below
	DCX	H
	PUSH	H		;Top of Stack (TOS) -> Subproc. table - 2
	MVI	C,B$READCONS	;Function Code
	LXI	D,RSA$BUF	;DE -> Buffer
	CALL	BDOS		;Read operator input
			;Convert input to 00H-terminated
	LXI	H,RSA$ACTC	;HL -> Actual no. of chars input
	MOV	E,M		;Get Actual no. of chars input
	MVI	D,0		;Make into word value
	INX	H		;HL -> first data character
	DAD	D		;HL -> first UNUSED character in buffer
	MVI	M,0		;Make input buffer 00H-terminated
			;Compare input to specified values
			;Main loop
RSA$ML:
	POP	H		;Recover subprocessor table pointer
	INX	H		;Move to top of next entry
	INX	H		;HL -> Text Address
	MOV	E,M		;Get text address
	INX	H
	MOV	D,M		;DE -> text

	MOV	A,D		;Check if at end of subproc. table
	ORA	E
	JZ	RSA$NFND	;Match not Found

	INX	H		;HL -> subprocessor address
	PUSH	H		;Save ptr to subprocessor table
	LXI	H,RSA$BUFC	;HL -> input characters
	CALL	FSCMP		;Folded string compare
	JNZ	RSA$ML		;No match, move to next entry
	POP	H		;Match found, recover subproc. ptr
	MOV	E,M		;Get actual subprocessor address
	INX	H
	MOV	D,M		;DE -> Subprocessor code
	XCHG			;HL -> Subprocessor code
	ORA	A		;Clear carry (match found)
	RET
;
RSA$NFND:
	LXI	H,0		;Indicate no match found
	STC			;Set carry
	RET
;
;
;	FSCMP
;	Folded (lower case to upper) string compare.
;	This subroutine compares two 00H-byte terminated
;	strings and returns with the condition flags set
;	to indicate their relationship.
;
;	Entry parameters
;
;		DE -> String 1
;		HL -> String 2
;
;	Exit parameters
;
;		Flags set (based on String1 - String2, on a 
;			   character-by-character basis)
;
;
FSCMP:
	LDAX	D		;Get String 1 character
	CALL	FOLD		;Fold to upper case
	PUSH	PSW		;Save String 1 character
	MOV	A,M		;Get String 2 character
	CALL	FOLD		;Fold to upper case
	MOV	B,A		;Save String 2 character
	POP	PSW		;Recover String 1 character
	CMP	B		;String 1 - String 2
	RNZ			;Return if not equal
	ORA	A		;Equal, so check if end of strings
	RZ			;Yes
	INX	D		;No, update String 1 pointer
	INX	H		;	and String 2 pointer
	JMP	FSCMP		;Check next character
;
;
;	FOLD
;	Folds a lower case letter (a-z) to UPPER CASE (A-Z)
;
;	The character to be folded is in A on entry and on exit.
;
FOLD:
	MOV	C,A		;Preserve input character
	MVI	A,'a'-1		;Check if folding needed
	CMP	C		;Compare to input character
	JNC	FOLDX		;No, char. is <= 'a'
	MVI	A,'z'		;Check if < 'z'
	CMP	C
	JC	FOLDX		;No, char. is > 'z'
	MVI	A,0DFH		;Fold character
	ANA	C
	RET
FOLDX:
	MOV	A,C		;Recover original input char.
	RET
