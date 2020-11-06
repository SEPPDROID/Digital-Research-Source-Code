* write one char to the crt
*  call with:  ascii char to write in D0
status1:	.equ		$3ff01	//really $3ff01
port1:		.equ		$3ff03	//really $3ff03

			.globl	_putchar
			.text
_putchar:
			move.l	(sp)+,A1	//return address
			move	(sp),R0		//char to send
			cmp.b	#9,R0		//tab?
			bne		l11
			move.b	#' ',R0		//make it space
l11:
			move	#1,R1
			move.l	#status1,A0
loop1:		btst	R1,(A0)	//test transmitter empty
			beq		loop1		//not empty
			move.b	R0,2(A0)	//send char
			cmp.b	#$0a,R0		//line feed?
			bne		l0
			move.b	#$0d,R0		//carriage return
			bra		loop1
l0:
			jmp		(A1)	//return
			.end
