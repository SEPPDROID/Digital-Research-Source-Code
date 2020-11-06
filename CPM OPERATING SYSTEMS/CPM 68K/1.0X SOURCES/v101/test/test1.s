*
*	as68 / lo68 test program -- part 1
*
	.globl	print
	.globl	exit
start:	move.l	#20,d2
loop:	jsr	print		*	String print routine
	subq.l	#1,d2		*	subtract 1
	bne	loop		*	and call again

	jsr	exit		*	call quit routine

