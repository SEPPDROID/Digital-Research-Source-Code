/****************************************************************
*								*
*		CP/M-8000 BDOS Program Loader			*
*								*
*		Copyright (c) 1982 Zilog Incorporated		*
*								*
*	This module contains routines for loading the file	*
*	naked "CPM.SYS" into memory, at the address specified	*
*	by the first entry in the MRT.  If we are properly	*
*	linked with the Bootstrap BIOS, this is a loader-	*
*	specific MRT which actually contains the address of	*
*	system space.						*
*								*
****************************************************************/

#include "stdio.h"		/* Standard declarations for BDOS, BIOS */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "biosdef.h"		/* Declarations of BIOS functions 	*/

#include "basepage.h"		/* Base page structure			*/

#include "x.out.h"		/* structure of x.out (".Z8[KS] file")	*/

#define SEP_ID	0x4000		/* Separate I/D flag			*/

#define PRNTSTR	9		/* Print String BDOS call		*/
#define SETDMA	26		/* Set DMA Address BDOS call		*/

#define BGETMRT 18		/* Number of the BIOS call		*/

#define open(fcbp) _bdos(15, 0, map_adr( (XADDR)fcbp, MYDATA) )
				/* BDOS open call		*/

extern	UWORD	bdos();		/* To do I/O into myself		*/
extern	XADDR	bios();	/* To get MRT pointer			*/
extern	VOID	xfer();		/* Transfer control to user program	*/
#define NPSEG	1		/* Number of physical segs available	*/
				/*  for CP/M kernel			*/
#define NSEG	16		/* Maximum number of x.out segments	*/
#define SEGLEN	0x10000		/* Length of a Z8000 segment		*/
#define BPLEN	(sizeof (struct b_page))
#define DEFSTACK 0x100		/* Default stack length			*/
#define NREGIONS 1		/* Number of regions in the Loader MRT	*/

/* return values */

#define GOOD	0		/* good return value			*/
#define BADHDR	1		/* bad header 		 		*/
#define NOMEM	2		/* not enough memory 			*/
#define READERR	3		/* read error 				*/

#define MYDATA	0		/* Argument for map_adr			*/
#define SYSPROG	3		/* Argument for map_adr			*/
#define SYSDATA	2		/* Argument for map_adr			*/
				/* Get actual code segment (as opposed	*/
				/*   to segment where it can be accessed*/
				/*   as data)				*/
#define TRUE_SYSPROG	(SYSPROG | 0x100)

struct m_rt {			/* The Memory Region Table		*/
	int entries;
	struct {
		XADDR	m_low;
		XADDR	m_len;
	} m_reg[NREGIONS];
};

struct context			/* Startup context for user's program	*/
	{
		short	regs[14];
		XADDR	segstkptr;
		short	ignore;
		short	FCW;
		XADDR	PC;
	};
extern	struct	context	context;	/* defined in bios.c */


/* Define the data structures used in ldcpm */

struct fcb cpmfcb =
{   (BYTE)0,			/* drive code */
    'C','P','M',' ',' ',' ',' ',' ','S','Y','S',	/* file name  */
    0,0,0,0,0,0,0,0,0,0,		/* the rest of the fcb */
    0,0,0,0,0,0,0,0,0,0,0
};


#define NSPREG	0		/* The MRT region for non-split programs */

#define READ	20		/* Read Sequential BDOS call		*/
extern UWORD	bdos();		/* To do I/O into myself (note this	*/
				/*   function does not map 2nd param -	*/
				/*   see mbdos macro below)		*/

XADDR	textloc,	/* Physical locations of pgm sections.	*/
	dataloc,
	bssloc,
	stkloc;

XADDR	textsiz,	/* Sizes of the various sections.	*/
	datasiz,
	bsssiz,
	stksiz;

UWORD	split,		/* Tells if split I/D or not		*/
	seg;		/* Tells if segmented or not		*/

char	*gp;		/* Buffer pointer for char input	*/
char	*mydma;		/* Local address of read buffer		*/

struct x_hdr	x_hdr;		/* Object File Header structure		*/
struct x_sg	x_sg[NSEG];	/* Segment Header structure		*/

XADDR	segsiz[NPSEG];	/* Segment lengths 			*/
XADDR	seglim[NPSEG];	/* Segment length limits		*/
XADDR	segloc[NPSEG];	/* Segment base physical addresses	*/

short	textseg,	/* Logical seg # of various segments	*/
	dataseg,
	bssseg,
	stkseg;


/**************************/
/*    Utility Routines    */
/**************************/

pstring(p)
		/* print string */
REG BYTE *p;
{
    while (*p) bconout(*p++);
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

/***************************
*    ldcpm main routine    *
***************************/


		/********************************/
		/*				*/
		/*         l d c p m            */
		/*				*/
		/********************************/


VOID						/* Load a program from	*/
ldcpm()					/*   info in cpmfcb	*/
{
	register unsigned k;
	register XADDR	physaddr;
	register char	*tp;

	if (seldsk(0)) badload(0);

	k = open(&cpmfcb);
	if( k >= 255 ) badload(1);

	k = load();				/* load the CP/M system */
	if(k != GOOD) badload(2);

	pstring("\r\n\nCP/M-Z8K(tm) Version 1.3 ");
	pstring("\n\rCopyright (c) 1983 Zilog Inc, Digital Research, Inc.\r\n\n");

/* Finally, ready to transfer control.  Must complete context first.	*/
	xfer(map_adr((XADDR) &context, MYDATA));	/* Go for it!	*/
}
		/********************************/
		/*				*/
		/* Start of load function	*/
		/*				*/
		/********************************/

load()					/* Load a program from FCB info */
{	register int	i,j;		/* Temporary counters etc.	*/
	struct m_rt	*mrp;		/* Pointer to a MRT structure	*/
	char		mybuf[SECLEN];	/* Local buffer for file reading*/

	mydma = mybuf;			/* Initialize addr for local DMA*/
	gp = &mybuf[SECLEN];		/* Point beyond end of buffer	*/

	mrp = (struct m_rt *) bios(BGETMRT);/* Get address of memory region */
					/*   table (note segment # lost)*/
	if (readhdr() == EOF)		/* Get x.out file header	*/
		return (READERR);	/* Read error on header		*/

	if (x_hdr.x_magic == X_NXN_MAGIC)  /* Is this acceptable x.out file*/
	{				/* CP/M kernel must be nonseg nonsplit*/
		split = FALSE;
		seg = FALSE;
	}
	else
		return (BADHDR);	/* Sorry, can't load it!	*/

/* Set the user space segment number, from the low address in the	*/
/*  appropriate entry of the MRT.					*/
/* m_reg[SPREG] is the region used for split I/D programs in the MRT	*/
/* m_reg[NSPREG] is used for non-split.					*/

/* NOTE -- the tpa limits passed in the MRT are really system space	*/
/*	   limits.  It is assumed that the system is non-split.		*/

	for (i = 0; i < x_hdr.x_nseg; i++) {	/* For each segment...	*/
		if( readxsg(i) == EOF) 		/* ...get segment hdr	*/
			return(READERR);
		seglim[i] = SEGLEN;		/* ...set max length	*/
		segsiz[i] = 0L;			/* ...and current size	*/
	}

					/* Set section base addresses   */

	textloc = dataloc = bssloc = stkloc = 0L;

					/* Zero section sizes		*/
	textsiz = datasiz = bsssiz = 0L;
	stksiz  = DEFSTACK;

	stkseg = bssseg = dataseg= textseg = 0;

				/* assign locations		*/
	segloc[textseg] = mrp->m_reg[NSPREG].m_low;

				/* Assign limits		*/
	seglim[textseg] = mrp->m_reg[NSPREG].m_len - stksiz;

				/* Assign stack location	*/
	stkloc = segloc[dataseg] + seglim[dataseg] + stksiz;
	

	for (i = 0; i < x_hdr.x_nseg; i++)	/* For each segment...	*/
		if( (j = loadseg(i)) != GOOD)	/* ...load memory. If	*/
			return (j);		/* error return, pass	*/
						/* it back.		*/
	return (GOOD);
}


/* Macro to call BDOS.  First parameter is passed unchanged, second	*/
/* is cast into an XADDR, then mapped to caller data space.		*/

#define mbdos(func, param) (bdos((func), map_adr((XADDR) (param), MYDATA)))


/* Macro to read the next character from the input file (much faster	*/
/*   than having to make a function call for each byte)			*/

#define fgetch() ((gp<mydma+SECLEN) ? (int)*gp++&0xff : fillbuff())


/* Routine to fill input buffer when fgetch macro detects it is empty	*/


int fillbuf()				/* Returns first char in buffer */
{					/*   or EOF if read fails	*/
					/* Set up address to read into	*/
	mbdos(SETDMA, mydma);
	if (mbdos(READ, &cpmfcb) != 0)	/* Have BDOS do the read*/
		return (EOF);
	gp = mydma;			/* Initialize buffer pointer	*/
	return ((int)*gp++ & 0xff);	/* Return first character	*/
}

/* Routine to read the file header */

int readhdr()
{
	register int n, k;
	register char *p;

	p = (char *) &x_hdr;
	for (n = 0; n < sizeof (struct x_hdr); n++) {
		if( (k = fgetch()) == EOF)
			return (k);
		*p++ = (char) k;
	}
	return (GOOD);
}

/* Routine to read the header for  segment i */

int readxsg(i)
int i;
{
	register int n, k;
	register char *p;

	p = (char *) &x_sg[i];
	for(n = 0; n < sizeof (struct x_sg); n++) {
		if ( (k = fgetch()) == EOF)
			return (READERR);
		*p++ = (char) k;
	}
	return (GOOD);
}

/* Routine to load segment number i					*/
/* This assumes that the segments occur in load order in the file, and	*/
/* that all initialized data and, in the case of combined I/D programs, */
/* text segments, precede all bss segments.				*/

/* In the case of segmented programs, the stack segment must exist,	*/
/* and all segments are presumed to be of maximum length.		*/
/* Text, data, bss, and stack lengths are sum of lengths of all such	*/
/* segments, and so may be bigger than maximum segment length.		*/

int loadseg(i)
int i;
{
	register UWORD l, length;	/* Total, incremental length	*/
	register int	type;		/* Type of segment loaded	*/
	register short	lseg;		/* logical segment index	*/
	register XADDR	phystarg;	/* physical target load address	*/

	l = x_sg[i].x_sg_len;		/* number of bytes to load	*/
	type = x_sg[i].x_sg_typ;	/* Type of segment		*/

	lseg  = textseg;		/*   try putting in text space	*/

	phystarg = segloc[lseg] + segsiz[lseg];	/* physical target addr	*/

	switch (type)			/* Now load data, if necessary	*/
					/* save physical address & size	*/
	{
	  case X_SG_BSS:		/* BSS gets cleared by runtime	*/
					/*   startup.                   */
		stkloc = (phystarg & 0xffff0000L) + SEGLEN - BPLEN - stksiz;
					/*  ...in case no stack segment	*/
		if (bssloc == 0L) bssloc = phystarg;
		bsssiz += l;
		if ((segsiz[lseg] += l) >= seglim[lseg])
			return (NOMEM);
		return (GOOD);			/* Transfer no data	*/
	
	  case X_SG_STK:			/* Stack segment:	*/
		if (stkloc == 0L) { 		/* if segmented, we now	*/
						/* know where to put	*/
			seglim[lseg] -= BPLEN;	/* the base page	*/
			stkloc = segloc[lseg] + seglim[lseg];
		}

		stkseg = lseg;
		stksiz += l;			/*   adjust size and	*/
		seglim[lseg] -= l;		/*   memory limit	*/
		if (segsiz[lseg] >= seglim[lseg])
			return (NOMEM);
		return (GOOD);			/* Transfer no data	*/
	
	  case X_SG_COD:		/* Pure text segment		*/
	  case X_SG_MXU:		/* Dirty code/data  (better not)*/
	  case X_SG_MXP:		/* Clean code/data  (be sep I/D)*/
		if (textloc == 0L) textloc = phystarg;
		textsiz += l;
		break;

	  case X_SG_CON:		/* Constant (clean) data	*/
	  case X_SG_DAT:		/* Dirty data			*/
		stkloc = (phystarg & 0xffff0000L) + SEGLEN - BPLEN - stksiz;
					/*  ...in case no stack or 	*/
					/*    bss segments		*/
		if (dataloc == 0L) dataloc = segloc[i];
		datasiz += l;
		break;
	}
						/* Check seg overflow	*/
	if ((segsiz[lseg] += l) >= seglim[lseg])
		return (NOMEM);
						/* load data from file	*/

	/* Following loop is optimized for load speed.  It knows*/
	/*   about three conditions for data transfer:		*/
	/* 1. Data in read buffer:				*/
	/*	Transfer data from read buffer to target	*/
	/* 2. Read buffer empty and more than 1 sector of data  */
	/*    remaining to load:				*/
	/*	Read data direct to target			*/
	/* 3. Read buffer empty and less than 1 sector of data	*/
	/*    remaining to load:				*/
	/*	Fill read buffer, then proceed as in 1 above	*/

	while (l)			/* Until all loaded	*/
	{				/* Data in disk buffer? */
		if (gp < mydma + SECLEN)
		{
			length = min(l, mydma + SECLEN - gp);
			cpy_out(gp, phystarg, length);
			gp += length;
		}
		else if (l < SECLEN)	/* Less than 1 sector	*/
		{			/*   remains to transfer*/
			length = 0;
			mbdos(SETDMA, mydma);
			fillbuf();
			gp = mydma;
		}
		else			/* Read full sector	*/
		{			/*   into target space  */
			length = SECLEN;
			bdos(SETDMA, phystarg);
			mbdos(READ, &cpmfcb);
		}

		phystarg += length;
		l -= length;
	}

	return (GOOD);
}
