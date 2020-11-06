/***********************************************************************
*
*			f t e l l   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"ftell" returns the present value of the read/write pointer
*	within the stream.  This is a meaningless number for console
*	and list devices.
*
*	Calling sequence:
*		offset = ftell(sp)
*	Where:
*		sp -> (FILE *) stream
*		offset = (LONG) where stream is reading from/writing to
*
******************************************************************************/
#include <stdio.h>
LONG ftell(sp)
	REG FILE *sp;				/* stream pointer	    */
{						/*			    */
	REG LONG filepos;			/* file position	    */
	REG BYTE *bp;				/* Buffer ptr		    */
	LONG lseek();				/* byte offset into file    */
						/*			    */
	if( isatty(fileno(sp)) )		/* are we talking to a tty? */
		return(0);			/* quit now if so	    */
	filepos = FAILURE;			/* default return value	    */
	if( sp->_flag & (_IOREAD|_IOWRT) )	/* if file is open	    */
	{					/*			    */
	    if((filepos=lseek(fileno(sp),0L,1))	/* get where next byte read */
		== FAILURE )			/*  from or written to	    */
		    return(FAILURE);		/*  quit if bad lseek	    */
	    filepos += sp->_ptr - sp->_base;	/* correct for # read/wrtn  */
	    if( sp->_flag & _IOREAD )		/* if reading from file	    */
	      if( filepos > 0 )			/*  and we've read from file*/
		filepos -= sp->_ptr - sp->_base + sp->_cnt; /*		    */
						/* adjust file position	    */
						/*  to reflect read ahead   */
	    if( sp->_flag & _IOASCI )		/* ascii file? **************/
	    {					/* count the newlines	    */
	      if( sp->_flag & _IOWRT )		/* add in newline's cr's    */
	      {					/*			    */
		for( bp=sp->_base; bp < sp->_ptr; bp++ ) /*		    */
		    if( *bp == '\n' )		/* count newlines in stuff  */
			filepos++;		/*   written/read so far    */
	      } else {				/* we're reading...	    */
		if( filepos > 0 )		/* check to see we've read  */
	          for(bp= &(sp->_ptr[sp->_cnt-1]);/* start at end of buffer */
			bp >= sp->_ptr; bp-- )	/* back up to next read char*/
		    if( *bp == '\n' )		/* count off for newlines   */
			filepos--;		/*			    */
	      }					/*			    */
	    }					/* fini ascii ***************/
	}					/****************************/
	return(filepos);			/*			    */
}						/****************************/
