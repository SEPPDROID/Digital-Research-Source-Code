
/****************************************************************
*								*
*		CP/M-Z8k Loader BDOS File I/O Module		*
*								*
*	This module contains all file handling BDOS functions	*
*	except for read and write for CP/M-Z8K.  Included are:	*
*								*
*		seldsk()    - select disk			*
*		openfile()  - open file				*
*		move()	    - general purpose byte mover	*
*								*
*								*
*	Modified for memory management on Z8000			*
*	The main change is that search no longer calls tmp_sel	*
*	tmp_sel in bdosmain.c now handles drive number = '?'	*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "biosdef.h"		/* and BIOS */

#include "pktio.h"		/* Packet I/O definitions */

/* declare external fucntions */
EXTERN UWORD 	dirscan();	/* directory scanning routine	*/
EXTERN UWORD	do_phio();	/* packet disk i/o handler */
EXTERN UWORD	swap();		/* assembly language byte swapper */

/************************
*  seldsk entry point	*
************************/

seldsk(dsknum)

REG UBYTE dsknum;		/* disk number to select */

{
    struct iopb selpkt;
    REG WORD	i;
    UWORD	j;
    BSETUP

	if (UBWORD(dsknum) > 15) return(-1);
	selpkt.devnum = dsknum;
	selpkt.iofcn = sel_info;
	selpkt.ioflags = 0;
	do_phio(&selpkt);	/* actually do the disk select	*/
	if( (GBL.dphp = selpkt.infop) == NULL)
		return(-1);
	GBL.dirbufp = (GBL.dphp)->dbufp;
			/* set up GBL copies of dir_buf and dpb ptrs */
	GBL.parmp = (GBL.dphp)->dpbp;
	return(0);
}


/*******************************
*  General purpose byte mover  *
*******************************/

move(p1, p2, i)

REG BYTE *p1;
REG BYTE *p2;
REG WORD  i;
{
    while (i--)
	*p2++ = *p1++;
}


/*************************************
*  General purpose filename matcher  *
*************************************/

BOOLEAN match(p1, p2, chk_ext)

REG UBYTE *p1;
REG UBYTE *p2;
BOOLEAN  chk_ext;
{
    REG WORD	i;
    REG UBYTE temp;
    BSETUP

    i = 12;
    do
    {
	temp = (*p1 ^ '?');
	if ( ((*p1++ ^ *p2++) & 0x7f) && temp )
	    return(FALSE);
	i -= 1;
    } while (i);
    if (chk_ext)
    {
	if ( (*p1 != '?') && ((*p1 ^ *p2) & ~((GBL.parmp)->exm)) )
	    return(FALSE);
	p1 += 2;
	p2 += 2;
	if ((*p1 ^ *p2) & 0x3f) return(FALSE);
    }
    return(TRUE);
}


/************************
*  openfile entry point	*
************************/

BOOLEAN openfile(fcbp, dirp, dirindx)	/* ARGSUSED */

REG struct fcb *fcbp;		/* pointer to fcb for file to open */
struct dirent  *dirp;		/* pointer to directory entry	*/
WORD	dirindx;

{
    REG UBYTE fcb_ext;		/* extent field from fcb	*/
    REG BOOLEAN rtn;
    BSETUP

    if ( rtn = match(fcbp, dirp, TRUE) )
    {
	fcb_ext = fcbp->extent;	 /* save extent number from user's fcb */
	move(dirp, fcbp, sizeof *dirp);
				/* copy dir entry into user's fcb  */
	fcbp->extent = fcb_ext;
	fcbp->s2 |= 0x80;	 /* set hi bit of S2 (write flag)	*/
    }
   return(rtn);
}
