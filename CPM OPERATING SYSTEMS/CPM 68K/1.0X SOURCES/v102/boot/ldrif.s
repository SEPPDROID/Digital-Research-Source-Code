
*****************************************
*  Assembly Language Interface Routine  *
*    for the CP/M-68K loader, cpmldr	*
*					*
*  It has one entry point:  _startld	*
*  It sets up the stack and jumps to	*
*	_ldcpm				*
*****************************************

	.globl	_startld	* public entry point
	.globl	ldr		* label for library search

	.globl	_bios		* external references
	.globl	_ldcpm
	.globl	_bios1		* library search
	.globl	_dirscan	*    "      "
	.globl	_seqread	*    "      "
	.text

ldr:
_startld:
	lea	stack,sp
	clr.l	d0
	jsr	_bios		* init the BIOS
	move	#22,d0
	move	#8,d1
	move.l	#privexc,d2
	jsr	_bios		* set exception for privilege violation
	ori	#$2000,sr	* set supervisor state
*				  if in user mode, will generate priv violation
	lea	stack,sp
	bra	_ldcpm


privexc:
	ori.w	#$2000,(sp)
	rte


	.bss

	.ds.l	256
stack:	.ds.w	1

	.end
