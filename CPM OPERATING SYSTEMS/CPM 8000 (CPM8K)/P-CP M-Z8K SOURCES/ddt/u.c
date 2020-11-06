/*
**
**
**
**
**
**
**
**
**		Zilog Extended Run Time Library
**
**		October 1, 1982	Thorn Smith
**
**
**
**
**		***** PROPRIETARY ******
**
**
**	This document is the property of Zilog corporation,
**	and is protected from unauthorized duplication under
**	applicable federal, state, and civil copyright laws.
**	No part of this document may be reproduced, stored,
**	or transmitted, in any form or by any means, such as
**	electronic, mechanical, photocopying, recording, or
**	otherwise, without the prior written permission of
**	Zilog, inc. 1315 Dell Ave., Campbell, Calif. 95008
**	tel: (408) 370-8000		twx: 910-338-7621
**
**
**
**
**
**
**
**
**	Copyright 1982 Zilog Corporation, all rights reserved.
**
** date listed:		______	____/____/____	____:____	____
**
** project name:	619ss - ZRTS software enhancement development
**
** program name:	Zilog extended run time library
**
** specifications:	Thorn Smith x 370-8317
**
** implementation:	Thorn Smith x 370-8317
**
** documentation:	Thorn Smith x 370-8317
**
** environment:		Zilog S8000 v2.1
**
** language:		Unix- C
**
** approvals-mgr:	________________________________________, Zilog
**
** approvals-q.a:	________________________________________, Zilog
**
** approvals-d.v:	________________________________________, Zilog
**
** program description:
**
**	*************************************************
**	*						*
**	*	I N C L U D E	F I L E S		*
**	*						*
**	*************************************************
**
*/

#include	"env.h"
#include	"u.h"

#define	POSTFIX

/*
**	*************************************************
**	*						*
**	*	I N T E R N A L   C O N S T A N T S	*
**	*						*
**	*************************************************
**
*/

/* (none) */

/*
**	*************************************************
**	*						*
**	*	G L O B A L S   /   E X T E R N A L S	*
**	*						*
**	*************************************************
**
*/

BYTE	tbyte;
WORD	tword;
LONG	tlong;
POINTER	tptr;

int	error,errctr;			/* global error constant */
int	infixflg;

char	*iscan,*oscan,*tscan;
char	ichksum,ochksum;
char	iline[COLS],oline[COLS];	/* the IO buffers */

char	symbuf[SYMLEN+1];

char	aflag,bflag,cflag,dflag,eflag,hflag,iflag,jflag,kflag;
char	lflag,mflag,nflag,oflag,pflag,qflag,rflag,sflag,tflag;
char	uflag,vflag,wflag,xflag,yflag,zflag;

/*
**	*************************************************
**	*						*
**	*	S T A R T   O F   C O D E		*
**	*						*
**	*************************************************
**
*/

/*
********	my dear aunt sally	(infix expression evulator)
*/

/*
**	This Routine is called by the debugger, and forces an error
**	condition when their is a undefined symbol.
**	The counter "infixflg" is bumped in getsym when a symbol is
**	not present and skips over the label. If the counter was bumped,
**	Then an error will be generated.
*/

MDASIZE	infixer()
{
	MDASIZE	infix(),accu;
	accu= infix();
	if (error)	return (accu);
	while (*iscan == ')')	{ iscan++;	error= 17; }
	if (error)	return (accu);
	if (!delchr(*iscan))	error= 12;
	if (error)	return (accu);
	if (infixflg)	error=10;
	return (accu);
}

/*
**	This Routine is called by the assembler, and will NOT
**	generate an error condition when their is a undefined symbol.
**	The counter "infixflg" is bumped in getsym() when the symbol is
**	not present and skips over the label, but an error is not flagged.
*/

MDASIZE	infix()
{
	MDASIZE	infix0();
	infixflg= 0;
	return (infix0());
}

/*
********	null	operator(s)
*/

MDASIZE	infix0()
{
	MDASIZE	infix4();
	return (infix4());
}

/*
********	||	operator(s)
*/

MDASIZE	infix4()
{
	MDASIZE	infix5(),accu,accu1;
	accu= infix5();
	while (op2test("||"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infix5();
		accu= (accu || accu1);
	}
	return (accu);
}

/*
********	&&	operator(s)
*/

MDASIZE	infix5()
{
	MDASIZE	infix6(),accu,accu1;
	accu= infix6();
	while (op2test("&&"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infix6();
		accu= (accu && accu1);
	}
	return (accu);
}

/*
********	|	operator(s)
*/

MDASIZE	infix6()
{
	MDASIZE	infix7(),accu,accu1;
	accu= infix7();
	while (op2test("|"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infix7();
		accu= (accu | accu1);
	}
	return (accu);
}

/*
********	^	operator(s)
*/

MDASIZE	infix7()
{
	MDASIZE	infix8(),accu,accu1;
	accu= infix8();
	while (op2test("^"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infix8();
		accu= (accu ^ accu1);
	}
	return (accu);
}

/*
********	&	operator(s)
*/

MDASIZE	infix8()
{
	MDASIZE	infix9(),accu,accu1;
	accu= infix9();
	while (op2test("&"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infix9();
		accu= (accu & accu1);
	}
	return (accu);
}

/*
********	==,!=	operator(s)
*/

MDASIZE	infix9()
{
	MDASIZE	infixa(),accu,accu1;
	accu= infixa();
infix9a:
	if (op2test("=="))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixa();
		accu= (accu == accu1);
		goto	infix9a;
	}
	if (op2test("!="))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixa();
		accu= (accu != accu1);
		goto	infix9a;
	}
	return (accu);
}

/*
********	<,<=,>,>=	operator(s)
*/

MDASIZE	infixa()
{
	MDASIZE	infixb(),accu,accu1;
	accu= infixb();
infixaa:
	if (op2test("<"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixb();
		accu= (accu < accu1);
		goto	infixaa;
	}
	if (op2test("<="))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixb();
		accu= (accu <= accu1);
		goto	infixaa;
	}
	if (op2test(">"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixb();
		accu= (accu > accu1);
		goto	infixaa;
	}
	if (op2test(">="))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixb();
		accu= (accu >= accu1);
		goto	infixaa;
	}
	return (accu);
}

/*
********	<<,>>	operator(s)
*/

MDASIZE	infixb()
{
	MDASIZE	infixc(),accu,accu1;
	accu= infixc();
infixba:
	if (op2test("<<"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixc();
		accu= (accu << accu1);
		goto	infixba;
	}
	if (op2test(">>"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixc();
		accu= (accu >> accu1);
		goto	infixba;
	}
	return (accu);
}

/*
********	+,-	operator(s)
*/

MDASIZE	infixc()
{
	MDASIZE	infixd(),accu,accu1;
	accu= infixd();
infixca:
	if (op2test("+")) {
		if (error)	return ((MDASIZE)0);
		accu1= infixd();
		accu+= accu1;
		goto infixca;
	}
	if (op2test("-"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixd();
		accu-= accu1;
		goto infixca;
	}
	return (accu);
}

/*
********	*,/	operator(s)
*/

MDASIZE	infixd()
{
	MDASIZE	infixe(),accu,accu1;
	accu= infixe();
infixda:
	if (op2test("*"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixe();
		accu*= accu1;
		goto infixda;
	}
	if (op2test("/"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixe();
		if (accu1 == 0)	error=14;
		if (error)	return ((MDASIZE)0);
		accu/= accu1;
		goto infixda;
	}
	if (op2test("%"))	{
		if (error)	return ((MDASIZE)0);
		accu1= infixe();
		if (accu1 == 0)	error=15;
		if (error)	return ((MDASIZE)0);
		accu= accu/accu1;
		goto infixda;
	}
	return (accu);
}

/*
********	++,--	operator(s)
*/

MDASIZE	infixe()
{
	MDASIZE	infixx();
	if (error)	return ((MDASIZE)0);
	if (op2test("++"))	{ return (infixx()+1); }
	if (op2test("--"))	{ return (infixx()-1); }
	return (infixx());
}

/*
********	terminal node
*/

MDASIZE	infixx()
{
	MDASIZE	infix0(),infixw(),accu;
	itrim();
	if (op2test("!"))		return (!infixx());
	if (op2test("~"))		return (~infixx());
	if (op2test("-"))		return (-infixx());
	if (*iscan == ':')		return (numin());
	if (*iscan == '.')		return (numin());
	if (*iscan == '@')		return (numin());
	if (isdec(*iscan,10) != -1)	return (numin());
	if (symchr(*iscan))		return (infixw());
	if (*iscan == '(')	{
		iscan++;
		accu= infix0();
		if (*iscan != ')')	error=16;
		if (*iscan == ')')	iscan++;
		return (accu);
	}
	if (*iscan == '\'')	{
		iscan++;
		accu= (char)*iscan;
		if (*iscan == '\\')	{
			iscan++;
			if (*iscan == '0')	accu= 0x00;
			if (*iscan == 't')	accu= 0x09;
			if (*iscan == 'n')	accu= 0x0A;
			if (*iscan == 'r')	accu= 0x0D;
		}
		iscan++;
		if (*iscan != '\'')	error=18;
		iscan++;
		return (accu);
	}
	if (delchr(*iscan))	return ((MDASIZE)0);
	error= 11;
	return ((MDASIZE)0);
}

/*
********	symbol postfix
*/

MDASIZE	infixw ()
{
	MDASIZE	infix0(),accu;
	int	symnum;
	skipsym();
	if (error)	return ((MDASIZE)0);
	symnum= findsym (symbuf);
	if (symnum == 0)	infixflg++;		/* undefined symbol */
	if (op2test("="))	{
		if (symnum == 0)	symnum= addsym(symbuf,1,(MDASIZE)0);
		if (error)	return ((MDASIZE)0);
		accu= infix0();
		if (error)	return ((MDASIZE)0);
		symtbl[symnum].uvalue= accu;
		return (symtbl[symnum].uvalue);
	}
	if (op2test("+="))	{
		if (symnum == 0)	symnum= addsym(symbuf,1,(MDASIZE)0);
		if (error)	return ((MDASIZE)0);
		accu= infix0();
		if (error)	return ((MDASIZE)0);
		symtbl[symnum].uvalue+= accu;
		return (symtbl[symnum].uvalue);
	}
	if (op2test("-="))	{
		if (symnum == 0)	symnum= addsym(symbuf,1,(MDASIZE)0);
		if (error)	return ((MDASIZE)0);
		accu= infix0();
		if (error)	return ((MDASIZE)0);
		symtbl[symnum].uvalue-= accu;
		return (symtbl[symnum].uvalue);
	}
/*
**	if ((*iscan == '+') && (*(iscan+1)=='+'))	{
**		iscan+= 2;
**		if (symnum == 0)	symnum= addsym(symbuf,1,(MDASIZE)0);
**		if (error)	return ((MDASIZE)0);
**		symtbl[symnum].uvalue+= 1;
**		return (symtbl[symnum].uvalue-1);
**	}
**	if ((*iscan == '-') && (*(iscan+1)=='-'))	{
**		iscan+= 2;
**		if (symnum == 0)	symnum= addsym(symbuf,1,(MDASIZE)0);
**		if (error)	return ((MDASIZE)0);
**		symtbl[symnum].uvalue-= 1;
**		return (symtbl[symnum].uvalue+1);
**	}
*/
	if (symnum == 0)	return ((MDASIZE)0);
	return (symtbl[symnum].uvalue);
}

/*
********	radix parser
*/

MDASIZE	numin()
{
	MDASIZE	number;
	char	*nscan;
	if (*iscan == '@')			return (adrin());
	if (*iscan == ':')	{ iscan++;	return (fltin( 8)); }
	if (*iscan == '.')			return (fltin(10));
	if (*iscan == '0')	{
		number= fltin(16);
		if (tolower(*iscan) == 'h')	iscan++;
		return (number);
	}
	if (isdec(*iscan,10) == -1)	{ error=28;	return(0); }
#ifdef	POSTFIX
	nscan= iscan;
	number= fltin(16);	/* prescan */
	if (tolower(*iscan) == 'h' || tolower(*iscan) == 'x')	{
		iscan= nscan;
		number= fltin(16);
		iscan++;
		return(number);
	}
	if (tolower(*iscan) == 'q' || tolower(*iscan) == 'o')	{
		iscan= nscan;
		number= fltin(8);
		iscan++;
		return(number);
	}
	if (tolower(*iscan) == 'b')	{
		iscan= nscan;
		number= fltin(2);
		iscan++;
		return(number);
	}
	if (tolower(*iscan) == 'd' || tolower(*iscan) == 'e')	{
		iscan= nscan;
		number= fltin(10);
		iscan++;
		return(number);
	}
#endif
	iscan= nscan;
	number= fltin(10);	/* default decimal */
	return (number);
}

/*
********	numeric decoder
*/

MDASIZE	fltin(radix)
int	radix;
{
	MDASIZE mantisa,power;
	int	digit,sign;

	mantisa= 0;
	power= 1;
	sign= 1;

	itrim();
	if (*iscan == '-')	{ iscan++;	sign= (-1); }
	while ((digit= isdec(*iscan,radix)) != -1) {
		mantisa = radix * mantisa + digit;
		iscan++;
	}
	if (*iscan == '.') {
		iscan++;
		while ((digit= isdec(*iscan,radix)) != -1) {
			mantisa= radix  * mantisa + digit;
			power*= radix;
			iscan++;
		}
	}
	itrim();
	return (mantisa / power * sign);
}

/*
********	operand tester rtn
*/

op2test(str)
char	*str;
{
	char	*nscan;
	itrim();
	nscan= iscan;
	while (!eolchr(*str))
		if (tolower(*nscan++) != tolower(*str++))	return (0);
	if (isdec(*nscan,10) != -1)	goto op2t1;
	if (spcchr(*nscan))	goto op2t1;
	if (symchr(*nscan))	goto op2t1;
	if (*nscan == '\'')	goto op2t1;
	if (*nscan == ':')	goto op2t1;
	if (*nscan == '@')	goto op2t1;
	if (*nscan == '(')	goto op2t1;
	if (*nscan == '!')	goto op2t1;
	if (*nscan == '~')	goto op2t1;
	if (*nscan == '-')	goto op2t1;
	return (0);
op2t1:
	iscan= nscan;
	return (1);
}

/*
********	single character decoder and tester
*/

isdec (c,radix)
char	c;
int	radix;
{
	if (c < '0')	return (-1);
	if (c >= '0' && c <= '9')	c= c - '0';
	c= tolower(c);
	if (c >= 'a' && c <= 'f')	c= c - 'a' + 10;
	if (c >= radix)	return (-1);
	return (c);
}

/*
****************	symbol routines (to be redefined if wanted)
*/


/*
********	Display A symbol table
*/

dspsyms()
{
	int	syml,tab;
	int	symnum;
	tab= (SYMLEN+9+2);
	for (symnum=1; symtbl[symnum].utype != LASTSYM; symnum++)	{
		if (symtbl[symnum].uname[0] == '\0')	break;
		syml= strlen(symtbl[symnum].uname);
		while (syml++ < SYMLEN)	printf (" ");
		printf ("%s=%09lx ",symtbl[symnum].uname,symtbl[symnum].uvalue);
		tab += (SYMLEN+9+2);
		if (tab < SWIDTH)	continue;
		printf ("\n");
		tab= (SYMLEN+9+2);
	}
	printf ("\n");
}

/*
********	Get a symbol from a given symbol table
*/

MDASIZE	getsym ()
{
	int	symnum;
	skipsym();
	if (error)	return ((MDASIZE)0);
	symnum= findsym (symbuf);
	if (symnum == 0)	infixflg++;		/* undefined symbol */
	return (symtbl[symnum].uvalue);
}

/*
********	add an unbuffered symbol into a given symbol table
*/

setsym (symtype,symval)
int	symtype;
MDASIZE	symval;
{
	if (*iscan >= '0' && *iscan <= '9')	{
		error=21;
		return (0);
	}
	skipsym();
	if (error)	return (0);
	return (addsym(symbuf,symtype,(MDASIZE)symval));
}

/*
********	copy the symbol at iscan into symbuf and while skippig over it
*/

skipsym()
{
	char	*wp;
	int	i;
	wp= symbuf;
	for (i=0; (i<SYMLEN) && (symchr(*iscan)); i++)	*wp++= *iscan++;
	*wp= '\0';
	if (i==0)	{ error=22; }
	if (i>=SYMLEN)	{
		while (symchr(*iscan)) iscan++;
		error=24;
	}
	return (i);
}

/*
********	add a buffered symbol table into a given symbol table
*/

addsym (sym,symtype,symval)
char	*sym;
int	symtype;
MDASIZE	symval;
{
	int	symnum;
	char	*wp;
	symnum= findsym (sym);
	if (symnum)	{ error=25;	return (0); }
	symnum= findsym ("\0");
	if (!symnum)	{ error=26;	return (0); }
	wp= symtbl[symnum].uname;
	while (*sym != '\0')	*wp++= *sym++;
	*wp='\0';
	symtbl[symnum].uvalue= symval;
	symtbl[symnum].utype= symtype;
	return (symnum);
}

/*
********	find a symbol in a given symbol table
*/

findsym (sym)
char	*sym;
{
	int	symnum;
	for (symnum=1; symtbl[symnum].utype != LASTSYM; symnum++)	{
		if (!strcmp(sym,symtbl[symnum].uname))	return (symnum);
		if (symtbl[symnum].uname[0] == '\0')	break;
	}
	symnum=0;
	return (symnum);
}

/*
********	check for proper symbol character
*/

symchr (c)
char	c;
{
	c= tolower(c);
	if (c >= 'a' && c <= 'z')	return (1);
	if (c >= '0' && c <= '9')	return (1);
	if (c=='$' || c=='_')	return (1);
	return (0);
}

/*
****************	utility routines
*/

/*
********	encode/decode utilities
*/

MDASIZE	adrin()		/* compressed address in (SSAAAA -> SS00AAAA) */
{
	MDASIZE	addr,addr1;
	itrim();
	if (*iscan == '@')	iscan++;
	addr= hexin(8);
	addr1= (addr << 8) & 0x7F000000L;
	return ((addr & 0xFFFFL) | addr1);
}


MDASIZE	hexin(len)	/* hexadecimal number in */
int	len;
{
	MDASIZE	number= 0;
	itrim();
	while (len-- && (ishex(*iscan) != -1)) {
		number= (number << 4) + ishex(*iscan++);
	}
	return (number);
}

/*
********	swap register bytes routine
*/

WORD	bswap(reg)
WORD	reg;
{
	return (((reg >> 8) & 0x00FF) | ((reg << 8) & 0xFF00));
}

/*
********	character test and translate routines
*/

scmp (s1,s2)
char	*s1,*s2;
{
	while (*s1)	{
		if (tolower(*s1) != tolower(*s2))	break;
		s1++;	s2++;
	}
	return (*s1 - *s2);
}

scmpn (s1,s2,count)
char	*s1,*s2;
int	count;
{
	while (*s1)	{
		if (tolower(*s1) != tolower(*s2))	break;
		if (count-- <= 1)	return(0);
		s1++;	s2++;
	}
	return (*s1 - *s2);
}

uctolc (p)
char	*p;
{
	while (*p != '\0')	{
		*p= tolower (*p);
		p++;
	}
}

tolower(c)
char	c;
{
	if (c >= 'A' && c <= 'Z') c= c-'A'+'a';
	return (c);
}

toupper(c)
char	c;
{
	if (c >= 'a' && c <= 'z') c= c-'a'+'A';
	return (c);
}

ishex(c)
char	c;
{
	if (c >= '0' && c <= '9') return ((c-'0'));
	if (c >= 'a' && c <= 'f') return ((c-'a'+10));
	if (c >= 'A' && c <= 'F') return ((c-'A'+10));
	return ((-1));
}

itrim()		/* trim white space (blanks and tabs) */
{
	while (spcchr(*iscan))	iscan++;
}

spcchr(c)
char	c;
{
	return (c == ' ' || c == '\t');
}

eolchr(c)
char	c;
{
	return (c == ';' || c == '\0' || c == '\n');
}

delchr(c)
char	c;
{
	return (spcchr(c) || eolchr(c) || c == ',');
}

/*
****************	err()	- error display rtn
** This routine displays the associated diagnostic message for "error"
** which is an external symbol.
**
*/

char	*errors[]= {

/* system errors */
	/* 00 */  "",
	/* 01 */  "	unknown instruction",
	/* 02 */  "	Too many errors to continue!",
	/* 03 */  "	unknown command flag",
	/* 04 */  "	missing file",
	/* 05 */  "	bad line delimiter",
	/* 06 */  "	bad field delimiter",
	/* 07 */  "	cant open memory file",
	/* 08 */  "	memory file read error",
	/* 09 */  "	memory file write error",

/* infix errors */
	/* 10 */  "infix:	undefined symbol in expression",
	/* 11 */  "infix:	undefined prefix operator",
	/* 12 */  "infix:	undefined operator",
	/* 13 */  "infix:	undefined postfix operator",
	/* 14 */  "infix:	division by zero",
	/* 15 */  "infix:	modulus by zero",
	/* 16 */  "infix:	missing ')' in expression",
	/* 17 */  "infix:	missing '(' in expression",
	/* 18 */  "infix:	missing closing \'",
	/* 19 */  "infix:	missing closing \"",

/* symbol errors */
	/* 20 */  "sym:	undefined symbol",
	/* 21 */  "sym:	bad character in symbol",
	/* 22 */  "sym:	missing symbol when expecting one",
	/* 23 */  "sym:	symbol defination too long",
	/* 24 */  "sym:	symbol name too long",
	/* 25 */  "sym:	symbol mutliply defined",
	/* 26 */  "sym:	symbol table overflow",
	/* 27 */  "",
	/* 28 */  "",
	/* 29 */  "infix:	undefined postfix radix operator",

/* mad errors */
	/* 30 */  "mad:	bad opcode",
	/* 31 */  "mad:	bad operand 1",
	/* 32 */  "mad:	bad operand 2",
	/* 33 */  "mad:	bad operand 3",
	/* 34 */  "mad:	bad operand 4",
	/* 35 */  "mad:	bad operand 5",
	/* 36 */  "mad:	bad operand 6",
	/* 37 */  "mad:	bad operand 7",
	/* 38 */  "mad:	bad operand 8",
	/* 39 */  "mad:	relative address out of range",
	/* 40 */  "mad:	opcode ok, but bad operand(s)",
	/* 41 */  "mad:	bad case for asfind(fmatch)",
	/* 42 */  "mad:	bad case for asbin",
	/* 43 */  "mad:	bad case for difind",
	/* 44 */  "mad:	bad case for disop",
	/* 45 */  "mad:	too many asm arguments",
	/* 46 */  "mad:	index mismatch",
	/* 47 */  "mad:	getnext mismatch",
	/* 48 */  "mad:	memory mismatch",
	/* 49 */  "mad:	Multiple dissembly",

/* dbg errors */
	/* 50 */ "dbg:	unknown dbg command",
	/* 51 */ "dbg:	bad arg1 ",
	/* 52 */ "dbg:	bad arg2 ",
	/* 53 */ "dbg:	bad arg3 ",
	/* 54 */ "dbg:	bad arg4 ",
	/* 55 */ "dbg:	bad arg5 ",
	/* 56 */ "dbg:	bad arg6 ",
	/* 57 */ "dbg:	bad arg7 ",
	/* 58 */ "dbg:	bad arg8 ",
	/* 59 */ "dbg:	bad arg9 ",

	/* 60 */ "",
	/* 61 */ "dbg:	need 1 arg",
	/* 62 */ "dbg:	need 2 args",
	/* 63 */ "dbg:	need 3 args",
	/* 64 */ "dbg:	need 4 args",
	/* 65 */ "dbg:	arg1 > arg2",
	/* 66 */ "dbg:	arg1 >= arg2",
	/* 67 */ "dbg:	from == to",
	/* 68 */ "",
	/* 69 */ "",

	/* 70 */ "dbg:	bad bkpt delimiter, use [+-,]",
	/* 71 */ "dbg:	breakpoint is already set",
	/* 72 */ "dbg:	bkpt table overflow",
	/* 73 */ "dbg:	cant find breakpoint",
	/* 74 */ "dbg:	no breakpoint to clear",
	/* 75 */ "dbg:	unknown register",
	/* 76 */ "dbg:	bad delimiter",
	/* 77 */ "dbg:	unknown flag nmeumonic",
	/* 78 */ "dbg:	zero bkpt count",
	/* 79 */ "dbg:	missing instruction",

/* inload errors */
	/* 80 */  "inload:	can't open inload file",
	/* 81 */  "inload:	Bad first prompt in inload file",
	/* 82 */  "inload:	Bad record length in inload file",
	/* 83 */  "inload:	Bad load address in inload file",
	/* 84 */  "inload:	Bad record type in inload file",
	/* 85 */  "inload:	Bad data record in inload file",
	/* 86 */  "inload:	Bad record type in inload file",
	/* 87 */  "inload:	Bad checksum in inload file",
	/* 88 */  "inload:	Data after checksum in inload file",
	/* 89 */  "inload:	cant load program",

/* asm errors */
	/* 90 */ "",
	/* 91 */ "asm:	cant open source file",
	/* 92 */ "asm:	cant open object file",
	/* 93 */ "asm:	cant open listing file",
	/* 94 */ "",
	/* 95 */ "asm:	undefined pseudo symbol in final pass",
	/* 96 */ "asm:	undefined symbol in final pass",
	/* 97 */ "asm:	undefined pseudop",
	/* 98 */ "asm:	symbol mismatch - internal fault!",
	/* 99 */ "",

	/*100 */ "",
	/*101 */ "asm:	'if' without 'endif'",
	/*102 */ "asm:	'else' without 'if'",
	/*103 */ "asm:	'endif' without 'if'",
	/*104 */ "asm:	cannot nest 'if'",
};

err()
{
	if (!error)	return;
	printf (" error #%d\t%s\n",error,errors[error]);
	error=0;	/* reset */
	errctr++;
}
