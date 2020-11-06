	.globl	_xbarf
_xbarf:
	link	a6,#0		*	C linkage convention
	trap	#15		*	Do regular trap
	lea	1,a0		*	now
	tst.w	(a0)		*		address trap
	unlk	a6		*	try to make
	rts			*		      it back
