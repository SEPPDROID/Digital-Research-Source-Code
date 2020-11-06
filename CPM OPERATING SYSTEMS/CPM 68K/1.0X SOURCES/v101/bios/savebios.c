/*=======================================================================*/
/*/---------------------------------------------------------------------\*/
/*|									|*/
/*|     CP/M-68K(tm) BIOS for the EXORMACS 				|*/
/*|									|*/
/*|     Copyright 1982, Digital Research.				|*/
/*|									|*/
/*\---------------------------------------------------------------------/*/
/*=======================================================================*/

#define DEBUG 0

#define TRACEPORT 0xffee015

#include "[bill.cpm68k.csupport]stdtypes.h"

char copyright[] = "Copyright 1982, Digital Research";

struct memb { BYTE byte; };
struct memw { WORD word; };
struct meml { LONG lword;};


/************************************************************************/ 
/*      I/O Device Definitions						*/
/************************************************************************/




/************************************************************************/ 
/*      Define the two ACIA ports on the DEBUG board			*/
/************************************************************************/

#define PORT1 0xFFEE011
#define PORT2 0xFFEE015
 
#define PORTCTRL 0
#define PORTSTAT 0
#define PORTRDR  2
#define PORTTDR  2
 
#define PORTRSET 3
#define PORTINIT 0x11
 
#define PORTRDRF 1
#define PORTTDRE 2
 

/************************************************************************/
/* Define Disk I/O Addresses and Related Constants			*/
/************************************************************************/

#define DSKIPC		0xFF0000	/* IPC Base Address */

#define DSKINTV		0x3FC		/* Address of Disk Interrupt Vector */

#define INTTOIPC	0xD		/* offsets */
#define RSTTOIPC	0xF
#define MSGTOIPC	0x101
#define ACKTOIPC	0x103
#define	PKTTOIPC	0x105
#define	MSGFMIPC	0x181
#define ACKFMIPC	0x183
#define PKTFMIPC	0x185

#define DSKREAD		0x10
#define DSKWRITE	0x20

#define	STX	0x02
#define ETX	0x03
#define	ACK	0x06
#define NAK	0x15



/************************************************************************/
/* BIOS  Table Definitions						*/
/************************************************************************/

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


/************************************************************************/
/*	CSV's								*/
/************************************************************************/

BYTE	csv0[16];
BYTE	csv1[16];
BYTE	csv2[16];


/************************************************************************/
/*	ALV's								*/
/************************************************************************/

BYTE	alv0[32];	/* (dsm0 / 8) + 1	*/
BYTE	alv1[32];	/* (dsm1 / 8) + 1	*/
BYTE	alv2[2002];	/* (dsm2 / 8) + 1	*/

 
/************************************************************************/
/*	Disk Parameter Blocks						*/
/************************************************************************/

/* The following dpb definitions express the intent of the writer,	*/
/* unfortunately, due to a compiler bug, these lines cannot be used.	*/
/* Therefore, the obscure code following them has been inserted.	*/

/*************   spt, bsh, blm, exm, jnk,   dsm, drm,  al0, al1, cks, off 

struct dpb dpb0={ 26,   3,   7,   0,   0,   242,  63, 0xC0,   0,  16,   2};
struct dpb dpb1={ 26,   4,  15,   0,   0,   242,  63, 0xC0,   0,  16,   2};
struct dpb dpb2={ 26,   3,   7,   0,   0, 16008,  63, 0xC0,   0,  16,   2};

********** end of readable definitions  *************/

/* The Alcyon C compiler assumes all structures are arrays of int, so 	*/
/* in the following definitions, adjacent pairs of chars have been 	*/
/* combined into int constants --- what a kludge!  **********************/

struct dpb dpb0 = { 26,  775, 0,   242, 63, -16384, 16, 2 };
struct dpb dpb1 = { 26, 1039, 0,   242, 63, -16384, 16, 2 };
struct dpb dpb2 = { 26,  775, 0, 16008, 63, -16384, 16, 2 };

/*************** End of kludge *****************/



/************************************************************************/
/* Sector Translate Table						*/ 
/************************************************************************/

BYTE	xlt[26] = {  1,  7, 13, 19, 25,  5, 11, 17, 23,  3,  9, 15, 21,
		     2,  8, 14, 20, 26,  6, 12, 18, 24,  4, 10, 16, 22 };


 
/************************************************************************/
/* Disk Parameter Headers						*/
/*									*/
/* Four disks are defined : dsk a: diskno=0, dev name=#fd04		*/
/*			    dsk b: diskno=1, dev name=#fd05		*/
/*			    dsk c: diskno=2, dev name=#hd00		*/
/*			    dsk d: diskno=3, dev name=#hd01		*/
/************************************************************************/

struct dph dphtab[4] =
		{ {&xlt, 0, 0, 0, &dirbuf, &dpb0, &csv0, &alv0}, /*dsk a*/
		  {&xlt, 0, 0, 0, &dirbuf, &dpb0, &csv1, &alv1}, /*dsk b*/
		  {  0L, 0, 0, 0, &dirbuf, &dpb2, &csv2, &alv2}, /*dsk c*/
		  {  0L, 0, 0, 0, &dirbuf, &dpb2, &csv2, &alv2}, /*dsk d*/
		};



/************************************************************************/
/*	Memory Region Table						*/
/************************************************************************/

struct mrt {	WORD count;
		LONG tpalow;
		LONG tpalen;
	   }
	memtab = { 1, 0x0002,0x0000, 0x0003,0xfff0 };/* kludge init by int */


/************************************************************************/
/*	IOBYTE								*/
/************************************************************************/

WORD iobyte;


/************************************************************************/
/*	Currently Selected Disk Stuff					*/
/************************************************************************/

WORD settrk, setsec, setdsk;
BYTE *setdma;



/************************************************************************/
/*	Track Buffering Definitions and Variables			*/
/************************************************************************/

#define	NUMTB	10	/* must be at least 3 for some algorithms */

struct	tbstr {	
		struct	tbstr  *nextbuf;
			BYTE	buf[26*128];
			WORD	dsk;
			WORD	trk;
			BYTE	valid;
			BYTE	dirty;
};

struct tbstr *firstbuf;
struct tbstr *lastbuf;

struct tbstr tbuf[NUMTB];




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


BYTE cnvdsk[4] = { 4, 5, 0, 1 };  /* convert from CP/M dsk # to Exormacs */

#define MAXDSK 3


#define DSKIDLE  0
#define MSGSENT	 1
#define NAKRCVD	 2
#define ACKRCVD  3
#define IODONE	 4
#define DSKERROR 5


WORD dskstate = DSKIDLE;


/************************************************************************/
/*      Generic Serial Port I/O Procedures				*/
/************************************************************************/
 
portinit(port)
REG BYTE *port;
{
        *(port + PORTCTRL) = PORTRSET;
        *(port + PORTCTRL) = PORTINIT;
}
 
 
 
 
 
portstat(port)
REG BYTE *port;
{
        return ( *(port + PORTSTAT) & PORTRDRF) == PORTRDRF;
}
 
 
 
 
BYTE portin(port)
REG BYTE *port;
{
        while (portstat(port) == 0) ;
        return *(port + PORTRDR);
}
 
 
 
 
portout(port, ch)
REG BYTE *port;
REG BYTE ch;
{
        while ( (*(port + PORTSTAT) & PORTTDRE) != PORTTDRE) ;
        *(port + PORTTDR) = ch;
}
 
 
/************************************************************************/ 
/*      Error procedure for BIOS					*/
/************************************************************************/

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

#if DEBUG

printrace(s)
REG BYTE *s;
{
	while (*s) {portout(TRACEPORT, *s); s += 1;}
}

printnum(n)
REG LONG n;
{
	REG WORD i;
	REG BYTE d;

	i = 32;

	while (i)
	{
		i -= 4;
		d = (n >> i) & 0x0fL;
		if ( d <= '9' ) d = d + '0';
		else		d = d - 10 + 'a';
		portout(TRACEPORT, d);
	}
	portout(TRACEPORT, ' ');
}


printds(ds)
WORD ds;
{
	switch(ds){
		case DSKIDLE:	printrace("DSKIDLE");
				break;
		case MSGSENT:	printrace("MSGSENT");
				break;
		case NAKRCVD:	printrace("NAKRCVD");
				break;
		case ACKRCVD:	printrace("ACKRCVD");
				break;
		case IODONE:	printrace("IODONE");
				break;
		case DSKERROR:	printrace("DSKERROR");
				break;
		default:	printrace("Invalid");
				break;
	}
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

#if DEBUG

	printrace("\n\rDisk Interrupt ");
	printds(dskstate);
	printrace("->");	

#endif

	switch ( dskstate )
	{
	  case DSKIDLE:	;
	  case IODONE:	;
	  case DSKERROR:;
	  case NAKRCVD:	{
				if ( ((DSKIPC+ACKFMIPC)->byte == ACK) ||
				     ((DSKIPC+ACKFMIPC)->byte == NAK)  )
				{
				   (DSKIPC+ACKFMIPC)->byte = 0;
				}
				
				if ( (DSKIPC+MSGFMIPC)->byte == 0x80 )
				{
				   (DSKIPC+ACKTOIPC)->byte = ACK;
				   (DSKIPC+MSGFMIPC)->byte = 0;
				   (DSKIPC+INTTOIPC)->byte = 0;
				}
			}
			break;

	  case MSGSENT:	{
				if ( (DSKIPC+ACKFMIPC)->byte == ACK )
				{
				   (DSKIPC+ACKFMIPC)->byte = 0;
				   dskstate = ACKRCVD;
				}
				else if ( (DSKIPC+ACKFMIPC)->byte == NAK )
				{
				   (DSKIPC+ACKFMIPC)->byte = 0;
				   dskstate = NAKRCVD;
				}
				else
				{
				   bioserr("Expected ACK or NAK");
				   dskstate = DSKERROR;
				   return;
				}

				if ( (DSKIPC+MSGFMIPC)->byte == 0x80 )
				{ dskstate = IODONE; }
			}
			break;

	  case ACKRCVD:	{
				if ( (DSKIPC+MSGFMIPC)->byte != 0x80 )
				{ bioserr("Expected status packet from IPC");}
				else dskstate = IODONE;

				(DSKIPC+MSGFMIPC)->byte = 0;
				(DSKIPC+ACKTOIPC)->byte = ACK;
				(DSKIPC+INTTOIPC)->byte = 0;
			}
			break;
	}

#if DEBUG
	printds(dskstate);
	printrace("\n\r");
#endif

} /* end of dskic */


sendpkt(pktadr, pktsize)
REG BYTE *pktadr;
REG WORD  pktsize;
{
	REG BYTE *iopackp;
	REG WORD  imsave;

	while ( (DSKIPC+MSGTOIPC)->byte ); /* wait til ready */
	if ( (DSKIPC+ACKFMIPC)->byte == NAK ) bioserr("NAK from IPC");
	(DSKIPC+ACKFMIPC)->byte = 0;
	(DSKIPC+MSGFMIPC)->byte = 0;
	iopackp = (DSKIPC+PKTTOIPC);
	do {*iopackp = *pktadr++; iopackp += 2; pktsize -= 1;} while(pktsize);
	(DSKIPC+MSGTOIPC)->byte = 0x80;
	imsave = setimask(7);
	dskstate = MSGSENT;
	(DSKIPC+INTTOIPC)->byte = 0;
	setimask(imsave);
}


dskxfer(dsk, trk, bufp, cmd)
REG WORD  dsk, trk, cmd;
REG BYTE *bufp;
{
	/* build packet */
	rwpack.dskno = cnvdsk[dsk];
	rwpack.iobf  = bufp;
	rwpack.lsect = trk * 13;
	rwpack.chcmd = cmd;
	sendpkt(&rwpack, 21);
	while (dskstate == MSGSENT); /* wait */
	if (dskstate == NAKRCVD) bioserr("NAK from IPC");
	while (dskstate == ACKRCVD); /* wait */

}




flush1(tbp)
REG struct tbstr *tbp;
{
	if ( tbp->valid && tbp->dirty )
		dskxfer(tbp->dsk, tbp->trk, tbp->buf, DSKWRITE);
	
	tbp->dirty = 0;
	
	return(0);
}


flush()
{
	REG struct tbstr *tbp;

	tbp = firstbuf;
	while (tbp)
	{
		flush1(tbp);
		tbp = tbp->nextbuf;
	}
}


fill(tbp)
REG struct tbstr *tbp;
{

	if ( tbp->valid && tbp->dirty ) flush1(tbp);

	dskxfer(setdsk, settrk, tbp->buf, DSKREAD);
		
	tbp->valid = 1;
	tbp->dirty = 0;
	tbp->trk	= settrk;
	tbp->dsk	= setdsk;

}


struct tbstr *gettrk()
{
	REG struct tbstr *tbp;
	REG struct tbstr *ltbp;
	REG struct tbstr *mtbp;

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
	flush1(ltbp);
	fill(ltbp);
	return ltbp;
}



read()
{
	REG BYTE	*p;
	REG BYTE	*q;
	REG WORD	 i;
	REG struct tbstr *tbp;

	tbp = gettrk();
	p = (tbp->buf) + ((setsec-1) << 7);	/* multiply by shifting */
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
	p = (tbp->buf) + ((setsec-1) << 7);	/* multiply by shifting */
	q = setdma;
	i = 128;
	if ( (mode != 1) || (tbp->dirty) )	/* normal case */
	{
		do {*p++ = *q++; i -= 1;} while (i);
		tbp->dirty = 1;
	}
	else	/* for dir write, only mark dirty if changed. */
	{
		do
		{
			if (*p != *q) tbp->dirty=1;
			*p++ = *q++;
			i -= 1;
		}
		while (i);
	}
	if ( mode == 1 ) flush();
	return(0);
}



BYTE sectran(s, xp)
WORD	 s;
BYTE	*xp;
{
	return xp[s];
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


LONG seldsk(dsk, logged)
REG BYTE dsk;
    BYTE logged;
{
	REG struct dph *dphp;
	REG BYTE	check;

	if (dsk > MAXDSK) return(0L);
	setdsk = dsk;
	dphp = &dphtab[dsk];
	if ( ! logged )
	{

		hmpack.dskno = cnvdsk[setdsk];
		hmpack.com1  = 0x30;
		hmpack.com2  = 0x02;
		(DSKIPC+ACKFMIPC)->byte = 6;
		if ( (DSKIPC+MSGTOIPC)->byte ) {bioserr("ERR 6"); return 0L;}
		else sendpkt(&hmpack, 7);
		while ( dskstate == MSGSENT ); /* wait */
		if (dskstate == NAKRCVD) {bioserr("NAK from IPC"); return 0L;}
		while ( dskstate == ACKRCVD ); /* wait */
		check = (DSKIPC+PKTFMIPC+0x18)->byte;
		switch ( check )
			{
				case 3:	dphp->dpbp = &dpb0;
					break;

				case 7: dphp->dpbp = &dpb1;
					break;

				default: {bioserr("ERR 7"); return 0L;}
					break;
			}
	}
	return(dphp);
}


/************************************************************************/
/*      BIOS PROPER							*/
/************************************************************************/

 
 
 
biosinit()
{
	REG WORD i;

        portinit(PORT1);
        portinit(PORT2);
	for ( i = 0; i < NUMTB; ++i )
	{
		tbuf[i].valid = 0;
		tbuf[i].dirty = 0;
		if ( (i+1) < NUMTB ) tbuf[i].nextbuf = &tbuf[i+1];
		else		     tbuf[i].nextbuf = 0;
	}
	firstbuf = &tbuf[0];
	lastbuf  = &tbuf[NUMTB-1];
}
 


 
LONG bios(d0, d1, d2)
WORD	d0;
LONG	d1, d2;
{

#if DEBUG

	if ( d0 > 7) {
		printrace("\n\rBIOS( ");
		printnum((long)d0);
		printnum(d1);
		printnum(d2);
		printrace(")\n\r");
	}

#endif

	switch(d0)
	{
		case 0:	init();				/* INIT		*/
			break;

		case 1:	wboot();			/* WBOOT	*/
			break;

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

		case 9:	return(seldsk((char)d1, (char)d2)); /* SELDSK	*/
			break;

		case 10: settrk = (int)d1;		/* SETTRK	*/
			 break;

		case 11: setsec = (int)d1;		/* SETSEC	*/
			 break;

		case 12: setdma = d1;			/* SETDMA	*/
			 break;

		case 13: return(read());		/* READ		*/
			 break;

		case 14: return(write((char)d1));	/* WRITE	*/
			 break;

		case 15: return(portstat(PORT2));	/* LISTST	*/
			 break;

		case 16: return(sectran((int)d1, d2));	/* SECTRAN	*/
			 break;

		case 18: return(&memtab);		/* GMRTA	*/
			 break;

		case 19: return(iobyte);		/* GETIOB	*/
			 break;

		case 20: iobyte = (int)d1;		/* SETIOB	*/
			 break;

		case 21: return(flush());		/* FLUSH	*/
			 break;

		case 22: return(setxvect((int)d1,d2));	/* SETXVECT	*/
			 break;



	} /* end switch */


} /* end bios procedure */ 
 
 
 
/* End of C Bios */
