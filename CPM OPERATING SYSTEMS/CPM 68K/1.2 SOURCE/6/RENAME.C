/***************************************************************************
 *									    
 *			r e n a m e   F u n c t i o n			    
 *			-----------------------------			    
 *	Copyright 1983 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	'rename()' changes a file's name.				    
 *									    
 *	Calling sequence:						    
 *		ret = rename(from,to)					    
 *									    
 *	Where:								    
 *		from -> a NULL terminated (existing) file name		    
 *		to -> the NULL terminated new file name			    
 *		ret = SUCCESS if everything ok, FAILURE o.w.		    
 *									    
 ****************************************************************************/

#include "portab.h"
#include "osif.h"
#include "osiferr.h"
#include "errno.h"

WORD	rename(from,to)				/* CLEAR FUNCTION ***********/
	BYTE 	*from;				/* Existing file name	    */
	BYTE 	*to;				/* New file name	    */
{
	BYTE fcbbuf[32+16];			/* Extra large fcb	    */
  REG	struct fcbtab *fcbp;			/* FCB pointer		    */
  REG	WORD nuser;				/* new user #		    */
  REG	WORD xuser;				/* system user #	    */
	WORD rv;				/* Return value		    */

	fcbp = fcbbuf;				/* Point fcb to start	    */
	if( FAILURE == _parsefn(from,fcbp))	/* Parse 'from' into fcb    */
		RETERR(FAILURE,EINVAL);		/*    Bad filename	    */
#if CPM						/*--------------------------*/
	nuser = fcbp->fuser;			/* Save specified User #    */
#endif						/*--------------------------*/
	fcbp = &(fcbbuf[16]);			/* F_RENAME needs it 	    */
	if( FAILURE == _parsefn(to,fcbp))	/* Parse 'to' to fcb[16]    */
		RETERR(FAILURE,EINVAL);		/*    Bad filename	    */
#if CPM						/*--------------------------*/
	if( fcbp->fuser )			/* user # specified?	    */
	{					/*			    */
		if(nuser && nuser!=fcbp->fuser)	/* Differing User #s?	    */
			RETERR(FAILURE,EINVAL);	/*   that's a no-no	    */
		nuser = fcbp->fuser;		/* Use this user #	    */
	}					/****			    */
	xuser = _chkuser(nuser);		/* Change user # if needed  */
#endif						/*--------------------------*/
						/****************************/
	rv = __OSIF(F_RENAME,fcbbuf);		/* Use special fcb	    */
						/****************************/
#if CPM						/*--------------------------*/
	_uchkuser(nuser,xuser);			/* Change back if needed    */
#endif						/*--------------------------*/
	if( rv != 0 )				/* Did it work?		    */
		RETERR(FAILURE,ERENAME);	/*   no...		    */
	return SUCCESS;				/* It worked!		    */
}						/****************************/
..		    */
	return SUCCESS;				/* It worked!		    */
}						/****************************/
..		    */
	return SUCCESS;				/* It worked!		    */
}						/****************************/
