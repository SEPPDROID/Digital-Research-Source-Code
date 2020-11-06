;	Figure 7-7.
;
;	Example CP/M Cold Bootstrap Loader
;
;	This program is written out to Track 0, Head 0, Sector 1
;	by the PUTCPMF5 program.
;	It is loaded into memory at location 100H on up by the
;	PROM-based bootstrap mechanism that gets control of the
;	CPU on power-up or system reset.
;
Version		EQU	'01'	;Equates used in the sign on message
Month		EQU	'07'
Day		EQU	'24'
Year		EQU	'82'
;
Debug		EQU	0	;Set Non-zero to debug as normal
				;  transient program
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
CCP$Length	EQU	0800H	;Constant
BDOS$Length	EQU	0E00H	;Constant
;
Length$In$K	EQU	((CCP$Length + BDOS$Length + BIOS$Length) / 1024) + 1
Length$In$Bytes	EQU	CCP$Length + BDOS$Length + BIOS$Length
;
	IF	NOT Debug
CCP$Entry	EQU	(Memory$Size - Length$In$K) * 1024
	ENDIF
	IF	Debug
CCP$Entry	EQU	3980H	;Read into a lower address.
				;This address is chosen to be above
				;  the area into which DDT initially loads
				;  and the 980H makes the addresses similar
				;  to the SYSGEN values so that the memory
				;  image can be checked with DDT.
	ENDIF

BDOS$Entry	EQU	CCP$Entry + CCP$Length + 6	
BIOS$Entry	EQU	CCP$Entry + CCP$Length + BDOS$Length
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
;	On this computer system, the floppy disk controller can read
;	multiple sectors in a single command. However, in order to
;	to produce a more general example it is shown only reading one
;	sector at a time.
;
Sectors$Per$Read	EQU	1
;
;
;	Cold Boot Characteristics
;
Start$Track		EQU	0	;Initial values for CP/M image
Start$Sector		EQU	2	;= " =
Sectors$To$Read		EQU	(Length$In$Bytes + Sector$Size - 1) / Sector$Size
;
;
;

	ORG	100H
Cold$Boot$Loader:
	JMP	Main$Code	;Enter main code body
				;For reasons of clarity, the main
				;  data structures are shown before the
				;  executable code.
CR	EQU	0DH		;Carriage Return
LF	EQU	0AH		;Line Feed
;
Signon$Message:
	DB	CR,LF,'CP/M Bootstrap Loader'
	IF	Debug
	DB	' (Debug)'
	ENDIF
	DB	CR,LF
	DB	'Version '
	DW	Version
	DB	' '
	DW	Month
	DB	'/'
	DW	Day
	DB	'/'
	DW	Year
	DB	CR,LF,0

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
Command$Table:	DB	01H	;Command - Read
Unit:		DB	0	;Unit (Drive) number = 0 or 1
Head:		DB	0	;Head number = 0 or 1 
Track:		DB	Start$Track	;Used as working variable
Sector$on$head:	DB	0	;Converted by low-level driver
Byte$Count:	DW	Sector$Size * Sectors$Per$Read
DMA$Address:	DW	CCP$Entry
Next$Status:	DW	Disk$Status	;Pointer to next Status Block
					;  if commands are chained.
Next$Control:	DW	Disk$Control$5	;Pointer to next Control Byte
					;  if commands are chained.

Main$Code:
	LXI	SP,Cold$Boot$Loader	;Stack grows down below code

	LXI	H,Signon$Message	;Sign on
	CALL	Display$Message

	LXI	H,Command$Table		;Point the disk controller at
	SHLD	Command$Block$5		;  the command block

	MVI	C,Sectors$To$Read	;Set sector count
Load$Loop:
	CALL	Cold$Boot$Read		;Read data into memory
	DCR	C			;Downdate sector count

	IF	NOT Debug
	JZ	BIOS$Entry		;Enter BIOS when load done
	ENDIF
	IF	Debug
	JZ	0			;Warm Boot
	ENDIF

	LXI	H,Sector		;Update sector number
	MVI	A,Sectors$Per$Read	;  by adding on number of sectors
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
	LXI	D,Sector$Size * Sectors$Per$Read
	DAD	D
	SHLD	DMA$Address
	JMP	Load$Loop		;Read next block
;
Cold$Boot$Read:			;At this point, the description of the
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
	JC 	Cold$Boot$Error		;Yes, an error occurred

;------ End of Physical Read routine ------

	POP	B			;Recover sector count in C
	RET
;
Cold$Boot$Error:
	LXI	H,Cold$Boot$Error$Message
	CALL	Display$Message		;Output error message
	JMP	Main$Code		;Restart the loader
;

Cold$Boot$Error$Message:
	DB	CR,LF,'Bootstrap Loader Error - retrying...',CR,LF,0
;
;	Equates for Terminal Output
;
Terminal$Status$Port	EQU	01H
Terminal$Data$Port	EQU	02H
;
Terminal$Output$Ready	EQU	0000$0001B
;
;
Display$Message:	;Displays the specified message on the console.
			;On entry, HL points to a stream of bytes to be
			;output. A 00H-byte terminates the message.
	MOV	A,M		;Get next message byte
	ORA	A		;Check if terminator
	RZ			;Yes, return to caller
	MOV	C,A		;Prepare for output

Output$Not$Ready:
	IN	Terminal$Status$Port	;Check if ready for output
	ANI	Terminal$Output$Ready
	JZ	Output$Not$Ready	;No, wait
	MOV	A,C			;Get Data character
	OUT	Terminal$Data$Port	;Output to Screen

	INX	H		;Move to next byte of message
	JMP	Display$Message	;Loop until complete message output

				;The PROM-based bootstrap loader checks
				;  to see that the characters "CP/M"
				;  are on the diskette bootstrap sector
				;  before it transfers control to it.
	ORG	2E0H
	DB	'CP/M'
	END	Cold$Boot$Loader
