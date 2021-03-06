/****************************************************************************
*
*		   C   F I L E   O P E N   R O U T I N E
*		   -------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	The "open" routine opens a "C" file and returns a file id. 
*	Comes in 3 flavors, one for ascii, one for binary, and the default
*	(currently to ascii).
*
*	Calling Sequence:
*		fid = open(fname,mode)
*		fid = opena(fname,mode)
*		fid = openb(fname,mode)
*		fid = _open(fname,mode,type);
*
*	Where:
*		fname	is the address of a null terminated file name.
*		mode	is the open mode:
*				0 => READ access only
*				1 => WRITE access only
*				2 => Both READ and WRITE
*		type	is 0 for ASCII files, 1 for BINARY
*
*
*****************************************************************************/
#include	"portab.h"			/* 			    */
#include	"osif.h"			/* 			    */
#include 	"osiferr.h"			/*			    */
#include	"errno.h"			/*			    */
EXTERN	BYTE	__tname[];			/* -> Terminal name	    */
EXTERN	BYTE	__lname[];			/* -> List device name	    */

WORD	_open (fname,mode,xtype)		/****************************/
	BYTE	*fname;				/* -> File name		    */
	WORD	mode;				/* Open mode		    */
	WORD	xtype;				/* File type		    */
{						/****************************/
	REG  WORD	ich;			/* Channel number for open  */
	REG  FD		*ch;			/* -> CCB for channel	    */
						/*			    */
	if((ich = _allocc()) == FAILURE)	/* Allocate a channel	    */
		return(FAILURE);		/*	Can't: EMFILE	    */
						/*			    */
	__chinit(ich);				/* Clear out channel's ccb  */
	ch = _getccb(ich);			/* Get address of ccb	    */
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
	if(__open(ich,fname,OPEN) != 0)		/* Use POS SVC interface    */
	{					/*			    */
		_freec(ich);			/* deallocate channel       */
		RETERR(FAILURE,ENOENT);		/* If return <> 0, NFG.	    */
	}					/****************************/
						/* Else:		    */
	ch -> flags |= OPENED;			/*	 Set OPEN bit	    */
	lseek(ch->chan, 0L, 2);			/*       Kludge to set      */
	lseek(ch->chan, 0L, 0);			/*       hi water mark      */
	return(ich);				/*	 Return Channel #   */
}						/****************************/

WORD	open(fname,mode)			/* CLEAR FUNCTION ***********/
	BYTE *fname; WORD mode;
{	return(_open(fname,mode,0));	}	/* default to ascii	    */

WORD	opena(fname,mode)			/* CLEAR FUNCTION ***********/
	BYTE *fname; WORD mode;
{	return(_open(fname,mode,0));	}	/* open ascii file	    */

WORD	openb(fname,mode)			/* CLEAR FUNCTION ***********/
	BYTE *fname; WORD mode;
{	return(_open(fname,mode,1));	}	/* open binary file	    */
