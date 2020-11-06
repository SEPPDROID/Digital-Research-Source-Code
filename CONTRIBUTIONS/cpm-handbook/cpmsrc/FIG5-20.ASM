; Figure 5-20
;
;	CF
;	Create File
;	This subroutine creates a file. It erases any previous
;	File before creating the new one.
;
;	Entry Parameters
;
;		DE -> File Control Block for new file
;
;	Exit Parameters
;
;		Carry Clear if operation successful (A = 0,1,2,3)
;		Carry Set if error (A = 0FFH)
;
;	Calling Sequence
;
;		LXI	D,FCB
;		CALL	CF
;		JC	ERROR
;
B$ERASE		EQU	19	;Erase File
B$CREATE	EQU	22	;Create File
BDOS		EQU	5	;BDOS Entry Point
;
;
CF:
	PUSH	D		;Preserve FCB Pointer
	MVI	C,B$ERASE	;Erase any existing file
	CALL	BDOS
	POP	D		;Recover FCB Pointer
	MVI	C,B$CREATE	;Create (and open new file)
	CALL	BDOS
	CPI	0FFH		;Carry set if OK, Clear if Error
	CMC			;Complete to use Carry set if Error
	RET
