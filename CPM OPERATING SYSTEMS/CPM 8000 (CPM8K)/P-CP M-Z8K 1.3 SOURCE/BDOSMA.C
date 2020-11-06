/****************************************************************
*								*
*		CP/M-Z8K Loader BDOS Main Routine		*
*								*
*	This is the main routine for the loader BDOS for P-CP/M.*
*	It has one entry point, _bdos, which is  called from 	*
*	the assembly language trap handler found in bdosif.z8k.	*
*	The parameters are a function number (integer) and an	*
*	information parameter, (passed as both an integer and	*
*	a pointer)						*
*	The BDOS can potentially return a pointer, long word,	*
*	or word							*
*								*
*	Memory management for Z8000, etc. added 821018 by SS.	*
*	One non-obvious change is using tmp_sel to copy in the	*
*	user's FCB, and copying it out again if changed.	*
*	Tmp_sel now handles '?' in the drive field, so it need	*
*	not be called from search (the only place outside of	*
*	this module where it is called)				*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "biosdef.h"		/* Declarations of BIOS functions */

/*  Declare EXTERN functions */

EXTERN		seldsk();	/* Select disk			*/
EXTERN BOOLEAN	openfile();	/* Open File			*/
EXTERN UWORD	dirscan();	/* General directory scanning routine */
EXTERN UWORD	seqread();	/* Sequential disk read		*/


/*  Declare the "state variables".  These are globals for the single-thread
    version of the file system, but are put in a structure so they can be
    based, with a pointer coming from the calling process		*/

GLOBAL struct stvars gbls;

struct tempstr
{
      UBYTE	  tempdisk;
      BOOLEAN	  reselect;
      struct fcb *fptr;
      XADDR	  fxptr;		/* xaddr of caller's FCB	*/
      struct fcb  tempfcb;		/* added for memory management	*/
					/* because caller's fcb may not	*/
					/* be directly accessible	*/
};

/****************************************************************
*								*
*		_bdos MAIN ROUTINE				*
*								*
*	Called with  _bdos(func, info, infop)			*
*								*	
*	Where:							*
*		func	is the BDOS function number (d0.w)	*
*		info	is the parameter as a word 		*
*		infop	is the parameter as a segmented pointer	*
*			note that info is the word form of infop*
*								*
****************************************************************/


UWORD _bdos(func,info,infop)
REG WORD func;		/* BDOS function number */
REG UWORD info;		/* parameter as word */
REG XADDR infop;	/* parameter as (segmented) pointer */
{
    REG UWORD rtnval;
    LOCAL struct tempstr temp;
    BSETUP

	temp.reselect = FALSE;
	temp.fxptr = infop;
	rtnval = 0;

	switch (func)	/* switch on function number */
	{

	  case 2:   bconout((UBYTE)info);	/* console output */
		    break;


	  case 14:  seldsk((UBYTE)info);	/* select disk */
		    GBL.dfltdsk = (UBYTE)info;
		    break;

	  case 15:  tmp_sel(&temp);		/* open file */
		    temp.fptr->extent = 0;
		    temp.fptr->s2 = 0;
		    rtnval = dirscan(openfile, temp.fptr);
		    break;

	  case 20:  tmp_sel(&temp);		/* read sequential */
		    rtnval = seqread(temp.fptr);
		    break;

	  case 26:  GBL.dmaadr = infop;		/* set dma address */
		    break;

	  default:  return(-1);			/* bad function number */
		    /* break; */

	};					/* end of switch statement */

	if (temp.reselect){          /* if reselected disk, restore it now */
		temp.fptr->drvcode = temp.tempdisk;
		cpy_out(temp.fptr, infop, sizeof *temp.fptr);
	}

	return(rtnval);			/* return the BDOS return value */

}					/* end _bdos */

/****************************************************************
*								*
*	bdos call - replaces system call in real CP/M-Z8K	*
*	Function is to arrange for parameters to _bdos to be 	*
*	in the right place.					*
*								*
****************************************************************/

bdos(func, longaddr)
int	func;
XADDR	longaddr;
{
	return(_bdos(func, (int) longaddr, longaddr));
}


/*****************************************************
**
** tmp_sel(temptr) -- temporarily select disk
**		      pointed to by temptr->fptr.
**
**	make local copy of FCB in caller's space.
**
*****************************************************/

tmp_sel(temptr)			/* temporarily select disk pointed to by fcb */
				/* also copy fcb into temp structure         */
REG struct tempstr *temptr;
{
    REG struct fcb *fcbp;
    REG UBYTE tmp_dsk;
    BSETUP

				/* get local copy of caller's FCB, */
				/* and point temptr->fptr at it    */

    cpy_in(temptr->fxptr, &temptr->tempfcb, sizeof(struct fcb));
    temptr->fptr = &temptr->tempfcb;

				/* get local copy of fcb pointer */
    fcbp = temptr->fptr;

				/* select disk if necessary	 */
    tmp_dsk = fcbp->drvcode;
    if (tmp_dsk == '?') {	/* -- drive '?' for search	 */
	seldsk( GBL.dfltdsk);
    } else {			/* -- drive 0 or disk+1		 */
	temptr->tempdisk = tmp_dsk;
	seldsk( tmp_dsk ? tmp_dsk - 1 : GBL.dfltdsk );

	fcbp->drvcode = GBL.user;
	temptr->reselect = TRUE;
    }
}
