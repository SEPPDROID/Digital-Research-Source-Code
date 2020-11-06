*
*	This module is designed to allow the use of "printf" without 
*	dragging in the entire floating point RTL.
*
	.globl	nofloat
	.globl	_petoa		*	Called from "_doprt"
	.globl	_pftoa		*	Called from "_doprt"
nofloat:
_petoa:
_pftoa:
	move.l	#9,d0		*	Print string BDOS Call
	move.l	#fpstring,d1	*	d1 -> string to print
	trap	#2		*	Print the error
	clr.l	d0		*	Now
	trap	#2		*		exit

	.data			*	
fpstring:
	.dc.b	'C RTL -- Program not linked for floating point'
	.dc.b	$0d,$0a,'$'
	.even
