/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"

int status;
extern char null[];

char ctype[] = {
    EOF,    ANYC,   ANYC,   ANYC,   ANYC,   ANYC,   ANYC,   ANYC, 
    ANYC,   WHITE,  NEWL,   ANYC,   ANYC,   ANYC,   ANYC,   ANYC, 
    ANYC,   ANYC,   ANYC,   ANYC,   NEWL,   ANYC,   ANYC,   ANYC, 
    ANYC,   ANYC,   ANYC,   ANYC,   ANYC,   ANYC,   ANYC,   ANYC, 
    WHITE,  NOT,    DQUOTE, POUND,  ANYC,   MOD,    AND,    SQUOTE, 
    LPAREN, RPAREN, MULT,   ADD,    COMMA,  SUB,    ANYC,   DIV,
    DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,  DIGIT,
    DIGIT,  DIGIT,  COLON,  ANYC,   LESS,   EQUAL,  GREAT,  QMARK,
    ANYC,   ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  
    ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,
    ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,
    ALPHA,  ALPHA,  ALPHA,  ANYC,   BSLASH, ANYC,   XOR,    ALPHA,
    ANYC,   ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  
    ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,
    ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,  ALPHA,
    ALPHA,  ALPHA,  ALPHA,  ANYC,   OR,     ANYC,   COMPL,  ANYC    
};  /* BUG2 - ANYC */

/* symhash - compute hash value for symbol*/
/*      Sums the symbols characters and takes that modulus the hash table*/
/*      size.*/
symhash(sym)                        /* returns hash value for symbol*/
char *sym;                          /* pointer to symbol*/
{
    register char *p;
    register int hashval, i;

    for( p = sym, i = SSIZE, hashval = 0; *p != '\0' && i > 0; i-- )
        hashval += *p++;
    return( hashval % (HSIZE - 2 ));	/* [vlh] 4.1 added -2 */
}

/* symequal - check for symbol equality*/
/*      Does comparison between two symbols.*/
symequal(sym1,sym2)                 /* returns 1 if equal, 0 otherwise*/
char *sym1;                         /* pointer to first symbol*/
char *sym2;                         /* pointer to second symbol*/
{
    register char *p, *q;
    register int i;

    q = sym2;
    i = SSIZE;
    for( p = sym1; *p == *q++; )
        if( *p++ == '\0' || --i == 0 ) {
            return(1);
		}
    return(0);
}

/* symcopy - symbol copy*/
/*      Copies one symbol to another.*/
symcopy(sym1,sym2)                  /* returns - none*/
char *sym1;                         /* pointer to symbol to copy*/
char *sym2;                         /* pointer to area to copy to*/
{
    register char *p, *q;
    register int i;

    for( p = sym1, q = sym2, i = SSIZE; --i >= 0; )
        if( *p )
            *q++ = *p++;
        else
            *q++ = '\0';
		*q = '\0';	/* [vlh] 4.1, force null terminator */
}

/* error - output error message*/
/*      Outputs line number and error message and keeps track of errors.*/
error(s,x1,x2,x3,x4,x5,x6)              /* returns - none*/
char *s;                                /* printf string*/
int x1, x2, x3, x4, x5, x6;             /* printf args*/
{
#ifndef ALTER_PR
    if (literal)
        printf((char *)STDERR,"%s, # line %d: ",lit_file, lit_num);
	else if (filep == &filestack[0]) /* [vlh] 3.4 not in include */
        printf((char *)STDERR,"%s, # line %d: ",source, lineno);
    else
        printf((char *)STDERR,"%s, # line %d: ",(filep)->ifile,
			   (filep)->lineno);
    printf((char *)STDERR,s,x1,x2,x3,x4,x5,x6);
	cputc('\n',STDERR);
#else
    if (literal)
        printf("%s, # line %d: ",lit_file, lit_num);
	else if (filep == &filestack[0]) /* [vlh] 3.4 not in include */
        printf("%s, # line %d: ",source, lineno);
    else
        printf("%s, # line %d: ",(filep)->ifile,(filep)->lineno);
    printf(s,x1,x2,x3,x4,x5,x6);
	printf("\n");
#endif
    status++;
}

/* putback - puts back a single character*/
/*      Checks for push back buffer overflow.*/
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
/*      Reverses token as its pushing it back.*/
pbtok(s)
char *s;
{
    register char *p;

    for( p = s + strlen(s); p > s ; )
        putback(*--p);
}

/* ngetch - get a (possibly) pushed back character*/
/*      This handles the include file stack and incrementing the line*/
/*      number for the lowest level file.*/
ngetch()                                /* returns character or EOF*/
{
    register int c, i;
	register char *p, *q;

    if( pbp > &pbbuf[0] )
        return(*--pbp);
    pbflag = 0;
    while( (c=getc(&(filep->inbuf))) < 0 ) {
        if( filep == &filestack[0] )
            return(EOF);
        close(filep->inbuf.fd);
        filep--;
#ifdef NONEST
        inbuf.cc = filep->tcc;
		inbuf.cp = filep->tcp;
        p = &inbuf.cbuf[0];
        q = &filep->tbuf[0];
        for(i = 0; i<BSIZE; i++)
            *p++ = *q++;
#else
/*sw This code no longer necessary ...
		inbuf.cc = 0;
        inbuf.cp = &inbuf.cbuf[0];
*/
#endif
/*sw    filep->inbuf.fd = filep->ifd; */
        if( filep == &filestack[0] ) {  /*need line for #include...*/
            lineno++;
			putid(source,lineno);	/* [vlh] 4.2 id line .... */
        }
		else {
			(filep)->lineno++;
			putid((filep)->ifile,(filep)->lineno);
		}
    }
    return( c );
}

/*
 * getsp - get symbol pointer
 *      Calculates the symbol table pointer for a given symbol, if symbol
 *      is not defined, will point to appropriate place to insert symbol.
 */
struct symbol *getsp(name)
char *name;
{
	register int wrap;
	register struct symbol *sp, *asp;

	wrap = 0;
	asp = 0;
	for( sp = &symtab[symhash(name)]; sp->s_def != null; ) {
		if( symequal(sp->s_name,name) )
			return(sp);
		if( !asp && sp->s_def == null )
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
char *name;								        /* symbol name*/
{
	register struct symbol *sp;

	sp = getsp(name);
	if( sp->s_def == 0 || sp->s_def == null )
		return(0);
	return((char *)sp);
}

/**
 * gettok - gets next token from input
 *      Collects character string in token and handles special tokens for
 *      the expression evaluator.
**/
gettok(token)                           /* returns token type*/
char *token;
{
    register char *p, c, *s;
    register int type, t, l;

    p = token;
    c = ngetch();
    *p++ = c;
    switch( type = ctype[c] ) {

    case SQUOTE:
    case DQUOTE:
        if(getstr(token,TOKSIZE,c))
			return(type);
		else {	/*[vlh]4.3, ignore incomplete strings... could be asm comment*/
			token[0] = '\n';
			return(NEWL);
		}

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
            if (Cflag) {
				putl((int)'/');
				putl((int)'*');
			}
			l = 0;	/* [vlh] 4.3, change in line counting technique */
            while( (c=ngetch()) != EOF )
                if( c == '\n' ) {
                    l++;	/* [vlh] 4.3, keep line counter */
					if (Cflag) {	/* [vlh] 4.2 */
						putl((int)'\0');
						s = line;
						while (*s)
							doputc(*s++,&outbuf);
						initl();
					}
					doputc(' ',&outbuf);
					doputc('\n',&outbuf);
                }
                else if( c == '*' && peekis('/') ) {
					if (Cflag) {	/* [vlh] 4.2 */
						putl((int)'*');
						putl((int)'/');
					}
                    break;
				}
				else if (Cflag)		/* [vlh] 4.2.c */
					putl((int)c);
            if( c == EOF )
                error("no */ before EOF");
			if (filep == &filestack[0]) /* [vlh] 4.3 */
				lineno += l;
			else						/* [vlh] 4.3, update include lineno */
				(filep)->lineno += l;
            type = WHITE;
            token[0] = ' ';
        }
        else if( peekis('/') ) {
			if (Cflag) {
				putl((int)'/');
				putl((int)'/');
			}
            while( (c=ngetch()) != EOF && c != '\n' )
                if (Cflag)
					putl(c);
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
/*      Gets specified number of characters, handling escapes.*/
getstr(str,nchars,endc)     /* returns - none*/
char *str;          /* pointer to string buffer*/
int nchars;         /* max number of characters*/
char endc;          /* ending string character*/
{
    register char *p;
    register int i, c;

    p = str;
    *p++ = endc;
    for( i = nchars-2; (c=ngetch()) != endc; ) {
        if( c == EOF || c == '\n' ) {	/* [vlh] 4.3, ignore non ended string */
			*p = '\0';
			p = str;
			while(*p)
				putl((int)*p++);
            return(0);
        }	/* [vlh] 4.3, may be assembly language comment !!!! */
        if( --i > 0 )                   /*BUG 1*/
            *p++ = c;
        else if( !i )
            error("string too long");
        if( c == '\\' ) {
            c = ngetch();
            if( --i > 0 )               /*BUG 1*/
                *p++ = c;
            else if( !i )
                error("string too long");
        }
    }
    *p++ = endc;
    *p = '\0';
	return(1);
}

/* peekis - peeks at next character for specific character*/
/*      Gets next (possibly pushed back) character, if it matches*/
/*      the given character 1 is returned, otherwise the character*/
/*      is put back.*/
peekis(tc)                          /* returns 1 if match, 0 otherwise*/
int tc;                             /* test character*/
{
    register int c;

    if( (c=ngetch()) == tc )
        return(1);
    putback(c);
    return(0);
}

doputc(ch,buffer)
char ch;
struct iob *buffer;
{
    if (!Eflag)
        putc(ch,buffer);
    else
        putchar(ch);
}

#ifdef DECC				/*sw You should really do this as a library...*/
getc(ibuf)
struct iob *ibuf;
{
    if (ibuf->cc <= 0) {
        ibuf->cp = &(ibuf->cbuf[0]);
        ibuf->cc = read(ibuf->fd,ibuf->cp,BSIZE);
    }
    if (ibuf->cc <= 0)
        return(-1);
    ibuf->cc--;
    return((int)(*(ibuf->cp)++)&0xff);
}

fopen(fname,ibuf)
char *fname;
register struct iob *ibuf;
{
    ibuf->cc = 0;   /* no chars */
    ibuf->fd = open(fname,0);
    return(ibuf->fd);
}
#endif

