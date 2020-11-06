
/****************************************************************
*								*
*		P-CP/M BDOS Miscellaneous Module		*
*								*
*	This module contains miscellaneous loose ends for	*
*	P-CP/M.  Included are:					*
*								*
*		bdosinit()  - BDOS initialization routine 	*
*			      called from CCP for system init	*
*		warmboot()  - BDOS warm boot exit routine 	*
*		error()     - BDOS error printing routine 	*
*		setexc()    - BDOS set exception vector		*
*		set_tpa()   - BDOS get/set TPA limits		*
*		cpy_bi()    - copy byte in			*
*								*
*								*
*	Modified for memory management on the Z8000		*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "biosdef.h"		/* BIOS definitions, needed for bios wboot */


/*  Declare external functions */
EXTERN		conout();		/* Console Output function	*/
EXTERN UBYTE	conin();		/* Console Input function	*/
EXTERN		prt_line();		/* Print String function 	*/
EXTERN UWORD	_bdos();		/* BDOS main routine		*/
EXTERN UBYTE	*traphnd();		/* assembly language trap handler */
EXTERN		initexc();		/* init the exception handler in  */
					/* exceptn.s			*/
EXTERN UWORD	dirscan();		/* Directory scanning routine	*/
EXTERN BOOLEAN  set_attr();		/* Set File attributes function */

/*  Declare external variables */
EXTERN	UWORD	log_dsk;		/* logged-on disk vector	*/
EXTERN	UWORD	ro_dsk;			/* read-only disk vector	*/
EXTERN	UWORD	crit_dsk;		/* vector of critical disks	*/
EXTERN  XADDR	tpa_lt;			/* TPA lower limit (temporary)	*/
EXTERN  XADDR	tpa_lp;			/* TPA lower limit (permanent)	*/
EXTERN  XADDR	tpa_ht;			/* TPA upper limit (temporary)	*/
EXTERN  XADDR	tpa_hp;			/* TPA upper limit (permanent)	*/
EXTERN  BOOLEAN	submit;			/* external variables from CCP	*/
EXTERN  BOOLEAN morecmds;


#define trap2v 34			/* trap 2 vector number */
#define ctrlc  3			/* control-c		*/


/********************************
*  bdos initialization routine	*
********************************/

bdosinit()
/* Initialize the File System */
{
    REG struct
    {
	WORD	nmbr;
	XADDR	low;
	LONG	length;
    } *segp;
    BSETUP

    bsetvec(trap2v, map_adr((long)traphnd, 257)); /* set up trap vector */
						   /* (inst. space addr) */
    GBL.kbchar = 0;		/* initialize the "global" variables */
    GBL.delim  = '$';
    GBL.lstecho = FALSE;
    GBL.echodel = TRUE;
    GBL.chainp  = XNULL;
    GBL.user    = 0;
    _bdos(13,0, XNULL);		/* reset disk system function */
    prt_line("
\r\nCPM-Z8000 Version 1.2 03/14/83$");
    prt_line("\r\nCopyright 1982 Digital Research Inc., Zilog Inc.$");
    segp = bgetseg();		/* get pointer to memory segment table */
    tpa_lt = tpa_lp = segp->low;
    tpa_ht = tpa_hp = tpa_lp + segp->length;
    initexc( &(GBL.excvec[0]) );
}


/************************
*  warmboot entry point	*
************************/

warmboot(parm)
/* Warm Boot the system */
WORD parm;			/* 1 to reset submit flag */
{
    BSETUP

    log_dsk &= ~ro_dsk;		/* log off any disk marked read-only */
			/* note that this code is specifically for a single-
			   thread system.  It won't work in a multi-task sys */
    ro_dsk = 0;
    crit_dsk = 0;
    if (parm)
	submit = morecmds = FALSE;
    tpa_lt = tpa_lp;
    tpa_ht = tpa_hp;
    initexc( &(GBL.excvec[0]) );
    bwboot();
}


/*************************/
/*  disk error handlers  */
/*************************/

prt_err(p)
/*  print the error message  */

BYTE  *p;
{
    BSETUP

    prt_line(p);
    prt_line(" error on drive $");
    conout(GBL.curdsk + 'A');
}


abrt_err(p)
/*  print the error message and always abort */

BYTE  *p;
{
    prt_err(p);
    warmboot(1);
}


ext_err(p)
/*  print the error message, and allow for retry, abort, or ignore */

BYTE  *p;
{
    REG UBYTE  ch;

    prt_err(p);
    do
    {
	prt_line("\n\rDo you want to:  Abort (A),  Retry (R),$");
	prt_line("  or Continue with bad data (C)? $");
	ch = conin() & 0x5f;
	prt_line("\r\n$");

	switch ( ch )
	{
	    case ctrlc: warmboot(1);
	    case 'A':  warmboot(1);
	    case 'C':  return(1);
	    case 'R':  return(0);
	}
    }   while (TRUE);
}


filero(fcbp)
/*  File R/O error  */

REG struct fcb *fcbp;
{
    REG BYTE *p;
    REG UWORD i;
    REG UBYTE  ch;

    p = (BYTE *)fcbp;
    prt_line("file error: $");
    i = 8;
    do conout(*++p); while (--i);
    conout('.');
    i = 3;
    do conout(*++p); while (--i);
    prt_line(" is read-only.$");
    do
    {
 prt_line("\r\nDo you want to: Change it to read/write (C), or Abort (A)? $");
	ch = conin() & 0x5f;
	prt_line("\r\n$");

	switch ( ch )
	{
	    case ctrlc: warmboot(1);
	    case 'A':   warmboot(1);
	    case 'C':   fcbp->ftype[robit] &= 0x7f;
			return( dirscan(set_attr, fcbp, 2) );
	}
    }   while (TRUE);
}


/************************
*  error entry point	*
************************/

error(errnum, fcbp)	/* VARARGS */
/* Print error message, do appropriate response */

UWORD        errnum;			/* error number */
struct fcb  *fcbp;			/* pointer to fcb */
{
    BSETUP

    prt_line("\r\nCP/M Disk $");
    switch (errnum)
    {
	case 0:  return( ext_err("read$") );
		 /* break; */

	case 1:  return( ext_err("write$") );
		 /* break; */

	case 2:  abrt_err("select$");
		 /* break; */

	case 3:  return( ext_err("select$") );
		 /* break; */

	case 4:  abrt_err("change$");
		 /* break; */

	case 5:  return filero(fcbp);
		 /* break; */

    }
}


/*****************************
*  set exception entry point *
*****************************/

setexc(xepbp)
/* Set Exception Vector */
REG XADDR xepbp;
{
    REG WORD i;
    REG struct
    {
        WORD vecnum;
        UBYTE *newvec;
        UBYTE *oldvec;
    } epb;

    BSETUP

    cpy_in(xepbp, &epb, sizeof epb);		/* copy in param block */

    i = epb.vecnum-2;
    if ( i==32 || i==33) return(-1);
    if ( (30 <= i) && (i <= 37) ) i -= 20;
    else if ( (i < 0) || (i > 9) ) return(-1);
    epb.oldvec = GBL.excvec[i];
    GBL.excvec[i] = epb.newvec;

    cpy_out(&epb, xepbp, sizeof epb);		/* copy out param block */

    return(0);
}


/*****************************
*  get/set TPA entry point   *
*****************************/

set_tpa(xp)
/* Get/Set TPA Limits */
REG XADDR xp;

#define set	1
#define sticky	2

{
struct
    {
	UWORD parms;
	XADDR low;
	XADDR high;
    } p;

    cpy_in(xp, &p, sizeof p);		/* copy in param block */

    if (p.parms & set)
    {
	tpa_lt = p.low;
	tpa_ht = p.high;
	if (p.parms & sticky)
	{
	    tpa_lp = tpa_lt;
	    tpa_hp = tpa_ht;
	}
    }
    else
    {
	p.low = tpa_lt;
	p.high = tpa_ht;
    }

    cpy_out(&p, xp, sizeof p);		/* copy out param block */

}


/*****************************************************
**
** ubyte = cpy_bi(xaddr)-- copy byte in
**
*****************************************************/

UBYTE cpy_bi(addr)
XADDR addr;
{
	UBYTE b;

	cpy_in(addr, &b, 1);
	return b;
}

