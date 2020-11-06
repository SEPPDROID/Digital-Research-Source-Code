*************************************************************************
*									*
*			INTERFACE MODULE BETWEEN			*
*			    CCP and THE BDOS				*
*									*
*									*
*	    THIS IS THE DUAL PROCESSOR,ROMABLE CP/M-68K SYSTEM		*
*	    ==================================================		*
*									*
*       (C) Copyright Digital Research 1983 all rights reserved    	*
*									*
*************************************************************************

.globl  _bdos
_bdos:	        move.w 4(sp),d0
		move.l 6(sp),d1
		trap #2
		rts
