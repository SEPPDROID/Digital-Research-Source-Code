/****************************************************************************/
/*									    */
/*			     s b r k   F u n c t i o n			    */
/*			     -------------------------			    */
/*									    */
/*	The "sbrk" function is used to allocate memory dynamically.	    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		addr = sbrk(incr);					    */
/*									    */
/*	Where:								    */
/*		incr	Is the incremental number of bytes to be added to   */
/*			the program heap area.				    */
/*									    */
/*		addr	Is the beginning address of the allocated area.	    */
/*			-1 is returned if allocation failed		    */
/*									    */
/****************************************************************************/
#include "stdio.h"
#include "cpm.h"
EXTERN	BYTE	*_break;			/* Old break address	    */
						/****************************/
BYTE	*sbrk(incr)				/*			    */
	WORD	incr;				/* Incremental storage	    */
						/*			    */
{						/****************************/
REG	BYTE	*t1;				/* Temporary		    */
	if(incr & 1)				/* Disallow odd incr's	    */
		incr++;				/* Round up to next	    */
						/*			    */
	t1 = _break + incr;			/* New break value	    */
						/****************************/
	if(brk(t1) == FAILURE)			/* Allocate		    */
		return(FAILURE);		/*	Can't		    */
						/****************************/
	t1 = _break;				/* Save old break	    */
	_break += incr;				/* Set new break	    */
	return(t1);				/* And return		    */
}						/****************************/
