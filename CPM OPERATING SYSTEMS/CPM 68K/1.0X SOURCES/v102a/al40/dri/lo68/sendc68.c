#include <stdio.h>
#include <klib.h>
#include "cout.h"
#include "sendc68.h"

/* this program reads a c.out format binary file and converts*/
/*  it to the absolute ASCII load format acceptable by MACSBUG*/
/*  and then sends it down the standard output file*/

#define MAGIC1 MAGIC+1
long lseek();
long datastart=0;
long bssstart=0;

long loctr=0;
extern int fout;
int pflg=0;
#define RBLEN 32	/*number of bytes per S1 record*/
int bcnt=RBLEN;
char cksum=0;
int syno=0;
char *ofn=0;
int ofd=0;
int noclear=0;


main(argc,argv)
char **argv;
{

long l,l1;
	register i,j;

	if(argc < 2 || argc > 3) {
		usage();
	i = 1;
	if(*argv[1] == '-') {
		noclear++;
		i++;
	}
	openfile(argv[i++]);
	if(argc>i) {
			usage();
		}
	}
	else {
		ofd = dup(1);	/*standard output file*/
	}
	fout = ofd;		/*use buffered output*/
	l1 = couthd.ch_tsize;
	loctr = couthd.ch_entry;
mloop:
	for(l=0; l<l1; l=+ 2) {
		i = getw(&ibuf);
		outword(i);
		loctr =+ 2;
	}
	if(couthd.ch_magic==MAGIC1) {
		while(bcnt != RBLEN)	/*fill out the last S1 buffer*/
			outword(0);
		loctr = datastart;
	}
	l1 = couthd.ch_dsize;
	for(l=0; l<l1; l=+ 2) {
		i = getw(&ibuf);
		outword(i);
		loctr =+ 2;
	}
	if(noclear==0) {
		if(couthd.ch_magic==MAGIC1) {
			while(bcnt != RBLEN)	/*fill out the last S1 buffer*/
				outword(0);
			loctr = bssstart;
		}
		l1 = couthd.ch_bsize;		/*size of bss*/
		while(l1 > 0) {
			outword(0);		/*clear the bss*/
			l1 =- 2;
			loctr =+ 2;
		}
	}
	while(bcnt != RBLEN)	/*fill out the last S1 buffer*/
		outword(0);
	printf("S9030000FC\n");

}

openfile(ap)
char *ap;
{

	register char *p;

	p = ap;
	if((ibuf.fildes=open(p,0,1)) < 0) {
		printf("unable to open %s\n",p);
		exit();
	}
	ifilname = p;	/*point to current file name for error msgs*/
	if(read(ibuf.fildes,&couthd,2) != 2) {
		printf("read error on file: %s\n",ifilname);
		exit();
	}
	ibuf.nunused = 0;
	lseek(ibuf.fildes,0L,0);
	readhdr();				/*read file header*/
}
readhdr()
{

	register int i;
	register short *p;

	p = &couthd;
	for(i=0; i<HDSIZE/2; i++)
		*p++ = getw(&ibuf);
	if(couthd.ch_magic != MAGIC) {
		if(couthd.ch_magic==MAGIC1) {
#ifdef VAX
			datastart.loword = getw(&ibuf);
			datastart.hiword = getw(&ibuf);
			bssstart.loword = getw(&ibuf);
			bssstart.hiword = getw(&ibuf);
#endif
#ifdef PDP11
			datastart.loword = getw(&ibuf);
			datastart.hiword = getw(&ibuf);
			bssstart.loword = getw(&ibuf);
			bssstart.hiword = getw(&ibuf);
#endif
#ifdef MC68000
			datastart.hiword = getw(&ibuf);
			datastart.loword = getw(&ibuf);
			bssstart.hiword = getw(&ibuf);
			bssstart.loword = getw(&ibuf);
#endif
		}
		else {
			printf("file format error: %s\n",ifilname);
			exit();
		}
	}
}
longseek(al,bp,pn)
long al;
struct buf *bp;
{

	if(lseek(bp->fildes,al,0) < 0) {
		printf("seek error on file %s\n",ifilname);
		exit();
	}
	bp->nunused = 0;	/*input buff empty*/
}

outword(i)
	register i;
{


	if(bcnt==RBLEN) {	/*beginning of record*/
		cksum = 0;
		if(loctr.hiword) {
			printf("S2");
			hexby(RBLEN+4);
			hexby(loctr.hiword);
		}
		else {
			printf("S1");
			hexby(RBLEN+3);	/*byte count*/
		}
		hexwd(loctr.loword);
	}
	hexwd(i);
	bcnt =- 2;
	if(bcnt==0) {
		hexby(-cksum-1);
		putchar('\n');
		bcnt = RBLEN;
	}
}

hexwd(i)
{

	hexby((i>>8)&0xff);
	hexby(i&0xff);
}

hexby(c)
{

	cksum =+ c;
	outhex((c>>4)&017);
	outhex(c&017);
}

outhex(x)
{

	if(x>=0 && x<=9)
		putchar(x+'0');
	else if(x>=10 && x<=15)
		putchar(x-10+'A');
	else
		exit();
}
usage()
{
		printf("Usage: sendc68 [-] commandfile [>outputfile]\n");
		exit();
}
