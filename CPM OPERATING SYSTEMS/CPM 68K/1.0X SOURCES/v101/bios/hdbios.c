/*=======================================================================*/
/*/---------------------------------------------------------------------\*/
/*|									|*/
/*|     CP/M-68K(tm) BIOS for the EXORMACS 				|*/
/*|									|*/
/*|     Copyright 1982, Digital Research.				|*/
/*|									|*/
/*|	Modified 9/7/82 wbt						|*/
/*|		10/5/82 wbt						|*/
/*|									|*/
/*\---------------------------------------------------------------------/*/
/*=======================================================================*/

#define LOADER  0		/* 1 for Loader BIOS, 0 for normal BIOS */


#include "[bill.cpm68k.csupport]stdtypes.h"

char copyright[] = "Copyright 1982, Digital Research";

struct memb { BYTE byte; };	/* use for peeking and poking memory */
struct memw { WORD word; };
struct meml { LONG lword;};


/************************************************************************/ 
/*      I/O Device Definitions						*/
/************************************************************************/


/************************************************************************/ 
/*      Define the two serial ports on the DEBUG board			*/
/************************************************************************/

/* Port Addresses */

#define PORT1 0xFFEE011
#define PORT2 0xFFEE015
 
/* Port Offsets */

#define PORTCTRL 0	/* Control Register */
#define PORTSTAT 0	/* Status  Register */
#define PORTRDR  2	/* Read  Data Register */
#define PORTTDR  2	/* Write Data Register */

/* Port Control Functions */
 
#define PORTRSET 3	/* Port Reset */
#define PORTINIT 0x11	/* Port Initialize */

/* Port Status Values */
 
#define PORTRDRF 1	/* Read  Data Register Full */
#define PORTTDRE 2	/* Write Data Register Empty */
 

/************************************************************************/
/* Define Disk I/O Addresses and Related Constants			*/
/************************************************************************/

#define DSKIPC		0xFF0000	/* IPC Base Address */

#define DSKINTV		0x3FC		/* Address of Disk Interrupt Vector */

#define INTTOIPC	0xD		/* offsets in mem mapped io area */
#define RSTTOIPC	0xF
#define MSGTOIPC	0x101
#define ACKTOIPC	0x103
#define	PKTTOIPC	0x105
#define	MSGFMIPC	0x181
#define ACKFMIPC	0x183
#define PKTFMIPC	0x185

#define DSKREAD		0x10		/* disk commands */
#define DSKWRITE	0x20

/* Some characters used in disk controller packets */

#define	STX	0x02
#define ETX	0x03
#define	ACK	0x06
#define NAK	0x15

#define PKTSTX		0x0		/* offsets within a disk packet */
#define PKTID		0x1
#define PKTSZ		0x2
#define PKTDEV		0x3
#define PKTCHCOM	0x4
#define PKTSTCOM	0x5
#define	PKTSTVAL	0x6
#define PKTSTPRM	0x8
#define	STPKTSZ		0xf


/************************************************************************/
/* BIOS  Table Definitions						*/
/************************************************************************/

/* Disk Parameter Block Structure */

struct dpb
{
	WORD	spt;
	BYTE	bsh;
	BYTE	blm;
	BYTE	exm;
	BYTE	dpbjunk;
	WORD	dsm;
	WORD	drm;
	BYTE	al0;
	BYTE	al1;
	WORD	cks;
	WORD	off;
};


/* Disk Parameter Header Structure */

struct dph
{
	BYTE	*xltp;
	WORD	 dphscr[3];
	BYTE	*dirbufp;
struct	dpb	*dpbp;
	BYTE	*csvp;
	BYTE	*alvp;
};



/************************************************************************/
/*	Directory Buffer for use by the BDOS				*/
/************************************************************************/

BYTE dirbuf[128];

#if ! LOADER

/************************************************************************/
/*	CSV's								*/
/************************************************************************/

BYTE	csv0[16];
BYTE	csv1[16];
BYTE	csv2[256];
BYTE	csv3[256];


/************************************************************************/
/*	ALV's								*/
/************************************************************************/

BYTE	alv0[32];	/* (dsm0 / 8) + 1	*/
BYTE	alv1[32];	/* (dsm1 / 8) + 1	*/
BYTE	alv2[412];	/* (dsm2 / 8) + 1	*/
BYTE	alv3[412];	/* (dsm2 / 8) + 1	*/

#endif
 
/************************************************************************/
/*	Disk Parameter Blocks						*/
/************************************************************************/

/* The following dpb definitions express the intent of the writer,	*/
/* unfortunately, due to a compiler bug, these lines cannot be used.	*/
/* Therefore, the obscure code following them has been inserted.	*/

/*************    spt, bsh, blm, exm, jnk,   dsm,  drm,  al0, al1, cks, off 

struct dpb dpb0={  26,   3,   7,   0,   0,   242,   63, 0xC0,   0,  16,   2};
struct dpb dpb2={  32,   5,  31,   1,   0,  3288, 1023, 0xFF,   0, 256,   4};

********** end of readable definitions  *************/

/* The Alcyon C compiler assumes all structures are arrays of int, so 	*/
/* in the following definitions, adjacent pairs of chars have been 	*/
/* combined into int constants --- what a kludge!  **********************/

struct dpb dpb0 = {  26,  775,   0,   242,   63, -16384,  16, 2 };
struct dpb dpb2 = {  32, 1311, 256,  3288, 1023, 0xFF00, 256, 4 };

/*************** End of kludge *****************/



/************************************************************************/
/* Sector Translate Table						*/ 
/************************************************************************/

BYTE	xlt[26] = {  0,  6, 12, 18, 24,  4, 10, 16, 22,  2,  8, 14, 20,
		     1,  7, 13, 19, 25,  5, 11, 17, 23,  3,  9, 15, 21 };


 
/************************************************************************/
/* Disk Parameter Headers						*/
/*									*/
/* Four disks are defined : dsk a: diskno=0, (Motorola's #fd04)		*/
/*			    dsk b: diskno=1, (Motorola's #fd05)		*/
/*			    dsk c: diskno=2, (Motorola's #hd00)		*/
/*			    dsk d: diskno=3, (Motorola's #hd01)		*/
/************************************************************************/

#if ! LOADER

/* Disk Parameter Headers */

struct dph dphtab[4] =
		{ {&xlt, 0, 0, 0, &dirbuf, &dpb0, &csv0, &alv0}, /*dsk a*/
		  {&xlt, 0, 0, 0, &dirbuf, &dpb0, &csv1, &alv1}, /*dsk b*/
		  {  0L, 0, 0, 0, &dirbuf, &dpb2, &csv2, &alv2}, /*dsk c*/
		  {  0L, 0, 0, 0, &dirbuf, &dpb2, &csv3, &alv3}, /*dsk d*/
		};
#else

struct dph dphtab[4] =
		{ {&xlt, 0, 0, 0, &dirbuf, &dpb0,    0L,    0L}, /*dsk a*/
		  {&xlt, 0, 0, 0, &dirbuf, &dpb0,    0L,    0L}, /*dsk b*/
		  {  0L, 0, 0, 0, &dirbuf, &dpb2,    0L,    0L}, /*dsk c*/
		  {  0L, 0, 0, 0, &dirbuf, &dpb2,    0L,    0L}, /*dsk d*/
		};
#endif

/************************************************************************/
/*	Memory Region Table						*/
/************************************************************************/

struct mrt {	WORD count;
		LONG tpalow;
		LONG tpalen;
	   }
	memtab = { 1, 0x0000,0x400, 0x0007,0x0c00 };/* kludge init by int */


#if ! LOADER

/************************************************************************/
/*	IOBYTE								*/
/************************************************************************/

WORD iobyte;	/* The I/O Byte is defined, but not used */

#endif


/************************************************************************/
/*	Currently Selected Disk Stuff					*/
/************************************************************************/

WORD settrk, setsec, setdsk;	/* Currently set track, sector, disk */
BYTE *setdma;			/* Currently set dma address */



/************************************************************************/
/*	Track Buffering Definitions and Variables			*/
/************************************************************************/

#if ! LOADER

#define	NUMTB	8 /* Number of track buffers -- must be at least 3 */
		  /* for some algorithms to work properly */

/* Define a track buffer structure */
struct	tbstr {	
		struct	tbstr  *nextbuf;     /* form linked list for LRU  */
			BYTE	buf[32*128]; /* big enough for 1/4 hd trk */
			WORD	dsk;	     /* disk for this buffer  */
			WORD	trk;	     /* track for this buffer */
			BYTE	valid;	     /* buffer valid flag     */
			BYTE	dirty;	     /* true if a BIOS write has   */
					     /*	put data in this buffer,   */
					     /*	but the buffer hasn't been */
					     /* flushed yet.		   */
};

struct tbstr *firstbuf;	/* head of linked list of track buffers */
struct tbstr *lastbuf;  /* tail of ditto */

struct tbstr tbuf[NUMTB];	/* array of track buffers */

#else

/* the loader bios uses only 1 track buffer */

BYTE buf1trk[32*128];	/* big enough for 1/4 hd trk */
BYTE bufvalid;
BYTE buftrk;

#endif


/************************************************************************/
/*	Disk I/O Packets for the UDC and other Disk I/O Variables	*/
/************************************************************************/

struct hmpkst {
		BYTE	a1;
		BYTE	a2;
		BYTE	a3;
		BYTE	dskno;
		BYTE	com1;
		BYTE	com2;
		BYTE	a6;
		BYTE	a7;
	      }
	hmpack = { 512, 1792, 0, 768 }; /* kludge init by words */


struct rwpkst {
		BYTE	stxchr;
		BYTE	pktid;
		BYTE	pktsize;
		BYTE	dskno;
		BYTE	chcmd;
		BYTE	devcmd;
		WORD	numblks;
		WORD	blksize;
		LONG	iobf;
		WORD	cksum;
		LONG	lsect;
		BYTE	etxchr;
		BYTE	rwpad;
	      };

struct rwpkst rwpack = { 512, 5376, 4097, 13, 256, 0, 0, 0, 0, 0, 768 };

#if ! LOADER

struct fmtpkst {
		BYTE	fmtstx;
		BYTE	fmtid;
		BYTE	fmtsize;
		BYTE	fmtdskno;
		BYTE	fmtchcmd;
		BYTE	fmtdvcmd;
		BYTE	fmtetx;
		BYTE	fmtpad;
	       };

struct fmtpkst fmtpack = { 512, 1792, 0x4002, 0x0300 };

#endif


#define NUMDSKS 4		/* number of disks defined */
#define MAXDSK  (NUMDSKS-1)	/* maximum disk number 	   */

BYTE cnvdsk[NUMDSKS] = { 4, 5, 0, 1 };  /* convert CP/M dsk# to EXORmacs */
BYTE rcnvdsk[6]	     = { 2, 3, 0, 0, 0, 1 }; /* and vice versa */

/* defines for IPC and disk states */

#define IDLE	0
#define ACTIVE	1

WORD ipcstate;	/* current IPC state */
WORD actvdsk;	/* disk number of currently active disk, if any */
LONG intcount;	/* count of interrupts needing to be processed  */



struct dskst {
		WORD	state;	/* from defines above	*/
		BYTE	ready;	/* 0 => not ready	*/
		BYTE	change;	/* 0 => no change	*/
	     }
	dskstate[NUMDSKS];


/************************************************************************/
/*      Generic Serial Port I/O Procedures				*/
/************************************************************************/

portinit(port)
REG BYTE *port;
{
        *(port + PORTCTRL) = PORTRSET;	/* reset the port */
        *(port + PORTCTRL) = PORTINIT;
}



/************************************************************************/ 
/*	Generic serial port status					*/
/************************************************************************/

 
portstat(port)
REG BYTE *port;
{
        if ( *(port + PORTSTAT) & PORTRDRF) return(0xff); /* input ready */
	else				    return(0x00); /* not ready	 */
}
 
 
/************************************************************************/ 
/*	Generic serial port input					*/
/************************************************************************/
 
BYTE portin(port)
REG BYTE *port;
{
        while ( ! portstat(port)) ;		/* wait for input	*/
        return ( 0x7f & *(port + PORTRDR));	/* got some, return it	*/
}
 
 
/************************************************************************/ 
/*	Generic serial port output					*/
/************************************************************************/
 
portout(port, ch)
REG BYTE *port;
REG BYTE ch;
{
	ch = ch & 0x7f;
        while ( ! (*(port + PORTSTAT) & PORTTDRE) ) ; /* wait for ok to send */
        *(port + PORTTDR) = ch;			      /* then send character */
}
 
 
/************************************************************************/ 
/*      Error procedure for BIOS					*/
/************************************************************************/

#if ! LOADER

bioserr(errmsg)
REG BYTE *errmsg;
{
        printstr("\n\rBIOS ERROR -- ");
        printstr(errmsg);
        printstr(".\n\r");
}
 
printstr(s)     /* used by bioserr */
REG BYTE *s;
{ 
        while (*s) {portout(PORT1,*s); s += 1; };
}

#else

bioserr()
{
	l : goto l;
}

#endif

/************************************************************************/
/*	Disk I/O Procedures						*/
/************************************************************************/


EXTERN dskia();
EXTERN setimask();

dskic()
{
	/* Disk Interrupt Handler -- C Language Portion */

	REG BYTE workbyte;
	BYTE	stpkt[STPKTSZ];

	workbyte = (DSKIPC + ACKFMIPC)->byte;
	if ( (workbyte == ACK) || (workbyte == NAK) )
	{
		if ( ipcstate == ACTIVE ) intcount += 1;
		else (DSKIPC + ACKFMIPC)->byte = 0;	/* ??? */
	}

	workbyte = (DSKIPC + MSGFMIPC)->byte;
	if ( workbyte & 0x80 )
	{
		getstpkt(stpkt);

		if ( stpkt[PKTID] == 0xFF )
		{
			/* unsolicited */

			unsolst(stpkt);
			sendack();
		}
		else
		{
			/* solicited */

			if ( ipcstate == ACTIVE ) intcount += 1;
			else sendack();
		}
	}


} /* end of dskic */


getstpkt(stpktp)
REG BYTE *stpktp;
{
	REG BYTE *p, *q;
	REG WORD i;

	p = stpktp;
	q = (DSKIPC + PKTFMIPC);

	for ( i = STPKTSZ; i; i -= 1 )
	{
		*p = *q;
		 p += 1;
		 q += 2;
	}
}


unsolst(stpktp)
REG BYTE *stpktp;
{
	REG WORD dev;
	REG WORD ready;
	REG struct dskst *dsp;

	dev = rcnvdsk[ (stpktp+PKTDEV)->byte ];
	ready = ((stpktp+PKTSTPRM)->byte & 0x80) == 0x0;
	dsp = & dskstate[dev];
	if (  ( ready  && !(dsp->ready) ) ||
	      (!ready) &&  (dsp->ready)     ) dsp->change = 1;
	dsp->ready = ready;
#if ! LOADER
	if ( ! ready ) setinvld(dev);
#endif
}


#if ! LOADER

setinvld(dsk)
REG WORD dsk;
{
	REG struct tbstr *tbp;

	tbp = firstbuf;
	while ( tbp )
	{
		if ( tbp->dsk == dsk ) tbp->valid = 0;
		tbp = tbp->nextbuf;
	}
}

#endif


waitack()
{
	REG WORD imsave;
	REG BYTE work;

	while (1)
	{
	  	while ( ! intcount ) ; /* wait */ 

		imsave = setimask(7);
		intcount -= 1;		
		work = (DSKIPC + ACKFMIPC)->byte;
		if ( (work == ACK) || (work == NAK) )
		{
			(DSKIPC + ACKFMIPC)->byte = 0;
			setimask(imsave);
			return(work == ACK);
		}
		setimask(imsave);
	}
}



sendack()
{
	/* acknowledge a message from the IPC */

	(DSKIPC + MSGFMIPC)->byte = 0;	/* clear message flag */
	(DSKIPC + ACKTOIPC)->byte = ACK;	/* send ACK   */
	(DSKIPC + INTTOIPC)->byte = 0;	/* interrupt IPC      */
}



sendpkt(pktadr, pktsize)
REG BYTE *pktadr;
REG WORD  pktsize;
{
	REG BYTE *iopackp;
	REG WORD  imsave;

	while ( (DSKIPC+MSGTOIPC)->byte ); /* wait til ready */
	(DSKIPC+ACKFMIPC)->byte = 0;
	(DSKIPC+MSGFMIPC)->byte = 0;
	iopackp = (DSKIPC+PKTTOIPC);
	do {*iopackp = *pktadr++; iopackp += 2; pktsize -= 1;} while(pktsize);
	(DSKIPC+MSGTOIPC)->byte = 0x80;
	imsave = setimask(7);
	dskstate[actvdsk].state = ACTIVE;
	ipcstate = ACTIVE;
	intcount = 0L;
	(DSKIPC+INTTOIPC)->byte = 0;
	setimask(imsave);
	waitack();
}


WORD dskwait(dsk, stcom, stval)
REG WORD dsk;
BYTE	 stcom;
WORD	 stval;
{
	REG WORD imsave;
	BYTE stpkt[STPKTSZ];

	imsave = setimask(7);
	while ( (! intcount) &&
		dskstate[dsk].ready && (! dskstate[dsk].change) )
	{
		setimask(imsave); imsave = setimask(7);
	}

	if ( intcount )
	{
		intcount -= 1;
		if ( ( (DSKIPC + MSGFMIPC)->byte & 0x80 ) == 0x80 )
		{
			getstpkt(stpkt);
			setimask(imsave);
			if ( (stpkt[PKTSTCOM] == stcom) &&
			     ( (stpkt+PKTSTVAL)->word == stval ) ) return (1);
			else					   return (0);
		}
	}
	setimask(imsave);
	return(0);
}


dskxfer(dsk, trk, bufp, cmd)
REG WORD  dsk, trk, cmd;
REG BYTE *bufp;
{
	/* build packet */

	REG WORD sectcnt;
	REG WORD result;

	rwpack.dskno = cnvdsk[dsk];
	rwpack.iobf  = bufp;
	sectcnt = (dphtab[dsk].dpbp)->spt;
	rwpack.lsect = trk * (sectcnt >> 1);
	rwpack.chcmd = cmd;
	rwpack.numblks = (sectcnt >> 1);
	actvdsk = dsk;
	dskstate[dsk].change = 0;
	sendpkt(&rwpack, 21);
	result = dskwait(dsk, 0x70, 0x0);
	sendack();
	dskstate[dsk].state = IDLE;
	ipcstate = IDLE;
	return(result);
}


#if ! LOADER

flush1(tbp)
struct tbstr *tbp;
{
	REG WORD ok;

	if ( tbp->valid && tbp->dirty )
		ok = dskxfer(tbp->dsk, tbp->trk, tbp->buf, DSKWRITE);
	else ok = 1;

	tbp->dirty = 0;		/* even if error, mark not dirty */
	tbp->valid = ok;	/* otherwise system has trouble  */
				/* continuing.			 */
	return(ok);
}


flush()
{
	REG struct tbstr *tbp;
	REG WORD ok;

	ok = 1;
	tbp = firstbuf;
	while (tbp)
	{
		if ( ! flush1(tbp) ) ok = 0;
		tbp = tbp->nextbuf;
	}
	return(ok);
}



fill(tbp)
REG struct tbstr *tbp;
{
	REG WORD ok;

	if ( tbp->valid && tbp->dirty ) ok = flush1(tbp);
	else ok = 1;

	if (ok) ok = dskxfer(setdsk, settrk, tbp->buf, DSKREAD);

	tbp->valid = ok;
	tbp->dirty = 0;
	tbp->trk = settrk;
	tbp->dsk = setdsk;

	return(ok);
}


struct tbstr *gettrk()
{
	REG struct tbstr *tbp;
	REG struct tbstr *ltbp;
	REG struct tbstr *mtbp;
	REG WORD imsave;

	imsave = setimask(7);
	if ( ! dskstate[setdsk].ready )
	{
		setimask(imsave);
		tbp = 0L;
		return (tbp);
	}
	tbp = firstbuf;
	ltbp = 0;
	mtbp = 0;

	while (tbp)
	{
		if ( (tbp->valid) && (tbp->dsk == setdsk) 
		     && (tbp->trk == settrk) )
		{
			if (ltbp)
			{
				ltbp->nextbuf = tbp->nextbuf;
				tbp->nextbuf  = firstbuf;
				firstbuf      = tbp;
			}
			setimask(imsave);
			return tbp;
		}
		else
		{
			mtbp = ltbp;
			ltbp = tbp;
			tbp  = tbp->nextbuf;
		}
	}

	if (mtbp) mtbp->nextbuf = 0;	/* detach lru buffer */
	if ( (! firstbuf->valid) || 
	     (firstbuf->trk > dphtab[firstbuf->dsk].dpbp->off) ) /* dir trk? */
	{
		ltbp->nextbuf = firstbuf;
		firstbuf = ltbp;
	}
	else
	{
		ltbp->nextbuf = firstbuf->nextbuf;
		firstbuf->nextbuf = ltbp;
	}
	setimask(imsave);
	if (flush1(ltbp) && fill(ltbp)) mtbp = ltbp;
	else				mtbp = 0L ;
	return (mtbp);
}



read()
{
	REG BYTE	*p;
	REG BYTE	*q;
	REG WORD	 i;
	REG struct tbstr *tbp;

	tbp = gettrk();

	if ( ! tbp ) return(1); /* failure */

	p = (tbp->buf) + (setsec << 7);	/* multiply by shifting */
	q = setdma;
	i = 128;
	do {*q++ = *p++; i -= 1;} while (i);
	return(0);
}


write(mode)
BYTE mode;
{
	REG BYTE	*p;
	REG BYTE	*q;
	REG WORD	 i;
	REG struct tbstr *tbp;

	tbp = gettrk();
	if ( ! tbp ) return (1); /* failure */

	p = (tbp->buf) + (setsec << 7);	/* multiply by shifting */
	q = setdma;
	i = 128;
	do {*p++ = *q++; i -= 1;} while (i);
	tbp->dirty = 1;
	if ( mode == 1 ) { if ( flush() ) return(0); else return(1); }
	else return(0);
}

#else

read()
{
	REG BYTE *p;
	REG BYTE *q;
	REG WORD  i;

	if ( ( (! bufvalid) || (buftrk != settrk) ) &&
	     ( ! dskxfer(setdsk, settrk, buf1trk, DSKREAD) ) ) {return(1);}
	bufvalid = 1;
	buftrk = settrk;
	p = buf1trk + (setsec << 7);
	q = setdma;
	i = 128;
	do { *q++ = *p++; i-=1; } while(i);
	return(0);
}

#endif


WORD sectran(s, xp)
REG WORD  s;
REG BYTE *xp;
{
	if (xp) return (WORD)xp[s];
	else	return s;
}


LONG setxvect(vnum, vval)
WORD vnum;
LONG vval;
{
	REG LONG  oldval;
	REG BYTE *vloc;

	vloc = ( (long)vnum ) << 2;
	oldval = vloc->lword;
	vloc->lword = vval;

	return(oldval);	

}


LONG slctdsk(dsk, logged)
REG BYTE dsk;
    BYTE logged;
{
	REG struct dph	*dphp;
	REG BYTE     st1, st2;
	BYTE	stpkt[STPKTSZ];

#if ! LOADER
	if ( (dsk > MAXDSK) || ( dsk == 2 ) )
	{
		printstr("\n\rBIOS ERROR -- DISK ");
		portout(PORT1, 'A'+dsk);
		printstr(" NOT SUPPORTED\n\r");
		return(0L);
	}
#endif

	setdsk = dsk;
	dphp = &dphtab[dsk];
	if ( ! (logged & 0x1) )
	{

		hmpack.dskno = cnvdsk[setdsk];
		hmpack.com1  = 0x30;
		hmpack.com2  = 0x02;
		actvdsk = dsk;
		dskstate[dsk].change = 0;
		sendpkt(&hmpack, 7);
		if ( ! dskwait(dsk, 0x72, 0x0) )
		{
			sendack();
			ipcstate = IDLE;
			return ( 0L );
		}
		getstpkt(stpkt);	/* determine disk type and size */
		sendack();
		ipcstate = IDLE;
		st1 = stpkt[PKTSTPRM];
		st2 = stpkt[PKTSTPRM+1];

		if ( st1 & 0x80 )	/* not ready / ready */
		{
			dskstate[dsk].ready = 0;
			return(0L);
		}
		else
			dskstate[dsk].ready = 1;

		switch ( st1 & 7 )
		{
		   case 1 :	/* floppy disk	*/

				dphp->dpbp = &dpb0;
				break;

		   case 2 :	/* hard disk	*/

				dphp->dpbp = &dpb2;
				break;

		   default :	bioserr("Invalid Disk Status");
				dphp = 0L;
				break;
		}
	}
	return(dphp);
}


#if ! LOADER
/****************************************************************/
/*								*/
/*	This function is included as an undocumented, 		*/
/*	unsupported method for EXORmacs users to format		*/
/*	disks.  It is not a part of CP/M-68K proper, and	*/
/*	is only included here for convenience, since the	*/
/*	Motorola disk controller is somewhat complex to		*/
/*	program, and the BIOS contains supporting routines.	*/
/*								*/
/****************************************************************/

format(dsk)
REG WORD dsk;
{
	REG WORD retval;

	slctdsk( (BYTE)dsk, (BYTE) 1 );

	fmtpack.dskno = cnvdsk[setdsk];
	actvdsk = setdsk;
	dskstate[setdsk].change = 0;
	sendpkt(&fmtpack, 7);
	if ( ! dskwait(setdsk, 0x70, 0x0) ) retval = 0;
	else				    retval = 1;
	sendack();
	ipcstate = IDLE;
	return(retval);
}

#endif 

/************************************************************************/
/*									*/
/*	Bios initialization.  Must be done before any regular BIOS	*/
/*	calls are performed.						*/
/*									*/
/************************************************************************/

biosinit()
{
	initprts();
	initdsks();
}

initprts()
{
        portinit(PORT1);
        portinit(PORT2);
}

initdsks()
{
	REG WORD i;
	REG WORD imsave;

#if ! LOADER
	for ( i = 0; i < NUMTB; ++i )
	{
		tbuf[i].valid = 0;
		tbuf[i].dirty = 0;
		if ( (i+1) < NUMTB ) tbuf[i].nextbuf = &tbuf[i+1];
		else		     tbuf[i].nextbuf = 0;
	}
	firstbuf = &tbuf[0];
	lastbuf  = &tbuf[NUMTB-1];
#else
	bufvalid = 0;
#endif

	for ( i = 0; i <= MAXDSK; i += 1)
	{
		dskstate[i].state  = IDLE;
		dskstate[i].ready  = 1;
		dskstate[i].change = 0;
	}

	imsave = setimask(7);  /* turn off interrupts */
	intcount = 0;
	ipcstate = IDLE;
	setimask(imsave);      /* turn on interrupts */
}
 


/************************************************************************/
/*									*/
/*      BIOS MAIN ENTRY -- Branch out to the various functions.		*/
/*									*/
/************************************************************************/
 
LONG cbios(d0, d1, d2)
WORD	d0;
LONG	d1, d2;
{

	switch(d0)
	{
#if ! LOADER
		case 0:	init();				/* INIT		*/
			break;

		case 1:	flush();			/* WBOOT	*/
			initdsks();
			wboot();
			break;
#endif
		case 2:	return(portstat(PORT1));	/* CONST	*/
			break;

		case 3: return(portin(PORT1));		/* CONIN	*/
			break;

		case 4: portout(PORT1, (char)d1);	/* CONOUT	*/
			break;

		case 5:	;				/* LIST		*/
		case 6: portout(PORT2, (char)d1);	/* PUNCH	*/
			break;

		case 7:	return(portin(PORT2));		/* READER	*/
			break;

		case 8:	settrk = 0;			/* HOME		*/
			break;

		case 9:	return(slctdsk((char)d1, (char)d2)); /* SELDSK	*/
			break;

		case 10: settrk = (int)d1;		/* SETTRK	*/
			 break;

		case 11: setsec = (int)d1;		/* SETSEC	*/
			 break;

		case 12: setdma = d1;			/* SETDMA	*/
			 break;

		case 13: return(read());		/* READ		*/
			 break;
#if ! LOADER
		case 14: return(write((char)d1));	/* WRITE	*/
			 break;
#endif
		case 15: return(portstat(PORT2));	/* LISTST	*/
			 break;

		case 16: return(sectran((int)d1, d2));	/* SECTRAN	*/
			 break;
#if ! LOADER
		case 18: return(&memtab);		/* GMRTA	*/
			 break;

		case 19: return(iobyte);		/* GETIOB	*/
			 break;

		case 20: iobyte = (int)d1;		/* SETIOB	*/
			 break;

		case 21: if (flush()) return(0L);	/* FLUSH	*/
			 else	      return(0xffffL);
			 break;
#endif
		case 22: return(setxvect((int)d1,d2));	/* SETXVECT	*/
			 break;
#if ! LOADER
		/**********************************************************/
		/*       This function is not part of a standard BIOS.	  */
		/*	 It is included only for convenience, and will	  */
		/*	 not be supported in any way, nor will it	  */
		/* 	 necessarily be included in future versions of	  */
		/* 	 CP/M-68K					  */
		/**********************************************************/
		case 63: return( ! format((int)d1) );	/* Disk Formatter */
			 break;
#endif
	} /* end switch */


} /* END OF BIOS */
 
 
 
/* End of C Bios */
