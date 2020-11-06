/*=======================================================================*/
/*/---------------------------------------------------------------------\*/
/*|									|*/
/*|     CP/M-68K(tm) BIOS for the EXORMACS 				|*/
/*|									|*/
/*|     Copyright 1982, Digital Research.				|*/
/*|									|*/
/*\---------------------------------------------------------------------/*/
/*=======================================================================*/



char copyright[] = "Copyright 1982, Digital Research";

struct memb { char byte; };
struct memw { int  word; };
struct meml { long lword;};


/************************************************************************/ 
/*      I/O Device Definitions						*/
/************************************************************************/




/************************************************************************/ 
/*      Define the two ACIA ports on the Debug board			*/
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
#define ACKFMIPC	0x103
#define	PKTTOIPC	0x105
#define	MSGFMIPC	0x181
#define ACKTOIPC	0x183
#define PKTFMIPC	0x185

#define	STX	0x02
#define ETX	0x03
#define	ACK	0x06
#define NAK	0x15



/************************************************************************/
/* BIOS  Table Definitions						*/
/************************************************************************/

struct dpb
{
	int	spt;
	char	bsh;
	char	blm;
	char	exm;
	char	dpbjunk;
	int	dsm;
	int	drm;
	char	al0;
	char	al1;
	int	cks;
	int	off;
};



struct dph
{
	char	*xltp;
	int	 dphscr[3];
	char	*dirbufp;
struct	dpb	*dpbp;
	char	*csvp;
	char	*alvp;
};



/************************************************************************/
/*	Directory Buffer for use by the BDOS				*/
/************************************************************************/

char dirbuf[128];


/************************************************************************/
/*	CSV's								*/
/************************************************************************/

char	csv0[16];
char	csv1[16];
char	csv2[16];


/************************************************************************/
/*	ALV's								*/
/************************************************************************/

char	alv0[32];	/* (dsm0 / 8) + 1	*/
char	alv1[32];	/* (dsm1 / 8) + 1	*/
char	alv2[2002];	/* (dsm2 / 8) + 1	*/

 
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

struct dpb dpb0 = { 26, 1799, 0,   242, 63, -16384, 16, 2 };
struct dpb dpb1 = { 26, 1039, 0,   242, 63, -16384, 16, 2 };
struct dpb dpb2 = { 26, 1799, 0, 16008, 63, -16384, 16, 2 };

/*************** End of kludge *****************/



/************************************************************************/
/* Sector Translate Table						*/ 
/************************************************************************/

char	xlt[26] = {  1,  7, 13, 19, 25,  5, 11, 17, 23,  3,  9, 15, 21,
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

struct mrt {	int  count;
		long tpalow;
		long tpalen;
	   }
	memtab = { 1, 0x400L, 0xFC00 };


/************************************************************************/
/*	IOBYTE								*/
/************************************************************************/

int iobyte;


/************************************************************************/
/*	Currently Selected Disk Stuff					*/
/************************************************************************/

int settrk, setsec, setdsk;
char *setdma;


char	trkbuf[26 * 128];
int	tbvalid = 0;
int	tbdirty = 0;
int	tbtrk;
int	tbdsk;


/************************************************************************/
/*	Disk I/O Packets for the UDC and other Disk I/O Variables	*/
/************************************************************************/

struct hmpkst {
		char	a1;
		char	a2;
		char	a3;
		char	dskno;
		char	com1;
		char	com2;
		char	a6;
		char	a7;
	      }
	hmpack = { 512, 1792, 0, 768 }; /* kludge init by words */


struct rwpkst {
		char stxchr;
		char pktid;
		char pktsize;
		char dskno;
		char chcmd;
		char devcmd;
		int  numblks;
		int  blksize;
		long iobf;
		int  cksum;
		long lsect;
		char etxchr;
		char rwpad;
	      };

struct rwpkst rwpack = { 512, 5376, 4097, 13, 256, 0, 0, 0, 0, 0, 768 };


char cnvdsk[4] = { 4, 5, 0, 1 };  /* convert from CP/M dsk # to Exormacs */

char flag2;	/* used by the disk interrupt handler	*/
char flag3;	/* ditto				*/

#define MAXDSK 3

/************************************************************************/
/*      Generic Serial Port I/O Procedures				*/
/************************************************************************/
 
portinit(port)
char *port;
{
        *(port + PORTCTRL) = PORTRSET;
        *(port + PORTCTRL) = PORTINIT;
}
 
 
 
 
 
portstat(port)
char *port;
{
        return ( *(port + PORTSTAT) & PORTRDRF) == PORTRDRF;
}
 
 
 
 
char portin(port)
char *port;
{
        while (portstat(port) == 0) ;
        return *(port + PORTRDR);
}
 
 
 
 
portout(port, ch)
char *port;
char ch;
{
        while ( (*(port + PORTSTAT) & PORTTDRE) != PORTTDRE) ;
        *(port + PORTTDR) = ch;
}
 
 
/************************************************************************/ 
/*      Error procedure for BIOS					*/
/************************************************************************/

bioserr(errmsg)
char *errmsg;
{
        printstr("\n\rBIOS ERROR -- ");
        printstr(errmsg);
        printstr(".\n\r");
        while(1);
}
 
printstr(s)     /* used by bioserr */
register char *s;
{ 
        while (*s) {portout(PORT1,*s); s += 1; };
}

printnum(n)
register long n;
{
	register int  i;
	register char c, j;

	i = 32;
	do {
		i -= 4;
		j = (n>>i) & 0x0F;
		if ( j < 10 )	c = '0' + j;
		else		c = 'A' + j - 10;
		portout(PORT2, c);
	} while (i);
	portout(PORT2, ' ');
}

printrace(s)
register char *s;
{
	while (*s) {portout(PORT2, *s); s += 1;}
}


/************************************************************************/
/*	Disk I/O Procedures						*/
/************************************************************************/


extern dskia();

dskic()
{
	/* Disk Interrupt Handler -- C Language Portion */

	if ( ! (DSKIPC+MSGFMIPC)->byte )
	{
		if ( (DSKIPC+ACKTOIPC)->byte != ACK )
		{
			(DSKIPC+ACKTOIPC)->byte = 0;
		}
		else
		{
			(DSKIPC+ACKTOIPC)->byte = 0;
			flag2 = 0;
		}
	}
	else
	{
		if ( (DSKIPC+PKTFMIPC)->byte == 2 )
		{
			(DSKIPC+ACKFMIPC)->byte = ACK;
			(DSKIPC+MSGFMIPC)->byte = 0;
			(DSKIPC+INTTOIPC)->byte = 0;
			if ( ((DSKIPC+PKTFMIPC+8 )->byte == 0xFF) &&
			     ((DSKIPC+PKTFMIPC+10)->byte == 0x80)   )
			{
				tbtrk = 0xDFFFFFL;
			}
			flag3 = 0;
		}
	}

} /* end of dskic */


sendpkt(pktadr, pktsize)
register char  *pktadr;
register int	pktsize;
{
	 int   i;
register char *iopackp;

	flag2 = 0xFF;
	flag3 = 0xFF;
	DSKINTV->lword = &dskia;
	i = pktsize;
	iopackp = (DSKIPC + PKTTOIPC);
	do { *iopackp = *pktadr++; iopackp += 2; i -= 1;} while (i); 
	(DSKIPC+MSGTOIPC)->byte  = 0x80;
	(DSKIPC+ACKTOIPC)->byte = 0;
	(DSKIPC+INTTOIPC)->byte = 0;
	while (flag2 == 0xFF);
	while (flag3 == 0xFF);
}


#define wrongtk ((! tbvalid) || (tbtrk != settrk) || (tbdsk != setdsk))
#define gettrk	if (wrongtk) filltb()


flush()
{

	if ( tbvalid ) /* write out the contents of the track buffer */
	{
		rwpack.dskno = cnvdsk[tbdsk];
		rwpack.iobf  = &trkbuf;
		rwpack.lsect = tbtrk * 13;
		rwpack.chcmd = 0x20;
		(DSKIPC + ACKFMIPC)->byte = ACK;
		if ( (DSKIPC + MSGTOIPC)->byte ) bioserr("ERR2");
		sendpkt(&rwpack, 21);
	}

	tbdirty = 0;
	
	return(0);
}





filltb()
{

	if ( tbvalid && tbdirty ) flush();

	rwpack.dskno = cnvdsk[setdsk];
	rwpack.iobf  = &trkbuf;
	rwpack.lsect = settrk * 13;
	rwpack.chcmd = 0x10;
	(DSKIPC + ACKFMIPC)->byte = ACK;
	if ( (DSKIPC + MSGTOIPC)->byte ) bioserr("ERR2");
	sendpkt(&rwpack, 21);
		
	tbvalid = 1;
	tbdirty = 0;
	tbtrk	= settrk;
	tbdsk	= setdsk;

}


read()
{
	register char	*p;
	register char	*q;
	register int	 i;

	gettrk;
	p = &trkbuf[128 * (setsec-1)];
	q = setdma;
	i = 128;
	do {*q++ = *p++; i -= 1;} while (i);
	return(0);
}


write(mode)
char mode;
{
	register char	*p;
	register char	*q;
	register int	 i;

	gettrk;
	p = &trkbuf[128 * (setsec-1)];
	q = setdma;
	i = 128;
	do {*p++ = *q++; i -= 1;} while (i);
	tbdirty = 1;
	if ( mode == 1 ) flush();
	return(0);
}


	
char sectran(s, xp)
int	 s;
char	*xp;
{
	return xp[s];
}


setxvect()
{
	/* dummy */
}


long seldsk(dsk, logged)
register char	dsk;
	 char	logged;
{
	register struct dph *dphp;
	register char	check;

	if (dsk > MAXDSK) return(0L);
	setdsk = dsk;
	dphp = &dphtab[dsk];
	if ( ! logged )
	{
		hmpack.dskno = cnvdsk[setdsk];
		hmpack.com1  = 0x30;
		hmpack.com2  = 0x02;
		(DSKIPC+ACKFMIPC)->byte = 6;
		if ( (DSKIPC+MSGTOIPC)->byte ) bioserr("ERR 6");
		else sendpkt(&hmpack, 7);
		check = (DSKIPC+PKTFMIPC+0x18)->byte;
		switch ( check )
			{
				case 3:	dphp->dpbp = &dpb0;
					break;

				case 7: dphp->dpbp = &dpb1;
					break;
				default: bioserr("ERR 7");
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
        portinit(PORT1);
        portinit(PORT2);
}
 


 
long bios(d0, d1, d2)
int	d0;
long	d1, d2;
{
	if ( d0 > 7) {
		printrace("\n\rBIOS( ");
		printnum((long)d0);
		printnum(d1);
		printnum(d2);
		printrace(")\n\r");
	}

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

		case 13: return(read());			/* READ		*/
			 break;

		case 14: return(write((char)d1));	/* WRITE	*/
			 break;

		case 15: return(portstat(PORT2));	/* LISTST	*/
			 break;

		case 16: return(sectran((int)d1, d2));	/* SECTRAN	*/
			 break;

		case 17: return(&memtab);		/* GMRTA	*/
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
