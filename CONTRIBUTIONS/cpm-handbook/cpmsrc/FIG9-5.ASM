;	Figure 9-5
;
;	This shows slightly more user-friendly error processor
;	for disk errors than that shown in the enhanced BIOS
;	in Figure 8-10. 
;	This version outputs a recommended course of action 
;	depending on the nature of the error detected.
;	Code that remains unchanged from Figure 8-10 has been
;	abbreviated.
;
;	Dummy Equates and data declarations needed to get
;	an error free assembly of this example.
;
Floppy$Read$Code	EQU	01H	;Read Command for controller
Floppy$Write$Code	EQU	02H	;Write Command for controller
;
Disk$Hung$Flag:		DB	0	;Set NZ when Watchdog timer times
					;  out.
Disk$Timer		EQU	600	;10 seconds delay (16.66ms tick)
;
Disk$Status$Block	EQU	43H	;Address in memory where controller
					;  returns status
					;Values from controller command table
Floppy$Command:		DB	0
Floppy$Head:		DB	0
Floppy$Track:		DB	0
Floppy$Sector:		DB	0

Deblocking$Required:	DB	0	;Flag set by SELDSK according
					;  to selected disk type

Disk$Error$Flag:	DB	0	;Error flag returned to BDOS
;
In$Buffer$Disk:		DB	0	;Logical disk Id. relating to current
					;  disk sector in deblocking buffer
;
;	Equates for Messages
;
BELL	EQU	07H	;Sound terminal Bell
CR	EQU	0DH	;Carriage Return
LF	EQU	0AH	;Line Feed
;
BDOS	EQU	5	;BDOS Entry Point (for system reset)
;
;
;
;
No$Deblock$Retry:
	;----------------------------------------------------
	; Omitted code to setup disk controller command table
	; and initiate the disk operation
	;----------------------------------------------------
	JMP	Wait$For$Disk$Complete
;
;
Write$Physical:			;Write contents of Disk Buffer to
				;  correct sector.
	MVI	A,Floppy$Write$Code	;Get Write Function code
	JMP	Common$Physical	;Go to common code
Read$Physical:			;Read previously selected Sector
				;  into Disk Buffer.
	MVI	A,Floppy$Read$Code	;Get Read Function code
Common$Physical:
	STA	Floppy$Command	;Set command table

;
Deblock$Retry:			;Re-entry point to re-try after error
	;---------------------------------------------------
	; Omitted code sets up disk controller command block
	; and initiates the disk operation
	;---------------------------------------------------
;
Wait$For$Disk$Complete:		;Wait until Disk Status Block indicates
				;  operation has completed, then check
				;  if any errors occurred.
				;On entry HL -> Disk Control byte
	XRA	A			;Ensure hung flag clear
	STA	Disk$Hung$Flag

	LXI	H,Disk$Timed$Out	;Setup Watchdog timer
	LXI	B,Disk$Timer		;Time delay
	CALL	Set$Watchdog
Disk$Wait$Loop:
	MOV	A,M			;Get control byte
	ORA	A
	JZ	Disk$Complete		;Operation done

	LDA	Disk$Hung$Flag		;Also check if timed out
	ORA	A
	JNZ	Disk$Error		;Will be set to 40H

	JMP	Disk$Wait$Loop

Disk$Timed$Out:			;Control arrives here from Watchdog
				;  routine itself - so this is effectively
				;  part of the interrupt service routine.
	MVI	A,40H			;Set Disk Hung error code
	STA	Disk$Hung$Flag		;  into error flag to pull
					;  control out of loop
	RET				;Return to Watchdog routine

Disk$Complete:
	LXI	B,0			;Reset Watchdog timer
					;HL is irrelevant here
	CALL	Set$Watchdog

	LDA	Disk$Status$Block	;Complete - now check status
	CPI	80H			;Check if any errors occurred
	JC	Disk$Error		;Yes
;
Disk$Error$Ignore:
	XRA	A			;No
	STA	Disk$Error$Flag		;Clear error flag
	RET

;
;	Disk Error Message handling
;
;
Disk$Error$Messages:		;This table is scanned, comparing the
				;  Disk Error Status with those in the
				;  table. Given a match, or even when
				;  then end of the table is reached, the
				;  address following the status value
				;  points to the correct advisory message text.
				;  Following this is the address of an
				;  error description message.
	DB	40H
	DW	Disk$Advice1,Disk$Msg$40
	DB	41H
	DW	Disk$Advice2,Disk$Msg$41
	DB	42H
	DW	Disk$Advice3,Disk$Msg$42
	DB	21H
	DW	Disk$Advice4,Disk$Msg$21
	DB	22H
	DW	Disk$Advice5,Disk$Msg$22
	DB	23H
	DW	Disk$Advice5,Disk$Msg$23
	DB	24H
	DW	Disk$Advice6,Disk$Msg$24
	DB	25H
	DW	Disk$Advice6,Disk$Msg$25
	DB	11H
	DW	Disk$Advice7,Disk$Msg$11
	DB	12H
	DW	Disk$Advice7,Disk$Msg$12
	DB	13H
	DW	Disk$Advice7,Disk$Msg$13
	DB	14H
	DW	Disk$Advice7,Disk$Msg$14
	DB	15H
	DW	Disk$Advice7,Disk$Msg$15
	DB	16H
	DW	Disk$Advice7,Disk$Msg$16
	DB	0			;<== Terminator
	DW	Disk$Advice7,Disk$Msg$Unknown	;Unmatched code
;
DEM$Entry$Size	EQU	5	;Entry size in Error Message Table
;
;
;	Message Texts
;
Disk$Msg$40:	DB	'Hung',0	;Timeout message
Disk$Msg$41:	DB	'Not Ready',0
Disk$Msg$42:	DB	'Write Protected',0
Disk$Msg$21:	DB	'Data',0
Disk$Msg$22:	DB	'Format',0
Disk$Msg$23:	DB	'Missing Data Mark',0
Disk$Msg$24:	DB	'Bus Timeout',0
Disk$Msg$25:	DB	'Controller Timeout',0
Disk$Msg$11:	DB	'Drive Address',0
Disk$Msg$12:	DB	'Head Address',0
Disk$Msg$13:	DB	'Track Address',0
Disk$Msg$14:	DB	'Sector Address',0
Disk$Msg$15:	DB	'Bus Address',0
Disk$Msg$16:	DB	'Illegal Command',0
Disk$Msg$Unknown:	DB	'Unknown',0
;
Disk$EM$1:			;Main disk error message - part 1
		DB	BELL,CR,LF
		DB	'Disk ',0
;
				;Error Text output next
;
Disk$EM$2:			;Main disk error message - part 2
		DB	' Error ('
Disk$EM$Status:	DB	0,0	;Status code in Hex
		DB	')',CR,LF,'     Drive '
Disk$EM$Drive:	DB	0	;Disk Drive code, A,B...
		DB	', Head '
Disk$EM$Head:	DB	0	;Head number
		DB	', Track '
Disk$EM$Track:	DB	0,0	;Track number
		DB	', Sector '
Disk$EM$Sector:	DB	0,0	;Sector number
		DB	', Operation - '
		DB	0		;Terminator
;
Disk$EM$Read:	DB	'Read.',0	;Operation names
Disk$EM$Write:	DB	'Write.',0
;
Disk$Advice0:	DB	CR,LF,'     ',0
Disk$Advice1:	DB	'Check disk loaded, Retry',0
Disk$Advice2:	DB	'Possible hardware problem',0
Disk$Advice3:	DB	'Write enable if correct disk, Retry',0
Disk$Advice4:	DB	'Retry several times',0
Disk$Advice5:	DB	'Reformat disk or use another disk',0
Disk$Advice6:	DB	'Hardware error, Retry',0
Disk$Advice7:	DB	'Hardware or Software error, Retry',0
;
Disk$Advice9:	DB	', or call for help if error persists',CR,LF
;
Disk$Action$Confirm:
		DB	0	;Set to character entered by user
		DB	CR,LF,0
;
;	Disk Error Processor
;
;	This routine builds and outputs an error message.
;	The user is then given the opportunity to :
;
;		R - Retry the operation that caused the error.
;		I - Ignore the error and attempt to continue.
;		A - Abort the program and return to CP/M.
;
Disk$Error:
	PUSH	PSW		;Preserve error code from controller
	LXI	H,Disk$EM$Status	;Convert code for message
	CALL	CAH			;Converts A to hex

	LDA	In$Buffer$Disk		;Convert disk id. for message
	ADI	'A'			;Make into letter
	STA	Disk$EM$Drive

	LDA	Floppy$Head		;Convert head number
	ADI	'0'
	STA	Disk$EM$Head

	LDA	Floppy$Track		;Convert track number
	LXI	H,Disk$EM$Track
	CALL	CAH

	LDA	Floppy$Sector		;Convert sector number
	LXI	H,Disk$EM$Sector
	CALL	CAH

	LXI	H,Disk$EM$1		;Output first part of message
	CALL	Output$Error$Message

	POP	PSW			;Recover error status code
	MOV	B,A			;For comparisons
	LXI	H,Disk$Error$Messages - DEM$Entry$Size
					;HL -> Table - one entry
	LXI	D,DEM$Entry$Size	;For loop below
Disk$Error$Next$Code:
	DAD	D			;Move to next (or first) entry

	MOV	A,M			;Get code number from table
	ORA	A			;Check if end of table
	JZ	Disk$Error$Matched	;Yes - pretend a match occurred
	CMP	B			;Compare to actual code
	JZ	Disk$Error$Matched	;Yes - exit from loop
	JMP	Disk$Error$Next$Code	;Check next code
;
Disk$Error$Matched:
	INX	H			;HL -> Advisory text address
	MOV	E,M
	INX	H
	MOV	D,M			;DE -> Advisory test
	PUSH	D			;Save for later
	
	INX	H			;HL -> Message text address
	MOV	E,M			;Get Address into DE
	INX	H
	MOV	D,M

	XCHG				;HL -> Text
	CALL	Output$Error$Message	;Display explanatory text

	LXI	H,Disk$EM$2		;Display second part of message
	CALL	Output$Error$Message

	LXI	H,Disk$EM$Read		;Choose operation text
					;  (assume a read)
	LDA	Floppy$Command		;Get controller command
	CPI	Floppy$Read$Code
	JZ	Disk$Error$Read		;Yes
	LXI	H,Disk$EM$Write		;No - change address in HL
Disk$Error$Read:
	CALL	Output$Error$Message	;Display operation type

	LXI	H,Disk$Advice0		;Display leading blanks
	CALL	Output$Error$Message

	POP	H			;Recover Advisory text pointer
	CALL	Output$Error$Message

	LXI	H,Disk$Advice9		;Display trailing component
	CALL	Output$Error$Message
;
Disk$Error$Request$Action:		;Ask the user what to do next
	CALL	Request$User$Choice	;Display prompt and get single
					;  character response (folded to 
					;  upper case)
	CPI	'R'			;Retry?
	JZ	Disk$Error$Retry
	CPI	'A'			;Abort
	JZ	System$Reset
	CPI	'I'			;Ignore
	JZ	Disk$Error$Ignore
	JMP	Disk$Error$Request$Action
;
Disk$Error$Retry:			;The decision on where to return to
					;  depends on whether the operation
					;  failed on a deblocked or
					;  non-deblocked drive
	LDA	Deblocking$Required
	ORA	A
	JNZ	Deblock$Retry
	JMP	No$Deblock$Retry
;
System$Reset:				;This is a radical approach, but
					;  it does cause CP/M to restart
	MVI	C,0			;System Reset
	CALL	BDOS

;
;	Omitted subroutines (listed in full in Figure 8-10)
;
Set$Watchdog:		;Set Watchdog timer (to number of "ticks" in BC, and
			;  to transfer control to (HL) if timer hits zero).
CAH:			;Convert A to two ASCII hex characters, storing
			;  the output in (HL) and (HL+1)
Output$Error$Message:	;Display the 00-byte terminated error message 
			;  pointed to by HL. Output is directed only to
			;  those console devices not being used for list
			;  output as well.
Request$User$Choice:	;Display prompt "Enter R, A, I..." and return
			;  single keyboard character (upper case) in A
	RET		;Dummy
