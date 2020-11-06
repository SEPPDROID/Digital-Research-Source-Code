/****************************************************************
*								*
*			CP/M-68K Loader 			*
*								*
*	This is the main routine for the CP/M-68K Loader.  	*
*	It has one entry point, ldcpm, which is called from 	*
*	the assembly language routine called startld found in	*
*	ldrif.s.  startld just sets up the stack.		*
*								*
*	Configured for Alcyon C on the VAX			*
*								*
****************************************************************/

#include "stdio.h"		/* Standard I/O declarations */

#define ALCYON TRUE	/* TRUE if compiling with the ALCYON 68K C compiler */
#define SECLEN 128		/* CP/M sector length	*/


/*  Declare EXTERN functions */

EXTERN WORD	_bdos();	/* Loader BDOS entry point	*/
EXTERN 		startld();	/* Assembly routine to set up stack & retry */
EXTERN BYTE	*bios1();	/* bios entry point for get segment table */
EXTERN		move();		/* general purpose byte mover */


#define conout(ch)	_bdos(2,ch)
#define seldsk(dsk)	_bdos(14,dsk)
#define open(fcbp)	_bdos(15,0,fcbp)
#define read(fcbp)	_bdos(20,0,fcbp)
#define setdma(p)	_bdos(26,0,p)


/* Define the data structures used in ldcpm */
struct fcb
{
    UBYTE drvcode;		/* drive code, must be 0 for loader BDOS */
				/* (auto disk select not supported)	*/
    UBYTE fname[11];		/* includes file name and type fields	*/
    UBYTE sys_info[4];		/* ex, s1, s2, rc.  These are don't cares */
    UBYTE dskmap[16];		/* disk map, also a don't care		*/
    UBYTE cur_rec;		/* current record field			*/
};

struct hdr
{
    WORD magic;			/* magic number.  We only recognize 0x601a */
    LONG size[5];		/* sizes of code, data, bss, sym tbl, stack */
    BYTE *tstart;		/* start address of text segment	*/
    WORD rlbflg;		/* relocation bits suppressed flag	*/
};

struct segtbl
{				/* memory segment table from LDRBIOS	*/
    WORD entrys;		/* number of segments, must be 1	*/
    BYTE *start;		/* low memory address			*/
    LONG length;		/* length of segment			*/
};


/**********************/
/*  Global Variables  */
/**********************/

BYTE buffer[SECLEN];	/* 128 byte dma buffer to read program headers */

/*  We need 2 FCB's: one to read CPM.SYS and one to read BIOS.SYS  */
/*  Unfortunately, due to a bug in the Alcyon compiler, we can't initialize */
/*  them in a straightforward way.  So:  */

#if ALCYON

struct fcb cpmfcb =
{   0x0043, 0x504d, 0x2020, 0x2020, 0x2053, 0x5953, /* drive, name, and type */
    0,0,0,0,0,0,0,0,0,0,0			/* the rest of the fcb */
};

#else

struct fcb cpmfcb =
{   (BYTE)0			/* drive code */
    'CPM     SYS',		/* file name  */
    0,0,0,0,0,0,0,0,0,0		/* the rest of the fcb */
    0,0,0,0,0,0,0,0,0,0,0
};

#endif

/**************************/
/*    Utility Routines    */
/**************************/

pstring(p)
		/* print string */
REG BYTE *p;
{
    while (*p) conout(*p++);
}


badload(errtype)
		/* Load failed.  Print message and try again */
REG WORD errtype;
{
    REG WORD i;

    switch (errtype)
    {
	case 0: pstring("\r\nBoot error.");
		startld();

	case 1: pstring("\n\rOpen or Read error on ");
		break;

	case 2: pstring("\n\rBad file format on ");
    }
    pstring("CPM.SYS");
    startld();
}


/********************
*  load subroutine  *
********************/

BYTE *load(fcbp)
/* load the file pointed to by fcbp and return pointer to load point */
REG struct fcb *fcbp;
{
    REG BYTE *p;
    REG struct hdr *hp;
    REG LONG length;

    setdma( buffer );
    if ( open(fcbp) >= 255 ) badload(1);
    fcbp->cur_rec = 0;
    if ( read(fcbp) ) badload(1);
    hp = buffer;
    if ( (hp->magic != 0x601a) || ! (hp->rlbflg) )
	badload(2);
    p = hp->tstart;
    length = hp->size[0] + hp->size[1];
    move(&buffer[sizeof *hp], p, SECLEN - (sizeof *hp));
			/* move the first record minus header to load point */
    p += SECLEN - (sizeof *hp);
    length -= SECLEN - (sizeof *hp);
    while (length > 0L)
    {
	setdma(p);
	if ( read(fcbp) ) badload(1);
	p += SECLEN;
	length -= SECLEN;
    }
    return(hp->tstart);
}


/***************************
*    ldcpm main routine    *
***************************/

ldcpm()

{
    WORD (*gocpm)();

    if (seldsk(0)) badload(0);
    gocpm = load(&cpmfcb);		/* load the CP/M system */
    pstring("\r\n\nCP/M-68K(tm) Version 1.1 ");
    pstring("\n\rCopyright (c) 1982 Digital Research, Inc.\r\n\n");
    (*gocpm)();				/* jump to the CCP */
}

