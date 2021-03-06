/***************************************************************************
*									    
*			  R d a s c   F u n c t i o n			    
*			  ---------------------------			    
*									    
*	The "_rdasc" function performs a read operation for ascii  files.   
*	All Return chars (0x13) are eliminated (and not counted) from input.
*									    
*	Calling Sequence:						    
*		ret = _rdasc(fp,buffer,bytes);				    
*									    
*	Where:								    
*		fp	-> to the open CCB				    
*		buffer	-> the user's buffer				    
*		bytes	=  the number of bytes to be read		    
*		ret	=  the number of bytes actually read		    
*
*	Edits:
*	20-jan-84 whf	Handle PCDOS ^Z bug
****************************************************************************/

#include "portab.h"
#include "osif.h"
#include "osiferr.h"
#include "errno.h"

UWORD _rdasc(fp,buff,bytes)			/****************************/
						/*			    */
REG	FD	*fp;				/* -> ccb for the file	    */
REG	BYTE	*buff;				/* -> buffer to receive data*/
REG	LONG	bytes;				/* =  # bytes to xfer	    */
{						/****************************/


#if CPM    /*===============================================================*/
REG	BYTE	*p1;				/* Temp byte pointer	    */
	BYTE	c;				/* Temp char		    */
	LONG	xsector;			/* Temp sector number	    */
	LONG	xbytes;				/* byte count temp	    */
						/****************************/
    xbytes = bytes;				/* Remember org. request    */
    while(bytes > 0)				/* Until read is satisfied  */
    {						/****************************/
	xsector = fp->offset >> 7;		/* Calc starting sector	    */
	if(xsector != fp->sector)		/* Match sector in buffer?? */
	{					/* No, must read first	    */
						/****************************/
	  if((fp->flags & DIRTY) != 0)		/* Buffer dirty??	    */
	  {					/* Yes, must write it 	    */
	    if(_blkio(fp,fp->sector,fp->buffer,	/* Try to write buffer	    */
	        1L,B_WRITE) != 1)		/* 			    */
		RETERR(FAILURE,EIO);		/* Unable to write, quit    */
	    fp->flags &= ~DIRTY;		/* Wipe dirty bit	    */
	  }					/****************************/
	  if(_blkio(fp,xsector,fp->buffer,1L,	/* Read proper sector	    */
		B_READ) != 1)			/*			    */
	  {					/* Assume no sparse ascii fs*/
		fp->flags |= ATEOF;		/* Set EOF flag		    */
		return(xbytes-bytes);		/* Do partial read	    */
	  }
	  fp->sector = xsector;			/* Mark buffer correctly    */
	}					/****************************/
	p1 = &(fp->buffer[(int)fp->offset&(SECSIZ-1)]); /* Load byte pointer*/
						/*			    */
	while(p1 < &(fp->buffer[SECSIZ]) && 	/* Ie, more data in buffer  */
	      bytes > 0)			/* And request not satisfied*/
	{					/*			    */
	  c = *p1;				/* Pick up next character   */
	  if(c==EOFCHAR)			/* ^Z??			    */
	  {					/* Yes,			    */
	    fp->flags |= ATEOF;			/*	Set EOF flag	    */
	    if(fp->offset > fp->hiwater)	/* set hiwater mark	    */
		fp->hiwater = fp->offset;	/*     if necessary	    */
	    return(xbytes-bytes);		/* Return number read	    */
	  }					/****************************/
	  else if (c == '\r')			/* Carriage return?	    */
	  {					/*			    */
		p1++;				/* Yes, just ignore that    */
		fp->offset++;			/* Increment file offset    */
	  }					/*			    */
	  else					/****************************/
	  {					/* Regular character	    */
	    *buff++ = c;			/* Load buffer with byte    */
	    bytes--;				/* Decrement count	    */
	    p1++;				/* increment counter	    */
	    fp->offset++;			/* Increment file offset    */
	  }					/****************************/
	}					/*			    */
    }						/* While bytes > 0	    */
#endif     /*===============================================================*/

#if PCDOS  /*===============================================================*/
	LONG	xbytes;				/* byte count save	    */
	UWORD	ii, jj;				/* More byte count temps    */
	UWORD	_cr_collapse();			/* Zaps CRs in buff	    */
						/****************************/
    xbytes = bytes;				/* Remember org. request    */
    while(bytes > 0  &&  (fp->flags&ATEOF)==0 )	/* Until read is satisfied  */
    {						/****************************/
	ii = _pc_readblk(&(fp->fcb),fp->offset,buff,(WORD)bytes,1); /*	    */
	for( jj=0; jj<ii; ++jj ) 		/* EOF scan		    */
	    if( buff[jj] == EOFCHAR ) break;	/* Last char EOF?	    */
	if( ii==0  ||  buff[jj] == EOFCHAR ) {	/* No chars or Last char EOF?*/
		ii = jj;			/* Num valid chars	    */
		fp->flags |= ATEOF;		/* Yes, mark file	    */
	}					/****			    */
	jj = _cr_collapse(buff,ii);		/* Stomp out Returns (0x13) */
	fp->offset += ii;			/* Calculate new offset	    */
	buff += jj;				/* advance the buffer ptr   */
	bytes -= jj;				/* discount by bytes in buff*/
    }						/* While bytes > 0	    */
    xbytes -= bytes;				/* Number really read	    */
#endif     /*===============================================================*/


    if(fp->offset > fp->hiwater)		/* Fix up hiwater mark	    */
	fp->hiwater = fp->offset;		/* 			    */
    return(xbytes);				/* Read fully satisfied	    */
}						/****************************/




#if PCDOS  /*===============================================================*/
						/****************************/
UWORD _cr_collapse(buf,cnt)			/* Stomp out Returns (0x13) */
REG BYTE *buf;					/* Where to stomp	    */
    UWORD cnt;					/* How much of buf to stomp */
{						/****************************/
REG	BYTE *chk;				/* Check pointer	    */
	BYTE *savbuf;				/* Save pointer		    */
	BYTE *endbuf;				/* Move to pointer	    */
						/*			    */
	savbuf = buf;				/* save for return val calc */
	endbuf = buf + cnt;			/* after last char in buf   */
	for( chk = buf; chk < endbuf; chk++ ) 	/* Check out entire buffer  */
	    if( *chk != '\r' ) 			/* if NOT a Return:	    */
		*buf++ = *chk;			/*   move it down	    */
	return buf-savbuf;			/* New num chars in buf	    */
}						/****************************/
#endif     /*===============================================================*/

=====================================*/

=====================================*/

