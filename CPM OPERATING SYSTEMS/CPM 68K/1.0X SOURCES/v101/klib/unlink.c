/****************************************************************************/
/*									    */
/*			U n l i n k   F u n c t i o n			    */
/*			-----------------------------			    */
/*									    */
/*	The unlink function is used to delete a CP/M file by name.	    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = unlink(filename);					    */
/*									    */
/*	Where:								    */
/*		filename	Is the null-terminated name of the file	    */
/*									    */
/*		ret		Is 0 for success, -1 for failure	    */
/*									    */
/****************************************************************************/
#include "stdio.h"
#include "cpm.h"
WORD	unlink(filename)
						/****************************/
	BYTE	*filename;			/* -> filename	 	    */
{						/****************************/
REG	WORD	ch;				/* Channel number	    */
REG	WORD	ret;				/* Temp return value	    */
						/****************************/
	if((ch=igetc()) < 0)			/* Allocate a channel	    */
		return(FAILURE);		/* Can't		    */
						/*			    */
	__chinit(ch);				/* Init fcb and ccb	    */
	ret = _open(ch,filename,DELETE);	/* Delete the file	    */
	freec(ch);				/* Free the channel	    */
	return(ret);				/* Return result of DELETE  */
}						/****************************/

