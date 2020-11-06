;	Figure 7-5.
;
;	Example Put CP/M
;
;	This program writes out the CP/M Cold Boot Loader, the
;	CCP, BDOS and BIOS to a floppy diskette. It runs under
;	CP/M as a normal Transient Program.
;
Version		EQU	'01'	;Equates used in the sign on message
Month		EQU	'07'
Day		EQU	'24'
Year		EQU	'82'
;
;
;	The actual PUTCPMF5.COM program consists of this code,
;	plus the BOOTF5.HEX, the CCP, BDOS and BIOS.
;
;	When this program executes, the memory image should 
;	look like this :
;
;	      Component      Base Address
;		BIOS		1F80H
;		BDOS		1180H
;		CCP		0980H
;		BOOTF5		0780H
;
;	The components are produced as follows :
;
;		BIOS.HEX	By assembling source code
;		BDOS )		From a CPMnn.COM file output
;		CCP  )		by MOVCPM and SAVEd on disk
;		BOOTF5.HEX	By assembling source code
;
;	The components are pieced together using DDT with the
;	following commands :
;
;		DDT CPMnn.COM
;		IPUTCPMF5.HEX
;		R			(Reads in this program)
;		IBOOTF5.HEX
;		R680			(Reads in Boot at 0780H)
;		IBIOS.HEX
;		R2980			(Reads in BIOS at 1F80H)
;		G0			(Exit from DDT)
;		SAVE 40 PUTCPMF5.COM	(Create final .COM file)
;
;	The actual layout of the diskette is as follows :
;								
; Track 0                     Sector
;         1     2     3     4     5     6     7     8     9	
; Head	+-----+-----+-----+-----+-----+-----+-----+-----+-----+	
;  0	|Boot |<======== CCP ========>|<=======  BDOS ========|	
;	+-----+-----+-----+-----+-----+-----+-----+-----+-----+	
;  1	|====== BDOS ====>|<=============  BIOS  ============>|	
;	+-----+-----+-----+-----+-----+-----+-----+-----+-----+	
;	  10    11    12    13    14    15    16    17    18
;                             Sector
;
;	Equates for defining memory size and the base address and
;	length of the system components.
;
Memory$Size	EQU	64	;Number of Kbytes of RAM
;
;	The BIOS Length must match that declared in the BIOS.
;
BIOS$Length	EQU	0900H
;
Boot$Length	EQU	512
CCP$Length	EQU	0800H	;Constant
BDOS$Length	EQU	0E00H	;Constant
;
Length$In$Bytes	EQU	CCP$Length + BDOS$Length + BIOS$Length
;
Start$Image	EQU	980H - Boot$Length	;Address of CP/M Image
Length$Image	EQU	Length$In$Bytes + Boot$Length 
;
;
;	Disk Characteristics
;
;	These equates describe the physical characteristics of
;	the floppy diskette so that the program can move from
;	one sector to the next, updating the track and resetting
;	the sector when necessary.
;
First$Sector$on$Track	EQU	1
Last$Sector$on$Track	EQU	18
Last$Sector$on$Head$0	EQU	9
Sector$Size		EQU	512
;
;
;	Controller Characteristics
;
;	On this computer system, the floppy disk controller can write
;	multiple sectors in a single command. However, in order to
;	to produce a more general example it is shown only reading one
;	sector at a time.
;
Sectors$Per$Write	EQU	1
;
;
;	Cold Boot Characteristics
;
Start$Track		EQU	0	;Initial values for CP/M image
Start$Sector		EQU	1	;= " =
Sectors$To$Write	EQU	(Length$Image + Sector$Size - 1) / Sector$Size
;

;
B$PRINTS	EQU	9	;Print string terminated by $
BDOS		EQU	5	;BDOS Entry Point
;
;

	ORG	100H
Put$CPM:
	JMP	Main$Code	;Enter main code body
				;For reasons of clarity, the main
				;  data structures are shown before the
				;  executable code.
CR	EQU	0DH		;Carriage Return
LF	EQU	0AH		;Line Feed
;
Signon$Message:
	DB	CR,LF,'Put CP/M on Diskette'
	DB	CR,LF
	DB	'Version '
	DW	Version
	DB	' '
	DW	Month
	DB	'/'
	DW	Day
	DB	'/'
	DW	Year
	DB	CR,LF,'$'

;
;	 Disk Control Tables
;
Disk$Control$5	EQU	45H	;5 1/4" Control Byte
Command$Block$5	EQU	46H	;Control Table Pointer
Disk$Status	EQU	43H	;Completion status
;
;
;	The command table Track and DMA$Address can also be used
;	as working storage and updated as the load process
;	continues. The Sector in the command table cannot be
;	used directly as the disk controller requires it to be
;	the sector number on the specified head (1 - 9) rather
;	than the sector number on track. Hence a separate variable
;	must be used.
;
Sector:		DB	Start$Sector
;
Command$Table:	DB	02H		;Command - Write
Unit:		DB	0		;Unit (Drive) number = 0 or 1
Head:		DB	0		;Head number = 0 or 1 
Track:		DB	Start$Track	;Used as working variable
Sector$on$head:	DB	0		;Converted by low-level driver
Byte$Count:	DW	Sector$Size * Sectors$Per$Write
DMA$Address:	DW	Start$Image
Next$Status:	DW	Disk$Status	;Pointer to next Status Block
					;  if commands are chained.
Next$Control:	DW	Disk$Control$5	;Pointer to next Control Byte
					;  if commands are chained.

Main$Code:
	LXI	SP,Put$CPM	;Stack grows down below code

	LXI	D,Signon$Message	;Sign on
	MVI	C,B$PRINTS		;Print string until $
	CALL	BDOS

	LXI	H,Command$Table		;Point the disk controller at
	SHLD	Command$Block$5		;  the command block

	MVI	C,Sectors$To$Write	;Set sector count
Write$Loop:
	CALL	Put$CPM$Write		;Write data onto diskette
	DCR	C			;Downdate sector count
	JZ	0			;Warm Boot

	LXI	H,Sector		;Update sector number
	MVI	A,Sectors$Per$Write	;  by adding on number of sectors
	ADD	M			;  by controller
	MOV	M,A			;Save result
	MVI	A,Last$Sector$On$Track + 1	;Check if at end of track
	CMP	M
	JNZ	Not$End$Track

	MVI	M,First$Sector$On$Track	;Yes, reset to beginning
	LHLD	Track			;Update Track number
	INX	H
	SHLD	Track

Not$End$Track:
	LHLD	DMA$Address		;Update DMA Address
	LXI	D,Sector$Size * Sectors$Per$Write
	DAD	D
	SHLD	DMA$Address
	JMP	Write$Loop		;Write next block
;
Put$CPM$Write:			;At this point, the description of the
				;  operation required is in the variables
				;  contained in the Command Table, along
				;  with the Sector variable.

	PUSH	B			;Save sector count in C

;------ Change this routine to match the disk controller in use ------

	MVI	B,0			;Assume head 0
	LDA	Sector			;Get requested sector
	MOV	C,A			;Take a copy of it
	CPI	Last$Sector$on$Head$0+1	;Check if on head 1
	JC	Head$0			;No
	SUI	Last$Sector$on$Head$0	;Bias down for head 1
	MOV	C,A			;Save copy
	INR	B			;Set head 1
Head$0:
	MOV	A,B			;Get head
	STA	Head
	MOV	A,C			;Get sector
	STA	Sector$On$Head

	LXI	H,Disk$Control$5	;Activate controller
	MVI	M,80H

Wait$For$Boot$Complete:
	MOV	A,M			;Get status byte		
	ORA	A			;Check if complete
	JNZ	Wait$For$Boot$Complete	;No
					;Yes, check for errors
	LDA	Disk$Status
	CPI	80H		
	JC 	Put$CPM$Error		;Yes, an error occurred

;------ End of Physical Write routine ------

	POP	B			;Recover sector count in C
	RET
;
Put$CPM$Error:
	LXI	D,Put$CPM$Error$Message
	MVI	C,B$PRINTS		;Print string until $
	CALL	BDOS			;Output error message
	JMP	Main$Code		;Restart the loader
;

Put$CPM$Error$Message:
	DB	CR,LF,'Error in writing CP/M - retrying...',CR,LF,'$'
	END	Put$CPM
