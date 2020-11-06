
*****************************************************************
*								*
*	CP/M-68K Interface Module for the Loader BDOS		*
*		For "C" version of CP/M-68K    			*
*								*
*	Copyright (c) 1982 Digital Research, Inc.		*
*								*
*	Version 0.1 -- September  22, 1982			*
*								*
*****************************************************************

*  Declare Public Routines

	.globl	_bios1		* 5 BIOS entry points from BDOS
	.globl	_bios2
	.globl	_bios3
	.globl	_bios4
	.globl	_bios5
	.globl	_swap		* byte swapper
	.globl	_udiv		* unsigned divide routine

*  External Routine

	.globl	_bios

*
* BIOS Interface Routines
*
*
*  Note - there are 5 BIOS entry points from the BDOS, labelled BIOS1 -
*    BIOS5, depending on the parameters passed.

_bios5:
* For BIOS functions sectran and set exception vector
* Has function number and 2 parameters, a word followed by a long word
	move.l	8(sp),d2	* get 2nd parameter (long word)
	bra	_bios2		* join common routine

_bios4:
* For BIOS function seldsk
* Has function number followed by 2 word parameters
	move.w	8(sp),d2	* get 2nd parameter (word)
	bra	_bios2		* join common routine

_bios3:
* For BIOS function set dma
* Has function number followed by 1 long parameter
	move.l	6(sp),d1	* get long word parameter
	bra	_bios1		* join common routine

_bios2:
* For all BIOS functions with a word parameter
* Word parameter follows function number
	move.w	6(sp),d1	* get 1st parameter (word)

_bios1:
* For all BIOS functions that have no parameter other than function number
	move.w	4(sp),d0	* get function number
	movem.l	d3-d7/a3-a6,-(sp)
	jsr	_bios		* do BIOS call
	movem.l	(sp)+,d3-d7/a3-a6
*				* returns value in d0
	rts

*
*  Utility Subroutines
*

_swap:
* Swap bytes of a word, return swapped value in d0			
	move.b	5(sp),d0
	lsl	#8,d0
	move.b	4(sp),d0
	rts

_udiv:
* Unsigned divide.
* Inputs: Long dividend, unsigned word divisor, address of return structure
* Return: In structure passed, unsigned word quotient, unsigned word remainder
	move.l	a0, -(sp)	* save a0
	move.l	14(sp), a0	* get address of return structure
	move.l	8(sp), d0	* get dividend
	divu	12(sp), d0	* do the divide
	move.w	d0, (a0)+	* store quotient
	swap	d0
	move.w	d0, (a0)	* store remainder
	move.l	(sp)+, a0	* restore a0
	rts

	.end
