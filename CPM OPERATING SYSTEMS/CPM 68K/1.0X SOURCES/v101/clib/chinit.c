/*****************************************************************************
*
*		     C C B   I N I T I A L I Z A T I O N
*		     -----------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Routine "_chinit" is called from the run-time initialization to clear
*	out all the CCB storage.
*	Calling sequence:
*		_chinit();
*
*	Routine "__chinit(fd)" is called from other low-level routines
*	for single channel initialization.
*	Calling sequence:
*		__chinit(fd)
*	where:	fd = file descriptor #
*
*****************************************************************************/
#include	<portab.h>			/* Include std definitions  */
#include	<cpm.h>				/* Include CP/M definitions */

FD		 _fds[MAXCCBS];			/* Allocate CCB storage	    */

_chinit()					/****************************/
{						/*			    */
	REG WORD	i;			/*	Index		    */
	for(i=0;i<MAXCCBS;i++)			/*	For all channels    */
		__chinit(i);			/*	Init fds(i);	    */
}						/****************************/

/*****************************************************************************
*	The __chinit routine initializes only 1 channel.
*****************************************************************************/

__chinit(i)
{						/****************************/
	REG FD 		*ch;			/* -> CCB		    */
	REG BYTE	*p;			/* Byte pointer temporary   */

	ch =  _getccb(i);			/* convert fd to CCB	    */
	ch -> chan   = i;			/* Load channel byte	    */
	ch -> flags  = 0;			/* clear flag word	    */
	ch -> sector = -1;			/* Set no sector in buff    */
	ch -> offset = 0;			/* Clear file offset word   */
	ch -> hiwater = 0;			/* Init hiwater mark	    */
	ch -> fcb.drive = 0;			/* Init drive field of fcb  */
	p = &ch->fcb.fname[0];			/* Set to file var	    */
	while (p< &ch->fcb.ftype[3])		/* Init file name fields    */
		*p++ = ' ';			/* To spaces		    */
	while (p < &ch->fcb.record)		/* Init rest of fcb	    */
		*p++ = 0;			/* To zeros		    */
}						/****************************/

