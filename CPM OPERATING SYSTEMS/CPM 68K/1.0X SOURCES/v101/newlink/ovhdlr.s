*	BDOS Function Definitions
*
reboot	   =	0
printstr   =	9
open	   =   15
setdma	   =   26
pgmldf     =   59
gettpa     =   63
*
.globl	_ovhdlr
.xdef	_loadr
	.text
*
_ovhdlr:move	sr,savecc		* save condition codes
	movem.l	d1-d7/a0-a6,saver	* save all registers
	movea.l	(a7)+,a6		* pop pointer to inline parameter
	move.l	#4,d6			* adjust return address
	add.l	a6,d6			*  to skip over table pointer
	move.l	d6,-(a7)		* push return address
*
*	GET FILE NAME AND LOAD PT
*
	movea.l	(a6),a6			* get ovl table address
	move.l	a6,d5			* check address with last loaded
	cmp.l	oldadd,d5		*   if it's the same,
	beq	done			*   file is already loaded.
	move.l	a6,oldadd		* save address for next time
	lea.l	fname,a5		* get address of name in fcb
	move.l	#11,d5			* set up loop counter (l2 cycles)
gfnm:	move.b	(a6)+,(a5)+
	dbf	d5,gfnm
	move.l	(a6),lowadr		* store load point address
*
*	OPEN OVERLAY FILE
*
	move.l	#fcb,d1
	movea.l	d1,a1
	move.w  #open,d0	*put BDOS function number in register d0
	trap	#2		*try to open the file to be loaded
	cmpi	#255,d0		*test d0 for BDOS error return code
	beq	openerr		*if d0 = 255 then goto openerr
	move.b	#0,cr		*zero record number in fcb
*
*	FILL THE LPB
*
	lea	ovbspg,a3	* get address of input buffer 
	move.l	a3,baspag	* put it in LPB
	move.l	a1,LPB		*put address of open FCB into LPB
	move.l	#LPB,a0		*put address of LPB into register a0
	jsr	_loadr		*load the module
	tst	d0		*was the load successful?
	bne	lderr		*if not then print error message
*
*	RESTORE AND RETURN
*
done:	movem.l	saver,d1-d7/a0-a6	* restore all registers
	move	savecc,ccr		* restore condition codes
	rts
*
*	PRINT ERROR MESSAGE
*
openerr:
	move.l	#openmsg,d1	*get address of error message 
*                               *to be printed
	bra	print
lderr:	move.l  #loaderr,d1	*get address of error message to 
*				*be printed
print:	move.w	#printstr,d0	*get BDOS function number
	trap	#2              *print the message
	move.b	#36,ex		*mark end of filename
	move.l	#fname,d1	*get address of filename
	move.w	#printstr,d0	*set up for BDOS call
	trap	#2		*print the filename
cmdrtn: move.w	#reboot,d0	*get BDOS function number
	trap	#2              *warmboot and return to the CCP

*
*	DATA
*
	.data
*
*	ERROR MESSAGE STRINGS
*
	.even
loaderr:	.dc.b  13,10,'Error Loading Overlay File $'
openmsg:	.dc.b  13,10,'Unable to Open Overlay File $'
*
*	BSS
*
	.bss
	.even
oldadd: .ds.l	1		* table for last loaded overlay
savecc:	.ds.w	1
saver:	.ds.l	14
ovbspg:	.ds.w	64		* input buffer (like basepage) for loadr
*
*	FILE CONTROL BLOCK
*
	.even
fcb:	.ds.b	1		* file control block
fname:	.ds.b	11
ex:	.ds.b	1
sysjnk: .ds.b	19
cr:	.ds.b	1
*
*	LOAD PARAMETER BLOCK
*
	.even
LPB:	   .ds.l	1      *FCB address of program file
lowadr:    .ds.l	1      *Low boundary of area in which 
*                              *to load program
hiadr:	   .ds.l	1      *High boundary of area in which to 
*                              *to load program
baspag:	   .ds.l	1      *Base page address of loaded program
usrstk:	   .ds.l	1      *Loaded program's initial stack pointer
flags:	   .ds.w	1      *Load program function control flags
	.end
