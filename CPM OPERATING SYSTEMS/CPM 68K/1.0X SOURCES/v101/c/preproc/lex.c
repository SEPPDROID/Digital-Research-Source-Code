#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "preproc.h"
char null[] "";

char ctype[] {
	EOF,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	/*BUG 2*/
	ANYC,	WHITE,	NEWL,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	/*BUG 2*/
	ANYC,	ANYC,	ANYC,	ANYC,	NEWL,	ANYC,	ANYC,	ANYC,	/*BUG 2*/
	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	ANYC,	/*BUG 2*/
	WHITE,	NOT,	DQUOTE,	POUND,	ANYC,	MOD,	AND,	SQUOTE,	/*BUG 2*/
	LPAREN,	RPAREN,	MULT,	ADD,	COMMA,	SUB,	ANYC,	DIV,
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,
	DIGIT,	DIGIT,	COLON,	ANYC,	LESS,	EQUAL,	GREAT,	QMARK,
	ANYC,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	/*BUG 2*/
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ANYC,	BSLASH,	ANYC,	XOR,	ALPHA,
	ANYC,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	/*BUG 2*/
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ANYC,	OR,		ANYC,	COMPL,	ANYC	/*BUG 2*/
};

/* symhash - compute hash value for symbol*/
/*		Sums the symbols characters and takes that modulus the hash table*/
/*		size.*/
symhash(sym)						/* returns hash value for symbol*/
char *sym;							/* pointer to symbol*/
{
	register char *p;
	register int hashval, i;

	for( p = sym, i = SSIZE, hashval = 0; *p != '\0' && i > 0; i-- )
		hashval =+ *p++;
	return( hashval % HSIZE );
}

/* symequal - check for symbol equality*/
/*		Does comparison between two symbols.*/
symequal(sym1,sym2)					/* returns 1 if equal, 0 otherwise*/
char *sym1;							/* pointer to first symbol*/
char *sym2;							/* pointer to second symbol*/
{
	register char *p, *q;
	register int i;

	q = sym2;
	i = SSIZE;
	for( p = sym1; *p == *q++; )
		if( *p++ == '\0' || --i == 0 )
			return(1);
	return(0);
}

/* symcopy - symbol copy*/
/*		Copies one symbol to another.*/
symcopy(sym1,sym2)					/* returns - none*/
char *sym1;							/* pointer to symbol to copy*/
char *sym2;							/* pointer to area to copy to*/
{
	register char *p, *q;
	register int i;

	for( p = sym1, q = sym2, i = SSIZE; --i >= 0; )
		if( *p )
			*q++ = *p++;
		else
			*q++ = '\0';
}

/* error - output error message*/
/*		Outputs line number and error message and keeps track of errors.*/
error(s,x1,x2,x3,x4,x5,x6)				/* returns - none*/
char *s;								/* printf string*/
int x1, x2, x3, x4, x5, x6;				/* printf args*/
{
	printf("# %d: ",lineno);
	printf(s,x1,x2,x3,x4,x5,x6);
	putchar('\n');
	mfail++;
}

/* putback - puts back a single character*/
/*		Checks for push back buffer overflow.*/
putback(c)
int c;
{
	if( pbp >= &pbbuf[PBSIZE] ) {
		error("too many characters pushed back");
		cexit();
	}
	*pbp++ = c;
}

/* pbtok - push back a token*/
/*		Reverses token as its pushing it back.*/
pbtok(s)
char *s;
{
	register char *p;

	for( p = s + strlen(s); p > s ; )
		putback(*--p);
}

/* ngetch - get a (possibly) pushed back character*/
/*		This handles the include file stack and incrementing the line*/
/*		number for the lowest level file.*/
ngetch()								/* returns character or EOF*/
{
	register int c;

	if( pbp > &pbbuf[0] )
		return(*--pbp);
	pbflag = 0;
	while( (c=getc(&inbuf[filep])) <= 0 ) {
		if( filep <= 0 )
			return(EOF);
		close(inbuf[filep].fd);
		filep--;
		if( filep <= 0 ) {
			putc('\n',&outbuf);
			lineno++;
		}
	}
	return( c );
}

/* getsp - get symbol pointer*/
/*		Calculates the symbol table pointer for a given symbol, if symbol*/
/*		is not defined, will point to appropriate place to insert symbol.*/
struct symbol *getsp(name)
char *name;
{
	register int wrap;
	register struct symbol *sp, *asp;

	wrap = 0;
	asp = 0;
	for( sp = &symtab[symhash(name)]; sp->s_def ; ) {
		if( symequal(sp->s_name,name) )
			return(sp);
		if( asp == 0 && sp->s_def == null )
			asp = sp;
		if( ++sp >= &symtab[HSIZE] ) {
			if( wrap++ ) {
				error("symbol table overflow");
				cexit();
			}
			sp = &symtab[0];
		}
	}
	return( asp ? asp : sp );
}

/* lookup - looks up a symbol to see if it is defined*/
/*		Returns pointer to definition if found.*/
char *lookup(name)							/* returns 0 or ptr to symbol*/
char *name;									/* symbol name*/
{
	register struct symbol *sp;

	sp = getsp(name);
	if( sp->s_def == 0 || sp->s_def == null )
		return(0);
	return(sp);
}

/* gettok - gets next token from input*/
/*		Collects character string in token and handles special tokens for*/
/*		the expression evaluator.*/
gettok(token)							/* returns token type*/
char *token;
{
	register char *p, c;
	register int type, count, t, l;

	p = token;
	c = ngetch();
	*p++ = c;
	switch( type = ctype[c] ) {

	case SQUOTE:
	case DQUOTE:
		getstr(token,TOKSIZE,c);
		return(type);

	case DIGIT:
	case ALPHA:
		for( ; p < &token[TOKSIZE]; p++ ) {
			*p = ngetch();
			if( (t=ctype[*p]) != ALPHA && t != DIGIT )
				break;
		}
		putback(*p);
		break;

	case NOT:
		if( peekis('=') ) {
			type = NEQUAL;
			*p++ = '=';
		}
		break;

	case GREAT:
		if( peekis('>') ) {
			type = RSHIFT;
			*p++ = '>';
		}
		else if( peekis('=') ) {
			type = GREQUAL;
			*p++ = '=';
		}
		break;

	case LESS:
		if( peekis('<') ) {
			type = LSHIFT;
			*p++ = '<';
		}
		else if( peekis('=') ) {
			type = LSEQUAL;
			*p++ = '=';
		}
		break;

	case EQUAL:
		if( peekis('=') )
			*p++ = '=';
		else
			type = ANYC;
		break;

	case DIV:
		if( peekis('*') ) {
			l = lineno;
			while( (c=ngetch()) != EOF )
				if( c == '\n' ) {
					if( filep == 0 && pbp == pbbuf ) {
						lineno++;
						putc('\n',&outbuf);
					}
				}
				else if( c == '*' && peekis('/') )
					break;
			if( c == EOF ) {
				lineno = l;
				error("no */ before EOF");
			}
			type = WHITE;
			token[0] = ' ';
		}
		else if( peekis('/') ) {
			while( (c=ngetch()) != EOF && c != '\n' )
				;
			type = NEWL;
			token[0] = '\n';
		}
		break;

	case BADC:
		error("bad character 0%o",c);
		break;

	}
	*p = '\0';
	return(type);
}

/* getstr - get a quoted (single or double) character string*/
/*		Gets specified number of characters, handling escapes.*/
getstr(str,nchars,endc)				/* returns - none*/
char *str;							/* pointer to string buffer*/
int nchars;							/* max number of characters*/
char endc;							/* ending string character*/
{
	register char *p;
	register int i;
	register int c;
	register int j;

	p = str;
	*p++ = endc;
	for( i = nchars-2; (c=ngetch()) != endc; ) {
		if( c == EOF || c == '\n' ) {
			error("string cannot cross line");
			break;
		}
		if( --i > 0 )					/*BUG 1*/
			*p++ = c;
		else if( i == 0 )
			error("string too long");
		if( c == '\\' ) {
			c = ngetch();
			if( --i > 0 )				/*BUG 1*/
				*p++ = c;
			else if( i == 0 )
				error("string too long");
		}
	}
	*p++ = endc;
	*p = '\0';
}

/* peekis - peeks at next character for specific character*/
/*		Gets next (possibly pushed back) character, if it matches*/
/*		the given character 1 is returned, otherwise the character*/
/*		is put back.*/
peekis(tc)							/* returns 1 if match, 0 otherwise*/
int tc;								/* test character*/
{
	register int c;

	if( (c=ngetch()) == tc )
		return(1);
	putback(c);
	return(0);
}
