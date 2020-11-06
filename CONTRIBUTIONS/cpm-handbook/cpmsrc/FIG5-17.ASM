; Figure 5-17
;
;	GNF
;	This subroutine returns an FCB setup with either the first 
;	file matched by an ambiguous file name, or (if specified 
;	by entry parameter) the next file name.
;
;	Note : this subroutine is context sensitive. You must not
;	       have more than one ambiguous file name sequence in
;	       process at any given time.
;
;>>>	Warning : This subroutine changes the DMA Address inside
;>>>		  the BDOS.
;
;	Entry Parameters
;
;		DE -> Possibly Ambiguous File Name
;			(00-byte terminated)
;			(Only needed for FIRST request)
;		HL -> File Control Block
;		A = 0  : Return FIRST file name that matches
;		  = NZ : Return NEXT file name that matches
;
;	Exit Parameters
;
;		Carry set : A = FF, no file name matches
;			    A not = 0FFH, error in input file name
;		Carry clear : FCB setup with next name
;			      HL -> Directory Entry returned by
;			            Search First/Next
;
;	Calling sequence
;
;		LXI	D,FILENAME
;		LXI	H,FCB
;		MVI	A,0	;or MVI A,1 for NEXT
;		CALL	GNF
;
B$SEARCHF	EQU	17	;Search for First file name
B$SEARCHN	EQU	18	;Search for Next file name
B$SETDMA	EQU	26	;Setup DMA Address
BDOS		EQU	5	;BDOS Entry Point
;
GNFDMA	EQU	80H		;Default DMA Address
GNFSVL	EQU	13		;Save Length (no. of chars to move)
GNFFCL	EQU	36		;File Control Block length
GNFSV:	DS	GNFSVL		;Save area for file name/type
;
GNF:
	PUSH	H		;Save FCB pointer
	PUSH	D		;Save File Name pointer
	PUSH	PSW		;Save First/Next flag

	LXI	D,GNFDMA	;Set DMA to known address
	MVI	C,B$SETDMA	;Function code
	CALL	BDOS
	POP	PSW		;Recover First/Next flag
	POP	H		;Recover File Name pointer
	POP	D		;Recover FCB Pointer
	PUSH	D		;Re-save FCB Pointer

	ORA	A		;Check if FIRST or NEXT
	JNZ	GNFN		;NEXT
	CALL	BF		;Build File Control BLock
	POP	H		;Recover FCB Pointer (to balance stack)
	RC			;Return if error in File Name
	PUSH	H		;Resave FCB pointer

				;Move Ambiguous File Name to save area
				;HL -> FCB
	LXI	D,GNFSV		;DE -> save area
	MVI	C,GNFSVL	;Get save length
	CALL	MOVE
	POP	D		;Recover FCB Pointer
	PUSH	D		;and re-save

	MVI	C,B$SEARCHF	;Search FIRST
	CALL	BDOS
	POP	H		;Recover FCB Pointer
	CPI	0FFH		;Check for error
	JZ	GNFEX		;Error exit
	JMP	GNFC		;Common code
;
GNFN:
				;Execute search FIRST to re-establish
				;contact with previous file
				;User's FCB still has Name/Type in it
	CALL	GNFZF		;Zero-fill all but File Name/Type
	POP	D		;Recover FCB address
	PUSH	D		;and re-save
	MVI	C,B$SEARCHF	;Re-find the file
	CALL	BDOS
	POP	D		;Recover FCB Pointer
	PUSH	D		;and re-save
	LXI	H,GNFSV		;Move File Name from save area into FCB
	MVI	C,GNFSVL	;Save area length
	CALL	MOVE

	MVI	C,B$SEARCHN	;Search NEXT
	CALL	BDOS
	POP	H		;Recover FCB address
	CPI	0FFH		;Check for error
	JZ	GNFEX		;Error exit
GNFC:
	PUSH	H		;Save FCB Address
	ADD	A		;Multiply BDOS Return Code * 32
	ADD	A		;* 4
	ADD	A		;* 8
	ADD	A		;* 16
	ADD	A		;* 32
	LXI	H,GNFDMA	;HL -> DMA Address
	MOV	E,A		;Make Code * 32 a word value in DE
	MVI	D,0
	DAD	D		;HL -> File's directory entry

				;Move File Name into FCB
	POP	D		;Recover FCB Address
	PUSH	H		;Save Directory Entry pointer
	PUSH	D		;and re-save
	MVI	C,GNFSVL	;Length of save area
	CALL	MOVE
	LDA	GNFSV		;Get Disk Drive from save area
	POP	D		;Recover FCB Address
	STAX	D		;Overwrite user number in FCB

				;Setup to zero-fill tail end of FCB
	CALL	GNFZF		;Zero-fill
	POP	H		;Recover Directory Entry Pointer
	XRA	A		;Clear Carry
	RET
;
GNFEX:
	STC			;Set Carry to indicate error
	RET
	
;
;	GNFZF
;	Get Next File - Zero Fill
;	This subroutine Zero-fills the rest of the bytes in an FCB
;	that follow the File Name and Type.
;
;	Entry Parameters
;
;		DE -> File Control Block
;
GNFZF:
	LXI	H,GNFSVL	;Bypass area that holds file name
	DAD	D		;HL -> FCB + GNFSVL
	MOV	D,H		;DE -> FCB + GNFSVL
	MOV	E,L
	INX	D		;DE -> FCB + GNFSVL + 1
	MVI	M,0		;FCB + GNFSVL = 0
	MVI	C,GNFFCL-GNFSVL	;Remainder of File Control Block
;	Drop into MOVE		;Spread 0's through remainder of FCB
;	v
;
;	MOVE
;	This subroutine moves C bytes from (HL) to (DE).
;
MOVE:
	MOV	A,M		;Get source byte
	STAX	D		;Save destination byte
	INX	D		;Increment destination pointer
	INX	H		;Increment source pointer
	DCR	C		;Downdate count
	JNZ	MOVE		;Go back for more
	RET
;
;	BF
;	Build File Control Block
;
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
;	This subroutine is shown in full in Figure 5.32
;
BF:	RET		;Dummy subroutine for this example
