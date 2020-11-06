#include "cout.h"
#include "lo68.h"
#define	LIBMAGIC	0177545
int loctr=0;
int pflg=0;
int syno=0;
int symflg=0;
extern int fout;

char *reloc[] {
	"DABS",
	"DRELOC",
	"TRELOC",
	"BRELOC",
	"EXTVAR",
	"LUPPER",
	"NOTUSED",
	"INSABS" };

long lseek();

main(argc,argv)
char **argv;
{

long l,l1;
	register i,j;

	if(argc<2) {
		printf("Usage: prtobj [-] objectfile\n");
		exit();
	}
	fout = dup(1);
	i = 1;
	if(*argv[1] == '-') {
		i++;
		symflg++;
	}
	pass2 = 1;
	openfile(argv[i]);
	if(couthd.ch_rlbflg == 0) {	/*relocation bits there*/
		l = couthd.ch_tsize+couthd.ch_dsize+couthd.ch_ssize+HDSIZE;	/*ptr to relocation bits*/
		longseek(l,&rbuf,3);
	}
	printf("file name: %s\n",argv[i]);
	l = couthd.ch_tsize;
	printf("text size   = %9ld = %08lx hex\n",l,l);
	l = couthd.ch_dsize;
	printf("data size   = %9ld = %08lx hex\n",l,l);
	l = couthd.ch_bsize;
	printf("bss size    = %9ld = %08lx hex\n",l,l);
	l = couthd.ch_ssize;
	printf("symbol size = %9ld = %08lx hex\n",l,l);
	l = couthd.ch_stksize;
	printf("stack size  = %9ld = %08lx hex\n",l,l);
	loctr = couthd.ch_entry;
	printf("entry point = %9ld = %08lx hex\n",loctr,loctr);
	if(symflg) {
		l = couthd.ch_tsize+couthd.ch_dsize+HDSIZE;
		longseek(l,&ibuf,3);
		goto dosyms;
	}
	l1 = couthd.ch_tsize;
	printf("TEXT:\n");
mloop:
	for(l=0; l<l1; l=+ 2) {
		i = getw(&ibuf);
		if(couthd.ch_rlbflg==0)
			j = getw(&rbuf);
		else
			j = DABS;
		printf("%08x   %04x   %s",loctr,((int)i)&0xffff,reloc[j&7]);
		if((j&7) == EXTVAR)
			printf(" ext# %5d\n",(j>>3)&017777);
		else
			putchar('\n');
		loctr =+ 2;
	}
	if(pflg==0) {
		printf("\nDATA:\n");
		l1 = couthd.ch_dsize;
		pflg++;
		goto mloop;
	}
dosyms:
	printf("\nSYMBOLS:\n");
	l1 = couthd.ch_ssize;
	syno = 0;
	while(l1) {
		printf("%4d  ",syno);
		syno++;
		for(i=0; i<8; i++)
			putchar((int)getc(&ibuf));	/*symbol name*/
		putchar((int)'\t');
		j = getw(&ibuf);		/*flags*/
		l.hiword = getw(&ibuf);
		l.loword = getw(&ibuf);
		printf("%04x%04x",((int)l.hiword)&0xffff,
				  ((int)l.loword)&0xffff);
		prtflags(j);
		l1 =- OSTSIZE;
	}
	myflush();
}

prtflags(af)
{

	register f;

	f = af;
	if(f&SYEQ)
		printf(" equ");
	if(f&SYGL)
		printf(" global");
	if(f&SYER)
		printf(" equreg");
	if(f&SYXR)
		printf(" external");
	if(f&SYDA)
		printf(" data");
	else if(f&SYTX)
		printf(" text");
	else if(f&SYBS)
		printf(" bss");
	else
		printf(" abs");
	putchar((int)'\n');
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
	if(couthd.ch_magic != LIBMAGIC) {
		lseek(ibuf.fildes,0L,0);
		readhdr();				/*read file header*/
	}
	if(pass2) {		/*need file descrptr for reloc bits*/
		rbuf.fildes = open(p,0,1);
		rbuf.nunused = 0;
	}
}
doseek(afd,aoff,apnam)
{

	if(lseek(afd,(long)aoff,apnam) < 0) {
		printf("seek error on file %s\n",ifilname);
		exit();
	}
}
readhdr()
{

	register int i;
	register short *p;

	p = &couthd;
	for(i=0; i<HDSIZE/2; i++)
		*p++ = getw(&ibuf);
	if(couthd.ch_magic != MAGIC) {
		printf("file format error: %s\n",ifilname);
		exit();
	}
}
longseek(al,bp,pn)
long al;
struct buf *bp;
{

	long l;
	register b,o;

	l = al>>9;
	b = l.loword;	/*block #*/
	o = al.loword&0777;	/*offset in block*/
	doseek(bp->fildes,b,pn);	/*seek to block*/
	doseek(bp->fildes,o,SEEKREL);	/*do offset*/
	bp->nunused = 0;	/*input buff empty*/
}
