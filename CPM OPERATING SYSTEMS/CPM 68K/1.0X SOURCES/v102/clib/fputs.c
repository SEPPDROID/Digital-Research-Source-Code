/**************************************************************************
*
*	f p u t s   F u n c t i o n
*	---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fputs" copies a null terminated string to a stream file.
*	To be compatible with Unix, it does NOT append a newline.
*
*	Calling sequence:
*		ret = fputs(s,stream)
*	Where:
*		ret = last char out, on error FAILURE
*		s  = string to put
*		stream -> stream (FILE *)
*
*****************************************************************************/
#include <stdio.h>
WORD fputs(s,f)					/****************************/
REG	BYTE *s;				/* null term string	    */
REG	FILE *f;				/* stream file pointer	    */
{						/****************************/
REG	WORD rv;				/* return val		    */
	rv = ZERO;				/* init for *s==NULL	    */
	while(*s) 				/* for all chars in s	    */
		if((rv=putc(*s++,f)) == FAILURE) /*   if putc fouls up	    */
			return(FAILURE);	/*     give up		    */
	return(rv);				/* it worked		    */
}						/****************************/
