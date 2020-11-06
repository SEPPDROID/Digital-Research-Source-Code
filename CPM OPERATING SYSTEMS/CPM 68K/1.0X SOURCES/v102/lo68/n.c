#include "cout.h"
#include "sendc68.h"

#define MAGIC1 MAGIC+1
int hdsize=0;

int loctr=0;
int pflg=0;
int syno=0;
int symflg;
extern int fout;
long lseek();
main(argc,argv)
char **argv;
{

long l,l1;
	register i,j,k;

	if(argc<2) {
		printf("Usage: nm68 objectfile\n");
		exit();
	}
	hdsize = HDSIZE;
	fout = dup(1);
	i = 1;
	openfile(argv[i]);
	l = couthd.ch_tsize+couthd.ch_dsize+hdsize;
	lseek(ibuf.fildes,l,0);
	ibuf.nunused = 0;
	l1 = couthd.ch_ssize;
	syno = 0;
	while(l1) {
		syno++;
		for(i=0; i<8; i++)			/* Print Symbol name*/
		{
			k = getc(&ibuf) & 0377;
			if (k < ' ') k = ' ';		/* Filter ctrl chars*/
			putchar(k);
		}
		putchar((char)'\t');
		j = getw(&ibuf);		/*flags*/
		l.hiword = getw(&ibuf);
		l.loword = getw(&ibuf);
		printf("%8lx",l);
		prtflags(j);
		l1 -= OSTSIZE;
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
		printf(" reg");
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
	putchar((char)'\n');
}
openfile(ap)
char *ap;
{

	register char *p;

	p = ap;
	if((ibuf.fildes=fopen(p,&ibuf,1)) < 0) {
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
			hdsize =+ 8;
		}
		else {
			printf("file format error: %s\n",ifilname);
			exit();
		}
	}
}
