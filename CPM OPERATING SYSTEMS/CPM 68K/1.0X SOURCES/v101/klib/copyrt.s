	.globl	_sw
_sw:
	rts

*********************************************************
*							*
*	Copyright Notice Module				*
*							*
*********************************************************

	  .globl	___cpyrt
	  .text
___cpyrt: .dc.b	'CP/M-68K(tm), Version 1.1, '
	  .dc.b 'Copyright (c) 1983, Digital Research'
	  .dc.b 'XXXX-0000-654321'
	  .dc.w 0
	  .end
