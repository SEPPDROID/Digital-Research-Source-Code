#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "preproc.h"
#define	CSTKSIZE	20

#ifdef	VAX
char *stdincl "lib:";		/*standard include directory*/
#endif

#ifdef UNIX
char *stdincl ="/usr/include/";
#endif

#ifdef MC68000
char *stdincl "";
#endif

int clabel =1000;
int nlabel =1001;
int lineno = 0;

struct builtin {
	char *b_name;
	int b_type;
} btab[] {
	"define",	DEFINE,
	"include",	INCLUDE,
	"undef",	UNDEF,
	"ifdef",	IFDEF,
	"ifndef",	IFNDEF,
	"else",		ELSE,
	"endif",	ENDIF,
	"if",		IF,
	0,
};

char cstack[CSTKSIZE]={0};
char *cstkptr=0;

/* domacro - do macro processing*/
/*		Does the macro pre-processing on the input file and leaves the*/
/*		result on the output file.*/
domacro(infile,outfile,nd,nds)		/* returns 1 if ok, 0 otherwise*/
char *infile;						/* input file name*/
char *outfile;						/* output file name*/
int nd;							/* number of defines*/
char *nds[];						/*points to defines*/
{
	register struct builtin *bp;
	register char *l;

	filep = 0;
	if( fopen(infile,&inbuf[filep],0) < 0 ) {
		error("can't open %s\n",infile);
		return(0);
	}
	if( fcreat(outfile,&outbuf,0666) < 0 ) {
		error("can't creat %s\n",outfile);
		return(0);
	}
	if( defap == 0 ) {
		defp = defap = sbrk(1024);
		defmax = defcount = 1024;
	}
	else {
		defcount = defmax;
		defp = defap;
	}
	lineno = 1;
	defused = 0;
	mfail = 0;
	pbp = &pbbuf[0];
	cstkptr = &cstack[0];
	install("Newlabel",NEWLABEL);
	install("Label",LABEL);
	while( --nd >= 0 )
		install(nds[nd],0);
	while( getline() ) {
		l = line;
		if( filep == 0 && pbp == pbbuf )
			lineno++;
		else if( pflag == 0 && asflag == 0 )
			putc(SOH,&outbuf);
		while( *l )
			putc(*l++,&outbuf);
		putc('\n',&outbuf);
	}
	if( cstkptr != &cstack[0] )
		error("unmatched conditional");
	if( defused > defmax )
		defmax = defused;
	myfflush(&outbuf);
	close(outbuf.fd);
	close(inbuf[0].fd);
	return(mfail==0);
}

/* kwlook - look up the macro built-in names*/
/*		Searches thru the built-in table for the name.*/
kwlook(name)						/* returns keyword index or 0*/
char *name;							/* keyword name to lookup*/
{
	register struct builtin *bp;

	for( bp = &btab[0]; bp->b_name; bp++ )
		if( strcmp(bp->b_name,name) == 0 )
			return(bp->b_type);
	return(0);
}

/* getline - get input line handling macro statements*/
/*		Checks for a preprocessor statement on the line and if there*/
/*		is one there, it processes it.  Note that most of the work is*/
/*		in determining whether we need to skip the current line or not.*/
/*		This is all handled with the condition stack and the skip variable.*/
/*		The skip variable is non-zero if any condition on the condition*/
/*		stack is SKIP.*/
getline()							/* returns 0 for EOF, 1 otherwise*/
{
	char token[TOKSIZE];
	register int type, i;
	register char *p;

	initl();
	if( (type=gettok(token)) == EOF )
		return(0);
	if( type == POUND ) {
		if( (type=getntok(token)) == NEWL )
			return(1);
		switch( kwlook(token) ) {

		case IFDEF:
			if( getntok(token) == ALPHA && lookup(token) )
				push(NOSKIP);
			else {
				push(SKIP);
				skip++;
			}
			break;

		case IFNDEF:
			if( getntok(token) == ALPHA && lookup(token) ) {
				push(SKIP);
				skip++;
			}
			else
				push(NOSKIP);
			break;

		case ENDIF:
			if( (i=pop()) == SKIP )
				skip--;
			else if( i != NOSKIP )
				error("invalid #endif");
			break;

		case ELSE:
			if( (i=pop()) == SKIP ) {
				skip--;
				push(NOSKIP);
			}
			else if( i == NOSKIP ) {
				skip++;
				push(SKIP);
			}
			else
				error("invalid #else");
			break;

		case DEFINE:
			if( skip == 0 )			/*if in skip, don't do define*/
				dodefine();
			break;

		case UNDEF:
			if( skip == 0 ) {		/*if in skip, don't undef*/
				if( (type=getntok(token)) == ALPHA )
					undefine(token);
			}
			break;

		case INCLUDE:
			if( skip == 0 )			/*if in skip, don't do include*/
				doinclude();
			break;

		case IF:
			if( cexpr() )			/*evaluate constant expression*/
				push(NOSKIP);		/*non-zero, so don't skip*/
			else {
				push(SKIP);
				skip++;
			}
			break;

		default:
			error("invalid preprocessor command");
			break;
		}
		eatup();
	}
	else if( type == NEWL )
		;
	else if( skip )
		eatup();
	else {
		for( ; type != NEWL && type != EOF ; type = gettok(token) ) {
			if( type == ALPHA && (p=lookup(token)) )
				expand(p);
			else {
				for( p = token; *p ; )
					putl(*p++);
			}
		}
	}
	putl('\0');
	return(1);
}

/* eatup - eat up the rest of the input line until a newline or EOF*/
/*		Does gettok calls.*/
eatup()								/* returns - none*/
{
	register int type;
	char token[TOKSIZE];

	while( (type=gettok(token)) != NEWL && type != EOF )
		;
}

/* putl - put a character to the current output line*/
/*		Checks for line overflow.*/
putl(c)								/* returns - none*/
int c;								/* character to put on line*/
{
	if( linep < &line[LINESIZE] )
		*linep++ = c;
	else if( loverflow == 0 ) {
		loverflow++;
		error("line overflow");
	}
}

/* initl - initialize current line*/
/*		Sets the line pointer and the line overflow flag.*/
initl()								/* returns - none*/
{
	*(linep= &line[0]) = '\0';
	loverflow = 0;
}

/* putd - put a character to the define buffer*/
/*		Does dynamic allocation for define buffer*/
putd(c)								/* returns - none*/
int c;								/* character to put in buffer*/
{
	if( defcount == 0 ) {
		if( sbrk(DEFSIZE) == -1 ) {
			error("define table overflow");
			cexit();
		}
		defcount = DEFSIZE;
	}
	defused++;
	defcount--;
	*defp++ = c;
}

/* undefine - does undef command*/
/*		Sets the symbols definition to the null pointer*/
undefine(name)						/* returns - none*/
char *name;							/* pointer to name to undef*/
{
	register struct symbol *sp;

	sp = getsp(name);
	if( sp->s_def )
		sp->s_def = null;
}

/* dodefine - do #define processing*/
/*		Checks the define name, collects formal arguements and saves*/
/*		macro definition, substituting for formal arguments as it goes.*/
dodefine()							/* returns - none*/
{
	char token[TOKSIZE], *args[MAXARGS], argbuf[ARGBSIZE];
	register char *abp, *p;
	register int type, nargs, i;
	register struct symbol *sp;

	if( (type=getntok(token)) != ALPHA ) {
		error("bad define name: %s",token);
		return;
	}
	sp = getsp(token);
	symcopy(token,sp->s_name);
	sp->s_def = defp;
	nargs = 0;
	abp = argbuf;
	if( (type=gettok(token)) == LPAREN ) {
		for( ; (type=getfarg(token)) != RPAREN; nargs++ ) {
			if( nargs >= MAXARGS ) {
				error("too many arguments");
				break;
			}
			args[nargs] = abp;
			for( p = token; *abp++ = *p++; ) {
				if( abp >= &argbuf[ARGBSIZE] ) {
					error("argument buffer overflow");
					break;
				}
			}
		}
		putd(nargs);
	}
	else {
		pbtok(token);
		putd(NOARGS);
	}
	type = getntok(token);			/*get next non-white token*/
	for( ; type != NEWL && type != EOF; type = gettok(token) ) {
		if( type == ALPHA ) {
			for( i = 0; i < nargs; i++ ) {
				if( strcmp(args[i],token) == 0 )
					break;
			}
			if( i < nargs ) {		/*sub ARG marker for formal arg*/
				putd(i+1);
				putd(ARG);
				continue;
			}
		}
		else if( type == BSLASH ) {
			if( (i=ngetch()) == '\n' ) {	/*multi-line macro?*/
				if( filep == 0 && pbp == pbbuf ) {
					lineno++;
					putc('\n',&outbuf);
				}
			}
			putd(i);
			continue;
		}
		for( p = token; *p ; )
			putd(*p++);
	}
	pbtok(token);
	putd('\0');
}

/* expand - expands the macro definition*/
/*		Checks for define recursion and #define x x problems, collects*/
/*		the actual arguments using getaarg, and then expands the macro*/
/*		by pushing it onto the push back buffer, substituting arguments*/
/*		as it goes.*/
expand(sp)							/* returns - none*/
struct symbol *sp;					/* pointer to macro to expand*/
{
	char argbuf[ARGBSIZE], *args[MAXARGS], token[TOKSIZE];
	register char *p, *abp, *mdef;
	register int i, j, nargs, type;

	if( pbflag++ > 100 ) {
		error("define recursion");
		return;
	}
	if( strcmp(sp->s_name,mdef=sp->s_def) == 0 ) {	/*handle #define x x*/
		while( *mdef )
			putl(*mdef++);
		return;
	}
	nargs = 0;
	if( *mdef == NOARGS )					/*suppress grabbing of args*/
		;
	else if( gettok(token) != LPAREN )
		pbtok(token);
	else {
		abp = &argbuf[0];
		while( (type=getaarg(token)) != EOF ) {
			if( nargs >= MAXARGS ) {
				error("too many arguments");
				return;
			}
			args[nargs++] = abp;
			for( p = token; *abp++ = *p++; ) {
				if( abp >= &argbuf[ARGBSIZE] ) {
					error("argument buffer overflow");
					return;
				}
			}
			if( type == RPAREN )
				break;
		}
	}
	if( *mdef == NEWLABEL ) {
		clabel = nlabel;
		if( nargs == 0 )
			nlabel++;
		else
			nlabel =+ atoi(args[0]);
	}
	else if( *mdef == LABEL ) {
		if( nargs == 0 )
			i = clabel;
		else
			i = clabel + atoi(args[0]);
		pbnum(i);
		pbtok("_L");
	}
	else {
		mdef++;						/*skip no. of args*/
		for( p = mdef + strlen(mdef) - 1; p >= mdef; p-- ) {
			if( *p == ARG ) {
				if( (j= *--p) <= nargs )
					pbtok(args[j-1]);
			}
			else
				putback(*p);
		}
	}
}

/* getfarg - get macro formal parameters*/
/*		Skips blanks and handles "," and ")".*/
getfarg(token)						/* returns token type*/
char *token;						/* token returned*/
{
	register int type;

	if( (type=getntok(token)) == RPAREN || type == ALPHA )
		return(type);
	if( type != COMMA || (type=getntok(token)) != ALPHA )
		error("bad argument:%s",token);
	return(type);
}

/* getntok - get next token, suppressing white space*/
/*		Merely gettok's until non-white space is there*/
getntok(token)						/* returns token type*/
char *token;						/* token returned*/
{
	register int type;

	while( (type=gettok(token)) == WHITE )
		;
	return(type);
}

/* getaarg - get macro actual argument*/
/*		This handles the collecting of the macro's call arguments.*/
/*		Note that you may have parenthesis as part of the macro argument,*/
/*		hence you need to keep track of them.*/
getaarg(argp)						/* returns token type*/
char *argp;							/* argument returned*/
{
	int type, plevel, i;
	register char *p, *ap;
	char token[TOKSIZE];
	
	ap = argp;
	*ap = '\0';
	plevel = 0;
	i = TOKSIZE;
	while( ((type=gettok(token)) != COMMA && type != RPAREN) || plevel ) {
		for( p = token; *ap = *p++; ap++ )
			if( --i <= 0 ) {
				error("macro argument too long");
				return(EOF);
			}
		if( type == LPAREN )
			plevel++;
		else if( type == RPAREN )
			plevel--;
		else if( type == EOF ) {
			error("unexpected EOF");
			cexit();
		}
	}
	if( ap == argp )
		type = EOF;
	return(type);
}

/* push - push a #ifdef condition value on condition stack*/
/*		Checks for stack overflow.*/
push(val)							/* returns - none*/
int val;							/* value to push*/
{
	if( cstkptr >= &cstack[CSTKSIZE] ) {
		error("condition stack overflow");
		cexit();
	}
	*cstkptr++ = val;
}

/* pop - pop the #ifdef, etc. condition stack*/
/*		Checks for stack undeflow.*/
pop()								/* returns - top of condition stack*/
{
	if( cstkptr <= &cstack[0] )
		return(-1);
	return( *--cstkptr );
}

/* doinclude - handle #include command*/
/*		Checks for file name or library file name and pushes file on*/
/*		include file stack.*/
doinclude()							/* returns - none*/
{
	register int type, fd;
	char token[TOKSIZE], fname[TOKSIZE];
	register char *p, *q, c;

	p = fname;
	if( (type=getntok(token)) == SQUOTE || type == DQUOTE ) {
		for( c = token[0], q = &token[1]; *q != c; )
			*p++ = *q++;
		*p = '\0';
	}
	else if( type != LESS ) {
		error("bad include file");
		return;
	}
	else {
		for( q = stdincl; *p = *q++; p++ )
			;
		while( (type=gettok(token))!=GREAT && type!=NEWL && type!=EOF )
			for( q = token; *p = *q++; p++ )
				;
		if( type != GREAT ) {
			error("bad include file name");
			pbtok(token);
			return;
		}
	}
	eatup();						/*need here...*/
	if( filep >= NINCS-1 )
		error("includes nested too deeply");
	else {
		filep++;
		if( fopen(fname,&inbuf[filep],0) < 0 ) {
			error("can't open %s",fname);
			filep--;
		}
	}
	putback('\n');					/*for eatup in domacro*/
}

install(name,def)					/* returns - none*/
char *name;							/* macro name*/
char def;							/* pointer to definition*/
{
	register struct symbol *sp;

	sp = getsp(name);
	symcopy(name,sp->s_name);
	sp->s_def = defp;
	putd(def);
	putd('\0');
}

pbnum(num)							/* returns - none*/
int num;
{
	register int digit;

	do {
		digit = num % 10;
		num =/ 10;
		putback(digit+'0');
	} while( num > 0 );
}
