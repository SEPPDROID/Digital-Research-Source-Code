/*****************************************************************************
*
*		A L L O C C   /   F R E E C   R O U T I N E S
*		---------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Routines "allocc" and "freec" are used to allocate / deallocate a 
*	channel number so that the user may do I/O directly without the OTS
*	getting in the way.
*	Note that this scheme preserves compatibility with OS's that really
*	truly use channels, and allows users to inform the C Run Time Library
*	routines that they are using the channel.
*	It's not important to use these routines under CP/M, since CP/M
*	allows you to have as many FCBs as you wish.
*
*	Calling Sequence:
*
*		i = allocc();
*		freec(i);
*
*****************************************************************************/
#include	<portab.h>			/* Include std definitions  */
#include	<cpm.h>				/* Also CP/M ones	    */
#include	<errno.h>			/* for error returns	    */
LONG		_chvec	={0};			/* Allocate storage	    */
WORD	allocc()				/****************************/
{						/*			    */
	REG WORD i;				/* Define 2 temporaries	    */
	REG LONG j;				/*			    */
						/*			    */
	j = 1;					/* Start with channel 0	    */
	for(i=0;i<MAXCCBS;i++)			/* Look at the bits	    */
	{					/*			    */
	  if((j & _chvec) == 0)			/* If 0, then channel free  */
	  {					/*			    */
		_chvec |= j;			/* set allocated bit	    */
		return(i);			/* and return the channel # */
	  }					/*			    */
	  j = j << 1;				/* Up to next bit	    */
	}					/* End FOR loop		    */
	RETERR(FAILURE,EMFILE);			/* All channels in use!	    */
}						/****************************/

WORD	freec(ch)				/****************************/
REG	WORD	ch;				/* Channel number to free   */
{						/*			    */
	_chvec &= ~(1 << ch);			/* Clear appropriate bit    */
	return(SUCCESS);			/* Return OK		    */
}						/****************************/
