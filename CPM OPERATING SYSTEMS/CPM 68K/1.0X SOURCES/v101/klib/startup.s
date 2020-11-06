******************************************************************************
*
*	C runtime startup for CP/M-68k.
*
******************************************************************************
*
ltpa=0			* Low TPA address
htpa=4			* High TPA address
lcode=8			* Code segment start
codelen=12		* Code segment length
ldata=16		* Data segment start
datalen=20		* Data segment length
lbss=24			* Bss  segment start
bsslen=28		* Bss  segment length
freelen=32		* free segment length
resvd=36		* Reserved
fcb2=56			* 2nd parsed fcb
fcb1=92			* 1st parsed fcb
command=128		* Command tail
prtstr=9		* Print string BDOS Call
exit=0			* BDOS exit call
.globl	__main
.globl	_exit
.globl	__break
.globl	__start
.globl  ___cpyrt
		.text
__start:	bra	start1			*	jump around copyright
*
		.dc.b	'CP/M-68K(tm), Version 1.1, Copyright (c) 1983, '
		.dc.b	'Digital Research'	*	copyright notice
		.dc.b	'XXXX-0000-654321'	*	serial number
*
		.even
start1:		move.l	4(a7),a0		*	a0 -> Base page
		move.l	lbss(a0),a1		*	a1 -> bss region
		move.l	a1,a3			*	Save this
		adda.l	bsslen(a0),a1		*	a1 -> 1st heap loc
xclear:						*	Clear heap area
		clr.w	(a3)+			*	clear a word
		cmpa.l	a3,sp			*	See if done
		bhi	xclear			*	Not yet, continue
		move.l	a1,__break		*	Put in "break" loc
		lea.l	command(a0),a2		*	a2 -> command line
		move.b	(a2)+,d0		*	d0 = byte count
		andi.l	#$ff,d0			*	clear junk
		move.w	d0,-(a7)		*	push length
		move.l	a2,-(a7)		*	Push commnd
		clr.l	a6			*	Clear frame pointer
		jsr	__main			*	call main routine
		jsr	_exit			*	call "exit"
*
*
		.bss
__break:	.ds.l	1			*	Break function
*
*
.globl	_brk
		.text
_brk:		link	a6,#0			*	preserve conventions
		clr.l	d0			*	clear return
		movea.l	8(sp),a0		*	New break?
		adda.l	#$100,a0		*	Chicken factor
		cmpa.l	a0,sp			*	Compare
		bhis	brkok			*	OK, continue
		move.l	#-1,d0			*	Load return reg
brkok:
		unlk	a6			*	Unlink
		rts				*	return
		
.globl	___BDOS
___BDOS:	link	a6,#0			*	link
		move.w	8(sp),d0		*	Load func code
		move.l	10(sp),d1		*	Load Paramter
		trap	#2			*	Enter BDOS
		cmpa.l	__break,sp		*	Check for stack ovf
		bhis	noovf			*	NO overflow, continue
		move.w	#prtstr,d0		*	String print
		lea	ovf,a0			*	a0-> message
		move.l	a0,d1			*	load proper reg
		trap	#2			*	Issue message
		move.w	#exit,d0		*	Exit
		trap	#2			*		now
noovf:						*	Here if all OK
		unlk	a6			*
		rts				*	Back to caller
*
*	Data area
*
		.data
		.globl	___pname		*	Program Name
		.globl	___tname		*	Terminal Name
		.globl	___lname		*	List device name
		.globl	___xeof			*	^Z byte
ovf:		.dc.b	'Stack Overflow$'	*	Error message
___pname:	.dc.b	'C runtime',0		*	Program name
___tname:	.dc.b	'CON:',0		*	Console name
___lname:	.dc.b	'LST:',0		*	List device name
___xeof:	.dc.b	$1a			*	Control-Z
