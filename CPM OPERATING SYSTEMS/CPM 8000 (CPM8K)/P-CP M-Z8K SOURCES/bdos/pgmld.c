/****************************************************************
*								*
*		CP/M-8000 BDOS Program Loader			*
*								*
*		Copyright (c) 1982 Zilog Incorporated		*
*								*
*	This function implements BDOS call 59: Program Load.	*
*	The single parameter passed is a pointer to a space 	*
*	where a partially filled LPB (Load Parameter Block)	*
*	can be found.  pgmld must fill in the base page		*
*	address and the starting user stack pointer.  In	*
*	addition, the Z8000 implementation will set a loader	*
*	flag if the program being loaded uses separate I/D	*
*	space or segmentation.					*
*								*
*	NOTE! unlike the usual CP/M loader, the Z8000 loader	*
*	returns the actual starting address of the code segment	*
*	(starting PC) in the LPB, clobbering the program load	*
*	address.  This is because the segment containing the	*
*	code may not be known until load time.			*
*								*
****************************************************************/

#include "stdio.h"		/* Standard declarations for BDOS, BIOS */

#include "bdosdef.h"		/* Type and structure declarations for BDOS */

#include "biosdef.h"		/* Declarations of BIOS functions 	*/

#include "basepage.h"		/* Base page structure			*/

#include "x.out.h"		/* structure of x.out (".Z8[KS] file")	*/

#define SPLIT	0x4000		/* Separate I/D flag for LPB		*/
#define SEG	0x2000		/* Segmented code flag for TPA		*/
#define NSEG	16		/* Maximum number of x.out segments	*/
#define SEGLEN	0x10000		/* Length of a Z8000 segment		*/
				/* Address of basepage (near top of TPA)*/
#define BPLEN	(sizeof (struct b_page))
#define DEFSTACK 0x100		/* Default stack length			*/
#define NREGIONS 2		/* Number of regions in the MRT		*/

/* return values */

#define GOOD	0		/* good return value			*/
#define BADHDR	1		/* bad header 		 		*/
#define NOMEM	2		/* not enough memory 			*/
#define READERR	3		/* read error 				*/

#define MYDATA	0		/* Argument for map_adr			*/
#define TPAPROG	5		/* Argument for map_adr			*/
#define TPADATA	4		/* Argument for map_adr			*/
				/* Get actual code segment (as opposed	*/
				/*   to segment where it can be accessed*/
				/*   as data)				*/
#define TRUE_TPAPROG	(TPAPROG | 0x100)

struct lpb			/* Load Parameter Block			*/
	{
		XADDR	fcbaddr;/* Address of fcb of opened file	*/
		XADDR	pgldaddr;/* Low address of prog load area	*/
		XADDR	pgtop;	/* High address of prog load area, +1	*/
		XADDR	bpaddr;	/* Address of basepage; return value	*/
		XADDR	stackptr;/* Stack ptr of user; return value	*/
		short 	flags;	/* Loader control flags; return value	*/
	} mylpb;

struct ustack			/* User's initial stack - nonsegmented  */
	{
		short	two;	/* "Return address" (actually address   */
				/*  of warm boot call in user's startup)*/
		short	bpoffset;/* Pointer to basepage			*/
	};

struct sstack			/* User's initial stack - segmented     */
	{
		XADDR	stwo;	/* "Return address" (actually address   */
				/*  of warm boot call in user's startup)*/
		XADDR	sbpadr; /* Pointer to basepage			*/
	};


struct m_rt {			/* The Memory Region Table		*/
	int count;
	struct {
		XADDR	tpalow;
		XADDR	tpalen;
	} m_reg[NREGIONS];
};

#define SPREG	1		/* The MRT region for split I/D programs */
#define NSPREG	0		/* The MRT region for non-split programs */
#define SDREG	2		/* The MRT region for split I/D data     */
#define NSDREG	0		/* The MRT region for non-split data     */

#define READ	20		/* Read Sequential BDOS call		*/
#define SETDMA	26		/* Set DMA Address BDOS call		*/
extern UWORD	bdos();		/* To do I/O into myself (note this	*/
				/*   function does not map 2nd param -	*/
				/*   see mbdos macro below)		*/

static XADDR	textloc,	/* Physical locations of pgm sections.	*/
		dataloc,
		bssloc,
		stkloc;

static XADDR	textsiz,	/* Sizes of the various sections.	*/
		datasiz,
		bsssiz,
		stksiz;

static UWORD	split,		/* Tells if split I/D or not		*/
		seg;		/* Tells if segmented or not		*/

static char	*gp;		/* Buffer pointer for char input	*/
static char	*mydma;		/* Local address of read buffer		*/

struct x_hdr	x_hdr;		/* Object File Header structure		*/
struct x_sg	x_sg[NSEG];	/* Segment Header structure		*/

static XADDR	segsiz[NSEG];	/* Segment lengths 			*/
static XADDR	seglim[NSEG];	/* Segment length limits		*/
static XADDR	segloc[NSEG];	/* Segment base physical addresses	*/

static short	textseg,	/* Logical seg # of various segments	*/
		dataseg,
		bssseg,
		stkseg;


		/********************************/
		/*				*/
		/* Start of pgmld function	*/
		/*				*/
		/********************************/

UWORD pgmld(xlpbp)			/* Load a program from LPB info */
XADDR xlpbp;
{	register int	i,j;		/* Temporary counters etc.	*/
	struct m_rt	*mrp;		/* Pointer to a MRT structure	*/
	char		mybuf[SECLEN];	/* Local buffer for file reading*/

					/* get local LPB copy		*/
	cpy_in(xlpbp, &mylpb, (long) sizeof mylpb);

	mydma = mybuf;			/* Initialize addr for local DMA*/
	gp = &mybuf[SECLEN];		/* Point beyond end of buffer	*/

	mrp = (struct m_rt *) bgetseg();/* Get address of memory region */
					/*   table (note segment # lost)*/
	if (readhdr() == EOF)		/* Get x.out file header	*/
		return (READERR);	/* Read error on header		*/

	switch (x_hdr.x_magic)		/* Is this acceptable x.out file*/
	{
	  case X_NXN_MAGIC:		/* Non-seg, combined I & D	*/
		split = FALSE;
		seg = FALSE;
		break;

	  case X_NXI_MAGIC:		/* Non-seg, separate I & D	*/
		split = SPLIT;
		seg = FALSE;
		break;

	  case X_SX_MAGIC:		/* Segmented - must be combined */
		split = FALSE;
		seg = SEG;
		break;

	  default:
		return (BADHDR);		/* Sorry, can't load it!	*/
	}

/* Set the user space segment number, from the low address in the	*/
/*  appropriate entry of the MRT.					*/
/* m_reg[SPREG] is the region used for split I/D programs in the MRT	*/
/* m_reg[NSPREG] is used for non-split.					*/
/* -1            is used for segmented					*/

/* NOTE -- the tpa limits passed in the LPB are ignored.  This is	*/
/*	   incorrect, but saves the caller from having to look at the	*/
/*	   load module to determine the magic number.			*/

	map_adr(seg ? -1L 
		    : (mrp->m_reg[split ? SPREG : NSPREG].tpalow), 
		0xffff);
	
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


	if (seg) {			/* Locate text & data segments	*/
					/* if segmented we know nothing	*/
		textseg = dataseg = bssseg = stkseg = 0;
	} else {			/* if nonsegmented ...		*/
					/* assign segment numbers	*/
		textseg = 0;
		dataseg = (split) ? 1 : 0;
		stkseg = bssseg = dataseg;

					/* assign locations		*/
		segloc[textseg] = map_adr(0L, TPAPROG);
		if (split) 
			segloc[dataseg] = map_adr(0L, TPADATA);

					/* Assign limits		*/
		seglim[textseg] = SEGLEN;
		seglim[dataseg] = mrp->m_reg[split ? SDREG : NSDREG].tpalen 
				  - BPLEN - stksiz;

					/* Assign stack location	*/
		stkloc = segloc[dataseg] + seglim[dataseg] + stksiz;
	}

	for (i = 0; i < x_hdr.x_nseg; i++)	/* For each segment...	*/
		if( (j = loadseg(i)) != GOOD)	/* ...load memory. If	*/
			return (j);		/* error return, pass	*/
						/* it back.		*/
	setbase(setaddr(&mylpb));		/* Set addresses in LPB,*/
						/* Set up base page	*/
	cpy_out((XADDR) &mylpb, xlpbp, sizeof mylpb);
	return (GOOD);
}


/* Macro to call BDOS.  First parameter is passed unchanged, second	*/
/* is cast into an XADR, then mapped to caller data space.		*/

#define mbdos(func, param) (bdos((func), map_adr((XADDR) (param), MYDATA)))


/* Macro to read the next character from the input file (much faster	*/
/*   than having to make a function call for each byte)			*/

#define fgetch() ((gp<mydma+SECLEN) ? (int)*gp++&0xff : fillbuff())


/* Routine to fill input buffer when fgetch macro detects it is empty	*/


int fillbuf()				/* Returns first char in buffer */
{					/*   or EOF if read fails	*/
					/* Set up address to read into	*/
	mbdos(SETDMA, mydma);
	if (bdos(READ, mylpb.fcbaddr) != 0)	/* Have BDOS do the read*/
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

	if (split) {			/* If separate I/D, this may	*/
		switch (type)		/*   be a bad guess		*/
		{
		  case X_SG_CON:	/* Separate I/D: all data goes  */
		  case X_SG_DATA:	/*   in data space		*/
		  case X_SG_BSS:
		  case X_SG_STK:
			lseg  = dataseg;
		}
	}

	if (seg) {			/* If segmented, compute phys.	*/
					/* address of segment		*/

		/* search to see if seg was used already	*/
		/* if so, use the same logical segment index.	*/
		/* (if not, loop ends with lseg == i)		*/

		for (lseg = 0; 
		     x_sg[lseg].x_sg_no != x_sg[i].x_sg_no; 
		     lseg++) ;

		segloc[lseg] = ((long)x_sg[i].x_sg_no) << 24;
	}

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
			bdos(READ, mylpb.fcbaddr);
		}

		phystarg += length;
		l -= length;
	}

	return (GOOD);
}

/* Routine to set the addresses in the Load Parameter Block		*/
/* Unlike normal CP/M, the original load address is replaced on return	*/
/* by the actual starting address of the program (true Code-space addr)	*/

int setaddr(lpbp)
struct lpb *lpbp;
{
	register int space;

	space = (split) ? TPADATA : TPAPROG;
	lpbp->pgldaddr = (seg) ? textloc : map_adr(textloc, TRUE_TPAPROG);
	lpbp->bpaddr = stkloc;
	lpbp->stackptr = stkloc - (seg? sizeof (struct sstack)
					    : sizeof (struct ustack));
	lpbp->flags = split | seg;
	return (space);
}

/* Routine to set up the base page.  The parameter indicates whether
 * the data and bss should be mapped in code space or in data space.
 */

VOID setbase(space)
int space;
{
	struct b_page	bp;

	if (seg) {
		bp.lcode = textloc;
		bp.ltpa  = 0L;
	} else {
		bp.lcode = bp.ltpa = map_adr(textloc, TRUE_TPAPROG);
	}
				
	bp.htpa = mylpb.stackptr;	/* htpa is where the stack is	*/
	bp.codelen = textsiz;

	bp.ldata = dataloc;
	bp.datalen = datasiz;

	if (bssloc == 0L) bssloc = dataloc + datasiz;
	bp.lbss = bssloc;
	bp.bsslen = bsssiz;

	bp.freelen = seglim[bssseg] - segsiz[bssseg];

	cpy_out(&bp, map_adr((long) stkloc, space), sizeof bp);
}
