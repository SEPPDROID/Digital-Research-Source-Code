/* NOTTYIN: eliminates console input functions from read(). */


nottyin() { ; }					/* stubroutine 'tag'	    */

static char * __nottyin_msg = "tty input";

_ttyin() { _optoff(__nottyin_msg); }

only 9 files instead of 16
 *		chkc.c   - (to be consistent with chinit()
 *		doprt.c  - stripped down, does not support longs & floats
 *		_ttyin.c - dummy routine
 *		_filesz - dummy rtn
 *
 *  (last mod 12/21/83  WHF )
 */
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

#define MAXCCBS  9
FD		 _fds[MAXCCBS]; 		/* Allocate CCB storage	    */

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
	* convert fd to CCB	    */
	ch -> chan   = i;			/* Load channel byte	    */
	ch -> flags  = 0;			/* clear flag word	    */
	* convert fd to CCB	    */
	ch -> chan   = i;			/* Load channel byte	    */
	ch -> flags  = 0;			/* clear flag word	    */
	