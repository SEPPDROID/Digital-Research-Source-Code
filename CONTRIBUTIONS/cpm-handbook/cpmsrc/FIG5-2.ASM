; Figure 5-2
;
;	EQUates for BDOS Function Code numbers.
;
B$SYSRESET	EQU	0	;System Reset
B$CONIN		EQU	1	;Read Console Byte
B$CONOUT	EQU	2	;Write Console Byte
B$READIN	EQU	3	;Read "Reader" Byte
B$PUNOUT	EQU	4	;Write "Punch" Byte
B$LISTOUT	EQU	5	;Write Printer Byte
B$DIRCONIO	EQU	6	;Direct Console I/O
B$GETIO		EQU	7	;Get IOBYTE
B$SETIO		EQU	8	;Set IOBYTE
B$PRINTS	EQU	9	;Print String
B$READCONS	EQU	10	;Read Console String
B$CONST		EQU	11	;Read Console Status
B$GETVER	EQU	12	;Get CP/M Version Number
B$DSKRESET	EQU	13	;Disk System Reset
B$SELDSK	EQU	14	;Select Disk
B$OPEN		EQU	15	;Open File
B$CLOSE		EQU	16	;Close File
B$SEARCHF	EQU	17	;Search for First Name Match
B$SEARCHN	EQU	18	;Search for Next Name Match
B$ERASE		EQU	19	;Erase (delete) File
B$READSEQ	EQU	20	;Read Sequential
B$WRITESEQ	EQU	21	;Write Sequential
B$CREATE	EQU	22	;Create File
B$RENAME	EQU	23	;Rename File
B$GETACTDSK	EQU	24	;Get Active (Logged-in) Disks
B$GETCURDSK	EQU	25	;Get Current Default Disk
B$SETDMA	EQU	26	;Set DMA (Read/Write) Address
B$GETALVEC	EQU	27	;Get Allocation Vector Address
B$SETDSKRO	EQU	28	;Set Disk to Read Only
B$GETRODSKS	EQU	29	;Get Read Only Disks
B$SETFAT	EQU	30	;Set File Attributes
B$GETDPB	EQU	31	;Get Disk Parameter Block Address
B$SETGETUN	EQU	32	;Set/Get User Number
B$READRAN	EQU	33	;Read Random
B$WRITERAN	EQU	34	;Write Random
B$GETFSIZ	EQU	35	;Get File Size
B$SETRANREC	EQU	36	;Set Random Record number
B$RESETD	EQU	37	;Reset Drive
B$WRITERANZ	EQU	40	;Write Random with Zero Fill
;
BDOS		EQU	5	;BDOS Entry Point

