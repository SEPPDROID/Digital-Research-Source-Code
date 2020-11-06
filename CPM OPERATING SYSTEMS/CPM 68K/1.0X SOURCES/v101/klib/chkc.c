/*****************************************************************************
*
*	      C H A N N E L   N U M B E R   V A L I D A T I O N
*	      -------------------------------------------------
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
#include	"stdio.h"			/* Include std definitions  */
#include	"cpm.h"				/* and CP/M definitions	    */
FILE	 *_chkc(ch)				/****************************/
	UWORD	 ch;				/* Facilitate error check   */
{						/*			    */
	REG	FILE *xcb;			/* -> CCB		    */
	EXTERN	FILE *_getc();			/* converts fd to fp	    */
						/****************************/
	if(ch >= MAXFILE)			/* Is channel in range?	    */
		return(FAILURE);		/* No, quit now.	    */
						/*			    */
	xcb = _getc(ch);			/* xcb -> ccb for channel   */
	if((xcb->flags & OPENED) == 0)		/* Is channel OPEN?	    */
		return (FAILURE);		/* Noooooooo!!		    */
	return(xcb);				/* Else, return pointer     */
}						/****************************/
