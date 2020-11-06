	.text
*
*	Global Code addresses
*
	.globl	_init
	.globl	_biosinit
	.globl	_flush
	.globl	_wboot
	.globl	_cbios
	.globl	_dskia
	.globl	_dskic
	.globl	_setimask
	.globl	_ccp
	.globl	cpm			*	Lowest addr of CP/M 
	.globl	_end			*	Highest addr of CP/M
*
*	Global data addresses
*
	.globl	_memtab			*	memory region table
	.globl	_dpb3			*	RAM disk dpb address
	.globl	_memdsk			*	-> First memory disk location
*
*	Vector Addresses
*
dskint:	.equ	$3fc			*	UDC Interrupt vector
trap3:	.equ	$8c			*	Trap 3 vector
buserr:	.equ	$8			*	Bus error vector
*
*
_init:	lea	entry,a0
	move.l	a0,trap3
	lea	_dskia,a0
	move.l	a0,dskint
*
*	Auto-Size TPA
*
	lea	_memtab,a0		*	a0 -> Memory region table
	move.w	#1,(a0)+		*	1 region
	move.l	#$400,(a0)+		*	TPA starts at 400
	move.l	#cpm-$408,(a0)+		*	Ends where CP/M begins
*
*	Auto-Size RAM disk
*
	move.l	buserr,-(sp)		*	Push bus err vector
	lea	_end,a0			*	a0 -> Last location in CP/M
	add.l	#cpm,a0			*	Linker doesn't reloc this!!
	move.l	a0,_memdsk		*	-> first location in RAM disk
	move.l	#quit,buserr		*	set up vector -> ourselves
loop:
	tst.w	(a0)+			*	Find
	bra	loop			*		End of memory
quit:
	add.l	#14,a7			*	Clear buserr gorp
	move.l	(a7)+,buserr		*	Pop buserr vector
	sub.l	#_end,a0		*	a0 = # bytes in RAM disk
	sub.l	#cpm,a0			*	Relocation bug
	move.l	a0,d0			*	Into D reg for shift
	move.l	#11,d1			*	Load shift count
	lsr.l	d1,d0			*	Divide by 2048
	move.w	d0,_dpb3+6		*	Load DRM field of dpb

	move	#$2000,sr
	jsr	_biosinit
	clr.l	d0
	rts
*
_wboot:	clr.l	d0
	jmp	_ccp
*
entry:	move.l	d2,-(a7)
	move.l	d1,-(a7)
	move.w	d0,-(a7)
	jsr	_cbios
	add	#10,a7
	rte
*
_dskia:	link	a6,#0
	movem.l	d0-d7/a0-a5,-(a7)
	jsr	_dskic
	movem.l	(a7)+,d0-d7/a0-a5
	unlk	a6
	rte
*
_setimask: move sr,d0
	lsr	#8,d0
	and.l	#7,d0
	move	sr,d1
	ror.w	#8,d1
	and.w	#$fff8,d1
	add.w	4(a7),d1
	ror.w	#8,d1
	move	d1,sr
	rts	
	.end
