/****************************************************************************/
/*									    */
/*			    R e a d   F u n c t i o n			    */
/*			    -------------------------			    */
/*									    */
/*	The read function simulates the UNIX read system call.  Any 	    */
/*	arbitrary number of bytes may be read at any arbitrary offset in    */
/*	the file.							    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = read(fd,buffer,bytes);				    */
/*									    */
/*	Where:								    */
/*									    */
/*		fd	Is an open file descriptor			    */
/*		buffer	Is the buffer address				    */
/*		bytes	Is the number of bytes to read			    */
/*									    */
/*		ret	Is the number of bytes which were actually read	    */
/*									    */
/****************************************************************************/
#include "stdio.h"
#include "cpm.h"

WORD	read(fd,buff,pbytes)			/****************************/
						/*			    */
	WORD	fd;				/* File descriptor	    */
REG	BYTE	*buff;				/* -> buffer start	    */
	WORD	pbytes;				/* =  byte count to read    */
						/*			    */
						/****************************/
{
						/****************************/
REG	FILE	*fp;				/* -> ccb to read from      */
EXTERN	LONG	_chkc();			/* Converts fd to fp	    */
LOCAL	LONG	ofs;				/* Offset temporary	    */
LOCAL	LONG	xbytes;				/* Bytes  temporary	    */
REG	LONG	xsector;			/* Sector temporary	    */
REG	BYTE	c;				/* Character temporary	    */
REG	BYTE	*p1;				/* Char pointer temporary   */
REG	LONG	bytes;				/* Byte convertor	    */
						/****************************/
	bytes = (LONG) pbytes;			/* Convert to long	    */
						/****************************/
	if((LONG)(fp=_chkc(fd)) == FAILURE)	/* File open & OK??	    */
		return(FAILURE);		/*			    */
	if((fp->flags & ATEOF) != 0)		/* End of file already?	    */
		return(0);			/* Yes, quit now	    */
						/*			    */
	if((fp->flags & ISLPT) != 0)		/* A wise guy??		    */
		return(FAILURE);		/* 	A wise guy!!	    */
						/****************************/
	if((fp->flags & ISTTY) != 0)		/* TTY?			    */
		return(_ttyin(buff,bytes));	/*	Yes, read 1 line    */
						/****************************/
	if((fp->flags & ISASCII) == 0)		/* ASCII??		    */
		return(_rdbin(fp,buff,bytes));	/*	No, read binary	    */
						/****************************/

/****************************************************************************/
/* Read loop for ASCII data ...						    */
/****************************************************************************/
xbytes = bytes;					/* Remember org. request    */
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
		return(FAILURE);		/* Unable to write, quit    */
	    fp->flags &= ~DIRTY;		/* Wipe dirty bit	    */
	  }					/****************************/
	  if(_blkio(fp,xsector,fp->buffer,1L,	/* Read proper sector	    */
		B_READ) != 1)			/*			    */
	  {					/*			    */
		fp->flags |= ATEOF;		/* Set EOF flag		    */
		return(xbytes-bytes);		/* Do partial read	    */
	  }
	  fp->sector = xsector;			/* Mark buffer correctly    */
	}					/****************************/
	ofs = fp->offset & (SECSIZ-1);		/* correct buffer offset    */
	p1 = &(fp->buffer[ofs]);		/* Load byte pointer	    */
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
if(fp->offset > fp->hiwater)			/* Fix up hiwater mark	    */
	fp->hiwater = fp->offset;		/* 			    */
return(xbytes);					/* Read fully satisfied	    */
}						/****************************/

/****************************************************************************/
/*									    */
/*			  R d b i n   F u n c t i o n			    */
/*			  ---------------------------			    */
/*									    */
/*	The "_rdbin" function performs a read operation for binary files.   */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = _rdbin(fp,buffer,bytes);				    */
/*									    */
/*	Where:								    */
/*		fp	-> to the open CCB				    */
/*		buffer	-> the user's buffer				    */
/*		bytes	=  the number of bytes to be read		    */
/*									    */
/*		ret	=  the number of bytes actually read		    */
/*									    */
/****************************************************************************/

_rdbin(fp,buff,bytes)				/****************************/
						/*			    */
REG	FILE	*fp;				/* -> ccb for the file	    */
REG	BYTE	*buff;				/* -> buffer to receive data*/
REG	LONG	bytes;				/* =  # bytes to xfer	    */
						/*			    */
{						/****************************/
REG	BYTE	*p;				/* Temp byte pointer	    */
LOCAL	LONG	xsector;			/* Temp sector number	    */
LOCAL	LONG	nsector;			/* Multi-sector count	    */
LOCAL	LONG	xbytes;				/* byte count temp	    */
REG	LONG	i;				/* Temp index		    */
						/****************************/
	xbytes = bytes;				/* Preserve org byte cnt    */
	xsector = fp->offset >> 7;		/* Calculate starting sector*/
						/*			    */
	if((fp->offset & (SECSIZ-1)) != 0)	/* If not on boundary,	    */
	{					/*	must buffer	    */
	  if(fp->sector != xsector)		/* Do sectors match?	    */
	  {					/*			    */
	    if((fp->flags & DIRTY) != 0)	/* Buffer dirty?	    */
	    {					/* 			    */
	      if(_blkio(fp,fp->sector,		/*			    */
		fp->buffer,1L,B_WRITE) != 1)	/* Try to write old buffer  */
			return(FAILURE);	/* Can't; just quit.	    */
	      fp->flags &= ~DIRTY;		/* Buffer now clean	    */
	    }					/****************************/
	    if(_blkio(fp,xsector,fp->buffer,	/* Now read the		    */
		1L,B_READ) != 1)		/*		next sector */
			return(FAILURE);	/* Can't		    */
	    fp->sector = xsector;		/* Mark buffer 		    */
	  }					/****************************/
	  p = &fp->buffer[fp->offset&(SECSIZ-1)];/* Compute buffer offset   */
						/*			    */
	  while(p < &(fp->buffer[SECSIZ]) &&	/* Move the bytes	    */
		bytes > 0)			/*			    */
	  {					/*			    */
		*buff++ = *p++;			/* Copy data byte	    */
		bytes--;			/* Decrement byte counter   */
		fp->offset++;			/* Increment offset	    */
	  }					/****************************/
	  if(bytes == 0)			/* Zero byte count now?	    */
	  {					/*			    */
		if(fp->offset > fp->hiwater)	/* Adjust hiwater if	    */
			fp->hiwater=fp->offset;	/*		necessary   */
		return(xbytes);			/* Yes, just return	    */
	  }					/*			    */
	  xsector++;				/* Bump sector pointer	    */
	}					/****************************/

/****************************************************************************/
/* At this point, a multi-sector transfer may be accomplished.		    */
/****************************************************************************/
						/*			    */
	i = 0;					/* In case no xfer	    */
	nsector = bytes >> 7;			/* Compute number of sectors*/
	if(nsector > 0)				/* need to transfer??	    */
	   i=_blkio(fp,xsector,buff,nsector,	/* Do the		    */
		B_READ);			/* 	   transfer	    */
						/*			    */
	xsector += i;				/* Update sector counter    */
	fp->offset += i*SECSIZ;			/* Update offset	    */
	bytes  -= i*SECSIZ;			/* Update byte count	    */
	buff   += i*SECSIZ;			/* Update buffer address    */
						/*			    */
	if(fp->offset > fp->hiwater)		/* Adjust hiwater	    */
		fp->hiwater = fp->offset;	/*		  if needed */
	if (i != nsector)			/* Short read??		    */
	{					/*			    */
	  fp->flags |= ATEOF;			/* set EOF Flag		    */
	  return(xbytes - bytes);		/* Return actual read count */
	}					/****************************/
	if(bytes == 0)				/* Done?		    */
		return(xbytes);			/* 	 Yes, return	    */
						/****************************/
	if((fp->flags & DIRTY) != 0)		/* Buffer dirty?	    */
	{					/* 			    */
	  if(_blkio(fp,fp->sector,		/*			    */
		fp->buffer,1L,B_WRITE) != 1)	/* Try to write old buffer  */
			return(FAILURE);	/* Can't; just quit.	    */
	  fp->flags &= ~DIRTY;			/* Clear dirty bit	    */
	}					/****************************/
	if(_blkio(fp,xsector,fp->buffer,1L,	/* Read into buffer	    */
		B_READ) != 1)			/*			    */
	{					/* 			    */
		fp->flags |= ATEOF;		/* End of file		    */
		return(xbytes-bytes);		/* return corrected count   */
	}					/****************************/
						/*			    */
	fp->offset += bytes;			/* Increment offset	    */
	if(fp->offset > fp->hiwater)		/* Adjust high water mark   */
		fp->hiwater = fp->offset;	/*	if necessary	    */
	p = &fp->buffer[0];			/* Start with 1st byte	    */
	while (bytes > 0)			/* Count down		    */
	{					/*		bytes	    */
		*buff++ = *p++;			/* Move a byte		    */
		bytes--;			/*	Down count	    */
	}					/****************************/
	return(xbytes);				/* Return requested length  */
}						/****************************/

/****************************************************************************/
/*									    */
/*			  _ t t y i n   F u n c t i o n			    */
/*			  -----------------------------			    */
/*									    */
/*	Function "_ttyin" is used to read a line from the terminal.  	    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = _ttyin(buffer,bytes);				    */
/*									    */
/*	Where:								    */
/*									    */
/*		buffer	-> the user's input buffer			    */
/*		bytes	=  the (maximum) number of bytes to read	    */
/*									    */
/*		ret 	=  the number of bytes actually read		    */
/*									    */
/****************************************************************************/

WORD	_ttyin(buff,bytes)			/****************************/
						/*			    */
REG	BYTE	*buff;				/* -> user's buffer	    */
REG	LONG	bytes;				/* =  requested byte count  */
						/*			    */
{						/****************************/
LOCAL	BYTE	ttybuf[257];			/* Biggest line from tty    */
REG	LONG	xbytes;				/* Returned byte count	    */
REG	BYTE	*p;				/* Temp pointer		    */
						/*			    */
	ttybuf[0] = 255;			/* Max # bytes		    */
	__BDOS(CONBUF,&ttybuf[0]);		/* Read line from BDOS	    */
	__BDOS(CONOUT,(LONG)'\n');		/* Pop a line feed	    */
						/****************************/
	xbytes = ttybuf[1] & 0xffL;		/* # characters read	    */
	p = &ttybuf[2];				/* p -> First character	    */
	while( bytes > 0 && xbytes > 0)		/* 			    */
	{					/* Copy 1 byte / time	    */
		*buff++ = *p++;			/*			    */
		bytes--;			/* Decrement request	    */
		xbytes--;			/* Decrement bytes in buff  */
	}					/****************************/
	if(bytes > 0)				/* Terminate on count?	    */
	{					/*			    */
		*buff++ = '\n';			/* No, plug in newline	    */
		p++;				/* Bump pointer		    */
	}					/****************************/
	return(p-&ttybuf[2]);			/* Return # bytes moved	    */
}						/****************************/
