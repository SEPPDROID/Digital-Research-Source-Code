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
#include <stdio.h>
WORD fseek(sp,offs,sense)
	FILE *sp;
	LONG offs;
	WORD sense;
{
	LONG p, lseek();
	if(sp->_flag & _IOWRT)			/* Only if file writable  */
	{
		if( fflush(sp) == FAILURE )
			return( FAILURE );
	}
	p = lseek(fileno(sp),offs,sense);
	return( p==(LONG) FAILURE ? FAILURE : SUCCESS);
}
