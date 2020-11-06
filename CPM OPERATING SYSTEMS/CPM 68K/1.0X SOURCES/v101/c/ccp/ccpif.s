*************************************************************************
*									*
*                    CPM68K INTERFACE MODULE FOR			*
*		     THE CONSOLE COMMAND PROCESSOR			*
*									*
*	    THIS IS THE DUAL-PROCESSOR,ROMABLE CP/M-68K SYSTEM		*
*	    ==================================================		*
*									*
*       (C) Copyright Digital Research 1983 all rights reserved		*
*									*
*************************************************************************


	.globl  _bios1
	.globl  _bdos
	.globl  _load68k
	.globl  _load_tbl
	.globl	init_tbl
	.globl	_load_try
	.globl  _autorom
	.globl	flags
	.globl	TPAB
	.globl  stack
	.globl	_bdosini
	.globl  _main
	.globl  _submit
	.globl  _morecmds
	.globl  _autost
	.globl  _usercmd
	.globl	_init
	.globl	_ccp
	.globl  _patch
	.globl  cpm

	.text
cpm:	
	jmp.l	ccpstart	* start ccp with possible initial command
	jmp.l   ccpclear	* clear auto start flag



	.bss
_autost:  .ds.b  1		* autostart command flag
_usercmd: .ds.b  130		* user command buffer

	.text
copy:	 .dc.b  'COPYRIGHT (C) 1982, Digital Research '


	.text
ccpclear:
	clr.b	_autost		* clear the autostart flag

ccpstart:
	lea	stack,sp	* set up the stack pointer
	clr.b   _autost		* clear the auto start flag
	jsr	_init		* call bios init
	move.w	d0,dskuser	* save user # & disk
*
*
*	ROM SYSTEM INITIALIZATION
*	OF BSS VARIABLES
*
*

	clr.b	_load_try	
	clr.b	_submit
	clr.b	_morecmds
	move.b  #$1,_autorom
	clr.w	flags
	clr.w	TPAB
	jsr	init_tbl



	jsr	_bdosini	* do bdos init
	move.w  #32,d0		* get user bdos func #
	clr.l	d1		* clear out d1
	move.b  dskuser,d1	* get the user #
	trap	#2		* set the user number
	clr.l	d0		* clear d0
	move.w  #14,d0		* select function
	clr.l	d1		* clear d1
	move.w	dskuser,d1	* get disk to be selected
	andi    #$0ff,d1	* mask off the user #
	trap	#2		* select the disk

_ccp:
	lea	stack,sp	* set up the stack pointer
	jsr	_main		* call the CCP
	bra	_ccp

	.bss
	.even

dskuser: .ds.w  1
	.even
_submit: .ds.b  1
	.even
_morecmds: .ds.b 1
	.even
_patch	.ds.l	25
	.end










