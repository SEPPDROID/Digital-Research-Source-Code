/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"
#include <ctype.h>

#ifndef VAX11
    char *stdincl = "/usr/include";      /*standard include directory*/
#else
    /* 11 apr 83, for vax */
    char *stdincl = "/usr/include/c68";  /*standard include directory*/
#endif

int clabel = LABSTART;
int nlabel = LABSTART+1;
char null[] = "";
int nincl;
char *incl[10];
char tmp[NUMLEN];   /* temporary spot for line number itoa conversion */
char ctype[];
char lit_file[MAXPSIZE];

struct builtin {
    char *b_name;
    int b_type;
} btab[] = {
    "define",   DEFINE,
    "include",  INCLUDE,
    "undef",    UNDEF,
    "ifdef",    IFDEF,
    "ifndef",   IFNDEF,
    "else",     ELSE,
    "endif",    ENDIF,
    "if",       IF,
    "line",     LINE,
    0,
};

char *getinclude();
char cstack[CSTKSIZE];
char *cstkptr, inclname[TOKSIZE];

/*
 * domacro - do macro processing
 *      Does the macro pre-processing on the input file and leaves the
 *      result on the output file.
 */
domacro(nd)
int nd;                         /* number of defines*/
{
    register char *l;
    register struct symbol *sp;
    register int x;
    
	filep = &filestack[0];          /* [vlh] 4.0 moved for error msgs */
    lineno = 1;                     /* [vlh] 4.0 moved for error msgs */
    if( fopen(source,&inbuf,0) < 0 ) {      /* 3rd arg for versados */
        error("can't open source file %s\n",source);
        return(0);
    }
    if (!Eflag)
		if( fcreat(dest,&outbuf,0) < 0 ) {   /* 3rd arg for versados */
			error("can't creat %s\n",dest);
			return(0);
		}
	putid(source,1);		/* identify as first line in source file */

	for (sp= &symtab[0]; sp<= &symtab[HSIZE-1]; sp++) /*3.4*/
		sp->s_def = null;	/* clear out symbol table */
	defp = defap = sbrk(DEFSIZE);
	if (defp == (char *)-1) {
		error("define table overflow");
		cexit();
	}
	defmax = defcount = DEFSIZE;

    defused = 0;
    pbp = &pbbuf[0];
    cstkptr = &cstack[0];
    install("Newlabel",NEWLABEL);
    install("Label",LABEL);
    while( --nd >= 0 )
        dinstall(defs[nd].ptr,defs[nd].value);
    while( getline(source) ) {
        l = line;
		if( filep == &filestack[0] && pbp == &pbbuf[0] )
            lineno++;
        else if (filep != &filestack[0]) /*[vlh] include file*/
			(filep-1)->lineno++;
        while( *l )
            doputc(*l++,&outbuf);
		doputc(' ',&outbuf);
		doputc('\n',&outbuf);
    	if (literal) {
			if (filep != &filestack[0]) {
				(filep-1)->lineno++;
				putid((filep-1)->ifile,(filep-1)->lineno);
			}
			else {
				lineno++;
				putid(source,lineno);
			}
			literal = 0;
		}
    }
    if( cstkptr != &cstack[0] )
        error("unmatched conditional");
    if( defused > defmax )
        defmax = defused;
    v6flush(&outbuf);
    if (!Eflag)
		close(outbuf.fd);
    close(inbuf.fd);
	return(1);
}

putid(fname,lnum)   /* [vlh] 4.0 SOH line header */
char *fname;
int lnum;
{
    register char *p;

    if (asflag || pflag)
        return;

    if (literal) {
		strcpy(lit_file,fname);
		lit_num = lnum;
	}
	
	doputc('#',&outbuf);
	doputc(' ',&outbuf);
    itoa(lnum,tmp,NUMLEN-1);
    for (p = tmp; *p==' '; )
		p++;
    for ( ; *p; p++)
        doputc(*p,&outbuf);
	doputc(' ',&outbuf);
	doputc('"',&outbuf);
    for (p = fname; *p; p++)
        doputc(*p,&outbuf);
	doputc('"',&outbuf);
	doputc('\n',&outbuf);
}

install(name,def)
char *name;
int def;
{
    register struct symbol *sp;

    sp = getsp(name);
    symcopy(name,sp->s_name);
    sp->s_def = defp;
    putd(def);
    putd('\0');
}

dinstall(name,def)                  /* returns - none*/
char *name;                         /* macro name*/
char *def;                          /* pointer to definition*/
{
    register struct symbol *sp;

    sp = getsp(name);
    symcopy(name,sp->s_name);
    sp->s_def = defp;
    putd(NOARGS);
    if (def)            /* [vlh] character strings... */
        while(*def) 
            putd(*def++);
    else 
        putd('1');      /* [vlh] default define value */
    putd('\0');
}

/* kwlook - look up the macro built-in names*/
/*      Searches thru the built-in table for the name.*/
kwlook(name)                        /* returns keyword index or 0*/
char *name;                         /* keyword name to lookup*/
{
    register struct builtin *bp;

    for( bp = &btab[0]; bp->b_name; bp++ )
        if( strcmp(bp->b_name,name) == 0 )
            return(bp->b_type);
    return(0);
}

/*
 * getline - get input line handling macro statements
 *  Checks for a preprocessor statement on the line and if there
 *  is one there, it processes it.  Note that most of the work is
 *  in determining whether we need to skip the current line or not.
 *  This is all handled with the condition stack and the skip variable.
 *  The skip variable is non-zero if any condition on the condition
 *  stack is SKIP.
 */
getline(infile)                     /* returns 0 for EOF, 1 otherwise*/
char *infile;                       /* [vlh] for quoted include files */
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
            else
                if( i != NOSKIP )
                    error("invalid #endif");
            break;

        case ELSE:
            if( (i=pop()) == SKIP ) {
                skip--;
                push(NOSKIP);
            }
            else
                if( i == NOSKIP ) {
                    skip++;
                    push(SKIP);
                }
                else
                    error("invalid #else");
            break;

        case DEFINE:
            if( !skip )         /*if in skip, don't do define*/
                dodefine();
            break;

        case UNDEF:
            if( !skip ) {       /*if in skip, don't undef*/
                if( (type=getntok(token)) == ALPHA )
                    undefine(token);
            }
            break;

        case INCLUDE:
            if( !skip ) {         /*if in skip, don't do include*/
                doinclude(infile);
				if (filep != &filestack[0])
					i = getline((filep-1)->ifile);
				else
					i = getline(infile);
				return(i);
			}
            break;

        case IF:
            if( cexpr() )           /*evaluate constant expression*/
                push(NOSKIP);       /*non-zero, so don't skip*/
            else {
                push(SKIP);
                skip++;
            }
            break;

        case LINE:  /* [vlh] 4.0 */
			doline();
			return(getline(infile));
            break;

        default:
            error("invalid preprocessor command");
            break;
        }
        eatup();
    }
    else
        if( type != NEWL ) {
            if( skip )
                eatup();
            else {
                for( ; type!=NEWL && type!=EOF ; type=gettok(token) )
                    if(type == ALPHA && (p=lookup(token)))
                            expand((struct symbol *)p);
                    else if (!special(token,infile))    /* [vlh] 4.1 */
                        for (p=token; *p; )
                            putl((int)(*p++));
            }
		}
    putl((int)('\0'));
    return(1);
}

/* special - check for predefined macros, if they exist expand them */
/*  __FILE - current file name, __LINE - current line number */
special(token,infile)               /* [vlh] 4.1 */
char *token, *infile;
{
    register char *p;
	int xline;
    char buf[8];

    if (strcmp(token,"__FILE") == 0) {
        putl((int)('"'));
        for (p = infile; *p; )
            putl((int)(*p++));
        putl((int)('"'));
    }
    else
        if (strcmp(token,"__LINE") == 0) {
			xline = (literal) ? lit_num : (filep == &filestack[0]) ? lineno 
				: (filep-1)->lineno;
            itoa(xline,buf,7);
            buf[7] = 0;
            for (p = &buf[0]; *p == ' '; )
                p++;
            while (*p)
                putl((int)(*p++));
        }
        else
            return(0);
    return(1);
}

/* eatup - eat up the rest of the input line until a newline or EOF*/
/*      Does gettok calls.*/
eatup()                             /* returns - none*/
{
    register int type;
    char etoken[TOKSIZE];

    while( (type=gettok(etoken)) != NEWL && type != EOF )
        ;
}

/* putl - put a character to the current output line*/
/*      Checks for line overflow.*/
putl(c)                             /* returns - none*/
int c;                              /* character to put on line*/
{
    if( linep < &line[LINESIZE] )
        *linep++ = c;
    else if ( !loverflow ) {
        loverflow++;
        error("line overflow");
    }
}

/* initl - initialize current line*/
/*      Sets the line pointer and the line overflow flag.*/
initl()                             /* returns - none*/
{
    *(linep= &line[0]) = '\0';
    loverflow = 0;
}

/* putd - put a character to the define buffer*/
/*      Does dynamic allocation for define buffer*/
putd(c)             /* returns - none*/
int c;              /* character to put in buffer*/
{
    if( !defcount ) {
        if( sbrk(DEFSIZE) == (char *)-1 ) {
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
/*      Sets the symbols definition to the null pointer*/
undefine(name)                      /* returns - none*/
char *name;                         /* pointer to name to undef*/
{
    register struct symbol *sp;

    sp = getsp(name);
    if( sp->s_def )
        sp->s_def = null;
}

/* dodefine - do #define processing*/
/*      Checks the define name, collects formal arguements and saves*/
/*      macro definition, substituting for formal arguments as it goes.*/
dodefine()                          /* returns - none*/
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
    type = getntok(token);          /*get next non-white token*/
    for( ; type != NEWL && type != EOF; type = gettok(token) ) {
        if (type==ALPHA || type==SQUOTE || type==DQUOTE) { /* [vlh] 4.1 */
            trymatch(token,type,nargs,args);    /* [vlh] 4.1 */
            continue;
        }
        else if( type == BSLASH ) {
            if( (i=ngetch()) == '\n' ) {    /*multi-line macro?*/
                if( filep == &filestack[0] && pbp == &pbbuf[0] ) {
                    lineno++;
                    doputc('\n',&outbuf);
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

/* trymatch - check for arguments */
trymatch(token,type,nargs,args)     /* [vlh] 4.1 */
char *token;
int type, nargs;
char *args[];
{
    register char *p;
    register int i, len;

    p = token;
    if (type != ALPHA)
        putd(*p++);
    while (*p != 0) {
        for(i = 0; i < nargs; i++)
            if ((len = pattern(args[i],p)) != 0)
                break;
        if (i < nargs) {    /* sub ARG marker for formal arg */
            putd(i+1);
            putd(ARG);
            p += len;
        }
        else
            do {
                putd(*p++);
            } while (isalnum(*p) || *p == '_');
        while (!(isalnum(*p)) && *p != '_' && *p)   /* get to next possible */
            putd(*p++);
    }
}

/* pattern - if the pattern occurs in the token starting at the first */
/*  position in the string, pattern returns the length of the pattern */
/*  else pattern returns a zero. */
pattern(pat,token)                      /* [vlh] 4.1 */
char *pat, *token;
{
    register int len;

    len = strlen(pat);
    if (len > strlen(token))    /* couldn't possibly work */
        return(0);
    if (isalnum(token[len]) || token[len]=='_')
        return(0);          /* not deliminated by non-alphanumeric */

    for (len = 0; *pat; ) {
        if( *pat++ != *token++ )
            return(0);
        len++;
    }
    return( len );
}

/* expand - expands the macro definition*/
/*  Checks for define recursion and #define x x problems, collects*/
/*  the actual arguments using getaarg, and then expands the macro*/
/*  by pushing it onto the push back buffer, substituting arguments*/
/*  as it goes.*/
expand(sp)              /* returns - none*/
struct symbol *sp;          /* pointer to macro to expand*/
{
    char argbuf[ARGBSIZE], *args[MAXARGS], token[TOKSIZE];
    register char *p, *abp, *mdef;
    register int i, j, nargs, type;

    if( pbflag++ > 100 ) {
        error("define recursion");
        return;
    }
    if( strcmp(sp->s_name,mdef=sp->s_def) == 0 ) {  /*handle #define x x*/
        while( *mdef )
            putl((int)(*mdef++));
        return;
    }
    nargs = 0;
    if( *mdef == NOARGS )                   /*suppress grabbing of args*/
        ;
    else if( getntok(token) != LPAREN ) /* [vlh] 4.1 ignore white space */
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
        if( !nargs )
            nlabel++;
        else
            nlabel += atoi(args[0]);
    }
    else if( *mdef == LABEL ) {
        if( !nargs )
            i = clabel;
        else
            i = clabel + atoi(args[0]);
        pbnum(i);
        pbtok("_L");
    }
    else {
        mdef++;                     /*skip no. of args*/
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
/*      Skips blanks and handles "," and ")".*/
getfarg(token)                      /* returns token type*/
char *token;                        /* token returned*/
{
    register int type;

    if( (type=getntok(token)) == RPAREN || type == ALPHA )
        return(type);
    if( type != COMMA || (type=getntok(token)) != ALPHA )
        error("bad argument:%s",token);
    return(type);
}

/* getntok - get next token, suppressing white space*/
/*      Merely gettok's until non-white space is there*/
getntok(token)                      /* returns token type*/
char *token;                        /* token returned*/
{
    register int type;

    while( (type=gettok(token)) == WHITE )
        ;
    return(type);
}

/* getaarg - get macro actual argument*/
/*      This handles the collecting of the macro's call arguments.*/
/*      Note that you may have parenthesis as part of the macro argument,*/
/*      hence you need to keep track of them.*/
getaarg(argp)                       /* returns token type*/
char *argp;                         /* argument returned*/
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
/*      Checks for stack overflow.*/
push(val)                           /* returns - none*/
int val;                            /* value to push*/
{
    if( cstkptr >= &cstack[CSTKSIZE] ) {
        error("condition stack overflow");
        cexit();
    }
    *cstkptr++ = val;
}

/* pop - pop the #ifdef, etc. condition stack*/
/*      Checks for stack undeflow.*/
pop()                               /* returns - top of condition stack*/
{
    if( cstkptr <= &cstack[0] )
        return(-1);
    return( *--cstkptr );
}

/* doinclude - handle #include command*/
/*      Checks for file name or library file name and pushes file on*/
/*      include file stack.*/
doinclude(infile)                   /* returns - none*/
char *infile;                       /* [vlh] for quoted include files */
{
    register int type, fd, i;
    char token[TOKSIZE], fname[TOKSIZE];
    register char *p, *q, c, *ptr1, *ptr2;

    p = fname;
    if( (type=getntok(token)) == SQUOTE || type == DQUOTE ) {
        for( c = token[0], q = &token[1]; *q != c; )
            *p++ = *q++;
        *p = '\0';
        p = getinclude(fname,infile);
    }
    else if( type != LESS ) {
        error("bad include file");
        return;
    }
    else {
        while( (type=gettok(token))!=GREAT && type!=NEWL && type!=EOF )
            for( q = token; *p = *q++; p++ )
                ;
        if( type != GREAT ) {
            error("bad include file name");
            pbtok(token);
            return;
        }
        p = getinclude(fname,(char *)0L);
    }
    eatup();                        /*need here...*/
    if( filep >= &filestack[FSTACK] )
        error("includes nested too deeply");
    else {
        fd = inbuf.fd;
#ifdef NONEST
        filep->tcc = inbuf.cc;
        filep->tcp = inbuf.cp;
        ptr1 = &filep->tbuf[0];
        ptr2 = &inbuf.cbuf[0];
        for(i=0; i<BSIZE; i++)
            *ptr1++ = *ptr2++;
#else
        seek(fd,-inbuf.cc,1);       /*back up file ptr*/
#endif
        inbuf.cc = 0;
        if( fopen(p,&inbuf,0) < 0 ) {   /* 3rd arg for versados */
            if (type != SQUOTE && type != DQUOTE)
                error("can't open include file %s",p);
            else
                error("can't open include file %s",fname);
#ifdef NONEST
			inbuf.cc = filep->tcc;
#endif
        }
        else {
            filep->ifd = fd;
            filep->lineno = 1;      /* [vlh] */
			putid(p,1);				/* id for include file */
            doifile(p);
            filep++;
        }
    }
}

doifile(p)  /* [vlh] */
char *p;
{
    register char *iptr;
    register int ndx;

    for( iptr = filep->ifile; *p; ) 
		*iptr++ = *p++;
    *iptr = 0;
}

/* getinclude - get include file full pathname */
char *
getinclude(fname,parent)    /* [vlh] */
char *fname;
char *parent;               /* search parent-file home directory ? */
{
    register char *q, *t;
    register int i, fd, ndx;

    for (i=0; i<nincl; i++) {
        for(t=inclname, q=incl[i]; *t++ = *q++; )
			;
		*(t-1) = FILESEP;
        for(q=fname; *t++ = *q++; ) ;
        *t = 0;
        if ((fd = open(inclname,0)) >= 0) {
            close(fd);
            return(&inclname[0]);
        }
    }
    if (parent) {   /* include filename surrounded by quotes */
        q = (filep == &filestack[0]) ? parent : (filep-1)->ifile;
        t = &inclname[0];
        while ((ndx = index(q,FILESEP)) >= 0) {
            ndx++;
            while (ndx--) *t++ = *q++;
        }
        for (q=fname; *t++ = *q++; )
			;
        *t = 0;
        if ((fd = open(inclname,0)) >= 0) { /* found it */
            close(fd);
            return(&inclname[0]);
        }
    }
    for(t=inclname, q=stdincl; *t++ = *q++; )
		;
	*(t-1) = FILESEP;
    for(q=fname; *t++ = *q++; ) ;
    *t = 0;
    return(&inclname[0]);
}

#define SKIPWHITE(ch)     do { ch = ngetch(); } while (ctype[ch] == WHITE)
doline()                      /* [vlh] 4.0 : returns - none */
{
    register char *ptr;
    char token[TOKSIZE];
    register int ch, lnum, type;

		/* get line number associated with #LINE */
    while ((type = gettok(token)) == WHITE);    /* skip white space */
    if (type != DIGIT) {
        error("invalid #line args");
        return;
    }
    lnum = atoi(token);
    literal = 1; 
	SKIPWHITE(ch);
    if (ctype[ch] != NEWL && ctype[ch] != EOF) {	/* associated filename */
        ptr = &token[0];
        do {
            *ptr++ = ch;
            ch = ngetch();
        } while (ctype[ch]!=NEWL && ctype[ch]!=EOF && ctype[ch]!=WHITE);
        *ptr = 0;
        putid(token,lnum);
    }
    else		/* source or header file */
        if (filep == &filestack[0])
            putid(source,lnum);				/* [vlh] 4.2.c */
		else
            putid((filep-1)->ifile,lnum);		/* [vlh] 4.2.c */
    if (ch != NEWL)
		for( ; ctype[ch]!=NEWL && ctype[ch]!=EOF; )
			ch = ngetch();
}

pbnum(num)                          /* returns - none*/
int num;
{
    register int digit;

    do {
        digit = num % 10;
        num /= 10;
        putback(digit+'0');
    } while( num > 0 );
}

