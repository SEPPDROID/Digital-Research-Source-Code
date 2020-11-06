; Figure 5-21
;
;	RF
;	Rename File
;	This subroutine renames a file.
;	It uses the BF (Build FCB) subroutine shown in Figure 5.32
;
;	Entry Parameters
;
;		*** No case-folding of file names occurs ***
;		HL -> Old File Name (00-byte terminated)
;		DE -> New File Name (00-byte terminated)
;
;	Exit Parameters
;
;		Carry clear if operation successful (A = 0,1,2,3)
;		Carry set if error (A =0FFH)
;
;	Calling sequence
;
;		LXI	H,OLDNAME	;HL -> Old Name
;		LXI	D,NEWNAME	;DE -> New Name
;		CALL	RF
;		JC	ERROR
;
B$OPEN		EQU	15		;Open File
B$RENAME	EQU	23		;Rename File
BDOS		EQU	5		;BDOS Entry Point
;
RFFCB:		DW	0,0,0,0,0,0,0,0,0	;1 1/2 FCB's long
		DW	0,0,0,0,0,0,0,0,0
		DW	0,0,0,0,0,0,0,0,0
;
;
RF:
		PUSH	D		;Save New Name pointer
		LXI	D,RFFCB		;Build Old Name FCB
					;HL already -> Old Name
		CALL	BF

		POP	H		;Recover New Name pointer
		LXI	D,RFFCB+16	;Build new name in second part
		CALL	BF		;of File Control Block

		LXI	D,RFFCB+16	;Experimentally try
		MVI	C,B$OPEN	;to Open the new File
		CALL	BDOS		;to ensure it does not
		CPI	0FFH		;already exist
		MVI	A,0FEH		;Assume error (flags unchanged)
		RC			;Carry set if A was 0,1,2,3

		LXI	D,RFFCB		;Rename the file
		MVI	C,B$RENAME
		CALL	BDOS
		CPI	0FFH		;Carry set if OK, clear if error
		CMC			;Invert to use Carry set if error
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
	RET		;Dummy subroutine : see Figure 5.32.
