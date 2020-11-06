/****************************************************************************/
/*									    */
/*			    L s e e k   F u n c t i o n			    */
/*			    ---------------------------			    */
/*									    */
/*	The lseek function simulates the UNIX lseek system call.	    */
/*									    */
/*	Calling sequence:						    */
/*									    */
/*		ret = lseek(fd,offset,sense);				    */
/*									    */
/*	Where:								    */
/*									    */
/*		fd	Is an open file descriptor			    */
/*		offset	Is the (long) file offset			    */
/*		sense	Is the meaning of offset			    */
/*			0 => From the beginning of the file		    */
/*			1 => From the present file position		    */
/*			2 => From the end of the file			    */
/*									    */
/*		ret	Is the resulting absolute file offset		    */
/*			-1 indicates failure				    */
/*									    */
/****************************************************************************/
#include "stdio.h"
#include "cpm.h"
LONG	lseek(fd,offs,sense)			/****************************/
						/*			    */
	WORD	fd;				/* Open file descriptor	    */
	LONG	offs;				/* File offset		    */
	WORD	sense;				/* Sense of offset	    */
						/*			    */
						/****************************/
{
						/****************************/
REG	FILE	*fp;				/* File pointer		    */
EXTERN	LONG	_chkc();			/* fd to fp translation	    */
						/*			    */
	if((LONG)(fp = _chkc(fd)) == FAILURE)	/* Convert to pointer	    */
		return(FAILURE);		/*   Can't		    */
						/*			    */
	switch(sense)				/****************************/
	{					/*			    */
	case	0:				/* From beginning of file   */
		fp->offset = offs;		/* Just set to offset	    */
		break;				/* Then quit		    */
						/****************************/
	case	1:				/* From present position    */
		fp->offset += offs;		/*			    */
		break;				/*			    */
						/****************************/
	case	2:				/* From end of file	    */
		fp->offset = fp->hiwater + offs;/* compute end of file	    */
		break;				/*			    */
						/****************************/
	default:				/* All others NFG	    */
		return(FAILURE);		/*			    */
	}					/****************************/
	fp->flags &= ~ATEOF;			/* any seek clears EOF	    */
	return(fp->offset);			/* Return resultant offset  */
}						/****************************/
