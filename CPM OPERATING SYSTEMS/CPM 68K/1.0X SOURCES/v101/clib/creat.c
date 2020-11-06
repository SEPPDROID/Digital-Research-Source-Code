/****************************************************************************
*
*		   C   F I L E   C R E A T E   R O U T I N E
*		   -----------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	The "creat" routine opens a new "C" file and returns a file id. 
*	Comes in 3 flavors: ascii (CP/M text files), binary, and default
*	(currently ascii).
*
*	Calling Sequence:
*		fid = creat(fname,prot)
*		fid = creata(fname,prot)
*		fid = creatb(fname,prot)
*		fid = _creat(fname,prot,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		prot	is the UNIX file protection
*		type	is 0 for ASCII, 1 for BINARY
*
*****************************************************************************/
#include	<portab.h>			/* Include std definitions  */
#include	<cpm.h>				/* Include CP/M definitions */
#include	<errno.h>			/* Error codes		    */
EXTERN	BYTE	__tname[];			/* Terminal name	    */
EXTERN	BYTE	__lname[];			/* List device name	    */
						/****************************/
WORD	creat (f,p)
	BYTE *f; WORD p;
{	return(_creat(f,p,0));		}	/* default to ascii	    */

WORD	creata (f,p)
	BYTE *f; WORD p;
{	return(_creat(f,p,0));		}	/* ascii file open	    */

WORD	creatb (f,p)
	BYTE *f; WORD p;
{	return(_creat(f,p,1));		}	/* binary file open	    */

WORD	_creat (fname,prot,type)		/****************************/
	BYTE	*fname;				/* -> File name		    */
	WORD	prot;				/* Open mode		    */
	WORD	type;				/* ASCII/BINARY flag	    */
{						/****************************/
	REG  WORD ich;				/* Channel number for open  */
	WORD allocc();				/* gets a channel	    */
	REG  FD 	*ch;			/* -> CCB for channel	    */
						/*			    */
	if((ich = allocc()) < 0)		/* Allocate a channel	    */
		return (FAILURE);		/*	Can't		    */
						/*			    */
	__chinit(ich);				/* Clear out channel's ccb  */
	ch = _getccb(ich);			/* Get address of ccb	    */
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
	if(__open(ich,fname,CREATE) != 0)	/* Use BDOS interface	    */
	{					/*			    */
		freec(ich);			/*   Release channel	    */
		RETERR(FAILURE,ENODSPC);	/*   Oops, No dir space.    */
	}					/****************************/
						/* Else:		    */
	ch -> flags |= OPENED;			/*	 Set OPEN bit	    */
	return(ich);				/*	 Return Channel #   */
}						/****************************/
