
/****************************************************************
*								*
*	    CP/M-Z8K Loader BDOS Disk Utilities Module		*
*								*
*	This module contains the miscellaneous utilities  	*
*	for manipulating the disk in P-CP/M.  Included are:	*
*								*
*		dirscan()   - general purpose dir scanning	*
*		dir_rd()    - read directory sector		*
*		rdwrt()	    - read/write disk sector		*
*								*
*								*
*	Modified for memory management on Z8000			*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "pktio.h"		/* Packet I/O definitions */

#include "biosdef.h"		/* Bios & mem mapperinterface */


/* declare external functions and variables */
EXTERN UWORD	do_phio();	/* external physical disk I/O routine */

/****************
* read routine  *
****************/

UWORD rdsec(secnum, dma)
/* General disk sector read routine */
/* It simply sets up a I/O packet and sends it to do_phio */

LONG	secnum;			/* logical sector number to read/write */
XADDR	dma;			/* dma address				*/

{
    struct iopb	rwpkt;
    BSETUP

    rwpkt.devnum = GBL.curdsk;		/* disk to read/write	*/
    rwpkt.iofcn = (BYTE)read;
    rwpkt.ioflags = (BYTE)0;
    rwpkt.devadr = secnum;			/* sector number	*/
    rwpkt.xferadr = dma;			/* dma address		*/

/*		parameters that are currently not used by do_phio
    rwpkt.devtype = disk;
    rwpkt.xferlen = 1;
				*/
    rwpkt.infop = GBL.dphp;			/* pass ptr to dph	*/
    return ( do_phio(&rwpkt) );
}


/***************************
*  directory read routine  *
***************************/

UWORD dir_rd(secnum)

UWORD secnum;
{
    BSETUP

    return( rdsec((LONG)secnum, map_adr((XADDR)GBL.dirbufp, 0) ) );
}


/************************
*  dirscan entry point	*
************************/

UWORD dirscan(funcp, fcbp)

BOOLEAN (*funcp)();		/* funcp is a pointer to a Boolean function */
REG struct fcb *fcbp;		/* fcbp is a pointer to a fcb */

{
    REG UWORD 	i;		/* loop counter		*/
    REG struct dpb *dparmp;	/* pointer to disk parm block */
    REG UWORD 	dirsec;		/* sector number we're working on */

    BSETUP

    dparmp = GBL.parmp;			/* init ptr to dpb */

    for ( i = 0; i <= dparmp->drm; i++ )
    {				/* main directory scanning loop		*/
	if ( ! (i & 3) )
	{			/* inside loop happens when we need to 
				   read another directory sector	*/
	    dirsec = i >> 2;
	    if( dir_rd(dirsec))	/* read the directory sector	*/
		return(-1);
	}
	if ( (*funcp)(fcbp, (GBL.dirbufp) + (i&3), i) )
			/* call function with parms of (1) fcb ptr,
			   (2) pointer to directory entry, and
		   	   (3) directory index		  	*/
	    return(i & 3);		/* return directory code	*/
    }
    return(255);
}
