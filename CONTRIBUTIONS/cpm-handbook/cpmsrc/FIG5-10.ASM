; Figure 5-10
;
;	IOBYTE Equates
;	These are for accessing the IOBYTE.
;
;	Mask Values to isolate specific devices.
;	(These can also be inverted to preserve all BUT the
;	 specific device)
;
IO$CONM	EQU	0000$0011B	;Console Mask
IO$RDRM	EQU	0000$1100B	;Reader Mask
IO$PUNM	EQU	0011$0000B	;Punch Mask
IO$LSTM	EQU	1100$0000B	;List Mask
;
				;Console Values
IO$CTTY	EQU	0		;Console -> TTY:
IO$CCRT	EQU	1		;Console -> CRT:
IO$CBAT	EQU	2		;Console Input <- RDR:
				;Console Output -> LST:
IO$CUC1	EQU	3		;Console -> UC1: (User console 1)
;
				;Reader Values
IO$RTTY	EQU	0 SHL 2		;Reader <- TTY:
IO$RRDR	EQU	1 SHL 2		;Reader <- RDR:
IO$RUR1	EQU	2 SHL 2		;Reader <- UR1: (User Reader 1)
IO$RUR2 EQU	3 SHL 2		;Reader <- UR2: (user Reader 2)
;
				;Punch Values
IO$PTTY	EQU	0 SHL 4		;Punch -> TTY:
IO$PPUN	EQU	1 SHL 4		;Punch -> PUN:
IO$PUP1	EQU	2 SHL 4		;Punch -> UP1: (User Punch 1)
IO$PUP2	EQU	3 SHL 4		;Punch -> UP2: (User Punch 2)
;
				;List Values
IO$LTTY	EQU	0 SHL 6		;List -> TTY:
IO$LCRT	EQU	1 SHL 6		;List -> CRT:
IO$LLPT	EQU	2 SHL 6		;List -> LPT: (Physical Line Printer)
IO$LUL1	EQU	3 SHL 6		;List -> UL1: (User List 1)
;
