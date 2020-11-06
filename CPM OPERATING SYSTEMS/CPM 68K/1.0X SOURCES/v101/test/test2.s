*
*	Part 2 of the test
*
	.globl	print
	.globl	exit

	jsr	print			*	If entered as only module
exit:
	move.l	#0,d0			*	Exit 
	trap	#2			*		to BDOS

print:
	move.l	#9,d0			*	Print string func code
	lea	hello,a0		*	a0 -> string
	move.l	a0,d1			*	load param reg
	trap	#2			*	do BDOS entry
	rts				*	and return

	.data
hello:	.dc.b	'Hello, Steve',$0d,$0a,'$'
