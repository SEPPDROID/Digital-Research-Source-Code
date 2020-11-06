/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

char *version = "@(#)main.c	1.6	12/28/83";

#include "preproc.h"

#define NARGS   64
#define FLAGS "[-C] [-P] [-E] [-D] [-I] [-6] [-7] [-3]"
#define USAGE "usage: %s %s source [dest]\n"

#ifdef MC68000
	char *v6incl    = "/usr/include/v6";
	char *v7incl    = "/usr/include/v7";
	char *s3incl    = "/usr/include/sys3";
	char *ucbincl   = "/usr/include/ucb";
	char *s5incl    = "/usr/include/sys5";
#endif

#ifdef UNIX
	char *v6incl    = "/usr/include/c68/v6";
	char *v7incl    = "/usr/include/c68/v7";
	char *s3incl    = "/usr/include/c68/sys3";
	char *ucbincl   = "/usr/include/c68/ucb";
	char *s5incl    = "/usr/include/c68/sys5";
#endif

#ifdef VMS
#	ifdef DECC
		char *v6incl    = "v6";
		char *v7incl    = "v7";
		char *s3incl    = "sys3";
		char *ucbincl   = "ucb";
		char *s5incl    = "sys5";
#	else
		char *v6incl    = "lib:";
		char *v7incl    = "lib:";
		char *s3incl    = "lib:";
		char *ucbincl   = "lib:";
		char *s5incl    = "lib:";
#	endif
#endif

#ifdef CPM
	char *v6incl    = "";
	char *v7incl    = "";
	char *s3incl    = "";
	char *ucbincl   = "";
	char *s5incl    = "";
#endif

char *incl[NINCL];
int ndefs, nincl;
int status = 0;

extern int errno;

/*
 * main - main routine for c68 Compiler system
 *      Handles the C68 arguments.  For each C file given, the macro
 *      pre-processor is called, then the parser, code generator and
 *      assember are fexec'd.  The loader arguments are collected and
 *      the loader is fexec'd.
 */
main(argc,argv)
int argc;
register char **argv;
{
    register char *arg, *calledby;
    register int c, i, j, x;

    calledby = *argv++; 
	if (argc < 2) {	/* cpp source */
		printf(USAGE,calledby,FLAGS);
		exit(-1);
	}
	
    for( ; --argc > 0 && **argv == '-'; ) {       /*process arguments*/
		*(arg = *argv++);
        arg++;
        for( i = 0; c = *arg++; ) {	
			
			switch( c ) {

				case 'D':
					defs[ndefs].ptr = arg;
					if ((x=index(arg,'=')) != -1) {
						defs[ndefs++].value = (arg+x+1);
						arg[x] = 0; /*get rid of value*/
					}
					else 
						defs[ndefs++].value = 0;
					i++;
					break;

				case 'I':
					incl[nincl++] = arg;
					i++;
					break;

				case 'C':	/* [vlh] 4.2 Leave comments in... */
					Cflag++;
				case 'E':   /* [vlh] 4.0 Preprocessor to stdout */
					Eflag++;
					continue;

				case 'P':	/* preprocessor pass only */
					pflag++;
					continue;

				case '6':       /* [vlh] 3.4 v6 compatibility */
					incl[nincl++] = v6incl;
					continue;

				case '7':       /* [vlh] 3.4 v7 compatibility */
					incl[nincl++] = v7incl;
					continue;

				case '3':       /* [vlh] 3.4 s3 compatibility */
					incl[nincl++] = s3incl;
					continue;

				case '4':       /* [vlh] 4.3 ucb compatibility */
					incl[nincl++] = ucbincl;
					continue;

				case '5':       /* [vlh] 3.4 s5 compatiblity */
					incl[nincl++] = s5incl;
					continue;

				default:
					printf(USAGE,calledby,FLAGS);
					exit(-1);

			}	/* end of case statement */
			if (i)
				break;
		}	/* end of for statement */
	}	/* end of for statement */

	if (argc > 2) {	/* source [dest] */
		printf(USAGE,calledby,FLAGS); 
		exit(-1); 
	}
	source = *argv++; 
	if ( !Eflag ) 
		if (argc==2)	/* destination file specified */
			strcpy(dest,*argv);
		else
			make_intermediate(); 

	asflag = (source[strlen(source)-1] == 's');
	domacro(ndefs);
    cexit();
}

/* cexit - exit from C compiler driver*/
/*      This deletes any existing temps and exits with the error status.*/
cexit()                                 /* returns - none*/
{
    exit(status);
}

/**
 * itoa - integer to ASCII conversion
 *      Converts integer to ASCII string, handles '-'.
**/
itoa(n,s,w)                             /* returns - none*/
int n;                                  /* number to convert*/
char *s;                                /* resulting string*/
int w;                                  /* minimum width of string*/
{
    register char *tp;
    register int sign, i;
    char temp[6];

    if( (sign=n) < 0 )
        n = -n;
    i = 0;
    tp = &temp[0];
    do {
        i++;
        *tp++ = n % 10 + '0';
    } while( (n /= 10) > 0 );
    if( sign < 0 ) {
        i++;
        *tp++ = '-';
    }
    while( --w >= i )                   /*pad on left with blanks*/
        *s++ = ' ';
    while( --i >= 0 )                   /*move chars reversed*/
        *s++ = *--tp;
    *s = '\0';
}

/* index - find the index of a character in a string*/
/*      This is identical to Software Tools index.*/
index(str,chr)                      /* returns index of c in str or -1*/
char *str;                          /* pointer to string to search*/
char chr;                           /* character to search for*/
{
    register char *s;
    register int i;

    for( s = str, i = 0; *s != '\0'; i++ )
    if( *s++ == chr )
        return(i);
    return(-1);
}

atoi(as)
char *as;
{
	register int n, sign;
	register char *s;

	s = as;
	while( *s==' ' || *s=='\n' || *s == '\t')
		s++;
	sign = 1;
	if( *s == '+' || *s == '-' )
		sign = (*s++=='+') ? 1 : -1;
	for( n = 0; *s >= '0' && *s <= '9'; s++ )
		n = (n * 10) + (*s - '0');
	return( sign * n );
}

make_intermediate()                     /* returns pointer to string*/
{										/* if source t.c dest <= t.i */
    register char *d, *s;
    register int ndx;

    s = source;
	while ((ndx = index(s,'/')) != -1) 
        s += ndx+1;  /* [vlh] */
    for( d = dest; *d++ = *s++; ) ;
	*(d-2) = 'i';		/* overwrite termination character */
}

/* cputc - put a character to a file descriptor (used by error) */
cputc(c, fn)
char c;
int fn;
{
#ifdef VERSADOS
	versaputchar(c);
#else
	if (fn == STDERR)
		write(STDERR, &c, 1);
	else
		putchar(c);
#endif
}

#ifdef VERSADOS
extern struct iob *fout;
putchar(c)
{
	versaputchar(c);
}
#endif

v6flush(v6buf)
struct iob *v6buf;
{
    register i;

    i = BSIZE - v6buf->cc;
    v6buf->cc = BSIZE;
    v6buf->cp = &(v6buf->cbuf[0]);
    if(write(v6buf->fd,v6buf->cp,i) != i)
		return(-1);
    return(0);
}

#ifdef VERSADOS

#define BSIZE 512

struct iob versfout = { 1, BSIZE, &versfout.cbuf[0]};

versaputchar(c)
char c;
{
    if (c == '\n') {    /* end of line */
        if (versaflush())   /* write one line */
            return(-1);
        return(c);
    }

    /* buffered output */
    if (versfout.cc <= 0) {
        versfout.cp = &(versfout.cbuf[0]);
        if (write(versfout.fd,versfout.cp,BSIZE) != BSIZE)
            return(-1);
        versfout.cc = BSIZE;
    }
    *(versfout.cp)++ = c;
    versfout.cc--;
    return(c);
}

versaflush()
{
    register short size, fildes;

    if ((size = (BSIZE - versfout.cc)) == 0)
        return(0);
    versfout.cc = BSIZE;
    versfout.cp = &(versfout.cbuf[0]);
    fildes = (versfout.fd <= STDERR) ? 6 : versfout.fd;
    if (write(fildes,versfout.cp,size) < 0)
        return(-1);
    return(0);
}
#endif
