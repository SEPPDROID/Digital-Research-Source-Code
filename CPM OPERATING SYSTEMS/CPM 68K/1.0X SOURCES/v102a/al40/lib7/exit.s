		.globl		_exit
		.text
_exit:	move.l	macsbug,A0
		jmp		(A0)
*
		.data
macsbug: .dc.w	0002
		 .dc.w	0008
		.end
