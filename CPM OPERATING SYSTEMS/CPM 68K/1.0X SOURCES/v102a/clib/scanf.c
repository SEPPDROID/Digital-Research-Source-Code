/**************************************************************************
*
*			s c a n f   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"scanf" scans the standard input for items specified, and assigns
*	them to user supplied variables (via pointers to these vars).
*
*	Calling sequence:
*		ret = scanf(fmt,p1,p2,...)
*	Where:
*		ret = the number of items successfully matched & assigned
*			EOF returned if encountered on input
*		fmt -> a string specifying how to parse the input
*		p1,p2,... -> where matched items get stored
*
*****************************************************************************/

#include "stdio.h"

WORD	scanf(fmt,ptrs)				/* CLEAR FUNCTION ***********/
	BYTE *fmt;
	BYTE *ptrs;
{
	return(_doscan(stdin,fmt,&ptrs));
}


/**************************************************************************
*
*			f s c a n f   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fscanf" scans the given stream for items specified, and assigns
*	them to user supplied variables (via pointers to these vars).
*
*	Calling sequence:
*		ret = fscanf(sp,fmt,p1,p2,...)
*	Where:
*		ret =  the number of items successfully matched & assigned
*			EOF returned if encountered on input
*		sp  -> (FILE *) stream pointer
*		fmt -> a string specifying how to parse the input
*		p1,p2,... -> where matched items get stored
*
*****************************************************************************/
WORD	fscanf(sp,fmt,ptrs)			/* CLEAR FUNCTION ***********/
	FILE *sp;
	BYTE *fmt;
	BYTE *ptrs;
{
	return(_doscan(sp,fmt,&ptrs));
}
