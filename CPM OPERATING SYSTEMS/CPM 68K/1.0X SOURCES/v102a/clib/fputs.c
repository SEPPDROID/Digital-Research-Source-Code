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

#include "stdio.h"

WORD	fputs(str,sp)				/* CLEAR FUNCTION ***********/
REG	BYTE *str;				/* null term string	    */
REG	FILE *sp;				/* stream file pointer	    */
{						/****************************/
REG	WORD rv;				/* return val		    */
	rv = NULL;				/* init for *s==NULL	    */
	while(*str) 				/* for all chars in s	    */
	    if((rv=putc(*str++,sp))==FAILURE)	/*   if putc fouls up	    */
			return(FAILURE);	/*     give up		    */
	return(rv);				/* it worked		    */
}						/****************************/
