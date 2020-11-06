*****************************************************************
*                                                               *
*       initiate program execution                              *
*                                                               *
*****************************************************************
*
*
	.globl	_GO
	.globl	_STEP
	.globl	_BDOS
	.globl	_GETSSP

        .text
*
*
_GO:    movem.l	d0-d7/a0-a5,regsave	save register variables
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
	move.l	a7,realssp
*
        move.l  (a0),pctr               pc to start at
        move.w  12(a0),status           starting status
	move.l	4(a0),a1		set up stack pointers
	move.l	a1,usp			user stack pointer
	move.l	8(a0),a7		supervisor stack pointer
        movem.l 14(a0),d0-d7/a0-a6      set up other registers
        move.l  pctr,-(a7)              set up for rte
	and.w	#$7FFF,status
        move.w  status,-(a7)
        rte                             begin executing
*
*	trap handling for _GO and _STEP
*
th:	move.w  (a7)+,status            grab info from sys stack
        move.l  (a7)+,pctr
	move.l	a7,savessp
	move.l	realssp,a7
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
	rts                             back to ddt proper
*
*
*****************************************************************
*								*
*	execute one user instruction (trace or notrace)		*
*								*
*****************************************************************
*
_STEP:	movem.l	d0-d7/a0-a5,regsave	save registers
	movem.l	a6/a7,asave	save registers
*
	move.w	#62,d0		get supervisor mode
	trap	#2
	move.l	a7,realssp
*
	move.w	#22,d0
	move.w	#trv,d1
	move.w	d1,thvnum
	move.l	#th,d2
	trap	#3
	move.l	d0,savev
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
	movem.l	14(a0),d0-d7/a0-a6	registers
	move.l	pctr,-(a7)	set up for rte
	move.w	status,-(a7)
	rte
*
	.bss
*
asave:   .ds.l    2
bsave:   .ds.l    2
regsave: .ds.l	14
realssp: .ds.l	1
savessp: .ds.l	1
saveusp: .ds.l	1
savea0: .ds.l  1
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
	.text
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
        .end
