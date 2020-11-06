/*
**
**
**
**
**		Z8000 Assembler / Disassembler
**
**
**		July 1, 1982	Thorn Smith
**
**
**
**
**		***** PROPRIETARY ******
**
**	Copyright 1982 Zilog Corporation, all rights reserved.
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
** date listed:		______	____/____/____	____:____	____
**
** project name:	619ss - ZRTS software enhancement development
**
** program name:	MAD (Mini Assembler/Disassembler)
**
** specifications:	Thorn Smith x 370-8317
**
** implementation:	Thorn Smith x 370-8317
**
** documentation:	Thorn Smith x 370-8317
**
** environment:		Zilog zeus z8001
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
**
*/

/*
**	*************************************************
**	*						*
**	*	I N C L U D E	F I L E S		*
**	*						*
**	*************************************************
**
*/

/* #include	<stdio.h> */
#include	"env.h"
#include	"u.h"
#include	"mad.h"

/*
**	*************************************************
**	*						*
**	*	M A C R O S				*
**	*						*
**	*************************************************
**
*/


/*
********	globals for assembler / disassembler
**
**	char	tline[COLS]	- temporary buffer used for beautification
**	int	types[MAXF]	- array for all of the fields
**	int	values[MAXF]	- array of numeric decode
**	int	fields[MAXF]	- pointers to fields in formatted oscan.
**	int	error		- error code (0 if o.k.)
*/

char	tline[COLS];
char	*fields[MAXF];
int	types[MAXF];
long	values[MAXF];
int	args,opcode;

int	opsa,opsb;
int	ttype,fldtype,bitoff,offset,bitnum;
int	ivalue;
long	tvalue;
int	z12,segmode;
POINTER	slide;		/* relative pc offset for assembler */

/*

************************************************************************
**
********	asminit (type,mode)
**
*/

madinit (type,mode)
int	type,mode;
{
	z12= type;
	segmode= mode;
}

/*

************************************************************************
**
********	asmbl	(base,lptr)	- micro assembler entry
** W	base=	a pointer to memory of where to put the produced binary, it is
**		a pointer to unsigned characters.
** R,W	lptr=	is a pointer to a host provided buffer < 40 chars that asmbl
**		will use for input, it must be '\0' terminated without a '\n'.
**
** This function only dispatches to the following assembler subroutines:
** first:	"asmfmt" to clean up the line, compress double delimiters
**		and white space, and to call the expression/label decoder
**		when a '~' is detected. The result of the expression is
**		sequentially "pushed" into values[] for later.
** second:	"asfind" to alphabeticaly search for a match between a
**		(the) formatted line produced by "asmfmt" and the ascii
**		templates in ops[]. If no match is found, then opnum will
**		return as zero, which defaults to a ".byte" pseudo-op.
** third:	"asbin" to then place the opnum into memory at "base".
**		This subroutine is two pass in installing the binary into
**		memory. First, it places the opcode field into memory, then
**		it makes a second pass to overlay/append the operands
**		from "values[]" into memory.
**
** "asmbl" will return a pointer to the found indexed item of ops[], which is
** an array of structures "op". A negative value represents an error condition.
** To bump "base" to the next binary address, call "getnext" with the index
** returned from "asmbl".
**
** WARNING:	the string given at lptr will be compressed and cleaned-up into
**		itself, and will be modified. It will still be null terminated
**		and will not have a '\n' in it.
**
*/

asmbl (base,lptr)
POINTER	base;
char	*lptr;
{
	int	opnum;
	error=0;
	asmfmt (lptr);		/* format lptr & decode values */
	if (args == 0)	return (0);
	if (error)	return (0);
	opnum= asfind ();	/* lookup the opcode and operands */
	if (error)	return (0);
	asbin (base,opnum);	/* install the binary into memory */
	if (error)	return (0);
	return (opnum);
}

/*

********	asmfmt (lptr)	- micro assembler preprocessor
** R,W	lptr=	is a pointer to a host provided buffer < 40 chars that asmfmt
**		will use for input, it must be '\0' terminated without a '\n'.
**
** This routine is the first one called by "asmbl". It compresses double
** "white" characters into single ones, to fix up the format for ops[].opcode.
** it also involkes the expression evaulator whenever a '~' is
** encountered and compresses them into values[]
**
** "asmfmt" returns either the number of arguments involked by a '~'.
** or a negative number indicating an error condition. The returned
** argument count is only used by "asmbl" to verify that their are
** accordingly values set also within the record.
**
** WARNING:	the string given at lptr will be compressed and cleaned-up into
**		itself, and will be modified. It will still be null terminated
**		and will not have a '\n' in it.
**
*/

asmfmt (str1)
char	*str1;
{
	int	temp;

	for (args=0; args < MAXF; args++)
		 values[args]= types[args]= 0;

	iscan= str1;
	oscan= tline;
	asmparse();	/* break-up line into fields[] */
	if (!args)	return;

	for (opcode=0; *opcodes[opcode]; opcode++)
		if (scmp(fields[0],opcodes[opcode]) == 0)	break;
	if (*opcodes[opcode] == '\0')	error=30;
	if (error)	return;
	values[0]= opcode;

	for (args=1; args <= MAXA; args++)	{
		if (error)	break;
		if (fields[args] == 0)	break;
		if (*fields[args] == ';')	break;
		if (*fields[args] == '\0')	break;
		iscan= fields[args];		/* iscan= read ptr */
		oscan= fields[args];		/* oscan= write ptr */

		/* 2.1) scan for discreet opcode,operand */
		temp= symfind(iscan);
		if (temp)	{
			types[args]= oprands[temp].utype;
			values[args]= oprands[temp].uvalue;
			continue;
		}

		/* 2.2) scan for addresses */
		if (*iscan == '@')	{
			iscan++;
			types[args]=AA;
			values[args]= infix();
			continue;
		}
		if (*iscan == '(')	{
			iscan++;
			if (*iscan == '#')	iscan++;
			types[args]=AA;
			values[args]= infix();
			if (*iscan != ')')	error=16 + args;
			continue;
		}

		/* 2.3) scan for constants */
		if (*iscan == '#')	{
			iscan++;
			types[args]=00;
			values[args]= infix();
			continue;
		}

		/* the default (assumed address) */
		types[args]=AA;
		values[args]= infix();
	}
}

asmparse()
{
	for (args=0; args < MAXF; args++)	fields[args]= 0;

	for (args=0; args < MAXF; args++)	{
		while (*iscan=='\t' || *iscan==' ' || *iscan==',')	iscan++;
		if (*iscan == '\0')	break;
		if (*iscan == '\n')	break;
		if (*iscan == '\r')	break;
		if (*iscan == ';')	break;
		fields[args]= oscan;
		movfld ();
	}
	*oscan= '\0';
}

movfld ()
{
	int	exprflg;
	char	c;
	exprflg= 1;
	if (*iscan == '(')	*oscan++= *iscan++;
	while (*iscan != '\0')	{
		if (*iscan == '\'')	{
			*oscan++= *iscan++;
			if (*iscan == '\\')	*oscan++= *iscan++;
			if (*(iscan+1) != '\'')	break;
			*oscan++= *iscan++;
			*oscan++= *iscan++;
		}

		c= *iscan;
		if (c == '+' || c == '-')	exprflg=0;
		if (c == '*' || c == '/')	exprflg=0;
		if (c == '%' || c == '!')	exprflg=0;
		if (c == '(' && exprflg)	break;
		if (c == ',')	break;
		if (c == ';')	break;
		if (c <= ' ')	break;
		*oscan++= c;	/* tolower? */
		iscan++;
		if (c == ')' && exprflg)	break;
	}
	*oscan++= '\0';
}

/*

********	asfind	(lptr)	- assembler match & binary load
** R	lptr=	is a pointer to a host provided buffer < 40 chars that asmbl
**		will use for input, it must be '\0' terminated without a '\n'.
**
** This routine is the second one called by "asmbl". It alphabeticaly searches
** for a matching ascii .opcode template in ops[] that matches (lptr), which
** is assumed to be previously formatted from "asmfmt".
**
** asfind will return a pointer to the found indexed item of ops[], which is
** an array of structures "op". A negative value represents an error condition.
** To bump "base" to the next binary address, call "getnext" with the index
** returned from "asmbl".
**
*/

asfind ()
{
	int	opnum;

	/* compare for matching templates */
	opcode= 0;
	for (opnum=0; ops[opnum].opc; opnum++) {
		if (ops[opnum].opc & 0200)	opcode++;

		/* opcode field */
		if (values[0] != opcode)	continue;

		/* operand(s) field */
		if (!fmatch(opnum,args=0))	continue;
		if (!fmatch(opnum,args=1))	continue;
		if (!fmatch(opnum,args=2))	continue;
		if (!fmatch(opnum,args=3))	continue;
		return (opnum);
	}
	error=40;
	opcode=0;
	return (0);
}

swinit (opnum,field)
int	opnum,field;
{
	opsa= ops[opnum].opa[field];
	fldtype= opsa & 0xFF00;
	bitoff=  opsa & 0x00FF;
	offset= bitoff / 10;
	bitnum= bitoff - (offset * 10);
	ttype= types[field+1];
	tvalue= values[field+1];
	ivalue= (int)tvalue;
}

/*
**	dumpit(inwhat)
**	char	*inwhat;
**	{
**		printf ("in %s #%d: ttype=%04x, ",inwhat,args,ttype);
**		printf ("tvalue=%04lx,\t",tvalue);
**		printf ("opsa=%04x,\t",opsa);
**		printf ("offset=%04x,\t",offset);
**		printf ("bitnum=%04x,\t",bitnum);
**		printf ("\n");
**	}
*/

fmatch (opnum,field)
int	opnum,field;
{
	swinit (opnum,field);
	field++;
	if (opsa == 0 && fields[field] == 0)	return(1);
	if (opsa == 0 && fields[field] != 0)	return(0);

	switch (fldtype)	{	/* in fmatch */
	case VI:
		if (fields[field] == 0)	return(1);
	case RB:
	case RR:
	case RL:
	case RQ:
	case AA:
	case SR:
	case UR:
		if (ttype == fldtype)	return (1);
		break;
	case B1:
	case B2:
	case B3:
	case B4:
	case X1:
	case X2:
	case X4:
	case P1:
	case P2:
	case P3:
	case N1:
	case N2:
	case N3:
	case ID:
		if (ttype == 00)	return(1);	break;
	case BN:
		if (ttype==0 || fields[field]==0)	return(1);
		break;
	case D1:
	case D2:
	case D3:
	case D4:
	case D5:
		if (ttype == AA)	return(1);
		break;
	case PP:
		if (ttype == PR || ttype == PL)	return(1);
	case PR:
	case PL:
		if (tvalue && (ttype == PR || ttype == PL))	return(1);
		break;
	case BB:
		if (ttype == RR || ttype == RL)	return(1);
		break;
	case BR:
		if (tvalue && (ttype == RR || ttype == RL))	return(1);
		break;
	case II:
	case IR:
	case IL:
		if (ttype==IR || ttype==IL || ttype==II)	return(1);
		break;
	case CF:
		if (fields[field] == 0)	return(1);
		if (tvalue>=7 && tvalue<=4 && ttype==CC)	return(1);
	case CC:
		if (ttype == CC)	return(1);	break;
	case XBWD:
		if (ivalue & 0xFF00)	break;
		if (ttype == 00)	return(1);
		break;
	default:
		error=41;	break;
	}
	return (0);
}

/*

********	asbin (base,opnum)
** W	base=	a pointer to memory of where to put the produced binary, it is
**		a pointer to unsigned characters.
** R	opnum=	an index within "ops[]", which is an array of structures "op"
**		That has all of the necessary template data for the opcode.
**		opnum is the value returned by asfind (or difind).
**
** This is the third subroutine called by "asmbl".
** This subroutine encodes the opcode and binary operands into memory at
** "base", and does so in two passes: The first pass loads the binary
** opcode (given as the index "opnum") over the memory, the second pass
** overlays (over or after) the encoded operands (given in values[]).
**
** This function returns no arguments, and sets no (user) errors.
**
*/

asbin (base,opnum)
POINTER	base;
int	opnum;
{
	POINTER	boff;
	int	opbytes;
	char	disp;

	/* put opcode into memory */
	boff= base;
	opbytes= ops[opnum].opc & 07;
	for (offset=0; offset < opbytes; offset++)	{
		stchr (boff++,(BYTE)ops[opnum].opb[offset]);
	}

	/* put (mask) operands into memory */
	for (args= 0; args < MAXA; args++)	{
		swinit (opnum,args);
		if (!opsa)	break;
		boff= base + offset;

		switch (fldtype)	{	/* in asbin */
		case B1:
		case B2:
			break;
		case X1:
			stchr(boff,(BYTE)tvalue);	break;
		case X2:
			stint(boff,(WORD)ivalue);	break;
		case X4:
			stint(boff,(WORD)(tvalue >> 16));
			stint(boff+2,(WORD)tvalue);	break;
		case D1:
			ivalue= (((int)base+2 - ivalue) >> 1) & 0x7F;
			stchr(boff,(BYTE)((ldchr(boff) & 0x80) | ivalue));
			break;
		case D2:
			disp= ((ivalue - ((int)base+2)) >> 1) & 0xFF;
			stchr (boff,(BYTE)disp);	break;
		case D3:
			ivalue= (((int)base+2 - ivalue) >> 1) & 0x0FFF;
			stint(boff,(WORD)(ldint(boff) & 0xF000) | ivalue);
			break;
		case D4:
		case D5:
		case AA:
			stint(boff,(WORD)ivalue);	break;
		case VI:
			stchr(boff,(BYTE)(ldchr(boff) | ivalue));	break;
		case ID:
			ivalue--;
			if (ivalue <= 0)	ivalue=0;
		case RB:
		case RR:
		case RL:
		case RQ:
		case B4:
		case II:
		case IR:
		case IL:
		case PP:
		case PR:
		case PL:
		case BB:
		case BR:
		case BL:
		case CC:
		case SR:
			ivalue= (ivalue & 0x000F) << (4-bitnum);
			stchr(boff,(BYTE)ldchr(boff) | ivalue);
			break;
		case UR:
			break;
		case CF:
			if (ivalue == 0)	break;
			stchr(boff,(BYTE)(ldchr(boff) | (1 << ivalue)));
			break;
		case BN:
			ivalue= (ivalue << 1) & 0x2;
			stchr(boff,(BYTE)(ldchr(boff) | ivalue));
			break;
		case N1:
		case N3:
			ivalue= -ivalue;
		case P1:
		case P3:
			stint(boff,(WORD)ivalue);
			break;
		case N2:
			ivalue= -ivalue;
		case P2:
			stchr(boff,(BYTE)ivalue);
			break;
		default:
			error=42;	break;
		}
		if (error)	break;
	}
}

/*

************************************************************************
**
********	deasm (base,lptr)	- standard call to the disassembler
** R	base=	a pointer to memory of where to disassemble binary, it is
**		a pointer to unsigned characters.
** W	lptr=	is a pointer to a host provided buffer < 40 chars that deasm
**		will use for output, it will be '\0' terminated without a '\n'.
**
** This function only dispatches to the following disassembler subroutines:
** first:	"difind" to scan for an ops[] index for a matching binary
**		instruction template at "base".
** second:	"disbin" to encode the binary half of the matched opcode
**		string into "iscan" (ex "1234: 56 78 9A ").
** third:	"disop" to encode and concatenate the nmeumonic half of
**		the matched opcode after "iscan" (ex "ld a,b").
**
** deasm will return a pointer to the found indexed item of ops[], which is
** an array of structures "op". Error conditions can only return as ".byte ~"
** To bump "base" to the next binary address, call "getnext" with the index
** returned from "deasm".
**
**
*/

deasm (base,lptr)
POINTER	base;
char	*lptr;
{
	int	opnum;
	error=0;
	opnum= difind (base);			/* score and match binary */
	if (error)	return (0);

	disbin (base,getnext(opnum),lptr);	/* encode the binary half */
	if (error)	return (0);

	while (*lptr != '\0')	lptr++;

	disop (base,opnum,lptr);		/* encode the nmemonic half */
	if (error)	return (0);

	return (opnum);
}

/*

********	difind (base)	- search for a binary match for disassembler
** R	base=	a pointer to memory of where to disassemble binary, it is
**		a pointer to unsigned characters.
**
** This function scans all of the templates on ops[] and by using the
** template control words, searches for a match. If none are found, then
** a zero is returned, which in "ops[]" is a pointer to a ".byte" opcode.
** no error codes are returned, except 0 for not found.
**
*/

difind (base)
POINTER	base;
{
	BYTE	m[MAXF],b[MAXF];
	int	opnum;
	int	opbytes;

	opcode= 0;
	for (opnum=1; ops[opnum].opc; opnum++)	{
		if (ops[opnum].opc & 0200)	opcode++;
		opbytes= (ops[opnum].opc >> 3) & 07;

		for (args=0; args < MAXA; args++)	{
			m[args]= ldchr (base+args);
			b[args]= ops[opnum].opb[args];
		}
		for (args=0; args < MAXA; args++)	{
			swinit (opnum,args);
			if (!opsa)	break;

			switch (fldtype)	{	/* in difind */
			case X4:
				m[offset+3]= 00;
				m[offset+2]= 00;
			case X2:
				m[offset+1]= 00;
			case X1:
				m[offset]= 00;
				break;
			case II:
			case IR:
			case IL:
			case BB:
			case BR:
			case BL:
			case PP:
			case PR:
			case PL:
				if ((bitnum==0) && !(m[offset] & 0xF0))
					{ opbytes= 0;	break; }
				if ((bitnum==4) && !(m[offset] & 0x0F))
					{ opbytes= 0;	break; }
			case I0:
			case P0:
			case B4:
			case RB:
			case RR:
			case RL:
			case RQ:
			case CC:
			case CF:
			case ID:
				if (bitnum == 0) m[offset]&= 0x0F;
				if (bitnum == 4) m[offset]&= 0xF0;
				break;
			case SR:
				m[offset]&= 0xF8;
			case UR:
				break;
			case D1:
				m[offset]&= 0x80;	break;
			case D2:
				m[offset]= 00;	break;
			case D3:
				m[offset]&= 0xF0; m[offset+1]= 0;	break;
			case D4:
			case D5:
				m[offset]= m[offset+1]= 0;	break;
			case AA:
				if (bitnum == 0) m[offset]= m[offset+1]= 00;
				break;
			case VI:
				m[offset] &= 0xFC;
				break;
			case BN:
				m[offset] &= 0xFD;
				break;
			case N1:
			case N3:
				if ((m[offset] & 0x80) == 0)	opbytes=0;
				m[offset]= 0;	m[offset+1]= 0;
				break;
			case N2:
				if ((m[offset] & 0x80) == 0)	opbytes=0;
				m[offset]= 0;
				break;
			case P1:
			case P3:
				if ((m[offset] & 0x80) != 0)	opbytes=0;
				m[offset]= 0;	m[offset+1]= 0;
				break;
			case P2:
				if ((m[offset] & 0x80) != 0)	opbytes=0;
				m[offset]= 0;
				break;
			default:
				error=43;
				break;
			}
		}

		if (!opbytes)		continue;
		if (m[0] != b[0])	continue;
		if (opbytes == 1)	return (opnum);
		if (m[1] != b[1])	continue;
		if (opbytes == 2)	return (opnum);
		if (m[2] != b[2])	continue;
		if (opbytes == 3)	return (opnum);
		if (m[3] != b[3])	continue;
		if (opbytes == 4)	return (opnum);
	}
	opcode=opnum= 0;
	return (opnum);
}

/*

********	disbin (base,opnum,lptr)	- encode binary half into ascii
** R	base=	a pointer to memory of where to disassemble binary, it is
**		a pointer to unsigned characters.
** R	opnum=	an index within "ops[]", which is an array of structures "op"
**		That has all of the necessary template data for the opcode.
**		opnum is the value returned by difind (or asfind).
** W	lptr=	is a pointer to a host provided buffer < 40 chars that deasm
**		will use for output, it will be '\0' terminated without a '\n'.
**
** This subroutine computes the length of the opcode, and encodes a string
** of the base value, ':', tab, and N bytes representing the length.
** The string in encoded into "lptr", and is '\0' terminated without a '\n'.
** (ex if base=1234, and base=00, lptr will return "1234: 00").
** No error conditions returned.
**
*/

disbin (base,nbytes,lptr)
POINTER	base;
int	nbytes;
char	*lptr;
{
	int	i;
	base&= -2;	/* round it */
	sprintf (lptr,"%08lx: ",(POINTER)base);
	for (i= 0; i < 6; i+= 2)	{
		if (i < nbytes)	{
			sprintf (lptr,"%s%04x ",lptr,ldint(base) & 0xFFFF);
			base+= 2;
		}
		else		{
			sprintf (lptr,"%s     ",lptr);
		}
	}
	sprintf (lptr,"%s\t",lptr);
}

/*

********	disop (base,opnum,lptr)	- encode nmemonic half into ascii
** R	base=	a pointer to memory of where to disassemble binary, it is
**		a pointer to unsigned characters.
** R	opnum=	an index within "ops[]", which is an array of structures "op"
**		That has all of the necessary template data for the opcode.
**		opnum is the value returned by difind (or asfind).
** W	lptr=	is a pointer to a host provided buffer < 40 chars that deasm
**		will use for output, it will be '\0' terminated without a '\n'.
**
** This subroutine computes the length of the opcode, and encodes a string
** of the nmeumonic for ops[opnum].
** The string in encoded into "lptr", and is '\0' terminated without a '\n'.
** No error conditions returned.
** (ex if base=1234, and base=00, lptr will return "nop").
**
*/

disop (base,opnum,lptr)
POINTER	base;
int	 opnum;
char	*lptr;
{
	POINTER	boff;
	int	field,fldctr;
	int	topnum;

	opcode= 0;
	for (topnum=1; (topnum <= opnum) && ops[topnum].opc; topnum++)
		if (ops[topnum].opc & 0200)	opcode++;

	fldctr=0;
	*lptr= '\0';
	sprintf (lptr,"%s\t",opcodes[opcode]);
	for (args= 0; args < MAXA; args++)	{
		swinit (opnum,args);
		if (!opsa)	break;
		if (args && fldtype != PR && fldtype != D4 && fldtype != CF)
			sprintf (lptr,"%s,",lptr);
		boff= base + offset;

		switch (fldtype)	{	/* in disop */
		case XBWD:
		case XBOF:
		case B3:
			tvalue= (ldchr(boff) >> (5-bitnum)) & 0x07;
			sprintf (lptr,"%s%01x",lptr,(int)tvalue);
			break;
		case ID:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0F;
			sprintf (lptr,"%s#%x",lptr,(int)tvalue+1);
			break;
		case B4:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0F;
			sprintf (lptr,"%s#%01x",lptr,(int)tvalue);
			break;
		case X1:
			tvalue= ldchr(boff) & 0xFF;
			sprintf (lptr,"%s#%03x",lptr,(int)tvalue); break;
		case X2:
			tvalue= ldint(boff) & 0xFFFF;
			sprintf (lptr,"%s#%05x",lptr,(int)tvalue); break;
		case X4:
			tvalue= ldint(boff);
			sprintf (lptr,"%s #%05x",lptr,(int)tvalue);
			tvalue= ldint(boff+2);
			sprintf (lptr,"%s%04x",lptr,(int)tvalue); break;
		case IR:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0F;
			field= typfind(II,(int)tvalue);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case RB:
		case RR:
		case II:
		case PP:
		case PR:
		case CC:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0F;
			field= typfind(fldtype,(int)tvalue);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case CF:
			ivalue= 7-args;
			if ((ldchr(boff) & (1 << ivalue)) == 0)	break;
			field= typfind(CC,ivalue);
			if (fldctr++)	sprintf (lptr,"%s,",lptr);
			sprintf(lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case BB:
		case BR:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0F;
			field= typfind(RR,(int)tvalue);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case SR:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x07;
			field= typfind(fldtype,(int)tvalue);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case UR:
			field= typfind(fldtype,1);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case RQ:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0C;
			field= typfind(fldtype,(int)tvalue);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case RL:
		case IL:
		case PL:
			tvalue= (ldchr(boff) >> (4-bitnum)) & 0x0E;
			field= typfind(fldtype,(int)tvalue);
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case D1:
			ivalue= (int)base + 2 - 2 * (ldchr(boff) & 0x7F);
			sprintf (lptr,"%s%05x",lptr,ivalue);	break;
		case D2:
			ivalue= (int)base + 2 + 2 * (ldchr(boff) & 0xFF);
			sprintf (lptr,"%s%05x",lptr,ivalue);	break;
		case D3:
			ivalue= (int)base + 2 - 2 * (ldint(boff) & 0xFFF);
			sprintf (lptr,"%s%05x",lptr,ivalue & 0xFFF);	break;
		case D4:
			ivalue= (ldint(boff) & 0xFFFF);
			sprintf (lptr,"%s(#%05x)",lptr,ivalue); break;
		case D5:
		case AA:
			ivalue= (ldint(boff) & 0xFFFF);
			sprintf (lptr,"%s%05x",lptr,ivalue); break;
		case VI:
			if (args)	break;
			field= typfind(fldtype,(int)(ldchr(boff) & 0x03));
			sprintf (lptr,"%s%s",lptr,oprands[field].uname);
			break;
		case BN:
			ivalue= ((ldchr(boff) >> 1) & 0x01) + 1;
			sprintf (lptr,"%s#%01x",lptr,ivalue);
			break;
		case P1:
		case P3:
			ivalue= ldint(boff) & 0x1F;
			sprintf (lptr,"%s#%d",lptr,ivalue);
			break;
		case P2:
			ivalue= ldchr(boff) & 0x7;
			sprintf (lptr,"%s#%d",lptr,ivalue);
			break;
		case N1:
		case N3:
			ivalue= -ldint(boff) & 0x1F;
			sprintf (lptr,"%s#%d",lptr,ivalue);
			break;
		case N2:
			ivalue= -ldchr(boff) & 0x1F;
			sprintf (lptr,"%s#%d",lptr,ivalue);
			break;
		default:
			error=44;	break;
		}
		if (error)	break;
		types[args+1]= fldtype;
		values[args+1]= tvalue;
	}
}


/*

************************************************************************
**
********	getnext (opnum)	- compute length of opcode in bytes.
** R	opnum=	an index within "ops[]", which is an array of structures "op"
**		That has all of the necessary template data for the opcode.
**		opnum is the value returned by difind (or asfind).
**
** This subroutine computes the length of the opcode, if opnum=0 indicating
** an unknown opcode, the return vaule is a length of 1.
** If you have an opnum from either "asfind" or "difind", you can bump the
** base to point to the next opcode by typing base+= getnext(opnum).
**
*/

getnext(opnum)
int	opnum;
{
	return (ops[opnum].opc & 07);
}

/*

********	symfind (sym)	- find the oprands[] index for a label
**
*/

symfind(sym)
char	*sym;
{
	int	i;
	for (i= 1; *oprands[i].uname != '\0'; i++)	{
		if (!scmp(sym,oprands[i].uname))	return (i);
	}
	return (0);
}

/*
********	typfind (v)	- find the oprands[] index for a type
**
*/

typfind (t,v)
int	t,v;
{
	int	i;
	for (i= 1; *oprands[i].uname != '\0'; i++)	{
		if (oprands[i].utype != t)	continue;
		if (oprands[i].uvalue != v)	continue;
		return (i);
	}
	return (0);
}

/********	END	********/

