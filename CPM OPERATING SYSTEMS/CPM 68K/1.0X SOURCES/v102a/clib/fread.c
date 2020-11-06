/**************************************************************************
*
*			f r e a d   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fread" reads NI items of size SZ from stream SP and deposits them
*	at BUFF.
*
*	Calling sequence:
*		nitems = fread(buff,size,nitems,stream)
*	Where:
*		buff -> where to read to
*		size = number bytes in each item
*		nitems = number bytes read/to read
*		stream -> file
*
*	nitems is set to 0 if an error occurs (including EOF).
*
*****************************************************************************/

#include "stdio.h"

WORD	fread(buff,siz,num,sp)			/* CLEAR FUNCTION ***********/
REG	BYTE *buff;
	WORD siz,
	     num;
REG	FILE *sp;
{
REG	WORD jj, kk, ch;


	for( jj=0; jj<num; jj++ )
	    for( kk=0; kk<siz; kk++ )
	    {
		if( (ch = getc(sp)) == FAILURE )
		    return(NULL);
		else *buff++ = ch;
	    }
	return(num);
}
