/****************************************************************************/
/*									    */
/*			   _ b l k i o   F u n c t i o n		    */
/*			   -----------------------------		    */
/*									    */
/*	The _blkio function is used to read / write a set of contiguous	    */
/*	sectors in an open file.					    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = _blkio(ccb,sector,buffer,count,bdosfunc);		    */
/*									    */
/*	Where:								    */
/*		ccb	-> the open file channel control block		    */
/*		sector	=  The desired starting sector number		    */
/*		buffer  =  The memory address to begin transfer		    */
/*		count	=  the number of sectors to read/write		    */
/*		bdosfunc=  The BDOS function number (B_READ / B_WRITE)	    */
/*									    */
/*									    */
/****************************************************************************/
#include	"stdio.h"
#include	"cpm.h"
LONG	_blkio(ccb,sector,buffer,count,bdosfunc)
						/****************************/
	REG  FILE	*ccb;			/* open file pointer	    */
	REG  LONG	sector;			/* Sector number	    */
	REG  BYTE	*buffer;		/* User's buffer area	    */
	REG  LONG	count;			/* # of sectors to do	    */
	REG  int	bdosfunc;		/* BDOS function to use	    */
						/****************************/

{						/****************************/
	REG  WORD	temp;			/* Temp return from BDOS    */
	REG  LONG	seccnt;			/* # sectors processed	    */
						/****************************/
	seccnt = 0;				/* Zero count initally	    */
						/*			    */
	while (count > 0)			/* Until all sectors xferred*/
	{					/*			    */
		__BDOS(SETDMA,buffer);		/* Set new DMA address	    */
		ccb->fcb.record = sector;	/* Set new record number    */
		temp=__BDOS(bdosfunc,&(ccb->fcb));/* do the read	    */
		if(temp != 0)			/* Check for errors	    */
		{				/****************************/
			return(seccnt);		/* Return nfg		    */
		}				/****************************/
						/*			    */
		sector++;			/* Increment sector number  */
		count--;			/* Down count		    */
		buffer += SECSIZ;		/* Increment address	    */
		seccnt++;			/* Bump sector count	    */
	}					/****************************/
	return(seccnt);				/* All is OK		    */
}						/****************************/
