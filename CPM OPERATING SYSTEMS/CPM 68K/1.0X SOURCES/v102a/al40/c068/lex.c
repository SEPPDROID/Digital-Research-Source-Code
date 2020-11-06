/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
#define	SOI	'\01'
#define	STEL	HSIZE/2

	/*
	 * the following are the cases within gettok, all other cases are
	 * single character unambiguous tokens.  Note that we need to take
	 * special care not to interfere with the single character unambiguous
	 * operators, this is why there is a gap between WHITSP and EXCLAM.
	 */
#define	BADC	0					/*bad character*/
#define	WHITSP	101					/*white space*/
#define	EXCLAM	102					/*exlamation point*/
#define	DQUOTE	103					/*double quote*/
#define	PERCNT	104					/*percent sign*/
#define	AMPER	105					/*ampersand*/
#define	SQUOTE	106					/*single quote*/
#define	STAR	107					/*asterisk or mult sign*/
#define	PLUS	108					/*plus sign*/
#define	MINUS	109					/*minus sign*/
#define	SLASH	110					/*divide sign*/
#define	DIGIT	111					/*0..9*/
#define	LCAROT	112					/*less than sign*/
#define	EQUAL	113					/*equals sign*/
#define	RCAROT	114					/*greater than*/
#define	ALPHA	115					/*a..z,A..Z and underbar*/
#define	CAROT	116					/*^*/
#define	BAR		117					/*vertical bar*/

char ctype[] {
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	WHITSP,	WHITSP,	WHITSP,	WHITSP,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	WHITSP,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	WHITSP,	EXCLAM,	DQUOTE,	BADC,	BADC,	PERCNT,	AMPER,	SQUOTE,
	LPAREN,	RPAREN,	STAR,	PLUS,	COMMA,	MINUS,	PERIOD,	SLASH,
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,
	DIGIT,	DIGIT,	COLON,	SEMI,	LCAROT,	EQUAL,	RCAROT,	QMARK,
	BADC,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	LBRACK,	BADC,	RBRACK,	CAROT,	ALPHA,
	BADC,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,	ALPHA,
	ALPHA,	ALPHA,	ALPHA,	LCURBR,	BAR,	RCURBR,	COMPL,	BADC
};

	/*key word table*/
struct resword {
	char *r_name;
	int	r_value;
} reswords[] {
	"auto",		R_AUTO,
	"break",	R_BREAK,
	"case",		R_CASE,
	"char",		R_CHAR,
	"continue",	R_CONTINUE,
	"do",		R_DO,
	"default",	R_DEFAULT,
	"double",	R_DOUBLE,
	"goto",		R_GOTO,
	"else",		R_ELSE,
	"extern",	R_EXTERNAL,
	"float",	R_FLOAT,
	"for",		R_FOR,
	"if",		R_IF,
	"int",		R_INT,
	"long",		R_LONG,
	"register",	R_REGISTER,
	"return",	R_RETURN,
	"short",	R_SHORT,
	"sizeof",	R_SIZEOF,
	"static",	R_STATIC,
	"struct",	R_STRUCT,
	"switch",	R_SWITCH,
	"typedef",	R_TYPEDEF,
	"union",	R_UNION,
	"unsigned",	R_UNSIGNED,
	"while",	R_WHILE,
	0,
};

#define	SELFMOD		0200
#define	ASMASK		0177

	/*
	 * this table is used to check for an operator after an equals sign.
	 * note that =-, =* and =& may all have an ambiguous meaning if not
	 * followed by a space, this is checked for in gettok.
	 */
char asmap[] {
	EQUALS,							/*==*/
	EQADD,							/*=+*/
	EQSUB|SELFMOD,					/*=-*/
	EQMULT|SELFMOD,					/*=**/
	EQDIV,							/*=/*/
	EQOR,							/*=|*/
	EQAND|SELFMOD,					/*=&*/
	EQXOR,							/*=^*/
	EQMOD,							/*=%*/
};

char escmap[] "\b\n\r\t";
int pbchar;							/*pushed back character*/
struct symbol *symtab[HSIZE];		/*hash table*/
struct symbol *symbols;			/*pointer to next avail symbol buf*/
int nsyms;							/*number of symbol bufs in memory*/

/*
 * getdec - get a decimal number
 *		Uses Horner's method to get decimal number.  Note that
 *		multiplication by 10 is cleverly programmed as two shifts and two
 *		adds.  This is because long multiplies are painful on both the
 *		PDP-11 and 68000.
 */
long getdec()						/* returns number*/
{
	register long value;
	register char c;

	for( value = 0; (c=ngetch()) >= '0' && c <= '9'; ) {
		value =<< 1;				/*value = value*2*/
		value =+ value << 2;		/*value*2 + value*8 = value*10*/
		value =+ (c-'0');
	}
	putback(c);
	return(value);
}

#define BIAS	127L
#define EXPSIZ	4
#define FRACSIZ	20

long toieee();
long toffp();
float power10();

/*
 *	getfp - get a floating point constant
 *		we've already gotten the significant digits, now build a
 *		floating point number with possible decimal digits and an
 *		exponent, yields an ieee formated floating point number,
 *		unless the fflag is on, then a ffp constant is generated.
 */
long
getfp(significant)
long significant;
{
	register char c;
	register long places;	/* decimal places */
	int esign;
	float exp, fraction, fp;

	places = 0L; esign = 0; fraction = significant; exp = 0.0;
	if ((c = ngetch()) == '.')	/* get decimal places */
		for( ; (c=ngetch()) >= '0' && c <= '9';) {
			fraction = fraction * 10.0;
			fraction = fraction + (c - '0');
			places++;
		}

	if (c=='e' || c=='E') {	/* exponent exists */
		esign = (peekis('-')) ? 1 : (peekis('+')) ? 0 : 0;
		for( ; (c=ngetch()) >= '0' && c <= '9'; ) {
			exp = exp * 10.0;
			exp = exp + (c - '0');
		}
	}

	putback(c);
	if (esign)
		exp = -exp;
	places = exp - places;
	fp = fraction * power10(places);
	if (fflag)
		return( toffp(fp) );
	else
		return ( toieee(fp) );
}

float
power10(pwr)			/* used by getfp, 10^pwr */
long pwr;
{
	float f;

	if (pwr < 0L)	/* negative power */
		for (f = 1.0; pwr < 0L; pwr++)
			f = f / 10.0;
	else			/* positive power */
		for (f = 1.0; pwr > 0L; pwr--)
			f = f * 10.0;
	return(f);
}

long
toffp(f)		/* converts current machine float to ffp rep */
float f;
{
	register long exp;
	register int sign, count;
	long l;

	if (f == 0.0)
		return(0L);
	if (f < 0.0) {
		sign = 1;
		f = -f;
	}
	else
		sign = 0;
	exp = 0L;
	for( ; f >= 1.0; f = f / 2.0)
		exp++;
	for( ; f < 0.5; f = f * 2.0)
		exp--;
	f = f * 16777216.0;	/* 2 ^ 24 */
	l = f;
	l =<< 8;
	if (sign)
		l =| 0x80;
	exp =+ 0x40;
	l =| (exp & 0x7f);
	return(l);
}

long
toieee(f)		/* converts current machine float to ieee rep */
float f;
{
	register long exp;
	register int sign, count;
	long l;

	if (f == 0.0)
		return(0L);
	if (f < 0.0) {
		sign = 1;
		f = -f;
	}
	else
		sign = 0;
	exp = 0L;
	for( ; f >= 2.0; f = f / 2.0)
		exp++;
	for( ; f < 1.0; f = f * 2.0)
		exp--;
	f = f - 1.0;
	f = f * 8388608.0;	/* 2 ^ 23 */
	l = f;
	if (sign)
		l =| 0x80000000;
	exp = (exp + BIAS)<<23;
	l =| (exp & 0x7f800000);
	return(l);
}

#define toupper(c)	((c) & ~32)
/* gethex - get an hexidecimal number*/
/*		Uses Horner's method to get hexidecimal number*/
long gethex()						/* returns number*/
{
	register long value;
	register char c, ch;

	value = 0;
	while( 1 ) {
		if( (c=ngetch()) >= '0' && c <= '9' )
			c =- '0';
		else if((ch=toupper(c)) >= 'A' && ch <= 'F' )	/* [vlh] */
			c = ch - ('A'-10);
		else
			break;
		value = (value<<4) + c;
	}
	putback(c);
	return(value);
}

/* getoct - get an octal number*/
/*		Uses Horner's method to get octal number*/
long getoct(flag)					/* returns number*/
int flag;							/* string flag 1=>in string, else 0*/
{
	register long value;
	register char c;
	register int count;

	count = 0;
	for( value = 0; (c=ngetch()) >= '0' && c <= '7'; ) {
		if( flag && ++count > 3 )
			break;
		value = (value<<3) + (c-'0');
	}
	putback(c);
	return(value);
}

/*
 * gettok - get next token from input
 *		Checks pushed-packed token buffer, supresses / * * / comments,
 *		folds multiple character special symbols into single word token.
 */
gettok()							/* returns token type*/
{
	register int c, nextc, i;
	register char *p;
	register long value;
	char sym[SSIZE];

	if( peektok ) {
		i = peektok;
		peektok = 0;
		return(i);
	}
	while( (c=ngetch()) != EOF ) {
		switch(ctype[c]) {

		case BADC:					/*bad character*/
			error("invalid character");
			break;

		case SEMI:
			cvalue = 0;	/* [vlh] not reserved word... */
		default:
			return( ctype[c] );

		case WHITSP:				/*skip all white space*/
			break;

		case EXCLAM:				/*!= or !*/
			return( peekis('=') ? NEQUALS : NOT );

		case DQUOTE:				/*quoted string*/
			getstr(cstr,STRSIZE,'"');
			cvalue = nextlabel++;
			return(STRING);

		case PERCNT:				/*%= or %*/
			return( peekis('=') ? EQMOD : MOD );

		case AMPER:					/*&=, && or &*/
			return( peekis('=') ? EQAND : peekis('&') ? LAND : AND );

		case SQUOTE:				/*character constant*/
			getstr(cstr,STRSIZE,'\'');
			if( cstrsize > CHRSPWORD+1 ) {
				error("character constant too long");
				cstrsize = CHRSPWORD + 1;
			}
			cvalue = 0;
			for( p = cstr; --cstrsize > 0; ) {
				cvalue =<< BITSPCHAR;
				cvalue =| (*p++ & 0377);
			}
			return(CINT);

		case STAR:					/**= or **/
			return( peekis('=') ? EQMULT : MULT );

		case PLUS:					/*+=, ++ or +*/
			return( peekis('=') ? EQADD : peekis('+') ? PREINC : ADD );

		case MINUS:					/*-=, --, -> or -*/
			return( peekis('=') ? EQSUB : peekis('-') ? PREDEC :
					peekis('>') ? APTR : SUB );

		case SLASH:					/*/ *..* /, //..., /= or /*/
			if( peekis('*') ) {
				while( (c=ngetch()) != EOF )
					if( c == '*' && peekis('/') )
						break;
				if( c == EOF ) {
					error("no */ before EOF");
					return(EOF);
				}
				continue;
			}
			if( peekis('/') ) {
				while( (c=ngetch()) != EOF && c != EOLC )
					;
				continue;
			}
			return( peekis('=') ? EQDIV : DIV );

		case DIGIT:					/*number constant (long or reg)*/
			i = 0;					/*flags if long constant*/
			if( c != '0' ) {
				putback(c);
dofp:
				value = getdec();
				if ((c=ngetch())=='.' || c=='e' || c=='E') { /*[vlh] 3.4 */
					putback(c);
					clvalue = getfp(value);
					return(CFLOAT);
				}
				putback(c);
				if( value > 32767 || value < -32768 )
					i++;
			}
			else if( peekis('x') || peekis('X') ) {
				value = gethex();
				if( value < 0 || value >= 0x10000L )
					i++;
			}
			else {
				if (peekis('.')) {
					putback('.');
					goto dofp;
				}
				value = getoct(0);
				if( value < 0 || value >= 0x10000L )
					i++;
			}
			if( peekis('l') || peekis('L') || i ) {
				clvalue = value;
				return(CLONG);
			}
			cvalue = value;
			return(CINT);

		case LCAROT:				/*<=, <<, <<= or <*/
			return( peekis('=') ? LESSEQ : peekis('<') ?
					(peekis('=') ? EQLSH : LSH) : LESS );

		case EQUAL:					/*==, =<<, =>>, =+, ..., =*/
			if( peekis('<') ) {
				if( peekis('<') )
					return(EQLSH);
			}
			else if( peekis('>') ) {
				if( peekis('>') )
					return(EQRSH);
			}
			else if( (i=index("=+-*/|&^%",(c=ngetch()))) >= 0 ) {
				i = asmap[i];
				if( i & SELFMOD ) {
					if( (nextc=ngetch()) != ' ' )
						if (!wflag)	/*[vlh] old fashion initialization*/
							error("=%c assumed",c);
					putback(nextc);
				}
				return( i & ASMASK );
			}
			else
				putback(c);
			return(ASSIGN);

		case RCAROT:				/*>=, >>, >>= or >*/
			return( peekis('=') ? GREATEQ : peekis('>') ?
					(peekis('=') ? EQRSH : RSH) : GREAT );

		case ALPHA:					/*[A-Za-z][A-Za-z0-9]**/
			p = &sym[0];
			i = SSIZE;
			for(; ctype[c] == ALPHA || ctype[c] == DIGIT; c=ngetch(),i-- )
				if( i > 0 )
					*p++ = c;
			if( i > 0 )
				*p = '\0';
			putback(c);
			csp = lookup(sym);
			if( csp->s_attrib & SRESWORD ) {
				cvalue = csp->s_offset;
				return(RESWORD);
			}
			smember = 0;
			return(SYMBOL);

		case CAROT:					/*^= or ^*/
			return( peekis('=') ? EQXOR : XOR );

		case BAR:					/*|=, || or |*/
			return( peekis('=') ? EQOR : peekis('|') ? LOR : OR );

		}
	}
	return(EOF);
}

/*
 * peekis - peeks at next character for specific character
 *		Gets next (possibly pushed back) character, if it matches
 *		the given character 1 is returned, otherwise the character
 *		is put back.
 */
peekis(tc)							/* returns 1 if match, 0 otherwise*/
int tc;								/* test character*/
{
	register int c;

	if( (c=ngetch()) == tc )
		return(1);
	putback(c);
	return(0);
}

/* ngetch - get a possibly pushed back character*/
/*		Checks pbchar variable, returns it if non-zero, handles counting*/
/*		of new lines and whether you are in an include or not.*/
ngetch()							/* returns character read or EOF*/
{
	register int c;
	register char *ifile;

	if( pbchar ) {
		c = pbchar;
		pbchar = 0;
	}
	else if( (c=getc(&ibuf)) == EOLC ) {
		if( inclflag )
			inclflag = 0;
		else
			lineno++;
	}
	else if( c == SOI) {	/*[vlh]add incl filename & line # */
		inclflag++;
		ifile = &inclfile;
		while ((c=getc(&ibuf)) != SOI)
			*ifile++ = c&0377;
		*ifile = 0;
		inclline = getdec() & 077777;
		c = ' ';
	}
	else if( c < 0 )
		c = EOF;
	return(c);
}

/*
 * peekc - peek at the next non-whitespace character after token
 *		This allows for the problem of having to look at two tokens
 *		at once.  The second token is always a semi-colon or colon,
 *		so we only look at the single character, rather than going
 *		thru gettok.
 */
peekc(tc)							/* returns 1 if match, 0 otherwise*/
int tc;								/* character to look for*/
{
	register int c;

	while( ctype[(c=ngetch())] == WHITSP) ;
	if( c == tc )
		return(1);
	putback(c);
	return(0);
}

/* putback - puts back a single character*/
/*		Checks pbchar for error condition.*/
putback(c)							/* returns - none*/
int c;
{
	if( pbchar )
		error("too many chars pushed back");
	else
		pbchar = c;
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

	cstrsize = 1;
	p = str;
	for( i = nchars; (c=ngetch()) != endc; i-- ) {
		if( c == EOF || c == EOLC ) {
			error("string cannot cross line");
			break;
		}
		if( c == '\\' ) {
			if( (c=ngetch()) >= '0' && c <= '7' ) {
				putback(c);
				if( (c=getoct(1)) < 0 || c > 255 ) {
					error("bad character constant");
					continue;
				}
			}
			else if( (j=index("bnrt",c)) >= 0 )
				c = escmap[j];
			else if( c == EOLC )	/*escape followed by nl->ignore*/
				continue;
		}
		if( i > 0 ) {				/*room left in string?*/
			cstrsize++;
			*p++ = c;
		}
		else if( !i )			/*only say error once...*/
			error("string too long");
	}
	if( i <= 0 )				/*string overflow?*/
		p--;
	*p = '\0';
}

/* syminit - initialize the symbol table, install reswords*/
/*		Goes thru the resword table and installs them into the symbol*/
/*		table.*/
syminit()							/* returns - none*/
{
	register struct resword *rp;

	for( rp = &reswords[0]; rp->r_name != 0; rp++ )
		install(rp->r_name,SRESWORD|SDEFINED,rp->r_value);
}

/* install - install a symbol in the symbol table*/
/*		Allocates a symbol entry, copies info into it and links it*/
/*		into the hash table chain.*/
char *
install(sym,attrib,offset)	/* returns pointer to symbol struct*/
char *sym;							/* symbol to install*/
int attrib;							/* attribues of symbol*/
int offset;							/* symbol offset (resword value)*/
{
	register struct symbol *sp;
	register int i;

	while( (sp=symbols) <= 0 ) {
		if( (sp=sbrk(SYMSIZE)) <= 0 )
			ferror("symbol table overflow");
		for( i = SYMSIZE/(sizeof *symbols); --i >= 0; ) {
			if (sp <= 0)
				ferror("bad symbol table");
			sp->s_next = symbols;
			symbols = sp++;
		}
	}
	symbols = sp->s_next;
	sp->s_attrib = attrib;
	sp->s_sc = 0; sp->s_type = 0; sp->s_dp = 0; sp->s_ssp = 0;
	sp->s_offset = offset;
	sp->s_struc = (instruct) ? strucptr[smember+instruct] : 0;
	symcopy(sym,sp->s_symbol);			/*copy symbol to symbol struct*/
	i = symhash(sym,instruct|smember);	/*link into chain list*/
	sp->s_next = symtab[i];
	symtab[i] = sp;
	return(sp);
}

/* lookup - looks up a symbol in symbol table*/
/*		Hashes symbol, then goes thru chain, if not found, then*/
/*		installs the symbol.*/
char *lookup(sym)					/* returns pointer to symbol buffer*/
char *sym;							/* pointer to symbol*/
{
	register struct symbol *sp, *hold;
	register char *p;
	int exact;		/* same name, diff type or offset */

	p = sym;
	for( sp = symtab[symhash(p,0)]; sp != 0; sp = sp->s_next )
		if((sp->s_attrib&(SRESWORD|STYPEDEF)) && symequal(p,sp->s_symbol))
			return(sp);
	if (!(smember|instruct)) {	/*[vlh]*/
		for( sp=symtab[symhash(p,0)]; sp!=0; sp=sp->s_next )
			if( symequal(p,sp->s_symbol) )	return(sp);
	}
	else {	/* doing a declaration or an expression */
		hold = 0; exact = 0;
		for( sp=symtab[symhash(p,instruct|smember)]; sp!=0; sp=sp->s_next )
			if( symequal(p,sp->s_symbol) )
				if (symsame(sp,hold,&exact)) return(sp);
				else if (!hold && !exact) hold = sp;
		if (hold && !exact) return(hold);
	}
	return(install(p,0,0));
}

/* freesyms - frees all local symbols at end of function declaration*/
/*		Searches thru symbol table, deleting all symbols marked as locals*/
freesyms()							/* returns - none*/
{
	register int i, tinfo;
	register struct symbol *sp, *tp, *nextp, **htp;

	for( htp = &symtab[0], i = HSIZE; --i >= 0; htp++ )
		for( tp = 0, sp = *htp; sp != 0; sp = nextp ) {
			nextp = sp->s_next;
			if( !(sp->s_attrib&SDEFINED) ) {
				error("undefined label: %.8s",sp->s_symbol);
				sp->s_attrib =| SDEFINED;
			}
			if( sp->s_attrib & (SGLOBAL|SRESWORD) )
				tp = sp;
			else {
				if( tp )
					tp->s_next = sp->s_next;
				else
					*htp = sp->s_next;
				sp->s_next = symbols;
				symbols = sp;
			}
		}
}

/* chksyms - checks symbol table for undefined symbols, etc.*/
/*		Goes thru the symbol table checking for undeclared forward*/
/*		referenced structures, and outputs local symbols for debugger.*/
chksyms()							/* returns - none*/
{
	register struct symbol **htp, *sp;
	register int i, sc;

	for( htp = &symtab[0], i = HSIZE; --i >= 0; htp++ )
		for( sp = *htp; sp != 0; sp = sp->s_next ) {
			sc = sp->s_sc;
			if(sc!=0 && sp->s_ssp>=0 && (btype(sp->s_type))==FRSTRUCT) {
				sp->s_ssp = frstab[sp->s_ssp]->s_ssp;	/* 3.4 ssp>0 */
				sp->s_type = (sp->s_type&~TYPE) | STRUCT;
			}
			if( sc == PDECLIST ) {
				error("not in parameter list: %.8s",sp->s_symbol);
				sp->s_sc = AUTO;
			}
			if( infunc )
				outlocal(sp->s_type,sp->s_sc,sp->s_symbol,sp->s_offset);
		}
}

/* symhash - compute hash value for symbol*/
/*		Sums the symbols characters and takes that modulus the hash table*/
/*		size.*/
symhash(sym,stel)					/* returns hash value for symbol*/
char *sym;							/* pointer to symbol*/
int stel;							/* structure element flag*/
{
	register char *p;
	register int hashval, i;

	hashval = (stel ? STEL : 0 );
	for( p = sym, i = SSIZE; *p != '\0' && i > 0; i-- )
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

	for( p = sym1, q = sym2, i = SSIZE; *p == *q++; )
		if( *p++ == '\0' || --i == 0 )
			return(1);
	return(0);
}

/* symsame - symbol member same as declared */
symsame(sp,hold,exact)	/* [vlh] */
struct symbol *sp, *hold;
int *exact;
{
	if (strucptr[smember+instruct])
		if (strucptr[smember+instruct]==sp->s_struc) return(1);
	if (hold)
		if (sp->s_type != hold->s_type || sp->s_offset != hold->s_offset)
			*exact = 1;
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
		*q++ = ( *p ? *p++ : '\0');
}

/* index - find the index of a character in a string*/
/*		This is identical to Software Tools index.*/
index(str,chr)						/* returns index of c in str or -1*/
char *str;							/* pointer to string to search*/
char chr;							/* character to search for*/
{
	register char *s;
	register int i;

	for( s = str, i = 0; *s != '\0'; i++ )
		if( *s++ == chr )
			return(i);
	return(-1);
}

/* next - if next token matches given token, skip and return success*/
/*		This allows for clean parsing of declarations.*/
next(tok)							/* returns 1 if matched, 0 otherwise*/
int tok;
{
	register int token;

	if( (token=gettok()) == tok )
		return(1);
	peektok = token;
	return(0);
}

/* pbtok - put back the given token*/
/*		This merely sets the peektok variable*/
pbtok(tok)							/* returns - none*/
int tok;
{
	if( peektok )
		error("too many tokens pushed back");
	peektok = tok;
}
