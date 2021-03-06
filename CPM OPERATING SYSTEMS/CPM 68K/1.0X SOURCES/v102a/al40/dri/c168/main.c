/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

char *version "@(#) c168 code generator 4.0 - Feb 10, 1983";

#include "cgen.h"
#include "cskel.h"
char *opap;
int errflg;
int nextlabel	10000;
char *readtree();
char *readsym();


/* main - main routine, handles arguments and files*/
main(argc,argv)						/* returns - none*/
int argc;							/* arg count*/
char **argv;						/* arg pointers*/
{
	register char *q;
	register int i;

#ifdef VERSADOS
	lflag++;
#endif
	for( i = 3; i < argc; i++ ) {
		q = argv[i];
		if( *q++ != '-' )
			usage(argc,argv,1);
		while( 1 ) {
			switch( *q++ ) {

			case 'D':
			case 'd':
				dflag++;
				continue;

			case 'L':
			case 'l':
				lflag++;
				continue;

			case 'E':
			case 'e':
				eflag++;
				continue;

			case 'F':
			case 'f':
				fflag++;
				continue;

			case 'M':
			case 'm':
				mflag++;
				continue;

			case 'O':
			case 'o':
				oflag++;
				continue;

			case 'C':
			case 'c':
				cflag++;
				continue;

			case '\0':
				break;

			default:
				usage(argc,argv,2);
			}
			break;
		}
	}
	if( argc < 3 )
		usage(argc,argv,3);
	if( fopen(argv[1],&ibuf,0) < 0 )	/* 3rd arg for versados */
		ferror("can't open %s\n",argv[1]);
	if( fcreat(argv[2],&obuf,0) < 0 )	/* 3rd arg for versados */
		ferror("can't create %s\n",argv[2]);
	readicode();
	v6flush(&obuf);
	exit(errcnt!=0);
}

/* readicode - read intermediate code and dispatch output*/
/*		This copies assembler lines beginning with '(' to assembler*/
/*		output and builds trees starting with '.' line.*/
readicode()								/*returns - none*/
{
	register int c;
	register struct tnode *tp;

	while( (c=getc(&ibuf)) > 0 ) {
		switch(c) {

		case '.':
			lineno = readint();
			opap = exprarea;
			if( tp = readtree() ) {
#ifndef	NODEBUG
				if( cflag )
					putexpr("readicode",tp);
#endif
				switch( tp->t_op ) {

				case INIT:
					outinit(tp->t_left);
					break;

				case CFORREG:
					outcforreg(tp->t_left);
					break;

				case IFGOTO:
					outifgoto(tp->t_left,tp->t_type,tp->t_su);
					break;

				default:
					outexpr(tp);
					break;
				}
			}
			break;

		case '(':
			while( (c=getc(&ibuf)) != '\n' )
				putchar(c);
			putchar(c);
			break;

		default:
			error("intermediate code error %c,%d",c,c);
			break;
		}
	}
}

/* readtree - recursive intermediate code tree read*/
char *readtree()						/* returns ptr to expression tree*/
{
	register int op, type, sc;
	register struct tnode *tp, *rtp;
	char sym[SSIZE];
	long l;

	if( (op=readint()) <= 0 )
		return(0);
	type = readint();
	switch( op ) {

	case SYMBOL:
		if( (sc=readint()) == EXTERNAL )
			tp = cenalloc(type,sc,readsym(sym));
		else
			tp = snalloc(type,sc,readint(),0,0);
		break;

	case CINT:
		tp = cnalloc(type,readint());
		break;

	case CLONG:
		l.hiword = readint();
		l.loword = readint();
		tp = lcnalloc(type,l);
		break;

	case CFLOAT:	/* [vlh] 3.4 */
		l.hiword = readint();
		l.loword = readint();
		tp = fpcnalloc(type,l);
		break;

	case IFGOTO:
	case BFIELD:
		sc = readint();
		if( tp = readtree() )
			tp = tnalloc(op,type,sc,0,tp,null);
		break;

	default:
		if( binop(op) ) {
			if( !(tp=readtree()) )
				return(0);
			if( !(rtp=readtree()) )
				return(0);
			tp = tnalloc(op,type,0,0,tp,rtp);
		}
		else if( tp = readtree() )
			tp = tnalloc(op,type,0,0,tp,null);
		break;
	}
	return(tp);
}

/* readint - reads an integer value from intermediate code*/
readint()
{
	register int i, c;

	i = 0;
	while(1) {
		switch( c = getc(&ibuf) ) {

		case '.':
		case '\n':
			return(i);

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			i =<< 4;
			i =+ (c-'0');
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			i =<< 4;
			i =+ (c-('a'-10));
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			i =<< 4;
			i =+ (c-('A'-10));
			break;

		default:
			error("intermediate code error - %c,%d",c,c);
		}
	}
}

/* readsym - read a symbol from intermediate code*/
char *readsym(sym)
char *sym;
{
	register int i, c;
	register char *s;

	for( i = SSIZE, s = sym; (c=getc(&ibuf)) != '\n'; )
		if( --i >= 0 )
			*s++ = c;
	if( i > 0 )
		*s = '\0';
	return(sym);
}

/* error - output an error message*/
error(s,x1,x2,x3,x4,x5,x6)
char *s;
int x1, x2, x3, x4, x5, x6;
{
	errcnt++;
	errflg++;
	if( lineno != 0 )
		printf("** %d: ",lineno);
	printf(s,x1,x2,x3,x4,x5,x6);
	putchar('\n');
	errflg--;
}

/* ferror - output error message and die*/
ferror(s,x1,x2,x3,x4,x5,x6)
char *s;
int x1, x2, x3, x4, x5, x6;
{
	error(s,x1,x2,x3,x4,x5,x6);
	exit(1);
}

/* tnalloc - allocate binary expression tree node*/
/*	returns ptr to node made.*/
char *tnalloc(op,type,info,dummy,left,right)
int op;							/* operator*/
int type;						/* resultant node type*/
int info;						/* info field*/
int dummy;						/* dummy field - used to match pass1 args*/
struct tnode *left;				/* left sub-tree*/
struct tnode *right;			/* righst sub-tree*/
{
	register struct tnode *tp;

	tp = talloc(sizeof(*tp));
	tp->t_op = op;
	tp->t_type = type;
	tp->t_su = info;			/* info for bit-field & condbr's*/
	tp->t_left = left;
	tp->t_right = right;
	return(tp);
}

/* cnalloc - allocate constant expression tree node*/
char *cnalloc(type,value)	/* returns pointer to node alloced*/
int type;						/* type of constant*/
int value;						/* value of constant*/
{
	register struct conode *cp;

	cp = talloc(sizeof(*cp));
	cp->t_op = CINT;
	cp->t_type = type;
	cp->t_value = value;
	return(cp);
}

/* lcnalloc - allocate constant expression tree node*/
char *lcnalloc(type,value)	/* returns pointer to node alloced*/
int type;						/* type of constant*/
long value;						/* value of constant*/
{
	register struct lconode *cp;

	cp = talloc(sizeof(*cp));
	cp->t_op = CLONG;
	cp->t_type = type;
	cp->t_lvalue = value;
	return(cp);
}

/* fpcnalloc - allocate constant expression tree node*/
char *fpcnalloc(type,value)	/* returns pointer to node alloced*/
int type;						/* type of constant*/
long value;						/* value of constant*/
{								/* [vlh] 3.4 */
	register struct lconode *cp;

	cp = talloc(sizeof(*cp));
	cp->t_op = CFLOAT;
	cp->t_type = type;
	cp->t_lvalue = value;
	return(cp);
}

/* talloc - allocate expression tree area*/
char *talloc(size)				/* returns pointer to area alloced*/
int size;						/* number of bytes to alloc*/
{
	register char *p;

	p = opap;
	if( p + size >= &exprarea[EXPSIZE] )
		ferror("expression too complex");
	opap = p + size;
	return(p);
}

/* symcopy - copy symbol*/
symcopy(sym1,sym2)					/* returns - none*/
char *sym1;						/* from symbol*/
char *sym2;						/* to symbol*/
{
	register char *p, *q;
	register int i;

	for( p = sym1, q = sym2, i = SSIZE; --i >= 0; )
		*q++ = (*p ? *p++ : '\0');
}

/* usage - ouput usage message*/
usage(argc,argv,n)
   char *argv[];
{
	register int i;
	error("usage call #%d:\n",n);
	for (i=0; i<argc; i++) error("   argv[%d] = %s\n",i,argv[i]);
	ferror("usage: c168 icode asm [-DLmec]");
}

/* putchar - special version*/
/*		This allows the use of printf for error messages, debugging*/
/*		output and normal output.*/
putchar(c)							/* returns - none*/
char c;								/* character to output*/
{
	if( errflg )					/*error message?*/
		write(1,&c,1);				/*write to standard output*/
	else {
		if( dflag > 1 )
			write(1,&c,1);			/*to standard output*/
		putc(c,&obuf);				/*put to assembler file*/
	}
}

v6flush(v6b)
struct iobuf *v6b;
{
	register i;

	i = BLEN - v6b->nunused;
	v6b->nunused = BLEN;
	v6b->xfree = &(v6b->buff[0]);
	if(write(v6b->fildes,v6b->xfree,i) != i)
		return(-1);
	return(0);
}
printf(string,a,b,c,d,e,f,g)
char	*string;
int	 a,b,c,d,e,f,g;
{
char	area[256];
register char *p;

	sprintf(area,string,a,b,c,d,e,f,g);
	for(p = &area[0]; *p; p++)
		putchar(*p);
}
