/****************************************************************************
*
*		   C   F I L E   O P E N   R O U T I N E
*		   -------------------------------------
*
*	The "open" routine opens a "C" file and returns a file id. 
*
*	Calling Sequence:
*
*		fid = open(fname,mode,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		mode	is the open mode:
*
*				0 => READ access only
*				1 => WRITE access only
*				2 => Both READ and WRITE
*		type	is 0 for ASCII files, 1 for BINARY
*
*
*****************************************************************************/
#include	"stdio.h"			/* Include Whitesmith's	    */
#include	"cpm.h"				/* + CP/M		    */
EXTERN	BYTE	__tname[];			/* -> CON:		    */
EXTERN	BYTE	__lname[];			/* -> LST:		    */
WORD	open (fname,mode,xtype)			/****************************/
	BYTE	*fname;				/* -> File name		    */
	WORD	mode;				/* Open mode		    */
	WORD	xtype;				/* File type		    */
{						/****************************/
	REG  WORD	ich;			/* Channel number for open  */
	REG  FILE	 *ch;			/* -> CCB for channel	    */
	EXTERN FILE	*_getc();		/* fd to fp converter	    */
						/*			    */
	if((ich = igetc()) < 0)			/* Allocate a channel	    */
		return (FAILURE);		/*	Can't		    */
						/*			    */
	__chinit(ich);				/* Clear out channel's ccb  */
	ch = _getc(ich);			/* Get address of ccb	    */
						/*			    */
	if(mode == READ)			/* 	 If read only,	    */
		ch -> flags |= ISREAD;		/*	    set READONLY bit*/
	if(xtype == 0)				/* Is ASCII file?	    */
		ch -> flags |= ISASCII;		/*	Yes, mark it	    */
						/*			    */
	if(_strcmp(fname,__tname) == 0)		/* if a terminal	    */
	{					/* 			    */
	  ch->flags |= ISTTY|OPENED;		/* mark as tty		    */
	  return(ich);				/* Return file descriptor   */
	}					/****************************/
	else if(_strcmp(fname,__lname) == 0)	/* List device?		    */
	{					/*			    */
	  ch->flags |= ISLPT|OPENED;		/* Mark as printer	    */
	  return(ich);				/* Return file descriptor   */
	}					/*			    */
						/****************************/
	if(_open(ich,fname,OPEN) != 0)		/* Use POS SVC interface    */
		return(FAILURE);		/* If return <> 0, NFG.	    */
						/* Else:		    */
	ch -> flags |= OPENED;			/*	 Set OPEN bit	    */
	return(ich);				/*	 Return Channel #   */
}						/****************************/
