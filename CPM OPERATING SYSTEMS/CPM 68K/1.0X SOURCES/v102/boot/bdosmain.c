
/****************************************************************
*								*
*		CP/M-68K Loader BDOS Main Routine		*
*								*
*	This is the main routine for the Loader BDOS for 	*
*	CP/M-68K. It has one entry point, _bdos.	 	*
*	the assembly language trap handler found in bdosif.s.	*
*	There are two parameters: a function number (integer)	*
*	and an information parameter, potentially a long word	*
*	The BDOS can potentially return a pointer, long word,	*
*	or word							*
*								*
*	Configured for Alcyon C on the VAX			*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "biosdef.h"		/* Declarations of BIOS functions */

/* Serial Number and Copyright Notice */

char *copyrt="CP/M-68K(tm), Version 1.1, Copyright (c) 1983, Digital Research";
char *serial = "XXXX-0000-654321";


/*  Declare EXTERN functions */

EXTERN WORD	seldisk();	/* Select disk			*/
EXTERN BOOLEAN	openfile();	/* Open File			*/
EXTERN UWORD	dirscan();	/* General directory scanning routine */
EXTERN UWORD	seqread();	/* Sequential disk read		*/


/*  Declare the "state variables".  These are globals for the single-thread
    version of the file system, but are put in a structure so they can be
    based, with a pointer coming from the calling process		*/

GLOBAL struct stvars gbls;


/****************************************************************
*								*
*		_bdos MAIN ROUTINE				*
*								*
*	Called with  _bdos(func, info, infop)			*
*								*	
*	Where:							*
*		func	is the BDOS function number (d0.w)	*
*		info	is the word parameter (d1.w)		*
*		infop	is the pointer parameter (d1.l)		*
*			note that info is the word form of infop*
*								*
****************************************************************/


UWORD _bdos(func,info,infop)
REG WORD func;		/* BDOS function number */
REG UWORD info;		/* d1.w word parameter  */
REG UBYTE *infop;	/* d1.l pointer parameter */
{

	switch (func)	/* switch on function number */
	{

	  case 2:   bconout((UBYTE)info);	/* console output */
		    break;

	  case 14:  return(seldisk(info));	/* select disk */

	  case 15:  infop->drvcode = 0;		/* open file */
		    infop->extent = 0;
		    infop->s2 = 0;
		    return(dirscan(openfile, infop));

	  case 20:  infop->drvcode = 0;		/* read sequential */
		    return(seqread(infop));

	  case 26:  GBL.dmaadr = infop;		/* set dma address */
		    break;

	  default:  return(0xffff);		/* bad function number */
		    /* break; */

	};					/* end of switch statement */

	return(0);			/* return the BDOS return value */
}					/* end _bdos */


