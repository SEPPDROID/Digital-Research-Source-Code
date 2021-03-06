
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
*/

char *version = "@(#) sendc68 - Sep 1, 1983";

#ifdef VAX11
#	include <c68/cout.h>
#	include <c68/sendc68.h>
#endif
#ifdef MC68000
#	include <cout.h>
#	include <sendc68.h>
#endif

/**
 * this program reads a c.out format binary file and converts
 * it to the absolute ASCII load format acceptable by MACSBUG
 * and then sends it down the standard output file
**/

#define RBLEN	32			/* number of bytes per S1 record */
#define DELAY	012000
#define SDEFAULT 0x400
#define EDEFAULT 0x1000
#define USAGE "[-r] [-d delay] [-s start] [-e end] [-] objectfile [outputfile]"

struct hdr2 couthd;
struct buf *file;

char cksum, *ofn;
long loctr, delay;
int fout, pflg, ofd, noclear, debug, syno;
int bcnt = RBLEN;
long nlstart = SDEFAULT;
long nlend = EDEFAULT;
int regulus = 0;

main(argc,argv)
int argc;
char *argv[];
{
	register long l, l1;
	register int i;
	short word;
	char *calledby;

	calledby = *argv++; 
	if( argc-- < 2 ) {
usage:
		printf("Usage: %s: %s\n",calledby,USAGE);
		exit();
	}
	file = &ibuf;
	while( **argv == '-' ) {
		switch(*++*argv) {
			case '\0':	/* '-' only */
				noclear = 1;
				break;
			case 'r':	/* don't put out cr-lf only lf */
				regulus = 1;
				break;
			case 'd':
				argv++; argc--;
				if ((delay = atol(*argv)) <= 0)
					delay = DELAY;
				break;
			case 's':
				argv++; argc--;
				if ((nlstart = atol(*argv)) < 0)
					nlstart = SDEFAULT;
				break;
			case 'e':
				argv++; argc--;
				if ((nlend = atol(*argv)) < 0)
					nlend = EDEFAULT;
				break;
			case 'D':
				debug = 1;
				break;
			default:
				goto usage;
		}
		argc--; argv++;
	}

	if (!argc)
		goto usage;
	openfile(calledby,*argv++);
	if( --argc ) {
		ofn = *argv;	/* tty device name */
		if( (ofd = creat(ofn,0666)) < 0 ) {
			printf("%s: unable to create [%s]\n",calledby,ofn);
			exit();
		}
	}
	else
		ofd = dup(1);	/* standard output file */
	
	fout = ofd;		/* use buffered output */
	l1 = couthd.ch_tsize;
	loctr = couthd.ch_entry;

/* Main Loop */
	for( l = 0; l < l1; l += 2) {
		lgetw(&word, file);
		outword(word);
		loctr += 2;
	}
	if( couthd.ch_magic == EX_ABMAGIC ) {
		while(bcnt != RBLEN)	/* fill out the last S1 buffer */
			outword(0);
		loctr = couthd.ch_dstart;
	}
	l1 = couthd.ch_dsize;
	for( l = 0; l < l1; l += 2) {
		lgetw(&word, file);
		outword(word);
		loctr += 2;
	}
	if( noclear == 0 ) {
		if( couthd.ch_magic == EX_ABMAGIC ) {
			while( bcnt != RBLEN )	/* fill out the last S1 buffer */
				outword(0);
			loctr = couthd.ch_bstart;
		}
		l1 = couthd.ch_bsize;		/* size of bss */
		while( l1 > 0 ) {
			outword(0);		/* clear the bss */
			l1 -= 2;
			loctr += 2;
		}
	}
	while( bcnt != RBLEN )	/* fill out the last S1 buffer */
		outword(0);
	printf("S9030000FC\n");
	if( !regulus )
		putchar(015);	/* carriage return */
	flush();
}

openfile(calledby,ap)
char *calledby,*ap;
{
	register char *p;

	if (debug)
		printf("%s input file : [%s]\n",calledby,ap);
	p = ap;
	if( (ibuf.fildes = open(p,0)) < 0 ) {
		printf("%s: unable to open %s\n",calledby,p);
		exit(-1);
	}
	ifilname = p;	/* point to current file name for error msgs */
	readhdr();				/* read file header */
}

#define BADMAGIC(magic) (magic<EX_MAGIC || magic>EX_4KSTXT)
readhdr()
{
	if (getchd(file, &couthd) == -1) {
		printf("read error on: %s\n",ifilname);
		exit(-1);
	}
	if (debug) {
		printf("magic = %x\n",couthd.ch_magic);
		printf("tsize = %ld\n",couthd.ch_tsize);
		printf("dsize = %ld\n",couthd.ch_dsize);
		printf("bsize = %ld\n",couthd.ch_bsize);
		printf("ssize = %ld\n",couthd.ch_ssize);
		printf("stksize = %ld\n",couthd.ch_stksize);
		printf("entry = %ld\n",couthd.ch_entry);
		if (couthd.ch_magic == EX_ABMAGIC) {
			printf("dstart = %ld\n",couthd.ch_dstart);
			printf("bstart = %ld\n",couthd.ch_bstart);
		}
	}
	if (BADMAGIC(couthd.ch_magic)) {
		printf("file format error: %s %x\n",ifilname,couthd.ch_magic);
		exit(-1);
	}
}

outword(i)
register int i;
{
	if( loctr >= nlstart && loctr < nlend )	/* MACSBUG RAM--dont load */
		return;

	if( bcnt == RBLEN ) {	/* beginning of record */
		cksum = 0;
		if (loctr >= 0x10000L) {
			printf("S2");
			hexby((int)(RBLEN+4));
			hexby((int)(loctr>>16));
		}
		else {
			printf("S1");
			hexby((int)(RBLEN+3));	/* byte count */
		}
		hexwd((int)loctr);
	}
	hexwd((int)i);
	bcnt -= 2;
	if( bcnt == 0 ) {
		hexby((int)(-cksum-1));
		putchar('\n');
		if( !regulus )
			putchar(015);	/* carriage return */
		bcnt = RBLEN;
		flush();
		dodelay();			/* give 68000 time to load */
	}
}

hexwd(i)
register int i;
{
	hexby(i>>8);
	hexby(i);
}

hexby(c)
register int c;
{
	c &= 0xff;
	cksum += c;
	outhex((c>>4)&017);
	outhex(c&017);
}

outhex(x)
register int x;
{
	if( x >= 0 && x <= 9 )
		putchar(x+'0');
	else if( x >= 10 && x <= 15 )
		putchar(x-10+'A');
	else
		exit(-1);
}

dodelay()
{
	register long i, j;

	for( i = 0; i != delay; i++ )
		j = 0;
}
