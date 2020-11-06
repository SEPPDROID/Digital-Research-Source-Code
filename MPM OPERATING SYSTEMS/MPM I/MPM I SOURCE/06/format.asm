;4-15-77;DISK FORMAT PROGRAM TO WORK WITH THE FDC-3 DISK CONTROLLER;FORMATS DISKETTES TO WORK WITH VERSION 4.1 OF THE;CP/M OPERATING SYSTEM.  EITHER SINGLE OR DOUBLE DENSITY CAN;BE FORMATTED.  IF DOUBLE DENSITY IS SPECIFIED, TRACK 0 IS ;IN SINGLE DENSITY AND THE REST ARE DOUBLE.;;SINCE THE FDC-3 IS USED WITH BOTH S100 AND ;DSC-2 COMPUTER SYSTEMS, THE ASSEMBLY TIME SWITCH DSC2;MUST BE SET CORRECTLYTRUE	EQU	0FFFFHFALSE	EQU	NOT TRUE;DSC2	EQU	TRUE	;THIS SHOULD BE SET TO FALSE FOR S100 SYSTEMS;;DEFINE DISK PORTS;	IF DSC2I127	EQU	80HI126	EQU	81HI125	EQU	82HI124	EQU	83H	ENDIF;	IF NOT DSC2I127	EQU	7FHI126	EQU	7EHI125	EQU	7DHI124	EQU	7CH	ENDIF;;PORT	EQU	4AHDENSITY	EQU	4CH;	ORG 100H	LXI	SP,2000HMONITOR	EQU	0005H;FIGURE WHERE THE CBIOS JUMP TABLE IS FOR THIS SYSTEM	LHLD	1	MVI	L,0	SHLD	JUMPTAB;;INITIALIZE DISKETTES;;SET UP DATA BUFFER OF E5	LXI	H,WRITEBUF	MVI	B,128DL0:	MVI	M,0E5H	INX	H	DCR	B	JNZ	L0;CHANGE ERROR LOCATION IN CBIOS TO THIS PROGRAM	LHLD	JUMPTAB	LXI	D,2EH	DAD	D	LXI	D,ERR	MOV	A,M	;SAVE OLD ERROR LOCATION	STA	OLDL	MOV	M,E	INX	H	MOV	A,M	STA	OLDH	MOV	M,D;SAVE OLD STATE OF DISK SELECT AND DENSITY	LDA	PORT	STA	SAVEPORT	LDA	DENSITY	STA	SAVEDENS	MVI	C,9	;PRINT LOGON MESSAGE	LXI	D,MESS0	CALL	MONITORAGAIN:;PRINT MESSAGE TO SELECT INITIALIZE ROM	MVI	C,9D	;MONITOR CALL TO PRINT BUFFER	LXI	D,MESS1	CALL	MONITOR;WAIT FOR ANSWER	MVI	C,01	CALL	MONITOR;CHECK FOR NO	CPI	'N'	JZ	DONE	CPI	06EH	JZ	DONE;NOW SELECT THE CORRECT DIRVE     8 ARE POSSIBLE	DCR	A	ANI	07H	MOV	C,A;PUT INTO MESSAGE	ORI	40H	INR	A	STA	DRV	CALL	SELDISKOK:;START WITH TRACK 0	XRA	A	STA	TRK	MVI	C,9D	;PRINT BUFFER CALL	LXI	D,MESS3	;PROMPT TO SELECT DENSITY	CALL	MONITOR;WAIT FOR RESPONSE	MVI	C,01	CALL	MONITOR	CPI	'S'	JZ	FILL0	CPI	'D'	JNZ	OK	;DO NOT ASSUME ANYTHINGFILL0:;SET UP SINGLE DENSITY	STA	DENS;SET UP DENSITY SELECTS IN LOW MEMORY 	XRA	A	STA	DENSITY		;DENSITY	LDA	PORT	ANI	0F7H	STA	PORT;SET UP PROPER ADDRESS MARKS FOR INITIALIZING	MVI	A,0FEH	STA	ID	MVI	A,0FCH	STA	INDEXAM	MVI	A,0C0H	STA	FINISH	;TRACK FINISHED	MVI	A,0E0H	STA	ECODE;SET UP CORRECT TABLE FOR USE IN WRITING THE E5 DATA PATTERN	LXI	H,SINTAB	SHLD	E5TABLE;SET UP CORRECT TABLE FOR WRITING THE ID FIELDS	LXI	B,SINTAB	MVI	A,26D	CALL	FILL	JMP	OK8FILL:	STA	SECTORS;NOW MAKE A IMAGE OF THE TRACK	LXI	H,BUFFER	PUSH	B	MOV	C,A	;NUMBER OF SECTORSLOOP:	MVI	B,6	;6 BYTES PER SECTOR	LXI	D,IDLOOP1:	LDAX	D	MOV	M,A	INX	H	INX	D	DCR	B	JNZ	LOOP1	DCR	C	JNZ	LOOP;NOW BUFFER +3 AND EVERY 6 BYTES ARE SECTOR #;FILL IN SECTOR NUMBERS	POP	B	;ADDRESS OF TABLE OF SECTOR NUMBERS	CALL	FILSECT	JMP	FINCODEFILSECT:	LXI	H,BUFFER+3	LXI	D,6LOOP3:	LDAX	B	CPI	0FFH	RZ	MOV	M,A	INX	B	DAD	D	JMP	LOOP3;NOW PUT FINNISH CODE AT THE LAST WORDFINCODE:	DCX	H	DCX	H	DCX	H	DCX	H	LDA	FINISH	MOV	M,A	RETOK8:;PRINT MESSAGE TO PUT IN CORRECT DISKETTE	LXI	D,MESS2	MVI	C,9	CALL	MONITOR	MVI	C,1	;WAIT FOR REPLY	CALL	MONITOR;ENABLE FORMATTING	LDA	PORT	ANI	0BFH	XRI	40H	STA	PORT;HOME THE DISK	LHLD	JUMPTAB	LXI	D,18H	DAD	D	CALL	SYST;NOW GO AHEAD AND INITIALIZE;INIT:;SET UP TRACK EROR COUNTER	XRA 	A	STA	ERRORS;;INITIAL:;DELAY 200 MILLISEC	LXI	B,8000HL6:	DCR	C	JNZ	L6	DCR	B	JNZ	L6;HERE WE GO - TURN MICROCODE ON;	LXI	H,INDEXAM	MOV	A,H;	IF NOT DSC2	CMA	ENDIF;	OUT	I126	;SET DMA ADDRESS OM FCD	MOV	A,L;	IF NOT DSC2	CMA	ENDIF;	OUT	I125	;POINTS TO INDEX AM	MVI	A,0A0H	;SELECT INITIALIZATION CODE AND WRITE DATA;	IF NOT DSC2	CMA	ENDIF;	OUT	I127WW:	IN	I127	ANI	88H	;WAIT FOR IO FINISH	JZ	WW	IN	I127	;CHECK FOR ERROR	ANI	0F0H	JZ	FILTRK	LXI	D,FORMATERR	JMP	ERR1;;INCREMENT TRACK, STEP;FILTRK:;NOW WRITE THA DATA PATTERN OF E5 IN EACH SECTOR	LXI	B,WRITEBUF	CALL	SETDMA;THIS IS TABLE DRIVEN TO SPEED IT UP;USE SINTAB FOR SD TRACKS;USE TABLE4 FOR DD TRACKS	LHLD	E5TABLEF6:	PUSH	H	MOV	C,M	CALL	SETSEC	CALL	WRITE	POP	H	INX	H	MVI	A,0FFH	CMP	M	JNZ	F6	;MORE SECTORS TO GONEXTRK:	LDA	TRK	INR	A	STA	TRK	CPI	1	JZ	CHECKDENS	CPI	2	JZ	NEWSECT	CPI	77	JNZ	DOSTP	JMP	AGAIN	;DO ANOTHERCHECKDENS:	LDA	DENS	CPI	'D'	JNZ	DOSTP;TIME TO CHANGE DENSTIY	LDA	PORT	ANI	0F7H	ORI	08H	STA	PORT	MVI	A,0FFH	STA	DENSITY	MVI	A,0EH	STA	ID	;ID ADDRESS MARK	MVI	A,0CH	STA	INDEXAM	XRA	A	STA	FINISH	INR	A	STA	ECODE	;SECTOOR FINISH CODE	LXI	H,TABLE4	SHLD	E5TABLE	LXI	B,DOBTAB	MVI	A,58D	;NUMBER OF SECTORS	CALL	FILL	JMP	DOSTPNEWSECT:;FOR OPTIMAL CPPM ACCESS, THE CPM TRACKS MUST BE INITIALIZED;DIFFERENTLY	LDA	SECTORS	CPI	26D	JZ	DOSTP	;ONLY NECESSARY FOR DD;FOR DD THE LOGIACAL SECTORS ARE EVERY 13TH PHYSICAL SECTOR	LXI	B,TABLE2	CALL	FILSECTDOSTP:;FIGURE ADDRESS OF SEEK	LHLD	JUMPTAB	LXI	D,01EH	DAD	D	LDA	TRK	MOV	C,A	;SEND PARAMETER IN C	CALL	SYST;FILL IN NEW TRACK BYTES	LXI	H,BUFFER+1	LXI	D,006	LDA	SECTORS	MOV	C,A	LDA	TRKLOOP5:	MOV	M,A	DAD	D	DCR	C	JNZ	LOOP5	JMP	INITDONE:	LXI	D,MESS4	MVI	C,09	CALL	MONITOR	MVI	C,01	CALL	MONITOR	JMP	REBOOTSELDISK:	LXI	D,01BH	LHLD	JUMPTAB	DAD	D	JMP	SYSTSETDMA:	LXI	D,24H	LHLD JUMPTAB	DAD	D	JMP	SYSTSETSEC:	LXI	D,21H	LHLD	JUMPTAB	DAD	D	JMP	SYSTWRITE:	LXI	D,2AH	LHLD	JUMPTAB	DAD	D	JMP	SYSTERR:	POP	H	;GET RID OF STACK RETURN	LXI	D,WRITERRERR1:	MVI	C,9	CALL	MONITOR	LDA	ERRORS	INR	A	CPI	10	STA	ERRORS	JNZ	INITIAL	LXI	D,GIVEUP	MVI	C,9	CALL	MONITORREBOOT:;RESTORE CBIOS JUMP TO ERROR RECOVERY	LHLD	JUMPTAB	LXI	D,2EH	DAD	D	LDA	OLDL	MOV	M,A	INX	H	LDA	OLDH	MOV	M,AWARMBT:; FIND OLD PORT INFO AND DISABLE FORMATTING	LDA	SAVEPORT	STA	PORT;RESTORE DENSITY 	LDA	SAVEDENS	STA	DENSITY	JMP	0000SYST:	PCHL	;GO TO CBIOSOLDL:	DB	0OLDH:	DB	0DENS:	DB	0SAVEPORT: DB	0SAVEDENS: DB	0JUMPTAB:	DW	0000E5TABLE:	DW	0000ERRORS:	DB	0SECTORS:	DB	0ID:	DB	0TRK:	DB	0	DB	0SECT:	DB	0	DB	0ECODE:	DB	01FINISH:	DB	0MESS0:	DW	0D0AH	IF DSC2	DB	'DISKETTE FORMAT PROGRAM VERSION DSC4.1$'	ENDIF;	IF NOT DSC2	DB	'DISKETTE FORMAT PROGRAM VERSION 4.1$'	ENDIF;MESS1:	DW	0D0AH	DB	'WHICH DRIVE DO YOU WANT TO FORMAT? $'MESS2:	DW	0D0AH	DB	'PUT DISKETTE TO BE INITIALIZED IN DRIVE  'DRV:	DB	0	DB	'$'MESS3:	DW	0D0AH	DB	'SINGLE OR DOUBLE DENSITY $'MESS4:	DW	0D0AH	DB	'REBOOTING $'FORMATERR:DW 0D0AH	DB	'HAVING TROUBLE FORMATING $'WRITERR:DW	0D0AH	DB	'HAVING TROUBLE WRITING $'GIVEUP: DW	0D0AH	DB	'CAN NOT DO IT $'SINTAB:	DB	1,2,3,4	DB	5,6,7,8	DB	9,10,11,12	DB	13,14,15,16	DB	17,18,19,20	DB	21,22,23,24	DB	25,26	DB	0FFHDOBTAB:	DB	1,30,2,31	DB	3,32,4,33	DB	5,34,6,35	DB	7,36,8,37	DB	9,38,10,39	DB	11,40,12,41	DB	13,42,14,43	DB	15,44,16,45	DB	17,46,18,47	DB	19,48,20,49	DB	21,50,22,51	DB	23,52,24,53	DB	25,54,26,55	DB	27,56,28,57	DB	29,58	DB	0FFHTABLE2: DB	1,10,19,28	DB	37,46,55,6	DB	15,24,33,42	DB	51,2,11,20	DB	29,38,47,56	DB	7,16,25,34	DB	43,52,3,12	DB	21,30,39,48	DB	57,8,17,26	DB	35,44,53,4	DB	13,22,31,40	DB	49,58,9,18	DB	27,36,45,54	DB	5,14,23,32	DB	41,50	DB	0FFHTABLE4: DB	1,19,37,55	DB	15,33,51,11	DB	29,47,7,25	DB	43,3,21,39	DB	57,17,35,53	DB	13,31,49,9	DB	27,45,5,23	DB	41,10,28,46	DB	6,24,42,2	DB	20,38,56,16	DB	34,52,12,30	DB	48,8,26,44	DB	4,22,40,58	DB	18,36,54,14	DB	32,50	DB	0FFHWRITEBUF: DS 128DINDEXAM:	DB	0BUFFER:	DS	360D	END