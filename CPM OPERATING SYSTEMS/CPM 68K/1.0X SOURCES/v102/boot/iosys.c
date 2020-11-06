
/****************************************************************
*								*
*		CP/M-68K BDOS Disk I/O System Module		*
*								*
*	This module translates from the packet oriented I/O	*
*	passed from the other BDOS modules into BIOS calls.	*
*								*
*	It includes only one external entry point:
*		do_phio()   - do physical i/o			*
*								*
*								*
*	Configured for Alcyon C on the VAX			*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "pktio.h"		/* Packet I/O definitions */

#include "biosdef.h"		/* Declarations for BIOS entry points */

EXTERN	udiv();			/* Assembly language unsigned divide routine */
				/* in bdosif.s.  It's used because Alcyon C  */
				/* can't do / or % without an external */

/************************
*  do_phio entry point	*
************************/

UWORD do_phio(iop)

REG struct iopb *iop;		/* iop is a pointer to a i/o parameter block */

{
    REG struct dph *hdrp;	  /* pointer to disk parameter header	*/
    REG struct dpb *dparmp;	  /* pointer to disk parameter block	*/
    struct
    {				/* structure for results of external divide */
	UWORD iotrk;
	UWORD iosect;
    } iopdiv;

    LOCK		/* lock the disk system while doing physical i/o */

    switch (iop->iofcn)
    {
	case sel_info:	
		iop->infop = bseldsk(iop->devnum, iop->ioflags);
		break;

	case read:
		hdrp = iop->infop;
		dparmp = hdrp->dpbp;
		udiv( iop->devadr, dparmp->spt, &iopdiv );
		bsettrk( iopdiv.iotrk + dparmp->trk_off );
		bsetsec( bsectrn( iopdiv.iosect, hdrp->xlt ) );
		bsetdma(iop->xferadr);
		return(bread());

    }

    UNLOCK
    return(0);
}
