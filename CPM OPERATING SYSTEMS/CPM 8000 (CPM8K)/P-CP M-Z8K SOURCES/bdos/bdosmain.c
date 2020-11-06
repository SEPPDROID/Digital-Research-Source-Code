
/****************************************************************
*								*
*		P-CP/M BDOS Main Routine			*
*								*
*	This is the main routine for the BDOS for P-CP/M.	*
*	It has one entry point, _bdos, which is  called from 	*
*	the assembly language trap handler found in bdosif.s.	*
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

EXTERN		warmboot();	/* Warm Boot function 		*/
EXTERN BOOLEAN	constat();	/* Console status		*/
EXTERN UBYTE	conin();	/* Console Input function	*/
EXTERN 		cookdout();	/* Cooked console output routine */
EXTERN UBYTE	rawconio();	/* Raw console I/O		*/
EXTERN		prt_line();	/* Print line until delimiter	*/
EXTERN		readline();	/* Buffered console read	*/
EXTERN		seldsk();	/* Select disk			*/
EXTERN BOOLEAN	openfile();	/* Open File			*/
EXTERN UWORD	close_fi();	/* Close File			*/
EXTERN UWORD	search();	/* Search first and next fcns	*/
EXTERN UWORD	dirscan();	/* General directory scanning routine */
EXTERN UWORD	bdosrw();	/* Sequential and Random disk read/write */
EXTERN BOOLEAN	create();	/* Create file			*/
EXTERN BOOLEAN	delete();	/* Delete file			*/
EXTERN BOOLEAN	rename();	/* Rename file			*/
EXTERN BOOLEAN	set_attr();	/* Set file attributes		*/
EXTERN		getsize();	/* Get File Size		*/
EXTERN		setran();	/* Set Random Record		*/
EXTERN		free_sp();	/* Get Disk Free Space		*/
EXTERN UWORD	flushit();	/* Flush Buffers		*/
EXTERN UWORD	pgmld();	/* Program Load			*/
EXTERN UWORD	setexc();	/* Set Exception Vector		*/
EXTERN		set_tpa();	/* Get/Set TPA Limits		*/
EXTERN		move();		/* general purpose byte mover	*/


/*  Declare "true" global variables; i.e., those which will pertain to the
    entire file system and thus will remain global even when this becomes
    a multi-tasking file system */

GLOBAL UWORD	log_dsk = 0;	/* 16-bit vector of logged in drives */
GLOBAL UWORD	ro_dsk = 0;	/* 16-bit vector of read-only drives */
GLOBAL UWORD	crit_dsk = 0;	/* 16-bit vector of drives in "critical"
				   state.  Used to control dir checksums */
GLOBAL XADDR	tpa_lp;		/* TPA lower boundary (permanent)	*/
GLOBAL XADDR	tpa_lt;		/* TPA lower boundary (temporary)	*/
GLOBAL XADDR	tpa_hp;		/* TPA upper boundary (permanent)	*/
GLOBAL XADDR	tpa_ht;		/* TPA upper boundary (temporary)	*/

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
	  case 0:   warmboot(0);		/* warm boot function */
		    /* break; */

	  case 1:   return((UWORD)conin());	/* console input function */
		    /* break; */

	  case 2:   cookdout((UBYTE)info,FALSE);/* "cooked" console output */
		    break;

	  case 3:   return((UWORD)brdr());	/* get reader from bios */
		    /* break; */

	  case 4:   bpun((UBYTE)info);		/* punch output to bios */
		    break;

	  case 5:   blstout((UBYTE)info);	/* list output from bios */
		    break;

	  case 6:   return((UWORD)rawconio(info)); /* raw console I/O */
		    /* break; */

	  case 7:   return(bgetiob());		/* get i/o byte */
		    /* break; */

	  case 8:   bsetiob(info);		/* set i/o byte function */
		    break;

	  case 9:   uprt_line(infop);		/* print line function */
		    break;

	  case 10:  ureadline(infop);		/* read buffered con input */
		    break;

	  case 11:  return((UWORD)constat());	/* console status */
		    /* break; */

	  case 12:  return(VERSION);		/* return version number */
		    /* break; */

	  case 13:  log_dsk = 0;		/* reset disk system */
		    ro_dsk  = 0;
		    crit_dsk= 0;
		    GBL.curdsk = 0xff;
		    GBL.dfltdsk = 0;
		    break;

	  case 14:  seldsk((UBYTE)info);	/* select disk */
		    GBL.dfltdsk = (UBYTE)info;
		    break;

	  case 15:  tmp_sel(&temp);		/* open file */
		    temp.fptr->extent = 0;
		    temp.fptr->s2 = 0;
		    rtnval = dirscan(openfile, temp.fptr, 0);
		    break;

	  case 16:  tmp_sel(&temp);		/* close file */
		    rtnval = close_fi(temp.fptr);
		    break;

	  case 17:  GBL.srchp = infop;		/* search first */
		    tmp_sel(&temp);
		    rtnval = search(temp.fptr, 0, &temp);
		    break;

	  case 18:  infop = GBL.srchp;		/* search next */
		    temp.fxptr = infop;
		    tmp_sel(&temp);
		    rtnval = search(temp.fptr, 1, &temp);
		    break;

	  case 19:  tmp_sel(&temp);		/* delete file */
		    rtnval = dirscan(delete, temp.fptr, 2);
		    break;

	  case 20:  tmp_sel(&temp);		/* read sequential */
		    rtnval = bdosrw(temp.fptr, TRUE, 0);
		    break;

	  case 21:  tmp_sel(&temp);		/* write sequential */
		    rtnval = bdosrw(temp.fptr, FALSE, 0);
		    break;

	  case 22:  tmp_sel(&temp);		/* create file */
		    temp.fptr->extent = 0;
		    temp.fptr->s1 = 0;
		    temp.fptr->s2 = 0;
		    temp.fptr->rcdcnt = 0;
			/* Zero extent, S1, S2, rcrdcnt. create zeros rest */
		    rtnval = dirscan(create, temp.fptr, 8);
		    break;

	  case 23:  tmp_sel(&temp);		/* rename file */
		    rtnval = dirscan(rename, temp.fptr, 2);
		    break;

	  case 24:  return(log_dsk);		/* return login vector */
		    /* break; */

	  case 25:  return(UBWORD(GBL.dfltdsk)); /* return current disk */
		    /* break; */

	  case 26:  GBL.dmaadr = infop;		/* set dma address */
		    break;

	  /* No function 27 -- Get Allocation Vector */

	  case 28:  ro_dsk |= 1<<GBL.dfltdsk;	/* set disk read-only */
		    break;

	  case 29:  return(ro_dsk);		/* get read-only vector */
		    /* break; */

	  case 30:  tmp_sel(&temp);		/* set file attributes */
		    rtnval = dirscan(set_attr, temp.fptr, 2);
		    break;

	  case 31:  if (GBL.curdsk != GBL.dfltdsk) seldsk(GBL.dfltdsk);
		    cpy_out( (GBL.parmp), infop, sizeof *(GBL.parmp) );
		    break;		/* return disk parameters */

	  case 32:  if ( (info & 0xff) <= 15 )	/* get/set user number */
			GBL.user = (UBYTE)info;
		    return(UBWORD(GBL.user));
		    /* break; */

	  case 33:  tmp_sel(&temp);		/* random read */
		    rtnval = bdosrw(temp.fptr, TRUE, 1);
		    break;

	  case 34:  tmp_sel(&temp);		/* random write */
		    rtnval = bdosrw(temp.fptr, FALSE, 1);
		    break;

	  case 35:  tmp_sel(&temp);		/* get file size */
		    getsize(temp.fptr);
		    break;

	  case 36:  tmp_sel(&temp);		/* set random record */
		    setran(temp.fptr);
		    break;

	  case 37:  info = ~info;		/* reset drive */
		    log_dsk &= info;
		    ro_dsk  &= info;
		    crit_dsk&= info;
		    break;

	  case 40:  tmp_sel(&temp);		/* write random with 0 fill */
		    rtnval = bdosrw(temp.fptr, FALSE, 2);
		    break;

	  case 46:  free_sp(info);		/* get disk free space */
		    break;

	  case 47:  GBL.chainp = GBL.dmaadr;	/* chain to program */
		    warmboot(0);		/* terminate calling program */
		    /* break; */

	  case 48:  return( flushit() );	/* flush buffers */
		    /* break; */

	  case 59:  return(pgmld(infop));	/* program load */
		    /* break; */

	  case 61:  return(setexc(infop));	/* set exception vector */
		    /* break; */

	  case 63:  set_tpa(infop);		/* get/set TPA limits */
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


/*****************************************************
**
** uprt_line(ptr) -- print line in user space
** ureadline(ptr) -- read line into user space
**
**	The pointer parameter is passed as a long,
**	since it may be in the user's memory space.
**
*****************************************************/

uprt_line(ptr)
XADDR ptr;
{
	UBYTE	c;
	XADDR	caddr;

	/* At one system-call per byte, this is VERY inefficient. */

	caddr = map_adr((XADDR)&c, 0);
	while (mem_cpy(ptr++, caddr, 1L), c != GBL.delim) 
		cookdout( c, FALSE);
}


ureadline(ptr)
XADDR ptr;
{
	char buf[258];

	cpy_in(ptr, buf, 1);			/* copy in user's buffer */

	readline(buf);				/* read line		 */

	cpy_out(buf, ptr, 2+(255&(int)buf[1]));	/* copy out result	 */
}

