;	CP/M DEBUGGER DISASSEMBLER/ASSEMBLER MODULE
	TITLE	'SYMBOLIC INTERACTIVE DEBUGGER (ASMOD) 12/16/77'
;
;	COPYRIGHT (C) 1976, 1977
;	DIGITAL RESEARCH
;	BOX 579 PACIFIC GROVE, CA
;	93950
;
FALSE	EQU	0		;VALUE OF "FALSE"
TRUE	EQU	NOT FALSE	;VALUE OF "TRUE"
ISIS2	EQU	FALSE		;TRUE IF ASSEMBLING FOR "IS"
DEBUG	EQU	FALSE		;TRUE IF CHECK-OUT TIME
RELOC	EQU	FALSE		;TRUE IF GENERATING RELOC IMAGE
RMAC	EQU	TRUE		;[JCE] True if using RMAC to link & relocate
;
; [JCE] Use of RMAC makes relocation easier
;
	IF	DEBUG
	ORG	1000H		;IN LOW MEMORY FOR DEBUG
	ELSE
	IF	ISIS2
	ORG	0E500H
	ELSE
	IF	RELOC
	ORG	0000H		;READY FOR RELOCATION
	ELSE
	IF	RMAC		;[JCE] No ORGs.
	cseg			;[JCE]
	ELSE			;[JCE]
	ORG	0D000H		;DEBUG IN 64K
	ENDIF
	ENDIF
	ENDIF
	ENDIF
;
JLOC1	EQU	0005H		;BDOS JUMP LOCATION
;
;
;	ENTRY POINTS FOR DEBUGGING MONITOR
DEMON	EQU	$+680H
BEGIN	EQU	DEMON+03H	;BEGINNING OF DEBUGGING MONITOR
GETBUFF	EQU	DEMON+9H	;READ BUFFER
GNC	EQU	DEMON+0CH
PCHAR	EQU	DEMON+0FH	;PRINT CHARACTER IN REG A
PBYTE	EQU	DEMON+12H	;PRINT BYTE
PADDX	EQU	DEMON+15H	;PRINT ADDRESS IN REG D,E
SCANEXP	EQU	DEMON+18H	;SCAN 0,1, OR 2 EXPRESSIONS
BREAK	EQU	DEMON+1EH	;CHECK FOR BREAK AT CONSOLE
PRLABEL	EQU	DEMON+21H	;PRINT SYMBOLIC LABEL
;
;
CI	EQU	GNC		;SYNONYM FOR GNC
;
CR	EQU	0DH
LF	EQU	0AH
TAB	EQU	09H
;
	IF	RMAC
	PUBLIC	MODBAS, DISIN, DISEN, ASSEM, DISPC, DISPM, DISPG
	ENDIF

MODBAS:	;MODULE LOCATION
	JMP	BEGIN	;ADDRESS FIELD IS ALTERED AT "BEGIN"
DISIN:	DB	0,0,0	;FILLER (USED IN SYMBOL TABLE)
DISEN:	JMP	DISENT
ASSEM:	JMP	ASMEN	;ENTRY POINT FOR ASSEMBLER
DISPC:
PC:	DS	2	;CURRENT FAKED PC DURING DISASSEMBLY
DISPM:
MPC:	DS	2	;MAX VALUE FOR PC (STOP ADDRESS)
DISPG:
PAGM:	DS	1	;PAGE MODE IF NON ZERO
TPC:	DS	2	;TEMPORARY PC FOR ASSEMBLER RESTORE ON ERROR
OLDSP:	DS	2	;ENTRY SP VALUE
;
;
CO:	;PRINT CHARACTER IN REGISTER C
	PUSH	PSW
	MOV	A,C		;PCHAR EXPECTS VALUE IN C
	CALL	PCHAR		;TO PRINT THE CHARACTER
	POP	PSW
	RET
;
;
DELIM:	;CHECK FOR DELIMITER
	CPI	' '
	RZ
	CPI	TAB
	RZ
	CPI	','
	RZ
	CPI	CR
	RZ
	CPI	7FH
	JZ	ASMEN	;RESTART CURRENT LINE
	RET
;
CRLF:	;RETURN AND LINE FEED
	MVI	C,CR
	CALL	CO
	MVI	C,LF
	CALL	CO
	RET
;
SCAN:	;FILL OPCODE WITH CHARACTERS
;
SC1:	CALL	CI
SCAN0:	;ENTER HERE IF CHARACTER SCANNED
	CPI	CR
	JZ	ERR
	CALL	DELIM
	JZ	SC1
;
;	CLEAR BUFFER
	MVI	C,4
	LXI	H,OPCODE
SC0:	MVI	M,' '
	INX	H
	DCR	C
	JNZ	SC0
;
;	GARBAGE REMOVED AT BEGINNING OF SCAN
	MVI	C,5
	LXI	H,OPCODE
SC2:	MOV	M,A	;STORE CHARACTER
	CALL	CI
	CALL	DELIM
	JZ	SC3
	INX	H
	DCR	C
	JZ	ERR	;TOO LONG
	JMP	SC2
;
SC3:	;END OF CURRENT SCAN, COMPARE FOR EMPTY
	LDA	OPCODE
	CPI	' '
	RET
;
HEX:	;CONVERT ACCUMULATOR TO HEXADECIMAL
	SUI	'0'
	CPI	10
	RC		;'0' - '9'
	ADI	('0'-'A'+10) AND 0FFH
	CPI	16
	RC
	JMP	ERR
;
GADDR:	;GET ADDRESS VALUE TO B (HIGH ORDER) AND C (LOW) WITH COPY OF C IN A
	CALL	SCANEXP	;READ 1 EXPRESSION
	DCR	A	;GOES TO ZERO
	JNZ	ERR	;? IF NOT A SINGLE EXPRESSION
	XCHG		;ADDRESS OF EXPRESSION TO HL
	MOV	C,M	;LOW BYTE
	INX	H
	MOV	B,M	;HIGH BYTE
	MOV	A,C	;COPY OF LOW BYTE TO A
	DCR	B
	INR	B	;SETS ZERO FLAG IF B IS ZERO
	RET
;
GBYTE:	;GET BYTE VALUE TO ACCUMULATOR AND C, CHECK FOR HIGH ORDER ZERO
	CALL	GADDR
	JNZ	ERR
	RET
;
;
;	************************************************************
;	********* ASSEMBLER MODULE STARTS HERE *********************
;	************************************************************
;
ADJ:	;MOVE REGISTER INDICATOR TO MIDDLE FIELD OF CODE
	RAL
	RAL
	RAL
	ANI	111000B
	RET
;
ADJ4:	;MOVE TO LEFT BY 4 AND MASK
	RAL
	RAL
	RAL
	RAL
	ANI	110000B
	RET
;
SEAR2:	;SAME AS SEAR, EXCEPT 2 CHARACTER MATCH
;	H,L ADDRESS TABLE TO MATCH ON
	XCHG
	LHLD	OPCODE	;2ND BYTE IN D, 1ST BYTE IN E
	XCHG		;H,L ADDRESS TABLE
SEA0:	MOV	A,E	;GET 1ST BYTE
	CMP	M	;MATCH?
	JNZ	SEA1	;TO ADDRESS NEXT ELT
	INX	H	;NEXT TO MATCH
	MOV	A,D	;2ND CHAR
	CMP	M
	RZ		;MATCH AT CURRENT ENTRY
	DCX	H
SEA1:	DCX	H
	DCX	H	;ADDRESSES NEXT ELEMENT
	DCR	C
	JNZ	SEA0	;FOR ANOTHER COMPARE
;
;	NO MATCH IN TABLE, RETURN WITH NON-ZERO VALUE
	DCR	C
	RET
;
;
SEAR:	;SEARCH FOR MATCH IN OPCODE TABLE, LENGTH OF TABLE IN REG-C
;	D,E CONTAINS ADDRESS OF BINARY EQUIVALENT OF OPCODE
;	H,L ADDRESS FOUR CHARACTER OPCODE TO MATCH
;	OPCODE CONTAINS FOUR BYTE OPCODE TYPED AT CONSOLE
;	RETURNS WITH ZERO VALUE IF OPCOE FOUND, WITH D,E
;	ADDRESSING PROPER BYTE, NON-ZERO IF NOT FOUND.
	MVI	B,4	;4 CHARACTER MATCH
;
	PUSH	D	;SAVE THE CURRENT BYTE VALUE LOCATION
	LXI	D,OPCODE	;ADDRESS CHARACTERS TYPED
SE1:	LDAX	D	;POINT TO FIRST BYTE TO MATCH
	CMP	M	;SAME CHARACTER AS TABLE?
	JNZ	SE2	;NO, SKIP TO NXT TABLE ENTRY
	INX	H	;YES, LOOK AT NEXT CHARACTER
	INX	D	;MOVE TO NEXT CHARACTER TYPED
	DCR	B	;DECREMENT CHARACTER COUNT
	JNZ	SE1	;MORE TO MATCH?
;
;	COMPLETE MATCH, RETURN WITH D,E ADDRESSING BYTE VALUE
	POP	D
	RET
;
;	MISMATCH, FINISH COUNT
SE2:	INX	H
	DCR	B
	JNZ	SE2
;
;	H,L AT END OF FOUR BYTE AREA, MOVE BACK 8
	LXI	D,-8
	DAD	D	;H,L READY FOR NXT MATCH
;
	POP	D	;RESTORE BYTE POINTER
	INX	D	;MOVE TO NEXT IN CASE MATCH OK
	DCR	C	;MORE OPCODES TO MATCH?
	JNZ	SEAR	;LOOK FOR MORE
;
;	NO MATCH FOUND IN TABLE, SET NON-ZERO VALUE AND RETURN
	DCR	C
	RET
;
;
GETREG:	;SCAN FOR SIMPLE REGISTER REFERENCE
	PUSH	B
	CALL	SCAN
	JZ	ERR
	MVI	C,8	;8 REGISTERS
	LXI	H,SREG	;SIMPLE REGISTERS
	CALL	SEAR2	;LOOK FOR 2 CHAR MATCH
	JNZ	ERR
	DCR	C
	MOV	A,C
	POP	B
	RET
;
GETD:	;GET DOUBLE PRECISION REGISTER
	PUSH	B
	CALL	SCAN
	JZ	ERR
	MVI	C,5
	LXI	H,DREG
	CALL	SEAR
	JNZ	ERR
	DCR	C
	MOV	A,C
	POP	B
	RET
;
GETDR:	;GET DOUBLE REGISTER (BDHSP)
	CALL	GETD
	CPI	4	;PSW?
	JZ	ERR
	RET
;
GETPR:	;GET PUSH/POP REGISTER (BDH OR PSW)
	CALL	GETD
	CPI	3
	JZ	ERR
	CPI	4
	RNZ
	DCR	A	;PSW MUST BE ADJUSTED
	RET
;
GCON:	;GET CONDITION CODE
;	BUFFER IS SCANNED, MOVE LEFT BEFORE COMPARE
	LXI	H,OPCODE
	LXI	D,OPCODE+1
	MVI	C,2	;MOVE TWO CHARACTERS
MOP:	LDAX	D	;LOAD CHARACTER TO MOVE
	MOV	M,A	;MOVE LEFT
	INX	H	;NEXT DESTINATION
	INX	D	;NEXT SOURCE
	DCR	C
	JNZ	MOP
;
;	MUST BE BLANK AT END
	LDAX	D
	CPI	' '
	JNZ	ERR
	MOV	M,A
;
;	NOW READY TO DO THE COMPARE
	LXI	H,CREG
	MVI	C,8
	CALL	SEAR2
	JNZ	ERR
	DCR	C
	MOV	A,C
	CALL	ADJ	;MOVE TO BITS 3,4,5 OF BYTE (LSB = 0)
	RET
;
GCONA:	;GET CONDITION CODE TO REGISTER A, DOUBLE ADDRESS TO B,C
	CALL	GCON	;CONDITION CODE TO A
	PUSH	PSW
	CALL	GADDR	;VALUE TO B,C
	POP	PSW
;	INCLUDE HIGH ORDER 11'S FOR J AND C OPCODES
	ORI	11000000B
	RET
;
SETMD:	;SET MEMORY AT LOCATION PC TO VALUE ADDRESSED BY D
	LDAX	D	;VALUE TO ACCUM
;
SETM:	;SET MEMORY AT LOCATION PC TO VALUE IN ACCUM, INC PC
	LHLD	TPC
	MOV	M,A	;STORE AT PC
	INX	H	;PC=PC+1
	SHLD	TPC
	RET
;
;
;
GETOP:	;PROCESS NEXT OPCODE
	CALL	CI
	CPI	CR
	JZ	GOBACK	;RETURN IF SIMPLE INPUT
	CPI	'.'	;ALTERNATE RETURN IS .
	JZ	GOBACK
	CALL	SCAN0
	JZ	ERR
;
CHK0:	;CHECK FOR OPCODES WITH NO OPERANDS
	MVI	C,17	;LENGTH OF GROUP-0
	LXI	H,ETAB1	;END OF GROUP-0
	LXI	D,TABLE	;FIRST BYTE VALUE
	CALL	SEAR	;LOOK FOR MATCH
	JNZ	CHK1	;NO MATCH, CHECK FOR GROUP-1
;
;	MATCHED OPCODE, D,E ADDRESS BYTE VALUE
	JMP	SETMD	;SET MEMORY AT PC AND INC PC
;
;	CHECK GROUP-1 VALUES
CHK1:	MVI	C,10	;LENGTH OF GROUP-1
	LXI	H,ETAB2
	CALL	SEAR	;D,E REMAIN SET
	JNZ	CHK2	;NO MATCH, CHECK NEXT GROUP
;
;	MATCH FOUND, SET BYTE AND GET BYTE OPERAND
	CALL	SETMD
	CALL	GBYTE	;GETS BYTE VALUE TO ACCUMULATOR
	JMP	SETM	;PUTS BYTE VALUE TO MEMORY AT PC
;
;	CHECK GROUP-2 OPCODES, REQUIRE DOUBLE BYTE OPERAND
CHK2:	MVI	C,6
	LXI	H,ETAB3
	CALL	SEAR
	JNZ	CHK3	;NO MATCH
;
;	FOUND MATCH, GET OPCODE BIT PATTERN AND STORE
	CALL	SETMD
OP2:	;ENTER HERE FOR DOUBLE BYTE OPERANDS
	CALL	GADDR	;VALUE IN B,A
	CALL	SETM
	MOV	A,B
	JMP	SETM
;
CHK3:	;CHECK FOR MOV INSTRUCTION
	MVI	C,1
	LXI	H,PMOV
	CALL	SEAR
	JNZ	CHK4
;
;	MOV INSTRUCTION GET DESTINATION OPERAND
	CALL	GETREG	;VALUE TO ACCUMULATOR
	CALL	ADJ
	MOV	B,A	;SAVE IN B
	MVI	C,01000000B	;BIT PATTERN FOR MOV
;
OP1:	;GET NEXT OPERAND FOR MOV, FIRST OPERAND FOR ACCUM/REG OPERATOR
	CALL	GETREG
	ORA	C	;SETS HIGH ORDER TWO BITS
	ORA	B	;SETS DESTINATION/OPERATOR
	JMP	SETM
;
CHK4:	;CHECK FOR GROUP-5 (ACCUM/REG OPERATOR)
	MVI	C,8
	LXI	H,ETAB5
	CALL	SEAR
	JNZ	CHK5
;
;	ACCUM/REG INSTRUCTION, C COUNTS OPERATORS AS SEARCH PROCEEDS
	DCR	C
	MOV	A,C
	CALL	ADJ
	MOV	B,A
;	OPERATOR NUMBER (SHIFTED) SAVED FOR LATER MASK
	MVI	C,10000000B	;ACCUM/REG OPERATOR INDICATOR
	JMP	OP1	;GETS OPERAND AND SAVES BYTE IN MEMORY
;
CHK5:	;MAY BE INR/DCR
	MVI	C,2
	LXI	H,PDCR
	CALL	SEAR
	JNZ	CHK6
;
;	C=2 IF DCR, =1 IF INR
	INR	C	;+1
	INR	C	;+2
	INR	C	;+3
	CALL	GETREG	;VALUE TO ACCUM
	CALL	ADJ
	ORA	C	;FILL PROPER INSTRUCTION INDICATOR
	JMP	SETM
;
CHK6:	;MAY BE A MVI INSTRUCTION
	MVI	C,1
	LXI	H,PMVI
	CALL	SEAR
	JNZ	CHK7
;
;	MVI INSTRUCTION, GET REGISTER
	CALL	GETREG	;VALUE GOES TO ACCUMULATOR
	CALL	ADJ
	ORI	110B
	CALL	SETM
	CALL	GBYTE
	JMP	SETM
;
CHK7:	;CHECK FOR GROUP-7
	MVI	C,6
	LXI	H,ETAB7
	CALL	SEAR
	JNZ	CHK8
;
;	LXI,STAX,INX,DAD,LDA, OR DCX
	MOV	A,C	;A=1...6
	CPI	4
	JC	IN0
;
;	MUST BE DAD,LDA, OR DCX
	ADI	5	;CHANGES ACCUM TO 9,10, OR 11
IN0:	;ACCUMULATOR CONTAINS CODE, SAVE IT
	MOV	B,A
	CALL	GETDR	;DOUBLE REGISTER VALUE TO ACCUM
	CALL	ADJ4	;ADJUST VALUE TO MIDDLE FIELD
	ORA	B	;FILLS REMAINING BITS
	CALL	SETM
;	MAY BE LXI
	ANI	11001111B
	CPI	1
	RNZ	;NOT LXI
	JMP	OP2	;PICK UP OPERAND
;
;
;
CHK8:	;RST?
	MVI	C,1
	LXI	H,PRST
	CALL	SEAR
	JNZ	CHK9
;
;	RST, GET OPERAND
	CALL	GBYTE
	CPI	8
	JNC	ERR
	CALL	ADJ
	ORI	11000111B
	JMP	SETM
;
CHK9:	;POP/PUSH?
	MVI	C,2
	LXI	H,PPOP+4
	CALL	SEAR
	JNZ	CHK10
;
;	C=2 IF PUSH, 1 IF POP
	DCR	C
	JNZ	PP0
;
;	POP, SET BIT PATTERN
	MVI	C,11000001B
	JMP	PP1
;
PP0:	;PUSH
	MVI	C,11000101B
PP1:	CALL	GETPR	;DOUBLE PUSH/POP REGISTER TO PROPER FIELD
	CALL	ADJ4	;MOVE TO FIELD
	ORA	C
	JMP	SETM
;
CHK10:	;J/C/R?
	LDA	OPCODE
	CPI	'J'
	JNZ	CHK11
	CALL	GCONA
;	CONDITION CODE TO FIELD IN ACCUM, ADDRESS TO B,C
	ORI	010B
	JMP	FADDR	;FILL ADDRESS
;
CHK11:	CPI	'C'
	JNZ	CHK12
	CALL	GCONA
	ORI	100B
;
FADDR:	CALL	SETM
	MOV	A,C
	CALL	SETM
	MOV	A,B
	JMP	SETM
;
CHK12:	CPI	'R'
	JNZ	ERR
	CALL	GCON
	ORI	11000000B
	JMP	SETM
;
;	************************************************************
;	*********** END OF ASSEMBLER MODULE, START DISASSEMBLER ****
;	************************************************************
;
RDBYTE:	LHLD	MPC
	PUSH	D	;SAVE DE
	XCHG		;MAX PC TO D,E
	LHLD	PC	;CURRENT PC
;	SUBTRACT PC FROM MPC, STOP IF CARRY GENERATED
	MOV	A,E
	SUB	L
	MOV	A,D
	SBB	H
	JNC	RD0
;
;	PC EXCEEDS MPC, RETURN
	LHLD	OLDSP
	SPHL		;RESTORE ORIGINAL STACK POINTER
	RET
;
RD0:	POP	D	;RESTORE D,E
	MOV	A,M
	INX	H
	SHLD	PC
	RET	

RGPRNT:	INR	A
	ANI	07
	CPI	06
	JC	RGP1
	ADI	03
RGP1:	CPI	05
	JC	RGP2
	ADI	02
RGP2:	ADI	41H
	MOV	C,A
	JMP	CO

DECODE:	MOV	B,A
	ANI	0F0H
	RRC
	RRC
	RRC
	RRC
	ADI	90H
	DAA	
	ACI	40H
	DAA	
	MOV	C,A
	CALL	CO
	MOV	A,B
	ANI	0FH
	ADI	90H
	DAA	
	ACI	40H
	DAA
	MOV	C,A
	JMP	CO

PRINT:	MVI	B,4
P1:	MOV	C,M
	CALL	CO
	INX	H
	DCR	B
	JNZ	P1
	MVI	C,' '
	JMP	CO
;
;	EXTRACT THE REGISTER FIELD FROM THE OPCODE
XTRACT:	MOV	A,D
	ANI	0011$1000B
	RRC
	RRC
	RRC
	RET
;
;	PRINT CONDITION CODE
CCPRNT:	CALL	XTRACT
	ADD	A
	MOV	C,A
	LXI	H,CCODE
	DAD	B
	MOV	C,M
	CALL	CO
	INX	H
	MOV	C,M
	CALL	CO
	MVI	C,' '
	CALL	CO
	JMP	CO

;	PRINT REGISTER REFERENCE
RPPRNT:	CALL	XTRACT
	ANI	06
	CPI	06
	JNZ	RGPRNT
	MVI	C,'S'
	CALL	CO
	MVI	C,'P'
	JMP	CO
;
;
PRPC:	;PRINT CRLF FOLLOWED BY PC VALUE
	CALL	CRLF
;	(ENTER HERE FROM DISASSEMBLER)
PRPC0:	LHLD	PC
	MOV	A,H
	CALL	DECODE
	MOV	A,L
	CALL	DECODE
	MVI	C,' '
	CALL	CO
	CALL	CO
	RET
;
DISENT:	;ENTER HERE FROM DEBUGGER
	LXI	H,0
	DAD	SP
	SHLD	OLDSP	;SP SAVED FOR LATER RETURN
;
;	CHECK FOR PAGE MODE DISPLAY
	LDA	PAGM	;GET PAGE MODE (NUMBER OF LINES TO PRINT)
	ORA	A	;SET FLAGS
	JZ	DISASM	;NOT PAGE MODE
;
;	SET MPC TO 0FFFFH
	LXI	H,0FFFFH
	SHLD	MPC
;	255 IMPLIES TRACE MODE
	INR	A
	JNZ	DISASM	;NOT TRACE MODE IF BR
;	TRACE MODE, SET TO 1 AND IGNORE ADDRESS FIELD
	INR	A	;1 IN ACC
	STA	PAGM
	LHLD	PC	;RECOVER PC
	JMP	DIS1
;
;
DISASM:
;	CHECK FOR BREAK AT CONSOLE
	CALL	BREAK
	JNZ	GOBACK
;
;	CHECK TO SEE IF ENOUGH LINES PRINTED IN PAGE MODE
	LXI	H,PAGM
	MOV	A,M
	ORA	A	;ZERO?
	JZ	DIS0	;JMP IF NOT PAGE MODE
;
;	PAGE MODE, DECREMENT AND CHECK FOR ZERO
	DCR	M
	JZ	GOBACK
;
DIS0:	LHLD	PC	;CURRENT PC
	CALL	PRLABEL	;OPTIONAL LABEL
	CALL	CRLF	;NEW LINE
	MVI	C,' '
	CALL	CO
	CALL	CO	;TWO LEADING BLANKS
	CALL	PRPC0	;PRINT THE VALUE
DIS1:	CALL	RDBYTE
;	SAVE THE OPCODE IN THE D REGISTER
	MOV	D,A
;	SEARCH THE FIRST 17 ITEMS FOR SIMPLE OPCODES
;	EI (FB) THROUGH NOP (00).  NOTE THAT THE SEARCH PROCEEDS
;	THROUGH "TABLE" STARTING AT THE BEGINNING, BUT THE OPCODES
;	ARE ACTUALLY STORED IN SYMBOLIC FORM IN REVERSE ORDER.
;
	LXI	H,TABLE
	LXI	B,17	;FIRST 17 SIMPLE OPCODES
GROUP1:	CMP	M	;TABLE VALUE = OPCODE?
	JZ	TYPE1	;SKIP TO PRINT IF SO
	INX	H	;MOVE TO THE NEXT TABLE ELEMENT
	DCR	C	;COUNT THE SIMPLE OPCODES DOWN
	JNZ	GROUP1	;TRY FOR ANOTHER
;
;	NOT A SIMPLE OPERATION CODE, CHECK FOR IMMEDIATE OP
;	ADI, ACI, OUT, SUI, IN, SBI, ANI, XRI, ORI, CPI
	MVI	C,10
GROUP2:	CMP	M
	JZ	TYPE2
	INX	H
	DCR	C
	JNZ	GROUP2
;
;	NOT AN IMMEDIATE OPERATION, CHECK FOR
;	SHLD LHLD STA LDA JMP OR CALL
	MVI	C,6
GROUP3:	CMP	M
	JZ	TYPE3
	INX	H
	DCR	C
	JNZ	GROUP3
;
;	NOT TYPE3 OPERATION CODE, CHECK FOR MOV
;	BY MASKING THE HIGH ORDER TWO FIBITS -
;	XX00 0000 IS PRODUCED IN THE ACCUMULATOR
	ANI	0C0H
;	MOV IS GIVEN BY 01 DDD SSS (DDD IS DEST, SSS IS SOURCE)
	CPI	40H
	JZ	MOVOP
;
;	NOT A MOV INSTRUCTION, CHECK FOR ACCUMLATOR-REGISTER OPS
;	BIT PATTERN 10 CCC RRR    CORRESPONDS TO
;	ADD (0), ADC (1), SUB (2), SBB (3), ANA (4),
;	XRA (5), ORA (6), CMP (7)
	CPI	80H
	JZ	ACCREG
;
;	NOT ACCUM-REGISTER, RESTORE OPCODE FOR FURTHER CHECKS
	MOV	A,D
;
;	LOOK FOR INR, DCR, AND MVI OPERATIONS
;	INR = 00 RRR 100, DCR = 00 RRR 101, MVI = 00 RRR 110
;	OR, INR = 00 RRR 4, DCR = 00 RRR 5, MVI = 00 RRR 6
	ANI	1100$0111B
	SUI	04
;	INR GOES TO ZERO
	JZ	INRREG
;	NOT INR, MAY BE DCR
	DCR	A
	JZ	DCRREG
	DCR	A
;	NOT DCR, MAY BE MVI
	JZ	MVIREG
;	NOT INR, DCR, OR MVI INSTRUCTION
;
;	RESTORE THE OPCODE
	MOV	A,D
;	LOOK FOR LXI STAX INX DAD LDAX DCX OPCODES
;	LXI = 00 RR 0001,
;	STAX= 00 RR 0010,
;	INX = 00 RR 0011,
;	DAD = 00 RR 1001,
;	LDAX= 00 RR 1010
;	DCX = 00 RR 1011
	ANI	0C0H
	JZ	LXILST	;TO PROCESS FURTHER
;
;	NOT ONE OF THE ABOVE, CHECK FURTHER
;	MUST BE OF THE FORM - 11 XXX XXX
	MOV	A,D
	ANI	0000$0111B	;TO EXTRACT THE RIGHTMOST BITS
;	RETURN CONDITIONALS TAKE THE FORM 11 XXX 000
	JZ	RETCON		;RETURN CONDITIONALLY
;	JUMP CONDITIONALS TAKE THE FORM 11 XXX 010 = 2
	SUI	02
	JZ	JMPCON
;	CALL CONDITIONALS TAKE THE FORM 11 XXX 100 = 4 - 2 = 2
	SUI	02
	JZ	CALLCON
;	RST'S TAKE THE FORM 11 XXX 111 = 7 - 4 = 3
	SUI	03
	JZ	RSTOP
;
;	NONE OF THE ABOVE, PUSHES AND POP'S REMAIN
	MOV	A,D		;RESTORE OPCODE
;	FIRST CAPTURE REMAINING OPCODES CB, D9, DD, ED, FD
	ANI	0000$1000B	;THIS BIT RESET FOR POP,PUSH
	JNZ	N8080		;NOT 8080 OPCODE IF SET
	MOV	A,D		;RESTORE IT
;	PUSH = 11 XX0 101 = 5, POP = 11 XX0 001 = 1
	ANI	07
;	USE THE RESULTING VALUE TO INDEX TO REGISTER TABLE
	MOV	C,A
	DCR	A	;POP GOES TO 00
	LXI	H,PPOP-1
	DAD	B
	CALL	PRINT
;	GET THE RELEVANT REGISTER
	CALL	XTRACT
;	CHECK FOR PSW OPERATION CODE
	CPI	06
	JNZ	D6
	LXI	H,PPSW
	CALL	PRINT
	JMP	DISASM
;
;	PRINT RST XXX INSTRUCTION
RSTOP:	LXI	H,PRST
	CALL	PRINT
	CALL	XTRACT
	CALL	DECODE
	JMP	DISASM
;
;	CALL CONDITIONAL 'C'
CALLCON:
	MVI	C,'C'
	CALL	CO
	CALL	CCPRNT
	JMP	PREXT	;TO PRINT THE ADDRESS
;
;	JUMP CONDITIONAL 'J'
JMPCON:
	MVI	C,'J'
	CALL	CO
	CALL	CCPRNT
	JMP	PREXT	;TO PRINT THE ADDRESS
;
;	RETURN CONDITIONAL 'R'
RETCON:
	MVI	C,'R'
	CALL	CO
	CALL	CCPRNT
	JMP	DISASM
;
;
;	PROCESS ONE OF LXI STAX INX DAD LDAX DCX
LXILST:	LXI	H,PLXI
;	CAPTURE 08, 10, 18, 20, 28, 30, AND 38
	MOV	A,D	;GET OPCODE
	ANI	111B	;RIGHTMOST BITS ZERO?
	JZ	N8080	;NOT 8080 IF SO
;	RECALL OPCODE TO DETERMINE WHICH ONE
	MOV	A,D
;	FIND THE PARTICULAR OPCODE
	ANI	0FH
;	LXI HAS LEAST SIGNIFICANT FOUR BITS = 0001
	DCR	A
	JZ	LXIREG
;	STAX 0010 BECOMES 0001 = 1
;	INX  0011 BECOMES 0010 = 2
;	DAD  1001 BECOMES 1000 = 8
;	LDAX 1010 BECOMES 1001 = 9
;	DCX  1011 BECOMES 1010 = 10
	CPI	03
	JC	D4
;	MUST BE DAD, LDAX OR DCX
	SUI	05
;	DAD 8 BECOMES 3
;	LDAX9 BECOMES 4
;	DCX10 BECOMES 5
;	ACCUMULATOR NORMALIZED
D4:	ADD	A
	ADD	A
;	VALUE IN ACCUM MULTIPLIED BY FOUR
	MOV	C,A
	DAD	B
	CALL	PRINT
;	STAX, INX, DAD, LDAX, OR DC X PRINTED, PRINT REGISTER
	CALL	RPPRNT
	JMP	DISASM
;
;	PRINT REGISTER ADDRESSED BY HL (E.G., IN LXI)
LXIREG:	CALL	PRINT
	CALL	RPPRNT
	MVI	C,','
	CALL	CO
	JMP	PREXT	;TO PRINT THE EXTENDED INSTRUCTION
;
;
MVIREG:	LXI	H,PMVI
	CALL	PRINT
	CALL	XTRACT
	CALL	RGPRNT
	MVI	C,','
	CALL	CO
	JMP	DATA8
;
DCRREG:	LXI	H,PDCR
	JMP	D5
;
INRREG:	LXI	H,PINR
;
;	PRINT THE INSTRUCTION GIVEN BY HL, FOLLOWED BY REGISTER
D5:	CALL	PRINT
	CALL	XTRACT
D6:	CALL	RGPRNT
	JMP	DISASM
;
;	FOUND ACCUM REGISTER OPERATION - MIDDLE BITS GIVE PCODE
ACCREG:	MOV	A,D
	ANI	38H	;SELECT OPCODE BITS
	RRC		;OPCODE * 4 FOR LENGTH FOUR STRING
	MOV	C,A
	LXI	H,PADD	;ADDRESS THE ACCUM-REGISTER LIST
	DAD	B
	CALL	PRINT
	JMP	D9
;
;	MOV OPERATION FOUND
MOVOP:	LXI	H,PMOV
	CALL	PRINT
	CALL	XTRACT
	CALL	RGPRNT
	MVI	C,','	;REGISTER DELIMITER
	CALL	CO
D9:	MOV	A,D
	ANI	07
	CALL	RGPRNT
	JMP	DISASM
;
;	TYPE GROUP3: CALL JMP LDA STA LHLD SHLD
TYPE3:
	MOV	A,C	;*4 FOR LENGTH 4
	ADD	A
	ADD	A
	MOV	C,A
	LXI	H,TAB3-4
	DAD	B
	CALL	PRINT
;
;	ARRIVE HERE TO PRINT THE ADDRESS FIELD
PREXT:	CALL	RDBYTE	;LOW ADDRESS TO A
	PUSH	PSW	;SAVE IT
	CALL	RDBYTE
	MOV	D,A	;SET HIGH ADDRESS
	POP	PSW	;RECALL LOW ADDRESS
	MOV	E,A	;DE IS THE ADDRESS TO PRINT
	CALL	PADDX
	JMP	DISASM
;
;	TYPE THE IMMEDIATE OPCODES (INCLUDING IN/OUT)
TYPE2:	MOV	A,C	;TYPE IMMEDIATE OPERATION CODE
	ADD	A	;*2
	ADD	A	;*4 FOR LENGTH FOUR CHAR STRING
	MOV	C,A	;BC = INDEX * 4 FOR OPCODE
	LXI	H,TAB2-4
	DAD	B
	CALL	PRINT
;
;	ARRIVE HERE TO PRINT THE IMMEDIATE VALUE
DATA8:	CALL	RDBYTE
	CALL	PBYTE	;BYTE VALUE PRINTED
	JMP	DISASM
;
;	FOUND OPCODE IN TABLE, POSITION GIVEN
;	BY COUNT IN BC (NOTE THAT C IS COUNTED DOWN, WHILE
;	INDEX WAS MOVING UP THE TABLE DURING THE SEARCH)
TYPE1:	MOV	A,C	;TYPE SIMPLE OPCODES FROM GROUP 1
	ADD	A	;POSITION * 2
	ADD	A	;POSITION * 4 (FOUR CHAR CODES)
	MOV	C,A	;BC IS INDEX * 4 OF OPCODE
	LXI	H,TAB1-4
	DAD	B	;HL NOW HOLDS ADDRESS OF CODE TO PRINT
	CALL	PRINT
	JMP	DISASM
;
N8080:	;NOT AN 8080 OPERATION CODE
	LXI	H,DBOP
	CALL	PRINT	;PRINT THE '??='
	MOV	A,D	;GET THE OPCODE
	CALL	PBYTE	;AND PRINT IT
	JMP	DISASM
;
ERR:	;ENTER HERE FOR ERROR REPORTING
	CALL	CRLF
	MVI	C,'?'
	CALL	CO
;
	LHLD	OLDSP
	SPHL
;	PC REMAINS UNCHANGED
;
;
ASMEN:	;ENTER HERE FROM DEBUGGER
	LXI	H,0
	DAD	SP
	SHLD	OLDSP
;
ASM0:	CALL	PRPC	;PRINT PC VALUE
	SHLD	TPC	;SAVE PC VALUE
	CALL	GETBUFF	;FILL INPUT BUFFER
	CALL	GETOP	;GET OPERATION
;	UPDATE PC, MUST BE CORRECT INPUT
	LHLD	TPC
	SHLD	PC
	JMP	ASM0
;
GOBACK:	LHLD	OLDSP
	SPHL
	RET
;
;	THE FIRST 17 ITEMS CORRESPOND TO SIMPLE OPCODES
;	(NOP BACKWARD THROUGH EI)
TABLE:	DB	000H,007H,00FH,017H	;NOP RLC RRC RAL
	DB	01FH,027H,02FH,037H	;RAR DAA CMA STC
	DB	03FH,076H,0C9H,0E3H	;CMC HLT RET XTHL
	DB	0E9H,0EBH,0F3H,0F9H	;PCHL XCHG DI SPHL
	DB	0FBH			;EI
;
;	THE NEXT 10 ITEMS CORRESPOND TO THE IMMEDIATE OPCODES
	DB	0C6H,0CEH,0D3H		;ADI ACI OUT
	DB	0D6H,0DBH,0DEH,0E6H	;SUI IN SBI ANI
	DB	0EEH,0F6H,0FEH		;XRI ORI

	DB	022H			;SHLD
	DB	02AH,032H,03AH,0C3H
	DB	0CDH
TAB1:	DB	'EI  ','SPHL','DI  ','XCHG'



	DB	'PCHL','XTHL','RET ','HLT '


	DB	'CMC ','STC ','CMA ','DAA '


	DB	'RAR ','RAL ','RRC ','RLC '


ETAB1:	DB	'NOP '
TAB2:	DB	'CPI ','ORI ','XRI ','ANI '


	DB	'SBI ','IN  ','SUI ','OUT '


	DB	'ACI '
ETAB2:	DB	'ADI '
;
TAB3:	DB	'CALL','JMP ','LDA ','STA '


	DB	'LHLD' 
ETAB3:	DB	'SHLD'
;

PMOV:	DB	'MOV '
PADD:	DB	'ADD ','ADC ','SUB ','SBB '


	DB	'ANA ','XRA ','ORA '
ETAB5:	DB	'CMP '

PINR:	DB	'INR '
PDCR:	DB	'DCR '
PMVI:	DB	'MVI '
PLXI:	DB	'LXI ','STAX','INX ','DAD '


	DB	'LDAX'
ETAB7:	DB	'DCX '
;

PRST:	DB	'RST '
PPSW:	DB	'PSW '
PPOP: 	DB	'POP ','PUSH'

CCODE:	DB	'NZ','Z ','NC','C '

	DB	'PO','PE','P '
CREG:	DB	'M '

	DB	'B ','C ','D ','E '
	DB	'H ','L ','M '
SREG:	DB	'A '
;
	DB	'B   ','D   ','H   ','SP  '
DREG:	DB	'PSW '
;
DBOP:	DB	'??= '
OPCODE:	DS	4

	IF	RMAC	;[JCE] Pack the module out to exactly 680h bytes
	DS	2	;      so that DEMON: shows as 0680 in the output
	ENDIF		;      from LINK.COM

	END
