	.text
	.globl	_init
	.globl	_ccp
*
*	Init Routine -- User fills in jump address
*
_init:	jmp	0
*
*	This jump to the CCP may be used by the user to achieve a warmboot
*
	jmp	_ccp
*
*
	.end

