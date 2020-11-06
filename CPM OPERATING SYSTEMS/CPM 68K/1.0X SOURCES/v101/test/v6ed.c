/*
 * Editor
 * Copyright 1974, Bell Telephone Laboratories, Incorporated
 * Modified by Joseph Sokol, National Bureau of Standards
 */
/* new modifications:	j - join, b - break, z - print page, prompt for
			command lines, n - print line with line number,
			write out ed-hup on hangups, retains marks on
			lines that have been subistuted for, message
			generated if file has not been written out
			when the q or e commands are given, u - acts
			like w command, except appends lines to 
			specified file.
*/
#define	LONG	long
#include "stdio.std"
#include <setjmp.h>
#define getchar xgetchar
#define putchar xputchar
#define puts xputs
char *getline(), *getblock(), *place();
long lseek();
#define	SIGHUP	1
#define	SIGINTR	2
#define	SIGQUIT	3
#define BRKPIPE 13
#define	FNSIZE	64
#define	LBSIZE	512
#define	ESIZE	128
#define	GBSIZE	256
#define	NBRA	5
#define	EOF	-1
#define	CBRA	1
#define	CCHR	2
#define	CDOT	4
#define	CCL	6
#define NCCL	8
#define	CDOL	10
#define	CEOF	11
#define	CKET	12
#define	STAR	01
#define	READ	0
#define	WRITE	1
#define	NP	014
jmp_buf	blivot={0};		/* For CP/M	*/
int	peekc;
int	brktst;
int	lastc;
char	savedfile[FNSIZE];
char	file[FNSIZE];
char	linebuf[LBSIZE];
char	rhsbuf[LBSIZE/2];
char	expbuf[ESIZE+4];
int	circfl;
int	*zero;
int	*dot;
int	*dol;
int	*endcore;
char	*address();
int	*sbrk();
int	*fendcore;
int	*addr1;
int	*addr2;
char	genbuf[LBSIZE];
int	count[2];
char	*nextip;
char	*linebp;
int	ninbuf;
int	io;
int	pflag;
int	onquit;
int	zflag   =  0;
int	zlength = 23;
int	vflag   =  1;
int	fchang  =  0;
int	initrd  =  1;
int	listf   =  0;
int	onhup();
int	col;
char	*globp;
int	tfile   = -1;
int	tline;
char	*tfname;
char	*loc1;
char	*loc2;
char	*locs;
char	ibuff[512];
int	iblock  = -1;
char	obuff[512];
int	oblock  = -1;
int	ichanged;
int	nleft;
char	TMPERR[]= {"TMP"};
int	names[26];
char	*braslist[NBRA];
char	*braelist[NBRA];

main(argc, argv)
char **argv; {

	register char *p1, *p2;
/*	extern int onintr();	*/
	int  done();

/*	onquit = signal(SIGQUIT, 1);
	signal(SIGHUP,onhup);
	signal(BRKPIPE,done);
*/
	argv++;
	if(argc > 1 && **argv == '-') {

		/* allow debugging quits? */

/*		if((*argv)[1] == 'q')  signal(SIGQUIT, 0);
		else  vflag = 0;
*/
		argv++;
		argc--;
	}

	if(argc > 1) {
		p1 = *argv;
		p2 = savedfile;
		while(*p2++ = *p1++);
		globp = "r";
	}

	fendcore = sbrk(0);
	init();
/*	if((signal(SIGINTR, 1) & 01) == 0)  signal(SIGINTR, onintr);
*/
	do  {
		if(fchang && vflag)  dontgo();
		setjmp(blivot);
		commands();
	} while(fchang && vflag);
	done();
}

done() {
	close(tfile);
	unlink(tfname);
	exit(0);
}

commands() {

	int getfile(), gettty();
	register char  *a1, c;

	for(;;) {

	if(pflag) {
		pflag = 0;
		addr1 = addr2 = dot;
		setdot();
		printlin();
		continue;
	}

	if(vflag && !globp)  write(1,"* ",2);
	addr1 = 0;
	addr2 = 0;

	do {
		addr1 = addr2;
		if((a1 = address()) == 0) {
			c = getchar();
			break;
		}
		addr2 = a1;
		if((c = getchar()) == ';') {
			c = ',';
			dot = a1;
		}
	} while(c == ',');

	if(addr1 == 0)  addr1 = addr2;

	switch(c) {

	case 'a':
		setdot();
		newline();
		append(gettty, addr2);
		fchang = 1;
		continue;

	case 'b':
		setprt();
		fchang = 1;
		continue;

	case 'c':
		delete(0);
		append(gettty, addr1-1);
		fchang = 1;
		continue;

	case 'd':
		delete(0);
		fchang = 1;
		continue;

	case 'f':
		setnoaddr();
		if((c = getchar()) != '\n') {
			peekc = c;
			savedfile[0] = 0;
			filename();
		}
		puts(savedfile);
		continue;

	case 'g':
		global(1);
		continue;

	case 'i':
		setdot();
		nonzero();
		newline();
		append(gettty, addr2-1);
		fchang = 1;
		continue;

	case 'j':
		setdot();
		nonzero();
		join();
		fchang = 1;
		continue;

	case 'k':
		if((c = getchar()) < 'a' || c > 'z')  errfunc("?");
		newline();
		setdot();
		nonzero();
		names[c-'a'] = *addr2 | 01;
		continue;

	case 'm':
		move(0);
		fchang = 1;
		continue;

	case '\n':
		if(addr2 == 0)  addr2 = dot+1;
		if(addr1 == 0)  addr1 = addr2;
		setdot();
		printlin();
		continue;

	case 'z':
		getzlen();
		zflag++;
		printlin();
		continue;

	case 'n':
	case 'l':
	case 'p':
		if(c == 'p')  listf = 0;
		if(c == 'l')  listf = 1;
		if(c == 'n')  listf = 2;
		newline();
		setdot();
		printlin();
		continue;

	case 'q':
		setnoaddr();
		newline();
		return;

	case 'e':
		setnoaddr();
		initrd = 1;
		if((peekc = getchar()) != ' ')  errfunc("?");
		savedfile[0] = 0;
		init();
		addr2 = zero;

	case 'r':
		filename();
		if((io = open(file, 0, 0)) < 0) {
			lastc = '\n';
			errfunc("?");
		}
		setall();
		ninbuf = 0;
		append(getfile, addr2);
		exfile();
		if(initrd)  initrd = 0;
		else  fchang = 1;
		continue;

	case 's':
		setdot();
		nonzero();
		substitute(globp);
		fchang = 1;
		continue;

	case 't':
		move(1);
		fchang = 1;
		continue;

	case 'v':
		global(0);
		fchang = 1;
		continue;

	case 'w':
		setall();
		nonzero();
		if(filename())  fchang = 0;
		if((io = creat(file, 0666, 0)) < 0)  errfunc("?");
		putfile();
		exfile();
		continue;

	case 'u':
		setall();
		nonzero();
		if(filename())  fchang = 0;
		if((io = open(file,1,0)) < 0)  errfunc("?");
		if(lseek(io,0L,2) < 0) {
			exfile();
			errfunc("?");
		}
		putfile();
		exfile();
		continue;

	case '=':
		setall();
		newline();
		count[1] = (addr2 - zero)&077777;
		putd();
		putchar('\n');
		continue;

	case '!':
		unix();
		continue;

	case EOF:
		return;
	}

	errfunc("?");
	}
}

char *address() {

	register *a1, minus, c;
	int n, relerr;

	minus = 0;
	a1 = 0;

	for(;;) {
		c = getchar();

		if('0' <= c && c <= '9') {
			n = 0;

			do {
				n =* 10;
				n =+ c - '0';
			} while((c = getchar()) >= '0' && c <= '9');

			peekc = c;
			if(a1 == 0)  a1 = zero;
			if(minus < 0)  n = -n;
			a1 =+ n;
			minus = 0;
			continue;
		}

		relerr = 0;
		if(a1 || minus)  relerr++;

		switch(c) {

		case ' ':
		case '\t':
			continue;

		case '+':
			minus++;
			if(a1 == 0)  a1 = dot;
			continue;

		case '-':
		case '^':
			minus--;
			if(a1 == 0)  a1 = dot;
			continue;

		case '?':
		case '/':
			compile(c);
			a1 = dot;

			for(;;) {
				if(c == '/') {
					a1++;
					if(a1 > dol)  a1 = zero;
				} else {
					a1--;
					if(a1 < zero)  a1 = dol;
				}
				if(execute(0, a1))  break;
				if(a1 == dot)  errfunc("?");
			}
			break;
	
		case '$':
			a1 = dol;
			break;

		case '.':
			a1 = dot;
			break;

		case '\'':
			if((c = getchar()) < 'a' || c > 'z')  errfunc("?");
			for(a1 = zero+1; a1 <= dol; a1++)
				if(names[c-'a'] == (*a1|01))  break;
			break;

		default:
			peekc = c;
			if(a1 == 0)  return(0);
			a1 =+ minus;
			if(a1 < zero || a1 > dol)  errfunc("?");
			return(a1);
		}

		if(relerr)  errfunc("?");
	}
}

onhup()  {

	globp = "w ed-hup\n";
	commands();
	unlink(tfname);
	exit(1);
}

dontgo() {

	fchang = 0;
	errfunc("modified file not written out");
}

setdot() {

	if(addr2 == 0)  addr1 = addr2 = dot;
	if(addr1 > addr2)  errfunc("?");
}

setall() {

	if(addr2 == 0) {
		addr1 = zero+1;
		addr2 = dol;
		if(dol == zero)  addr1 = zero;
	}
	setdot();
}

setnoaddr() {

	if(addr2)  errfunc("?");
}

nonzero() {

	if(addr1 <= zero)  errfunc("?");
	if(addr1 > dol || addr2 > dol)  errfunc("?");
}

newline() {

	register c;

	if((c = getchar()) == '\n')  return;

	if(c == 'p' || c == 'l' || c == 'n') {
		pflag++;
		if(c == 'l')  listf = 1;
		if(c == 'p')  listf = 0;
		if(c == 'n')  listf = 2;
		if(getchar() == '\n')  return;
	}

	errfunc("?");
}

filename() {

	register char *p1, *p2;
	register c;

	count[1] = 0;
	c = getchar();

	if(c == '\n' || c == EOF) {
		p1 = savedfile;
		if(*p1 == 0)  errfunc("?");
		p2 = file;
		while(*p2++ = *p1++);
		return(1);
	}

	if(c != ' ')  errfunc("?");
	while((c = getchar()) == ' ');
	if(c == '\n')  errfunc("?");
	p1 = file;

	do {
		*p1++ = c;
	} while((c = getchar()) != '\n');

	*p1++ = 0;

	if(savedfile[0] == 0) {
		p1 = savedfile;
		p2 = file;
		while(*p1++ = *p2++);
	}

	return(0);
}

exfile() {

	close(io);
	io = -1;
	if(vflag) {
		putd();
		putchar('\n');
	}
}

getzlen() {

	register  char c,zmode;
	register  int i;

	while((c = getchar()) == ' ');

	if(c == '.' || c == '-' || c == '+')  {
		zmode = c;
		c = getchar();
	}  else  zmode = '+';

	if(c >= '0' && c <= '9') {
		i = c - '0';
		while((c = getchar()) >= '0' && c <= '9')
			i = i*10 + (c - '0');
	}  else  i = zlength;

	peekc = c;
	newline();
	zlength = i;
	if(addr2 == 0)  addr1 = dot;

	switch(zmode)  {

		case '.':
			addr1 =- zlength/2;
			break;

		case '-':
			addr1 =- zlength - 1;
			break;
	}

	addr2 = addr1 + zlength - 1;
	if(addr2 > dol)  addr2 = dol;
	if(addr1 <= zero)  addr1 = zero + 1;
}
		
onintr() {

/*	signal(SIGINTR, onintr);	*/
	putchar('\n');
	lastc = '\n';
	errfunc("?");
}

errfunc(s) 
char *s;
{

	register c;

	listf = 0;
	puts(s);
	count[0] = 0;
	lseek(0, 0L, 2);
	pflag = 0;
	if(globp)  lastc = '\n';
	globp = 0;
	peekc = lastc;

	while((c = getchar()) != '\n' && c != EOF);

	if(io > 0) {
		close(io);
		io = -1;
	}

	longjmp(blivot,0);
}

getchar() {

	if(lastc = peekc) {
		peekc = 0;
		return(lastc);
	}

	if(globp) {
		if((lastc = *globp++) != 0)  return(lastc);
		globp = 0;
		return(EOF);
	}

/*	if(read(0, &lastc, 1) <= 0)  return(lastc = EOF);
*/
	if((lastc=getc(stdin))<= 0)  return(lastc = EOF);
	lastc =& 0177;
	return(lastc);
}

gettty() {

	register c;
	register char *gf;
	register char *p;

	p = linebuf;
	gf = globp;

	while((c = getchar()) != '\n') {

		if(c == EOF) {
			if(gf)  peekc = c;
			return(c);
		}

		if((c =& 0177) == 0)  continue;
		*p++ = c;
		if(p >= &linebuf[LBSIZE-2])  errfunc("?");
	}

	*p++ = 0;
	if(linebuf[0] == '.' && linebuf[1] == 0)  return(EOF);
	return(0);
}

getfile() {

	register c;
	register char *lp, *fp;

	lp = linebuf;
	fp = nextip;

	do {
		if(--ninbuf < 0) {
			if((ninbuf = read(io, genbuf, LBSIZE)-1) < 0)
				return(EOF);
			fp = genbuf;
		}

		if(lp >= &linebuf[LBSIZE])  errfunc("?");

		if((*lp++ = c = *fp++ & 0177) == 0) {
			lp--;
			continue;
		}

		if(++count[1] == 0)  ++count[0];
	} while(c != '\n');
	*--lp = 0;
	nextip = fp;
	return(0);
}

putfile() {

	int *a1;
	register char *fp, *lp;
	register nib;

	nib = 512;
	fp = &genbuf[0];
	a1 = addr1;

	do {
		lp = getline(*a1++);
		for(;;) {
			if(--nib < 0) {
				write(io, genbuf, (int)(fp-genbuf));
				nib = 511;
				fp = genbuf;
			}

			if(++count[1] == 0)  ++count[0];

			if((*fp++ = *lp++) == 0) {
				fp[-1] = '\n';
				break;
			}
		}
	} while(a1 <= addr2);

	write(io, genbuf, (int)(fp-genbuf));
}

join() {

	register char *a1,*a2;

	a1 = genbuf;
	addr1 = addr2 - 1;

	newline();
	nonzero();

	a2 = getline(*addr2);
	while(*a1++ = *a2++);

	a1 = getline(*addr1);
	while(*a1++);
	--a1;

	a2 = genbuf;
	while(*a1++ = *a2++);

	*addr2 = putline();
	addr2 = addr1;

	delete(1);

	return(0);
}

append(f, a)
int (*f)(); 
char *a;
{

	register *a1, *a2, *rdot;
	int nline, tl;
	struct { long integer; };
/*	struct { int integer; }; */

	nline = 0;
	dot = a;

	while((*f)() == 0) {
		if(dol >= endcore) {
			if(sbrk(1024) == -1)  errfunc("?");
			endcore.integer =+ 1024;
		}
		tl = putline();
		nline++;
		a1 = ++dol;
		a2 = a1+1;
		rdot = ++dot;
		while(a1 > rdot)  *--a2 = *--a1;
		*rdot = tl;
	}
	return(nline);
}

unix() {

/*	register savint, pid, rpid;
	int retcode;

	setnoaddr();

	if((pid = fork()) == 0) {
		signal(SIGHUP,onhup);
		signal(SIGQUIT, onquit);
		execl("/bin/sh", "sh", "-t", 0);
		exit();
	}

	savint = signal(SIGINTR, 1);

	while((rpid = wait(&retcode)) != pid && rpid != -1);

	signal(SIGINTR, savint);

	puts("!");
*/
	errfunc("On CP/M??");
}

delete(join_called) {

	register *a1, *a2, *a3;

	setdot();

	if(!join_called) {
		newline();
		nonzero();
	}

	a1 = addr1;
	a2 = addr2+1;
	a3 = dol;
	dol =- a2 - a1;

	do {
		*a1++ = *a2++;
	} while(a2 <= a3);

	a1 = addr1;
	if(a1 > dol)  a1 = dol;
	dot = a1;
}

char *getline(tl) {

	register char *bp, *lp;
	register nl;

	lp = linebuf;
	bp = getblock(tl, READ);
	nl = nleft;
	tl =& ~0377;

	while(*lp++ = *bp++) {
		if(--nl == 0) {
			bp = getblock(tl=+0400, READ);
			nl = nleft;
		}
	}

	return(linebuf);
}

putline() {

	register char *bp, *lp;
	register nl;
	int tl;

	lp = linebuf;
	tl = tline;
	bp = getblock(tl, WRITE);
	nl = nleft;
	tl =& ~0377;

	while(*bp = *lp++) {
		if(*bp++ == '\n') {
			*--bp = 0;
			linebp = lp;
			break;
		}
		if(--nl == 0) {
			bp = getblock(tl=+0400, WRITE);
			nl = nleft;
		}
	}

	nl = tline;
	tline =+ (((lp-linebuf)+03)>>1)&077776;
	return(nl);
}

char *getblock(atl, iof) {

	extern read(), write();
	register bno, off;
	
	bno = (atl>>8)&0377;
	off = (atl<<1)&0774;

	if(bno >= 255) {
		puts(TMPERR);
		errfunc("?");
	}

	nleft = 512 - off;

	if(bno == iblock) {
		ichanged =| iof;
		return(ibuff+off);
	}

	if(bno == oblock)  return(obuff+off);

	if(iof == READ) {
		if(ichanged)  blkio(iblock, ibuff, write);
		ichanged = 0;
		iblock = bno;
		blkio(bno, ibuff, read);
		return(ibuff+off);
	}

	if(oblock >= 0)  blkio(oblock, obuff, write);
	oblock = bno;
	return(obuff+off);
}

blkio(b, buf, iofcn)
char *buf;
int (*iofcn)(); 
{
	register i;
	int read();
	lseek(tfile, (long)b*512L, 0);

	if((i = (*iofcn)(tfile, buf, 512)) != 512)
	{
		if(iofcn == &read && i >= 0)
		{

		  for(; i < 512; i++)
		  	buf[i] = 0;
		  return(0);
		}
		else
		{
			puts(TMPERR);
			errfunc("?");
		}
	}

}

init() {

	register char *p;
	register pid;

	close(tfile);
	tline = 0;
	iblock = -1;
	oblock = -1;
	tfname = "exxxxx.tmp";
	ichanged = 0;
/*	pid = getpid();

	for(p = &tfname[11]; p > &tfname[6];) {
		*--p = (pid&07) + '0';
		pid =>> 3;
	}
*/
	tfile = creatb(tfname, 2);
	brk(fendcore);
	dot = zero = dol = fendcore;
	endcore = fendcore - 2;
}

global(k) {

	register char *gp;
	register c;
	register int *a1;
	char globuf[GBSIZE];

	if(globp)  errfunc("?");
	setall();
	nonzero();
	if((c = getchar()) == '\n')  errfunc("?");
	compile(c);
	gp = globuf;

	while((c = getchar()) != '\n') {
		if(c == EOF)  errfunc("?");
		if(c == '\\') {
			c = getchar();
			if(c != '\n')  *gp++ = '\\';
		}
		*gp++ = c;
		if(gp >= &globuf[GBSIZE-2])  errfunc("?");
	}

	*gp++ = '\n';
	*gp++ = 0;

	for(a1 = zero; a1 <= dol; a1++) {
		*a1 =& ~01;
		if(a1 >= addr1 && a1 <= addr2 && execute(0,a1) == k)  *a1 =| 01;
	}

	for(a1 = zero; a1 <= dol; a1++) {
		if(*a1 & 01) {
			*a1 =& ~01;
			dot = a1;
			globp = globuf;
			commands();
			a1 = zero;
		}
	}
}

substitute(inglob) {

	register gsubf, *a1, nl;
	int getsub();
	int tst;

	gsubf = compsub();

	for(a1 = addr1; a1 <= addr2; a1++) {
		if(execute(0, a1) == 0)  continue;
		inglob =| 01;
		dosub();
		if(gsubf) {
			while(*loc2) {
				if(execute(1) == 0)  break;
				dosub();
			}
		}
		tst = chkmrk(a1);
		*a1 = putline();
		nl = append(getsub, a1);
		if(tst >= 0)  names[tst] = *a1|01;
		a1 =+ nl;
		addr2 =+ nl;
	}

	if(inglob == 0)  errfunc("?");
}

chkmrk(a) 
char *a;
{

	register char *a1;
	register int n;

	a1 = a;

	for(n = 0;n < 26;n++) {
		if(names[n] == (*a1 | 01))  return(n);
	}
	return(-1);
}

compsub() {

	register seof, c;
	register char *p;
	int gsubf;

	if((seof = getchar()) == '\n')  errfunc("?");
	compile(seof);
	p = rhsbuf;

	for(;;) {
		c = getchar();
		if(c == '\\')  c = getchar() | 0200;
		if(c == '\n')  errfunc("?");
		if(c == seof)  break;
		*p++ = c;
		if(p >= &rhsbuf[LBSIZE/2])  errfunc("?");
	}

	*p++ = 0;

	if((peekc = getchar()) == 'g') {
		peekc = 0;
		newline();
		return(1);
	}

	newline();
	return(0);
}

getsub() {

	register char *p1, *p2;

	p1 = linebuf;
	if((p2 = linebp) == 0)  return(EOF);
	while(*p1++ = *p2++);
	linebp = 0;
	return(0);
}

dosub() {

	register char *lp, *sp, *rp;
	int c;

	lp = linebuf;
	sp = genbuf;
	rp = rhsbuf;
	while(lp < loc1)  *sp++ = *lp++;

	while(c = *rp++) {
		if(c == '&') {
			sp = place(sp, loc1, loc2);
			continue;
		} else  if(c < 0 && (c =& 0177) >= '1' && c < NBRA+'1') {
			  sp = place(sp, braslist[c-'1'], braelist[c-'1']);
			  continue;
			}
		*sp++ = c&0177;
		if(sp >= &genbuf[LBSIZE])  errfunc("?");
	}

	lp = loc2;
	loc2 = sp + (long)&linebuf[0] - (long)&genbuf[0];

	while(*sp++ = *lp++)
		if(sp >= &genbuf[LBSIZE])  errfunc("?");

	lp = linebuf;
	sp = genbuf;
	while(*lp++ = *sp++);
}

char *place(asp, al1, al2) 
char *asp, *al1, *al2;
{

	register char *sp, *l1, *l2;

	sp = asp;
	l1 = al1;
	l2 = al2;

	while(l1 < l2) {
		*sp++ = *l1++;
		if(sp >= &genbuf[LBSIZE])  errfunc("?");
	}

	return(sp);
}

move(cflag) {

	register int *adt, *ad1, *ad2;
	int getcopy();

	setdot();
	nonzero();
	if((adt = address()) == 0)  errfunc("?");
	newline();
	ad1 = addr1;
	ad2 = addr2;

	if(cflag) {
		ad1 = dol;
		append(getcopy, ad1++);
		ad2 = dol;
	}

	ad2++;

	if(adt < ad1) {
		dot = adt + (ad2-ad1);
		if((++adt) == ad1)  return;
		reverse(adt, ad1);
		reverse(ad1, ad2);
		reverse(adt, ad2);
	} else  if(adt >= ad2) {
			dot = adt++;
			reverse(ad1, ad2);
			reverse(ad2, adt);
			reverse(ad1, adt);
		} else  errfunc("?");
}

reverse(aa1, aa2) 
char *aa1, *aa2;
{

	register int *a1, *a2, t;
 
	a1 = aa1;
	a2 = aa2;

	for(;;) {
		t = *--a2;
		if(a2 <= a1)  return;
		*a2 = *a1;
		*a1++ = t;
	}
}

getcopy() {

	if(addr1 > addr2)  return(EOF);
	getline(*addr1++);
	return(0);
}

compile(aeof) {

	register eof, c;
	register char *ep;
	char *lastep;
	char bracket[NBRA], *bracketp;
	int nbra;
	int cclcnt;

	ep = expbuf;
	eof = aeof;
	bracketp = bracket;
	nbra = 0;

	if((c = getchar()) == eof) {
		if(*ep == 0)  errfunc("?");
		return;
	}

	circfl = 0;

	if(c == '^') {
		c = getchar();
		circfl++;
	}

	if(c == '*')  cerror();
	peekc = c;

	for(;;) {
		if(ep >= &expbuf[ESIZE])  cerror();
		c = getchar();

		if(c == eof) {
			*ep++ = CEOF;
			return;
		}

		if(c != '*')  lastep = ep;

		switch(c) {

		case '\\':
			if((c = getchar()) == '(') {
				if(nbra >= NBRA)  cerror();
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;
			}
			if(c == ')') {
				if(bracketp <= bracket)  cerror();
				*ep++ = CKET;
				*ep++ = *--bracketp;
				continue;
			}
			*ep++ = CCHR;
			if(c == '\n')  cerror();
			*ep++ = c;
			continue;

		case '.':
			*ep++ = CDOT;
			continue;

		case '\n':
			cerror();

		case '*':
			if(*lastep == CBRA || *lastep == CKET)  errfunc("?");
			*lastep =| STAR;
			continue;

		case '$':
			if((peekc = getchar()) != eof)  goto defchar;
			*ep++ = CDOL;
			continue;

		case '[':
			*ep++ = CCL;
			*ep++ = 0;
			cclcnt = 1;
			if((c = getchar()) == '^') {
					c = getchar();
					ep[-2] = NCCL;
			}
			do {
				if(c == '\n')  cerror();
				*ep++ = c;
				cclcnt++;
				if(ep >= &expbuf[ESIZE])  cerror();
			} while((c = getchar()) != ']');
			lastep[1] = cclcnt;
			continue;

		defchar:
		default:
			*ep++ = CCHR;
			*ep++ = c;
		}
	}
}

cerror() {

	expbuf[0] = 0;
	errfunc("?");
}

execute(gf, addr)
int *addr; {

	register char *p1, *p2, c;

	if(gf) {
		if(circfl)  return(0);
		p1 = linebuf;
		p2 = genbuf;
		while(*p1++ = *p2++);
		locs = p1 = loc2;
	} else {
		if(addr == zero)  return(0);
		p1 = getline(*addr);
		locs = 0;
	}

	p2 = expbuf;

	if(circfl) {
		loc1 = p1;
		return(advance(p1, p2));
	}

	/* fast check for first character */
	if(*p2 == CCHR) {
		c = p2[1];
		do {
			if(*p1 != c)  continue;
			if(advance(p1, p2)) {
				loc1 = p1;
				return(1);
			}
		} while(*p1++);
		return(0);
	}

	/* regular algorithm */
	do {
		if(advance(p1, p2)) {
			loc1 = p1;
			return(1);
		}
	} while(*p1++);

	return(0);
}

advance(alp, aep) 
char *alp, *aep;
{

	register char *lp, *ep, *curlp;
	char *nextep;

	lp = alp;
	ep = aep;

	for(;;)
	switch(*ep++) {

	case CCHR:
		if(*ep++ == *lp++)  continue;
		return(0);

	case CDOT:
		if(*lp++)  continue;
		return(0);

	case CDOL:
		if(*lp == 0)  continue;
		return(0);

	case CEOF:
		loc2 = lp;
		return(1);

	case CCL:
		if(cclass(ep, *lp++, 1)) {
			ep =+ *ep;
			continue;
		}
		return(0);

	case NCCL:
		if(cclass(ep, *lp++, 0)) {
			ep =+ *ep;
			continue;
		}
		return(0);

	case CBRA:
		braslist[*ep++] = lp;
		continue;

	case CKET:
		braelist[*ep++] = lp;
		continue;

	case CDOT|STAR:
		curlp = lp;
		while(*lp++);
		goto star;

	case CCHR|STAR:
		curlp = lp;
		while(*lp++ == *ep);
		ep++;
		goto star;
 
	case CCL|STAR:
	case NCCL|STAR:
		curlp = lp;
		while(cclass(ep, *lp++, ep[-1] == (CCL|STAR)));
		ep =+ *ep;
	star:
		do {
			lp--;
			if(lp == locs)  break;
			if(advance(lp, ep))  return(1);
		} while(lp > curlp);
		return(0);

	default:
		errfunc("?");
	}
}

cclass(aset, ac, af) 
char *aset;
{

	register char *set, c;
	register n;
 
	set = aset;
	if((c = ac) == 0)  return(0);
	n = *set++;

	while(--n)
		if(*set++ == c)  return(af);

	return(!af);
}

putd() {

/*	register r;
	extern ldivr;
 
	count[1] = ldiv(count[0], count[1], 10);
	count[0] = 0;
	r = ldivr;
	if(count[1])  putd();
	putchar(r + '0');
*/
	long	j;
	struct	{short	hiword; short loword;};
	j.hiword = count[0];
	j.loword = count[1];
	printf("%6ld	",j);	
}

puts(as) 
char *as;
{

	register char *sp;

	sp = as;
	col = 0;
	while(*sp)  putchar(*sp++);
	putchar('\n');
}

char	line[70];
char	*linp	line;

putchar(ac) {

	register char *lp;
	register c;
 
	lp = linp;
	c = ac;

	if(listf == 1) {
		col++;
		if(col >= 72) {
			col = 0;
			*lp++ = '\\';
			*lp++ = '\n';
		}
		if(c == '\t') {
			c = '>';
			goto esc;
		}
		if(c == '\b') {
			c = '<';
		esc:
			*lp++ = '-';
			*lp++ = '\b';
			*lp++ = c;
			goto out;
		}
		if(c < ' ' && c != '\n') {
			*lp++ = '\\';
			*lp++ = (c>>3) + '0';
			*lp++ = (c&07) + '0';
			col =+ 2;
			goto out;
		}
	}
	*lp++ = c;
out:
	if(c == '\n' || lp >= &line[64]) {
		linp = line;
		write(1,line,(int)(lp-line));
		return;
	}
	linp = lp;
}

printlin() {

	register *a1;
	register  char *s,*t;

	nonzero();
	a1 = addr1;

	do  {
		if(listf == 2) {
			count[1] = (a1-zero)&077777;
			putd();
			putchar('\t');
		}
		t = getline(*a1++);
		if(zflag) {
			for(s = t; *s; s++) {
				if(*s == NP) {
					*s = 0;
					puts(t);
					dot = a1;
					zflag = 0;
					return;
				}
			}
		}
		puts(t);
	} while(a1 <= addr2);

	dot = addr2;
	zflag = 0;
	return;
}

setprt() {

	register seof,*a1;
	register  char *lp;
	char *sp;
	int part();

	setdot();
	nonzero();
	if((seof = getchar()) == '\n')  errfunc("?");
	compile(seof);
	newline();

	for(a1 = addr1; a1 <= addr2; ++a1) {
		getline(*a1);
		if(execute(0,a1) == 0)  continue;
		++addr2;
		lp = loc2;
		while(*lp++);
		sp = lp;
		while(--lp >= loc2)   *sp-- = *lp;
		*loc2 = 0;
		*a1 = putline();
		lp = linebuf;
		sp = loc2 + 1;
		while(*lp++ = *sp++);
		brktst = 0;
		append(part,a1);
		dot = ++a1;
	}

	return(0);
}

part() {
	return(brktst++);
}
