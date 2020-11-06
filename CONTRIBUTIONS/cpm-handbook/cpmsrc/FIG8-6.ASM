;	Figure 8-6
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
