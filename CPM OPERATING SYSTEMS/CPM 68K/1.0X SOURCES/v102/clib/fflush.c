/****************************************************************************
*
*			f f l u s h   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	'fflush' causes any buffered data in a stream to be written out
*	to the file.  The stream remains open.
*
*	Calling sequence:
*		ret = fflush(stream)
*	Where:
*		stream is a (FILE *)
*		ret = SUCCESS or FAILURE (I/O error)
*
*****************************************************************************/
#include <stdio.h>
WORD fflush(sp)
REG	FILE *sp;				/* stream to flush	    */
{						/****************************/
REG	WORD	n,				/* num written		    */
		ns;				/* num sposed to be written */
						/****************************/
	if((sp->_flag&(_IONBUF|_IOWRT))==_IOWRT /* does it have a wrt buf?  */
	    && (ns=sp->_ptr-sp->_base) > 0 )	/*  and does buf need wrt?  */
	{					/* yes!			    */
		n=write(sp->_fd,sp->_base,ns);	/* do it		    */
		if(ns!=n)			/* did they all git writ?   */
		{				/* oooops	  	    */
			sp->_flag |= _IOERR;	/*   this stream no good    */
			return(FAILURE);	/*   let em know	    */
		}				/*			    */
	}					/****************************/
	if((sp->_flag&_IOWRT)!=0 && sp->_base!=NULL)/* written to already?  */
	{					/*			    */
		if(sp->_flag&_IONBUF)		/* is this a nobuf stream?  */
			sp->_cnt = 1;		/* yes			    */
		else	sp->_cnt = BUFSIZ-1;	/* standard size	    */
	} else sp->_cnt = 0;			/* ow. zap out count	    */
	sp->_ptr=sp->_base;			/* reset buf		    */
	return(SUCCESS);			/*			    */
}						/****************************/
