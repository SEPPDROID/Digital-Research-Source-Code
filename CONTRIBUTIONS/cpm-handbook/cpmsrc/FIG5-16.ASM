; Figure 5-16
;
;	OPENF
;	Open File
;
;	Given a pointer to a 00H-byte terminated file name,
;	and an area that can be used for a File Control Block,
;	this subroutine builds a valid File Control Block, and
;	attempts to open the File.
;
;	If the file is opened, it returns with Carry Flag clear.
;	If the file cannot be opened, this subroutine returns 
;	with the Carry Flag set.
;
;	Entry Parameters
;
;		DE -> 36-byte area for File Control Block
;		HL -> 00H-byte terminated file name of the form
;			{disk:} Name {.typ}
;			(That is, the disk and typ are optional)
;
;	Exit Parameters
;
;		Carry Clear : File opened correctly.
;		Carry Set   : File not opened.
;
;	Calling Sequence
;
;		LXI	D,FCB
;		LXI	H,FNAME
;		CALL	OPENF
;		JC	ERROR
;
;		where
;
;	FCB:	DS	36	;Space for File Control Block
;	FNAME:	DB	'A:TESTFILE.DAT',0
;
;
B$OPEN		EQU	15	;File Open Function Code
BDOS		EQU	5	;BDOS Entry Point
;
;
OPENF:
	PUSH	D		;Preserve pointer to FCB
	CALL	BF		;Build File Control Block (FCB)
	MVI	C,B$OPEN
	POP	D		;Recover Pointer to FCB
	CALL	BDOS
	RAL			;If A=0FFH, Carry set
				;otherwise Carry Clear
	RET
;
;	BF
;	Build File Control Block
;	This subroutine formats a 00H-byte terminated string
;	(presumed to be a File Name) into an FCB, setting the
;	Disk, File name and Type and clearing the remainder of the
;	FCB to 0's.
;
;	Entry Parameters
;
;		DE -> File Control Block (36 Bytes)
;		HL -> File Name String (00H-byte terminated)
;
;	Exit Parameters
;
;		The built File Control block.
;
;	Calling Sequence
;
;		LXI	D,FCB
;		LXI	H,FILENAME
;		CALL	BF
;
;
BF:
	INX	H		;Check if 2nd char. is ":"
	MOV	A,M		;Get character from file name
	DCX	H		;HL -> now back at 1st char
	CPI	':'		;If ":", then Disk specified
	JNZ	BF$ND		;No disk
	MOV	A,M		;Get disk letter
	ANI	0001$1111B	;A (41H) -> 1, B (42H) -> 2 ...
	INX	H		;Bypass disk letter
	INX	H		;Bypass ":"
	JMP	BF$SD		;Store disk in FCB
BF$ND:				;No disk present
	XRA	A		;Indicate default disk
BF$SD:
	STAX	D		;Store disk in FCB
	INX	D		;DE -> 1st char. of name in FCB
	MVI	C,8		;File name length
	CALL	BF$GT		;Get token
				;Note - at this point, BF$GT will
				;have advanced the string pointer to
				;either a '.' or 00H byte
	CPI	'.'		;Check terminating character
	JNZ	BF$NT		;No file type specified
	INX	H		;Bypass '.' in file name
BF$NT:
	MVI	C,3		;File Type Length
	CALL	BF$GT		;Get Token
				;Note - if no file type is present
				;BF$GT will merely spacefill the FCB
	MVI	B,0		;0-fill the remainder of the FCB
	MVI	C,24		;36 - 12 (Disk, Name, Type = 12 chars.)
	CALL	BF$FT		;Re-use Fill Token S/R
	RET
;
;	BF$GT
;	Build FCB - Get Token
;
;	This subroutine scans a file name string,
;	placing characters into a File Control Block.
;	On encountering a terminator character ("." or 00H),
;	the remainder of the token is space filled. 
;	If an "*" is encountered, the remainder of the token
;	is filled with "?".
;
;	Entry Parameters
;
;		DE -> Into File Control Block
;		HL -> Into File Name String
;		C = Maximum no. of characters in token
;
;	Exit Parameters
;
;		File Control Block contains next token.
;		A = Terminating Character.
BF$GT:
	MOV	A,M		;Get next string character
	ORA	A		;Check if end of string
	JZ	BF$SFT		;Yes, space fill token
	CPI	'*'		;Check if ?-fill required
	JZ	BF$QFT		;Yes, fill with ?
	CPI	'.'		;Assume current token is Filename
				;check if file type coming up
				;(If current token is File type this
				;check is benignly redundant)
	JZ	BF$SFT		;Yes, space fill token
	STAX	D		;None of the above, so store in FCB
	INX	D		;Update FCB Pointer
	INX	H		;Update String Pointer
	DCR	C		;Countdown on token length
	JNZ	BF$GT		;Still more characters to go
BF$SKIP:			;Skip chars. until '.' or 00H
	MOV	A,M		;Get next string character
	ORA	A		;Check if 00H
	RZ			;Yes
	CPI	'.'		;Check if '.'
	RZ			;Yes
	INX	H		;Update string pointer (only)
	JMP	BF$SKIP	;Try next character

BF$SFT:			;Space fill token
	MVI	B,' '
	JMP	BF$FT		;Common Fill Token code
				;BF$FT returns to caller

BF$QFT:			;Question Mark Fill Token
	MVI	B,'?'
	CALL	BF$FT		;Common Fill Token Code
	JMP	BF$SKIP	;Bypass multiple '*' etc.

BF$FT:				;Fill Token
	PUSH	PSW		;Save terminating character
	MOV	A,B		;Get fill characer
BF$FTL:			;Inner loop
	STAX	D		;Store in FCB
	INX	D		;Update FCB Pointer
	DCR	C		;Downdate residual count
	JNZ	BF$FTL		;Keep going
	POP	PSW		;Recover terminating character
	RET
