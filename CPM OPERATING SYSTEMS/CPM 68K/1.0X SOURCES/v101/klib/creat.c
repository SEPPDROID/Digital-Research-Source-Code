/****************************************************************************
*
*		   C   F I L E   C R E A T E   R O U T I N E
*		   -----------------------------------------
*
*	The "creat" routine opens a new "C" file and returns a file id. 
*
*	Calling Sequence:
*
*		fid = creat(fname,prot,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		prot	is the UNIX file protection
*		type	is 0 for ASCII, 1 for BINARY
*
*****************************************************************************/
#include	"stdio.h"			/* Include std definitions  */
#include	"cpm.h"				/* Include CP/M definitions */
extern	char	__tname[];			/* -> CON:		    */
extern	char	__lname[];			/* -> LST:		    */
WORD	creat (fname,prot,type)			/****************************/
	BYTE	*fname;				/* -> File name		    */
	WORD	prot;				/* Open mode		    */
	WORD	type;				/* ASCII/BINARY flag	    */
{						/****************************/
	REG  WORD	ich;			/* Channel number for open  */
	REG  FILE 	*ch;			/* -> CCB for channel	    */
EXTERN	FILE		*_getc();		/* Converts fd to fp	    */
						/*			    */
	if((ich = igetc()) < 0)			/* Allocate a channel	    */
		return (FAILURE);		/*	Can't		    */
						/*			    */
	__chinit(ich);				/* Clear out channel's ccb  */
	ch = _getc(ich);			/* Get address of ccb	    */
						/*			    */
	if(type==0)				/* ASCII file?		    */
		ch -> flags |= ISASCII;		/* 	Yes, mark it.	    */
						/****************************/
	if(_strcmp(fname,__tname) == 0)		/* Terminal file?	    */
	{					/*			    */
		ch -> flags |= ISTTY|OPENED;	/* Set flag		    */
		return(ich);			/* Return file descriptor   */
	}					/****************************/
	else if(_strcmp(fname,__lname) == 0)	/* List device?		    */
	{					/*			    */
		ch -> flags |= ISLPT|OPENED;	/* set flag		    */
		return(ich);			/*			    */
	}					/*			    */
						/****************************/
	if(_open(ich,fname,CREATE) != 0)	/* Use BDOS interface	    */
		return(FAILURE);		/* If return <> 0, NFG.	    */
						/* Else:		    */
	ch -> flags |= OPENED;			/*	 Set OPEN bit	    */
	return(ich);				/*	 Return Channel #   */
}						/****************************/
