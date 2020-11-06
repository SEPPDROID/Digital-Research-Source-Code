/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

char *version "@(#) c068 parser 4.0 - Feb 11, 1983";
/*
	ALCYON C Compiler for the Motorola 68000 - Parser

	Called from c68:

		c068 source icode strings

	source:		input source code, preprocessed with comments stripped

	icode:		contains the intermediate code for the code generator,
				for a detailed explanaion see ../doc/icode.

	strings:	contains all the string constants.

	The basic structure of the parser is as follows:

	main							main driver for parser
		syminit						initializes symbol table
		doextdef					external definition syntax
			getatt					get type attributes
				dlist				declaration list for structures/unions
					getatt			recursive gettype call
					dodecl			do one declaration
						declarator	handle declarator syntax
			dodecl					do one external declaraion
			initlist				external initialization list
				cexpr				constant expressions
					expr			arithmetic expressions
						maketree	build operator tree
			funcbody				function body
				dlist				declaration list
				stmt				function statements
					stmt			recursive stmt call
					expr			arithmetic expressions

 */

#include "parser.h"

int nextlabel	1;
int lineno;
char *exprp		&exprarea[0];

/*
 * main - main routine for parser
 *		Checks arguments, opens input and output files, does main loop
 *		for external declarations and blocks.
 */
main(argc,argv)							/* returns - none*/
int argc;								/* argument count*/
char *argv[];							/* argument pointers*/
{
	register char *q;
	register int i;

	for( i = 4; i < argc; i++ ) {
		q = argv[i];
		if( *q++ != '-' )
			usage();
		while( 1 ) {
			switch( *q++ ) {

			case 'F':
			case 'f':
				fflag++;
				continue;

			case 'E':
			case 'e':
				eflag++;
				continue;

			case 'W':
			case 'w':
				wflag++;
				continue;

			case '\0':
				break;

			default:
				usage();
			}
			break;
		}
	}
	if( argc < 4 )
		usage();
	if( fopen(argv[1],&ibuf,0) < 0 )	/* 3rd arg for versados */
		ferror("can't open %s",argv[i]);
	if( fcreat(argv[2],&obuf,0) < 0 || fcreat(argv[3],&sbuf,0) < 0 )
		ferror("temp creation error");
	obp = &obuf;
	lineno++;
	frstp = -1;		/* [vlh] 3.4 - initialize only once */
	syminit();
	while( !peek(EOF) )
		doextdef();
	outeof();
	outdata();
	copysfile(argv[3]);
	exit(errcnt!=0);
}

/* usage - output usage error message and die*/
usage()
{
	ferror("usage: c068 source icode str [-e] [-f] [-w]");
}

/* error - report an error message*/
/*		outputs current line number and error message*/
error(s,x1,x2,x3,x4,x5,x6)			/* returns - none*/
char *s;							/* error message*/
int x1, x2, x3, x4, x5, x6;			/* args for printf*/
{
	register char *savep;

	savep = obp;
	obp = 0;
	errcnt++;
	if (!inclflag) {
		if( lineno )
			printf("* %d: ",lineno);
	}
	else	/*[vlh] generate filename and approp line #*/
		printf("%s: * %d: ",inclfile,inclline);
	printf(s,x1,x2,x3,x4,x5,x6);
	printf("\n");
	obp = savep;
}

/* ferror - fatal error*/
/*		Outputs error message and exits*/
ferror(s,x1,x2,x3,x4,x5,x6)			/* returns - none*/
char *s;							/* error message*/
int x1, x2, x3, x4, x5, x6;			/* args for printf*/
{
	error(s,x1,x2,x3,x4,x5,x6);
	exit(1);
}

v6flush(v6buf)
struct iobuf *v6buf;
{
	register i;

	i = BLEN - v6buf->nunused;
	v6buf->nunused = BLEN;
	v6buf->xfree = &(v6buf->buff[0]);
	if(write(v6buf->fildes,v6buf->xfree,i) != i)
		return(-1);
	return(0);
}
