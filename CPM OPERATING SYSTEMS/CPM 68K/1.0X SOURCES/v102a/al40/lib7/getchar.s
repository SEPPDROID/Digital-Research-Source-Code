*
* read one char from console 
*
CRTS=$3ff01
CRTD=$3ff03
			.globl	_getchar
			.globl	_putchar
_getchar:
			move.b	CRTS,R0
			and		#1,R0		//receive register full
			beq		_getchar			//not yet
			move.b	CRTD,R0		//read the char
			and.l	#$7f,R0		//leave only 7 bits
			cmp.b	#$0d,R0		//carriage return?
			bne		l0
			move.b	#$0a,R0		//turn into line feed
l0:
			move	R0,-(sp)
			jsr		_putchar	//echo it
			move	(sp)+,R0	//pop arg
			rts
