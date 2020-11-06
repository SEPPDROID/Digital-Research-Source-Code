;	Figure 10-2
;	Debug Subroutines
;
;<----	NOTE :
;	The line numbers at the extreme left are included purely
;	to reference the code from the text.
;	There are deliberately induced discontinuities
;	in the numbers in order to allow space for expansion.
;
;	Because of the need to test these routines thoroughly,
;	and in case you wish to make any changes, the testbed
;	routines for the debug package itself has been left in
;	in this figure.
;
;	Debug Testbed.
;
	ORG	100H
START:
	LXI	SP,Test$Stack		;Setup local stack
	CALL	DB$Init			;Initialize the debug package
	CALL	DB$On			;Enable Debug Output
					;Simple test of A-register display
	MVI	A,0AAH			;Preset a value in the A register
	LXI	B,0BBCCH		;Prefill all other registers, partly
	LXI	D,0DDEEH		;  to check the debug display, but
	LXI	H,0FF11H		;  also to check register save/restore
;#
;	Test register display
;
	ORA	A			;Set M-flag, Clear Z-flag, Set E-Flag
	STC				;Set Carry
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$F
	DB	'Flags',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$A
	DB	'A Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$B
	DB	'B Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$C
	DB	'C Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$D
	DB	'D Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$E
	DB	'E Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$H
	DB	'H Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$L
	DB	'L Register',0
;#
;	Test Memory Dump Display
;
	CALL	DB$Display		
	DB	DB$M			;Dump memory
	DW	108H,128H		;Check start/end at non-multiples
	DB	'Memory Dump #1',0	;  of 10H.
;
	CALL	DB$Display		
	DB	DB$M			;Dump memory
	DW	100H,11FH		;Check start and end on displayed
	DB	'Memory Dump #2',0	;  line boundaries.
;
	CALL	DB$Display		
	DB	DB$M			;Dump memory
	DW	101H,100H		;Check error handling where
	DB	'Memory Dump #3',0	;  start > end address.
;
	CALL	DB$Display		
	DB	DB$M			;Dump memory
	DW	100H,100H		;Check end-case of single byte
	DB	'Memory Dump #4',0	;  output.
;#
;	Test register pair display
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$BC
	DB	'BC Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$DE
	DB	'DE Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$HL
	DB	'HL Register',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$SP
	DB	'SP Register',0
;
	LXI	B,Byte$BC		;Set up registers for byte tests
	LXI	D,Byte$DE
	LXI	H,Byte$HL
;#
;	Test byte indirect display
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$B$BC
	DB	'Byte at (BC)',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$B$DE
	DB	'Byte at (DE)',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$B$HL
	DB	'Byte at (HL)',0
;
	LXI	B,Word$BC		;Set up the registers for word tests
	LXI	D,Word$DE
	LXI	H,Word$HL

	CALL	DB$Display		;Call the Debug Routine
	DB	DB$W$BC
	DB	'Word at (BC)',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$W$DE
	DB	'Word at (DE)',0
;
	CALL	DB$Display		;Call the Debug Routine
	DB	DB$W$HL
	DB	'Word at (HL)',0
;#
;	Test DB$On/Off
;
	CALL	DB$Off			;Disable Debug Output
	CALL	DB$MSGI			;Display in-line message
	DB	0DH,0AH,'This message should NOT appear',0

	CALL	DB$On
	CALL	DB$MSGI
	DB	0DH,0AH,'Debug output has been re-enabled.',0
;#
;	Test pass count logic
;
	CALL	DB$Off			;Disable debug output
	CALL	DB$Set$Pass		;Set Pass count
	DW	30
;
	MVI	A,34			;Set loop counter greater than pass
					;  counter.
Test$Pass$Loop:
	CALL	DB$Pass			;Decrement pass count
	CALL	DB$MSGI			;Display in-line message
	DB	0DH,0AH,'This message should display 5 times',0
	DCR	A
	JNZ	Test$Pass$Loop
;#
;	Test debug input/output
;
	CALL	DB$Off			;Check that debug IN/OUT
					;  must still occur when debug
					;  output is disabled.
	RST	4			;Debug input
	DB	11H			;Port number
	RST	5			;Debug output (value return from input)
	DB	22H			;Port number

	JMP	0			;Warm Boot at end of testbed
;
;
;	Dummy values for Byte and Word displays
Byte$BC:	DB	0BCH
Byte$DE:	DB	0DEH
Byte$HL:	DB	0F1H
;
Word$BC:	DW	0B0CH
Word$DE:	DW	0D0EH
Word$HL:	DW	0F01H
;
		DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
		DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
		DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
Test$Stack:
;
;
;
	ORG	400H			;To avoid unnecessary listings
					;  when only the testbed changes
;-----------------------------------------------------------------------
;#
;
;	Debug Subroutines
;
;
;	Equates for DB$Display Codes
;	These equates are the offsets down the table of addresses
;	for various subroutines to be used.
;
DB$F	EQU	00	;Flags
DB$A	EQU	02	;A register
DB$B	EQU	04	;B
DB$C	EQU	06	;C
DB$D	EQU	08	;D
DB$E	EQU	10	;E
DB$H	EQU	12	;H
DB$L	EQU	14	;L
DB$BC	EQU	16	;BC
DB$DE	EQU	18	;DE
DB$HL	EQU	20	;HL
DB$SP	EQU	22	;Stack Pointer
DB$M	EQU	24	;Memory
DB$B$BC	EQU	26	;(BC)
DB$B$DE	EQU	28	;(DE)
DB$B$HL	EQU	30	;(HL)
DB$W$BC	EQU	32	;(BC+1),(BC)
DB$W$DE	EQU	34	;(DE+1),(DE)
DB$W$HL	EQU	36	;(HL+1),(HL)
;
;
;	Equates
RST4		EQU	20H	;Address for RST 4 - IN instruction
RST5		EQU	28H	;Address for RST 5 - OUT instruction
;
B$CONIN		EQU	1	;BDOS CONIN Function Code
B$CONOUT	EQU	2	;BDOS CONOUT Function Code
B$READCONS	EQU	10	;BDOS Read Console Function Code 
BDOS		EQU	5	;BDOS Entry Point
;
False		EQU	0
True		EQU	NOT False
;
					;Equates to specify how DB$CONOUT
					;  and DB$CONIN should perform
					;  their Input/Output.
DB$Polled$IO	EQU	False		;)
DB$BIOS$IO	EQU	False		;) Only one MUST be true
DB$BDOS$IO	EQU	True		;)
;
					;Equates for Polled I/O
DB$Status$Port	EQU	01H		;Console status port
DB$Data$Port	EQU	02H		;Console data port
;
DB$Input$Ready	EQU	0000$0010B	;Incoming data ready
DB$Output$Ready	EQU	0000$0001B	;Ready for output
;
					;Data for BIOS I/O
BIOS$CONIN:	DB	JMP		;The initialization routine sets these
		DW	0		;  two JMP addresses into the BIOS
BIOS$CONOUT:	DB	JMP
		DW	0
;
;	Main debug variables and constants
;
DB$Flag:	DB	0	;Main Debug Control Flag.
				;  When this flag is non-zero, all debug
				;  output will be made. When zero, all
				;  debug output will be suppressed.
				;  It is altered either directly by the user
				;  or using the routines DB$On, DB$Off and 
				;  DB$Pass.
;
DB$Pass$Count:	DW	0	;Pass counter.
				;  When this is non-zero, calls to DB$Pass
				;  decrement it by one. When it reaches
				;  zero, the debug control flag, DB$Flag,
				;  is set non-zero - thereby enabling
				;  debug output.
;
DB$Save$HL:			;Save area for HL
DB$Save$L:	DB	0
DB$Save$H:	DB	0

DB$Save$SP:	DW	0	;Save area for Stack Pointer
DB$Save$RA:	DW	0	;Save area for Return Address
DB$Call$Address:	DW	0	;Starts out the same as DB$Save$RA
				;  but DB$Save$RA gets updated during
				;  debug processing. This value is
				;  output ahead of the caption.
DB$Start$Address:		;Start address for memory display
		DW	0
DB$End$Address:			;End address for memory display
		DW	0
DB$Display$Code:		;Display code requested
		DB	0
;
;
				;Stack Area
		DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
		DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
		DW	9999H,9999H,9999H,9999H,9999H,9999H,9999H,9999H
DB$Save$E:	DB	0	;E Register
DB$Save$D:	DB	0	;D Register
DB$Save$C:	DB	0	;C Register
DB$Save$B:	DB	0	;B Register
DB$Save$F:	DB	0	;Flags
DB$Save$A:	DB	0	;A Register
DB$Stack:			;Debug stack area
				;  The registers in the stack area are PUSHed
				;  onto the stack and accessed directly.
;
;	Register Caption Messages
;
;	The table below, indexed by the Display$Code is used to access
;	the Register Caption String.
;
DB$Register$Captions:
	DW	DB$F$RC		;Flags
	DW	DB$A$RC		;A register
	DW	DB$B$RC		;B
	DW	DB$C$RC		;C
	DW	DB$D$RC		;D
	DW	DB$E$RC		;E
	DW	DB$H$RC		;H
	DW	DB$L$RC		;L
	DW	DB$BC$RC	;BC
	DW	DB$DE$RC	;DE
	DW	DB$HL$RC	;HL
	DW	DB$SP$RC	;Stack Pointer
	DW	DB$M$RC		;Memory
	DW	DB$B$BC$RC	;(BC)
	DW	DB$B$DE$RC	;(DE)
	DW	DB$B$HL$RC	;(HL)
	DW	DB$W$BC$RC	;(BC+1),(BC)
	DW	DB$W$DE$RC	;(DE+1),(DE)
	DW	DB$W$HL$RC	;(HL+1),(HL)
;
DB$F$RC:	DB	'Flags',0	;Flags
DB$A$RC:	DB	'A',0		;A register
DB$B$RC:	DB	'B',0		;B
DB$C$RC:	DB	'C',0		;C
DB$D$RC:	DB	'D',0		;D
DB$E$RC:	DB	'E',0		;E
DB$H$RC:	DB	'H',0		;H
DB$L$RC:	DB	'L',0		;L
DB$BC$RC:	DB	'BC',0		;BC
DB$DE$RC:	DB	'DE',0		;DE
DB$HL$RC:	DB	'HL',0		;HL
DB$SP$RC:	DB	'SP',0		;Stack Pointer
DB$M$RC:	DB	'Start, End Address ',0	;Memory
DB$B$BC$RC:	DB	'(BC)',0	;(BC)
DB$B$DE$RC:	DB	'(DE)',0	;(DE)
DB$B$HL$RC:	DB	'(HL)',0	;(HL)
DB$W$BC$RC:	DB	'(BC+1),(BC)',0	;(BC+1),(BC)
DB$W$DE$RC:	DB	'(DE+1),(DE)',0	;(DE+1),(DE)
DB$W$HL$RC:	DB	'(HL+1),(HL)',0	;(HL+1),(HL)
;
;	Flags Message
;
DB$Flags$Msg:	DB	'CxZxMxExIx',0	;Compatible with DDT's display
;
;	Flags Masks used to test user's flag byte
;
DB$Flag$Masks:
		DB	0000$0001B	;Carry
		DB	0100$0000B	;Zero
		DB	1000$0000B	;Minus
		DB	0000$0100B	;Even Parity
		DB	0001$0000B	;Interdigit Carry (Aux Carry)
		DB	0		;Terminator
;#
;	DB$Init
;	This routine initializes the debug package.
;
DB$Init:
	IF	DB$BIOS$IO		;Use BIOS for CONIN/CONOUT
	LHLD	1			;Get Warm Boot address from base
					;  page. H = BIOS Jump Vector page
	MVI	L,09H			;Get CONIN offset in Jump Vector
	SHLD	BIOS$CONIN + 1		;Setup address
	MVI	L,0CH			;Get CONOUT offset in Jump Vector
	SHLD	BIOS$CONOUT + 1
	ENDIF

			;Setup JMP instructions to receive control
			;  when an RST instruction is executed
	MVI	A,JMP		;Set JMP instructions at RST points
	STA	RST4
	STA	RST5
	LXI	H,DB$Input	;Address of Fake Input Routine
	SHLD	RST4 + 1
	LXI	H,DB$Output	;Address of Fake Output Routine
	SHLD	RST5 + 1

	RET
;#
;	DB$CONINU
;	This routine returns the next character from the console,
;	but converting 'a' to 'z' to UPPER CASE letters.
;
DB$CONINU:
	CALL	DB$CONIN		;Get character from keyboard
	JMP	DB$A$To$Upper		;Fold to upper and return
;#
;	DB$CONIN
;	This routine returns the next character from the console.
;	According to the setting of EQUates, it uses either simple
;	polled I/O, the BDOS (function 2) or the BIOS.
;
;	Exit Parameters
;
;		A = Character from console
;
DB$CONIN:
	IF	DB$Polled$IO		;Simple polled input
	IN	DB$Status$Port		;Check if incoming data
	ANI	DB$Input$Ready
	JZ	DB$CONIN		;No
	IN	DB$Data$Port		;Input data character
	PUSH	PSW			;Save data character
	MOV	C,A			;Ready for output
	CALL	DB$CONOUT		;Echo it back
	POP	PSW			;Recover data character
	RET
	ENDIF

	IF	DB$BDOS$IO		;Use BDOS for input
	MVI	C,B$CONIN		;Read console
	JMP	BDOS			;BDOS returns to our caller
	ENDIF

	IF	DB$BIOS$IO		;Use BIOS for input
	JMP	BIOS$CONIN		;This was setup during BIOS
					;  initialization
	ENDIF

;#
;	DB$CONOUT
;	This routine outputs the character in the C register to the
;	Console, either using simple polled I/O, the BDOS or the BIOS.
;
;	Entry Parameters
;		A = Byte to be output
;
DB$CONOUT:
	LDA	DB$Flag			;Check if debug output enabled
	ORA	A
	RZ				;Ignore output if disabled

	IF	DB$Polled$IO		;Use simple polled output
	IN	DB$Status$Port		;Check if ready for output
	ANI	DB$Output$Ready
	JZ	DB$CONOUT		;No
	MOV	A,C			;Get data byte
	OUT	DB$Data$Port
	RET
	ENDIF

	IF	DB$BDOS$IO		;Use BDOS for output
	MOV	E,C			;Move into correct register
	MVI	C,B$CONOUT
	JMP	BDOS			;BDOS returns to our caller
	ENDIF

	IF	DB$BIOS$IO		;Use BIOS for output
	MOV	A,C			;Move into correct register
	JMP	BIOS$CONOUT		;Setup during debug initialization
	ENDIF
;#
;
;	DB$On
;	This routine enables all debug output by setting the
;	DB$Flag non-zero.
;
DB$On:
	PUSH	PSW			;Preserve registers
	MVI	A,0FFH
	STA	DB$Flag			;Set control flag on
	POP	PSW
	RET
;#
;
;	DB$Off
;	This routine disables all debug output by setting the
;	DB$Flag to zero.
;
DB$Off:
	PUSH	PSW			;Preserve registers
	XRA	A
	STA	DB$Flag			;Clear control flag
	POP	PSW
	RET
;#
;
;	DB$Set$Pass
;	This routine sets the Pass Counter. Subsequent calls to DB$Pass
;	decrement the count, and when it reaches 0, debug output
;	is enabled.
;
;	Calling sequence
;
;		CALL	DB$Set$Pass
;		DW	Pass$Count$Value
;
DB$Set$Pass:
	SHLD	DB$Save$HL		;Preserve user's HL
	POP	H			;Recover return address
	PUSH	D			;Preserve user's DE
	MOV	E,M			;Get LS byte of count
	INX	H			;Update pointer
	MOV	D,M			;Get MS byte
	INX	H			;HL points to return address
	XCHG				;HL = pass counter
	SHLD	DB$Pass$Count		;Set Debug pass counter
	XCHG				;HL points to return address
	POP	D			;Recover user's DE
	XTHL				;Recover user's HL and set 
					;  return address on top of stack
	RET
;#
;
;	DB$Pass
;	This routine decrements the Debug pass counter -
;	if the result is negative, it takes no further action.
;	If the result is zero, it sets the Debug control flag non-zero
;	to enable debug output.
;
DB$Pass:
	PUSH	PSW			;Save user's registers
	PUSH	H
	LHLD	DB$Pass$Count		;Get pass count
	DCX	H
	MOV	A,H			;Check if count now negative
	ORA	A
	JM	DB$Pass$x		;Yes - take no further action
	SHLD	DB$Pass$Count		;Save downdated count
	ORA	L			;Check if count now zero
	JZ	DB$Pass$ED		;Yes - enable debug
DB$Pass$x:				;
	POP	H			;Recover user's registers
	POP	PSW
	RET
;
DB$Pass$Ed:				;Enable debug
	MVI	A,0FFH
	STA	DB$Flag			;Set debug control flag
	JMP	DB$Pass$x
;#
;
;	DB$Display
;	This is the primary debug display routine.
;
;	Calling sequence
;
;		CALL	DB$Display
;		DB	Display$Code
;		DB	'Caption String',0
;
;		Display code identifies which register(s) are to be
;		displayed.
;
;		When the display code specifies a block of memory
;		the sequence is:
;
;		CALL	DB$Display
;		DB	Display$Code
;		DW	Start$Address,End$Address
;		DB	'Caption String',0
;
DB$Display:
;
DB$Display$Enabled:
	SHLD	DB$Save$HL		;Save user's HL

	XTHL				;Get return address from stack
	SHLD	DB$Save$RA		;This gets updated by debug code
	PUSH	H			;Save return address temporarily
	DCX	H			;Subtract 3 to address CALL instruction
	DCX	H			;  itself
	DCX	H
	SHLD	DB$Call$Address		;Save actual address of CALL
	POP	H			;Recover return address

	PUSH	PSW			;Temporarily save flags to avoid
					;  them being changed by DAD SP
	LXI	H,0			;Preserve Stack Pointer
	DAD	SP
	INX	H			;Correct for extra PUSH PSW needed
	INX	H			;  to save the flags
	SHLD	DB$Save$SP
	POP	PSW			;Recover flags

	LXI	SP,DB$Stack		;Switch to local stack
	
	PUSH	PSW			;Save other user's registers
	PUSH	B			;The stack area is specially laid
	PUSH	D			;  out to access these registers

	LHLD	DB$Save$RA		;Get return address
	MOV	A,M			;Get display code
	STA	DB$Display$Code		
	INX	H			;Update return address

	CPI	DB$M			;Check if memory to be displayed
	JNZ	DB$Not$Memory
	MOV	E,M			;Get DE = start address
	INX	H
	MOV	D,M
	INX	H
	XCHG				;HL = start address
	SHLD	DB$Start$Address
	XCHG				;HL -> end address
	MOV	E,M			;Get DE = end address
	INX	H
	MOV	D,M
	INX	H
	XCHG				;HL = end address, DE -> Caption
	SHLD	DB$End$Address
	XCHG				;HL -> caption string
DB$Not$Memory:
;
;	Output Preamble and Caption String
;  	The format for everything except memory display is:
;
;	nnnn : Caption String : RC = vvvv
; 	 ^                      ^    ^
; 	Call Address            |    Value
;                        Register Caption (A, B, C...)
;
; 	A Carriage Return, Line Feed is output at the start of the
; 	message - but NOT at the end.
;
; 	Memory displays look like :
;
; nnnn : Caption String : Start, End  ssss, eeee
; ssss : hh hh hh hh hh hh hh hh hh hh hh hh hh hh hh hh : cccc cccc cccc cccc
;

	PUSH	H			;Save pointer to caption string
	CALL	DB$CRLF			;Display Carriage Return, Line Feed
	CALL	DB$Display$CALLA	;Display DB$Call$Address in hex.

	POP	H			;Recover pointer to caption string
DB$Display$Caption:			;HL -> caption string
	MOV	A,M			;Get character
	INX	H
	ORA	A			;Check if end of string
	JZ	DB$End$Caption		;Yes

	PUSH	H			;Save string pointer
	MOV	C,A			;Ready for output
	CALL	DB$CONOUT		;Display character
	POP	H			;Recover string pointer
	JMP	DB$Display$Caption	;Go back for next character
;
DB$End$Caption:
	SHLD	DB$Save$RA		;Save updated return address

	CALL	DB$Colon		;Display ' : '

					;Display register caption
	LDA	DB$Display$Code		;Get user's display code
	MOV	E,A			;Make display code into word
	MVI	D,0
	PUSH	D			;Save word value for later

	CPI	DB$M			;Memory display is a special case
	JZ	DB$Display$Mem$Caption	;Yes

	LXI	H,DB$Register$Captions	;Make pointer to address in table
	DAD	D			;HL -> Word containing address of
					;  register caption
	MOV	E,M			;Get LS byte of address
	INX	H
	MOV	D,M			;DE -> register caption string
	XCHG				;HL -> register caption string
	CALL	DB$MSG			;Display message addressed by HL

	CALL	DB$MSGI			;Display in-line message
	DB	' = ',0
	JMP	DB$Select$Routine	;Go to correct processor
;
DB$Display$Mem$Caption:			;The Memory Display requires a special
					;  caption with the start and end 
					;  addresses
	LXI	H,DB$M$RC		;Display specific caption
	CALL	DB$MSG
	CALL	DB$Colon		;Display ' : '

	LHLD	DB$Start$Address	;Display start address
	CALL	DB$DHLH			;Display HL in Hex.

	CALL	DB$MSGI			;Display in-line message
	DB	', ',0

	LHLD	DB$End$Address		;Get end address
	CALL	DB$DHLH			;Display HL in Hex.
	CALL	DB$CRLF			;Display Carriage Return, Line Feed
					;Drop into select routine
DB$Select$Routine:
	POP	D			;Recover word value Display$Code
	LXI	H,DB$Display$Table
	DAD	D			;HL -> Address of code to process
					;  display requirements
	MOV	E,M			;Get LS byte of address
	INX	H			;Update pointer
	MOV	D,M			;Get MS byte of address
	XCHG				;HL -> code

	LXI	D,DB$Exit		;Fake link on stack
	PUSH	D
	PCHL				;"CALL" display processor
;
DB$Exit:				;Return to the user
	POP	D			;Recover user's registers saved
	POP	B			;  on local debug stack
	POP	PSW
	LHLD	DB$Save$SP		;Revert to user's stack
	SPHL
	LHLD	DB$Save$RA		;Get updated return address (bypasses
					;  in-line parameters
	XTHL				;Replace on top of user's stack
	LHLD	DB$Save$HL		;Get user's HL
	RET				;Transfer to correct return address


DB$Display$Table:
	DW	DP$F		;Flags
	DW	DP$A		;A register
	DW	DP$B		;B
	DW	DP$C		;C
	DW	DP$D		;D
	DW	DP$E		;E
	DW	DP$H		;H
	DW	DP$L		;L
	DW	DP$BC		;BC
	DW	DP$DE		;DE
	DW	DP$HL		;HL
	DW	DP$SP		;Stack Pointer
	DW	DP$M		;Memory
	DW	DP$B$BC		;(BC)
	DW	DP$B$DE		;(DE)
	DW	DP$B$HL		;(HL)
	DW	DP$W$BC		;(BC+1),(BC)
	DW	DP$W$DE		;(DE+1),(DE)
	DW	DP$W$HL		;(HL+1),(HL)
;#
;	Debug display processing routines
;
DP$F:				;Flags
				;The Flags are displayed in the same way that
				;  DDT uses : C1Z0M0E0I0
	LDA	DB$Save$F	;Get Flags
	MOV	B,A		;Preserve copy
	LXI	H,DB$Flags$Msg + 1	;HL -> first 0/1 in message
	LXI	D,DB$Flag$Masks	;DE -> Table of flag mask values
DB$F$Next:
	LDAX	D		;Get next flag mask
	ORA	A		;Check if end of table
	JZ	DB$F$Display	;Yes, display the results

	ANA	B		;Check if this flag is set
	MVI	A,'1'		;Assume yes
	JNZ	DB$F$NZ		;Yes it is set
	MVI	A,'0'		;No it is clear
DB$F$NZ:
	MOV	M,A		;Store '0' or '1' in message text
	INX	H		;Update pointer to next 0/1
	INX	H
	INX	D		;Update flag mask pointer
	JMP	DB$F$Next
DB$F$Display:			;Display results
	LXI	H,DB$Flags$Msg
	JMP	DB$MSG		;Display message and return
;
DP$A:				;A register
	LDA	DB$Save$A	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$B:				;B
	LDA	DB$Save$B	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$C:				;C
	LDA	DB$Save$C	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$D:		;D
	LDA	DB$Save$D	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$E:		;E
	LDA	DB$Save$E	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$H:		;H
	LDA	DB$Save$H	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$L:		;L
	LDA	DB$Save$L	;Get saved value
	JMP	DB$DAH		;Display it and return
;
DP$BC:		;BC
	LHLD	DB$Save$C	;Get saved WORD value
	JMP	DB$DHLH		;Display it and return
;
DP$DE:		;DE
	LHLD	DB$Save$E	;Get saved WORD value
	JMP	DB$DHLH		;Display it and return
;
DP$HL:		;HL
	LHLD	DB$Save$HL	;Get saved WORD value
	JMP	DB$DHLH		;Display it and return
;
DP$SP:		;Stack Pointer
	LHLD	DB$Save$SP	;Get saved WORD value
	JMP	DB$DHLH		;Display it and return
;
DP$M:		;Memory
	LHLD	DB$End$Address		;Increment end address to make
	INX	H			;  arithmetic easier
	SHLD	DB$End$Address

	LHLD	DB$Start$Address	
	CALL	DB$M$Check$End		;Compare HL to End$Address
	JC	DB$M$Address$OK		;End > Start
	CALL	DB$MSGI			;Error Start > End
	DB	0DH,0AH,'** ERROR - Start Address > End **',0
	RET
;
DB$M$Next$Line:
	CALL	DB$CRLF			;Output Carriage Return, Line Feed
DB$M$Address$OK:			;Bypass CR,LF for first line
	CALL	DB$MSGI			;Indent line
	DB	'  ',0
	LHLD	DB$Start$Address	;Get start of line address
	CALL	DB$DHLH			;Display in hex

	CALL	DB$Colon		;Display ' : '

	LHLD	DB$Start$Address	
DB$M$Next$Hex$Byte:
	PUSH	H			;Save memory address
	CALL	DB$Blank		;Output a blank
	POP	H			;Recover current byte address
	MOV	A,M			;Get byte from memory
	INX	H			;Update memory pointer
	PUSH	H			;Save for later
	CALL	DB$DAH			;Display in hex
	POP	H			;Recover memory updated address
	CALL	DB$M$Check$End		;Compare HL vs End Address
	JZ	DB$M$Display$ASCII	;Yes - End of area
	MOV	A,L			;Check if at start of new line
	ANI	0000$1111B		;  by check if address is XXX0H
	JZ	DB$M$Display$ASCII	;Yes
	JMP	DB$M$Next$Hex$Byte	;No - loop back for another
;
DB$M$Display$ASCII:			;Display bytes in ASCII
	CALL	DB$Colon		;Display ' : '
	LHLD	DB$Start$Address	;Start ASCII as beginning of line
DB$M$Next$ASCII$Byte:
	MOV	A,M			;Get byte from memory
	PUSH	H			;Save memory address
	ANI	0111$1111B		;Remove parity
	MOV	C,A			;Prepare for output
	CPI	' '			;Check if non-graphic
	JNC	DB$M$Display$Char	;Char >= Space
	MVI	C,'.'			;Display non-graphic as '.'
DB$M$Display$Char:
	CPI	7FH			;Check if DEL (may be non-graphic)
	JNZ	DB$M$Not$DEL		;No - it is graphic
	MVI	C,'.'			;Force to '.'
;
DB$M$Not$DEL:
	CALL	DB$CONOUT		;Display character
	POP	H			;Recover memory address
	INX	H			;Update memory pointer
	SHLD	DB$Start$Address	;Update memory copy
	CALL	DB$M$Check$End		;Check if end of memory dump
	JZ	DB$M$Exit		;Yes - Done
	MOV	A,L			;Check if end of line
	ANI	0000$1111B		;  by checking address = XXX0H
	JZ	DB$M$Next$Line		;Yes - start next line
	MOV	A,L			;Check if extra blank needed
	ANI	0000$0011B		;  if address is multiple of 4
	JNZ	DB$M$Next$ASCII$Byte	;No - go back for next character
	CALL	DB$Blank		;Yes, output blank
	JMP	DB$M$Next$ASCII$Byte	;Go back for next character
	
;
DB$M$Exit:
	JMP	DB$CRLF			;Output Carriage Return, Line Feed
					;  and Return
;
DB$M$Check$End:				;Compares HL vs End$Address
	PUSH	D			;Save DE (Defensive programming)
	XCHG				;DE = Current Address
	LHLD	DB$End$Address		;Get end address
	MOV	A,D			;Compare MS bytes
	CMP	H
	JNZ	DB$M$Check$End$X	;Exit now as they are unequal
	MOV	A,E			;Compare LS bytes
	CMP	L
DB$M$Check$End$X:
	XCHG				;HL = Current address
	POP	D			;Recover DE
	RET				;Return with condition flags set
;
DP$B$BC:	;(BC)
	LHLD	DB$Save$C	;Get saved WORD value
	MOV	A,M		;Get byte addressed by it
	JMP	DB$DAH		;Display it and return
;
DP$B$DE:	;(DE)
	LHLD	DB$Save$E	;Get saved WORD value
	MOV	A,M		;Get byte addressed by it
	JMP	DB$DAH		;Display it and return
;
DP$B$HL:	;(HL)
	LHLD	DB$Save$HL	;Get saved WORD value
	MOV	A,M		;Get byte addressed by it
	JMP	DB$DAH		;Display it and return
;
DP$W$BC:	;(BC+1),(BC)
	LHLD	DB$Save$C	;Get saved WORD value
	MOV	E,M		;Get WORD addresses by it
	INX	H
	MOV	D,M
	XCHG			;HL = WORD to be displayed
	JMP	DB$DHLH		;Display it and return
;
DP$W$DE:	;(DE+1),(DE)
	LHLD	DB$Save$E	;Get saved WORD value
	MOV	E,M		;Get WORD addresses by it
	INX	H
	MOV	D,M
	XCHG			;HL = WORD to be displayed
	JMP	DB$DHLH		;Display it and return
;
DP$W$HL:	;(HL+1),(HL)
	LHLD	DB$Save$HL	;Get saved WORD value
	MOV	E,M		;Get WORD addresses by it
	INX	H
	MOV	D,M
	XCHG			;HL = WORD to be displayed
	JMP	DB$DHLH		;Display it and return
;
;#
;	DB$Display$CALLA
;	This routine displays the DB$Call$Address in hexadecimal,
;	followed by " : ".
;
DB$Display$CALLA:
	PUSH	H		;Save caller's HL
	LHLD	DB$Call$Address	;Get the call address
	CALL	DB$DHLH		;Display HL in hex.
	POP	H		;Recover caller's HL
	JMP	DB$Colon	;Display " : " and return
;
;#
;
;	DB$DHLH
;	Display HL in Hex.
;
;	Entry Parameters
;
;		HL = Value to be displayed
;
DB$DHLH:
	PUSH	H		;Save input value
	MOV	A,H		;Get MS byte first
	CALL	DB$DAH		;Display A in Hex
	POP	H		;Recover input value
	MOV	A,L		;Get LS byte
	JMP	DB$DAH		;Display it and return
;	
;#
;
;	DB$DAH
;	Display A register in Hexadecimal
;
;	Entry Parameters
;
;		A = Value to be converted and output
;
DB$DAH:
	PUSH	PSW		;Take a copy of the value to be converted
	RRC			;Shift A right four places
	RRC
	RRC
	RRC
	CALL	DB$Nibble$To$Hex	;Convert LS 4 bits to ASCII
	CALL	DB$CONOUT		;Display the character
	POP	PSW			;Get original value again
	CALL	DB$Nibble$To$Hex	;Convert LS 4 bits to ASCII
	JMP	DB$CONOUT		;Display and return to caller
;#
;
;	DB$CAH
;	Convert A register to Hexadecimal ASCII and store in
;	specified address.
;
;	Entry Parameters
;
;		A = Value to be converted and output
;		HL -> Buffer area to receive two characters of output
;
;	Exit Parameters
;
;		HL -> Byte following last hex byte output
;
DB$CAH:
	PUSH	PSW		;Take a copy of the value to be converted
	RRC			;Shift A right four places
	RRC
	RRC
	RRC
	CALL	DB$Nibble$To$Hex	;Convert to ASCII Hex.
	MOV	M,A			;Save in memory
	INX	H			;Update pointer
	POP	PSW			;Get original value again
	CALL	DB$Nibble$To$Hex	;Convert to ASCII Hex.
	MOV	M,A			;Save in memory
	INX	H			;Update pointer
	RET
;#
;
;	Minor subroutines.
;
;
;	DB$Nibble$To$Hex
;	This is a minor subroutine that converts the least
;	significant four bits of the A register into an ASCII
;	Hex. character in A and C
;
;	Entry Parameters
;
;		A = Nibble to be converted in LS 4 bits
;
;	Exit Parameters
;
;		A,C  = ASCII Hex. character
;
DB$Nibble$To$Hex:
	ANI	0000$1111B	;Isolate LS four bits
	ADI	'0'		;Convert to ASCII
	CPI	'9' + 1		;Compare to maximum
	JC	DB$NTH$Numeric	;No need to convert to A -> F
	ADI	7		;Convert to a letter
DB$NTH$Numeric:
	MOV	C,A		;For convenience of other routines
	RET			


;
;	DB$CRLF
;	Simple routine to display Carriage Return, Line Feed.
;
DB$CRLF:
	CALL	DB$MSGI		;Display in-line message
	DB	0DH,0AH,0
	RET
;
;	DB$Colon
;	Simple routine to display ' : '.
;
DB$Colon:
	CALL	DB$MSGI		;Display in-line message
	DB	' : ',0
	RET
;
;	DB$Blank
;	Simple routine to display ' '.
;
DB$Blank:
	CALL	DB$MSGI		;Display in-line message
	DB	' ',0
	RET
;#
;
;	Message Processing subroutines.
;
;	DB$MSGI (Message In-line)
;	Output null-byte terminated message that follows the
;	CALL to MSGOUTI.
;
;	Calling sequence
;
;		CALL	DB$MSGI
;		DB	'Message',0
;		... next instruction
;
;	Exit Parameters
;		HL -> instruction following message
;
;
DB$MSGI:
				;Get return address of stack, save
				;  user's HL on top of stack
	XTHL			;HL -> Message

	PUSH	PSW		;Save all user's registers
	PUSH	B
	PUSH	D
DB$MSGI$Next:
	MOV	A,M		;Get next data byte
	INX	H		;Update message pointer
	ORA	A		;Check if null byte
	JNZ	DB$MSGIC	;No, continue

	POP	D		;Recover user's registers
	POP	B
	POP	PSW	
	XTHL			;Recover user's HL from stack, replacing
				;  it with updated return address
	RET			;Return to address after 00-byte
				;after in-line message
DB$MSGIC:
	PUSH	H		;Save message pointer
	MOV	C,A		;Ready for output
	CALL	DB$CONOUT
	POP	H		;Recover message pointer
	JMP	DB$MSGI$Next	;Go back for next char.

;
;	DB$MSG
;	Output null-byte terminated message.
;
;	Calling sequence
;
;	MESSAGE:	DB	'Message',0
;		:
;		LXI	H,MESSAGE
;		CALL	DB$MSG
;
;	Exit Parameters
;		HL -> Null byte terminator
;
;
DB$MSG:
	PUSH	PSW			;Save user's registers
	PUSH	B
	PUSH	D
DB$MSG$Next:
	MOV	A,M		;Get next byte for output
	ORA	A		;Check if 00-byte terminator
	JZ	DB$MSG$X	;Exit
	INX	H		;Update message pointer
	PUSH	H		;Save updated pointer
	MOV	C,A		;Ready for output
	CALL	DB$CONOUT
	POP	H		;Recover message pointer
	JMP	DB$MSG$Next	;Go back for next character
;
DB$MSG$X:
	POP	D		;Recover user's registers
	POP	B
	POP	PSW
	RET
;#
;
;	Debug Input Routine
;
;	This routine helps debug code in which INput instructions
;	would normally occur. The Opcode of the IN instruction
;	must be replaced by a value of 0E7H (RST 4).
;
;	This routine picks up the port number contained in the byte
;	following the RST 4, converts it to Hexadecimal, and
;	displays the message :
;
;		Input from Port XX :
;
;	It then accepts two characters (in Hex) from the keyboard,
;	converts these to binary in A, and then returns control
;	to the byte following the port number.
;
;	*******
;	WARNING - This routine uses both DB$CONOUT and BDOS calls
;	*******
;
DBIN$Message:	DB	'Input from Port '
DBIN$Port:	DB	'XX : ',0
;
;
DB$Input:
	SHLD	DB$Save$HL	;Save user's HL
	POP	H		;Recover address of port number
	DCX	H		;Backup to point to RST
	SHLD	DB$Call$Address	;Save for later display
	INX	H		;Restore to point to port number
				;Note : A need not be preserved
	MOV	A,M		;Get Port number
	INX	H		;Update return address to bypass port number
	SHLD	DB$Save$RA	;Save return address
	PUSH	B		;Save remaining registers
	PUSH	D
	PUSH	PSW		;Save port number for later


	CALL	DB$Flag$Save$On	;Save current state of debug flag
				;  and enable debug output
	
	CALL	DB$CRLF		;Display Carriage Return, Line Feed
	CALL	DB$Display$CALLA;Display call address
	POP	PSW		;Recover port number
	LXI	H,DBIN$Port
	CALL	DB$CAH		;Convert to Hex and store in message
	LXI	H,DBIN$Message	;Output Prompting Message
	CALL	DB$MSG
	MVI	C,2		;Get 2 digit hex value
	CALL	DB$GHV		;Returns value in HL
	MOV	A,L		;Get just single byte

	CALL	DB$Flag$Restore	;Restore debug output to previous state

	POP	D		;Recover registers
	POP	B
	LHLD	DB$Save$HL	;Get previous HL
	PUSH	H		;Put on top of stack
	LHLD	DB$Save$RA	;Get return address
	XTHL			;TOS = Return Address, HL = Previous value
	RET
;#
;
;	Debug Output Routine
;
;	This routine helps debug code in which OUTput instructions
;	would normally occur. The Opcode of the OUT instruction
;	must be replaced by a value of 0EFH (RST 5).
;
;	This routine picks up the port number contained in the byte
;	following the RST 5, converts it to Hexadecimal, and
;	displays the message :
;
;		Output to Port XX : AA
;
;	Where AA is the contents of the A register prior to the
;	RST 5 being executed.
;	Control is then returned to the byte following the port number.
;
;	*******
;	WARNING - This routine uses both DB$CONOUT and BDOS calls
;	*******
;
;
DBO$Message:	DB	'Output to Port '
DBO$Port:	DB	'XX : '
DBO$Value:	DB	'AA',0
;
;
DB$Output:
	SHLD	DB$Save$HL	;Save user's HL
	POP	H		;Recover address of port number
	DCX	H		;Backup to point to RST
	SHLD	DB$Call$Address	;Save for later display
	INX	H		;Restore to point at port number
	STA	DB$Save$A	;Preserve value to be output
	MOV	A,M		;Get Port number
	INX	H		;Update return address to bypass port number
	SHLD	DB$Save$RA	;Save return address
	PUSH	B		;Save remaining registers
	PUSH	D
	PUSH	PSW		;Save port number for later

	CALL	DB$Flag$Save$On	;Save current state of debug flag
				;  and enable debug output

	CALL	DB$CRLF		;Display Carriage Return, Line Feed
	CALL	DB$Display$CALLA;Display call address
	POP	PSW		;Recover port number
	LXI	H,DBO$Port
	CALL	DB$CAH		;Convert to Hex and store in message

	LDA	DB$Save$A
	LXI	H,DBO$Value	;Convert value to be output
	CALL	DB$CAH		;Convert to Hex and store in message

	LXI	H,DBO$Message	;Output Prompting Message
	CALL	DB$MSG

	CALL	DB$Flag$Restore	;Restore debug flag to previous state

	POP	D		;Recover registers
	POP	B
	LHLD	DB$Save$HL	;Get previous HL
	PUSH	H		;Put on top of stack
	LHLD	DB$Save$RA	;Get return address
	XTHL			;TOS = Return Address, HL = Previous value
	LDA	DB$Save$A	;Recover A (NOTE : FLAG NOT RESTORED)
	RET
;#
;
;	DB$Flag$Save$On
;	This routine is only used for DB$IN/OUT.
;	It saves the current state of the debug control flag,
;	D$Flag, and then enables it so as to make sure that
;	DB$IN/OUT output always goes out.
;
DB$Flag$Previous:	DB	0	;Previous flag value
;
DB$Flag$Save$On:
	PUSH	PSW			;Save caller's registers
	LDA	DB$Flag			;Get current value
	STA	DB$Flag$Previous	;Save it
	MVI	A,0FFH			;Set flag
	STA	DB$Flag
	POP	PSW
	RET
;#
;
;	DB$Flag$Restore
;	This routine is only used for DB$IN/OUT.
;	It restores the debug control flag, DB$Flag, to
;	its former state.
;
DB$Flag$Restore:
	PUSH	PSW
	LDA	DB$Flag$Previous	;Get previous setting
	STA	DB$Flag			;Set debug control flag
	POP	PSW
	RET

;
;#
;
;	Get Hex Value
;
;	This subroutine outputs a prompting message, and then reads
;	the keyboard in order to get a hexadecimal value.
;	It is somewhat simplistic in that the first non-hex value
;	terminates the input. The maximum number of digits to be
;	converted is specified as an input parameter.
;	entered, only the last four are significant.
;
;****************************************************************
;			W A R N I N G
;	DB$GHV will ALWAYS use the BDOS to perform a Read Console
;	Function (#10). Be CAREFUL if you use this routine from
;	within an executing BIOS.
;****************************************************************
;
;	Entry Parameters
;
;		HL -> 00-byte terminated message to be output
;		C = number of hexadecimal digits to be input
;
;
DB$GHV$Buffer:			;Input buffer for console characters
DB$GHV$Max$Count:
	DB	0		;Set to the maximum number of chars
				;  to be input
DB$GHV$Input$Count:
	DB	0		;Set by the BDOS to the actual number
				;  or chars entered
DB$GHV$Data$Bytes
	DS	5		;Buffer space for the characters
;
;
DB$GHV:
	MOV	A,C			;Get maximum characters to be input
	CPI	5			;Check against maximum count
	JC	DB$GHV$Count$OK		;Carry set if A < 5
	MVI	A,4			;Force to only four characters
DB$GHV$Count$OK:
	STA	DB$GHV$Max$Count	;Setup maximum count in input buffer
	CALL	DB$MSG			;Output Prompting Message
	LXI	D,DB$GHV$Buffer		;Accept characters from console
	MVI	C,B$READCONS		;Function Code
	CALL	BDOS

	MVI	C,B$CONOUT		;Output a Line Feed
	MVI	E,0AH
	CALL	BDOS

	LXI	H,0			;Initial value
	LXI	D,DB$GHV$Data$Bytes	;DE -> Data characters
	LDA	DB$GHV$Input$Count	;Get count of characters input
	MOV	C,A			;Keep count in C
DB$GHV$Loop:
	DCR	C			;Downdate count
	RM				;Return when all done (HL has value)
	LDAX	D			;Get next character from buffer
	INX	D			;Update buffer pointer
	CALL	DB$A$To$Upper		;Convert A to upper case if need be
	CPI	'0'			;Check if less than 0
	RC				;Yes, terminate
	CPI	'9' + 1			;Check if > 9
	JC	DB$GHV$Hex$Digit	;No - it must be numeric
	CPI	'A'			;Check if < 'A'
	RC				;Yes - terminate
	CPI	'F' + 1			;Check if > 'F'
	RNC				;Yes - terminate
	SUI	'A' - 10		;Convert A through F to numeric
	JMP	DB$GHV$Shift$Left$4	;Combine with current result
;
DB$GHV$Hex$Digit:
	SUI	'0'			;Convert to binary
DB$GHV$Shift$Left$4:
	DAD	H			;Shift HL left four bits
	DAD	H
	DAD	H
	DAD	H
	ADD	L			;Add binary value in LS 4 bits of A
	MOV	L,A			;Put back into HL total
	JMP	DB$GHV$Loop		;Loop back for next character
;#
;
;	A to Upper
;	Converts the contents of the A register to an upper
;	case letter if it is currently a lower case letter.
;
;	Entry Parameters
;
;		A = Character to be converted
;
;	Exit Parameters
;
;		A = Converted character
;
DB$A$To$Upper:
	CPI	'a'		;Compare to lower limit
	RC			;No need to convert
	CPI	'z' + 1		;Compare to upper limit
	RNC			;No need to convert
	ANI	5FH		;Convert to upper case
	RET
