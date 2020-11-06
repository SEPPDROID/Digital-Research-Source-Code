/*****************************************************************************
*
*	      C H A N N E L   N U M B E R   V A L I D A T I O N
*	      -------------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	This routine is used to validate a channel number and return the
*	pointer to the ccb area.  The channel must be in range and open.
*
*	Calling Sequence:
*
*		ccbptr = _chkc(ch);
*
*	Where:
*
*		ch	Is the channel number
*		ccbptr	Is the returned ccb address
*
*****************************************************************************/
#include	<portab.h>			/* Include std definitions  */
#include	<cpm.h>				/* and CP/M definitions	    */
#include	<errno.h>
FD	 *_chkc(ch)				/****************************/
REG	UWORD	 ch;				/* Facilitate error check   */
{						/*			    */
	REG	FD *xcb;			/* -> CCB		    */
						/****************************/
	if(ch >= MAXCCBS)			/* Is channel in range?	    */
		RETERR(NULL,EBADF);		/* No, quit now.	    */
						/*			    */
	xcb = _getccb(ch);			/* xcb -> ccb for channel   */
	if((xcb->flags & OPENED) == 0)		/* Is channel OPEN?	    */
		RETERR(NULL,EBADF);		/* Noooooooo!!		    */
	return(xcb);				/* Else, return pointer     */
}						/****************************/
