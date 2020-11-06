*****************************************************************
*								*
*  Usual filename:  BDOS10.S					*
*  Originally Written by W. B. Tyler, 1982.			*
*  Adapted by:  Timothy M. Benson				*
*  Control:  5 May 83  15:25  (TMB)				*
*  Modified 18 March 1984 for extended exception handling WBT.	*
*  This version works with the 68010, not the 68000.		*
*                                                               *
*       initiate program execution                              *
*                                                               *
*****************************************************************
*
	.globl	_GO
	.globl	_STEP
	.globl	_BDOS
	.globl	_GETSSP
	.globl	_EXINIT
	.globl	_GETVBR
	.globl	_GETSFC
	.globl	_GETDFC

        .text
*
*
_GO:	clr.l	errflag
	movem.l	d0-d7/a0-a5,regsave	save register variables
	movem.l a6/a7,asave
	move.w	#22,d0			set up illegal trap vector
	move.w	#iltv,d1
	move.w	d1,thvnum
	move.l	#th,d2
	trap	#3
	move.l	d0,savev
*
        move.l  4(a7),a0                addr of state save area
*
	move.w	#62,d0			get supervisor mode
	trap	#2
*
        move.l  (a0),pctr               pc to start at
        move.w  12(a0),status           starting status
	move.l	4(a0),a1		set up stack pointers
	move	a1,usp			user stack pointer
	move.l	8(a0),a7		supervisor stack pointer
	move.l	78(a0),d0		set up control registers...
	movec	d0,vbr
	move.l	82(a0),d0
	movec	d0,sfc
	move.l	86(a0),d0
	movec	d0,dfc
        movem.l 14(a0),d0-d7/a0-a6      set up other registers
	clr.w	-(a7)			format word
        move.l  pctr,-(a7)              set up for rte
	and.w	#$7FFF,status
        move.w  status,-(a7)
        rte                             begin executing
*
*	trap handling for _GO and _STEP
*
th1:	move.w  (a7)+,status            grab info from sys stack
        move.l  (a7)+,pctr		(this one for exceptions)
	movem.l	d0-d1,bsave		save registers
	Move.W	#62,D0			Back into supervisor mode
	Trap	#2			By using the BDOS call
	movem.l	bsave,d0-d1		restore registers
	jmp	th2

th:	move.w  (a7)+,status            grab info from sys stack
	Movem.L	D0-D1/A0,bsave		Save some stuff
	Move.L	#OLDVEC,A0		Get address of exception storage
	Move.W	#9,D0			Load exception vector count (-1)
	Move.L	(A7),D1			Get return address from stack
th3:	Cmp.L	(A0)+,D1		Is this the one we have?
	Beq	th4			Yep.
	Dbf	D0,th3			Are we done yet?
	Movem.L	bsave,D0-D1/A0		User exception vector
        move.l  (a7)+,pctr		unstack PC
	add.l	#2,a7			unstack format word
th2:	move.l	a7,savessp
	movem.l	d0-d2,-(a7)
	movec	vbr,d0			save control registers
	movec	sfc,d1
	movec	dfc,d2
	move.l	d0,savevbr
	move.l	d1,savesfc
	move.l	d2,savedfc
	movem.l	(a7)+,d0-d2
	move.l	a0,savea0
	move.l	usp,a0
	move.l	a0,saveusp
	move.l	savea0,a0
 	andi	$5fff,sr
	movem.l a6/a7,bsave             save regs
        move.l	asave+4,a7              get old regs
        move.l  4(a7),a6
        move.l  pctr,(a6)
	move.l	saveusp,4(a6)
	move.l	savessp,8(a6)
        move.w  status,12(a6)
	and.w	#$7FFF,12(a6)
        movem.l d0-d7/a0-a5,14(a6)
	move.l	savevbr,78(a6)
	move.l	savesfc,82(a6)
	move.l	savedfc,86(a6)
	move.w	#22,d0
	move.w	thvnum,d1
	move.l	savev,d2
	trap	#3
        move.l  bsave,70(a6)
        move.l  bsave+4,74(a6)
	btst	#5,status
	beq	wasusr
	move.l	savessp,74(a6)
wasusr: move.l  asave,a6
        movem.l	regsave,d0-d7/a0-a5
	move.l	errflag,d0		return error flag
	rts                             back to SID proper

th4:	Movem.L	bsave,D0-D1/A0		Restore registers (again)
	Move.W	status,-(SP)		Save status register again
THRTE:	Rte				Continue processing as in GO.

*
*
*****************************************************************
*								*
*	execute one user instruction (trace or notrace)		*
*								*
*****************************************************************
*
_STEP:	clr.l	errflag
	movem.l	d0-d7/a0-a5,regsave	save registers
	movem.l	a6/a7,asave	save registers
*
	move.w	#62,d0		get supervisor mode
	trap	#2
*
	move.w	#22,d0
	move.w	#trv,d1
	move.w	d1,thvnum
	move.l	#th,d2
	trap	#3
	move.l	d0,savev	Save previous trace exception pointer.
*
	move.l	asave+4,a0	caller's stack ptr
	move.l	4(a0),a0	address of cpu state save area
	move.l	(a0),pctr	starting pc
	move.w	12(a0),d0	status
	ori.w	#$8000,d0	set trace bit
	move.w	d0,status	starting status
	move.l	4(a0),a1	user stack pointer
	move.l	a1,usp
	move.l	8(a0),a7	system stack pointer
	move.l	78(a0),d0
	movec	d0,vbr
	move.l	82(a0),d0
	movec	d0,sfc
	move.l	86(a0),d0
	movec	d0,dfc
	movem.l	14(a0),d0-d7/a0-a6	registers
	clr.w	-(a7)		format word
	move.l	pctr,-(a7)	set up for rte
	move.w	status,-(a7)
	rte
*
*
*****************************************************************
*								*
*	BDOS Call Subroutine -- C Callable			*
*								*
*****************************************************************
*
_BDOS:	move.w	4(a7),d0
	move.l	d1,saved1
	move.l	6(a7),d1
	trap	#2
	move.l	saved1,d1
	rts
*
	.bss
*
saved1: .ds.l	1
*
	.text
*
*****************************************************************
*								*
*	GETSSP -- supplies system stack pointer to C		*
*								*
*****************************************************************
*
_GETSSP: move.w	#62,d0
	trap	#2
	move.l	a7,d0
	andi	#$5fff,sr
	rts
*
*
*****************************************************************
*								*
*	Routines for getting control register values from C	*
*								*
*****************************************************************
*
*
_GETVBR: move.w	#62,d0
	 trap	#2
	movec	vbr,d0
	andi	#$5fff,sr
	rts
_GETSFC: move.w	#62,d0
	trap	#2
	movec	sfc,d0
	andi	#$5fff,sr
	rts
_GETDFC: move.w	#62,d0
	trap	#2
	movec	dfc,d0
	andi	#$5fff,sr
	rts
*
*
*****************************************************************
*								*
*	Exception Vector Setting				*
*								*
*	This routine sets all the exception vectors handled by	*
*	DDT on behalf of the program being debugged.		*
*								*
*****************************************************************
*
*
_EXINIT:
	Movem.L	D0-D7/A0-A5,-(SP)	* Save all them registers
	Move.L	#EPB,A0			* Initialize Exception Parameter
	Move.W	#2,(A0)			* .. Block. 1st vector is #2,
	Move.L	#IBRVEC,2(A0)		* Place to come to on interrupt
	Move.L	#OLDVEC,A1		* Make pointer to old vectors
EXINI1:	Move.W	#$3D,D0			* Func $3D == Set Exc. Vector
	Move.L	A0,D1			* Get address of block
	Trap	#2

	Move.W	#22,D0			* get BIOS exception vector call
	Move.W	(A0),D1			* Get the vector number
	Move.L	#THRTE,D2		* Temporary exception vector handler
	Movem.L	A0-A1,-(SP)		* Save the address registers
	Trap	#3			* Call up the BIOS
	Movem.L	(SP)+,A0-A1		* Restore the address registers
	Move.L	D0,(A1)+		* Save the old BIOS vector
	Move.L	D0,D2			* restore the old vector in the BIOS
	Move.W	#22,D0			* The call (see above)
	Move.W	(A0),D1			* Das Vectornummer
	Movem.L	A0-A1,-(SP)		* Take a guess
	Trap	#3
	Movem.L	(SP)+,A0-A1

	Add.W	#1,(A0)			* Increase vector num. to change
	Add.L	#6,2(A0)		* And set new exception handler
	Cmp.W	#12,(A0)		* Done yet?
	Bne	EXINI1			* Nope, do it again
	Movem.L	(SP)+,D0-D7/A0-A5	* Restore all those registers
	Rts

EXHANDL:
	move.l	#$ff,errflag		* indicate error to caller
	Move.L	(SP)+,STKVAL		* Save RA to IBRVEC
	Move.L	SP,ESTKSV		* Save old user stack
	Move.L	#EXCSTK,SP		* And get one of our own
	Movem.L	D0-D7/A0-A6,-(SP)	* Plenty big enough to save regs
	Move.L	STKVAL,D1		* Get the IBRVEC RA again
	Sub.L	#IBRVEC+6,D1		* Make it an offset into IBRVEC
	Divu.W	#3,D1			* Get (EXV-2) * 2
	And.L	#$FFFF,D1		* Clear out the remainder
	Lsl.L	#1,D1			* Make it an offset into messages

	Cmp.W	#8,D1			* If Bus Error or Address Error
	Bge	EXHAN1
	Add.L	#8,ESTKSV		* .. Then get rid of garbage

EXHAN1:	Add.L	#EXMSLT,D1		* Now a pointer to message pointer
	Move.L	D1,A0
	Move.L	(A0),D1			* Now a pointer to the message
	Move.W	#9,D0			* Good enough for printing
	Trap	#2
	Move.L	#ATMES,D1		* Print " at "
	Move.W	#9,D0
	Trap	#2
	Move.L	ESTKSV,A0		* Get exception RA
	Move.L	2(A0),-(SP)		* Throw it on the stack
	Jsr	_puthexl		* And print it out
	Move.L	(SP)+,A0		* Clean up the stack
	Move.L	#ENDMES,D1		* Print out the <CR><LF>
	Move.W	#9,D0
	Trap	#2
	Movem.L	(SP)+,D0-D7/A0-A6	* Get old register values
	Move.L	ESTKSV,SP		* And the old stack pointer
	Jmp	th1			* And return to DDT
*
*
	.data
*
*	Message Table contains all the exception handler messages
*
BUSERR:	Dc.B	"Bus Error$"
ADRERR:	Dc.B	"Address Error$"
ILLINS:	Dc.B	"Illegal Instruction$"
ZERDIV:	Dc.B	"Divide by Zero$"
CHKINS:	Dc.B	"'CHK' Exception$"
TPVINS:	Dc.B	"'TRAPV' Exception$"
PRVERR:	Dc.B	"Privilege Violation$"
XTRACE:	Dc.B	"Trace Exception$"
LN10EM:	Dc.B	"Line 1010 Emulator$"
LN15EM:	Dc.B	"Line 1111 Emulator$"

ATMES:	Dc.B	" at $"
ENDMES:	Dc.B	$0D,$0A,"$"

	.even

OLDVEC:	.Ds.L	10			* Old vector addresses
EXMSLT:	.Dc.L	BUSERR,ADRERR,ILLINS	* Message addresses
	.Dc.L	ZERDIV,CHKINS,TPVINS
	.Dc.L	PRVERR,XTRACE,LN10EM
	.Dc.L	LN15EM

	.data
IBRVEC:	Jsr	EXHANDL			* This is where exceptions point
	Jsr	EXHANDL			* Sole purpose: Put their return
	Jsr	EXHANDL			* .. address on the stack so that
	Jsr	EXHANDL			* .. the exception number can be
	Jsr	EXHANDL			* .. calculated
	Jsr	EXHANDL
	Jsr	EXHANDL
	Jsr	EXHANDL
	Jsr	EXHANDL
	Jsr	EXHANDL

	.bss
*
ESTKSV:	.Ds.L	1			* One LONG for the old user stack
STKVAL:	.Ds.L	1			* Value of pointer from IBRVEC
	.Ds.B	256			* Plenty of room for Exception Stack
EXCSTK:

errflag: .ds.l	1
asave:   .ds.l    2
bsave:   .ds.l    3
regsave: .ds.l	14
savessp: .ds.l	1
saveusp: .ds.l	1
savea0: .ds.l  1
savevbr: .ds.l	1
savesfc: .ds.l	1
savedfc: .ds.l	1
iltv     =      $4	illegal instruction vector number
trv	 = 	$9	trace exception vector number
*
savev:	 .ds.l	1
thvnum:	 .ds.w	1
*
setexp	= 61
setsup	= 62
*
pctr:    .ds.l    1
status:  .ds.w    1
*
*

EPB:
vnum:	.ds.w	1
newv:	.ds.l	1
oldv:	.ds.l	1
*
setexv=61
linef=11
buserr=2
*
*
        .end
                                                                                   1
linef=11
buserr=2
*
*
        .end
                                                                                   