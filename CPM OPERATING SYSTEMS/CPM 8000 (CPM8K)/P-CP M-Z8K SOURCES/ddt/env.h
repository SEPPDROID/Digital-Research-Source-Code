/*
********	Environmental varibles for z8000dm
*/

#define	Z8001	1

#ifdef	CPM

#include "portab.h"
#include "cpm.h"
#include "stdio.h"
#include "bdos.h"

#define	NREGIONS	2	/* number of regions in the MRT		*/
#define	BGETMRT	18		/* SC to get MRT			*/
#define	PGLOAD	59		/* BDOS call to load program		*/
#define	SECLEN	128		/* size of a sector			*/

struct b_page
	{
		XADDR		ltpa;		/* Low TPA address	    */	
		XADDR		htpa;		/* High TPA address	    */	
		XADDR		lcode;		/* Start address of code seg*/
		long		codelen;	/* Code segment length	    */
		XADDR		ldata;		/* Start address of data seg*/
		long		datalen;	/* Data segment length	    */
		XADDR		lbss;		/* Start address of bss seg */
		long		bsslen;		/* Bss segment length	    */
		long		freelen;	/* Free segment length	    */
		char		resvd1[20];	/* Reserved area	    */
		struct fcbtab	fcb2;		/* Second basepage FCB	    */
		struct fcbtab	fcb1;		/* First basepage FCB	    */
		char		buff[128];	/* Default DMA buffer,	    */
						/*   command line tail	    */
	};


struct lpb {
	XADDR	fcbaddr;	/* Address of fcb of opened file	*/
	XADDR	pgldaddr;	/* Low address of prog load area	*/
	XADDR	pgtop;		/* High address of prog load area, +1	*/
	XADDR	bpaddr;		/* Address of basepage; return value	*/
	XADDR	stackptr;	/* Stack ptr of user; return value	*/
	short 	lcflags;	/* Loader control flags; return value	*/
} LPB;

struct m_rt {			/* The Memory Region Table		*/
	int entries;
	struct {
		XADDR	m_low;
		XADDR	m_len;
	} m_reg[NREGIONS];
};

struct ustack	{		/* User's initial stack (nonsegmented)	*/
	short	two;		/* "Return address" (actually a call to */
				/*   BDOS warm boot in runtime startup)	*/
	short	bpgaddr;	/* Input parameter: pointer to basepage	*/
};

extern long	map_adr();

#else

#define	BYTE	char
#define	WORD	short
#define	LONG	long

#endif

#define	POINTER	long
#define	OPCODE	short
#define	BKPT	0x7F00

#define	COLS	256
#define	SWIDTH	64

	/* swap macros */
#define	MSWP(a)	(a)
#define	PSWP(a)	(a)

	/* globals from env.c */
#define	MEMSIZE	0x100
extern	BYTE	memory[];
extern	BYTE	ldchr();
extern	WORD	ldint();
extern	int	memode;

	/* globals from dbg.c */
extern	int	tractr;
extern	POINTER	temppc;

/*
********	Z8000 Processor state frame
*/

union	pstate	{
	struct	{
		WORD	r0;	/* 00 */
		WORD	r1;	/* 02 */
		WORD	r2;	/* 04 */
		WORD	r3;	/* 06 */
		WORD	r4;	/* 08 */
		WORD	r5;	/* 0a */
		WORD	r6;	/* 0c */
		WORD	r7;	/* 0e */
		WORD	r8;	/* 10 */
		WORD	r9;	/* 12 */
		WORD	ra;	/* 14 */
		WORD	rb;	/* 16 */
		WORD	rc;	/* 18 */
		WORD	rd;	/* 1a */
		WORD	re;	/* 1c */
		WORD	rf;	/* 1e */
		WORD	id;	/* 20 */
		WORD	fcw;	/* 22 */
#ifdef	Z8001
		WORD	pcs;	/* 24 */
#endif
		WORD	pc;	/* 24-26 */
	} lregs;
	struct	{
		BYTE	rh0;	BYTE	rl0;
		BYTE	rh1;	BYTE	rl1;
		BYTE	rh2;	BYTE	rl2;
		BYTE	rh3;	BYTE	rl3;
		BYTE	rh4;	BYTE	rl4;
		BYTE	rh5;	BYTE	rl5;
		BYTE	rh6;	BYTE	rl6;
		BYTE	rh7;	BYTE	rl7;
		BYTE	rh8;	BYTE	rl8;
		BYTE	rh9;	BYTE	rl9;
		BYTE	rha;	BYTE	rla;
		BYTE	rhb;	BYTE	rlb;
		BYTE	rhc;	BYTE	rlc;
		BYTE	rhd;	BYTE	rld;
		BYTE	rhe;	BYTE	rle;
		BYTE	rhf;	BYTE	rlf;
		BYTE	id1;	BYTE	id2;
		BYTE	f;	BYTE	f1;
	} sregs;
};
