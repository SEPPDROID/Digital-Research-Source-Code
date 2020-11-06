	TITLE	'ASM MAIN MODULE'
;	MP/M RESIDENT ASSEMBLER MAIN PROGRAM
;
;		COPYRIGHT  (C) 1976, 1977, 1978, 1979, 1980, 1981
;		DIGITAL RESEARCH
;		BOX 579, PACIFIC GROVE
;		CALIFORNIA, 93950
;
;		Revised:
;		  14 Sept 81  by Thomas Rolander
;
;
	org	0
base	equ	$

	ORG	1BA0H
;	MODULE ENTRY POINTS
IOMOD	EQU	base+200H	;IO MODULE
SCMOD	EQU	base+1100H	;SCANNER MODULE
SYMOD	EQU	base+1340H	;SYMBOL TABLE MODULE
BMOD	EQU	base+15A0H	;BINARY SEARCH MODULE
OPMOD	EQU	base+1860H	;OPERAND SCAN MODULE
;
SETUP	EQU	IOMOD+3H	;FILE SETUP FOR EACH PASS
PCON	EQU	IOMOD+12H	;WRITE CONSOLE BUFFER TO CR
WOBUFF	EQU	IOMOD+15H	;WRITE PRINT BUFFER AND REINITIALIZE
PERR	EQU	IOMOD+18H	;WRITE ERROR CHARACTER TO PRINT BUFFER
DHEX	EQU	IOMOD+1BH	;SEND HEX CHARACTER TO MACHINE CODE FILE
EOR	EQU	IOMOD+1EH	;END OF PROCESSING, CLOSE FILES AND TERMINATE
;
INITS	EQU	SCMOD+3H	;INITIALIZE SCANNER MODULE
SCAN	EQU	SCMOD+6H	;SCAN NEXT TOKEN
;
INISY	EQU	SYMOD+3H	;INITIALIZE SYMBOL TABLE
LOOKUP	EQU	SYMOD+6H	;LOOKUP SYMBOL IN ACCUMULATOR
FOUND	EQU	SYMOD+9H	;FOUND IF NZ FLAG
ENTER	EQU	SYMOD+0CH	;ENTER SYMBOL IN ACCUMULATOR
SETTY	EQU	SYMOD+0FH	;SET TYPE FIELD
GETTY	EQU	SYMOD+12H	;GET TYPE FIELD
SETVAL	EQU	SYMOD+15H	;SET VALUE FIELD
GETVAL	EQU	SYMOD+18H	;GET VALUE FIELD
;
BGET	EQU	BMOD+6H		;BINARY SEARCH AND GET TYPE/VALUE PAIR
;
OPAND	EQU	OPMOD+3H	;GET OPERAND VALUE TO 'EVALUE'
MULF	EQU	OPMOD+6H	;MULT D,E BY H,L TO H,L
DIVF	EQU	OPMOD+9H	;DIVIDE HL BY DE, RESULT TO DE
;
;
;	COMMON EQUATES
PBMAX	EQU	90	;MAX PRINT SIZE
PBUFF	EQU	base+10CH	;PRINT BUFFER
PBP	EQU	PBUFF+PBMAX	;PRINT BUFFER POINTER
;
TOKEN	EQU	PBP+1	;CURRENT TOKEN UDER SCAN
VALUE	EQU	TOKEN+1	;VALUE OF NUMBER IN BINARY
ACCLEN	EQU	VALUE+2	;ACCUMULATOR LENGTH
ACMAX	EQU	64	;MAX ACCUMULATOR LENGTH
ACCUM	EQU	ACCLEN+1
;
EVALUE	EQU	ACCUM+ACMAX	;VALUE FROM EXPRESSION ANALYSIS
;
SYTOP	EQU	EVALUE+2	;CURRENT SYMBOL TOP
SYMAX	EQU	SYTOP+2		;MAX ADDRESS+1
;
PASS	EQU	SYMAX+2	;CURRENT PASS NUMBER
FPC	EQU	PASS+1	;FILL ADDRESS FOR NEXT HEX BYTE
ASPC	EQU	FPC+2	;ASSEMBLER'S PSEUDO PC
SYBAS	EQU	ASPC+2	;BASE OF SYMBOL TABLE
SYADR	EQU	SYBAS+2	;CURRENT SYMBOL ADDRESS
;
;	GLOBAL EQUATES
IDEN	EQU	1	;IDENTIFIER
NUMB	EQU	2	;NUMBER
STRNG	EQU	3	;STRING
SPECL	EQU	4	;SPECIAL CHARACTER
;
PLABT	EQU	0001B	;PROGRAM LABEL
DLABT	EQU	0010B	;DATA LABEL
EQUT	EQU	0100B	;EQUATE
SETT	EQU	0101B	;SET
MACT	EQU	0110B	;MACRO
;
EXTT	EQU	1000B	;EXTERNAL
REFT	EQU	1011B	;REFER
GLBT	EQU	1100B	;GLOBAL
;
CR	EQU	0DH	;CARRIAGE RETURN
LF	EQU	0AH	;LINE FEED
EOF	EQU	1AH	;END OF FILE
NBMAX	EQU	16	;STARTING POSITION OF PRINT LINE
;
;
RT	EQU	16	;REGISTER TYPE
PT	EQU	RT+1	;PSEUDO OPERATION
PENDIF	EQU	5	;PSEUDO OPERATOR 'ENDIF'
OBASE	EQU	PT+1
O1	EQU	OBASE+1	;FIRST OPERATOR
O15	EQU	OBASE+15;LAST OPERATOR
;
;	MAIN STATEMENT PROCESSING LOOP
	XRA	A
	STA	PASS	;SET TO PASS 0 INITIALLY
	CALL	INISY	;INITIALIZE THE SYMBOL TABLE
RESTART:	;PASS LOOP GOES FROM 0 TO 1
	CALL	INITS	;INITIALIZE THE SCANNER
	CALL	SETUP	;SET UP THE INPUT FILE
	LXI	H,0
	SHLD	SYLAB	;ASSUME NO STARTING LABEL
	SHLD	FPC
	SHLD	ASPC
	SHLD	EPC	;END PC
;
SCNEXT:	;SCAN THE NEXT INPUT ITEM
	CALL	SCAN
SCN0:	LDA	TOKEN
	CPI	NUMB	;SKIP LEADING NUMBERS FROM LINE EDITORS
	JZ	SCNEXT
	CPI	SPECL	;MAY BE PROCESSOR TECH'S COMMENT
	JNZ	SCN1
;	SPECIAL CHARACTER, CHECK FOR *
	LDA	ACCUM
	CPI	'*'
	JNZ	CHEND	;END OF LINE IF NOT *
;	* FOUND, NO PRECEDING LABEL ALLOWED
	CALL	SETLA
	JNZ	STERR	;ERROR IF LABEL
	JMP	CHEN1	;SCAN THE COMMENT OTHERWISE
;
SCN1:	;NOT NUMBER OR SPECIAL CHARACTER, CHECK FOR IDENTIFIER
	CPI	IDEN
	JNZ	STERR	;ERROR IF NOT
;
;	IDENTIFIER FOUND, MAY BE LABEL, OPCODE, OR MACRO
	CALL	BGET	;BINARY SEARCH FIXED DATA
	JZ	CHKPT	;CHECK FOR PSEUDO OR REAL OPERATOR
;
;	BINARY SEARCH WAS UNSUCCESSFUL, CHECK FOR MACRO
	CALL	LOOKUP
	CALL	FOUND
	JNZ	LFOUN	;NZ FLAG SET IF FOUND
;
;	NOT FOUND, ENTER IT
	CALL	ENTER	;THIS MUST BE PASS 0
	LDA	PASS
	ORA	A
	CNZ	ERRP	;PHASE ERROR IF NOT
	JMP	SETSY	;SET SYLAB
;
;	ITEM WAS FOUND, CHECK FOR MACRO
LFOUN:	CALL	GETTY
	CPI	MACT
	JNZ	SETSY
;
;	MACRO DEFINITION FOUND, EXPAND MACRO
	CALL	ERRN	;NOT CURRENTLY IMPLEMENTED
	JMP	CHEN1	;SCANS TO END OF CURRENT LINE
;
SETSY:	;LABEL FOUND - IS IT THE ONLY ONE?
	LHLD	SYLAB
	MOV	A,L
	ORA	H
	CNZ	ERRL	;LABEL ERROR IF NOT
	LHLD	SYADR	;ADDRESS OF SYMBOL
	SHLD	SYLAB	;MARK AS LABEL FOUND
;
;	LABEL FOUND, SCAN OPTIONAL ':'
	CALL	SCAN
	LDA	TOKEN
	CPI	SPECL
	JNZ	SCN0	;SKIP NEXT SCAN IF NOT SPECIAL
	LDA	ACCUM
	CPI	':'
	JNZ	SCN0
	JMP	SCNEXT	;TO IGNORE ':'
;
;	BINARY SEARCH FOUND SYMBOL, CHECK FOR PSEUDO OR REAL OP
CHKPT:	CPI	PT	;PSEUDO OPCODE?
	JNZ	CHKOT
;
;	PSEUDO OPCODE FOUND, BRANCH TO CASES
	MOV	E,B	;B HAS PARTICULAR OPERATOR NUMBER
	MVI	D,0	;DOUBLE PRECISION VALUE TO D,E
	DCX	D	;BIASED BY +1
	LXI	H,PTTAB	;BASE OF JUMP TABLE
	DAD	D
	DAD	D
	MOV	E,M
	INX	H
	MOV	H,M
	MOV	L,E
	PCHL		;JUMP INTO TABLE
;
PTTAB:	;PSEUDO OPCODE JUMP TABLE
	DW	SDB	;DB
	DW	SDS	;DS
	DW	SDW	;DW
	DW	SEND	;END
	DW	SENDIF	;ENDIF
	DW	SENDM	;ENDM
	DW	SEQU	;EQU
	DW	SIF	;IF
	DW	SMACRO	;MACRO
	DW	SORG	;ORG
	DW	SSET	;SET
	DW	STITLE	;TITLE
;
SDB:
	CALL	FILAB	;SET LABEL FOR THIS LINE TO ASPC
SDB0:
	CALL	SCAN	;PAST DB TO NEXT ITEM
	LDA	TOKEN	;LOOK FOR LONG STRING
	CPI	STRNG
	JNZ	SDBC	;SKIP IF NOT STRING
	LDA	ACCLEN
	DCR	A	;LENGTH 1 STRING?
	JZ	SDBC
;	LENGTH 0,2,... STRING
	MOV	B,A
	INR	B
	INR	B	;BECOMES 1,3,... FOR 0,2,... LENGTHS
	LXI	H,ACCUM	;ADDRESS CHARACTERS IN STRING
SDB1:	DCR	B	;COUNT DOWN TO ZERO
	JZ	SDB2	;SCAN DELIMITER AT END OF STRING
	PUSH	B	;SAVE COUNT
	MOV	B,M	;GET CHARACTER
	INX	H
	PUSH	H	;SAVE ACCUM POINTER
	CALL	FILHB	;SEND TO HEX FILE
	POP	H
	POP	B
	JMP	SDB1
SDB2:	CALL	SCAN	;TO THE DELIMITER
	JMP	SDB3
;
;	NOT A LONG STRING
SDBC:	CALL	OPAND	;COMPUTE OPERAND
	LHLD	EVALUE	;VALUE TO H,L
	MOV	A,H
	ORA	A	;HIGH ORDER MUST BE ZERO
	CNZ	ERRD	;DATA ERROR
	MOV	B,L	;GET LOW BYTE
	CALL	FILHB
SDB3:	;END OF ITEM - UPDATE ASPC
	CALL	SETAS	;SET ASPC TO FPC
	CALL	DELIM
	CPI	','
	JZ	SDB0	;FOR ANOTHER ITEM
	JMP	CHEND	;CHECK END OF LINE SYNTAX
;
SDS:
	CALL	FILAB	;HANDLE LABEL IF IT OCCURRED
	CALL	PADD	;PRINT ADDRESS
	CALL	EXP16	;SCAN AND GET 16BIT OPERAND
	XCHG		;TO D,E
	LHLD	ASPC	;CURRENT PSEUDO PC
	DAD	D	;+EXPRESSION
	SHLD	ASPC
	SHLD	FPC	;NEXT TO FILL
	JMP	CHEND
;
SDW:
	CALL	FILAB	;HANDLE OPTIONAL LABEL
SDW0:
	CALL	EXP16	;GET 16BIT OPERAND
	PUSH	H	;SAVE A COPY
	MOV	B,L	;LOW BYTE FIRST
	CALL	FILHB	;SEND LOW BYTE
	POP	H	;RECLAIM A COPY
	MOV	B,H	;HIGH BYTE NEXT
	CALL	FILHB	;SEND HIGH BYTE
	CALL	SETAS	;SET ASPC=FPC
	CALL	DELIM	;CHECK DELIMITER SYNTAX
	CPI	','
	JZ	SDW0	;GET MORE DATA
	JMP	CHEND
;
SEND:
	CALL	FILAB
	CALL	PADD	;WRITE LAST LOC
	LDA	PBUFF
	CPI	' '
	JNZ	CHEND
	CALL	EXP16	;GET EXPRESSION IF IT'S THERE
	LDA	PBUFF
	CPI	' '
	JNZ	SEND0
	SHLD	EPC	;EXPRESSION FOUND, STORE IT FOR LATER
SEND0:	MVI	A,' '
	STA	PBUFF	;CLEAR ERROR, IF IT OCCURRED
	CALL	SCAN	;CLEAR CR
	LDA	TOKEN
	CPI	SPECL
	JNZ	STERR
	LDA	ACCUM
	CPI	LF
	JNZ	STERR
	JMP	ENDAS	;END OF ASSEMBLER
;
SENDIF:
	JMP	POEND
;
SENDM:
	CALL	ERRN
	JMP	POEND
;
SEQU:
	CALL	SETLA
	JZ	STERR	;MUST BE A LABEL
	LHLD	ASPC	;HOLD TEMP ASPC
	PUSH	H	;IN STACK
	CALL	EXP16	;GET 16BIT OPERAND
	SHLD	ASPC	;VALUE OF EXPRESSION
	CALL	FILAB
	CALL	PADDR	;COMPUTED VALUE
	LXI	H,PBUFF+6	;SPACE AFTER VALUE
	MVI	M,'='
	POP	H	;REAL ASPC
	SHLD	ASPC	;CHANGE BACK
	JMP	CHEND
;
SIF:
	CALL	FILAB	;IN CASE OF LABEL
	CALL	EXP16	;GET IF EXPRESSION
	LDA	PBUFF
	CPI	' '
	JNZ	CHEND	;SKIP IF ERROR
	MOV	A,L	;GET LSB
	RAR
	JC	CHEND	;TRUE IF CARRY BIT SET
;
;	SKIP TO EOF OR ENDIF
SIF0:	CALL	SCAN
	LDA	TOKEN
	CPI	SPECL
	JNZ	SIF1
	LDA	ACCUM
	CPI	EOF
	MVI	A,'B'	;BALANCE ERROR
	CZ	PERR
	JZ	ENDAS
	JMP	SIF0	;FOR ANOTHER
SIF1:	;NOT A SPECIAL CHARACTER
	CPI	IDEN
	JNZ	SIF0	;NOT AN IDENTIFIER
	CALL	BGET	;LOOK FOR ENDIF
	JNZ	SIF0	;NOT FOUND
	CPI	PT	;PSEUDO OP?
	JNZ	SIF0
	MOV	A,B	;GET OPERATOR NUMBER
	CPI	PENDIF	;ENDIF?
	JNZ	SIF0	;GET ANOTHER TOKEN
	JMP	POEND	;OK, CHECK END OF LINE
;
SMACRO:
	CALL	ERRN
	JMP	CHEND
;
SORG:
	CALL	EXP16
	LDA	PBUFF
	CPI	' '
	JNZ	CHEND	;SKIP ORG IF ERROR
	SHLD	ASPC	;CHANGE PC
	SHLD	FPC	;CHANGE NEXT TO FILL
	CALL	FILAB	;IN CASE OF LABEL
	CALL	PADD
	JMP	CHEND
;
SSET:
	CALL	SETLA
	JZ	STERR	;MUST BE LABELLED
;
	CALL	GETTY
	CPI	SETT
	CNZ	ERRL	;LABEL ERROR
	MVI	A,SETT
	CALL	SETTY	;REPLACE TYPE WITH 'SET'
	CALL	EXP16	;GET THE EXPRESSION
	PUSH	H	;SAVE IT
	CALL	SETLA	;RE-ADDRESS LABEL
	POP	H	;RECLAIM IT
	CALL	SETVAL
	LXI	H,0
	SHLD	SYLAB	;PREVENT LABEL PROCESSING
	JMP	CHEND
;
;
STITLE:
	CALL	ERRN	;NOT IMPLEMENTED
;
POEND:	;PSEUDO OPERATOR END - SCAN TO NEXT TOKEN
	CALL	SCAN
	JMP	CHEND
;
;	NOT A PSEUDO OPCODE, CHECK FOR REAL OPCODE
CHKOT:	SUI	O1	;BASE OF OPCODES
	CPI	O15	;PAST LAST OPCODE?
	JNC	STERR	;STATEMENT ERROR IF SO
;
;	FOUND OPCODE, COMPUTE INDEX INTO TABLE AND JUMP TO CASE
	MOV	E,A
	MVI	D,0
	LXI	H,OPTAB
	DAD	D
	DAD	D
	MOV	E,M
	INX	H
	MOV	H,M
	MOV	L,E
	PCHL		;JUMP TO CASE
;
OPTAB:	;OPCODE CATEGORIES
	DW	SSIMP	;SIMPLE
	DW	SLXI	;LXI
	DW	SDAD	;DAD
	DW	SPUSH	;PUSH/POP
	DW	SJMP	;JMP/CALL
	DW	SMOV	;MOV
	DW	SMVI	;MVI
	DW	SACCI	;ACCUM IMMEDIATE
	DW	SLDAX	;LDAX/STAX
	DW	SLHLD	;LHLD/SHLD/LDA/STA
	DW	SACCR	;ACCUM-REGISTER
	DW	SINC	;INC/DCR
	DW	SINX	;INX/DCX
	DW	SRST	;RESTART
	DW	SIN	;IN/OUT
;
SSIMP:	;SIMPLE OPERATION CODES
	CALL	FILHB	;SEND HEX VALUE TO MACHINE CODE FILE
	CALL	SCAN	;TO NEXT TOKEN
	JMP	INCPC
;
SLXI:	;LXI H,16B
	CALL	SHDREG	;SCAN DOUBLE PRECISION REGISTER
	CALL	CHCOM	;CHECK FOR COMMA FOLLOWING REGISTER
	CALL	SETADR	;SCAN AND EMIT DOUBLE PRECISION OPERAND
	JMP	INCPC
;
SDAD:	;DAD B
	CALL	SHDREG	;SCAN AND EMIT DOUBLE PRECISION REGISTER
	JMP	INCPC
;
SPUSH:	;PUSH B POP D
	CALL	SHREG	;SCAN SINGLE PRECISION REGISTER TO A
	CPI	111000B	;MAY BE PSW
	JZ	SPU0
;	NOT PSW, MUST BE B,D, OR H
	ANI	001000B	;LOW BIT MUST BE 0
	CNZ	ERRR	;REGISTER ERROR IF NOT
SPU0:	MOV	A,C	;RECALL REGISTER AND MASK IN CASE OF ERROR
	ANI	110000B
	ORA	B	;MASK IN OPCODE FOR PUSH OR POP
	JMP	FILINC	;FILL HEX VALUE AND INCREMENT PC
;
SJMP:	;JMP 16B/ CALL 16B
	CALL	FILHB	;EMIT JMP OR CALL OPCODE
	CALL	SETADR	;EMIT 16BIT OPERAND
	JMP	INCPC
;
SMOV:	;MOV A,B
	CALL	SHREG
	ORA	B	;MASK IN OPCODE
	MOV	B,A	;SAVE IN B TEMPORARILY
	CALL	CHCOM	;MUST BE COMMA SEPARATOR
	CALL	EXP3	;VALUE MUST BE 0-7
	ORA	B	;MASK IN OPCODE
	JMP	FILINC
;
SMVI:	;MVI A,8B
	CALL	SHREG
	ORA	B	;MASK IN OPCODE
	CALL	FILHEX	;EMIT OPCODE
	CALL	CHCOM	;SCAN COMMA
	CALL	SETBYTE	;EMIT 8BIT VALUE
	JMP	INCPC
;
SACCI:	;ADI 8B
	CALL	FILHB	;EMIT IMMEDIATE OPCODE
	CALL	SETBYTE	;EMIT 8BIT OPERAND
	JMP	INCPC
;
SLDAX:	;LDAX B/STAX D
	CALL	SHREG
	ANI	101000B	;MUST BE B OR D
	CNZ	ERRR	;REGISTER ERROR IF NOT
	MOV	A,C	;RECOVER REGISTER NUMBER
	ANI	010000B	;CHANGE TO B OR D IF ERROR
	ORA	B	;MASK IN OPCODE
	JMP	FILINC	;EMIT OPCODE
;
SLHLD:	;LHLD 16B/ SHLD 16B/ LDA 16B/ STA 16B
	CALL	FILHB	;EMIT OPCODE
	CALL	SETADR	;EMIT OPERAND
	JMP	INCPC
;
SACCR:	;ADD B
	CALL	EXP3	;RIGHT ADJUSTED 3BIT VALUE FOR REGISTER
	ORA	B	;MASK IN OPCODE
	JMP	FILINC
;
SINC:	;INR B/DCR D
	CALL	SHREG	;GET REGISTER
	ORA	B
	JMP	FILINC
;
SINX:	;INX H/DCX B
	CALL	SHREG
	ANI	001000B	;MUST BE B D M OR SP
	CNZ	ERRR	;REGISTER ERROR IF NOT
	MOV	A,C	;RECOVER REGISTER
	ANI	110000B	;IN CASE OF ERROR
	ORA	B	;MASK IN OPCODE
	JMP	FILINC
;
SRST:	;RESTART 4
	CALL	SHREG	;VALUE IS 0-7
	ORA	B	;OPCODE MASKED
	JMP	FILINC
;
SIN:	;IN 8B/OUT 8B
	CALL	FILHB	;EMIT OPCODE
	CALL	SETBYTE	;EMIT 8BIT OPERAND
	JMP	INCPC
;
FILINC:	;FILL HEX VALUE FROM A BEFORE INCREMENTING PC
	CALL	FILHEX
;
INCPC:	;CHANGE ASSEMBLER'S PSEUDO PROGRAM COUNTER
	CALL	FILAB	;SET ANY LABELS WHICH OCCUR ON THE LINE
	CALL	SETAS	;ASPC=FPC
	JMP	CHEND	;END OF LINE SCAN
;
;
;	UTILITY SUBROUTINES FOR OPERATION CODES
;
DELIM:	;CHECK DELIMITER SYNTAX FOR DATA STATEMENTS
	LDA	TOKEN
	CPI	SPECL
	CNZ	ERRD
	LDA	ACCUM
	CPI	','
	RZ
	CPI	';'
	RZ
	CPI	CR
	CNZ	ERRD
	RET
;
EXP16:	;GET 16BIT VALUE TO H,L
	PUSH	B
	CALL	SCAN	;START SCANNING OPERAND FIELD
	CALL	OPAND
	LHLD	EVALUE	;VALUE TO H,L
	POP	B
	RET
;
EXP8:	;GET 8BIT VALUE TO REG A
	CALL	EXP16
	MOV	A,H
	ORA	A
	CNZ	ERRV	;VALUE ERROR IF HIGH BYTE NOT ZERO
	MOV	A,L
	RET
;
EXP3:	;GET 3BIT VALUE TO REG A
	CALL	EXP8
	CPI	8
	CNC	ERRV	;VALUE ERROR IF >=8
	ANI	111B	;REDUCE IF ERROR OCCURS
	RET
;
SHREG:	;GET 3BIT VALUE AND SHIFT LEFT BY 3
	CALL	EXP3
	RAL
	RAL
	RAL
	ANI	111000B
	MOV	C,A	;COPY TO C
	RET
;
SHDREG:	;GET DOUBLE REGISTER TO A
	CALL	SHREG
	ANI	001000B	;CHECK FOR A,C,E, OR L
	CNZ	ERRR	;REGISTER ERROR
	MOV	A,C	;RECOVER REGISTER
	ANI	110000B	;FIX IT IF ERROR OCCURRED
	ORA	B	;MASK OPCODE
	JMP	FILHEX	;EMIT IT
;
SETBYTE:	;EMIT 16BIT OPERAND
	CALL	EXP8
	JMP	FILHEX
;
SETADR:	;EMIT 16BIT OPERAND
	CALL	EXP16
	JMP	FILADR
;
CHCOM:	;CHECK FOR COMMA FOLLOWING EXPRESSION
	PUSH	PSW
	PUSH	B
	LDA	TOKEN
	CPI	SPECL
	JNZ	COMER
;	SPECIAL CHARACTER, CHECK FOR COMMA
	LDA	ACCUM
	CPI	','
	JZ	COMRET	;RETURN IF COMMA FOUND
COMER:	;COMMA ERROR
	MVI	A,'C'
	CALL	PERR
COMRET:
	POP	B
	POP	PSW
	RET
;
CHEND:	;END OF LINE CHECK
	CALL	FILAB	;IN CASE OF A LABEL
	LDA	TOKEN
	CPI	SPECL
	JNZ	STERR	;MUST BE A SPECIAL CHARACTER
	LDA	ACCUM
	CPI	CR	;CARRIAGE RETURN
	JNZ	CHEN0
;	CARRIAGE RETURN FOUND, SCAN PICKS UP LF AND PUSHES LINE
	CALL	SCAN
	JMP	SCNEXT
;
CHEN0:	;NOT CR, CHECK FOR COMMENT
	CPI	';'
	JNZ	CHEN2
	CALL	FILAB	;IN CASE LABELLED EMPTY LINE
;	CLEAR COMMENT TO END OF LINE
CHEN1:	CALL	SCAN
	LDA	TOKEN
	CPI	SPECL
	JNZ	CHEN1
	LDA	ACCUM
	CPI	LF
	JZ	SCNEXT
	CPI	EOF
	JZ	ENDAS	;END OF ASSEMBLY IF EOF
	CPI	'!'
	JZ	SCNEXT	;LOGICAL END OF LINE
	JMP	CHEN1	;NONE OF THE ABOVE
;
;	NOT CR OR LF, MAY BE LOGICAL END OF LINE
CHEN2:	CPI	'!'
	JZ	SCNEXT
	CPI	EOF
	JZ	ENDAS
;
;	STATEMENT ERROR IN OPERAND FIELD
STERR:	MVI	A,'S'
	CALL	PERR
	JMP	CHEN1	;TO DUMP LINE
;
DIFF:	;COMPUTE DE-HL TO HL
	MOV	A,E
	SUB	L
	MOV	L,A
	MOV	A,D
	SBB	H
	MOV	H,A
	RET
;
ENDAS:	;END OF ASSEMBLY FOR THIS PASS
	LXI	H,PASS
	MOV	A,M
	INR	M	;PASS NUMBER INCREMENTED
	ORA	A
	JZ	RESTART
	CALL	SCAN	;TO CLEAR LAST LINE FEED
	CALL	PADD	;WRITE LAST ADDRESS
	LXI	H,PBUFF+5
	MVI	M,CR	;SET TO CR FOR END OF MESSAGE
	LXI	H,PBUFF+1
	CALL	PCON	;PRINT LAST ADDRESS
;
;	COMPUTE REMAINING SPACE
	LHLD	SYTOP
	XCHG
	LHLD	SYBAS
	CALL	DIFF	;DIFFERENCE TO H,L
	PUSH	H	;SYTOP-SYBAS TO STACK
	LHLD	SYMAX
	XCHG
	LHLD	SYBAS
	CALL	DIFF	;SYMAX-SYBAS TO H,L
	MOV	E,H
	MVI	D,0	;DIVIDED BY 256
	POP	H	;SYTOP-SYBAS TO H,L
	CALL	DIVF	;RESULT TO DE
	XCHG
	CALL	PADDR	;PRINT H,L TO PBUFF
	LXI	H,PBUFF+5	;MESSAGE
	LXI	D,EMSG		;END MESSAGE
ENDA0:	LDAX	D
	ORA	A	;ZERO?
	JZ	ENDA1
	MOV	M,A
	INX	H
	INX	D
	JMP	ENDA0
;
EMSG:	DB	'H USE FACTOR',CR,0
;
ENDA1:	LXI	H,PBUFF+2	;BEGINNING OF RATIO
	CALL	PCON
	LHLD	EPC
	SHLD	FPC	;END PROGRAM COUNTER
	JMP	EOR
;
;	UTILITY SUBROUTINES
COMDH:	;COMPARE D,E WITH H,L FOR EQUALITY (NZ FLAG IF NOT EQUAL)
	MOV	A,D
	CMP	H
	RNZ
	MOV	A,E
	CMP	L
	RET
;
SETAS:	;ASPC=FPC
	LHLD	FPC
	SHLD	ASPC
	RET
;
SETLA:	;SYADR=SYLAB, FOLLOWED BY CHECK FOR ZERO
	LHLD	SYLAB
	SHLD	SYADR
	CALL	FOUND
	RET
;
FILAB:	;FILL LABEL VALUE WITH CURRENT ASPC, IF LABEL FOUND
	CALL	SETLA
	RZ		;RETURN IF NO LABEL DETECTED
;
;	LABEL FOUND, MUST BE DEFINED ON PASS-1
	LXI	H,0
	SHLD	SYLAB	;TO MARK NEXT STATEMENT WITH NO LABEL
	LDA	PASS
	ORA	A
	JNZ	FIL1
;
;	PASS 0
	CALL	GETTY
	PUSH	PSW	;SAVE A COPY OF TYPE
	ANI	111B	;CHECK FOR UNDEFINED
	CNZ	ERRL	;LABEL ERROR
	POP	PSW	;RESTORE TYPE
	ORI	PLABT	;SET TO LABEL TYPE
	CALL	SETTY	;SET TYPE FIELD
	LHLD	ASPC	;GET CURRENT PC
	CALL	SETVAL	;PLACE INTO VALUE FIELD
	RET
;
FIL1:	;CHECK FOR DEFINED VALUE
	CALL	GETTY
	ANI	111B
	CZ	ERRP	;PHASE ERROR
;	GET VALUE AND COMPARE WITH ASPC
	CALL	GETVAL	;TO H,L
	XCHG
	LHLD	ASPC
	CALL	COMDH
	CNZ	ERRP	;PHASE ERROR IF NOT THE SAME
	RET
;
FILHEX:	;WRITE HEX BYTE IN REGISTER A TO MACHINE CODE FILE IF PASS-1
	MOV	B,A
FILHB:	LDA	PASS
	ORA	A
	MOV	A,B
	JZ	FILHI
;
;	PASS - 1, WRITE HEX AND PRINT DATA
	PUSH	B	;SAVE A COPY
	CALL	DHEX	;INTO MACHINE CODE FILE
;	MAY BE COMPLETELY EMPTY LINE, SO CHECK ADDRESS
	LDA	PBUFF+1
	CPI	' '
	LHLD	ASPC
	CZ	PADDR	;PRINT ADDRESS FIELD
;
	LDA	NBP
	CPI	NBMAX	;TRUNCATE CODE IF TOO MUCH ON THIS LINE
	POP	B	;RECALL HEX DIGIT
	JNC	FILHI
;	ROOM FOR DIGIT ON THIS LINE
	MOV	A,B
	CALL	WHEXB	;WRITE HEX BYTE TO PRINT LINE
FILHI:	LHLD	FPC
	INX	H
	SHLD	FPC	;READY FOR NEXT BYTE
	RET
;
FILADR:	;EMIT DOUBLE PRECISION VALUE FROM H,L
	PUSH	H	;SAVE A COPY
	MOV	B,L
	CALL	FILHB	;LOW BYTE EMITTED
	POP	H	;RECOVER A COPY OF H,L
	MOV	B,H
	JMP	FILHB	;EMIT HIGH BYTE AND RETURN
;
;	UTILITY FUNCTIONS FOR PRINTING HEX ADDRESSES AND DATA
CHEX:	;CONVERT TO HEX
	ADI	'0'
	CPI	'0'+10
	RC
	ADI	'A'-'0'-10
	RET
;
WHEXN:	;WRITE HEX NIBBLE
	CALL	CHEX	;CONVERT TO ASCII FROM HEX
	LXI	H,NBP
	MOV	E,M	;NEXT POSITION TO PRINT
	MVI	D,0	;DOUBLE PRECISION
	INR	M	;NBP=NBP+1
	LXI	H,PBUFF
	DAD	D
	MOV	M,A	;STORE IN PRINT BUFFER
	RET
;
WHEXB:	;WRITE HEX BYTE TO PRINT BUFFER
	PUSH	PSW
	RAR
	RAR
	RAR
	RAR
	ANI	0FH	;HIGH ORDER NIBBLE NORMALIZE IN A
	CALL	WHEXN	;WRITE IT
	POP	PSW
	ANI	0FH
	JMP	WHEXN	;WRITE AND RETURN
;
PADD:	LHLD	ASPC
PADDR:	;PRINT ADDRESS FIELD OF PRINT LINE FROM H,L
	XCHG
	LXI	H,NBP	;INITIALIZE NEXT TO FILL
	PUSH	H	;SAVE A COPY OF NBP'S ADDRESS
	MVI	M,1
	MOV	A,D	;PRINT HIGH BYTE
	PUSH	D	;SAVE A COPY
	CALL	WHEXB
	POP	D
	MOV	A,E
	CALL	WHEXB
	POP	H	;ADDRESSING NBP
	INR	M	;SKIP A SPACE AFTER ADDRESS FIELD
	RET
;
ERRR:	;EMIT REGISTER ERROR
	PUSH	PSW
	PUSH	B
	MVI	A,'R'
	CALL	PERR
	POP	B
	POP	PSW
	RET
;
ERRV:	;EMIT VALUE ERROR
	PUSH	PSW
	PUSH	H
	MVI	A,'V'
	CALL	PERR
	POP	H
	POP	PSW
	RET
;
ERRD:	PUSH	PSW
	MVI	A,'D'	;DATA ERROR
	JMP	ERR
;
ERRP:	PUSH	PSW
	MVI	A,'P'
	JMP	ERR
;
ERRL:	PUSH	PSW
	MVI	A,'L'	;LABEL ERROR
	JMP	ERR
;
ERRN:	PUSH	PSW
	MVI	A,'N'	;NOT IMPLEMENTED
;
ERR:
	CALL	PERR
	POP	PSW
	RET
;
SYLAB:	DS	2	;ADDRESS OF LINE LABEL
EPC:	DS	2	;END PC VALUE
NBP:	DS	1	;NEXT BYTE POSITION TO WRITE FOR MACHINE CODE
	END
