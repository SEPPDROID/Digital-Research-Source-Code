; Figure 5-8
;
;--------------------------------------------------------------
;	TESTBED CODE
;	Because of the complexity of this subroutine, the actual
;	testbed code has been left in this example. It assumes
;	that DDT or ZSID will be used to checkout
	IF	1	;Change to IF 0 to disable testbed
	ORG	100H	
	JMP	START	;Bypass "variables" setup by DDT
OPTIONS:	DB	0		;Option Flags
TERMS:		DB	'A','E','I',0	;Terminators
BUFFER		DB	5		;Max. Characters in Buffer
		DB	0		;Actual Count
		DB	99,99,99,99,99,99,99	;Data Bytes
START:
	LXI	H,BUFFER	;Get address of buffer
	LXI	D,TERMS		;Address of terminator table
	LDA	OPTIONS		;Get options set by DDT
	MOV	B,A		;Put in correct register
	CALL	RCS		;Enter subroutine
	CALL	38H		;Force DDT breakpoint
	JMP	START		;Test again
	ENDIF			;End of testbed x`ce
;---------------------------------------------------------------
;
;	RCS : Read Console String (using Raw Input)
;
;	Reads a string of characters into a memory buffer using
;	Raw Input. Supports options :
;
;	*	to echo characters or not (when echoing, a Carriage
;		Return will be echoed followed by Line Feed)
;	*	warm boot on input of Control-C or not
;	*	terminating input either on :
;			- max. no of chars input
;			- matching terminator character
;
; Calling Sequence
;
;	LXI	H,BUFFER	Buffer has structure
;				BUFFER:	DB	10	Max. size
;					DB	0	Actual Read
;					DS	10+1	Buffer area
;	MVI	B,OPTIONS	Options required (see EQUates)
;	LXI	D,TERMS		Pointer to 00H-byte terminated
;				Chars, any one of which is a terminator.
;	CALL	RCS
;
; Exit Parameters
;
;	BUFFER : Updated with data bytes and actual character count
;		 input. (Does not include the terminator).
;	A = Terminating Code
;		0 = 	Maximum number of characters input.
;		NZ =	Terminator character found.
;
RCS$ECHO	EQU	0000$0001B	;Input characters to be echoed
RCS$ABORT	EQU	0000$0010B	;Abort on Control-C
RCS$FOLD	EQU	0000$0100B	;Fold lower case to upper
RCS$TERM	EQU	0000$1000B	;DE -> Term. char. set
;
B$DIRCONIO	EQU	6	;Direct console I/O
BDOS		EQU	5	;BDOS entry point

CTL$C		EQU	03H	;Control-C
CR		EQU	0DH	;Carriage Return
LF		EQU	0AH	;Line Feed
BS		EQU	08H	;Backspace

RCS$ST:				;Internal standard terminator table
	DB	0DH		;Carriage return
	DB	0AH		;Line feed
	DB	0		;End of table
;
RCS$BSS:			;Destructive backspace sequence
	DB	BS,' ',BS,0
;
RCS:				;<<<<< Main Entry
	INX	H		;HL -> Actual count
	MVI	M,0		;Reset to initial state
	DCX	H		;HL -> Max. count
RCS$L:
	PUSH	H		;Save buffer pointer
	CALL	RCS$GC		;Get character and execute :
				; ECHO, ABORT, and FOLD options
				;C = character input
	POP	H		;Recover buffer pointer
	MVI	A,RCS$TERM	;Check if user-specified terminator
	ANA	B		;B = options
	JNZ	RCS$UST		;User specified terminators
	LXI	D,RCS$ST	;Standard terminators
RCS$UST:
	CALL	RCS$CT		;Check for terminator
	JZ	RCS$NOTT	;Not terminator
	MOV	B,A		;Preserve terminating char
RCS$MCI:			;(Max. char input shares this code)
	MVI	C,0		;Terminate buffer
	CALL	RCS$SC		;Save character
	MOV	A,B		;Recover terminating char
	ORA	A		;Set flags
	RET
RCS$NOTT:			;Not a terminator
	MVI	A,BS		;Check for backspace
	CMP	C
	JZ	RCS$BS		;Backspace entered
	CALL	RCS$SC		;Save character in buffer
	CALL	RCS$UC		;Update count
	JNZ	RCS$L		;Not max. so get another char
	MVI	B,0		;Fake terminating char
	JMP	RCS$MCI		;A = 0 for max. chars input
;
RCS$BS:				;Backspace entered
	PUSH	H		;Save buffer pointer
	INX	H		;HL -> actual count
	DCR	M		;Backup one
	JM	RCS$NBS		;Check if count now -ve
	LXI	H,RCS$BSS	;HL -> Backspacing sequence
	MVI	A,RCS$ECHO	;No, check if echoing
	ANA	B		;(BS will have been echoed if so)
	JZ	RCS$BSNE	;No, input BS not echoed
	INX	H		;Bypass initial Backspace
RCS$BSNE:
	PUSH	B		;Save Options and character
	PUSH	D		;Save terminator table pointer
	CALL	WCS		;Write console string
	POP	D		;Recover terminator table pointer
	POP	B		;Recover options and character
	JMP	RCS$BSX		;Exit from Backspace logic
RCS$NBS:
	INR	M		;Reset count to 0
RCS$BSX:
	POP	H		;Recover buffer pointer
	JMP	RCS$L		;Get next character
	
RCS$SC:			;Save character in C in buffer
			;HL -> buffer pointer
	PUSH	D		;Save terminator table pointer
	PUSH	H		;Save buffer pointer
	INX	H		;HL -> actual count in buffer
	MOV	E,M		;Get actual count
	INR	E		;Count of 0 points to first data byte
	MVI	D,0		;Make word value of actual count
	DAD	D		;HL -> next free data byte
	MOV	M,C		;Save data byte away
	POP	H		;Recover buffer pointer
	POP	D		;Recover terminator table pointer
	RET
;
RCS$UC:			;Update buffer count and check for max.
			;Return Z set if = to max, NZ if not
			;HL -> buffer on entry
	PUSH	H		;Save buffer pointer
	MOV	A,M		;Get max. count
	INX	H		;HL -> actual count
	INR	M		;Increase actual count
	CMP	M		;Compare max. to actual
	POP	H		;Recover buffer pointer
	RET			;Z-flag set
;
RCS$GC:			;Get character and execute 
			; ECHO, ABORT and FOLD options
	PUSH	D		;Save terminator table pointer
	PUSH	H		;Save buffer pointer
	PUSH	B		;Save option flags
RCS$WT:
	MVI	C,B$DIRCONIO	;Function code
	MVI	E,0FFH		;Specify input
	CALL	BDOS
	ORA	A		;Check if data waiting
	JZ	RCS$WT		;Go back and wait
	POP	B		;Recover option flags
	MOV	C,A		;Save data byte
	MVI	A,RCS$ABORT	;Check if abort option enabled
	ANA	B
	JZ	RCS$NA		;No abort
	MVI	A,CTL$C		;Check for control-C
	CMP	C
	JZ	0		;Warm boot
RCS$NA:
	MVI	A,RCS$FOLD	;Check if folding enabled
	ANA	B
	CNZ	TOUPPER		;Fold to UPPER CASE
	MVI	A,RCS$ECHO	;Check if echo required
	ANA	B
	JZ	RCS$NE		;No echo required
	PUSH	B		;Save options and character
	MOV	E,C		;Move character for output
	MVI	C,B$DIRCONIO	;Function code
	CALL	BDOS		;Echo character
	POP	B		;Recover options and character
	MVI	A,CR		;Check if Carriage Return
	CMP	C
	JNZ	RCS$NE		;No
	PUSH	B		;Save options and character
	MVI	C,B$DIRCONIO	;Function code
	MVI	E,LF		;Output line feed
	CALL	BDOS
	POP	B		;Recover options and character
RCS$NE:
	POP	H		;Recover buffer pointer
	POP	D		;Recover terminator table
	RET			;Character in C
;
RCS$CT:			;Check for terminator
			;C = character just input
			;DE -> 00-byte character string of Term. Chars
			;Returns Z status if no match found, NZ if found
			; (with A = C = Terminating character)
	PUSH	D		;Save table pointer
RCS$CTL:
	LDAX	D		;Get next terminator character
	ORA	A		;Check for end of table
	JZ	RCS$CTX		;No terminator matched
	CMP	C		;Compare to input character
	JZ	RCS$CTX		;Terminator matched
	INX	D		;Move to next terminator
	JMP	RCS$CTL		;loop to try next character in table
RCS$CTX:			;Check terminator exit
	ORA	A		;At this point, A will either be 0
				; if the end of the table has been
				; reached, or NZ if a match has been
				; found. The Z-flag will be set.
	POP	D		;Recover table pointer
	RET

;
;	TOUPPER - Fold lower case letters to upper
;
;	C = Character on entry and exit
;
TOUPPER:
	MVI	A,'a'-1		;Check if folding needed
	CMP	C		;Compare to input char
	JNC	TOUPX		;No, char is < or =  'a'-1
	MVI	A,'z'		;Maybe, char is = or > 'a'
	CMP	C
	JC	TOUPX		;No, char is > 'z'
	MVI	A,0DFH		;Fold character
	ANA	C
	MOV	C,A		;Return folded character
TOUPX:
	RET
;
;
;	WCS - Write Console String (Using Raw I/O)
;
;	Output terminates when a 00H byte is encountered.
;	A Carriage Return is output when a Line Feed is
;	encountered.
;
;	Calling sequence
;
;		LXI	H,BUFFER
;		CALL	WCS
;
;	Exit Parameters
;
;		HL -> 00H byte terminator
;
;
WCS:
	PUSH	H		;Save buffer pointer
	MOV	A,M		;Get next character
	ORA	A		;Check if 00H
	JZ	WCSX		;Yes, exit
	CPI	LF		;Check if Line Feed
	CZ	WCSLF		;Yes, O/P CR
	MOV	E,A		;Character to be output
	MVI	C,B$DIRCONIO	;Function Code
	CALL	BDOS		;Output character
	POP	H		;Recover Buffer pointer
	INX	H		;Update to next char.
	JMP	WCS		;Output next char
WCSLF:				;Line Feed encountered
	MVI	C,B$DIRCONIO	;Function Code
	MVI	E,CR		;Output a CR
	CALL	BDOS
	MVI	A,LF		;Recreate Line Feed
	RET			;Output LF
WCSX:				;Exit
	POP	H		;Balance the stack
	RET
