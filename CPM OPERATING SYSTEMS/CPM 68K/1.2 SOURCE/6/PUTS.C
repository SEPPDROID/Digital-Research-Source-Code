/**************************************************************************
*
*			p u t s   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"puts" copies a null terminated string to the standard output.
*	It copies a newline char after the end of the string.
*
*	Calling sequence:
*		ret = puts(s)
*	Where:
*		s  = string to put
*		ret = last char output
*
*****************************************************************************/

#include "stdio.h"

WORD	puts(str)				/* CLEAR FUNCTION ***********/
REG	BYTE *str;				/* null term string	    */
{						/****************************/
	while(*str) 				/* for all chars in s	    */
	    if(putc(*str++,stdout) == FAILURE)	/*   if putc fouls up	    */
		return(FAILURE);		/*     give up		    */
	return( putc('\n',stdout) );		/* append newline & return  */
}						/****************************/
*/
}						/****************************/
*/
}						/****************************/
