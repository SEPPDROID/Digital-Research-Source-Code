/***************************************************************************
*
*			f d o p e n   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fdopen" associates a file which was opened by a file descriptor
*	(using "open" or "creat") with a stream.
*
*	Calling sequence:
*		stream = fdopen( fd, mode )
*	Where:
*		stream -> stream info (FILE *) (NULLPTR returned on fail)
*		fd     =  small int returned by open or creat
*		mode   = "r" for read-only, "w" for write, "a" for append
*
*****************************************************************************/

#include "stdio.h"

FILE *	fdopen( fd, mode )			/* CLEAR FUNCTION ***********/
REG	WORD fd;
REG	BYTE *mode;
{
REG	FILE *sp;
REG	WORD ii;
						/****************************/
	if( fd<0  || lseek(fd,0L,1)==FAILURE )	/* is fd valid?		    */
		return(NULLFILE);		/*	no, oh well	    */
	for( ii=0; 				/* look at _iob table	    */
	    (sp=(&_iob[ii]))->_flag&(_IOREAD|_IOWRT); /* not marked rd/wrt  */
	    ii++ )				/*			    */
		if( ii >= MAXFILES )		/* if off end of table	    */
		    break;			/*			    */
	if( ii >= MAXFILES )			/*			    */
		return(NULLFILE);		/*   fail		    */
	if( *mode != 'r' && *mode != 'R')	/* not 'r'ead mode?	    */
	{					/*			    */
		sp->_flag |= _IOWRT;		/*   set this flag	    */
		if( *mode == 'a' ||		/* 'a'ppend mode?	    */
		    *mode == 'A' )		/*			    */
			lseek(fd,0L,2);		/* its out there, seef EOF  */
	}					/*   ************************/
	else 	sp->_flag |= _IOREAD;		/* 'r'ead mode		    */
						/****************************/
	sp->_cnt = 0;				/* init count		    */
	sp->_fd = fd;				/*  and file des	    */
	sp->_base = sp->_ptr = NULLPTR;		/*  and buffer pointers	    */
						/****************************/
	return(sp);				/* return the stream ptr    */
}						/****************************/
