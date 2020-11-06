*
*	Setjmp / longjmp implementation.  See file "setjmp.h" for details.
*
	.globl	_setjmp			*	Set up longjump
	.globl	_longjmp		*	do longjump
*
_setjmp:				*	Save environment
	movea.l	4(a7),a0		*	a0 -> Environment buffer
	move.l	a7,(a0)+		*	Stack pointer to first long
	move.l	a6,(a0)+		*	Frame pointer to second long
	move.l	(a7),(a0)		*	Return address to third long
	clr.l	d0			*	Return 0 value
	rts				*	Return to caller
*
_longjmp:				*	Return to saved point
	movea.l	4(a7),a0		*	a0 -> Environment buffer
	move.w	8(a7),d0		*	Load return value
	ext.l	d0			*	just in case ...
	movea.l	(a0)+,a7		*	Reset stack
	movea.l	(a0)+,a6		*	Reset frame
	move.l	(a0),(a7)		*	Load TOS with return addr
	rts				*	and return
