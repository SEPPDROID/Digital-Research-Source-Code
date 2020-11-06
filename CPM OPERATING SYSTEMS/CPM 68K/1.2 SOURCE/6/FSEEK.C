/**************************************************************************
*
*			f s e e k   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fseek" sets the read/write pointer of a stream to an arbitrary
*	offset.
*
*	Calling sequence:
*		ret = fseek(sp,offset,sense)
*	Where:
*		sp -> (FILE *) stream to seek on
*		offset = signed number of bytes
*		sense =	0  =>	offset from beginning of file
*			1  =>	offset from current position
*			2  =>	from end of file
*		ret = 0 for success, -1 for failure
*
*****************************************************************************/

#include "stdio.h"

WORD	fseek(sp,offs,sense)			/* CLEAR FUNCTION ***********/
REG	FILE *sp;
	LONG offs;
	WORD sense;
{
	LONG p, lseek();

	if ( fflush(sp) == FAILURE )
		return( FAILURE );
	p = lseek(fileno(sp),offs,sense);
	sp->_flag &= ~_IOEOF;			/* any seek clears EOF flag */
	return( p==(LONG) FAILURE ? FAILURE : SUCCESS);
}


/************************************************************************
*
*			r e w i n d   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"rewind" sets the read/write pointer of a stream file to the
*	beginning of the stream.
*
*	Calling sequence:
*		ret = rewind(sp)
*	Where:
*		sp -> a stream file (FILE *)
*		ret = 0 for success, -1 for failure
*
*************************************************************************/
WORD	rewind(sp)				/* CLEAR FUNCTION ***********/
	FILE *sp;
{
	return(fseek(sp,0L,0));
}

WORD	rewind(sp)				/* CLEAR FUNCTION ***********/
	FILE *sp;
{
	return(fseek(sp,0L,0));
}

WORD	rewind(sp)				/* CLEAR FUNCTION ***********/
	FILE *sp;
{
	return(fseek(sp,0L,0));
}
