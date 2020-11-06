/************************************************************************
*
*			_ f l s b u f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	'_flsbuf' handles writes to the stream when its buffer is full.
*	Included is the ability to handle 'non-buffered' (_IONBUF), as
*	well as line buffered (_IOLBUF) output.  It is supposed to be
*	called by 'putc'.  It will init the buffers, as needed.
*
*	Calling sequence:
*		chr = _flsbuf( ch, stream )
*	Where:
*		ch  = the 1st char to be buffered
*		stream -> where its going (FILE *)
*		chr = ch if write ok, FAILURE (-1 WORD) o.w.
*
*****************************************************************************/
#include <stdio.h>
WORD _flsbuf(c,sp)				/****************************/
REG	BYTE c;					/* char to be written	    */
REG	FILE *sp;				/* stream to write to	    */
{						/****************************/
	BYTE csave;				/* a temp save area	    */
	BYTE *malloc();				/* alloc memory		    */
REG	WORD n,					/* number chars written	    */
	     ns;				/* num chars sposed to be w.*/
	ns = n = 0;				/* init them		    */
	if( (sp->_flag & _IOWRT) == 0 )		/* is this file writeable?  */
		return(FAILURE);		/*   no!		    */
	if( (sp->_flag & _IONBUF) == 0  &&	/* is it not a no buff file */
		sp->_base == NULL )		/*   and no init yet?	    */
	{					/* ifso...*******************/
	    if( (sp->_ptr=sp->_base=malloc(BUFSIZ)) == NULL ) /* alloc ok?  */
		sp->_flag |= _IONBUF;		/* set to a no buff file    */
	    else {				/* o.w. we have a buffer!   */
		sp->_flag |= _IOABUF;		/* mark it as alloc'd	    */
		if( isatty(sp->_fd) )		/* do we handle newlines?   */
		    sp->_flag |= _IOLBUF;	/* yessir, this is a linbuf */
		else {				/* o.w. *********************/
		    sp->_cnt = BUFSIZ-2;	/* lv room for 1st & last ch*/
		    return(*sp->_ptr++ = c);	/* store 1st & quit	    */
		}				/*tty chk********************/
	    }					/*malloc*********************/
	}					/*null buff******************/
	if( sp->_flag & _IONBUF )		/* insure this pts ok	    */
		sp->_ptr = sp->_base = &csave;	/* give buff a temp place   */
	*sp->_ptr++ = c;			/* put this somewhere	    */
	if( sp->_flag & _IONBUF )		/* if a no buff file	    */
	{					/*			    */
	    n=write(sp->_fd,sp->_base,(ns=1));	/* write single char	    */
	    sp->_cnt = 0;			/* enforce coming back again*/
	} 					/****************************/
	else if( sp->_flag & _IOLBUF )		/* if a line buff file	    */
	{					/*			    */
	    if(c=='\n' || sp->_ptr >= sp->_base+BUFSIZ ) /* buf full or '\n'*/
	    {					/*			    */
		n=write(sp->_fd,sp->_base,	/* output the line	    */
		    (ns=sp->_ptr-sp->_base));	/*			    */
		sp->_ptr = sp->_base;		/* reset the buffer	    */
	    }					/*			    */
	    sp->_cnt = 0;			/* enforce coming right back*/
	}					/****************************/
	else					/* o.w. we really have a    */
	{					/*	full buffer	    */
	    n=write(sp->_fd,sp->_base,		/* output the whole buff    */
		(ns=sp->_ptr-sp->_base));	/*			    */
	    sp->_cnt = BUFSIZ-1;		/* lv room for last char    */
	    sp->_ptr = sp->_base;		/* init ptr		    */
	}					/* end buf handlers**********/
	if( ns != n )				/* error on write?	    */
	{					/*			    */
	    sp->_flag |= _IOERR;		/* mark stream		    */
	    return(FAILURE);			/* and die		    */
	}					/****************************/
	return(((WORD) c) & 0xff );		/* everything ok, quit	    */
}						/****************************/
