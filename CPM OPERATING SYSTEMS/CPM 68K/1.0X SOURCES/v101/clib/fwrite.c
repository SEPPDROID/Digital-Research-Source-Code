/**************************************************************************
*
*			f w r i t e   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fwrite" writes NI items of size SZ from memory at BUFF into stream
*	SP.
*
*	Calling sequence:
*		nitems = fwrite(buff,size,nitems,stream)
*	Where:
*		buff -> where to write from
*		size = number bytes in each item
*		nitems = number bytes written/to write
*		stream -> file
*
*	nitems is set to 0 if an error occurs (including EOF).
*
*****************************************************************************/
#include <stdio.h>
WORD fwrite(buff,sz,ni,sp)
REG	BYTE *buff;
	WORD sz,
	     ni;
REG	FILE *sp;
{
REG	WORD jj, kk;

	for( jj=0; jj<ni; jj++ )
	    for( kk=0; kk<sz; kk++ )
		if( fputc(*buff++,sp) == FAILURE ) /* used for side effects */
		    return(ZERO);
	return(ni);
}
