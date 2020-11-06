/************************************************************************
*
*			f p u t n   F u n c t i o n
*			---------------------------
*	Copyright 1983 by Digital Research Inc.  All rights reserved.
*
*	'fputn()' writes 'n' chars to a buffered file.  It optimizes
*	for 'non-buffered' (_IONBUF) output.
*
*	Calling sequence:
*		rv = fputn(cp,n,stream)
*	Where:
*		cp -> chars to be put
*		n  =  num chars to be put
*		stream -> where its going (FILE *)
*		rv = SUCCESS if write ok, FAILURE (-1 WORD) o.w.
*
*****************************************************************************/

#include "stdio.h"

WORD	fputn(buf,num,sp)			/* CLEAR FUNCTION ***********/
REG	BYTE *buf;				/* chars to be written	    */
REG	WORD num;				/* num chars to be written  */
REG	FILE *sp;				/* stream to write to	    */
{						/****************************/
	if( sp->_flag & _IONBUF )		/* Non-buffered file?	    */
	{					/*			    */
		sp->_cnt = 0;			/* Always force to zero	    */
		if(num!=write(sp->_fd,buf,num))	/* try to write		    */
		{				/*  no good		    */
			sp->_flag |= _IOERR;	/* mark this file	    */
			return FAILURE;		/* and complain		    */
		}				/*			    */
		return SUCCESS;			/*			    */
	}					/****************************/
						/*			    */
	while(num--)				/* Buffered output	    */
		if( FAILURE==fputc(*buf++,sp))	/* try to put in buf	    */
			return FAILURE;		/*  no good		    */
	return SUCCESS;				/* everything a-ok	    */
}						/****************************/
