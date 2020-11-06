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
*		rval = fread(buff,size,nitems,stream)
*	Where:
*		buff -> where to read to
*		size = number bytes in each item
*		nitems = number bytes read/to read
*		stream -> file
*
*		rval = number items successfully read
*
*	Edits:
*	8-Feb-84 whf	return nitems actually read for EOF.
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
		    return(jj);			/* # items successfully read */
		else *buff++ = ch;
	    }
	return(num);
}
*buff++ = ch;
	    }
	return(num);
}
*buff++ = ch;
	    }
	return(num);
}
