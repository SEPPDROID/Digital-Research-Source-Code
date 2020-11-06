/****************************************************************************/
/*									    */
/*			     s b r k   F u n c t i o n			    */
/*			     -------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.       */
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
#include <portab.h>
EXTERN	BYTE	*_break;			/* Old break address	    */
						/****************************/
BYTE	*sbrk(incr)				/*			    */
	WORD	incr;				/* Incremental storage	    */
						/*			    */
{						/****************************/
REG	BYTE	*t1,*t2;			/* Temporary		    */
REG	LONG	inc;				/* Temp size increment	    */
	inc = ((LONG)incr) & 0xffffL;		/* Un sign-extend	    */
	if(inc & 1)				/* Disallow odd incr's	    */
		inc++;				/* Round up to next	    */
						/*			    */
	t1 = _break;				/* Save the old break	    */
	t2 = _break + inc;			/* New break value	    */
						/****************************/
	if(brk(t2) == FAILURE)			/* Allocate		    */
		return(FAILURE);		/*	Can't		    */
						/****************************/
	return(t1);				/* And return		    */
}						/****************************/
