/****************************************************************************/
/*									    */
/*			U n l i n k   F u n c t i o n			    */
/*			-----------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
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
#include <portab.h>
#include <cpm.h>
WORD	unlink(filename)
						/****************************/
	BYTE	*filename;			/* -> filename	 	    */
{						/****************************/
REG	WORD	ch;				/* Channel number	    */
REG	WORD	ret;				/* Temp return value	    */
						/****************************/
	if((ch=allocc()) < 0)			/* Allocate a channel	    */
		return(FAILURE);		/* Can't		    */
						/*			    */
	__chinit(ch);				/* Init fcb and ccb	    */
	ret = __open(ch,filename,DELETE);	/* Delete the file	    */
	freec(ch);				/* Free the channel	    */
	return(ret);				/* Return result of DELETE  */
}						/****************************/

