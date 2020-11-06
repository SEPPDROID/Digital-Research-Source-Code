
/****************************************************************
*								*
*		P-CP/M BDOS Disk Utilities Module		*
*								*
*	This module contains the miscellaneous utilities  	*
*	for manipulating the disk in P-CP/M.  Included are:	*
*								*
*		dirscan()   - general purpose dir scanning	*
*		setaloc()   - set bit in allocation vector	*
*		clraloc()   - clear bit in allocation vector	*
*		getaloc()   - get free allocation block		*
*		dchksum()   - directory checksum calculator	*
*		dir_rd()    - read directory sector		*
*		dir_wr()    - write directory sector		*
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
EXTERN UWORD	error();	/* external error routine	*/

EXTERN UWORD	log_dsk;	/* logged-on disk vector */
EXTERN UWORD	ro_dsk;		/* read-only disk vector */
EXTERN UWORD	crit_dsk;	/* critical disk vector */


/**********************
* read/write routine  *
**********************/

UWORD rdwrt(secnum, dma, parm)
/* General disk sector read/write routine */
/* It simply sets up a I/O packet and sends it to do_phio */

LONG	secnum;			/* logical sector number to read/write */
XADDR	dma;			/* dma address				*/
REG WORD parm;			/* 0 for read, write parm + 1 for write */

{
    struct iopb	rwpkt;
    BSETUP

    rwpkt.devnum = GBL.curdsk;		/* disk to read/write	*/
    if (parm)
    {
	rwpkt.iofcn = (BYTE)write; /* if parm non-zero, we're doing a write */
	rwpkt.ioflags = (BYTE)(parm-1);	/* pass write parm	*/
        if ( ro_dsk & (1 << (rwpkt.devnum)) ) error(4);
				/* don't write on read-only disk	*/
    }
    else
    {
	rwpkt.iofcn = (BYTE)read;
	rwpkt.ioflags = (BYTE)0;
    }
    rwpkt.devadr = secnum;			/* sector number	*/
    rwpkt.xferadr = dma;			/* dma address		*/

/*		parameters that are currently not used by do_phio
    rwpkt.devtype = disk;
    rwpkt.xferlen = 1;
				*/
    rwpkt.infop = GBL.dphp;			/* pass ptr to dph	*/
    while ( do_phio(&rwpkt) )
	if ( error( parm ? 1 : 0 ) ) break;
    return(0);	
}


/***************************
*  directory read routine  *
***************************/

UWORD dir_rd(secnum)

UWORD secnum;
{
    BSETUP

    return( rdwrt((LONG)secnum, map_adr((XADDR)GBL.dirbufp, 0), 0) );
}


/****************************
*  directory write routine  *
****************************/

UWORD dir_wr(secnum)

REG WORD secnum;
{
    REG UWORD rtn;
    UBYTE dchksum();
    BSETUP

    rtn = rdwrt( (LONG)secnum, map_adr((XADDR)GBL.dirbufp, 0), 2);
    if ( secnum < (GBL.parmp)->cks )
	*((GBL.dphp)->csv + secnum) = dchksum();
    return(rtn);
}


/*******************************
*  directory checksum routine  *
*******************************/

UBYTE dchksum()
/* Compute checksum over one directory sector */
/* Note that this implementation is dependant on the representation */
/*   of a LONG and is therefore not very portable.  But it's fast   */
{
    REG LONG	*p;		/* local temp variables */
    REG LONG	lsum;
    REG WORD	i;

    BSETUP

    p = (LONG *)GBL.dirbufp;	/* point to directory buffer */
    lsum = 0;
    i = SECLEN / (sizeof lsum);
    do
    {
	lsum += *p++;		/* add next 4 bytes of directory */
	i -= 1;
    } while (i);
    lsum += (lsum >> 16);
    lsum += (lsum >> 8);
    return( (UBYTE)(lsum & 0xff) );
}


/************************
*  dirscan entry point	*
************************/

UWORD dirscan(funcp, fcbp, parms)

BOOLEAN (*funcp)();		/* funcp is a pointer to a Boolean function */
REG struct fcb *fcbp;		/* fcbp is a pointer to a fcb */
REG UWORD parms;		/* parms is 16 bit set of bit parameters */

/* Parms & 1  = 0 to start at beginning of dir, 1 to continue from last */
/* Parms & 2  = 0 to stop when *funcp is true, 1 to go until end	*/
/* Parms & 4  = 0 to check the dir checksum, 1 to store new checksum	*/
/* Parms & 8  = 0 to stop at hiwater, 1 to go until end of directory	*/

#define continu 1
#define full	 2
#define initckv  4
#define pasthw   8

{
    REG UWORD 	i;		/* loop counter		*/
    REG struct dpb *dparmp;	/* pointer to disk parm block */
    REG UWORD 	dirsec;		/* sector number we're working on */
    REG UWORD  	rtn;		/* return value		*/
    REG UBYTE	*p;		/* scratch pointer	*/
    REG UWORD	bitvec;		/* disk nmbr represented as a vector */

    BSETUP

    dparmp = GBL.parmp;			/* init ptr to dpb */
    rtn  = 255;				/* assume it doesn't work */

/* Sorry about this FOR loop, but the initialization terms and end test
    really do depend on the input parameters, so......			*/
    for ( i = ( (parms & continu) ? GBL.srchpos + 1 : 0);
	  i <= ( (parms & pasthw) ? (dparmp->drm) : (GBL.dphp)->hiwater );
	  i++ )
    {				/* main directory scanning loop		*/
	if ( ! (i & 3) )
	{			/* inside loop happens when we need to 
				   read another directory sector	*/
retry:	    dirsec = i >> 2;
	    dir_rd(dirsec);	/* read the directory sector	*/
	    if ( dirsec < (dparmp->cks) )  /* checksumming on this sector? */
	    {
		p = ((GBL.dphp)->csv) + dirsec;
					/* point to checksum vector byte  */
		if (parms & initckv) *p = dchksum();
		else if (*p != dchksum())
		{			/* checksum error! */
		    (GBL.dphp)->hiwater = dparmp->drm;  /* reset hi water */
		    bitvec = 1 << (GBL.curdsk);
		    if (crit_dsk & bitvec)	/* if disk in critical mode */
			ro_dsk |= bitvec;	/* then set it to r/o	*/
		    else
		    {
			log_dsk &= ~bitvec;	/* else log it off  */
			seldsk(GBL.curdsk);	/* and re-select it */
			goto retry;		/* and re-do current op */
		    }
		}
	    }
	}

	GBL.srchpos = i;
	if ( (*funcp)(fcbp, (GBL.dirbufp) + (i&3), i) )
			/* call function with parms of (1) fcb ptr,
			   (2) pointer to directory entry, and
		   	   (3) directory index		  	*/
	{
	    if (parms & full) rtn = 0;	/* found a match, but keep going */
	    else return(i & 3);		/* return directory code	*/
	}
    }
    return(rtn);
}


/****************************************
*  Routines to manage allocation vector *
*	setaloc()			*
*	clraloc()			*
*	getaloc()			*
****************************************/

setaloc(bitnum)
/*  Set bit in allocation vector	*/
REG UWORD	bitnum;
{
    BSETUP

    *((GBL.dphp)->alv + (bitnum>>3)) |= 0x80 >> (bitnum & 7);
}


clraloc(bitnum)
/* Clear bit in allocation vector	*/
REG UWORD	bitnum;
{
    BSETUP

    if (bitnum)
	*((GBL.dphp)->alv + (bitnum>>3)) &= ~(0x80 >> (bitnum & 7));
}


UWORD	getaloc()
/* Get a free block in the file system and set the bit in allocation vector */
{
    REG UWORD	i;		/* loop counter		*/
    REG WORD	diskmax;	/* # bits in alv - 1	*/
    REG UBYTE	*p;		/* ptr to byte */

    BSETUP
    LOCK		/* need to lock the file system while messing
			   with the allocation vector		*/

    diskmax = (GBL.parmp)->dsm;
			/* get disk max field from dpb	*/
    p = (GBL.dphp)->alv;
    for (i = 0; i <= diskmax; i++)
    {
	if ( ~(*(p + (i >> 3))) & (0x80 >> (i&7)) )
	{			/* found a zero in allocation vector */
	    setaloc(i);
	    UNLOCK		/* can unlock file system now	*/
	    return(i);		/* return block number		*/
	}
    }
    return(~0);			/* if no free block found, return -1 */
}
