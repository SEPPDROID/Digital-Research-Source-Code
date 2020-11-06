/************************************************************************
*									*
*		CP/M-8000 CCP Program Loader (__LOAD)			*
*									*
*		Copyright (c) 1982  Zilog Incorporated			*
*									*
*	This function arranges for the BDOS program loader to be	*
*	invoked in a suitable environment.  Suitable means that it	*
*	fills in whatever is expected in the LPB, gets the rest back	*
*	from the BDOS, and then completes the basepage and resets the	*
*	dma buffer.							*
*									*
*	The loading of segmented programs is not currently supported	*
*									*
************************************************************************/

#include "stdio.h"		/* Standard declarations for BDOS, BIOS */

#include "bdosdef.h"		/* BDOS type and structure declarations	*/

#include "biosdef.h"		/* Declarations of BIOS functions 	*/

#include "basepage.h"		/* Base page structure			*/

#define SEP_ID	0x4000		/* Separate I/D flag			*/
#define	SEG	0x2000		/* Segmented load module		*/

#define	NREGIONS 2		/* Number of MRT regions		*/

/* Return values: indexes into msgs (included here for documentation	*/
/*   purposes - only GOOD is used in this module			*/

#define GOOD	0		/* good return value			*/
#define BADHDR	1		/* bad header 		 		*/
#define NOMEM	2		/* not enough memory 			*/
#define READERR	3		/* read error 				*/

#define WARMBOOT 0		/* Warm reboot BDOS call		*/
#define PRNTSTR	9		/* Print String BDOS call		*/
#define SETDMA	26		/* Set DMA Address BDOS call		*/
#define PGLOAD	59		/* Program Load BDOS call		*/

#define MYDATA	0		/* Argument for map_adr			*/
#define TPADATA	4		/* Argument for map_adr			*/
#define TPAPROG	5		/* Argument for map_adr			*/
				/* Get actual code segment (as opposed	*/
				/*   to segment where it can be accessed*/
				/*   as data)				*/
#define TRUE_TPAPROG	(TPAPROG | 0x100)

#define BGETMRT 18		/* Number of the BIOS call		*/

extern char cmdfcb[];		/* the FCB for everything		*/
extern char *tail;		/* where the command tail is		*/

extern	UWORD	bdos();		/* To do I/O into myself		*/
extern	XADDR	bios();		/* To get MRT pointer			*/
extern	VOID	fill_fcb();	/* Parse filename into fcb		*/
extern	VOID	xfer();		/* Transfer control to user program	*/

struct lpb {
	XADDR	fcbaddr;	/* Address of fcb of opened file	*/
	XADDR	pgldaddr;	/* Low address of prog load area	*/
	XADDR	pgtop;		/* High address of prog load area, +1	*/
	XADDR	bpaddr;		/* Address of basepage; return value	*/
	XADDR	stackptr;	/* Stack ptr of user; return value	*/
	short 	flags;		/* Loader control flags; return value	*/
} LPB;

struct m_rt {			/* The Memory Region Table		*/
	int entries;
	struct {
		XADDR	m_low;
		XADDR	m_len;
	} m_reg[NREGIONS];
};

struct ustack			/* User's initial stack (nonsegmented)	*/
	{
		short	two;	/* "Return address" (actually a call to */
				/*   BDOS warm boot in runtime startup)	*/
		short	bpgaddr;/* Input parameter: pointer to basepage	*/
	} stack =
	{
		0x0002		/* bpgaddr initialized at runtime	*/
	};

struct sstack			/* User's initial stack (segmented)	*/
	{
		XADDR	stwo;	/* "Return address" (actually a call to */
				/*   BDOS warm boot in runtime startup)	*/
		XADDR	sbpgadr;/* Input parameter: pointer to basepage	*/
	} sstack;


/* Error messages for bad loads						*/

static char	*msgs[] =
	{
		"",
		"File is not executable$",
		"Insufficient memory$",
		"Read error on program load$",
		"Program Load Error$"
	};

struct context			/* Startup context for user's program	*/
	{
		short	regs[16];
		short	ignore;
		short	FCW;
		XADDR	PC;
	} context =

	{			/* Regs 0-14 cleared, 15 set up below	*/
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		0,		/* Ignore: value is zero		*/
		0x1800		/* FCW: nonsegmented normal, VI, NVI set*/
	};			/* PC initialized below			*/


		/********************************/
		/*				*/
		/*        _ _ L O A D           */
		/*				*/
		/********************************/


VOID						/* Load a program from	*/
__LOAD()					/*   info in cmdfcb	*/
{
	register short	k;
	struct m_rt	*mpr;
	register XADDR	physaddr;
	BYTE		tlen;			/* length of cmd tail	*/
	register char	*tp;

	mpr = (struct m_rt *) bios(BGETMRT);

/* Set up the Load Parameter Block					*/
/* Strictly speaking we should look at magic number to find out 	*/
/* which segment(s) the program needs.					*/

	LPB.pgldaddr = mpr->m_reg[0].m_low;
	LPB.pgtop = mpr->m_reg[0].m_len;
	LPB.fcbaddr = map_adr((XADDR) cmdfcb, MYDATA);

/* Try loading the program.  Print message and reboot if load fails	*/

	if ((k = bdos(PGLOAD, map_adr((XADDR) &LPB, MYDATA))) != GOOD)
	{
		bdos(PRNTSTR, map_adr((XADDR) msgs[min(4, k)], MYDATA));
		bdos(WARMBOOT, 0L);
	}

/* Move command tail to basepage buffer; reset DMA address to that buffer. */
/* Due to difficulty of adding structure member offset to something	*/
/*   which is not a structure pointer, we use a kludge to get physical	*/
/*   DMA buffer address: buffer is at last thing on basepage and has a	*/
/*   length of one sector.						*/

/* Compute length of tail, since CCP does not supply it			*/

	for (tlen=0, tp=tail; *tp++ != NULL; tlen++) ;

/* Next expression written in strange way to overcome compiler bug	*/

	physaddr = LPB.bpaddr - (SECLEN - sizeof (struct b_page));
	bdos(SETDMA, physaddr);
	cpy_out(&tlen, physaddr, 1L);			/* tail length	*/
	cpy_out(tail, physaddr+1, (long) SECLEN-1);	/* and cmd tail	*/

/* Fill base page fcb's							*/

	fill_fcb(1, cmdfcb);
	physaddr -=  sizeof (struct fcb);	/* Another kludge	*/
	cpy_out(cmdfcb, physaddr, sizeof (struct fcb));

	fill_fcb(2, cmdfcb);
	physaddr -= sizeof (struct fcb);	/* Yet a third kludge	*/
	cpy_out(cmdfcb, physaddr, sizeof(struct fcb));

/* Now build a user stack which looks like:
 *		-----------------	^		^
 *   word/long	| Base page addr|	^ High address	^
 *		-----------------	^		^
 *   word/long	|	2	|     <-- Stack pointer points here
 *		-----------------
 */
	if (LPB.flags & SEG) {				/* Segmented    */
		sstack.sbpgadr = LPB.bpaddr;
		sstack.stwo    = LPB.pgldaddr + 2;
		cpy_out(&sstack, LPB.bpaddr - sizeof sstack, sizeof sstack);
	} else {					/* Nonsegmented */
		stack.bpgaddr = (short) LPB.bpaddr;
		cpy_out(&stack, LPB.bpaddr - sizeof stack, sizeof stack);
	}

/* Finally, ready to transfer control.  Must complete context first.	*/

	if (LPB.flags & SEG) {				/* Segmented    */
		context.regs[14] = (short)(LPB.stackptr >> 16);
		context.regs[15] = (short)LPB.stackptr;
		context.PC = LPB.pgldaddr;
		context.FCW= 0x9800;
	} else {					/* Nonsegmented!*/
		context.regs[15] = (short) LPB.stackptr;
		context.PC = map_adr(LPB.pgldaddr, TRUE_TPAPROG);
		context.FCW= 0x1800;
	}
	xfer(map_adr((XADDR) &context, MYDATA));	/* Go for it!	*/
}
