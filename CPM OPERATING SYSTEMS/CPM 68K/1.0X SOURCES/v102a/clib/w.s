******************************************************************************
*
*	C runtime startup for CP/M-68k.  (Wild card version)
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
.globl	__wmain
.globl	__exit
.globl	__break
.globl	__start
.globl	___cpmrv
.globl	__base
.globl  _sw_
.globl	__sovf
		.text
__start:	clr.l	d5			* clear a register
		jsr	_sw_			* security reasons
		bra	start			*	Branch around ID
		.dc.b 'C Runtime V01.11W, Copyright(c) 1983, Digital Research '
serial:		.dc.b	'XXXX-0000-654321'	*	serial number
		.even
start:		move.l	4(a7),a0		*	a0 -> Base page
		move.l	a0,__base		*	Load C external
		move.l	lbss(a0),a1		*	a1 -> bss region
		move.l	a1,a3			*	Save this
		adda.l	bsslen(a0),a1		*	a1 -> 1st heap loc
		move.l	a1,__break		*	Put in "break" loc
		lea.l	command(a0),a2		*	a2 -> command line
		move.b	(a2)+,d0		*	d0 = byte count
		andi.l	#$ff,d0			*	clear junk
		move.w	d0,-(a7)		*	push length
		move.l	a2,-(a7)		*	Push commnd
		clr.l	a6			*	Clear frame pointer
		jsr	__wmain			*	call main routine
		jmp	__exit			*	call "exit"
*
*
		.bss
__base:		.ds.l	1			*	-> Base Page
__break:	.ds.l	1			*	Break function
___cpmrv:	.ds.w	1			*	Last CP/M return val
*
*
.globl	_brk
		.text
_brk:		
		movea.l	4(sp),a0		*	New break?
		move.l 	a0,d0
		lea	$100(a0),a0		*	Chicken factor
		cmpa.l	a0,sp			*	Compare
		bhis	brkok			*	OK, continue
		move.l	#-1,d0			*	Load return reg
		rts				*	Return
brkok:
		move.l	d0,__break		*	Save the break
		clr.l	d0			*	Set OK return
		rts				*	return
		
.globl	___BDOS
___BDOS:	link	a6,#0			*	link
		move.w	8(sp),d0		*	Load func code
		move.l	10(sp),d1		*	Load Paramter
		trap	#2			*	Enter BDOS
		cmpa.l	__break,sp		*	Check for stack ovf
		bhis	noovf			*	NO overflow, continue
__sovf:		move.w	#prtstr,d0		*	String print
		lea	ovf,a0			*	a0-> message
		move.l	a0,d1			*	load proper reg
		trap	#2			*	Issue message
__exit:		move.w	#exit,d0		*	Exit
		trap	#2			*		now
noovf:						*	Here if all OK
		unlk	a6			*
		rts				*	Back to caller
	.page
*
*	Fill function:
*
*	fill(dest,char,cnt);
*
*	BYTE	*dest;		/* -> area to be filled	*/
*	BYTE	char;		/* =  char to fill	*/
*	WORD	cnt;		/* =  # bytes to fill   */
*
	.globl	_fill
_fill:
	move.l	4(a7),a0			*	-> Output area
	move.w	8(a7),d1			*	=  output char
	move.w	10(a7),d0			*	=  output count
	ext.l	d0				*	make it long
	subq.l	#1,d0				*	decrement
	ble	filldone			*	Done if le
fillit:	move.b	d1,(a0)+			*	move a byte
	dbra	d0,fillit			*	Continue
filldone: clr.l	d0				*	always return 0
	rts					*

*
*	Index function to find out if a particular character is in a string.
*
	.globl	__index
__index:
	move.l	4(a7),a0			*	a0 -> String
	move.w	8(a7),d0			*	D0 = desired character
xindex:	tst.b	(a0)				*	EOS?
	bne	notend				*	No, continue to look
	clr.l	d0				*	Not found
	rts					*	Quit
notend:	cmp.b	(a0)+,d0			*	check for character
	bne	xindex				*	
	move.l	a0,d0				*	Found it
	subq.l	#1,d0				*	set return pointer
	rts					*
	.page
*
*	_salloc function.
*
*	This function allocates a data area on the stack.  
*
*	Calling Sequence:
*
*	adr = _salloc(size);
*
*	Returns the address of the area allocated.
*
	.globl	__salloc
__salloc:
	move.l	(sp)+,a0			*	Save return address
	clr.l	d0				*	Zap d0 high word
	move.w	(sp)+,d0			*	Get arg (word size)
	addq.l	#1+4,d0				*	Round 
	bclr.l	#0,d0				*		up to word
	move.l	__break,a1			*	a1 -> break area
	lea	$100(a1),a1			*	Add chicken factor
	lea	0(a1,d0.l),a1			*	And size
	cmpa.l	a1,sp				*	OK?
	blo	__sovf				*	Overflow, die
	sub.l	d0,sp				*	allocate
	lea	4(sp),a1			*	a1 -> area start
	move.l	a1,d0				*	set return code
	jmp	(a0)				*	and "return"
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
		.end

