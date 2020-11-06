;	Figure 8-10
;
;	Revision History
;	This file contains two fixes not shown in the Programmer's Handbook
;	for CP/M.
;	Line 3828 has been changed from STA MOB$Maximum$Busy to
;				        STA MOB$Character
;	A new line has been inserted after line 9270,
;		LXI	H,Disk$Control$5
;	Also note that the 300 or so lines of code inadvertently not
;	printed in the book (see bottom of page 261, top of 262) is
;	present in this file.
;
;	AJL 08/29/83.
;
;********************************************************
;*							*
;*	       Enhanced BIOS Listing			*
;*							*
;********************************************************
;
;	This is a skeletal example of an enhanced BIOS.
;	As such it includes fragments of the standard BIOS
;	shown as Figure 6-4, but only in outline so as to
;	avoid cluttering up the enhancements with the
;	supporting substructure. Many of the original
;	comment blocks have been abbreviated or deleted
;	entirely.
;
;<---- NOTE : 	The line numbers at the left are included purely
;		to allow reference to the code from the text.
;		There are deliberately induced discontinuities
;		in the numbers to allow space for expansion.
;
VERSION		EQU	'00'	;Equates used in the sign on message
MONTH		EQU	'02'
DAY		EQU	'26'
YEAR		EQU	'83'
;
;************************************************************************
;*									*
;*	This BIOS is for a computer system with the following		*
;*	hardware configuration :					*
;*									*
;*		- 8080 CPU						*
;*		- 64K Bytes of RAM					*
;*		- 3 Serial I/O Ports (Using Signetics 2651) for :	*
;*		       Console, Communications and List        		*
;*		- Two 5 1/4" Mini Floppy  double-sided,	double-		*
;*		  density drives. These drives use 512-byte sectors.	*
;*		  These are used as logical disks A: and B:.		*
;*		  Full track buffering is supported.			*
;*		- Two 8" standard Diskette Drives (128-byte sectors)	*
;*		  These are used as logical disks C: and D:.		*
;*		- A memory-based disk (M-DISK) is supported.		*
;*									*
;*		  Two intelligent disk controllers are used, one for	*
;*		  each diskette type. These controllers access memory	*
;*		  directly, both to read the details of the 		*
;*		  operations they are to perform and also to read	*
;*		  and write data from and to the diskettes.		*
;*									*
;*									*
;************************************************************************

	
;	Equates for characters in the ASCII character set.
;
XON	EQU	11H	;Re-enables transmission of data
XOFF	EQU	13H	;Disables transmission of data
ETX	EQU	03H	;End of Transmission
ACK	EQU	06H	;Acknowledge
CR	EQU	0DH	;Carriage Return
LF	EQU	0AH	;Line Feed
TAB	EQU	09H	;Horizontal Tab
BELL	EQU	07H	;Sound terminal's Bell
;
;
;	Equates for defining memory size and the base address and
;	length of the system components.
;
Memory$Size	EQU	64	;Number of Kbytes of RAM
;
;	The BIOS Length must be determined by inspection.
;	Comment out the ORG BIOS$Entry line below by changing the first
;	character to a semi-colon, (this will make the Assembler start 
;	the BIOS at location 0), then assemble the BIOS and round up to 
;	the nearest 100H the address displayed on the console at the end 
;	of the assembly.
;
BIOS$Length	EQU	2500H	;<-- Revised to an approximate value
				;    to reflect enhancements
;
CCP$Length	EQU	0800H	;Constant
BDOS$Length	EQU	0E00H	;Constant
;
Overall$Length	EQU	(CCP$Length + BDOS$Length + BIOS$Length + 1023) / 1024
;
CCP$Entry	EQU	(Memory$Size - Overall$Length) * 1024
BDOS$Entry	EQU	CCP$Entry + CCP$Length + 6	
BIOS$Entry	EQU	CCP$Entry + CCP$Length + BDOS$Length
;
BDOS		EQU	0005H	;BDOS entry point (used for making
				;  system reset requests)
;
;#
;	ORG	BIOS$Entry	;Assemble code at BIOS address
;
;	BIOS Jump Vector
;
	JMP	BOOT	;Cold Boot - entered from CP/M Bootstrap Loader
Warm$Boot$Entry:	;  Labelled so that the initialization code can
			;  put the Warm Boot entry address down in location
			;  0001H and 0002H of the base page.
	JMP	WBOOT	;Warm Boot - entered by jumping to location 0000H
			;  Reloads the CCP which could have been
			;  overwritten by previous program in Transient
			;  Program area.
	JMP	CONST	;Console Status - returns A = 0FFH if there is a
			;  Console Keyboard character waiting.
	JMP	CONIN	;Console Input - returns the next console keyboard
			;  Character in A.
	JMP	CONOUT	;Console Output - outputs the character in C to
			;  the console device.
	JMP	LIST	;List Output - outputs the character in C to the
			;  list device.
	JMP	AUXOUT	;Auxiliary Output - outputs the character in C to the 
			;  logical auxiliary device.
	JMP	AUXIN	;Auxiliary Input - returns the next input character from
			;  the logical auxiliary device in A.
	JMP	HOME	;Homes the currently selected disk to Track 0.
	JMP	SELDSK	;Selects the disk drive specified in register C and
			;  returns the address of the Disk Parameter Header
	JMP	SETTRK	;Sets the track for the next read or write operation
			;  from the BC register pair.
	JMP	SETSEC	;Sets the sector for the next read or write operation
			;  from the A register.
	JMP	SETDMA	;Sets the Direct Memory Address (Disk Read/Write)
			;  address for the next read or write operation
			;  from the DE register pair.
	JMP	READ	;Reads the previously specified Track and Sector from
			;  the selected disk into the DMA address.
	JMP	WRITE	;Writes the previously specified Track and Sector onto
			;  the selected disk from the DMA address.
	JMP	LISTST	;Returns A = 0FFH if the list device(s) are
			;  logically ready to accept another output byte.
	JMP	SECTRAN	;Translates a logical sector into a physical one.
;
;	Additional "Private" BIOS Entry Points
;
	JMP	AUXIST	;Returns A = 0FFH if there is input data for
			;  the logical auxiliary device
	JMP	AUXOST	;Returns A = 0FFH if the auxiliary device(s) are
			;  logically ready to accept another output byte.
	JMP	Specific$CIO$Initialization
			;Initializations character device whose device
			;  number is in register A on entry
	JMP	Set$Watchdog
			;Sets up Watchdog timer to CALL address specified
			;  in HL, after BC clock ticks have elapsed
	JMP	CB$Get$Address
			;Configuration Block Get Address
			;  Returns address in HL of data element whose
			;  code number is specified in C
;
;#
;	Long Term Configuration Block
;
Long$Term$CB:
;
;
;	Public Files (files in user 0 being accessible from all
;	other user numbers), is enabled when this flag is set
;	non-zero.
;
CB$Public$Files:	DB	0	;Default is OFF
;
;
;	The Forced Input pointer is initialized to point to the
;	following string of characters. These are injected into 
;	the console input stream on system startup.
;
CB$Startup:		DB	'SUBMIT STARTUP',LF,0,0,0,0,0,0
;
;	Logical to Physical Device Re-direction
;
;		Each logical device has a 16-bit word associated
;		with it. Each bit in the word is assigned to a
;		specific PHYSICAL device. For INPUT, only one bit
;		can be set - and input will be read from the 
;		corresponding physical device. OUTPUT can be
;		directed to several devices, so more than one
;		bit can be set.
;
;		The following EQUates are used to indicate
;		specific Physical devices.
;
;			1111 11             )
;			5432 1098 7654 3210 )<- Device Number
Device$0	EQU	0000$0000$0000$0001B
Device$1	EQU	0000$0000$0000$0010B
Device$2	EQU	0000$0000$0000$0100B
;
;		The following words are tested by the logical
;		device drivers in order to transfer control
;		to the appropriate physical device drivers
;
CB$Console$Input:	DW	Device$0
CB$Console$Output:	DW	Device$0
;
CB$Auxiliary$Input:	DW	Device$1
CB$Auxiliary$Output:	DW	Device$1
;
CB$List$Input:		DW	Device$2
CB$List$Output:		DW	Device$2
;
;		The table below relates specific bits in the
;		redirection words above to specific Device
;		Tables used by the physical drivers
;
CB$Device$Table$Addresses:
	DW	DT$0
	DW	DT$1
	DW	DT$2
	DW	0,0,0,0,0,0,0,0,0,0,0,0,0	;Unassigned
;
;
;	Device Initialization Byte Streams
;
;	These initialization streams are output during the device
;	initialization phase, or on request whenever the Baud rate
;	needs to be changed. They are defined in the Long Term
;	Configuration Block so as to "freeze" their contents from one
;	system startup until the next.
;
;	The address of each stream is contained in each Device Table.
;
;	The stream format is :
;
;		DB	xx		;Port number (00H terminates)
;		DB	nn		;Number of bytes to output to port
;		DB	vv,vv,vv..	;Values to be output
;
D0$Initialize$Stream:		;Example data for an 8251A chip
	DB	0EDH			;Port number for 8251A
	DB	6			;Number of bytes
	DB	0,0,0			;Dummy bytes to get chip ready
	DB	0100$0010B		;Reset and raise DTR
	DB	01$10$11$10B		;1 stop, no parity, 8 bits/char,
					;  Divide down of 16
	DB	0010$0101B		;RTS high, enable Tx/Rx
				;Example data for an 8253 chip
	DB	0DFH			;Port number for 8253 Mode
	DB	1			;Number of Bytes to output
	DB	01$11$011$0B		;Select :
					;	Counter 1
					;	Load LS Byte first
					;	Mode 3, Binary Count
	DB	0DEH			;Port number for counter
	DB	2			;Number of Bytes to output
D0$Baud$Rate$Constant:			;Label used by utilities
	DW	0007H			;9600 Baud (based on 16x divider)
	DB	0			;Port number of 00 terminates stream

D1$Initialize$Stream:		;Example data for an 8251A chip
	DB	0DDH			;Port number for 8251A
	DB	6			;Number of bytes
	DB	0,0,0			;Dummy bytes to get chip ready
	DB	0100$0010B		;Reset and raise DTR
	DB	01$10$11$10B		;1 stop, no parity, 8 bits/char,
					;  Divide down of 16
	DB	0010$0101B		;RTS high, enable Tx/Rx


	DB	0DFH			;Port number for 8253 Mode
	DB	1			;Number of Bytes to output
	DB	10$11$011$0B		;Select :
					;	Counter 2
					;	Load LS Byte first
					;	Mode 3, Binary Count
	DB	0DEH			;Port number for counter
	DB	2			;Number of Bytes to output
D1$Baud$Rate$Constant:
	DW	0038H			;1200 Baud (based on 16x divider)
	DB	0			;Port number of 00 terminates stream

D2$Initialize$Stream:		;Example data for an 8251A chip
	DB	0DDH			;Port number for 8251A
	DB	6			;Number of bytes
	DB	0,0,0			;Dummy bytes to get chip ready
	DB	0100$0010B		;Reset and raise DTR
	DB	01$10$11$10B		;1 stop, no parity, 8 bits/char,
					;  Divide down of 16
	DB	0010$0101B		;RTS high, enable Tx/Rx


	DB	0DFH			;Port number for 8253 Mode
	DB	1			;Number of Bytes to output
	DB	11$11$011$0B		;Select :
					;	Counter 3
					;	Load LS Byte first
					;	Mode 3, Binary Count
	DB	0DEH			;Port number for counter
	DB	2			;Number of Bytes to output
D2$Baud$Rate$Constant:
	DW	0038H			;1200 Baud (based on 16x divider)
	DB	0			;Port number of 00 terminates stream

;	
;	This following table is used to determine the maximum
;	value for each character position in the ASCII time 
;	value above (except the ":"). Note - this table is present
;	in the Long Term Configuration Block so that the clock can
;	be set "permanently" to either 12 or 24 hour format.
;
;	NOTE : The table is processed backwards - to correspond 
;	with the ASCII time.
;	Each character represent the value for the corresponding 
;	character in the ASCII time at which a Carry-and-Reset-to-zero
;	should occur.
;
	DB	0		;"Terminator"
CB$12$24$Clock:
	DB	'34'		;Change to '23' for a 12-hour clock
	DB	0FFH		;"Skip" character
	DB	'6:'		;Maximum minutes are 59
	DB	0FFH		;"Skip" character
	DB	'6:'		;Maximum seconds are 59
Update$Time$End:		;Used when updating the time
;
;
;	Variables for the Real Time Clock and Watchdog
;	timer.
;
RTC$Ticks$per$Second	DB	60	;Number of Real Time Clock
					;  ticks per elapsed second
RTC$Tick$Count		DB	60	;Residual count before next
					;  second will elapse
RTC$Watchdog$Count	DW	0	;Watchdog timer tick count
					;(0 = no watchdog timer set)
RTC$Watchdog$Address	DW	0	;Address to which control
					;  will be transferred if the
					;  watchdog count hits 0

;
;	Function Key Table
;
;	This table consists of a series of entries, each one having the
;	following structure :
;
;		DB	Second character of sequence emitted by
;			terminal's function key
;	(	DB	Third character of sequence - NOTE : this is	  )
;	(		field will not be present if the source code	  )
;	(		has been configured to accept only two characters )
;	(		in function key sequences.			  )
;	(		NOTE : Adjust the EQUates for :			  )
;	(			Function$Key$Length			  )
;	(			Three$Character$Function		  )
;
;		DB	A character string to be forced into the console
;			input stream when the corresponding function key
;			is pressed. The last byte of this string must be
;			00H in order to terminate the forced input.
;
Function$Key$Lead	EQU	1BH	;Signals Function Key Sequence
Function$Key$Length	EQU	3	;Number of characters in Function
					;  key input sequence (NOTE : this
					;  can only be 3 or 2 characters).

;
					;The logic associated with Function
					;  key recognition is made easier with
					;  the following EQUate
Three$Character$Function	EQU	Function$Key$Length - 2
				;Three$Character$Function will be TRUE if the
				;  function keys emit a three character
				;  sequence, FALSE if they emit a two character
				;  sequence.

;	Each entry in the table must be the same length, as defined by :
;
CB$Function$Key$Entry$Size	EQU	16 + 1 + Function$Key$Length - 1
;					^    ^                         ^
;				        |    |                         |
;		Maximum length of substitute |		Lead character is not
;		string                       |          in table entry
;		      			     For the terminating 00H
;
;	The last entry in the table is marked by a 00-byte.
;
;	The example values shown below are for a VT-100 terminal.
;
CB$Function$Key$Table:
;                        123456789.1234  5  6 7 <- Use to check length
	DB	'O','P','Function Key 1',LF,0,0
	DB	'O','Q','Function Key 2',LF,0,0
	DB	'O','R','Function Key 3',LF,0,0
	DB	'O','S','Function Key 4',LF,0,0
;
;                        123456789.1
	DB	'[','A','Up Arrow',LF,0,0,0,0,0,0,0,0
	DB	'[','B','Down Arrow',LF,0,0,0,0,0,0
	DB	'[','C','Right Arrow',LF,0,0,0,0,0
	DB	'[','D','Left Arrow',LF,0,0,0,0,0,0

	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	;Spare entries
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	
	DB	0FFH,0FFH	;Terminator for utility that pre-programs
				;  function key sequence
;
;
;	Console Output Escape Sequence Control Table
;
;	This table is referenced after a Function$Key$Lead character
;	has been detected in the CONOUT routine. The next character
;	to be output to the console is compared to the first byte
;	in each 3-byte table entry. If a match is found, then control
;	is transferred to the address following the byte that matched
;
CONOUT$Escape$Table:
	DB	't'		;Read current time
	DW	CONOUT$Time
	DB	'd'		;Read current date
	DW	CONOUT$Date
	DB	'u'		;Set current time
	DW	CONOUT$Set$Time
	DB	'e'		;Set current date
	DW	CONOUT$Set$Date

	DB	0		;Terminator
;
Long$Term$CB$End:
;
;#
;
;	Interrupt Vector 
;
;	Control is transferred here by the Programmable Interrupt
;	Controller - an Intel 8259A.
;
;	NOTE : The Interrupt Controller chip requires that the
;		interrupt vector table start on a paragraph
;		boundary. This is achieved by the following ORG line
	ORG	($ AND 0FFE0H) + 20H
Interrupt$Vector:
			;Interrupt Number
	JMP	RTC$Interrupt		;0 - Clock
	DB	0				;Skip a byte
	JMP	Character$Interrupt	;1 - Character I/O
	DB	0
	JMP	Ghost$Interrupt		;2 - Not used
	DB	0
	JMP	Ghost$Interrupt		;3 - Not used
	DB	0
	JMP	Ghost$Interrupt		;4 - Not used
	DB	0
	JMP	Ghost$Interrupt		;5 - Not used
	DB	0
	JMP	Ghost$Interrupt		;6 - Not used
	DB	0
	JMP	Ghost$Interrupt		;7 - Not used
;
;#

;	Device Port Numbers and other Equates
;
CIO$Base$Port	EQU	80H		;Base Port number

D0$Base$Port	EQU	CIO$Base$Port		;Device 0
D0$Data$Port	EQU	D0$Base$Port
D0$Status$Port	EQU	D0$Base$Port + 1
D0$Mode$Port	EQU	D0$Base$Port + 2
D0$Command$Port	EQU	D0$Base$Port + 3
;

D1$Base$Port	EQU	CIO$Base$Port + 4 	;Device 1
D1$Data$Port	EQU	D1$Base$Port
D1$Status$Port	EQU	D1$Base$Port + 1
D1$Mode$Port	EQU	D1$Base$Port + 2
D1$Command$Port	EQU	D1$Base$Port + 3

D2$Base$Port	EQU	CIO$Base$Port + 8	;Device 2
D2$Data$Port	EQU	D2$Base$Port
D2$Status$Port	EQU	D2$Base$Port + 1
D2$Mode$Port	EQU	D2$Base$Port + 2
D2$Command$Port	EQU	D2$Base$Port + 3

D$Mode$Value$1	EQU	01$00$11$10B
				;1 stop bit, no parity
				;8 bits, Async 16x rate
D$Mode$Value$2	EQU	00$11$1100B
				;Tx/Rx on internal clock
				;9600 Baud
D$Command$Value	EQU	00$100111B
				;Normal mode
				;Enable Tx/Rx
				;RTS and DTR active
D$Error		EQU	0011$1000B
D$Error$Reset	EQU	00$110111B
				;Same as command value plus error reset
D$Output$Ready	EQU	0000$0001B
D$Input$Ready	EQU	0000$0010B
D$DTR$High	EQU	1000$0000B	;Note : this is actually the
					;       Data Set Ready pin
					;       on the chip. It is connected
					;       to the DTR pin on the cable
D$Raise$RTS	EQU	00$1$00111B	;Raise RTS, Tx/Rx Enable
D$Drop$RTS	EQU	00$0$00111B	;Drop RTS, Tx/Rx Enable
;
;
;	Interrupt Controller Ports (Intel 8259A)
;
;	Note : these equates are placed here so that they
;		follow the  definition of the Interrupt Vector
;		and thus avoid 'P' (Phase) errors in ASM.
;
IC$OCW1$Port	EQU	0D9H	;Operational Control Word 1
IC$OCW2$Port	EQU	0D8H	;Operational Control Word 2 
IC$OCW3$Port	EQU	0D8H	;Operational Control Word 3
IC$ICW1$Port	EQU	0D8H	;Initialization Control Word 1
IC$ICW2$Port	EQU	0D9H	;Initialization Control Word 2
;
IC$EOI		EQU	20H	;Non-specific End of Interrupt
;
IC$ICW1		EQU	(Interrupt$Vector AND 1110$0000B) + 000$10110B
				;Sets the A7-A5 bits of the interrupt
				;  vector address plus :
				;	Edge triggered
				;	4 byte interval
				;	Single 8259 in system
				;	No ICW4 needed
IC$ICW2		EQU	Interrupt$Vector SHR 8
				;Address bits A15 - A8 of the interrupt
				;  vector address. Note the interrupt
				;  vector is the first structure in
				;  the Long Term Configuration Block

IC$OCW1		EQU	1111$1100B	;Interrupt Mask
				;Interrupt 0 (Clock) enabled
				;Interrupt 1 (Character Input) enabled
;
;#
;
;
Display$Message:	;Displays the specified message on the console.
			;On entry, HL points to a stream of bytes to be
			;output. A 00H-byte terminates the message.
	MOV	A,M		;Get next message byte
	ORA	A		;Check if terminator
	RZ			;Yes, return to caller
	MOV	C,A		;Prepare for output
	PUSH	H		;Save message pointer
	CALL	CONOUT		;Goto main console output routine
	POP	H		;Recover message pointer
	INX	H		;Move to next byte of message
	JMP	Display$Message	;Loop until complete message output
;
;#
;
Enter$CPM:	;This routine is entered either from the Cold or Warm
		;Boot code. It sets up the JMP instructions in the
		;Base Page, and also sets the high-level disk driver's
		;Input/Output Address (also known as the DMA address).
;
	MVI	A,JMP		;Get Machine Code for JMP
	STA	0000H		;Setup JMP at location 0000H
	STA	0005H		;and at location 0005H
;
	LXI	H,Warm$Boot$Entry	;Get BIOS Vector address
	SHLD	0001H		;Put address at location 0001H

	LXI	H,BDOS$Entry	;Get BDOS entry point address
	SHLD	6		;Put address at location 0005H
;
	LXI	B,80H		;Set Disk I/O Address to default
	CALL	SETDMA		;Use normal BIOS routine
;
	EI			;Ensure interrupts are enabled
	LDA	Default$Disk	;Handover current Default Disk to
	MOV	C,A		;Console Command Processor
	JMP	CCP$Entry	;Transfer to CCP
;
;#
;
;	Device Table Equates
;	The drivers use an Device Table for each
;	Physical Device they service. The EQUates that follow
;	are used to access the various fields within the
;	Device Table.
;
;			Port Numbers and Status Bits
DT$Status$Port		EQU	0	;Device Status Port number
DT$Data$Port		EQU	DT$Status$Port+1
					;Device Data Port number
DT$Output$Ready		EQU	DT$DataPort+1
					;Output ready status mask
DT$Input$Ready		EQU	DT$Output$Ready+1
					;Input ready status mask
DT$DTR$Ready		EQU	DT$Input$Ready+1
					;DTR ready to send mask
DT$Reset$Int$Port	EQU	DT$DTR$Ready+1
					;Port number used to reset an
					;  interrupt
DT$Reset$Int$Value	EQU	DT$Reset$Int$Port+1
					;Value output to reset interrupt
DT$Detect$Error$Port	EQU	DT$Reset$Int$Value+1
					;Port number for error detect
DT$Detect$Error$Value	EQU	DT$Detect$Error$Port+1
					;Mask for detecting error (parity etc.)
DT$Reset$Error$Port	EQU	DT$Detect$Error$Value+1
					;Output to port to reset error
DT$Reset$Error$Value	EQU	DT$Reset$Error$Port+1
					;Value to output to reset error
DT$RTS$Control$Port	EQU	DT$Reset$Error$Value+1
					;Control port for lowering RTS
DT$Drop$RTS$Value	EQU	DT$RTS$Control$Port+1
					;Value, when output, to drop RTS
DT$Raise$RTS$Value	EQU	DT$Drop$RTS$Value+1
					;Value, when output, to raise RTS
;
;		Device Logical Status (incl. Protocols)
DT$Status		EQU	DT$Raise$RTS$Value+1
					;Status Bits
DT$Output$Suspend	EQU	0000$0001B	;Output suspended pending
						;  Protocol action
DT$Input$Suspend	EQU	0000$0010B	;Input suspended until
						;  buffer empties
DT$Output$DTR		EQU	0000$0100B	;Output uses DTR high to send
DT$Output$Xon		EQU	0000$1000B	;Output uses Xon/Xoff
DT$Output$Etx		EQU	0001$0000B	;Output uses Etx/Ack
DT$Output$Timeout	EQU	0010$0000B	;Output uses Timeout
DT$Input$RTS		EQU	0100$0000B	;Input uses RTS high to receive
DT$Input$Xon		EQU	1000$0000B	;Input uses Xon/Xoff
;
DT$Status$2		EQU	DT$Status+1	;Secondary Status Byte
DT$Fake$Typeahead	EQU	0000$0001B	;Requests Input$Status to
						;return "Data Ready" when
						;Control Characters are in
						;input buffer
;
DT$Etx$Count		EQU	DT$Status$2+1
					;No. of chars sent in Etx protocol
DT$Etx$Message$Length	EQU	DT$Etx$Count+2
					;Specified message length
;
;			Input Buffer values
DT$Buffer$Base		EQU	DT$Etx$Message$Length+2
					;Address of Input Buffer
DT$Put$Offset		EQU	DT$Buffer$Base+2
					;Offset for Putting chars into buffer
DT$Get$Offset		EQU	DT$Put$Offset+1
					;Offset for Getting chars from buffer
DT$Buffer$Length$Mask	EQU	DT$Get$Offset+1
					;Length of buffer - 1
					;Note : Buffer length must always be
					;  a binary number; e.g. 32, 64 or 128
					;This mask then becomes :
					;  32 ->  31 (0001$1111B)
					;  64 ->  63 (0011$1111B)
					; 128 -> 127 (0111$1111B)
					;After the Get/Put offset has been
					;incremented it is ANDed with the mask
					;to reset it to zero when the end of
					;the buffer has been reached.
DT$Character$Count	EQU	DT$Buffer$Length$Mask+1
					;Count of the number of characters
					;  currently in the buffer
DT$Stop$Input$Count	EQU	DT$Character$Count+1
					;Stop input when the count reaches
					;  this value
DT$Resume$Input$Count	EQU	DT$Stop$Input$Count+1
					;Resume input when the count reaches
					;  this value
DT$Control$Count	EQU	DT$Resume$Input$Count+1
					;Count of the number of control
					;  characters in the buffer
DT$Function$Delay	EQU	DT$Control$Count+1
					;Number of clock ticks to delay to
					;  allow all characters after function
					;  key lead-in to arrive
DT$Initialize$Stream	EQU	DT$Function$Delay+1
					;Address of byte stream necessary to
					;  initialize this device

;#
;
;	Device Tables
;
DT$0:
	DB	D0$Status$Port	;Status Port (8251A chip)
	DB	D0$Data$Port	;Data Port
	DB	D$Output$Ready	;Output Data Ready
	DB	D$Input$Ready	;Input Data Ready
	DB	D$DTR$High	;DTR ready to send
	DB	IC$OCW2$Port	;Reset Interrupt Port (00H is a unused port)
	DB	IC$EOI		;Reset Interrupt Value (Non-specific EOI)
	DB	D0$Status$Port	;Detect Error Port
	DB	D$Error		;Mask : Framing, Overrun, Parity errors
	DB	D0$Command$Port	;Reset Error Port
	DB	D$Error$Reset	;Reset Error : RTS high, Reset, Tx/Rx enable
	DB	D0$Command$Port	;Drop/Raise RTS Port
	DB	D$Drop$RTS	;Drop RTS Value (keep Tx & Rx enabled)
	DB	D$Raise$RTS	;Raise RTS Value (keep Tx & Rx enabled)
	DB	DT$Input$Xon + DT$Input$RTS	;Protocol and Status
	DB	0		;Status #2
	DW	1024		;Etx/Ack Message Count
	DW	1024		;Etx/Ack Message Length
	DW	D0$Buffer	;Input Buffer
	DB	0		;Put Offset into buffer
	DB	0		;Get Offset into buffer
	DB	D0$Buffer$Length -1 ;Buffer length mask
	DB	0		;Count of Character in buffer
	DB	D0$Buffer$Length - 5 ;Stop input when count hits this value
	DB	D0$Buffer$Length / 2 ;Resume input when count hits this value
	DB	0		;Count of Control Characters in buffer
	DB	6		;Number of 16.66ms ticks to allow Function
				;  Key sequence to arrive (approx. 90ms)
	DW	D0$Initialize$Stream	;Address of initialization stream
;
DT$1:
	DB	D1$Status$Port	;Status Port (8251A chip)
	DB	D1$Data$Port	;Data Port
	DB	D$Output$Ready	;Output Data Ready
	DB	D$Input$Ready	;Input Data Ready
	DB	D$DTR$High	;DTR ready to send
	DB	IC$OCW2$Port		;Reset Interrupt Port (00H is a unused port)
	DB	IC$EOI		;Reset Interrupt Value (Non-specific EOI)
	DB	D1$Status$Port	;Detect Error Port
	DB	D$Error		;Mask : Framing, Overrun, Parity errors
	DB	D1$Command$Port	;Reset Error Port
	DB	D$Error$Reset	;Reset Error : RTS high, Reset, Tx/Rx enable
	DB	D1$Command$Port	;Drop/Raise RTS Port
	DB	D$Drop$RTS	;Drop RTS Value (keep Tx & Rx enabled)
	DB	D$Raise$RTS	;Raise RTS Value (keep Tx & Rx enabled)
	DB	DT$Input$Xon + DT$Input$RTS	;Protocol and Status
	DB	0		;Status #2
	DW	1024		;Etx/Ack Message Count
	DW	1024		;Etx/Ack Message Length
	DW	D1$Buffer	;Input Buffer
	DB	0		;Put Offset into buffer
	DB	0		;Get Offset into buffer
	DB	D1$Buffer$Length -1 ;Buffer length mask
	DB	0		;Count of Character in buffer
	DB	D1$Buffer$Length - 5 ;Stop input when count hits this value
	DB	D1$Buffer$Length / 2 ;Resume input when count hits this value
	DB	0		;Count of Control Characters in buffer
	DB	6		;Number of 16.66ms ticks to allow Function
				;  Key sequence to arrive (approx. 90ms)
	DW	D1$Initialize$Stream	;Address of initialization stream
;
;
DT$2:
	DB	D2$Status$Port	;Status Port (8251A chip)
	DB	D2$Data$Port	;Data Port
	DB	D$Output$Ready	;Output Data Ready
	DB	D$Input$Ready	;Input Data Ready
	DB	D$DTR$High	;DTR ready to send
	DB	IC$OCW2$Port	;Reset Interrupt Port (00H is a unused port)
	DB	IC$EOI		;Reset Interrupt Value (Non-specific EOI)
	DB	D2$Status$Port	;Detect Error Port
	DB	D$Error		;Mask : Framing, Overrun, Parity errors
	DB	D2$Command$Port	;Reset Error Port
	DB	D$Error$Reset	;Reset Error : RTS high, Reset, Tx/Rx enable
	DB	D2$Command$Port	;Drop/Raise RTS Port
	DB	D$Drop$RTS	;Drop RTS Value (keep Tx & Rx enabled)
	DB	D$Raise$RTS	;Raise RTS Value (keep Tx & Rx enabled)
	DB	DT$Input$Xon + DT$Input$RTS	;Protocol and Status
	DB	0		;Status #2
	DW	1024		;Etx/Ack Message Count
	DW	1024		;Etx/Ack Message Length
	DW	D2$Buffer	;Input Buffer
	DB	0		;Put Offset into buffer
	DB	0		;Get Offset into buffer
	DB	D2$Buffer$Length -1 ;Buffer length mask
	DB	0		;Count of Character in buffer
	DB	D2$Buffer$Length - 5 ;Stop input when count hits this value
	DB	D2$Buffer$Length / 2 ;Resume input when count hits this value
	DB	0		;Count of Control Characters in buffer
	DB	6		;Number of 16.66ms ticks to allow Function
				;  Key sequence to arrive (approx. 90ms)
	DW	D2$Initialize$Stream	;Address of initialization stream
;
;#
;	General Character I/O Device Initialization
;
;	This routine will be CALLed from the main CP/M
;	initialization code.
;
;	It makes repeated calls to the Specific Character I/O
;	Device initialization routine.
;
General$CIO$Initialization:
	XRA	A		;Set Device Number (used to access the
				;  table of Device Table Addresses in the
				;  Configuration Block)
	MOV	C,A		;Match to externally callable interface
GCI$Next$Device:
	CALL	Specific$CIO$Initialization	;Initialize the device
	INR	A		;Move to next device
	CPI	16		;Check if all possible devices (0-15)
	RZ			;  have been initialized
	JMP	GCI$Next$Device
;
;#
;
;	Specific Character I/O Initialization
;
;	This routine outputs the specified byte values to the specified
;	ports as controlled by the Initialization Streams in the
;	Configuration Block. Each Device Table contains a pointer to
;	these streams. The Device Table itself is select according
;	to the Device NUMBER - this is an entry parameter for this
;	routine.
;	This routine will be CALLed either from the General Device
;	Initialization routine above, or directly by a BIOS call from
;	a system utility executing in the TPA.
;
;	Entry Parameters
;
;		C = Device Number
;
;	Exit Parameters
;
;		A = Device Number (preserved)
;
;===========================
Specific$CIO$Initialization:		;<=== BIOS Entry Point (Private)
;===========================
	MOV	A,C		;Get Device Number
	PUSH	PSW		;Preserve Device Number
	ADD	A		;Make Device Number into word pointer
	MOV	C,A
	MVI	B,0		;Make into a Word
	LXI	H,CB$Device$Table$Addresses	;Get table base
	DAD	B		;HL -> Device Table Address
	MOV	E,M		;Get LS Byte
	INX	H
	MOV	D,M		;Get MS Byte : DE -> Device Table

	MOV	A,D		;Check if Device Table address = 0
	ORA	E
	JZ	SCI$Exit	;Yes, device table non-existent

	LXI	H,DT$Initialize$Stream
	DAD	D		;HL -> Initialization Stream Address
	MOV	E,M		;Get LS Byte
	INX	H
	MOV	D,M		;Get MS Byte
	XCHG			;HL -> Initialization Stream itself
	CALL	Output$Byte$Stream	;Output byte stream to various
					;  ports
;
SCI$Exit:
	POP	PSW		;Recover user's Device Number in C
	RET
;
;#
;	Output Byte Stream
;
;	This routine outputs initialization bytes to port
;	numbers. The byte stream has the following format
;
;		DB	ppH	Port number
;		DB	nn	Number of bytes to output
;		DB	vvH,vvH...   Bytes to be output
;		:
;		:	Repeated
;		:
;		DB	00H	Port number of 0 terminates
;
;	Entry Parameters
;
;		HL -> Byte Stream
;
Output$Byte$Stream:
OBS$Loop:
	MOV	A,M		;Get Port Number
	ORA	A		;Check if 00H (terminator)
	RZ			;Exit if at end of stream
	STA	OBS$Port	;Store in port number below
	INX	H		;HL -> Count of bytes
	MOV	C,M		;Get count
	INX	H		;HL -> First initialization byte
;
OBS$Next$Byte:
	MOV	A,M		;Get next byte
	INX	H		;HL -> next data byte (or port number)

	DB	OUT
OBS$Port:
	DB	0		;<- Setup in instruction above
	DCR	C		;Count down on byte counter
	JNZ	OBS$Next$Byte	;Output next data byte
	JMP	OBS$Loop	;Go back for next port number
;
;#
;	CONST - Console Status
;
;	This routine checks both the Forced Input pointer and
;	the Character Count for the appropriate input buffer.
;	The A register is set to indicate whether or not there
;	is data waiting.
;
;	Entry Parameters : None.
;
;	Exit Parameters
;
;		A = 000H if there is no data waiting
;		A = 0FFH if there is data waiting
;
;==========================
CONST:					;<=== BIOS Entry Point (Standard)
;==========================
	LHLD	CB$Console$Input	;Get redirection word
	LXI	D,CB$Device$Table$Addresses
	CALL	Select$Device$Table	;Get Device Table address
	JMP	Get$Input$Status	;Get status from input device
					;  and return to caller
;#
;
;	CONIN - Console Input
;
;	This routine returns the next character for the console input
;	stream. Depending on the circumstances, this can be a character
;	from the console input buffer, or from a previous stored string
;	of characters to be "forced" into the input stream.
;
;	The "forced input" can come from any previously stored character
;	string in memory. It is used to inject the current time and date
;	or a string associated with a Function Key into the console
;	stream. On system startup, a string of "SUBMIT STARTUP" is
;	forced into the console input stream to provide a mechanism
;	
;	Normal ("unforced") input comes from whichever physical device
;	is specified in the Console Input redirection word (see the
;	Configuration Block).
;
CONIN$Delay$Elapsed:	DB	0	;Flag used during Function Key
					;  processing to indicate that
					;  a predetermined delay has
					;  elapsed
;
;==========================
CONIN:					;<=== BIOS Entry Point (Standard)
;==========================
	LHLD	CB$Forced$Input		;Get the Forced Input pointer
	MOV	A,M			;Get the next character of input
	ORA	A			;Check if a null
	JZ	CONIN$No$FI		;Yes, no forced input
	INX	H			;Yes, update the pointer
	SHLD	CB$Forced$Input		;  and store it back
	RET
;
CONIN$No$FI				;No forced input
	LHLD	CB$Console$Input	;Get redirection word
	LXI	D,CB$Device$Table$Addresses
	CALL	Select$Device$Table	;Get Device Table address
	CALL	Get$Input$Character	;Get next character from input device
	
				;Function Key Processing
	CPI	Function$Key$Lead	;Check if first character of Function
					;  key sequence (normally an ESCape)
	RNZ				;Return to BIOS caller if not
	PUSH	PSW			;Save lead in character
	LXI	H,DT$Function$Delay	;Get delay time constant for
					;  delay while waiting for subsequent
					;  characters of function key sequence
					;  to arrive
	DAD	D
	MOV	C,M			;Get delay value
	MVI	B,0			;Make into word value
	XRA	A			;Indicate timer not yet timed out
	STA	CONIN$Delay$Elapsed
	LXI	H,CONIN$Set$Delay$Elapsed ;Address to resume at after delay
	CALL	Set$Watchdog		;Sets up delay based on real time
					;  clock such that control will be 
					;  transferred to specified address
					;  after time interval has elapsed
CONIN$Wait$for$Delay:			;Wait here until delay has elapsed
	LDA	CONIN$Delay$Elapsed	;Check flag set by Watchdog routine
	ORA	A
	JZ	CONIN$Wait$for$Delay

CONIN$Check$for$Function:
	LXI	H,DT$Character$Count	;Now check if the remaining characters
					;  of the sequence have been input
	DAD	D
	MOV	A,M			;Get count of characters in buffer
	CPI	Function$Key$Length - 1
	JNC	CONIN$Check$Function	;Enough characters in buffer for
					;  possible Function Key sequence
	POP	PSW			;Insufficient characters in buffer
					;  to be a Function Key, so return
					;  to caller with Lead character
	RET

;
;	The following routine is CALLed by the Watchdog routine
;	when the specified delay has elapsed.
;
CONIN$Set$Delay$Elapsed:
	MVI	A,0FFH			;Indicate Watchdog timer timed out
	STA	CONIN$Delay$Elapsed
	RET				;Return to Watchdog routine
;
;
CONIN$Check$Function:
	LXI	H,DT$Get$Offset		;Save the current "Get Pointer"
	DAD	D			;  into the buffer
	MOV	A,M			;Get the pointer
	PUSH	PSW			;Save pointer on the stack

	LXI	H,DT$Get$Offset		;Check the second (and possibly third
	CALL	Get$Address$in$Buffer	;  character in the sequence)
	MOV	B,M			;Get the second character

	IF	Three$Character$Function
	PUSH	B			;Save for later use
	LXI	H,DT$Get$Offset		;Retreive the third character
	CALL	Get$Address$in$Buffer
	POP	B			;Recover second character
	MOV	C,M			;Now BC = Char2, Char 3
	ENDIF

	PUSH	D			;Save Device Table pointer
	LXI	H,CB$Function$Key$Table - CB$Function$Key$Entry$Size
					;Get pointer to function key table
					;  in Configuration Block
	LXI	D,CB$Function$Key$Entry$Size	;Get entry size ready for loop
CONIN$Next$Function:
	DAD	D			;Move to next (or first) entry
	MOV	A,M			;Get second character of sequence
	ORA	A			;Check if end of function key table
	JZ	CONIN$Not$Function	;Yes - it is not a function key
	CMP	B			;Compare second characters
	JNZ	CONIN$Next$Function	;No match, so try next entry in table

	IF	Three$Character$Function
	INX	H			;HL -> Third character
	MOV	A,M			;Get third character of sequence
	DCX	H			;Simplify logic for 2 & 3 char seq.
	CMP	C			;Compare third characters
	JNZ	CONIN$Next$Function	;No match, so try next entry in table
	INX	H			;When match found, compensate for
					;  extra decrement
	ENDIF

	INX	H			;HL -> first character of substitute
					;  string of characters (00-byte term)
	SHLD	CB$Forced$Input		;Make the CONIN routine inject the
					;  substitute string into the input
					;  stream
	
					;Now that a Function Sequence has been
					;  identified, the stack must be 
					;  balanced prior to return
	POP	D			;Get the Device Table pointer
	POP	PSW			;Dump the Get Offset value
	POP	PSW			;Dump the Function Sequence Lead char.

	LXI	H,DT$Character$Count	;Downdate the character count
	DAD	D			;  to reflect the characters removed
					;  from the buffer
	MOV	A,M			;Get the count
	SUI	Function$Key$Length -1	;(the lead character has already
	MOV	M,A			;  been deducted)
	JMP	CONIN			;Return to CONIN processing to get
					;  the forced input characters
CONIN$Not$Function:
				;Attempts to recognize a function key sequence
				;  have failed. The Get Offset pointer must be
				;  restored to its previous value so that
				;  those character(s) presumed to be part of
				;  the function sequence are not lost.

	POP	D			;Recover Device Table pointer
	POP	PSW			;Recover previous "Get" offset
	LXI	H,DT$Get$Offset
	DAD	D			;HL -> Get offset in table
	MOV	M,A			;Reset Get offset as it was after
					;  the lead character was detected
	POP	PSW			;Recover lead character
	RET				;Return the lead character to the user
;
;#
;	Console Output
;
;	This routine outputs data characters to the console device(s).
;	It also "traps" ESCape sequences being output to the console,
;	triggering specific actions according to the sequences.
;	A primitive "state-machine" is used to step through ESCape
;	sequence recognition.
;	In addition to outputting the next character to the ALL of the
;	devices currently selected in the Console Output redirection word,
;	it checks to see that output to the selected device has not been
;	suspended by virtue of Xon/Xoff protocol, and that Data Terminal
;	Ready is high if it should be.
;	Once the character has been output, if Etx/Ack protocol is in use,
;	and the specified length of message has been output, an Etx
;	character is output and the device flagged as being suspended.
;
;	Entry Parameters
;
;		C = Character to be output
;
;	CONOUT storage variables
;
CONOUT$Character:	DB	0	;Save area for character to be output

CONOUT$Processor:	DW	CONOUT$Normal	
					;This is the address of the piece of
					;  code that will process the next
					;  character. The default case is
					;  CONOUT$Normal
CONOUT$String$Pointer:	DW	0	;This points to a string (normally
					;  in the Configuration Block) that
					;  is being preset by characters from
					;  the console output stream
CONOUT$String$Length:	DB	0	;This contains the maximum number of
					;  characters to be preset into a
					;  from the console output stream

;
;	*** WARNING ***
;	The Output Error Message routine shares the code in this
;	subroutine. On entry here, the data byte to be output
;	will be on the stack, and the DE registers setup correctly
;	*** WARNING ***
;
CONOUT$OEM$Entry:
	STA	CONOUT$Character	;Save data byte
	JMP	CONOUT$Entry2		;HL already has special bit map
;
;=====================
CONOUT:				;<=== BIOS Entry Point (Standard)
;=====================
	LHLD	CONOUT$Processor	;Get address of processor to handle
					;  the next character to be output
					;(Default is : CONOUT$Normal)
	PCHL				;Transfer control to the processor
;
;
CONOUT$Normal:				;Normal processor for console output
	MOV	A,C			;Check if possible start of ESCAPE
	CPI	Function$Key$Lead	;  sequence
	JZ	CONOUT$Escape$Found	;Perhaps
CONOUT$Forced:
	MOV	A,C			;Forced output entry point
	STA	CONOUT$Character	;Not Escape sequence - Save data byte

	LHLD	CB$Console$Output	;Get console redirection word
;
CONOUT$Entry2:			;<=== Output Error Message entry point
;
	LXI	D,CB$Device$Table$Addresses	;Addresses of Dev. Tables
	PUSH	D				;Put onto stack ready for loop
	PUSH	H

CONOUT$Next$Device:
	POP	H			;Recover Redirection bit map
	POP	D			;Recover Device Table Addresses pointer
	CALL	Select$Device$Table	;Get Device Table in DE
	ORA	A			;Check if a Device has been
					;  selected (i.e. bit map not all zero)
	JZ	CONOUT$Exit		;No, exit
	PUSH	B	;Yes - B..	;Save Redirection bit map
	PUSH	H			;Save Device Table Addresses pointer
CONOUT$Wait:
	CALL	Check$Output$Ready	;Check if device not suspended and
					;  (if appropriate) DTR is high
	JZ	CONOUT$Wait		;No, wait

	DI				;Interrupts off to avoid
					;  involuntary re-entrancy
	LDA	CONOUT$Character	;Recover the data byte
	MOV	C,A			;Ready for output
	CALL	Output$Data$Byte	;Output the data byte
	EI

	CALL	Process$Etx$Protocol	;Deal with Etx/Ack protocol
	JMP	CONOUT$Next$Device	;Loop back for next device

CONOUT$Exit:
	LDA	CONOUT$Character	;Recover data character
	MOV	A,C			;CP/M "Convention"
	RET
;
CONOUT$Escape$Found:			;Possible Escape Sequence
	LXI	H,CONOUT$Process$Escape	;Vector processing of next character
CONOUT$Set$Processor:
	SHLD	CONOUT$Processor	;Set vector address
	RET				;Return to BIOS caller
;#
;
;	Console Output : Escape Sequence Processing
;
CONOUT$Process$Escape:			;Control arrives here with character
					;  after ESCape in C
	LXI	H,CONOUT$Escape$Table	;Get base of recognition table
CONOUT$Next$Entry:
	MOV	A,M			;Check if at end of table
	ORA	A
	JZ	CONOUT$No$Match		;Yes, no match found
	CMP	C			;Compare to data character
	JZ	CONOUT$Match		;They match
	INX	H			;Move to next entry in table
	INX	H
	INX	H
	JMP	CONOUT$Next$Entry	;Go back and check again
;
CONOUT$No$Match:			;No match found, so original
					;  ESCape and following character
					;  must be output
	PUSH	B			;Save character after ESCape
	MVI	C,Function$Key$Lead	;Get ESCape character
	CALL	CONOUT$Forced		;Output to console devices
	POP	B			;Get character after ESCape
	CALL	CONOUT$Forced		;Output it, too
;
CONOUT$Set$Normal:
	LXI	H,CONOUT$Normal		;Set vector back to normal
	JMP	CONOUT$Set$Processor	;  for subsequent characters
;

CONOUT$Match:
	INX	H			;HL -> LS byte of sub-processor
	MOV	E,M			;Get LS byte
	INX	H
	MOV	D,M			;Get MS byte
	XCHG				;HL -> sub-processor
	PCHL				;Goto sub-processor
;
CONOUT$Date:			;Sub-processor to inject current date
				;  into console input stream (using
				;  forced input)
	LXI	H,Date
CONOUT$Set$Forced$Input:
	SHLD	CB$Forced$Input
	RET			;Return to BIOS' caller
;
CONOUT$Time:			;Sub-processor to inject time into
				;  console input stream
	LXI	H,Time$In$ASCII
	JMP	CONOUT$Set$Forced$Input
;
CONOUT$Set$Date:		;Sub-processor to set the date by taking
				;  the next 8 characters of console output
				;  and storing them into the date string
	LXI	H,Time$Date$Flags	;Set flag to indicate that the
	MVI	A,Date$Set		;  Date has been set by program
	ORA	M
	MOV	M,A
	MVI	A,8			;Set character count
	LXI	H,Date			;Set address
	JMP	CONOUT$Set$String$Pointer
;
;
CONOUT$Set$Time:		;Sub-processor to set the time by taking
				;  the next 8 characters of console output
				;  and storing them into the time string
	LXI	H,Time$Date$Flags	;Set flag to indicate that the
	MVI	A,Time$Set		;  Time has been set by program
	ORA	M
	MOV	M,A
	MVI	A,8			;Set character count
	LXI	H,Time$in$ASCII		;Set address
	JMP	CONOUT$Set$String$Pointer
;
CONOUT$Set$String$Pointer:		;HL -> String, A = count
	STA	CONOUT$String$Length	;Save count
	SHLD	CONOUT$String$Pointer	;Save address
	LXI	H,CONOUT$Process$String	;Vector further output
	JMP	CONOUT$Set$Processor
;
CONOUT$Process$String:		;Control arrives here for each character 
				;  in the string in register C. The 
				;  characters are stacked into the 
				;  receiving string until either a 00-byte
				;  is encountered or the specified number
				;  of characters is stacked.
	LHLD	CONOUT$String$Pointer	;Get current address for stacking chars
	MOV	A,C			;Check if current character is 00H
	ORA	A
	JZ	CONOUT$Set$Normal	;Revert to normal processing
	MOV	M,A			;Otherwise, stack character
	INX	H			;Update pointer
	MVI	M,00H			;Stack fail-safe terminator
	SHLD	CONOUT$String$Pointer	;Save updated pointer
	LXI	H,CONOUT$String$Length	;Downdate count
	DCR	M
	JZ	CONOUT$Set$Normal	;Revert to normal processing
					;  if count hits 0
	RET				;Return with output vectored back
					; to CONOUT$Process$String
;
;#
;
;	AUXIST - Auxiliary Input Status
;
;	This routine checks the Character Count in the 
;	appropriate input buffer.
;	The A register is set to indicate whether or not there
;	is data waiting.
;
;	Entry Parameters : None.
;
;	Exit Parameters
;
;		A = 000H if there is no data waiting
;		A = 0FFH if there is data waiting
;
;==========================
AUXIST:				;<=== BIOS Entry Point (Private)
;==========================
	LHLD	CB$Auxiliary$Input		;Get redirection word
	LXI	D,CB$Device$Table$Addresses	;  and table pointer
	CALL	Select$Device$Table	;Get Device Table address
	JMP	Get$Input$Status	;Get status from input device
					;  and return to caller
;
;#
;
;	Auxiliary Output Status
;
;	This routine sets the A register to indicate whether the
;	Auxiliary Device(s) is/are ready to accept output data.
;	As more than one device can be used for Auxiliary output, this
;	routine returns an Boolean AND of all of their statuses.
;
;	Entry Parameters : None
;
;	Exit Parameters
;
;		A = 000H if one or more List devices are not ready
;		A = 0FFH if all List devices are ready
;
;
;======================
AUXOST:				;<=== BIOS Entry Point (Private)
;======================
	LHLD	CB$Auxiliary$Output		;Get list redirection word
	JMP	Get$Composite$Status
;
;#
;
;	AUXIN - Auxiliary Input (Replacement for READER)
;
;	This routine returns the next input character from the
;	appropriate logical auxiliary device.
;
;	Entry Parameters : None.
;
;	Exit Parameters
;
;		A = data character
;
;==========================
AUXIN:				;<=== BIOS Entry Point (Standard)
;==========================
	LHLD	CB$Auxiliary$Input		;Get redirection word
	LXI	D,CB$Device$Table$Addresses	;  and table pointer
	CALL	Select$Device$Table	;Get Device Table address
	JMP	Get$Input$Character	;Get next input character
					;  and return to caller
;
;#
;	Auxiliary Output (Replaces PUNCH)
;
;	This routine outputs a data byte to the Auxiliary device(s).
;	It is similar to CONOUT except that it uses the Watchdog
;	Timer in order to detect if a device stays busy for more
;	than 30 seconds at a time. It outputs a message to the console
;	if this happens.
;
;	Entry Parameters
;
;		C = data byte
;
AUXOUT$Busy$Message:	DB	CR,LF,7,'Auxiliary device not Ready?',CR,LF,0
;
;=====================
AUXOUT:				;<=== BIOS Entry Point (Standard)
;=====================
	LHLD	CB$Auxiliary$Output	;Get Aux. redirection word
	LXI	D,AUXOUT$Busy$Message	;Message to be output if timeout
					;  occurs
	JMP	Multiple$Output$Byte
;
;#
;
;	List Status
;
;	This routine sets the A register to indicate whether the
;	List Device(s) is/are ready to accept output data.
;	As more than one device can be used for List output, this
;	routine returns an Boolean AND of all of their statuses.
;
;	Entry Parameters : None
;
;	Exit Parameters
;
;		A = 000H if one or more List devices are not ready
;		A = 0FFH if all List devices are ready
;
;
;======================
LISTST:					;<=== BIOS Entry Point (Standard)
;======================
	LHLD	CB$List$Output		;Get list redirection word
	JMP	Get$Composite$Status
;
;#
;	List Output
;
;	This routine outputs a data byte to the List device.
;	It is similar to CONOUT except that it uses the Watchdog
;	Timer in order to detect if the printer stays busy for more
;	than 30 seconds at a time. It outputs a message to the console
;	if this happens.
;
;	Entry Parameters
;
;		C = data byte
;
LIST$Busy$Message:	DB	CR,LF,7,'Printer not Ready?',CR,LF,0
;
;=====================
LIST:				;<=== BIOS Entry Point (Standard)
;=====================
	LHLD	CB$List$Output		;Get list redirection word
	LXI	D,LIST$Busy$Message	;Message to be output if timeout
					;  occurs
	JMP	Multiple$Output$Byte
;
;#
;	Request User Choice
;
;	This routine displays an error message, requesting
;	a choice of :
;
;		R - Retry the operation that caused the error.
;		I - Ignore the error and attempt to continue.
;		A - Abort the program and return to CP/M.
;
;	This routine accepts a character from the console,
;	folds it to upper case and returns to the caller
;	with the response in the A register.
;
RUC$Message:
		DB	CR,LF
		DB	'     Enter R - Retry, I - Ignore, A - Abort : ',0
;
;
Request$User$Choice:
	CALL	CONST			;"Gobble" up any type-ahead
	JZ	RUC$Buffer$Empty
	CALL	CONIN
	JMP	Request$User$Choice

RUC$Buffer$Empty:
	LXI	H,RUC$Message		;Display prompt
	CALL	Output$Error$Message

	CALL	CONIN			;Get console character
	CALL	A$To$Upper		;Make upper case for comparisons
	STA	Disk$Action$Confirm	;Save in confirmatory message
	PUSH	PSW			;Save for later

	LXI	H,Disk$Action$Confirm
	CALL	Output$Error$Message

	POP	PSW			;Recover action code
	RET
;
;#
;
;	Output Error Message
;
;	This routine outputs an error message to all the currently
;	selected console devices EXCEPT those being used to receive
;	LIST output as well. This is to avoid "deadly embrace" situations
;	where the printer being busy for too long causes an error message
;	to be output - and console output is being directed to the
;	printer as well....
;
;	This subroutine makes use of most of the CONOUT subroutine.
;	For reasons of memory economy it enters CONOUT using its
;	own private entry point.
;
;	Entry Parameters
;
;		HL -> 00-byte terminated error message
;
Output$Error$Message:
	PUSH	H			;Save message address
	LHLD	CB$Console$Output	;Get Console Redirection bit map
	XCHG
	LHLD	CB$List$Output		;Get List Redirection bit map
					;HL = List, DE = Console
					;Now set to 0 all bits in the Console
					;  bit map that are set to 1 in the
					;  List bit map
	MOV	A,H			;Get MS byte of List
	CMA				;Invert
	ANA	D			;Preserve only bits where 0's were
	MOV	H,A			;Save result
	MOV	A,L			;Repeat for LS byte of List
	CMA
	ANA	E
	MOV	L,A			;HL now has only pure console
					;  devices
	ORA	H			;Ensure that at least one device
	JZ	OEM$Device$Present	;  is selected
	LXI	H,0001H			;Otherwise use default of Device 0
OEM$Device$Present:
OEM$Next$Character:
	POP	D			;Recover message address into DE
	LDAX	D			;Get next byte of message
	INX	D			;Update message pointer
	ORA	A			;Check if end of message
	RZ				;Yes, exit
	PUSH	D			;Save message address for later
	PUSH	H			;Save special bit map
					;Data character is in A
	CALL	CONOUT$OEM$Entry	;Enter shared code
	POP	H			;Recover special bit map
	JMP	OEM$Next$Character
;
;
;
;	Get Composite Status
;
;	This routine sets the A register to indicate whether the
;	Output Device(s) is/are ready to accept output data.
;	As more than one device can be used for output, this
;	routine returns an Boolean AND of all of their statuses.
;
;	Entry Parameters
;
;		HL = I/O Redirection Bit Map for output device(s)
;
;	Exit Parameters
;
;		A = 000H if one or more List devices are not ready
;		A = 0FFH if all List devices are ready
;
GCS$Status:	DB	0	;Composite status of all devices
;
;
Get$Composite$Status:
	MVI	A,0FFH			;Assume all devices are ready
	STA	GCS$Status		;Preset composite status byte

	LXI	D,CB$Device$Table$Addresses	;Addresses of Dev. Tables
	PUSH	D			;Put onto stack ready for loop
	PUSH	H			;Save bit map
GCS$Next$Device:
	POP	H			;Recover Redirection bit map
	POP	D			;Recover Device Table Addresses pointer
	CALL	Select$Device$Table	;Get Device Table in DE
	ORA	A			;Check if a Device has been
					;  selected (i.e. bit map not all zero)
	JZ	GCS$Exit		;No, exit
	PUSH	B	;Yes - B..	;Save Redirection bit map
	PUSH	H			;Save Device Table Addresses pointer
	CALL	Check$Output$Ready	;Check if device ready
	LXI	H,GCS$Status		;AND together with previous devices
	ANA	M			;  status
	MOV	M,A			;Save composite status

	JMP	GCS$Next$Device		;Loop back for next device
;
GCS$Exit:
	LDA	GCS$Status		;Return with composite status
	ORA	A
	RET
;
;#
;
;	Multiple Output Byte
;
;	This routine outputs a data byte to the all of the
;	devices specified in the I/O Redirection Word.
;	It is similar to CONOUT except that it uses the Watchdog
;	Timer in order to detect if any of the devices stays busy for more
;	than 30 seconds at a time. It outputs a message to the console
;	if this happens.
;
;	Entry Parameters
;
;		HL = I/O Redirection Bit Map
;		DE -> Message to be output if timeout occurs
;		C = data byte
;
MOB$Maximum$Busy	EQU	1800	;Number of Clock Ticks (each at
					;  16.666 milliseconds) that the
					;  device might be busy for.
MOB$Character:		DB	0	;Character to be output
MOB$Busy$Message:	DW	0	;Address of message to be
					;  output if timeout occurs
MOB$Need$Message:	DB	0	;Flag used to detect that the 
					;  Watchdog timer timed out
;
Multiple$Output$Byte:
	MOV	A,C			;Get data byte
	STA	MOB$Character		;Save copy (fix: AJL 082983)
	XCHG				;HL -> Timeout message
	SHLD	MOB$Busy$Message	;Save for later use
	XCHG				;HL = Bit map again

	LXI	D,CB$Device$Table$Addresses	;Addresses of Dev. Tables
	PUSH	D			;Save on stack ready for loop
	PUSH	H			;Save I/O Redirection bit map
MOB$Next$Device:
	POP	H			;Recover Redirection bit map
	POP	D			;Recover Device Table Addresses pointer
	CALL	Select$Device$Table	;Get Device Table in DE
	ORA	A			;Check if any device selected
	JZ	MOB$Exit

	PUSH	B	;<- Yes : B	;Save Device Table Addresses pointer
	PUSH	H			;Save Redirection bit map
;
MOB$Start$Watchdog:
	XRA	A				;Reset message needed flag
	STA	MOB$Need$Message
	LXI	B,MOB$Maximum$Busy		;Time delay
	LXI	H,MOB$Not$Ready		;Address to go to
	CALL	Set$Watchdog			;Start timer

MOB$Wait:
	LDA	MOB$Need$Message		;Check if Watchdog timed out
	ORA	A
	JNZ	MOB$Output$Message		;Yes, output warning message
	CALL	Check$Output$Ready		;Check if device ready
	JZ	MOB$Wait			;No, wait
;
	DI					;Interrupts off to avoid
						;  involuntary re-entrancy
	LXI	B,0				;Turn off Watchdog
	CALL	Set$Watchdog			; (HL setting is irrelevant)

	LDA	MOB$Character			;Get data byte
	MOV	C,A
	CALL	Output$Data$Byte		;Output the data byte
	EI
	CALL	Process$Etx$Protocol		;Deal with Etx/Ack protocol
	JMP	MOB$Next$Device
;
MOB$Ignore$Exit:				;Ignore timeout error
	POP	H				;Balance the stack
	POP	D
;
MOB$Exit:
	MOV	A,C				;CP/M "Convention"
	RET
;
MOB$Output$Message:
	LHLD	MOB$Busy$Message		;Display warning message
	CALL	Output$Error$Message		;  on selected console devices
MOB$Request$Choice:
	CALL	Request$User$Choice		;Display message and get
						;  action character
	CPI	'R'				;Retry
	JZ	MOB$Start$Watchdog		;Restart Watchdog and try again
	CPI	'I'				;Ignore
	JZ	MOB$Ignore$Exit
	CPI	'A'				;Abort
	JZ	System$Reset			;  Give BDOS Function 0
	JMP	MOB$Request$Choice
;
MOB$Not$Ready:			;Watchdog timer routine will CALL this
				;  routine if the device is busy
				;  for more than approximately 30 seconds
				;Note - this is an interrupt service routine
	MVI	A,0FFH			;Set request to output message
	STA	MOB$Need$Message
	RET				;Return to the Watchdog routine
;
;#
;	Check Output Ready
;
;	This routine checks to see if the specified device is ready
;	to receive output data.
;	It does so by checking to see if the device has been suspended
;	for protocol reasons and if Data Terminal Ready is low.
;
;	NOTE : This routine does NOT check if the USART itself is ready.
;		This test is done in the Output Data Byte routine itself.
;
;	Entry Parameters
;
;		DE -> Device Table
;
;	Exit Parameters
;
;		A = 000H (Zero-flag set)   : Device not ready
;		A = 0FFH (Zero-flag clear) : Device ready
;
Check$Output$Ready:
	LXI	H,DT$Status		;Get device status
	DAD	D			;HL -> Status byte
	MOV	A,M			;Get status byte
	MOV	B,A			;Take a copy of the status byte
	ANI	DT$Output$Suspend	;Check if output is suspended
	JNZ	COR$Not$Ready		;Yes - indicate not ready

	MVI	A,DT$Output$DTR		;Check if DTR must be high to send
	ANA	B			;Mask with device status from table
	JZ	COR$Ready		;No, device is logically ready

	LXI	H,DT$Status$Port	;Setup to read device status
	DAD	D
	MOV	A,M			;Get status port number
	STA	COR$Status$Port		;Setup instruction below

	DB	IN
COR$Status$Port:
	DB	0		;<-- Setup by instruction above
	MOV	C,A			;Save hardware status

	LXI	H,DT$DTR$Ready		;Yes, setup to check chip status
	DAD	D			;  to see if DTR is high
	MOV	A,M			;Get DTR high status mask
	ANA	C			;Test chip status
	JZ	COR$Not$Ready		;DTR low - indicate not ready
;
COR$Ready:
	MVI	A,0FFH			;Indicate device ready for output
	ORA	A
	RET
;
COR$Not$Ready:				;Indicate device not ready for output
	XRA	A
	RET
;
;#
;
;	Process Etx/Ack Protocol
;
;	This routine maintains Etx/Ack protocol.
;	After a specified number of data characters have been output
;	to the device, an Etx character is output and the device
;	put into Output Suspended state. Only when an incoming
;	Ack character is received (under interrupt control) will
;	output be resumed to the device.
;
;	Entry Parameters
;
;		DE -> Device Table
;
;	Exit Parameters
;
;		Message Count downdated (& reset if necessary)
;
Process$Etx$Protocol:
	LXI	H,DT$Status		;Check if Etx/Ack protocol enabled
	DAD	D
	MOV	A,M
	ANI	DT$Output$Etx
	RZ				;No, so return immediately
	LXI	H,DT$Etx$Count		;Yes, so downdate count
	DAD	D
	PUSH	H			;Save address of count for later
	MOV	C,M			;Get LS byte
	INX	H
	MOV	B,M			;Get MS byte
	DCX	B
	MOV	A,B
	ORA	C			;Check if count now zero
	JNZ	PEP$Save$Count		;No
	LXI	H,DT$Etx$Message$Length	;Yes, reset to message length
	DAD	D
	MOV	C,M			;Get LS byte
	INX	H
	MOV	B,M			;Get MS byte
PEP$Save$Count:
	POP	H			;Recover address of count
	MOV	M,C			;Save count back in table
	INX	H
	MOV	M,B
;
	ORA	A			;Re-establish whether count hit 0
	RNZ				;No - no further processing required
	MVI	C,ETX			;Yes - send Etx to device
	DI				;Avoids involuntary re-entrancy
	CALL	Output$Data$Byte
	EI
	LXI	H,DT$Status		;Flag device as output suspended
	DAD	D
	DI				;Avoid interaction with interrupts
	MOV	A,M			;Get status byte
	ORI	DT$Output$Suspend	;Set bit
	MOV	M,A			;Save back in table
	EI
	RET
;
;#
;
;	Select Device Table
;
;	This routine scans a 16-bit word, and depending on which is the
;	first 1-bit set, selects the corresponding Device Table Address.
;
;	Entry Parameters
;
;		HL = Bit map
;		DE -> Table of Device Table Addresses
;			The first address in the list is CALLed
;			if the least significant bit of the Bit Map is
;			non-zero, and so on.
;
;	Exit Parameters
;
;		BC -> Current entry in Device Table Addresses
;		DE = Selected Device Table address
;		HL = Shifted bit map
;		Non-zero if a 1-bit was found
;		Zero if Bit Map now entirely 0000
;
;	Note : If HL is 0000H on input, then the first entry in the
;	Device Table Addresses will be returned in DE.
;
Select$Device$Table:
	MOV	A,H		;Get most significant byte of bit map
	ORA	L		;Check if HL completely 0
	RZ			;Return indicating no more bits set
	MOV	A,L		;Check if the LS bit is non-zero
	ANI	1
	JNZ	SDT$Bit$Set	;Yes, return corresponding address
	INX	D		;No, update table pointer
	INX	D
	CALL	SHLR		;Shift HL right one bit
	JMP	Select$Device$Table	;Check next bit
SDT$Bit$Set:
	PUSH	H		;Save shifted bit map
	MOV	B,D		;Take copy of table pointer
	MOV	C,E
	XCHG			;HL -> Address in Table
	MOV	E,M
	INX	H
	MOV	D,M		;DE -> Selected Device Table
				;Setup registers in case of another
				;  entry
	POP	H		;Recover shifted bit map
	CALL	SHLR		;Shift bit map right one bit
	INX	B		;Update DT Address Table pointer to
	INX	B		;  entry
	MVI	A,1		;Indicate that a one bit was found
	ORA	A		;  and registers are setup correctly
	RET
;
;#
;
;	Get Input Character
;
;	This routine gets the next input character from the device
;	specified in the Device Table handed over as an input
;	parameter.
;
Get$Input$Character:
	LXI	H,DT$Character$Count	;Check if any characters have 
	DAD	D			;  been stored in the buffer
GIC$Wait:
	EI				;Ensure that incoming chars will
					;  be detected
	MOV	A,M			;Get character count
	ORA	A
	JZ	GIC$Wait		;No characters, so wait
	DCR	M			;Down date character count for
					;  the character about to be
					;  removed from the buffer
	LXI	H,DT$Get$Offset		;Use the "GET" offset to access
	CALL	Get$Address$in$Buffer	;Returns HL -> Character
					;  and with Get Offset updated
	MOV	A,M			;Get the actual data character
	PUSH	PSW			;Save until later

	LXI	H,DT$Character$Count	;Check downdated count of chars in
	DAD	D			;  buffer, checking if input should be
					;  resumed after having been suspended
	MOV	A,M			;Get downdated count
	LXI	H,DT$Resume$Input$Count	;Check if current count matches
	DAD	D			;  buffer empty threshold
	CMP	M
	JNZ	GIC$Check$Control	;Not at threshold, check if control
					;  character input
	LXI	H,DT$Status		;At threshold, check which means
	DAD	D			;  for pausing input are to be used
	DI				;Ensure no interaction from interrupts
					;  while changing the status byte
	MOV	A,M			;Get Status/Protocol byte
	ANI	0FFH AND NOT DT$Input$Suspend	;Clear suspension flag
	MOV	M,A			;Store back updated status
	EI
	PUSH	PSW			;Save for later use
	ANI	DT$Input$RTS		;Check if Clear to Send to be raised
	JZ	GIC$Check$Input$Xon	;No
	LXI	H,DT$RTS$Control$Port	;Yes, get Control Port number
	DAD	D
	MOV	A,M
	STA	GIC$Raise$RTS$Port		;Store in instruction below
	LXI	H,DT$Raise$RTS$Value
	DAD	D
	MOV	A,M			;Get value needed to raise RTS

	DB	OUT
GIC$Raise$RTS$Port:
	DB	0		;<- Setup in instruction above
					;Drop into check for Xon
GIC$Check$Input$Xon:		;Check if Xon/Xoff protocol being used
				;  to temporarily suspend input
	POP	PSW			;Recover Status/Protocol byte
	ANI	DT$Input$Xon		;Check if Xon bit set
	JZ	GIC$Check$Control	;No, see if control char. input
	MVI	C,XON			;Yes, output Xon character
	CALL	Output$Data$Byte

GIC$Check$Control:
	CALL	Check$Control$Char	;Check if it is a Control Character
	JZ	GIC$Not$Control		;No, or it is CR, LF or TAB
	LXI	H,DT$Control$Count	;Yes, down date count of Control
	DAD	D			;  characters in buffer
	DCR	M
GIC$Not$Control:
	POP	PSW			;Recover data character
	RET

;
;#
;
;***************************************************
;*						   *
;*   Interrupt Driven Serial Input/Output Drivers  *
;*						   *
;***************************************************
;
;
;	These drivers are designed to capture incoming characters
;	using hardware-generated interrupts. Output is not
;	interrupt driven as, in general, the user program can
;	always generate output far faster than the hardware can
;	output it - therefore, the output would rapidly go into
;	a "steady state" waiting for the hardware to catch up.
;
;	The input drivers support the following features :
;
;		*  Serial Protocols for controlling output
;			DTR High to Send
;			Xon/Xoff
;			ETX/Ack with variable message length
;		*  Serial Protocols for controlling input
;			RTS high to send
;			Xon/Xoff
;
;	Input data characters are stored in a circular buffer
;	until the user program is ready to accept them. As an option,
;	the drivers can prevent overrun of this input buffer either
;	by using Request To Send low or Xoff to request that input
;	stop.
;
;
;	Character Interrupt Routine
;
;	This routine receives control when the hardware detects an
;	incoming data character. The hardware chips and necessary
;	memory locations will have been setup during the initialization
;	code.
;
;	Because the actual hardware architecture can vary so much,
;	this routine has been written to show the generic processing
;	required. It assumes a "flat" interrupt structure that transfers
;	control to this routine regardless of which device is actually
;	interrupting. It also assumes that once the data character has
;	been input, a specific value must be output to a particular port
;	in order to recondition the hardware ready for the next interrupt.
;#
;
Character$Interrupt:
	PUSH	H		;Save just HL on user's stack
	LXI	H,0		;Get current stack pointer value
	DAD	SP
	SHLD	PI$User$Stack	;Save user's stack
	LXI	SP,PI$Stack	;Switch to local stack
	PUSH	PSW		;Save remaining registers
	PUSH	B
	PUSH	D

			;Now check each device in turn for incoming
			;	data characters
	LXI	D,DT$0		;Device 0
	CALL	Service$Device	;If interrupting, input the data character
				;  and process according to what the
				;  character is and the device's status
	LXI	D,DT$1		;Device 1
	CALL	Service$Device

	LXI	D,DT$2		;Device 2
	CALL	Service$Device

	MVI	A,IC$EOI	;Tell the Interrupt Controller chip
	OUT	IC$OCW2$Port	;  that the interrupt has been serviced
	POP	D		;Restore registers
	POP	B
	POP	PSW
	LHLD	PI$User$Stack	;Switch back to user's stack
	SPHL
	POP	H
	EI			;Re-enable interrupts in the CPU
	RET			;Resume pre-interrupt processing
;
;#
;
;	Service Device
;
;	This routine performs the device interrupt servicing,
;	checking to see if the device described in the specified
;	Device Table (address in DE) is actually interrupting,
;	and if so, inputs the character. Depending on which data character
;	is input, this routine will either stack it in the input buffer
;	(shutting off the input stream if the buffer is nearly full),
;	or will suspend or de-suspend the output to the device.
;
;	Entry Parameters
;
;		DE -> Device Table
;
Service$Device:
	LXI	H,DT$Status$Port		;Check if this device is really
	DAD	D			;  interrupting
	MOV	A,M			;Get status port number
	STA	SD$Status$Port		;Store into instruction below

	DB	IN			;Input Status
SD$Status$Port:
	DB	0	;<-- Setup by instruction above
;
	LXI	H,DT$Input$Ready	;Check if status indicates data ready
	DAD	D
	ANA	M			;Mask with Input Ready value
	RZ				;No, return to Interrupt Service
					;Check if any errors have occurred
	LXI	H,DT$Detect$Error$Port	;Setup to read error status
	DAD	D			;  interrupting
	MOV	A,M			;Get status port number
	STA	SD$Error$Port		;Store into instruction below

	DB	IN			;Input Error Status
SD$Error$Port:
	DB	0	;<-- Setup by instruction above
;
	LXI	H,DT$Detect$Error$Value	;Mask with error bit(s)
	DAD	D
	ANA	M
	JZ	SD$No$Error		;No bit(s) set
	LXI	H,DT$Reset$Error$Port	;Setup to reset error
	DAD	D
	MOV	A,M			;Get Reset Port number
	STA	SD$Reset$Error$Port	;Store in instruction below
	LXI	H,DT$Reset$Error$Value
	DAD	D
	MOV	A,M			;Get reset interrupt value

	DB	OUT
SD$Reset$Error$Port:
	DB	0	;<-- Setup in instruction above

SD$No$Error:
	LXI	H,DT$Data$Port		;Input the data character (this may
	DAD	D			;be garbled if an error occurred)
	MOV	A,M			;Get data port number
	STA	SD$Data$Port		;Store into instruction below

	DB	IN			;Input data character
SD$Data$Port:
	DB	0	;<-- Setup by instruction above

	MOV	B,A			;Take copy of data character above
	LXI	H,DT$Status		;Check if either Xon or Etx protocols
	DAD	D			;  are currently active
	MOV	A,M			;Get protocol byte
	ANI	DT$Output$Xon + DT$Output$Etx
	JZ	SD$No$Protocol		;Neither are active
	ANI	DT$Output$Xon		;Check if Xon/Xoff is active
	JNZ	SD$Check$if$Xon		;Yes, check if Xon char input
					;No, assume Etx/Ack active
	MVI	A,ACK			;Check if input character is ACK
	CMP	B
	JNZ	SD$No$Protocol		;No, process character as data
SD$Output$Desuspend:			;Yes, device now ready
					;  to accept more data, so indicate
					;  output to device is de-suspended.
					;The non-interrupt driven output
					;  routine checks the suspend bit.
	MOV	A,M			;Get Status/Protocol byte again
	ANI	0FFH AND NOT DT$Output$Suspend ;Preserve all bits BUT suspend
	MOV	M,A			;Save back with suspend = 0
	JMP	SD$Exit			;Exit to interrupt service without
					;  saving data character
;
SD$Check$if$Xon:			;Xon/Xoff protocol active, so
					;  if Xoff received, suspend output
					;  if Xon, received, de-suspend output.
					;The non-interrupt driven output
					;  routine checks the suspend bit.
	MVI	A,XON			;Check if XON character input
	CMP	B
	JZ	SD$Output$Desuspend	;Yes, enable output to device
	MVI	A,XOFF			;Check if XOFF character input
	CMP	B
	JNZ	SD$No$Protocol		;No, process character as data
SD$Output$Suspend:			;Device needs pause in output of
					;  data, so indicate output suspended
	MOV	A,M			;Get Status/Protocol byte again
	ORI	DT$Output$Suspend	;Set suspend bit to 1
	MOV	M,A			;Save back in device table
	JMP	SD$Exit			;Exit to interrupt service without
					;  saving the input character.
;
SD$No$Protocol:
	LXI	H,DT$Buffer$Length$Mask	;Check if there is still space
	DAD	D			;  in the input buffer
	MOV	A,M			;Get length - 1
	INR	A			;Update to actual length
	LXI	H,DT$Character$Count	;Get current count of characters
	DAD	D			;  in buffer
	CMP	M			;Check if count = length
	JZ	SD$Buffer$Full		;Yes, output bell character
	PUSH	B			;Save data character
	LXI	H,DT$Put$Offset		;Compute address of character in
					;  input buffer
	CALL	Get$Address$In$Buffer	;HL -> Character position
	POP	B			;Recover input character
	MOV	M,B			;Save character in input buffer
				;Update number of characters in input
				;  buffer, checking if input should
				;  temporarily halted
	LXI	H,DT$Character$Count
	DAD	D
	INR	M			;Update character count
	MOV	A,M			;Get updated count
	LXI	H,DT$Stop$Input$Count	;Check if current count matches
	DAD	D			;  buffer full threshold
	CMP	M
	JNZ	SD$Check$Control	;Not at threshold, check if control
					;  character input
	LXI	H,DT$Status		;At threshold, check which means
	DAD	D			;  for pausing input are to be used
	MOV	A,M			;Get Status/Protocol byte
	ORI	DT$Input$Suspend	;Indicate input is suspended
	MOV	M,A			;Save updated status in table
	PUSH	PSW			;Save for later use
	ANI	DT$Input$RTS		;Check if Clear to Send to be dropped
	JZ	SD$Check$Input$Xon	;No
	LXI	H,DT$RTS$Control$Port	;Yes, get Control Port number
	DAD	D
	MOV	A,M
	STA	SD$Drop$RTS$Port		;Store in instruction below
	LXI	H,DT$Drop$RTS$Value
	DAD	D
	MOV	A,M			;Get value needed to drop RTS

	DB	OUT
SD$Drop$RTS$Port:
	DB	0		;<- Setup in instruction above
					;Drop into Input Xon test
SD$Check$Input$Xon:		;Check if Xon/Xoff protocol being used
				;  to temporarily suspend input
	POP	PSW			;Recover Status/Protocol byte
	ANI	DT$Input$Xon		;Check if Xon bit set
	JZ	SD$Check$Control	;No, see if control char. input
	MVI	C,XOFF			;Yes, output Xoff character
	CALL	Output$Data$Byte	;Output Data Byte
;
SD$Check$Control:		;Check if Control Character (other than
				;  CR, LF or Tab) input, and update
				;  count of control characters in buffer
	CALL	Check$Control$Char	;Check if Control Character
	JZ	SD$Exit			;No it is not a Control Character
	LXI	H,DT$Control$Count
	DAD	D
	INR	M			;Update count of control chars.
;
SD$Exit:			;Reset hardware interrupt system
	LXI	H,DT$Reset$Int$Port
	DAD	D
	MOV	A,M			;Get Reset Port number
	ORA	A			;Check if Port specified
					;  (assumes it will always be NZ)
	RZ				;Bypass reset if no port specified
	STA	SD$Reset$Int$Port	;Store in instruction below
	LXI	H,DT$Reset$Int$Value
	DAD	D
	MOV	A,M			;Get reset interrupt value

	DB	OUT
SD$Reset$Int$Port:
	DB	0	;<-- Setup in instruction above
	RET				;Return to interrupt service routine
;
SD$Buffer$Full:				;Input buffer completely full
	MVI	C,BELL			;Send bell character as desparate
	JMP	Output$Data$Byte	;  measure. Note JMP - return to
					;  caller will be done by subroutine
;
;#
;
;	Get Address in Buffer
;
;	This routine computes the address of the next character to
;	access in a Device Buffer.
;
;	Entry Parameters
;
;		DE -> Appropriate Device Table
;		HL = The offset in the Device Table of either the
;			Get$Offset or the Put$Offset
;
;	Exit Parameters
;
;		DE unchanged
;		HL -> Address in Character Buffer
;
Get$Address$In$Buffer:
	DAD	D			;HL -> Get/Put Offset in Dev. Table
	PUSH	H			;Preserve pointer to table
	MOV	C,M			;Get offset value
	MVI	B,0			;Make into word value
				;Update offset value, resetting to
				;  0 at end of buffer
	MOV	A,C			;Get copy of offset
	INR	A			;Update to next position
	LXI	H,DT$Buffer$Length$Mask
	DAD	D
	ANA	M			;Mask LS bits with length - 1
	POP	H			;Recover pointer to offset in table
	MOV	M,A			;Save new value (set to 0 if nec.)
	LXI	H,DT$Buffer$Base	;Get Base address of input buffer
	DAD	D			;HL -> Address of Buffer in table
	MOV	A,M			;Get LS byte of address
	INX	H			;HL -> MS byte of address
	MOV	H,M			;H = MS byte
	MOV	L,A			;L = LS byte
	DAD	B			;Add on offset to base
	RET

;
;#
;
;	Check Control Character
;
;	This routine checks the character in A to see if it is a
;	Control Character other than CR, LF or TAB. The result is
;	returned in the Z-flag.
;
;	Entry Parameters
;
;		A = Character to be checked
;
;	Exit Parameters
;
;		Zero status if A does not contain a Control Character
;			or if it is CR, LF or TAB
;
;		Non-zero if A contains a Control Character other than
;			CR, LF, or TAB.
Check$Control$Char:
	MVI	A,' ' - 1		;Space is first non-control char.
	CMP	B
	JC	CCC$No			;Not a control character
	MVI	A,CR			;Check if Carriage Return
	CMP	B
	JZ	CCC$No			;Not really a Control Character
	MVI	A,LF			;Check if Line Feed
	CMP	B
	JZ	CCC$No			;Not really a Control Character
	MVI	A,TAB			;Check if Horizontal Tab
	CMP	B
	JZ	CCC$No			;Not really a Control Character
	MVI	A,1			;Indicate a Control Character
	ORA	A
	RET
CCC$No:					;Indicate A does not contain
	XRA	A			;  a Control Character
	RET
;
;#
;
;	Output Data Byte
;
;	This is a simple polled output routine that outputs a single
;	character (in register C on entry) to the device specified in
;	the Device Table defined.
;	Preferably, this routine would have been re-entrant, however
;	it does have to store the port numbers. Therefore, to use it
;	from code executed with interrupts enabled, the instruction
;	sequence must be :
;
;		DI		;Interrupts off
;		CALL	Output$Data$Byte
;		EI		;Interrupts on
;
;	Failure to do so may cause involuntary re-entrancy.
;
;	Entry Parameters
;
;		C = Character to be output
;		DE -> Device Table
;
Output$Data$Byte:
	PUSH	B			;Save registers
	LXI	H,DT$Output$Ready	;Get output ready status mask
	DAD	D
	MOV	B,M
	LXI	H,DT$Status$Port	;Get status port number
	DAD	D
	MOV	A,M
	STA	ODB$Status$Port		;Store in instruction below
ODB$Wait$until$Ready:

	DB	IN			;Read status
ODB$Status$Port:
	DB	0	;<--  Setup in instruction above

	ANA	B			;Check if ready for output
	JZ	ODB$Wait$until$Ready	;No
	LXI	H,DT$Data$Port		;Get Data Port
	DAD	D
	MOV	A,M
	STA	ODB$Data$Port		;Store in instruction below
	MOV	A,C			;Get character to output

	DB	OUT
ODB$Data$Port:
	DB	0	;<--  Setup in instruction above

	POP	B			;Restore registers
	RET
;
;#
;
;
;	Input Status Routine
;
;	This routine returns a value in the A register indicating whether
;	there is one or more data characters waiting in the input buffer.
;	Some products, such as Microsoft BASIC, defeat normal type-ahead
;	by constantly "gobbling" characters in order to see if an incoming
;	Control-S, -Q or -C has been received. In order to preserve
;	type-ahead under these circumstances, the Input Status return
;	can, as an option selected by the user, only return "data waiting"
;	if the input buffer contains a Control-S, -Q or -C. This fools
;	Microsoft BASIC into allowing type ahead.
;
;	Entry Parameters
;
;		DE -> Device Table
;
;	Exit Parameters
;
;		A = 000H if no characters are waiting in the input
;			buffer
;
;
Get$Input$Status:
	LXI	H,DT$Status$2		;Check if Fake mode enabled
	DAD	D			;HL -> status byte in table
	MOV	A,M			;Get status byte
	ANI	DT$Fake$Typeahead	;Isolate status bit
	JZ	GIS$True$Status		;Fake mode disabled
				;
				;Fake mode - only indicates data
				;	ready if control chars in buffer
	LXI	H,DT$Control$Count	;Check if any control characters
	DAD	D			;in the input buffer
	XRA	A			;Cheap 0
	ORA	M			;Set flags according to count
	RZ				;Return indicating zero
GIS$Data$Ready:
	XRA	A			;Cheap 0
	DCR	A			;Set A = 0FFH and flags NZ
	RET				;Return to caller
;
GIS$True$Status:		;
				;True status, based on any characters
				;	ready in input buffer
	LHLD	CB$Forced$Input		;Check if any forced input waiting
	MOV	A,M			;Get next character of forced input
	ORA	A			;Check if non-zero
	JNZ	GIS$Data$Ready		;Yes - indicate data waiting

	LXI	H,DT$Character$Count	;Check if any characters at all
	DAD	D			;present in buffer
	MOV	A,M			;Get character count
	ORA	A
	RZ				;Empty buffer, A = 0, Z-set
	JMP	GIS$Data$Ready
;
;
;#
;
;	Real Time Clock Processing
;
;	Control is transferred to the RTC$Interrupt routine each time
;	the real time clock ticks. The tick count is downdated in order
;	to see if a complete second has elapsed - if so, the ASCII time
;	in the configuration block is updated.
;
;	Each tick, the Watchdog count is downdated to see if control
;	must be "forced" to a previously specified address on return
;	from the RTC interrupt. The Watchdog timer can be used to pull
;	control out of what would otherwise be an infinite loop - such
;	as waiting for the printer to come ready.
;
;
;	Set Watchdog
;
;	This is a non-interrupt level subroutine that simply sets the
;	Watchdog count and address
;
;	Entry Parameters
;
;		BC = Number of clock ticks before watchdog should
;			time out
;		HL = Address to which control will be transferred when
;			Watchdog times out
;
Set$Watchdog:
	DI				;Avoid interference from interrupts
	SHLD	RTC$Watchdog$Address	;Set address
	MOV	H,B
	MOV	L,C
	SHLD	RTC$Watchdog$Count	;Set count
	EI
	RET
;
;	
;#
;
			;Control is received here each time the
			;  Real Time Clock ticks
RTC$Interrupt:
	PUSH	PSW			;Save other registers
	SHLD	PI$User$HL		;Switch to local stack
	LXI	H,0
	DAD	SP			;Get user's stack
	SHLD	PI$User$Stack		;Save it
	LXI	SP,PI$Stack		;Switch to local stack
	PUSH	B
	PUSH	D

	LXI	H,RTC$Tick$Count	;Downdate tick count
	DCR	M
	JNZ	RTC$Check$Watchdog	;Is not at 0 yet
					;One second has elapsed so
	LDA	RTC$Ticks$per$Second	;Reset to original value
	MOV	M,A
					;Update ASCII Real Time Clock
	LXI	D,Time$in$ASCII$End	;DE -> 1 character after ASCII time
	LXI	H,Update$Time$End	;HL -> 1 character after control table
RTC$Update$Digit:
	DCX	D			;Downdate pointer to Time in ASCII
	DCX	H			;Downdate pointer to control table
	MOV	A,M			;Get next control character
	ORA	A			;Check if end of table and therefore
	JZ	RTC$Clock$Updated	;  all digits of clock updated
	JM	RTC$Update$Digit	;Skip over ":" in ASCII time
	LDAX	D			;Get next ASCII time digit
	INR	A			;Update it
	STAX	D			;  and store it back
	CMP	M			;Compare to maximum value
	JNZ	RTC$Clock$Updated	;No carry needed so update complete
	MVI	A,'0'			;Reset digit to ASCII 0
	STAX	D			;  and store back in ASCII time
	JMP	RTC$Update$Digit	;Go back for next digit
;
RTC$Clock$Updated:
RTC$Check$Watchdog:
	LHLD	RTC$Watchdog$Count	;Get current watchdog count
	DCX	H			;Downdate it
	MOV	A,H			;Check if it is now 0FFFFH
	ORA	A
	JM	RTC$Dog$Not$Set		;It must have been 0 beforehand
	ORA	L			;Check if it is now 0
	JNZ	RTC$Dog$NZ		;No - it has not timed out yet

					;Watchdog timed out, so "CALL"
					;  appropriate routine
	LXI	H,RTC$Watchdog$Return	;Setup return address
	PUSH	H			;  ready for RETurn
	LHLD	RTC$Watchdog$Address	;Transfer control as though by CALL
	PCHL
RTC$Watchdog$Return:			;Control will come back here from
					;  the user's Watchdog routine
	JMP	RTC$Dog$Not$Set		;Behave as though Watchdog not active

RTC$Dog$NZ:
	SHLD	RTC$Watchdog$Count	;Save downdated count
RTC$Dog$Not$Set:			;(Leaves count unchanged)
	MVI	A,IC$EOI		;Reset the interrupt controller chip
	OUT	IC$OCW2$Port

	POP	D			;Restore registers from local stack
	POP	B
	LHLD	PI$User$Stack		;Switch back to user's stack
	SPHL
	LHLD	PI$User$HL		;Recover user's registers
	POP	PSW
	EI				;Re-enable interrupts
	RET
;
;#
;
;	Shift HL Right one bit
;
SHLR:
	ORA	A		;Clear Carry
	MOV	A,H		;Get MS Byte
	RAR			;Bit 7 set from previous Carry
				;Bit 0 goes into Carry
	MOV	H,A		;Put shifted MS byte back
	MOV	A,L		;Get LS Byte
	RAR			;Bit 7 = Bit 0 of MS Byte
	MOV	L,A		;Put back into result
	RET

;
;#
;	High Level Diskette Drivers
;
;	These drivers perform the following functions :
;
;	SELDSK	Select a specified disk and return the address of
;		the appropriate Disk Parameter Header.
;	SETTRK	Set the Track number for the next Read or Write.
;	SETSEC	Set the Sector number for the next Read or Write.
;	SETDMA	Set the DMA (Read/Write) address for the next Read or Write.
;	SECTRAN	Translate a logical sector number into a physical
;	HOME	Set the Track to 0 so that the next Read or Write will
;		be on Track 0.
;
;	In addition, the high level drivers are responsible for making
;	the 5 1/4" floppy diskettes that use a 512-byte sector appear
;	to CP/M as though they used a 128-byte sector. They do this
;	by using what is called Blocking/Deblocking code. This
;	Blocking/Deblocking code is described in more detail later in
;	this listing, just prior to the code itself.
;
;
;
;	Disk Parameter Tables
;
;	As discussed in Chapter 3, these describe the physical 
;	characteristics of the disk drives. In this example BIOS,
;	there are two types of disk drives, standard single-sided,
;	single-density 8", and double-sided, double density 5 1/4"
;	mini diskettes.
;
;	The standard 8" diskettes do not need to use the Blocking/
;	Deblocking code but the 5 1/4" drives do. Therefore an additional
;	byte has been prefixed onto the Disk Parameter Block to 
;	tell the disk drivers what each logical disk's physical
;	diskette type is, and whether or not it needs deblocking.
;
;
;	Disk Definition Tables
;
;	These consist of Disk Parameter Headers, with one entry
;	per logical disk driver, and Disk Parameter Blocks with
;	either one Parameter Block per logical disk, or the same
;	Parameter Block for several logical disks.
;
;#
;
Disk$Parameter$Headers:			;Described in Chapter 3
;
			;Logical Disk A: (5 1/4" Diskette)
	DW	Floppy$5$Skewtable	;5 1/4" Skew Table
	DW	0,0,0			;Reserved for CP/M
	DW	Directory$Buffer
	DW	Floppy$5$Parameter$Block
	DW	Disk$A$Workarea
	DW	Disk$A$Allocation$Vector	
;
			;Logical Disk B: (5 1/4" Diskette)
	DW	Floppy$5$Skewtable	;Shares same Skew Table as A:
	DW	0,0,0			;Reserved for CP/M
	DW	Directory$Buffer	;Share same buffer as A:
	DW	Floppy$5$Parameter$Block	;Same DPB as A:
	DW	Disk$B$Workarea			;Private workarea 
	DW	Disk$B$Allocation$Vector	;Private allocation vector 
;
			;Logical Disk C: (8" Floppy)
	DW	Floppy$8$Skewtable	;8" Skew Table
	DW	0,0,0			;Reserved for CP/M
	DW	Directory$Buffer	;Share same buffer as A:
	DW	Floppy$8$Parameter$Block
	DW	Disk$C$Workarea			;Private workarea
	DW	Disk$C$Allocation$Vector	;Private allocation vector
;
			;Logical Disk D: (8" Floppy)
	DW	Floppy$5$Skewtable	;Shares same Skew Table as A:
	DW	0,0,0			;Reserved for CP/M
	DW	Directory$Buffer	;Share same buffer as A:
	DW	Floppy$8$Parameter$Block	;Same DPB as C:
	DW	Disk$D$Workarea			;Private workarea
	DW	Disk$D$Allocation$Vector	;Private allocation vector

			;Logical Disk M: (Memory Disk)
M$Disk$DPH:
	DW	0			;No Skew required
	DW	0,0,0			;Reserved for CP/M
	DW	Directory$Buffer
	DW	M$Disk$Parameter$Block
	DW	0			;Disk cannot be changed - therefore
					;  no workarea is required
	DW	M$Disk$Allocation$Vector
;
;
;	Equates for Disk Parameter Block
;
;	Disk Types
;
Floppy$5	EQU	1	;5 1/4" Mini Floppy
Floppy$8	EQU	2	;8" Floppy (SS SD)
M$Disk		EQU	3	;Memory Disk
;
;	Blocking/Deblocking Indicator
;
Need$Deblocking	EQU	1000$0000B	;Sector size > 128 bytes
;
;#
;
;	Disk Parameter Blocks
;
;	5 1/4" Mini Floppy
;
				;Extra byte prefixed to indicate
				;disk type and blocking required.
	DB	Floppy$5 + Need$Deblocking
				;The parameter block has been amended
				;  to reflect the new layout of one
				;  track per diskette side - rather
				;  than viewing one track as both
				;  sides on a given head position.
				;It has also been adjusted to reflect
				;  one "new" track more being used for
				;  the CP/M image, with the resulting
				;  change in the number of allocation
				;  blocks and the number of reserved
				;  tracks.
Floppy$5$Parameter$Block:
	DW	36		;128-byte Sectors per Track
	DB	4		;Block Shift
	DB	15		;Block Mask
	DB	1		;Extent Mask
	DW	171		;Maximum Allocation Block Number
	DW	127		;Number of Directory Entries - 1
	DB	1100$0000B	;Bit Map for reserving 1 Alloc. Block
	DB	0000$0000B	;  for File Directory
	DW	32		;Disk Changed Workarea size
	DW	3		;Number of Tracks before Directory
;
;
;	Standard 8" Floppy
				;Extra byte prefixed to DPB for
				;this version of the BIOS
	DB	Floppy$8	;Indicates disk type and the fact
				;that no deblocking is required
Floppy$8$Parameter$Block:
	DW	26		;Sectors per Track
	DB	3		;Block Shift
	DB	7		;Block Mask
	DB	0		;Extent Mask
	DW	242		;Maximum Allocation Block Number
	DW	63		;Number of Directory Entries - 1
	DB	1100$0000B	;Bit Map for reserving 2 Alloc. Blocks
	DB	0000$0000B	;  for File Directory
	DW	16		;Disk Changed Workarea size
	DW	2		;Number of Tracks before Directory
;
;	M$Disk
;
				;The M$Disk presumes that 4 x 48K memory
				;  banks are available - the following
				;  table describes the disk as having
				;  8 tracks, two tracks per memory bank
				;  with each track having 192 128-byte
				;  sectors.
				;  The track number divided by 2 will be
				;  used to do bank select.
	DB	M$Disk		;Type is M$Disk - no deblocking
M$Disk$Parameter$Block:
	DW	192		;Sectors per "Track". Each track is
				;  a 24K of memory
	DB	3		;Block Shift (1024 byte Allocation)
	DB	7		;Block Mask
	DB	0		;Extent Mask
	DW	192		;Maximum Allocation Block Number
	DW	63		;Number of Directory Entries -1
	DB	1100$0000B	;Bit Map for reserving 2 Allocation Blocks
	DB	0000$0000B	;  for File Directory
	DW	0		;Disk Cannot be changed - therefore no
				;  work area
	DW	0		;No reserved tracks
;
Number$of$Logical$Disks		EQU	4
;
;#
;
SELDSK:		;Select disk in Register C
			;C = 0 for drive A, 1 for B, etc.
			;Return the address of the appropriate
			;Disk Parameter Header in HL, or 0000H
			;if the Selected Disk does not exist.	
			;
	LXI	H,0		;Assume an error
	MOV	A,C		;Check if requested disk valid

	CPI	'M' - 'A'	;Check if Memory Disk
	JZ	SELDSK$M$Disk	;Yes

	CPI	Number$of$Logical$Disks
	RNC			;Return if > Maximum number of disks
;
	STA	Selected$Disk	;Save Selected Disk number
				;Set up to return DPH address
	MOV	L,A		;Make disk into word value
	MVI	H,0
				;Compute offset down Disk Parameter
				;Header table by multiplying by
				;Parameter Header length (16 bytes)
	DAD	H		;*2
	DAD	H		;*4
	DAD	H		;*8
	DAD	H		;*16
	LXI	D,Disk$Parameter$Headers	;Get Base address
	DAD	D		;DE -> Appropriate DPH
	PUSH	H		;Save DPH address
;
				;Access Disk Parameter Block in order
				;to extract special prefix byte that
				;identifies disk type and whether
				;Deblocking is required
				;			
	LXI	D,10		;Get DPB pointer offset in DPH
	DAD	D		;DE -> DPB address in DPH
	MOV	E,M		;Get DPB address in DE
	INX	H
	MOV	D,M
	XCHG			;DE -> DPB

SELDSK$Set$Disk$Type:
	DCX	H		;DE -> Prefix Byte
	MOV	A,M		;Get Prefix Byte
	ANI	0FH		;Isolate Disk Type
	STA	Selected$Disk$Type	;Save for use in low level driver
	MOV	A,M		;Get another copy of Prefix Byte
	ANI	Need$Deblocking		;Isolate Deblocking flag
	STA	Selected$Disk$Deblock	;Save for use in low level driver
	POP	H		;Recover DPH Pointer
	RET
;
SELDSK$M$Disk:				;M$Disk Selected
	LXI	H,M$Disk$DPH		;Return correct Parameter Header
	JMP	SELDSK$Set$Disk$Type	;Resume normal processing
;
;#
;
;	Set Logical Track for next Read or Write
;
SETTRK:
	MOV	H,B		;Selected Track in BC on entry
	MOV	L,C
	SHLD	Selected$Track	;Save for low level driver
	RET
;
;#
;
;	Set Logical Sector for next Read or Write
;
;
SETSEC:				;Logical sector in C on entry
	MOV	A,C
	STA	Selected$Sector	;Save for low level driver
	RET
;
;#
;
;	Set Disk DMA (Input/Output) Address for next Read or Write
;
DMA$Address:	DW	0		;DMA Address
;
SETDMA:				;Address in BC on entry
	MOV	L,C		;Move to HL to save
	MOV	H,B
	SHLD	DMA$Address	;Save for low level driver
	RET
;
;#
;
;	Translate logical sector number to physical
;
;	Sector Translation Tables
;	These tables are indexed using the logical sector number,
;	and contain the corresponding physical sector number.
;
Floppy$5$Skewtable:	;Each physical sector contains four
			;128-byte sectors.
	;	Physical 128b	Logical 128b       Physical 512-byte
	DB	00,01,02,03	;00,01,02,03		0  )
	DB	16,17,18,19	;04,05,06,07		4  )
	DB	32,33,34,35	;08,09,10,11		8  )
	DB	12,13,14,15	;12,13,14,15		3  ) Head
	DB	28,29,30,31	;16,17,18,19		7  )  0
	DB	08,09,10,11	;20,21,22,23		2  )
	DB	24,25,26,27	;24,25,26,27		6  )
	DB	04,05,06,07	;28,29,30,31		1  )
	DB	20,21,22,23	;32,33,34,35		5  )
;
	DB	36,37,38,39	;36,37,38,39		0  ]
	DB	52,53,54,55	;40,41,42,43		4  ] 
	DB	68,69,70,71	;44,45,46,47		8  ]
	DB	48,49,50,51	;48,49,50,51		3  ] Head
	DB	64,65,66,67	;52,53,54,55		7  ]  1
	DB	44,45,46,47	;56,57,58,59		2  ]
	DB	60,61,62,63	;60,61,62,63		6  ]
	DB	40,41,42,43	;64,65,66,67		1  ]
	DB	56,57,58,59	;68,69,70,71		5  ]
;
;
Floppy$8$Skewtable:	;Standard 8" Driver
	;	01,02,03,04,05,06,07,08,09,10	 Logical Sectors
	DB	01,07,13,19,25,05,11,17,23,03	;Physical Sectors
	;
	;	11,12,13,14,15,16,17,18,19,20	 Logical Sectors	
	DB	09,15,21,02,08,14,20,26,06,12	;Physical Sectors
	;
	;	21,22,23,24,25,26	 Logical Sectors
	DB	18,24,04,10,16,22	;Physical Sectors

;#
;
SECTRAN:			;Translate logical sector into physical
			;On entry, BC = Logical Sector number
			;	   DE -> Appropriate Skew Table
			;
			;on exit, HL = Physical Sector number
	XCHG			;HL -> Skew Table base
	DAD	B		;Add on Logical Sector number
	MOV	L,M		;Get Physical Sector number
	MVI	H,0		;Make into a 16-bit value
	RET
;	
;#
;
;
HOME:			;Home the selected logical disk to track 0.
			;Before doing this, a check must be made to see
			;if the Physical Disk Buffer has information in
			;it that must be written out. This is indicated by
			;a flag, Must$Write$Buffer, that is set in the
			;Deblocking code.
			;
	LDA	Must$Write$Buffer	;Check if Physical Buffer must
	ORA	A			;  be written out to disk
	JNZ	HOME$No$Write
	STA	Data$In$Disk$Buffer	;No, so indicate that buffer
					;  is now unoccupied.
HOME$No$Write:
	MVI	C,0			;Set to track 0 (logically -
	CALL	SETTRK			;  no actual disk operation occurs)
	RET

;
;#
;	Data written to or read from the mini-floppy drive is transferred
;	via a Physical Buffer that is one complete track in length,
;	9 * 512 bytes. It is declared at the end of the BIOS, and has
;	some small amount of initialization code "hidden" in it.
;
;	The Blocking/Deblocking code attempts to minimise the amount
;	of actual disk I/O by storing the disk and  track 
;	currently residing in the Physical Buffer. 
;	If a read request occurs of a 128-byte CP/M 'sector' 
;	that already is in the Physical Buffer, no disk access occurs.
;	If a write request occurs, if the 128-byte CP/M 'sector'
;	is already in the Physical Buffer, no disk access will occur,
;	UNLESS the BDOS indicates that it is writing to the directory.
;	Directory writes cause an immediate write to disk of the entire
;	track in the Physical Buffer.
;
;
Allocation$Block$Size	EQU	2048
Physical$Sec$Per$Track	EQU	9	;Adjusted to reflect a "new"
					;  track is only one side of the
					;  disk.
Physical$Sector$Size	EQU	512	;This is the actual sector size
					;  for the 5 1/4" mini-floppy diskettes.
					;The 8" diskettes and Memory Disk
					;  use 128-byte sectors.
					;Declare the Physical Disk Buffer for the
					;5 1/4" Diskettes
CPM$Sec$Per$Physical	EQU	Physical$Sector$Size/128
CPM$Sec$Per$Track	EQU	CPM$Sec$Per$Physical*Physical$Sec$Per$Track
Bytes$Per$Track		EQU	Physical$Sec$Per$Track*Physical$Sector$Size
Sector$Mask		EQU	CPM$Sec$Per$Physical-1
Sector$Bit$Shift	EQU	2		;LOG2(CPM$Sec$Per$Physical)
;
			;These are the values handed over by the BDOS
			;  when it calls the WRITE operation.
			;The allocated/unallocated indicates whether the
			;  BDOS wishes to write to an unallocated Allocation
			;  block (it only indicates this for the first
			;  128-byte sector write), or to an Allocation block
			;  that has already been allocated to a file.
			;The BDOS also indicates if it wishes to write to 
			;  the File Directory.
			;
Write$Allocated		EQU	0
Write$Directory		EQU	1
Write$Unallocated	EQU	2	;<== Ignored for track buffering
;
Write$Type:		DB	0	;Contains the type of write as
					;  indicated by the BDOS.
;
;
In$Buffer$Dk$Trk:		;Variables for Physical sector currently
				;  in Disk$Buffer in memory
In$Buffer$Disk: 	DB	0	;) These are moved and compared
In$Buffer$Track: 	DW	0	;) as a group, so do not alter
					; these lines.
In$Buffer$Disk$Type:	DB	0	;Disk Type for sector in buffer
;
Data$In$Disk$Buffer: 	DB	0	;When Non-zero, the Disk Buffer has
					;  data from the disk in it.
Must$Write$Buffer: 	DB	0	;Non-zero when data has been written
					;  into Disk$Buffer but not yet
					;  written out to disk. 
;
Selected$Dk$Trk:		;Variables for Selected Disk, Track and Sector
				; (Selected by SELDSK, SETTRK and SETSEC).
Selected$Disk:		DB	0	;) These are moved and compared
Selected$Track:		DW	0	;) as a group so do not alter order.

Selected$Sector:	DB	0	;Not part of group but needed here.

Selected$Physical$Sector: DB	0	;Selected Physical Sector derived 
					;  from Selected (CP/M) sector by 
					;  shifting it right the number of 
					;  bits specified by Sector$Bit$Shift.

;

Disk$Error$Flag: 	DB	0	;Non-zero to indicate an error
					;  that could not be recovered
					;  by the disk drivers. The BDOS
					;  will output a 'Bad Sector' message.
Disk$Hung$Flag:		DB	0	;Non-zero if a Watchdog timeout
					;  occurs
Disk$Timer		EQU	600	;Number of 16.66ms clock ticks
					;  for a 10 second timeout
;
				;Flags used inside the deblocking code

Read$Operation: 	DB	0	;Non-zero when a CP/M 128-byte
					;  sector is to be read.
Selected$Disk$Deblock:	DB	0	;Non-zero when the selected disk
					;  needs deblocking (Set in SELDSK).
Selected$Disk$Type:	DB	0	;Indicates 8" or 5 1/4" Floppy or 
					;  M$Disk selected. (Set in SELDSK).
;
;#
;
;	Read in the 128-byte CP/M sector specified by previous calls
;	to Select Disk, Set Track and Sector. The sector will be read
;	into the address specified in the previous Set DMA Address call.
;
;	If reading from a disk drive using sectors larger than 128 bytes,
;	deblocking code will be used to 'unpack' a 128-byte sector from
;	the physical sector.
READ:
	LDA	Selected$Disk$Deblock	;Check if Deblocking needed 
	ORA	A			;(flag was set in SELDSK call)
	JZ	Read$No$Deblock		;No, use normal non-deblocked

				;The deblocking algorithm used is such
				;  that a Read operation can be viewed,
				;  up until the actual data transfer as though
				;  it was the first write to an unallocated 
				;  Allocation Block
	MVI	A,1			;Indicate that it is really a Read
	STA	Read$Operation		;  that is to be performed

	MVI	A,Write$Allocated	;Fake deblocking code into believing
	STA	Write$Type		;  that this is a write to an
					;  allocated Allocation Block.
	JMP	Perform$Read$Write	;Use common code to execute read
;
;#
;	Write a 128-byte sector from the current DMA Address out
;	the previously selected Disk, Track and Sector.
;
;	On arrival here, the BDOS will have set register C to indicate
;	whether this write operation is to an already Allocated Allocation
;	Block (which means a pre-read of the sector may be needed), or
;	to the Directory (in which case the data will be written to the
;	disk immediately).
;
;	Only writes to the directory take place immediately, in all other
;	cases, the data will be moved from the DMA address into the disk
;	buffer, and only be written out when circumstances force the
;	transfer. The number of physical disk operations can therefore
;	be reduced considerably.
;
WRITE:
	LDA	Selected$Disk$Deblock	;Check if deblocking is required
	ORA	A			;(flag set in SELDSK call)
	JZ	Write$No$Deblock

	XRA	A			;Indicate that a write operation
	STA	Read$Operation		;  is required (i.e NOT a read)
	MOV	A,C			;Save the BDOS Write Type
	ANI	1			;  but only distinguish between
					;  Write to allocated block or
	STA	Write$Type		;  directory write
;
;
;#
;
Perform$Read$Write:	;Common code to execute both reads and
			;  writes of 128-byte sectors.
	XRA	A		;Assume that no disk errors will
	STA	Disk$Error$Flag	;occur

	LDA	Selected$Sector	;Convert Selected 128-byte sector
	RAR			;  into physical sector by dividing by 4
	RAR
	ANI	3FH		;Remove any unwanted bits
	STA	Selected$Physical$Sector
					;
	LXI	H,Data$In$Disk$Buffer	;Check if disk buffer already has
	MOV	A,M			;  data in it.
	MVI	M,1			;(Unconditionally indicate that
					; the buffer now has data in it)
	ORA	A			;Did it indeed have data in it?
	JZ	Read$Track$into$Buffer	;No, proceed to read a physical
					;  track into the buffer.
				;
				;The buffer does have a physical track
				;  in it. Check if it is the right one.
				;
	LXI	D,In$Buffer$Dk$Trk	;Check if track in buffer is the
	LXI	H,Selected$Dk$Trk	;  same as that selected earlier.
	CALL	Compare$Dk$Trk		;Compare ONLY Disk and Track
	JZ	Track$In$Buffer		;Yes - it is already in buffer

					;No, it will have to be read in
					;  over current contents of buffer
	LDA	Must$Write$Buffer	;Check if buffer has data in that
	ORA	A			;  must be written out first.
	CNZ	Write$Physical		;Yes, write it out
;
Read$Track$into$Buffer:
	CALL	Set$In$Buffer$Dk$Trk	;Set in buffer variables from
					;  Selected Disk, Track
					;  to reflect which track is in the
					;  buffer now
	CALL	Read$Physical		;Read the track into the buffer
	XRA	A			;Reset the flag to reflect buffer
	STA	Must$Write$Buffer	;  contents.
;
Track$In$Buffer:		;Selected Track and
				;  Disk is already in the buffer.
				;Convert the Selected CP/M (128-byte)
				;  sector into a relative address down
				;  the buffer.
	LDA	Selected$Sector	;Get Selected Sector Number
	MOV	L,A		;Multiply by 128 by shifting 16-bit value
	MVI	H,0		;left 7 bits
	DAD	H		;* 2
	DAD	H		;* 4
	DAD	H		;* 8
	DAD	H		;* 16
	DAD	H		;* 32
	DAD	H		;* 64
	DAD	H		;* 128
;
	LXI	D,Disk$Buffer	;Get base address of Disk Buffer
	DAD	D		;Add on Sector number * 128
				;HL -> 128-byte sector number start
				;  address in disk buffer
	XCHG			;DE -> Sector in Disk Buffer
	LHLD	DMA$Address	;Get DMA address set in SETDMA call
	XCHG			;Assume a Read operation, so
				;  DE -> DMA Address
				;  HL -> Sector in Disk Buffer
	MVI	C,128/8		;Because of the faster method used
				;  to move data in and out of the
				;  disk buffer, (eight bytes moved per
				;  loop iteration) the count need only
				;  be 1/8th of normal.
				;At this point -
				;	C = Loop Count
				;	DE -> DMA Address
				;	HL -> Sector in Disk Buffer
	LDA	Read$Operation	;Determine whether data is to be moved
	ORA	A		;  out of the buffer (Read) or into the
	JNZ	Buffer$Move	;  buffer (Write)
				;Writing into buffer
					;(A must be 0 get here)
	INR	A			;Set flag to force a write
	STA	Must$Write$Buffer	;  of the disk buffer later on.
	XCHG				;Make DE -> Sector in Disk Buffer
					;     HL -> DMA Address
;
;
Buffer$Move:
	CALL	Move$8		;Moves 8 bytes * C times from (HL)
				;  to (DE)

				;
	LDA	Write$Type	;If Write to Directory, write out
	CPI	Write$Directory	;  buffer immediately
	LDA	Disk$Error$Flag	;Get error flag in case delayed write or read
	RNZ			;Return if delayed write or read
				;
	ORA	A		;Check if any disk errors have occured
	RNZ			;Yes, abandon attempt to write to directory
				;
	XRA	A		;Clear flag that indicates buffer must be
	STA	Must$Write$Buffer	;  written out.
	CALL	Write$Physical	;Write buffer out to physical track
	LDA	Disk$Error$Flag	;Return error flag to caller
	RET
;
;
;
Set$In$Buffer$Dk$Trk:			;Indicate Selected Disk, Track
					;  now residing in buffer
	LDA	Selected$Disk
	STA	In$Buffer$Disk

	LHLD	Selected$Track
	SHLD	In$Buffer$Track	

	LDA	Selected$Disk$Type	;Also reflect disk type 
	STA	In$Buffer$Disk$Type	

	RET
;
;
Compare$Dk$Trk:			;Compares just the Disk and Track
				;  pointed to by DE and HL
	MVI	C,3		;Disk (1), Track (2)
Compare$Dk$Trk$Loop:
	LDAX	D		;Get comparitor
	CMP	M		;Compare with comparand
	RNZ			;Abandon comparison if inequality found
	INX	D		;Update comparitor pointer
	INX	H		;Update comparand pointer
	DCR	C		;Count down on loop count
	RZ			;Return (with Zero flag set)
	JMP	Compare$Dk$Trk$Loop
;
;
Move$Dk$Trk:			;Moves the Disk, Track
				;  variables pointed at by HL to
				;  those pointed at by DE
	MVI	C,3		;Disk (1), Track (2)
Move$Dk$Trk$Loop:
	MOV	A,M		;Get source byte
	STAX	D		;Store in destination
	INX	D		;Update pointers
	INX	H
	DCR	C		;Count down on byte count
	RZ			;Return if all bytes moved
	JMP	Move$Dk$Trk$Loop
;
;#
;
;	Move 8 bytes
;
;	This routine moves 8 bytes at a block, C times, from
;	(HL) to (DE). It uses "drop through" coding to speed
;	up execution.
;
;	Entry Parameters
;
;		C = Number of 8 byte blocks to move
;		DE -> Destination Address
;		HL -> Source Address
;		
Move$8:	
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H
	MOV	A,M		;Get byte from source
	STAX	D		;Put into destination
	INX	D		;Update pointers
	INX	H

	DCR	C		;Count down on loop counter
	JNZ	Move$8		;Repeat until done
	RET

;
;#
;
;	Introduction to the Disk Controllers on this computer system.
;
;	There are two 'smart' disk controllers on this system, one
;	for the 8" Floppy Diskette drives, and one for the 5 1/4"
;	Mini-diskette drives.
;
;	The controllers are 'hard-wired' to monitor certain locations
;	in memory to detect when they are to perform some disk
;	operation. The 8" controller looks at location 0040H, and
;	the 5 1/4" controller looks at location 0045H. These are
;	called their Disk Control Bytes. If the most significant
;	bit of a Disk Control Byte is set, the controller will then
;	look at the word following the respective Control Bytes.
;	This word must contain the address of a valid Disk Control
;	Table that specifies the exact disk operation to be performed.
;
;	Once the operation has been completed, the controller resets
;	its Disk Control Byte to 00H, and it is this that indicates
;	completion to the disk driver code.
;
;	The controller also sets a return code in a Disk Status Blocke -
;	both controllers use the SAME location for this; 0043H.
;	If the first byte of this Status Block is less than 80H, then
;	a disk error has occurred. For this simple BIOS, no further details
;	of the Status settings are relevant. Note that the disk controller
;	has built-in re-try logic - reads and writes are attempted ten
;	times before the controller returns an error.
;
;	The Disk Control Table layout is shown below. Note that the
;	controllers have the capability for Control Tables to be
;	chained together so that a sequence of disk operations can
;	be initiated. In this BIOS this feature is not used. However,
;	the controller requires that the chain pointers in the
;	Disk Control Tables be pointed back to the main Control Bytes
;	in order to indicate the end of the chain.
;
Disk$Control$8		EQU	40H	;8" Control Byte
Command$Block$8		EQU	41H	;Control Table Pointer
;
Disk$Status$Block	EQU	43H	;8" AND 5 1/4" Status Block
;
Disk$Control$5		EQU	45H	;5 1/4" Control Byte
Command$Block$5		EQU	46H	;Control Table Pointer
;
;
;	Floppy Disk Control Tables
;
Floppy$Command:			DB	0	;Command
Floppy$Read$Code		EQU	01H
Floppy$Write$Code		EQU	02H
Floppy$Unit:			DB	0	;Unit (Drive) number = 0 or 1
Floppy$Head:			DB	0	;Head number = 0 or 1 
Floppy$Track:			DB	0	;Track number
Floppy$Sector:			DB	0	;Sector number
Floppy$Byte$Count:		DW	0	;Number of bytes to read/write
Floppy$DMA$Address:		DW	0	;Transfer Address
Floppy$Next$Status$Block:	DW	0	;Pointer to next Status Block
						; if commands are chained.
Floppy$Next$Control$Location:	DW	0	;Pointer to next Control Byte
						;  if commands are chained.
;
;#
;
;
Write$No$Deblock:		;Write contents of Disk Buffer to
				;  correct sector.
	MVI	A,Floppy$Write$Code	;Get Write Function code
	JMP	Common$No$Deblock	;Go to common code
Read$No$Deblock:		;Read previously selected Sector
				;  into Disk Buffer.
	MVI	A,Floppy$Read$Code	;Get Read Function code
Common$No$Deblock:
	STA	Floppy$Command	;Set Command Function code
				;Set up non-deblocked command table

	LDA	Selected$Disk$Type	;Check if Memory Disk operation
	CPI	M$Disk
	JZ	M$Disk$Transfer	;Yes it is M$Disk

No$Deblock$Retry:		;Re-entry point to retry after error
	LXI	H,128		;Bytes per sector
	SHLD	Floppy$Byte$Count
	XRA	A		;8" Floppy only has head 0
	STA	Floppy$Head
				;
	LDA	Selected$Disk	;8" Floppy controller only knows about
				;  units 0 and 1 so Selected$Disk must
				;  be converted
	ANI	01H		;Turn into 0 or 1
	STA	Floppy$Unit	;Set unit number
				;
	LDA	Selected$Track	
	STA	Floppy$Track	;Set track number
				;
	LDA	Selected$Sector
	STA	Floppy$Sector	;Set sector number
				;
	LHLD	DMA$Address	;Transfer directly between DMA Address
	SHLD	Floppy$DMA$Address	;and 8" controller.
				;
				;The disk controller can accept chained
				;  disk control tables, but in this case,
				;  they are not used, so the 'Next' pointers
				;  must be pointed back at the initial
				;  control bytes in the base page.
	LXI	H,Disk$Status$Block		;Point next status back at
	SHLD	Floppy$Next$Status$Block	;  main status block
						;
	LXI	H,Disk$Control$8		;Point next control byte
	SHLD	Floppy$Next$Control$Location	;  back at main control byte
					;
	LXI	H,Floppy$Command	;Point controller at control table
	SHLD	Command$Block$8
					;
	LXI	H,Disk$Control$8	;Activate controller to perform
	MVI	M,80H			;  operation.
	JMP	Wait$For$Disk$Complete

;
;#
;	Memory Disk Driver
;
;	This routine must use an intermediary buffer as the
;	DMA Address in Bank ("Track") 0, occupies the same
;	place in the overall address space as the M$Disk itself.
;	The M$Disk$Buffer is above the 48K mark, and therefore
;	remains in the address space regardless of which bank/track
;	is selected.
;
;
;	For writing, the 128-byte sector must be processed :
;
;		1. Move sector DMA$Address -> M$Disk$Buffer
;		2. Select correct track (+1 to get bank number)
;		3. Move sector M$Disk$Buffer -> M$Disk image
;		4. Select Bank 0
;
;	For reading, the processing is :
;
;		1. Select correct track/bank
;		2. Move sector M$Disk image -> M$Disk$Buffer
;		3. Select Bank 0
;		4. Move sector M$Disk$Buffer -> DMA$Address
;
;	If there is any risk of any interrupt causing control
;	to be transferred to an address below 48K, interrupts MUST
;	be disabled when any Bank other than 0 is selected.
;
M$Disk$Transfer:
	LDA	Selected$Sector	;Compute address in memory
	MOV	L,A		;  by muliplying sector * 128
	MVI	H,0
	DAD	H		;* 2
	DAD	H		;* 4
	DAD	H		;* 8
	DAD	H		;* 16
	DAD	H		;* 32
	DAD	H		;* 64
	DAD	H		;* 128

	LDA	Selected$Track	;Compute which half of bank sector
				;  is in by using LS bit of Track
	MOV	B,A		;Save copy for later
	ANI	1		;Isolate lower/upper indicator
	JZ	M$Disk$Lower$Half

	LXI	D,(48 * 1024) / 2	;Upper half - so bias address
	DAD	D

M$Disk$Lower$Half:		;HL -> Sector in memory
	MOV	A,B		;Recover Selected Track
	RAR			;Divide by 2 to get Bank number
	INR	A		;Bank 1 is first track
	MOV	B,A		;Preserve for later use

	LDA	Floppy$Command	;Check if reading or writing
	CPI	Floppy$Write$Code
	JZ	M$Disk$Write	;Writing
				;Reading

	CALL	Select$Bank	;Select correct memory bank
	LXI	D,M$Disk$Buffer	;DE -> M$Disk$Buffer, HL -> M$Disk image
	MVI	C,128/8		;Number of 8 byte blocks to move
	CALL	Move$8

	MVI	B,0		;Revert to normal memory bank
	CALL	Select$Bank

	LHLD	DMA$Address	;Get user's DMA Address
	LXI	D,M$Disk$Buffer
	XCHG			;DE -> User's DMA, HL -> M$Disk Buffer
	MVI	C,128/8		;Number of 8 byte blocks to move
	CALL	Move$8

	XRA	A		;Indicate no error
	RET

M$Disk$Write:			;Writing
	PUSH	H		;Save sector's address in M$Disk image
	LHLD	DMA$Address	;Move sector into M$Disk$Buffer
	LXI	D,M$Disk$Buffer
	MVI	C,128/8		;Number of 8 byte blocks to move
	CALL	Move$8		;(Does not use B register)
				;B = Memory Bank to select
	CALL	Select$Bank

	POP	D		;Recover sector's M$Disk image address
	LXI	H,M$Disk$Buffer
	MVI	C,128/8
	CALL	Move$8		;Move into M$Disk image

	MVI	B,0		;Select bank 0
	CALL	Select$Bank

	XRA	A		;Indicate no error
	RET
;
;#
;	Select Bank
;
;	This routine switches in the required memory bank.
;	Note that the hardware port that controls bank selection
;	also has other bits in it - these are preserved across
;	bank selections.
;
;	Entry Parameter
;
;		B = Bank Number
;
Bank$Control$Port	EQU	40H
Bank$Mask		EQU	1111$1000B	;To PRESERVE other bits
;
Select$Bank:
	IN	Bank$Control$Port	;Get current setting in port
	ANI	Bank$Mask		;Preserve all other bits
	ORA	B			;Set bank code
	OUT	Bank$Control$Port	;Select the Bank
	RET
;
;#
;

Write$Physical:			;Write contents of Disk Buffer to
				;  correct sector.
	MVI	A,Floppy$Write$Code	;Get Write Function code
	JMP	Common$Physical	;Go to common code
Read$Physical:			;Read previously selected Sector
				;  into Disk Buffer.
	MVI	A,Floppy$Read$Code	;Get Read Function code
;
Common$Physical:
	STA	Floppy$Command	;Set command table

;
Deblock$Retry:			;Re-entry point to re-try after error
	LDA	In$Buffer$Disk$Type	;Get disk type currently in buffer
	CPI	Floppy$5		;Confirm it is a 5 1/4" Floppy
	JZ	Correct$Disk$Type	;Yes
	MVI	A,1			;No, indicate disk error
	STA	Disk$Error$Flag
	RET
Correct$Disk$Type:		;Setup disk control table
				;
	LDA	In$Buffer$Disk	;Convert disk number to 0 or 1
	ANI	1		;  for disk controller
	STA	Floppy$Unit

	LHLD	In$Buffer$Track	;Setup head and track number
	MOV	A,L		;Even numbered tracks will be on
	ANI	1		;  head 0, odd numbered on head 1
	STA	Floppy$Head	;Set head number

	MOV	A,L		;Note : this is single byte value
	RAR			;/2 for track (carry off from ANI above)
	STA	Floppy$Track

	MVI	A,1			;Start with sector 1 as a whole
	STA	Floppy$Sector		;  track will be transferred
					;
	LXI	H,Bytes$Per$Track	;Set byte count for complete
	SHLD	Floppy$Byte$Count	;  track to be transferred
					;
	LXI	H,Disk$Buffer		;Set transfer address to be
	SHLD	Floppy$DMA$Address	;  Disk Buffer.
					;
					;As only one control table is in
					;  use, close the Status and Busy
					;  chain pointers back to the
					;  main control bytes.
	LXI	H,Disk$Status$Block
	SHLD	Floppy$Next$Status$Block
	LXI	H,Disk$Control$5
	SHLD	Floppy$Next$Control$Location

	LXI	H,Floppy$Command	;Setup command block pointer
	SHLD	Command$Block$5

	LXI	H,Disk$Control$5	;Activate 5 1/4" disk controller
	MVI	M,80H
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
	LXI	H,Disk$Control$5	;Check complete (fix: AJL 082983)
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
;#
;	Disk Error Message handling
;
;
Disk$Error$Messages:		;This table is scanned, comparing the
				;  Disk Error Status with those in the
				;  table. Given a match, or even when
				;  then end of the table is reached, the
				;  address following the status value
				;  points to the correct message text.
	DB	40H
	DW	Disk$Msg$40
	DB	41H
	DW	Disk$Msg$41
	DB	42H
	DW	Disk$Msg$42
	DB	21H
	DW	Disk$Msg$21
	DB	22H
	DW	Disk$Msg$22
	DB	23H
	DW	Disk$Msg$23
	DB	24H
	DW	Disk$Msg$24
	DB	25H
	DW	Disk$Msg$25
	DB	11H
	DW	Disk$Msg$11
	DB	12H
	DW	Disk$Msg$12
	DB	13H
	DW	Disk$Msg$13
	DB	14H
	DW	Disk$Msg$14
	DB	15H
	DW	Disk$Msg$15
	DB	16H
	DW	Disk$Msg$16
	DB	0			;<== Terminator
	DW	Disk$Msg$Unknown	;Unmatched code
;
DEM$Entry$Size	EQU	3	;Disk Error Message Table entry size
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
	LXI	D,DEM$Entry$Size	;Get entry size for loop below
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
	INX	H			;HL -> Address of text
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
;
Disk$Error$Request$Action:		;Ask the user what to do next
	CALL	Request$User$Choice	;Display prompt and wait for input
					;  Returns with A = Upper case char.
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
	LDA	Selected$Disk$Deblock
	ORA	A
	JNZ	Deblock$Retry
	JMP	No$Deblock$Retry
;
System$Reset:				;This is a radical approach, but
					;  it does cause CP/M to restart
	MVI	C,0			;System Reset
	CALL	BDOS

;
;
;	A to Upper
;
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
A$To$Upper:
	CPI	'a'		;Compare to lower limit
	RC			;No need to convert
	CPI	'z' + 1		;Compare to upper limit
	RNC			;No need to convert
	ANI	5FH		;Convert to upper case
	RET
;
;	Convert A register to Hexadecimal
;
;	This subroutine converts the A register to Hexadecimal.
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
CAH:
	PUSH	PSW		;Take a copy of the value to be converted
	RRC			;Shift A right four places
	RRC
	RRC
	RRC
	CALL	CAH$Convert	;Convert to ASCII
	POP	PSW		;Get original value again
				;Drop into subroutine, which converts
				;  and returns to caller
CAH$Convert:
	ANI	0000$1111B	;Isolate LS four bits
	ADI	'0'		;Convert to ASCII
	CPI	'9' + 1		;Compare to maximum
	JC	CAH$Numeric	;No need to convert to A -> F
	ADI	7		;Convert to a letter
CAH$Numeric:
	MOV	M,A		;Save character away
	INX	H		;Update character pointer
	RET

;
;
;#
;
;	Disk Control Table images for Warm Boot
;
Boot$Control$Part$1:
	DB	1			;Read Function
	DB	0			;Unit (Drive) number
	DB	0			;Head number
	DB	0			;Track number
	DB	2			;Starting sector number
	DW	8*512			;Number of bytes to read
	DW	CCP$Entry		;Read into this address
	DW	Disk$Status$Block	;Pointer to next Status Block
	DW	Disk$Control$5		;Pointer to next Control Table
Boot$Control$Part2:
	DB	1			;Read Function
	DB	0			;Unit (Drive) number
	DB	1			;Head number
	DB	0			;Track Number
	DB	1			;Starting Sector number
	DW	3*512			;Number of bytes to read
	DW	CCP$Entry + (8*512)	;Read into this address
	DW	Disk$Status$Block	;Pointer to next Status Block
	DW	Disk$Control$5		;Pointer to next Control Table

;
;
;#
;
WBOOT:		;Warm Boot Entry
			;On warm boot, the CCP and BDOS must be reloaded
			;  into memory. In this BIOS, only the 5 1/4"
			;  diskettes will be used, therefore this code
			;  is hardware specific to the controller. Two
			;  prefabricated control tables are used.
	LXI	SP,80H
	LXI	D,Boot$Control$Part1	;Execute first read of warm boot
	CALL	Warm$Boot$Read		;Load Drive 0, Track 0, 
					;  Head 0, Sectors 2 to 8
	LXI	D,Boot$Control$Part2	;Execute second read
	CALL	Warm$Boot$Read		;Load Drive 0, Track 0,
					;  Head 1, Sectors 1 - 3
	CALL	Patch$CPM		;Make custom enhancements patches
	JMP	Enter$CPM		;Setup base page and enter CCP
;
Warm$Boot$Read:			;On entry, DE -> Control Table image
				;This control table is moved into
				;  the main disk control table and
				;  then the controller activated.
	LXI	H,Floppy$Command	;HL -> actual control table
	SHLD	Command$Block$5		;Tell the controller its address
					;Move the control table image
					;  into the control table itself.
	MVI	C,13		;Set byte count
Warm$Boot$Move:
	LDAX	D		;Get image byte
	MOV	M,A		;Store into actual control table
	INX	H		;Update pointers
	INX	D
	DCR	C		;Count down on byte count
	JNZ	Warm$Boot$Move	;Continue until all bytes moved

	LXI	H,Disk$Control$5	;Activate controller
	MVI	M,80H
Wait$For$Boot$Complete:
	MOV	A,M			;Get status byte		
	ORA	A			;Check if complete
	JNZ	Wait$For$Boot$Complete	;No
					;Yes, check for errors
	LDA	Disk$Status$Block
	CPI	80H		
	JC 	Warm$Boot$Error		;Yes, an error occurred
	RET
;
Warm$Boot$Error:
	LXI	H,Warm$Boot$Error$Message
	CALL	Display$Message
	JMP	WBOOT			;Restart warm boot
;
Warm$Boot$Error$Message:
	DB	CR,LF,'Warm Boot Error - retrying...',CR,LF,0
;
;
;#
;
Ghost$Interrupt:	;Control will only arrive here under the most
			;  unusual circumstances as the Interrupt
			;  Controller will have been programmed to
			;  suppress unused interrupts.
			;
	PUSH	PSW			;Save pre-interrupt registers
	MVI	A,IC$EOI		;Indicate End of Interrupt
	OUT	IC$OCW2$Port
	POP	PSW
	RET
;
;
;#
;
;	Patch CP/M
;
;	This routine makes some very special patches to the
;	CCP and BDOS in order to make some custom enhancements
;
;	Public Files:
;		On large hard disk systems it is extremely useful
;		to partition the disk using the USER number features.
;		However it becomes very wasteful of disk space because
;		multiple copies of common programs must be stored in
;		each user area. This patch makes User 0 PUBLIC - it
;		can be accessed from any other user area.
;		*** WARNING ***
;		Files in User 0 MUST be set to System and Read/Only
;		status to avoid them being accidentally damaged.
;		Because of the side-effects associated with Public
;		Files, the patch can be turned on or off using
;		a flag in the Long Term Configuration Block.
;
;	User Prompt :
;		When using CP/M's USER command and user numbers
;		in general, it is all too easy to become confused
;		and forget which user number you are "in". This
;		patch modifies the CCP to display a prompt which
;		shows not only the default disk id., but also the
;		current user number, and an indication of whether
;		Public Files are enabled:
;
;				P3B>  or  3B>
;				^
;				When Public Files are enabled.
;
;	Equates for Public Files
;
PF$BDOS$Exit$Point	EQU	BDOS$Entry + 758H
PF$BDOS$Char$Matches	EQU	BDOS$Entry + 776H
PF$BDOS$Resume$Point	EQU	BDOS$Entry + 75BH
PF$BDOS$Unused$Bytes	EQU	13
;
;
;	Equates for User Prompt
;
UP$CCP$Exit$Point	EQU	CCP$Entry + 388H
UP$CCP$Resume$Point	EQU	CCP$Entry + 38BH
UP$CCP$Get$User		EQU	CCP$Entry + 113H
UP$CCP$Get$Disk$Id	EQU	CCP$Entry + 1D0H
UP$CCP$CONOUT		EQU	CCP$Entry + 8CH
;
;
;	Setup the intervention points
;
Patch$CPM:
	MVI	A,JMP		;Set up opcode
	STA	PF$BDOS$Exit$Point
	STA	UP$CCP$Exit$Point
	LXI	H,Public$Patch
	SHLD	PF$BDOS$Exit$Point + 1
	LXI	H,Prompt$Patch	;Get address of intervening code
	SHLD	UP$CCP$Exit$Point + 1

	RET			;Return to enter CP/M
;
;
;
Public$Patch:			;Control arrives here from the BDOS
				;The BDOS is in the process of scanning
				;  down the target file name in the
				;  Search Next function
				;  HL -> The name of the file searched for
				;  DE -> Directory entry
				;  B = Character count

	LDA	CB$Public$Files	;Check if Public Files are to be enabled
	ORA	A
	JZ	No$Public$Files	;No

	MOV	A,B		;Get character count
	ORA	A		;Check if looking at first byte
				;  (that contains the user number)
	JNZ	No$Public$Files ;No, ignore this patch

	LDAX	D		;Get user number from directory entry
	CPI	0E5H		;Check if active directory entry
	JZ	No$Public$Files	;Yes, ignore this patch

	MOV	A,M		;Get user number
	ORA	A		;Check if User 0
	JZ	PF$BDOS$Char$Matches	;Force character match

No$Public$Files:		;Replaced patched out code
	MOV	A,B			;Check if count indicates that
	CPI	PF$BDOS$Unused$Bytes	;  registers are pointing at
					;  unused bytes field of FCB
	JMP	PF$BDOS$Resume$Point	;Return to BDOS
;
Prompt$Patch:			;Control arrives here from the CCP
				;The CCP is just about to get the
				;  drive id. when control gets here.
				;The CCP's version of CONOUT is used
				;  so that the CCP can keep track of
				;  the cursor position.

	LDA	CB$Public$Files	;Check if Public Files are enabled
	ORA	A
	JZ	UP$Private$Files	;No

	MVI	A,'P'
	CALL	UP$CCP$CONOUT	;Use CCP's CONOUT routine

UP$Private$Files:
	CALL	UP$CCP$Get$User	;Get current user number
	CPI	9 + 1		;Check if one or two digits
	JNC	UP$2$Digits
	ADI	'0'		;Convert to ASCII
UP$1$Digit:
	CALL	UP$CCP$CONOUT	;Output the character
	CALL	UP$CCP$Get$Disk$Id	;Get disk identifier
	JMP	UP$CCP$Resume$Point	;Return to CCP
;
UP$2$Digits:
	ADI	'0' - 10	;Subtract 10 and convert to ASCII
	PUSH	PSW		;Save converted second digit
	MVI	A,'1'		;Output leading '1'
	CALL	UP$CCP$CONOUT
	POP	PSW		;Recover second digit
	JMP	UP$1$Digit	;Output remainder of prompt and return to
				;  the CCP

;
;#
;
;	Configuration Block Get Address
;
;	This routine is called by Utility Programs running in the TPA.
;	Given a specific code number it returns the address of a specific
;	object in the Configuration Block.
;
;	By using this routine, Utility Programs need not know the exact
;	layout of the Configuration Block.
;
;	Entry Parameters
;
;		C = Object Identity Code (in effect, this is the
;			subscript of the object's address in the
;			table below)
;
;==========================
CB$Get$Address:				;<=== BIOS Entry Point (Private)
;==========================
	PUSH	PSW			;Save User's registers
	PUSH	B
	PUSH	D

	MOV	L,C			;Make Code into a word
	MVI	H,0
	DAD	H			;Convert Code into word offset
	LXI	D,CB$Object$Table	;Get base address of table
	DAD	D			;HL -> Object's address in table
	MOV	E,M			;Get LS byte
	INX	H
	MOV	D,M			;Get MS byte
	XCHG				;HL = Address of object

	POP	D			;Recover User's registers
	POP	B
	POP	PSW

	RET
;
;#
;
CB$Object$Table:
				;	Code
				;        vv
	DW	Date			;01 Date in ASCII
	DW	Time$In$ASCII		;02 Time in ASCII
	DW	Time$Date$Flags		;03 Flags indicated if Time/Date set
	DW	CB$Forced$Input		;04 Forced input pointer
	DW	CB$Startup		;05 System Startup message
					;   Redirection words
	DW	CB$Console$Input	;06
	DW	CB$Console$Output	;07
	DW	CB$Auxiliary$Input	;08
	DW	CB$Auxiliary$Output	;09
	DW	CB$List$Input		;10
	DW	CB$List$Output		;11
	
	DW	CB$Device$Table$Addresses ;12
	DW	CB$12$24$Clock		;13 Selects 12/24 hr format Clock
	DW	RTC$Ticks$per$Second	;14
	DW	RTC$Watchdog$Count	;15
	DW	RTC$Watchdog$Address	;16
	DW	CB$Function$Key$Table	;17
	DW	CONOUT$Escape$Table	;18

	DW	D0$Initialize$Stream	;19
	DW	D0$Baud$Rate$Constant	;20
	DW	D1$Initialize$Stream	;21
	DW	D1$Baud$Rate$Constant	;22
	DW	D2$Initialize$Stream	;23
	DW	D2$Baud$Rate$Constant	;24
	DW	Interrupt$Vector	;25
	DW	LTCB$Offset		;26
	DW	LTCB$Length		;27
	DW	CB$Public$Files		;30
	DW	Multi$Command$Buffer	;31
;
;#
;	The Short Term Configuration Block.
;
;	It contains variables that can be set once CP/M
;	has been initiated, but that are never preserved
;	from one loading of CP/M to the next. This part of
;	the Configuration Block are the last initialized bytes
;	in the BIOS.
;
;	The two values below are used by utility programs that
;	need to read in the Long Term Configuration Block from disk.
;	The BIOS starts on a 256-byte page boundary - and therefore
;	will always be on a 128-byte sector boundary in the reserved
;	area on the disk. A utility program can then, using the
;	CB$Get$Address Private BIOS call, determine how many 128-byte
;	sectors need to be read in by the formula :
;
;		(LCTB$Offset + LTCB$Length) / 128
;
;	The LTCB$Offset is the offset from the start of the BIOS
;	where the first byte of the Long Term Configuration Block
;	starts. Using the Offset and the Length, the utility can
;	copy the RAM version of the LTCB down over the disk image
;	that it has read in from the disk, and then write the
;	updated LTCB back onto the disk.
;
LTCB$Offset:	DW	Long$Term$CB - BIOS$Entry
LTCB$Length:	DW	Long$Term$CB$End - Long$Term$CB
;
;	Forced Input Pointer
;
;	If CONIN ever finds that this pointer is pointing at a non-zero
;	byte, then this byte will be injected into the console input
;	stream as though it had been typed on the console. The
;	pointer is then updated to the next byte in memory.

CB$Forced$Input:	DW	CB$Startup
;
;
Date:			;Current System Date
	DB	'10/17/82',LF	;Unless otherwise set to the contrary
				;  this is the Release Date of the system.
				;Normally, it will be set by the DATE utility.
	DB	0		;00-byte terminator

Time$in$ASCII:		;Current System Time
HH:	DB	'00'		;Hours
	DB	':'
MM:	DB	'00'		;Minutes
	DB	':'
SS:	DB	'00'		;Seconds
Time$in$ASCII$End:		;Used when updating the time
	DB	LF
	DB	0		;00-byte terminator
;
;
Time$Date$Flags:	;This byte contains two flags that are  used 
			;  to indicate whether the Time and/or Date 
			;  have been set either programmatically or 
			;  by using the TIME and DATE utilities. These 
			;  flags can be tested by utility programs that
			;  need to have the correct time and date set.
	DB	0
Time$Set	EQU	0000$0001B
Date$Set	EQU	0000$0010B

;
;#
;	Uninitialized Buffer Areas
;
;	With the exception of the main Disk$Buffer, which does contain
;	a few bytes of code, all of the other uninitialized variables
;	occur here. This has the effect of reducing the number of
;	bytes that need be stored in the CP/M image out on the disk,
;	as uninitialized areas do not need to be kept on the disk.
;
;
;#
;
;	The Cold Boot initialization code is only needed once.
;	It can be overwritten once it has been executed.
;	Therefore, it is 'hidden' inside the main disk buffer.
;
;
Disk$buffer:	DS	Physical$Sector$Size * Physical$Sec$Per$Track
;
					;Save the location counter
After$Disk$Buffer	EQU	$	;$ = Current value of location counter
;
			ORG	Disk$Buffer	;Wind the location counter back
;
Initialize$Stream:	;This stream of data is used by the
			;Initialize subroutine. It has the following
			;format :
			;
			;	DB	Port Number to be initialized
			;	DB	Number of byte to be output
			;	DB	xx,xx,xx,xx Data to be output
			;	:
			;	:
			;	DB	Port number of 00H terminates
			;
			;
;
;	Initialization Stream declared here
	DB	IC$ICW1$Port	;Program the 8259 interrupt controller
	DB	1
	DB	IC$ICW1

	DB	IC$ICW2$Port
	DB	1
	DB	IC$ICW2

	DB	IC$OCW1$Port
	DB	1
	DB	IC$OCW1
	
	DB	83H			;Program the 8253 clock generator
	DB	1
	DB	00$11$010$0B		;Counter 0, periodic interrupt, mode 2

	DB	80H			;RTC uses channel 0
	DB	2
	DW	17921			;19721 * 930 Nanoseconds =
					;  16.666 Milliseconds). 60 ticks/sec.
	DB	0		;Port number of 0 terminates
;
;
Signon$Message:	
	DB	'CP/M 2.2.'
	DW	VERSION		;Current version number
	DB	' '
	DW	MONTH		;Current Date
	DB	'/'
	DW	DAY
	DB	'/'
	DW	YEAR
	DB	CR,LF,LF
	DB	'Enhanced BIOS',CR,LF,LF
	DB	'Disk Configuration :',CR,LF,LF
	DB	'     A: 0.35 Mbyte 5" Floppy',CR,LF
	DB	'     B: 0.35 Mbyte 5" Floppy',CR,LF,LF
	DB	'     C: 0.24 Mbyte 8" Floppy',CR,LF
	DB	'     D: 0.24 Mbyte 8" Floppy',CR,LF
	DB	'     M: 0.19 Mbyte Memory Disk',CR,LF,LF
;
	DB	0
;
;	Messages for M$Disk
;
M$Disk$Setup$Message:
	DB	'     M$Disk already contains valid information.',CR,LF,0
M$Disk$Not$Setup$Message:
	DB	'     M$Disk has been initialized to empty state.',CR,LF,0
;
M$Disk$Dir$Entry:		;Dummy Directory entry used to determine
				;  if the M$Disk contains valid information
	DB	15		;User 15
	DB	'M$Disk  '
	DB	' '+80H,' '+80H,' '	;System and Read/Only
	DB	0,0,0,0
	DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
;
Default$Disk	EQU	0004H	;Default disk in base page
;
BOOT:		;Entered directly from the BIOS JMP Vector.
		;Control will be transferred here by the CP/M
		;Bootstrap Loader.
		;
				;Initialize system.
				;This routine uses the Initialize$Stream
				;declared above.

	DI			;Disable Interrupts to prevent any
				;side-effects during initialization.
	LXI	H,Initialize$Stream	;HL -> Data stream
	CALL	Output$Byte$Stream	;Output it to the specified
					;  ports

	CALL	General$CIO$Initialization ;Initialize character devices

	LXI	H,Signon$Message	;Display Signon message on console
	CALL	Display$Message
;
	CALL	Patch$CPM	;Make necessary Patches to CCP and BDOS
				;  for custom enhancements

				;Initialize M$Disk.
				;If the M$Disk directory has the
				;  special Reserved File Name "M$disk"
				;  (with lower case letters and marked
				;  SYS and R/O), then the M$Disk is
				;  assumed to contain valid data.
				;If the "M$Disk" file is absent, the
				;  M$Disk Directory entry is moved into
				;  the M$Disk image, and the remainder of
				;  the directory set to 0E5H.
	MVI	B,1		;Select bank 1
	CALL	Select$Bank	;  which contains the M$Disk directory

				;Check if M$Disk directory entry present
	LXI	H,0		;Start address for first directory
	LXI	D,M$Disk$Dir$Entry
	MVI	C,32		;Length to compare
M$Disk$Test:
	LDAX	D		;Get byte from initialized variable
	CMP	M		;Compare with M$Disk image
	JNZ	M$Disk$Not$Setup	;Match fails
	INX	D
	INX	H
	DCR	C
	JZ	M$Disk$Setup	;All bytes match
	JMP	M$Disk$Test
;
M$Disk$Setup:
	LXI	H,M$Disk$Setup$Message	;Inform user
;
M$Disk$Setup$Done:
	CALL	Display$Message

	XRA	A		;Set default disk drive to A:
	STA	Default$Disk
	EI			;Interrupts can now be enabled

	JMP	Enter$CPM	;Go into CP/M
;
M$Disk$Not$Setup:
	LXI	D,0		;Move M$Disk directory entry into
	LXI	H,M$Disk$Dir$Entry	;  M$Disk image
	MVI	C,32/8			;Number of 8 byte blocks to move
	CALL	Move$8
;
				;DE -> next byte after M$Disk directory
				;  entry in image
	MVI	A,0E5H			;Setup to do memory fill
	STAX	D			;Store first byte in "source" area
	MOV	H,D			;Set HL to DE +1
	MOV	L,E
	INX	H
	MVI	C,((2 * 1024) - 32) / 8	;2 Allocation Blocks
					;less 32 bytes for M$Disk entry
	CALL	Move$8			;Use Move$8 to do fill operation

	LXI	H,M$Disk$Not$Setup$Message
	JMP	M$Disk$Setup$Done	;Output message and enter CP/M
;
;
	DB	0		;Dummy
Last$Initialized$Byte:		;<== Address of last initialized byte
;
;	End of Cold Boot Initialisation Code
;
	ORG	After$Disk$Buffer	;Reset location counter
;
Multi$Command$Buffer:	DS	128	;This can be used to insert long
					;  command sequences into the
					;  console input stream by setting
					;  the Forced Input pointer here
;
D0$Buffer$Length	EQU	32	;Must be binary number
D0$Buffer:	DS	D0$Buffer$Length
;
D1$Buffer$Length	EQU	32	;Must be binary number
D1$Buffer:	DS	D1$Buffer$Length
;
D2$Buffer$Length	EQU	32	;Must be binary number
D2$Buffer:	DS	D2$Buffer$Length
;
;	Data Areas for the Character Drivers
;
PI$User$Stack:	DS	2	;Storage area for User's Stack Pointer
				;  when an interrupt occurs
PI$User$HL:	DS	2	;Save area for User's HL
		DS	40	;Stack area for use by Interrupt Service
PI$Stack:			;  routines to avoid overflowing the
				;  User's stack area
;
Directory$Buffer:	DS	128	;Disk Directory Buffer
;
M$Disk$Buffer:		DS	128	;Intermediary buffer for
					;  M$Disk
;
;	Disk Work Areas
;
;	These are used by the BDOS to detect any unexpected
;	change of diskettes. The BDOS will automatically set 
;	such a changed diskette to Read-Only status.
;
Disk$A$Workarea:	DS	32	; A:
Disk$B$Workarea:	DS	32	; B:
Disk$C$Workarea:	DS	16	; C:
Disk$D$Workarea:	DS	16	; D:
;
;
;	Disk Allocation Vectors
;
;	These are used by the BDOS to maintain a bit map of
;	which Allocation Blocks are used and which are free.
;	One byte is used for eight Allocation Blocks, hence the
;	expression of the form (Allocation Blocks/8)+1.
;
Disk$A$Allocation$Vector	DS	(174/8)+1	; A:
Disk$B$Allocation$Vector	DS	(174/8)+1	; B:
;
Disk$C$Allocation$Vector	DS	(242/8)+1	; C:
Disk$D$Allocation$Vector	DS	(242/8)+1	; D:
;
M$Disk$Allocation$Vector	DS	(192/8)+1	; M$Disk

	END	;Of Enhanced BIOS Listing

