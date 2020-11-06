/****************************************************************
*								*
*		CP/M-Z8K Loader BDOS Disk Read Module		*
*								*
*	This module contains functions to perform sequential   	*
*	or random access read or write to the disk for P-CP/M	*
*								*
*	It includes the following external functions:		*
*								*
*		seqread()    - sequential and random disk I/O	*
*								*
*								*
*	Modified for memory management on the Z8000		*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */


/* External function definitions */
EXTERN	UWORD	rdsec();	/* disk read/write routine	*/
EXTERN	UWORD	swap();		/* assembly language byte swapper */
EXTERN	UWORD	dirscan();	/* directory scanning routine	*/
EXTERN	BOOLEAN openfile();	/* open file function passed to dirscan */

/**********************************************************/
/*  First, some utility functions used by seqio and ranio */
/**********************************************************/

/******************************
*  FCB block number routines  *
******************************/

WORD	blkindx(fcbp)
/*  return index into fcb disk map	*/

REG struct fcb	*fcbp;		/* pointer to fcb	*/
{
    REG struct dpb *dparmp;	/* pointer to disk parameter block */
    REG WORD i;
    REG WORD blkshf;
    BSETUP

    dparmp = GBL.parmp;
    blkshf = dparmp->bsh;
    i = ((fcbp->extent) & dparmp->exm) << (7 - blkshf);
    return (i + (UBWORD(fcbp->cur_rec) >> blkshf) );
}


UWORD	blknum(fcbp, index, wrdfcb)
/* return block number in fcb indicated by index */

REG struct fcb	*fcbp;		/* pointer to fcb	*/
REG WORD	index;		/* index into disk map of fcb */
WORD		wrdfcb;		/* boolean, fcb disk map of words? */
{
    if (wrdfcb)
	return( swap(fcbp->dskmap.big[index]) );
    else return( UBWORD(fcbp->dskmap.small[index]) );
}



/*********************
*  disk read routine *
*********************/

UWORD do_io(block, rcrd)

UWORD	block;		/* block number		*/
UBYTE	rcrd;		/* record number	*/
{
    REG LONG lsec;
    REG struct dpb *dparmp;
    BSETUP

    dparmp = GBL.parmp;		/* init dpb pointer */
    lsec = ((LONG)block << (dparmp->bsh)) +
		(LONG)(rcrd & (dparmp->blm));
    return( rdsec(lsec, GBL.dmaadr) );
}

/*******************************************
*  routine for crossing extent boundaries  *
*******************************************/

WORD new_ext(fcbp)
/*  If sequential I/O, open the next extent	*/

REG struct fcb	*fcbp;		/* pointer to fcb */
{
    REG UBYTE mod;		/* module number		*/
    REG UBYTE ext;		/* extent number		*/
    BSETUP

    mod = (fcbp->s2) & 0x3f;
    ext = (fcbp->extent) + 1;	/* for sequential, incr extent	*/
    if (ext >= 32)
    {
	ext = 0;
	mod += 1;
    }
    if (mod >= 64) return(6);	/* past maximum file size */
    if ( mod == ((fcbp->s2) & 0x3f) )
	if ( ! ((ext ^ (fcbp->extent)) & ~((GBL.parmp)->exm) & 0x1f) )
	{			/* we're in same logical extent */
	    fcbp->extent = ext;
	    return(0);
	}
    /* Extent or Module numbers don't match	*/
    /* Close the old extent and open a new one	*/
    fcbp->s2 = mod;
    fcbp->extent = ext;
    if ( dirscan(openfile, fcbp, 0) >= 255 )  /* open new extent */
	return(4);		/* reading unwritten extent */
    return(0);
}


/************************************
* Routine to calculate the maximum  *
* extent number of an FCB in a	    *
* extent-folded environment	    *
************************************/

UWORD calcext(fcbp)

REG struct fcb *fcbp;

{
    REG UWORD i;
    REG BYTE *p;
    BSETUP

    i = 15;
    p = &(fcbp->dskmap.small[16]);
    do
    {
	if (*--p) break;
	i -= 1;
    } while (i);
/* Now i contains the index of the last non-zero block in the FCB */
    if ((GBL.parmp)->dsm > 255) i >>= 1;
    i >>= 7 - ((GBL.parmp)->bsh);
    return ( (fcbp->extent) & ~((GBL.parmp)->exm) & 0x1f | i );
}


/*********************************
* Routine to get the actual	 *
* record count of the currently  *
* active logical extent of a FCB *
*********************************/

UWORD get_rc(fcbp)

REG struct fcb *fcbp;

{
    REG UWORD ext;

    ext = calcext(fcbp);	/* find last active extent in fcb */
    if (ext == fcbp->extent) return(UBWORD(fcbp->rcdcnt));
			/* if this is the last active fcb, return fcb's rc */
    else if (ext > fcbp->extent) return(128);
			/* if the fcb has more extents past this one, then */
			/* the current one is logically full	*/
    else return (0);
			/* if we seeked past the last active extent, rc = 0 */
}


/************************
*  bdosrw entry point	*
************************/

UWORD seqread(fcbp)

REG struct fcb *fcbp;		/* fcbp is a pointer to a fcb */
				/* 2 = random with zero fill	*/
{
    REG UWORD	block;		/* block number from fcb	*/
    REG WORD	index;		/* index into disk map of fcb	*/
    REG UBYTE	*buf;		/* type compatibility kludge	*/
    REG WORD	parm;		/* parameter to do-io		*/
    REG WORD	bigfile;	/* file system is in word mode	*/
    BSETUP

    bigfile = ((GBL.parmp)->dsm) & ~0xff;
    if (fcbp->cur_rec == 128)
	{				/* time to try next extent */
	    if ( new_ext(fcbp) )
		return(1);	/* if can't open new extent, error */
	    fcbp->cur_rec = 0;	/* opened new extent, zero cur_rec */
	}

    if ( UBWORD(fcbp->cur_rec) >= get_rc(fcbp) )
	return(1);	 	/* this is end of file */
    index = blkindx(fcbp);		/* get index into fcb disk map */
    block = blknum(fcbp, index, bigfile);
    if ( ! block) return(1);
    return( do_io(block, (fcbp->cur_rec)++));
}
