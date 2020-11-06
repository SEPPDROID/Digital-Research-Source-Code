/****************************************************************
*								*
*			SYSGEN		 			*
*								*
*	This is a stand-alone system generation program for	*
*	CP/M-68K, very similar to the traditional Sysgen on 	*
*	CP/M-80.  It has two modes of operation:  if invoked	*
*	without a command tail (i.e.,  A>SYSGEN ), it gets and	*
*	puts system tracks (with prompts, just as Sysgen on the	*
*	8080).  If the user types a command tail of a string	*
*	of filenames, SYSGEN concatenates those files and 	*
*	allows the user to put the result on the system tracks	*
*	In the file concatenation mode, SYSGEN will detect if	*
*	a file is a CP/M-68K executable file, and if so, will	*
*	"strip" it (remove the header, symbol table, and	*
*	relocation bits)					*
*								*
*	Configured for Alcyon C on the VAX			*
*								*
****************************************************************/

#include <stdio.h>		/* Standard I/O declarations */

#define SECLEN 128		/* CP/M sector length			*/
#define	BUFSIZE 16384		/* size of internal buffer		*/
				/* make it long enough to hold the system */
#define	sectoff	1		/* 0 for sectors 0 .. n-1		*/
				/* 1 for sectors 1 .. n			*/
#define skew	1		/* currently not used			*/


/*  Declare EXTERN functions */

EXTERN		printf();	/* formatted print 		*/
EXTERN	LONG	__BDOS();	/* direct call to BDOS		*/
EXTERN	WORD	open();		/* open file			*/
EXTERN	WORD	read();		/* read file			*/
EXTERN	LONG	lseek();	/* long seek			*/


/* Define the data structures used in sysgen */
struct dpb			/* disk parameter table		*/
{
	UWORD	spt;		/* sectors per track 		*/
	UBYTE	bsh;		/* block shift factor		*/
	UBYTE	blm;		/* block mask			*/
	UBYTE	exm;		/* extent mask			*/
	UBYTE	dpbdum;		/* dummy byte for fill		*/
	UWORD	dsm;		/* max disk size in blocks	*/
	UWORD	drm;		/* max directory entries	*/
	UWORD	dir_al;		/* initial allocation for dir	*/
	UWORD	cks;		/* number dir sectors to checksum */
	UWORD	trk_off;	/* track offset			*/
};

struct	dph			/* disk parameter header	*/
{
	UBYTE	*xlt;		/* pointer to sector translate table	*/
	UWORD	hiwater;	/* high water mark for this disk	*/
	UWORD	dum1;		/* dummy (unused)			*/
	UWORD	dum2;
	UBYTE	*dbufp;		/* pointer to 128 byte directory buffer	*/
	struct dpb *dpbp;	/* pointer to disk parameter block	*/
	UBYTE	*csv;		/* pointer to check vector		*/
	UBYTE	*alv;		/* pointer to allocation vector		*/
};

struct bpb
{		/* BIOS parameter block  */
    WORD	d0;
    LONG	d1;
    LONG	d2;
};


/*******************/
/*  error routine  */
/*******************/

error(num)
WORD	num;
{
    switch (num)
    {	
	case 0:	printf("\nDisk read error");
		break;

	case 1:	printf("\nDisk write error");
		break;

	case 2:	printf("\nFile read error");
		break;

	case 3:	printf("\nBuffer overflow");
		break;
    }
    __BDOS(0);
}


/******************/
/*  BIOS callers  */
/******************/

/* First define function numbers */
#define biosf	50
#define	seldskf	 9
#define	settrkf	10
#define	setsecf	11
#define	setdmaf	12
#define	readf	13
#define	writef	14

BYTE *seldsk(dsk)
WORD	dsk;
{
    struct bpb b;

    b.d0 = seldskf;
    b.d1 = (LONG)dsk;
    return( __BDOS(biosf, &b) );
}

settrk(trk)
WORD	trk;
{
    struct bpb b;

    b.d0 = settrkf;
    b.d1 = (LONG)trk;
    return( __BDOS(biosf, &b) );
}

setsec(sec)
WORD	sec;
{
    struct bpb b;

    b.d0 = setsecf;
    b.d1 = (LONG)sec;
    return( __BDOS(biosf, &b) );
}

setdma(p)
BYTE	*p;
{
    struct bpb b;

    b.d0 = setdmaf;
    b.d1 = (LONG)p;
    return( __BDOS(biosf, &b) );
}

WORD b_read()
{
    struct bpb b;

    b.d0 = readf;
    return( __BDOS(biosf, &b) );
}

WORD b_write()
{
    struct bpb b;

    b.d0 = writef;
    b.d1 = (LONG)0;
    return( __BDOS(biosf, &b) );
}


/*********************/
/*  prompt for disk  */
/*********************/

struct dpb *getdsk(msg)
BYTE	*msg;
{
    REG WORD	ch;
    REG struct dph *dphp;

    do
    {
	printf(msg);
	printf("  (A - P)\nor <return> to reboot  ");
	ch = __BDOS(1);
	if (ch == '\r') __BDOS(0);
	if ('A' <= ch  &&  ch <= 'P') ch -= 'A';
	else if ('a' <= ch  &&  ch <= 'p') ch -= 'a';
	else ch = -1;
    } while (ch < 0);
    if ( (dphp = seldsk(ch)) == NULL) error(0);
    return(dphp->dpbp);
}

/******************************/
/*  print completion message  */
/******************************/

prt_done()
{
    printf("\nFunction Complete");
}


/*******************************/
/*  Get system tracks routine  */
/*******************************/

WORD  getsys(p, parmp)

REG BYTE *p;		/* pointer to buffer		*/
REG struct dpb *parmp;	/* pointer to disk parm block	*/
{
    REG WORD	nb;	/* number bytes in buffer	*/
    REG WORD	trk, sect;

    trk = nb = 0;
    do
    {
	settrk(trk);
	sect = 0;
	do
	{
	    if (nb > BUFSIZE - SECLEN) error(3);	/* buffer overflow */
	    setsec(sect + sectoff);
	    setdma(p);
	    if (b_read()) error(0);
	    sect += 1;
	    p += SECLEN;
	    nb += SECLEN;
	} while (sect < parmp->spt);
	trk += 1;
    } while (trk < parmp->trk_off);
    return(nb);
}


/*******************************/
/*  Put system tracks routine  */
/*******************************/

putsys(p, nbytes, parmp)

REG BYTE *p;		/* pointer to buffer		*/
REG WORD nbytes;	/* number bytes to put		*/
REG struct dpb *parmp;	/* pointer to disk parm block	*/
{
    REG WORD	trk, sect;

    trk = 0;
    do
    {
	settrk(trk);
	sect = 0;
	do
	{
	    setsec(sect + sectoff);
	    setdma(p);
	    if (b_write(0)) error(1);
	    sect += 1;
	    p += SECLEN;
	    nbytes -= SECLEN;
	    if (nbytes <= 0) return;
	} while (sect < parmp->spt);
	trk += 1;
    } while (trk < parmp->trk_off);
}


/******************************/
/*  Get a File to the Buffer  */
/******************************/

getfile(fname, bp, np)

BYTE	*fname;		/* name of file		*/
BYTE	*bp;		/* buffer		*/
WORD	*np;		/* number of bytes in buffer (passed by reference) */
{
    REG WORD	fd;	/* file descriptor	*/
    REG WORD	i;
    REG WORD	rtn;
    struct
    {
	WORD magic;		/* magic number */
	LONG size[5];		/* sizes of code, data, bss, sym tbl, stack */
	BYTE *tstart;		/* start address of text segment	*/
	WORD rlbflg;		/* relocation bits suppressed flag	*/
    } hdr;

    fd = open(fname, 0);
    if (fd == -1) error(2);
    if ( read(fd, &hdr, sizeof hdr) != sizeof hdr) error(2);
    if (hdr.magic == 0x601b) lseek(fd, (sizeof hdr) + 8, 0);
    if ( (hdr.magic == 0x601a) || (hdr.magic == 0x601b) )
    {
	/* CP/M-68K executable file */
	i = hdr.size[0] + hdr.size[1];
	if ( *np + i > BUFSIZE) error(3);	/* buffer overflow */
	if (read(fd, bp + *np, i) != i) error(2);
	*np += i + (WORD)hdr.size[3];
    }
    else
    {
	/* miscellaneous binary file */
	lseek(fd, 0, 0);	/* seek to beginning of file */
	i = BUFSIZE - *np;	/* max nmbr bytes we can fit in buffer */
	if ( i <= 0 ) error(3);	/* buffer overflow */
	rtn = read( fd, bp + *np, i);
	if (rtn < 0) error(2);	/* read error */
	*np += rtn;
    }
}


/***************************
*   sysgen main routine    *
***************************/

main(argc, argv)

REG WORD  argc;		/* number of command line arguments */
REG BYTE  **argv;	/* pointer to array of arguments    */
{
    BYTE	buff[BUFSIZE];	/* buffer for system		 */
    WORD	nbytes;		/* number of bytes in buff	 */
    REG struct dpb *parmp;	/* pointer to dpb for selected disk */

    nbytes = 0;
    if (--argc)
	while (argc--)
	    getfile( *++argv, buff, &nbytes);
    else
    { 
	parmp  = getdsk("\nSource Drive?");
	nbytes = getsys(buff, parmp);
	prt_done;
    }
    while ( TRUE )
    {
	parmp = getdsk("\nDestination Drive?");
	putsys( buff, nbytes, parmp);
	prt_done;
    }
}
