/***************************************************************************
 *									    
 *			W r t c h r    F u n c t i o n			    
 *			------------------------------			    
 *									    
 *	Function "wrtchr" is called from "write" to handle character 	    
 *	oriented devices: TTY, LPT, and QUE.				    
 *									    
 *	Calling sequence:						    
 *		ret = _wrtchr(fp,buffer,bytes);				    
 *									    
 *	Where:								    
 *		fp	Points to the affected ccb.			    
 *		buffer	Is the buffer address				    
 *		bytes	Is the number of bytes to write			    
 *									    
 *		ret	Is the number of bytes actually written		    
 *									    
 ****************************************************************************/

#include "portab.h"
#include "osif.h"
#include "sgtty.h"

UWORD	_wrtchr(fp,buf,bytes)			/****************************/
						/*			    */
	FD	*fp;				/* -> CCB		    */
	BYTE	*buf;				/* -> User's buffer	    */
	UWORD	bytes;				/* # bytes to write	    */
{						/****************************/
	UWORD	nbs;				/* num bytes remaining	    */
	WORD	ii;				/* local counter	    */
	BYTE	cp[SECSIZ];			/* ptr to local buffer	    */
	WORD	col;				/* column counter	    */
	WORD	nsp;				/* temp counter		    */
	int	(*fnout)();			/* func placeholder	    */
	int	_ttyout(), _lstout();		/* device specific output fn*/
	int	DoAscii;			/* Flag: xlate newlines?    */
	int	DoXTabs;			/* Flag: xpand tabs?	    */
	struct sgttyb *tyb;			/* Special TTY ptr	    */
						/*			    */
	DoAscii = TRUE;				/* Default: xlate newlines  */
	DoXTabs = FALSE;			/* Default: no expand tabs  */
	if( fp->flags & ISTTY )			/* If TTY Output	    */
		fnout = _ttyout;		/*   use that function	    */
	else if( fp->flags & ISLPT )		/* If LST Output	    */
		fnout = _lstout;		/*   use this function	    */
#if CPM3
	else if( fp->flags & ISQUE )
	{
						/* Maybe change DoAscii here*/
	}
#endif
	if( fp->flags & ISSPTTY )		/* Special Output handling? */
	{					/*   yes...		    */
		tyb = &(fp->fcb);		/* assume info stored here  */
		DoXTabs = (tyb->sg_flags) & XTABS;
	}					/*			    */
						/****************************/
	col = 0;				/* Assume we're at start of */
						/*   line: not really kosher*/
	for( nbs=bytes; nbs != 0; )		/* For all the bytes 	    */
	{					/****************************/
	    for( ii=1; ii<SECSIZ  &&  nbs!=0; )	/* Not too full & chars left*/
	    {					/*			    */
		if( *buf == '\n' )		/* Newline char?	    */
		{				/* yes...		    */
		  col = -1;			/* set to zero when incr'd  */
		  if( DoAscii )			/* Newline needs xlation?   */
		  {				/*   yes...		    */
		    if( ii+1 >= SECSIZ ) break;	/* Buf too full, next time  */
		    cp[ii] = '\r';		/* Preceed with Return char */
		    ++ii;			/* Incr num in buffer	    */
		  }				/*			    */
		} else				/*****			    */
		if( *buf == '\t'  &&  DoXTabs )	/* Expand this tab char?    */
		{				/*   yes...		    */
		    nsp = 8-(col&7);		/* calc number spaces	    */
		    if( ii+nsp >= SECSIZ ) break; /* Buf too full, next time*/
		    col += nsp;			/*			    */
		    while( nsp-- )		/* for all the spaces	    */
		    {				/*			    */
			cp[ii] = ' '; ++ii;	/*  put them in buf	    */
		    }				/*			    */
		    ++buf;			/* Skip over tab	    */
		    continue;			/*			    */
		}				/*****			    */
		cp[ii] = *buf++;		/* Xfer char to buffer	    */
		++ii; --nbs; ++col;		/* Handle counters	    */
	    }					/******			    */
	    cp[0] = ii-1;			/* Num chars output	    */
	    (*fnout)(cp);			/* go do the output	    */
	}					/****************************/
	return bytes;				/* return what they gave us */
}						/****************************/
********************/
********************/
