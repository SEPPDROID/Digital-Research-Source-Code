/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
*/

char *version = "@(#)nm - Sep 19, 1983";

#include <stdio.h>

#ifndef MC68000
#	include <c68/cout.h>
#	include <c68/sendc68.h>
#	include <c68/ar68.h>
#else
#	include <cout.h>
#	include <sendc68.h>
#	include <ar68.h>
#endif

#define SYNAMLEN	8
char outbuf1[BUFSIZ];
int loctr;
int pflg;
int symflg;
FILE *ifp;
struct hdr2 couthd;
struct libhdr libhd;
long l, value;
int flags;
char symbol[20];
long offset;
int magic;
char eflag, gflag, qflag, xflag, dflag, tflag, bflag, aflag;
int optioncount;

main(argc,argv)
char **argv;
{
	int argc_old;

	setbuf(stdout,outbuf1);
	eflag = gflag = qflag = xflag = dflag = tflag = bflag = aflag = 0;
	argc_old = argc;
	optioncount = 0;

	while (argc > 1)			/* set flags, ignore filenames	*/	
	  if (*argv[argc-1] == '-')
		{
		optioncount++;
		switch (*++argv[--argc])
		  {
		  case 'e': 
			eflag = 1; 
			break;
		  case 'g': 
			gflag = 1;
			break;
		  case 'q':
		    qflag = 1;
			break;
		  case 'x':
		    xflag = 1;
			break;
		  case 'd':
		    dflag = 1;
			break;
		  case 't':
		    tflag = 1;
			break;
		  case 'b':
			bflag = 1;
			break;
		  case 'a':
			aflag = 1;
			break;
		  default :
			printf("nm: nonexistent option %s\n", argv[argc]);
			exit(-1);
			break;
		  }
	    --argv[argc];
		}
		else argc--;

	argc = argc_old;
	if ((argc - optioncount) <= 1 )
		*argv = "a.out";		/* default */
	else {
		--argc;
		++argv;
	}

	/*	process each file - ignore options in parameter list	*/

	while (argc--) {
		if (**argv == '-') {
			argv++;
			continue;
		}
		if (!openfile(*argv++))
			continue;
		offset = 0;
		lgetw(&magic, ifp);			/* get magic number */
		if (magic == LIBMAGIC) {	/* we've got a library (archive) */
			offset = sizeof(magic);
			while (getarhd(ifp, &libhd) != EOF && libhd.lfname[0] != 0) {
				offset += sizeof(libhd);
				ifilname = libhd.lfname;
				prtsym();		/* check for a.out format and print symbols */
				offset += libhd.lfsize;
#ifdef PDP11
				longseek(offset, fileno(ifp), 0);
#else
				fseek(ifp, offset, 0);
#endif
			}
			fclose(ifp);
			continue;
		}
#ifdef PDP11
		longseek(0L, fileno(ifp), 0);
#else
		fseek(ifp, 0L, 0);		/* put the magic number back */
#endif
		prtsym();				/* check for a.out format and print symbols */
		fclose(ifp);
	}
	exit(0);
}

prtsym()		/* print all symbols for current object file */
{
	register long symsize;
	register char *p;
	register int i, c;

	printf("%s:\n",ifilname);
	if (!readhdr())
		return;						/* not a.out format */
	if (couthd.ch_magic != EX_ABMAGIC)
		l = couthd.ch_tsize+couthd.ch_dsize+HDSIZE;
	else
		l = couthd.ch_tsize+couthd.ch_dsize+HDSIZ2;
#ifdef PDP11
	if( longseek(l+offest,fileno(ifp),3) == 0 )
#else
	if( fseek(ifp ,l+offset ,0) == EOF )
#endif
		return;
	for (symsize = couthd.ch_ssize; symsize > 0; symsize -= OSTSIZE) {
		p = symbol;
		for (i = SYNAMLEN; --i != -1;) {
			if( (c = getc(ifp)) > 0 )
				*p++ = c;
			else
				*p = '\0';
		}
		lgetw(&flags,ifp);
		lgetl(&value,ifp);
		if (!optioncount || accept(flags)) {
			printf("%-11s",symbol);
			printf("%8lx",value);
			prtflags(flags);
		}
	}
}

accept(af)
int af;
{
	register int f;

	f = af;
	if (eflag&&(f&SYEQ))
		return(1);
	if (gflag&&(f&SYGL))
		return(1);
	if (qflag&&(f&SYER))
		return(1);
	if (xflag&&(f&SYXR))
		return(1);
	if (dflag&&(f&SYDA))
		return(1);
	if (tflag&&(f&SYTX))
		return(1);
	if (bflag&&(f&SYBS))
		return(1);
	if (aflag&&((!(f&SYDA))&&(!(f&SYTX))&&(!(f&SYBS))))
		return(1);

	return(0);
}


prtflags(af)
{
	register int f;

	f = af;
	if( f&SYEQ )
		printf(" equ");
	if( f&SYGL )
		printf(" global");
	if( f&SYER )
		printf(" reg");
	if( f&SYXR )
		printf(" external");
	if( f&SYFN)
		printf(" filename");
	if( f&SYDA )
		printf(" data");
	else if( f&SYTX )
		printf(" text");
	else if( f&SYBS )
		printf(" bss");
	else
		printf(" abs");
	putchar('\n');
}

openfile(ap)
char *ap;
{
	register char *p;

	p = ap;
	if( (ifp = fopen(p,"r")) == NULL ) {
		printf("unable to open %s\n",p);
		return(0);
	}
	ifilname = p;	/* point to current file name for error msgs */
	return(1);
}

readhdr()
{
	if( getchd(ifp,&couthd) != 0 ) {
		printf("error reading %s\n",ifilname);
		return(0);
	}

	if( couthd.ch_magic  < MAGIC || couthd.ch_magic > MAGICST ) {
		printf("file format error: %s %x\n",ifilname,couthd.ch_magic);
		return(0);
	}
	return(1);
}

#ifdef PDP11
longseek(al,fildes,pn)
long al;
int fildes, pn;
{
	long l;
	register b,o;

	l = al>>9;
	b = l.loword;	/* block # */
	o = al.loword&0777;	/* offset in block */
	if( doseek(fildes,b,pn) == 0 )	/* seek to block */
		return(0);
	if( doseek(fildes,o,SEEKREL) == 0 )	/* do offset */
		return(0);
	return(1);
}

doseek(afd,aoff,apnam)
{

	if( seek(afd,aoff,apnam) < 0 ) {
		printf("seek error on file %s\n",ifilname);
		return(0);
	}
	return(1);
}
#endif
