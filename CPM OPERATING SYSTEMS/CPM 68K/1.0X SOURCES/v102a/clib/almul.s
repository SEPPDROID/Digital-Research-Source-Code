.text
.globl	almul
.globl	lmul
*
*	/ this routine multiplies together two 32 bit signed long integers.
*	/ the first arg is a pointer to the long, which is multiplied by the
*	/ second arg, with the result being stored at the firstarg.
*	/ lmul.s does the meat of the work
*
almul:
	move.l	A5,-(sp)
	move.l	8(sp),A5
	move.l	12(sp),-(sp)
	move.l	(A5),-(sp)
	jsr		lmul
	add		#8,sp
	move.l	R0,(A5)
	move.l	(sp)+,A5
	rts
