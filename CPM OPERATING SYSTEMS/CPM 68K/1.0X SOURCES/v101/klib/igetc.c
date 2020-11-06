/*****************************************************************************
*
*		I G E T C   /   F R E E C   R O U T I N E S
*		-------------------------------------------
*
*	Routines "igetc" and "freec" are used to allocate / deallocate a 
*	channel number so that the user may do I/O directly without the OTS
*	getting in the way.
*
*	Calling Sequence:
*
*		i = getc();
*		freec(i);
*
*****************************************************************************/
#include	"stdio.h"			/* Include std definitions  */
#include	"cpm.h"				/* Also CP/M ones	    */
WORD		chmask	= 0;			/* Allocate storage	    */
WORD	igetc()					/****************************/
{						/*			    */
	REG	i,j;				/* Define 2 temporaries	    */
						/*			    */
	j = 1;					/* Start with channel 0	    */
	for(i=0;i<MAXFILE;i++)			/* Look at all 16 bits	    */
	{					/*			    */
	  if((j & chmask) == 0)			/* If 0, then channel free  */
	  {					/*			    */
		chmask |= j;			/* set allocated bit	    */
		return(i);			/* and return the channel # */
	  }					/*			    */
	  j = j << 1;				/* Up to next bit	    */
	}					/* End FOR loop		    */
	return(FAILURE);			/* All channels in use!	    */
}						/****************************/

WORD	freec(ch)				/****************************/
{						/*			    */
	REG	i,j;				/* Define 2 temporaries	    */
						/*			    */
	j = 1;					/* Start with channel 0	    */
	for (i=0; i<ch; i++)			/* Compute		    */
		j = j << 1;			/*	    bit mask	    */
	chmask &= (~j);				/* clear allocation bit	    */
	return(SUCCESS);			/* Return OK		    */
}						/****************************/
