/****************************************************************************/
/*									    */
/*			  C l o s e   F u n c t i o n			    */
/*			  ---------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
/*									    */
/*	The close function is used to terminate access to a file / device.  */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = close(fd);					    */
/*									    */
/*	Where "fd" is an open file descriptor to be CLOSEd.		    */
/*									    */
/****************************************************************************/
#include <portab.h>
#include <cpm.h>
#include <errno.h>
						/****************************/
EXTERN	BYTE	__xeof;				/* End of file for ASCII    */
WORD close(fd)					/****************************/
						/*			    */
	WORD	fd;				/* File descriptor to close */
						/*			    */
{						/****************************/
EXTERN	FD	*_chkc();			/* fd -> fp conversion	MGL */
REG	FD	*fp;				/* file pointer Temporary   */
						/*			    */
	if((fp=_chkc(fd)) == NULL)		/* File Open?		    */
		return(FAILURE);		/*		no, quit    */
						/*			    */
	if ((fp->flags & (ISTTY|ISLPT)) != 0)	/* Character device?	    */
	{					/*			    */
		__chinit(fd);			/* Clean the channel again  */
		freec(fd);			/* Release the channel	    */
		return(SUCCESS);		/* All done		    */
	}					/****************************/
	if((fp->flags & ISASCII) != 0 &&	/* ASCII file?		    */
	   (fp->flags & ISREAD)  == 0)		/* and not read-only	    */
	{					/*			    */
	   if( fp->offset < fp->hiwater )	/* Have we been seeking?    */
	      lseek(fd,0L,2);			/*   Seek to EOF ifso	    */
	   write(fd,&__xeof,1);			/* Write a ^Z character	    */
	}					/****************************/
	if((fp->flags & DIRTY) != 0)		/* Buffer dirty?	    */
	{					/* 			    */
	  if(_blkio(fp,fp->sector,fp->buffer,	/* Write out		    */
	     1L,B_WRITE) != 1)			/*		buffer	    */
		RETERR(FAILURE,EIO);		/* can't		    */
	}					/****************************/
	if(__BDOS(CLOSE,&(fp->fcb))==0xFF)	/* Close the fcb	    */
		RETERR(FAILURE,EIO);		/*			    */
	__chinit(fd);				/* Release the space	    */
	freec(fd);				/* Release the channel	    */
	return(SUCCESS);			/* Done			    */
}						/****************************/
