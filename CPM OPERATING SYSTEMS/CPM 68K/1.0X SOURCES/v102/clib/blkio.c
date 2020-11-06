/****************************************************************************/
/*									    */
/*			   _ b l k i o   F u n c t i o n		    */
/*			   -----------------------------		    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.       */
/*									    */
/*	The _blkio function is used to read / write a set of contiguous	    */
/*	sectors in an open file.					    */
/*	Note: it will use CP/M function 44 (Set Multi-Sector Count) if	    */
/*	MAXSXFR > 1.  This is for BDOS version 3.0 or greater.		    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = _blkio(ccb,sector,buffer,count,bdosfunc);		    */
/*									    */
/*	Where:								    */
/*		ret	=  sectors actually read/written		    */
/*		ccb	-> the open file channel control block		    */
/*		sector	=  The desired starting sector number		    */
/*		buffer  =  The memory address to begin transfer		    */
/*		count	=  the number of sectors to read/write		    */
/*		bdosfunc=  The BDOS function number (B_READ / B_WRITE)	    */
/*									    */
/*									    */
/****************************************************************************/
#include	<portab.h>
#include	<cpm.h>
	LONG
_blkio(ccb,sector,buffer,count,bdosfunc)
						/****************************/
	REG  FD		*ccb;			/* open file pointer	    */
	REG  LONG	sector;			/* Sector number	    */
	REG  BYTE	*buffer;		/* User's buffer area	    */
	REG  LONG	count;			/* # of sectors to do	    */
	REG  int	bdosfunc;		/* BDOS function to use	    */
						/****************************/

{						/****************************/
	REG  WORD	nsecs;			/* # secs to xfer each time */
	REG  LONG	seccnt;			/* # sectors processed	    */
	REG  WORD	xuser;			/* old user number	    */
						/****************************/
	seccnt = 0;				/* Zero count initally	    */
	xuser = __BDOS(USER,0xFFL);		/* Get current user #	    */
	if(xuser != ccb->user)			/* File have a different 1? */
	  __BDOS(USER,(LONG)ccb->user);		/* Yes, set to that	    */
						/*			    */
	while (count > 0)			/* Until all sectors xferred*/
	{					/*			    */
		__BDOS(SETDMA,buffer);		/* Set new DMA address	    */
		ccb->fcb.record = sector;	/* Set new record number    */
#if _MAXSXFR > 1				/* Can we xfer multi-secs?  */
		nsecs = min(_MAXSXFR,count);	/* Ifso, xfer many	    */
		__BDOS(SETMSC,nsecs);		/* Always set before r/w    */
#else						/*   if possible	    */
		nsecs = 1;			/* o.w. xfer singly	    */
#endif						/*			    */
		if(__BDOS(bdosfunc,&(ccb->fcb))	/* do the read	    	    */
			!= 0)			/* Check for errors	    */
		{				/****************************/
	/*** WE SHOULD CHECK FOR SPARSE FILES HERE... ***/
		  if(nsecs>1)			/* were we trying multi-sec?*/
		    nsecs = __cpmrv >>_MAXSHFT;	/* adjust nsecs read ifso   */
		  else nsecs=0;			/* ow. assume 0 secs read   */
		  if(ccb->user != xuser)	/* Changed users?	    */
			__BDOS(USER,(LONG)xuser);/* Yes, change back	    */
		  return(seccnt+nsecs);		/* Return nfg		    */
		}				/****************************/
						/*			    */
		sector += nsecs;		/* Increment sector number  */
		count  -= nsecs;		/* Down count		    */
		seccnt += nsecs;		/* Bump sector count	    */
		buffer += nsecs*SECSIZ;		/* Increment address	    */
	}					/****************************/
	if(ccb->user != xuser)			/* Different user #'s??	    */
	  __BDOS(USER,(LONG)xuser);		/* Yes, reset to original   */
	return(seccnt);				/* All is OK		    */
}						/****************************/
