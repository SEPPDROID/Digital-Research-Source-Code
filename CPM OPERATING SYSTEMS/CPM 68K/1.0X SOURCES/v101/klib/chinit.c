/*****************************************************************************
*
*		     C C B   I N I T I A L I Z A T I O N
*		     -----------------------------------
*
*	Routine "_chinit" is called from the run-time initialization to clear
*	out all the CCB storage.
*
*	Calling sequence:
*
*		_chinit();
*
*****************************************************************************/
#include	"stdio.h"			/* Include std definitions  */
#include	"cpm.h"				/* Include CP/M definitions */
FILE		 _iob[16];			/* Allocate CCB storage	    */
_chinit()					/****************************/
{						/*			    */
	REG WORD	i;			/*	Index		    */
	for(i=0;i<MAXFILE;i++)			/*	For all channels    */
		__chinit(i);			/*	Init iob(i);	    */
}						/****************************/

/*****************************************************************************
*
*	The __chinit routine initializes only 1 channel.
*
*****************************************************************************/

__chinit(i)
{						/****************************/
	REG FILE 	*ch;			/* -> CCB		    */
	REG BYTE	*p;			/* Byte pointer temporary   */
EXTERN	FILE		*_getc();		/* Converts fd to fp	    */
	ch =  _getc(i);				/* Load address		    */
	ch -> chan   = i;			/* Load channel byte	    */
	ch -> flags  = 0;			/* clear flag word	    */
	ch -> sector = -1;			/* Set no sector in buff    */
	ch -> offset = 0;			/* Clear file offset word   */
	ch -> hiwater = 0;			/* Init hiwater mark	    */
	ch -> fcb.drive = 0;			/* Clear drive code	    */
	p = &ch->fcb.fname[0];			/* Set to file var	    */
	while (p< &ch->fcb.ftype[3])		/* Init file name fields    */
		*p++ = ' ';			/* To spaces		    */
	while (p < &ch->fcb.record)		/* Init rest of fcb	    */
		*p++ = 0;			/* To zeros		    */
}						/****************************/

/*****************************************************************************
*
*	The _getc routine returns the address of a particular ccb.
*
*****************************************************************************/

FILE *_getc(i)					/***************************/
	REG  WORD	i;			/* input channel number    */
{						/***************************/
	extern	FILE  _iob[];			/* Declare external	   */
	return(&_iob[i]);			/* Load addr, return	   */
}						/***************************/
