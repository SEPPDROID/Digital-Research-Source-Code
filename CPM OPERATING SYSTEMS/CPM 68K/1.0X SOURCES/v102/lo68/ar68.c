/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/
#include <stdio.h>
#include <klib.h>
#include "machine.h"
#include "iobuf.h"
#include "cout.h"

#define	BLKSIZE	512
char buff[BLKSIZE]={0};

/* flags for cp1file*/
#define IEVEN	1
#define OEVEN	2
#define WHDR	4

#define	DIR	0100000
#define	CHR	040000
#define	BLK	020000
#define	ROWN	0400
#define	WOWN	0200
#define	XOWN	0100
#define	RGRP	040
#define	WGRP	020
#define	XGRP	010
#define	ROTH	04
#define	WOTH	02
#define	XOTH	01

#define LIBMAGIC	0177545

#define LNAMLEN 14
struct lib2hdr {
	char l2fname[LNAMLEN];
	long l2modti;
	char l2userid;
	char l2gid;
	short  l2fimode;
	long l2fsize;
	short l2junk;
} libhd={0};

struct {
	int f_devnum;
	int f_inum;
	int f_fimode;
	char f_nlinks;
	char f_userid;
	char f_gid;
	char f_size0;
	int f_size1;
	int f_addr[8];
	long f_acti;
	long f_modti;
};

short libmagic LIBMAGIC;

int rflg=0, dflg=0, xflg=0, tflg=0, vflg=0;
int uflg=0;
int pflg=0;
int areof=0;
int arfd=0;
int tempfd=0;
int aflg=0, bflg=0;
int psflg=0;
int matchflg=0;
char *psname=0;

char *arname=0,tempname[80]={0,0,0};
					/************************/
#ifdef UNIX				/* 			*/
char *dname="/tmp/";			/* UNIX			*/
#else					/* CP/M and VMS		*/
char *dname="";				/* Disk Name		*/
#endif					/************************/
#ifdef	UNIX				/* 			*/
char *tname="arXXXXXX";			/* Unix temp file name	*/
#else					/* CP/M and VMS		*/
char *tname="ar68.tmp";			/* Temp file name	*/
#endif					/************************/

long lseek();
long fixlong();
long filesize();
int endit();

int (*docom)()=0;
int replace();
int delete();
int extract();
int xtell();
#ifdef JUNK
long mklong();
#endif
int print();
#ifdef	VAX
struct {
	short loword;
	short hiword;
};
#endif

#ifdef	PDP11
struct {
	short loword;
	short hiword;
};
#endif

#ifdef	MC68000
struct	{
	short hiword;
	short loword;
};
#endif

main(argc,argv)
char **argv;
{

	register char *p1, *p2;
	register char **ap;
	register int i;
	register long l;
	register long k;
	long	kk;
	if(argc < 3) {
usage:
		error("usage: ar68 drtwx[abiv][f d:] [opmod] archive obmod1 [obmod2] ... [>filespec]\n");

		endit();
	}
/*	signal(1,endit);	*/
	ap = &argv[1];
	p1 = *ap++;
	i = argc-3;
	while(*p1) {
		switch(*p1++) {

			case 'r':
				rflg++;
				docom = replace;
				break;

			case 'a':
				aflg++;
				psname = *ap++;
				i--;
				break;

/*	These options removed because they don't work!

			case 'b':
			case 'i':
				bflg++;
				i--;
				psname = *ap++;
				break;

*/
			case 'd':
				dflg++;
				docom = delete;
				break;

			case 'x':
				xflg++;
				docom = extract;
				break;

			case 't':
				tflg++;
				docom = xtell;
				break;

			case 'w':
				pflg++;
				docom = print;
				break;

			case 'v':
				vflg++;
				break;

			case 'f':		/* Drive for temp files */
				dname = *ap++;	/* Set proper disk name */
				break;
			default:
				error("invalid option flag: %c\n",*--p1);
				endit();
		}
	}
	uflg = rflg+dflg;
	if((uflg+xflg+tflg+pflg) != 1) {
		error("one and only one of drtwx flags required\n");
		endit();
	}
	psflg = aflg+bflg;
	if(psflg > 1) {
		error("only one of abi flags allowed\n");
		endit();
	}
	strcat (tempname,dname);		/* Form 1st part of string */
	strcat (tempname,tname);		/* Add in file name	   */

#ifdef	UNIX					/* For UNIX, make temp file*/
	mktemp(tempname);
#endif

	arname = *ap++;
	arfd = openar(arname,rflg);
	if(i==0 && tflg) {
		listall();
		endit();
	}
	if(i<=0)
		goto usage;
	if( xflg == 0 ) {
		if((tempfd=creat(tempname,0644,1)) < 0) {
			error("cannot create %s\n",tempname);
			endit();
		}
		if(write(tempfd,&libmagic,sizeof libmagic) != sizeof libmagic)
		{
			error("temp file write error\n");
			endit();
		}
	}
	while(i) {
		excom(*ap++);
		i--;
	}
	excom(0L);		/*end of arg list*/
	if (xflg == 0)
	{
		k = lseek(tempfd,0L,1);		/* Fetch file size	*/
#ifndef	UNIX
		l = lseek(tempfd,0L,0);		/* Rewind temp file	*/
		if (l < 0)			/* Error?		*/
		{
			printf("seek error on %s\n",tempname);
			exit(-1);
		}
#else
		close(tempfd);
		if((tempfd=open(tempname,0,1)) < 0)
		{
		  printf("Cannot re-open %s\n",tempname);
		  exit(-1);
		}
#endif
		close(arfd);			/* Close input file	*/
		unlink(arname);			/* Delete the file	*/
		arfd=creat(arname,0666,1);	/* Create the file again*/
		if(arfd < 0)			/* Error?		*/
		{
		  printf("Unable to re-create %s -- archive is in %s\n",
		  arname,tempname);
		  exit(-1);
		}
		i = 1;				/* Klutz		*/
		while (i > 0 && k > 0)		/* Copy the file back	*/
		{
			i = (k > BLKSIZE) ? BLKSIZE : k;
			i = read(tempfd,buff,i);
			if(i < 0)
			{
				printf("Read error on %s\n",tempname);
				exit(-1);
			}
			else if (i > 0)
			{
				if(write(arfd,buff,i) != i)
				{
					printf("Write error on %s\n",arname);
					exit (-1);
				}
			}
			k -= i;
		}
		kk = 0;			
		write(arfd,&kk,sizeof(k));	/* Put 0 on end of file */
		close (tempfd);
		unlink(tempname);
	}
	endit();
}

openar(arp,crfl)
char *arp;
{

	register i;
	int ib;

	if((i=open(arp,2,1)) < 0) {	/*does not exist*/
		areof = 1;
		if((i = creat(arp,0666,1)) < 0)
		{
			printf("Cannot Create %s\n",arname);
			exit(-1);
		}
		else
			return(i);
	}
	if(read(i,&ib,sizeof libmagic) != sizeof libmagic || ib != LIBMAGIC) {
notar:
		error("not archive format: %s\n",arp);
		endit();
	}
	return(i);
}

/* execute one command -- call with filename or 0*/

excom(ap)
char *ap;
{

	register char *p;
	if(p=ap)
	{
		if(p[1] == ':')			/* CP/M Drive number	   */
			p += 2;			/* Bump to file name	   */
	}
	if((p) && (matchflg==0)) {		/*look thru archive for file name*/
		while(areof==0 && nextar()) {	/*get next ar file header*/
			if(streq(p,&libhd.l2fname[0]))
				break;
			if(psflg && streq(psname,&libhd.l2fname[0])) {
				break;
			}
			skcopy(uflg);	/*skip or copy*/
		}
	}
	(*docom)(ap);	/*exec requested command*/
}

xtell(ap)
char *ap;
{

	register char *p;
	register i;
	register char *p1;

	if(ap==0)
		endit();
	if(vflg) {		/*long list*/
		pmode(libhd.l2fimode);
		printf(" %d/%d %6ld ",libhd.l2userid,libhd.l2gid,
				libhd.l2fsize);
/*		p1 = ctime(&libhd.l2modti);
		p1[24] = '\0';
		p1 =+ 4;
		printf("  %s  ",p1); */
	}
	pfname();
	skcopy(0);
}

pfname()
{
	printf("%.14s\n",libhd.l2fname);
}

replace(ap)
char *ap;
{

	register i;
	int ifd;
	register long l;
	register char *p1;

	if( ap == 0 ) {
		if( bflg && areof == 0 ) {
			l = -(sizeof libhd);
			lseek(arfd,l,1);
		}
		cprest();
		return;
	}
	if((ifd=open(ap,0,1)) < 0) {
		error("cannot open %s\n",ap);
		endit();
	}
	if(areof && psflg) {
		error("%s not in library\n",psname);
		endit();
	}
	if((bflg|aflg) && matchflg==0) { /*copy archive file before appending*/
		if(aflg)
			skcopy(1);
		matchflg++;
	}
	p1 = (ap[1] == ':') ? ap + 2 : ap;	/* Drive name	*/
	copystr(p1,&libhd.l2fname[0],LNAMLEN);
	if(areof|aflg) {
		inform('a');
	}
	else if(bflg) {
		inform('i');
	}
	else {
		inform('r');
		skcopy(0);		/*skip old copy*/
	}
	p1 = buff;
	libhd.l2modti = 0;
	libhd.l2userid = 0;
	libhd.l2gid = 0;
	libhd.l2fimode = 0666;
	libhd.l2fsize = filesize(&ifd,ap);
	cp1file(ifd,tempfd,WHDR+OEVEN,ap,tempname);
	close(ifd);
}

struct iobuf ssbuf={0};
long filesize(fdp,fname)
int *fdp;
char *fname;
{
	struct hdr tcout;
	register long size;
	register short *p;
	register int i;

	close(*fdp);
	if( fopen(fname,&ssbuf,1) < 0 )
		error("cannot open %s\n",fname);
	p = &tcout;
	for( i = 0; i < (sizeof tcout)/2; i++ )
		*p++ = getw(&ssbuf);
	close(ssbuf.fildes);
	if( tcout.ch_magic != MAGIC )
		error("not object file: %s\n",fname);
	size = tcout.ch_tsize + tcout.ch_dsize;
	if( tcout.ch_rlbflg == 0 )
		size =+ size;
	size =+ tcout.ch_ssize;
	if( (*fdp=open(fname,0,1)) < 0 )
		error("cannot reopen %s\n",fname);
	size =+ sizeof tcout;
#ifndef UNIX
	size = ((size+3)/4)*4;		/*turkey round-up to long for VAX*/
#endif
	return(size);
}

struct { char byt1; char byt2; char byt3; char byt4; };
long fixlong(l)
long l;
{
	long tl;

	tl.byt1 = l.byt4;
	tl.byt2 = l.byt3;
	tl.byt3 = l.byt2;
	tl.byt4 = l.byt1;
	return(tl);
}
fixshort(i)
int i;
{
	int ti;

	ti.byt1 = i.byt2;
	ti.byt2 = i.byt1;
}

delete(ap)
char *ap;
{

	if(ap==0) {
		cprest();
		return;
	}
	inform('d');
	skcopy(0);
}

extract(ap)
char *ap;
{

	register ofd;
	register long l;
	register i;

	if(ckafile(ap))
		return;
	if((ofd=creat(ap,0666)) < 0) {
		error("cannot create %s\n",ap);
		endit();
	}
	inform('x');
	cp1file(arfd,ofd,IEVEN,arname,ap);
	close(ofd);
}

print(ap)
char *ap;
{

	if(ckafile(ap))
		return;
	cp1file(arfd,1,IEVEN,arname,"std output");
}

endit()
{

	exit();
}

/* list all file in the library*/

listall()
{

	while(nextar()) {
		xtell(-1);
	}
}

/*read next ar file header into libhd*/

nextar()
{

	if( read(arfd,&libhd,sizeof libhd) != sizeof libhd ||
			libhd.l2fname[0] == '\0' ) {
		areof++;
		return(0);
	}
	return(1);
}

/* call with cpflag=0 for skip, cpflag=1 for copy*/
skcopy(cpflag)
{

	register long l;
	register int i;

	if(areof)
		return;
	l = libhd.l2fsize;
	if(l&1)
		l++;
	if(cpflag) {
		inform('c');
		cp1file(arfd,tempfd,WHDR+OEVEN+IEVEN,arname,tempname);
	}
	else {
		if(lseek(arfd,l,1) < 0) {
		  error("seek error on library\n");
		  endit();
		}
	}
}

#ifdef JUNK
char *mktemp(ap)
char *ap;
{

	register char *p;
	register i,j;

/*	i = getpid();		/*process id*/
/*	p = ap;
	while(*p)
		p++;
	for(j=0; j<5; j++) {
		*--p = ((i&7) + '0');
		i =>> 3;
	}
*/	return(ap);
}
#endif

streq(s1, s2)
char *s1, *s2;
{
	register char *p1, *p2;

	p1 = s1;
	p2 = s2;
	while(*p1++ == *p2)
		if(*p2++ == 0)
			return(1);
	return(0);
}


int	m1[] { 1, ROWN, 'r', '-' };
int	m2[] { 1, WOWN, 'w', '-' };
int	m3[] { 1, XOWN, 'x', '-' };
int	m4[] { 1, RGRP, 'r', '-' };
int	m5[] { 1, WGRP, 'w', '-' };
int	m6[] { 1, XGRP, 'x', '-' };
int	m7[] { 1, ROTH, 'r', '-' };
int	m8[] { 1, WOTH, 'w', '-' };
int	m9[] { 1, XOTH, 'x', '-' };

int	*m[] { m1, m2, m3, m4, m5, m6, m7, m8, m9};

pmode(aflg1)
{
	register int **mp;

	for (mp = &m[0]; mp < &m[9];)
		select(*mp++,aflg1);
}

select(pairp,flg)
int *pairp;
int flg;
{
	register int n, *ap, f;

	ap = pairp;
	f = flg;
	n = *ap++;
	while (--n>=0 && (f&*ap++)==0)
		ap++;
	putchar(*ap);
}

inform(cc)
char cc;
{

	if(vflg) {
		putchar(cc);
		putchar(' ');
		pfname();
	}
}

copystr(ap1,ap2,alen)
char *ap1, *ap2;
{

	register char *p1, *p2;
	register len;

	p1 = ap1;
	p2 = ap2;
	len = alen;
	while(len) {
		if(!(*p2++ = *p1++))
			break;
		len--;
	}
	while(--len > 0)
		*p2++ = '\0';
}

#ifdef JUNK
long mklong(ai1,ai2)
{

	long l;

	l.hiword = ai1;
	l.loword = ai2;
	return(l);
}
#endif

cprest()
{

	while(areof==0 && nextar())
		skcopy(1);		/*copy rest of library*/
}

cp1file(ifd,ofd,aflags,iname,oname)
char *iname, *oname;
{

	register i;
	register int j;
	register long l;
	register flags;

	flags = aflags;
	if(flags&WHDR) {
		if(write(ofd,&libhd,sizeof libhd) != sizeof libhd) {
iwrerr:
			error("write error on %s\n",oname);
			endit();
		}
	}
	for( l = libhd.l2fsize; l > BLKSIZE; l =- BLKSIZE ) {
		if((i=read(ifd,buff,BLKSIZE)) != BLKSIZE) {
irderr:
			error("read error on %s\n",iname);
			endit();
		}
		if(write(ofd,buff,BLKSIZE) != BLKSIZE) {
			goto iwrerr;
		}
	}
	if( l > 0 ) {
		i = l;
		if((j=read(ifd,buff,i)) != i)
			goto irderr;
		if(flags&OEVEN) {
			if(i&1) {
				buff[i] = '\0';
				i++;
			}
		}
		if(write(ofd,buff,i) != i)
			goto iwrerr;
	}
	if(flags&IEVEN)
		if(l&1)
			read(ifd,buff,1);
}

ckafile(ap)
char *ap;
{

	if(ap==0)
		endit();
	if(areof) {
		error("%s not in archive file\n",ap);
		return(1);
	}
	return(0);
}

error(x1,x2,x3,x4,x5,x6)
int x1, x2, x3, x4, x5, x6;
{
	printf(x1,x2,x3,x4,x5,x6);
	if( tempfd ) {
		close(tempfd);
		unlink(tempname);
	}
	exit();
}
