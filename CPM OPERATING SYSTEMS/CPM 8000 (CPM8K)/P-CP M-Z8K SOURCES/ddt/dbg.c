/*
**
**
**
**
**
**
**
**
**		  Zilog Portable Debugger
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
** program name:	Zilog portable debugger
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

#ifndef	CPM
#include	<stdio.h>
#endif

#include	"env.h"
#include	"u.h"

/*
**	*************************************************
**	*						*
**	*	P R E P R O C E S S   C O N S T A N T S	*
**	*						*
**	*************************************************
**
*/

#define	SYMMAX	300
#define	ARGMAX	5
#define	MAXBP	20
#define	DSEG	0x9000000L
#define	SSEG	0xA000000L

#define	LADDR	(MEMSIZE-0x10)

/*
**	*************************************************
**	*						*
**	*	G L O B A L S   /   E X T E R N A L S	*
**	*						*
**	*************************************************
**
*/

/* globals to other modules */
extern	int	tractr;
extern	POINTER	temppc;
extern	union pstate	frame;

int	tractr;
int	sstctr;
POINTER	temppc;
union pstate	frame;

/* common symbols */
POINTER	aptr,dptr,iptr,lptr,sptr;
POINTER	arg[ARGMAX+1];
int	dargs;
WORD	fbits;
POINTER	setmem();

struct	symbol	symtbl[SYMMAX];

int	targc;
char	**targv;

/*
********	debugger flags
*/

struct	bitable	dbgbits[]	={
	'l',	LSTEP,
	'm',	MUFOM,
	't',	TTRACE,
	'\0',	0x0000,
};

/*
**

**	*************************************************
**	*						*
**	*	S T A R T   O F   C O D E		*
**	*						*
**	*************************************************
**
*/

/*
********	main
*/

main (argc,argv)
int	argc;
char	**argv;
{
	POINTER	addr;
	int	diopn;

#ifdef	CPM
	extern	long	getsp();
	extern	int	bkptrap();
	struct bios_parm sclink;

	if (cflag++)	goto noreloc;

	targc= argc;
	targv= argv;

	mem_cpy (map_adr(0L,1),DSEG,0xFFFFL);	 /* copy to segment 9 */

	frame.lregs.fcw= 0x1800;		/* ei VI,NVI */
	frame.lregs.pcs= (WORD)(DSEG >> 16);	/* new pc= current pc in seg9 */
	frame.lregs.pc= (WORD)main;
	frame.lregs.re= (WORD)(getsp() >> 16);	/* new sp= current sp */
	frame.lregs.rf= (WORD)(getsp() + 2);
	xfer (map_adr((LONG) &frame & 0xFFFFL,257));	/* ZAP! relocate */

noreloc:
	argc= targc;
	argv= targv;

#endif

	printf ("Zilog Portable debugger, @1982 Zilog, TRS\n");
	symtbl[SYMMAX-1].utype= LASTSYM;
	addsym ("$",1,(MDASIZE)0);

	while (--argc && (*++argv)[0] == '-')
		for (iscan= argv[0]+1; *iscan != '\0'; iscan++)
			switch (*iscan)	{
			case 'j':
				jflag++;
				if (ishex(*(iscan+1)) < 0)	break;
				iscan++;
				tlong= hexin(8);
				frame.lregs.pc= (WORD)PSWP((WORD)(tlong));
#ifdef	Z8001
				frame.lregs.pcs= (WORD)(tlong >> 16);
#endif
				iscan--;
				break;
			case 'm':
				yflag^= MUFOM;
				break;
			case 'x':
				if (ishex(*(iscan+1)) < 0)	break;
				iscan++;
				xflag= (int)hexin(4);
				iscan--;
				break;
			case 'y':
				if (ishex(*(iscan+1)) < 0)	break;
				iscan++;
				yflag= (int)hexin(4);
				iscan--;
				break;
			default:
				error=03;
			}
	err();
	while (argc-- && ((*argv)[0] != '-')) { inload(&frame,*argv++);	err(); }

	dbginit (&frame);
	err();

#ifdef	CPM
	sclink.req= 22;
	sclink.p1=  32L;
	sclink.p2=  map_adr((LONG)bkptrap & 0xFFFFL,257);
	_bios_call (&sclink);		/* link sc 0 to bkpt to _bkpt */

	frame.lregs.fcw= 0x1800;		/* ei VI,NVI */
	frame.lregs.pcs= (WORD)(SSEG >> 16);	/* init user pc= 0A00 0000 */
/*
**	frame.lregs.pc= (WORD)PSWP(0x0000);
*/

	frame.lregs.re= (WORD)(SSEG >> 16);	/* init user sp= 0A00 FDFC */
	frame.lregs.rf= (WORD)PSWP(0xFDFC);

	if (!jflag)	setbkpt ((POINTER)0x0A000000,(WORD)1);

	xfer (map_adr((LONG) &frame & 0xFFFF,257));

#else		/* assumed ILS */
dbgloop:
	if (!jflag)	tractr= dbg(&frame);
	while (!tractr)	{	/* run until trap */
		addr= PSWP((WORD)frame.lregs.pc) & 0xFFFFL;
		if (tstrap(addr))	break;
		diopn= sstep(&frame);
		sstctr++;
		if (error)	break;
		if (!diopn)	break;
		if (yflag & TTRACE)	diopn= sstate(&frame);
	}
	while (tractr--)	sstep(&frame);
	goto dbgloop;
#endif
}

/*
********	Breakpoint Entry
*/

dbg (state)
union	pstate	*state;
{
	MDASIZE	accu;
	POINTER	addr;
	POINTER	bkptadr;
	WORD	bkptctr;
	char	cmd,cmd1;
	int	diopn,mmode;

	addr= PSWP((WORD)state->lregs.pc) & 0xFFFFL;
#ifdef	Z8001
	addr|= ((LONG)state->lregs.pcs << 16);
	if (state->lregs.id == BKPT)	{
		addr-= 2;
		state->lregs.pc= (WORD)addr;
		clrbkpt(addr);
	}
#else
	if (tstrap(addr))	clrbkpt(addr);
#endif
	error= 0;

	if (yflag & LSTEP)	{
		state->lregs.pc= PSWP((WORD)temppc);
#ifdef	Z8001
		state->lregs.pcs= (WORD)(temppc >> 16);
#endif
		yflag&= ~(LSTEP);
	}

	diopn= sstate (state);

cmdline:
	error=0;
	printf("> ");
	iscan= gets(iline);
	if (iscan == NULL)	exit(0);
/*
**	uctolc (iscan);
*/
	tscan= iscan;
cmdloop:
	cmd= tolower(*iscan++);
	cmd1= tolower(*iscan);

	mmode= 8;	/* default memory mode (8-bits wide) */
	if (cmd == 'd' || cmd == 'f' || cmd == 's')	{
		if (cmd1 == 's')	{ mmode=  8;	iscan++; }
		if (cmd1 == 'w')	{ mmode= 16;	iscan++; }
/*
**		if (cmd1 == 'l')	{ mmode= 32;	iscan++; }
**		if (cmd1 == 'q')	{ mmode= 64;	iscan++; }
*/
	}
	if (cmd == 'p' && cmd1 == 'w')	{ mmode= 16;	iscan++; }

	itrim();		/* set *iscan to first arg field */
	switch(cmd) {
	case '\0':	/* null line (currently does nothing) */
	case '\n':
		break;
	case '?':	/* help facility */
		printf ("?\t\t\t\tHelp\n");
		printf ("*\t\t\t\tRemark\n");
		printf ("$ [symbol=] expr\t\tSymbolic Assignment\n");
		printf ("@ address, instr\t\tSingle patch\n");
		printf ("^ instr\t\t\t\tExecute in local mode\n");
		printf ("a from\t\t\t\tAssemble source code\n");
		printf ("b [addr][,addr]...\t\tSet/Display Breakpoint(s)\n");
		printf ("c [addr][,addr]...\t\tClear (All) Breakpoint(s)\n");
		printf ("dx [from [,to] || [,,count]]\tDump memory\n");
		printf ("fx from,to,word\t\t\tFill memory\n");
		printf ("i filename\t\t\tInload (overlay) a file\n");
		printf ("j [expr]\t\t\tJump to user code (execute)\n");
		printf ("l [from [,to] || [,,count]]\tList (disam) \n");
		printf ("m from,to,bytes\t\t\tMove Memory\n");
		printf ("n [count]\t\t\tSingle Step (next)\n");
		printf ("p port [,data]\t\t\tPort read [write] data\n");
		printf ("r [x=expr] [x=expr]...\t\tSet/Disp regs(s)\n");
		printf ("sx [addr]\t\t\tSet memory\n");
		printf ("x [flags]\t\t\tflip FCW status bits\n");
		break;

	case '*':	/* remark */
		break;

/*
**	case ':':
**		iscan--;
**		inline();
**		break;
*/

	case '$':	/* symbol = expr	Set/display a symbol */
		if (eolchr(*iscan))	{ dspsyms();	break; }
		if (*iscan == '=')	{ error=22;	break; }
		if (symchr(*iscan) && !(*iscan >= '0' && *iscan <= '9'))
			skipsym();
		if (error)	break;
		if (*iscan == '=')	{
			iscan++;
			accu= infixer();
			if (error)	break;
			iscan= &iline[1];
			itrim();
			setsym (1,accu);
			break;
		}
		iscan= &iline[1];
		itrim();
		accu= infixer();
		if (error)	break;
		symtbl[1].uvalue= accu;
		printf ("%ld\t",accu);
		printf ("0%lx\t",accu);
		printf (":%lo\n",accu);
		break;

	case '@':	/* @addr instr	assemble instr into address (in user space) */
		tptr= infixer();
		if (error)	break;
		if (eolchr(*iscan))	error= 79;
		if (error)	break;
		diopn= asdis(tptr,iscan);
		break;

	case ' ':
	case '^':	/* ^ instr	assemble instr into local space and Sstep */
		diopn= asdis((POINTER)LADDR,iscan);
		if (error)	break;
		temppc= PSWP(state->lregs.pc) & 0xFFFFL;
		state->lregs.pc= PSWP((WORD)LADDR);
#ifdef	Z8001
		temppc|= ((LONG)state->lregs.pcs << 16);
		state->lregs.pcs= (WORD)(LADDR >> 16);
#endif
		yflag|= LSTEP;
		return (1);

	case 'a':	/* A [from]	assemble multiple instructions into "from"++ */
		getargs();
		if (error)	break;
		if (dargs)	aptr= arg[1];
		while (1) {
			error=0;
			adrout(aptr);
			iscan= gets(iline);
			if (eolchr(*iscan))	break;
			diopn= asdis(aptr,iscan);
			if (error)	{ err();	continue; }
			aptr += getnext(diopn);
		}
		break;

	case 'b':	/* B [addr][,addr] ...	breakpoint set/display */
		while (!eolchr(*iscan)) {
			bkptctr= 1;
			bkptadr= infixer();
			if (*iscan == ':')	{
				iscan++;
				error=0;
				bkptctr= (WORD)infixer();
			}
			if (error)	break;
			if (!delchr(*iscan))	error=70;
			if (error)	break;
			setbkpt ((POINTER)bkptadr,(WORD)bkptctr);
			if (error)	break;
			if (*iscan != ',')	break;
			iscan++;
		}
		dispbkpt();
		break;

	case 'c':	/* C [addr][,addr] ...	breakpoint clear/clear all */
		if (eolchr(*iscan))	nobkpts();
		while (!eolchr(*iscan)) {
			bkptadr= infixer();
			if (error)	break;
			if (!delchr(*iscan))	error=70;
			if (error)	break;
			clrbkpt ((POINTER)bkptadr);
			if (error)	break;
			if (*iscan != ',')	break;
			iscan++;
		}
		dispbkpt();
		break;

	case 'd':	/* D [from][to][count]	dump memory in hexadecimal and ascii */
		getargs();
		if (error)	break;
		if (dargs == 0)	arg[1]= dptr;
		if (arg[3] == 0)	arg[3]= 0x100;
		if (arg[2] == 0)	arg[2]= arg[1]+arg[3];
		if (arg[4] == 0)	arg[4]= 16;
		if (arg[1] > arg[2])	{ error=65; break; }
		dmpmem (arg[1],arg[2],mmode,(WORD)arg[4]);
		dptr= arg[2];
		break;

	case 'f':	/* F from,to,byte	fill memory with constant data */
		getargs();
		if (error)	break;
		if (dargs != 3)	{
			error=63;
			break;
		}
		if (arg[1] >= arg[2])	{
			error=66;
			break;
		}
		while (arg[1] < arg[2])	{
			if (mmode ==  8)	{
				stchr(arg[1],(BYTE)arg[3]);
				arg[1]+= 1;
			}
			if (mmode == 16)	{
				stint(arg[1],(WORD)arg[3]);
				arg[1]+= 2;
			}
		}
		break;

	case 'g':	/* G addr [bkpts]	begin execution (load PC & jump) */
	case 'j':	/* J addr [bkpts]	begin execution (load PC & jump) */
		getargs();
		if (error)	break;
		if (!arg[1])	return (0);
		state->lregs.pc= (WORD)(arg[1]);
#ifdef	z8001
		state->lregs.pcs= (WORD)(arg[1] >> 16);
#endif
		return (0);

	case 'i':	/* I filename		build a file control block for IO */
		inload (state,iscan);
		break;

	case 'l':	/* L [from][to][count]	list memory using disassembler */
		getargs();
		if (error)	break;
		if (dargs == 0)	arg[1]= lptr;
		if (arg[3] == 0)	arg[3]= 0x10;
		if (arg[2] == 0)	arg[2]= arg[1]+arg[3];
		if (arg[1] > arg[2])	{
			error=65;
			break;
		}
		for (lptr= arg[1]; lptr <= arg[2];) {
			memode= CODE;
			diopn= deasm(lptr,oline);
			memode= DATA;
			printf ("%s\n",oline);
			lptr += getnext(diopn);
		}
		break;

	case 'm':	/* Mx From,to,count	Move memory */
		getargs();
		if (error)	break;
		if (dargs != 3)	{
			error=63;
			break;
		}
		if (arg[1] == arg[2])	{
			error=67;
			break;
		}
		movmem(arg[1],arg[2],arg[3]);
		break;
	case 'n':	/* N count	single step n times */
		getargs();
		if (error)	break;
		if (!dargs)	arg[1]= 1;
		return (arg[1]);

	case 'p':	/* P port,address	read/write io port */
		getargs();
		if (error)	break;
		if (dargs == 1)	{
			adrout (sptr);
			printf ("%02x\n",CHR(pichr(arg[1])));
		}
		if (dargs == 2)	{
			pochr(arg[1],(BYTE)arg[2]);
		}
		break;

	case 'r':	/* R [reg=value]	register set/display */
		if (!eolchr(*iscan))	setregs(state);
		if (error)	break;
		diopn= sstate(state);
		break;

	case 's':	/* S [addr]		set memory */
		if (eolchr(*iscan))	goto nextsl;
		if (delchr(*iscan))	goto nextsf;
		arg[1]= infixer();
		if (error)	break;
		sptr= arg[1];
		if (eolchr(*iscan))	goto nextsl;
		if (delchr(*iscan))	goto nextsf;
nextsl:	/* process next line */
		adrout (sptr);
		if (mmode ==  8)	{ printf ("%02x ",CHR(ldchr(sptr))); }
		if (mmode == 16)	{ printf ("%04x ",SHT(ldint(sptr))); }
		iscan= gets(iline);
		if (eolchr(*iscan))	break;
		if (spcchr(*iscan) || *iscan == '.')	{
			if (mmode ==  8)	sptr+= 1;
			if (mmode == 16)	sptr+= 2;
			goto nextsl;
		}
		sptr= setmem(sptr,mmode);	/* set memory from the line */
		if (error)	break;
		goto nextsl;

nextsf:	/* process next field */
		if (eolchr(*iscan))	break;
		if (delchr(*iscan))	iscan++;
		sptr= setmem(sptr,mmode);
		if (error)	break;
		goto nextsf;

	case 'x':	/* X [bit]	set fcw bits nmemonicaly */
		fbits= (state->sregs.f1 << 8) & 0xFF00;
		fbits|= (state->sregs.f & 0xFF);
		fbits= sflags(fbits,pflags);
		state->sregs.f1= (fbits >> 8) & 0xFF;
		state->sregs.f= (fbits) & 0xFF;
		printf("%04x: ",SHT(fbits));
		dflags(fbits,pflags);
		printf ("\n");
		break;

	case 'y':	/* Y [bit]	set dbgbits nmemonically */
		yflag= sflags((WORD)yflag,dbgbits);
		printf("%04x: ",SHT(yflag));
		dflags((WORD)yflag,dbgbits);
		printf ("\n");
		break;

	case 'z':
		printf ("ssteps= %d\n",sstctr);
		sstctr= 0;
		break;

	default:
		error=50;
	}
	err();
	while (!eolchr(*tscan))	tscan++;
	if (*tscan == ';')	{ tscan++;	iscan= tscan;	goto cmdloop; }

	goto cmdline;
}

/*
********	get arguments utility
*/

getargs()
{
	for (dargs=0; dargs < ARGMAX; dargs++)	arg[dargs]= 0;
	dargs=0;
	if (eolchr(*iscan))	return;
	for (dargs=1; (dargs < ARGMAX); dargs++)	{
		arg[dargs]= infixer();
		if (error)	break;
		if (eolchr(*iscan))	break;
		if (delchr(*iscan))	{ iscan++;	continue; }
		error=50+dargs;
		break;
	}
}

/*
********	dump memory utility
*/

dmpmem (from,to,mmode,width)
POINTER	from,to;
int	mmode;
WORD	width;
{
	char	c;

	tptr= from;	/* tptr= base address for current line */
	while (from < to)	{
		if (tptr == from)	adrout (from);
		if (mmode == 8)	{
			printf ("%02x ",CHR(ldchr(from)));	from+= 1;
		}
		if (mmode == 16)	{
			printf ("%04x ",SHT(ldint(from)));	from+= 2;
		}
		if ((from-tptr < width) && (from < to))	continue;
		printf (" ");
		while (tptr < from)	{
			c= ldchr(tptr++);
			if (c < ' ')	c='.';
			printf ("%c",c);
		}
		printf ("\n");
	}
}

POINTER	setmem (from,mmode)
POINTER	from;
{
	MDASIZE	accu;
	while (!eolchr(*iscan))	{
		while (spcchr(*iscan) || *iscan == ',')	iscan++;
		if (*iscan == '"')	{
			iscan++;
			while (!eolchr(*iscan))	{
				if (*iscan == '\"')	break;
				stchr(from++,(BYTE)*iscan++);
			}
			if (*iscan == '"')	iscan++;
			continue;
		}
		accu= infixer();
		if (error)	break;
		if (mmode ==  8)	{ stchr (from,(BYTE)accu); from+= 1; }
		if (mmode == 16)	{ stint (from,(WORD)accu); from+= 2; }
	}
	return (from);
}

/*
********	move memory utility
*/

movmem (from,to,count)
POINTER	from,to,count;
{
	if (from > to)	{
		while (count--)	stchr(to++,ldchr(from++));
		return;
	}
	if (from < to)	{
		from+= count;
		to+= count;
		while (count--)	stchr(--to,ldchr(--from));
	}
}

/*
*********	breakpoint display / set / clear and find routines
*/

struct	{
	POINTER	bkptadr;
	WORD	bkptdat;
	WORD	bkptctr;
} bkpts[MAXBP];

dispbkpt()
{
	int	bkptc;
	for (bkptc=1; (bkptc < MAXBP); bkptc++)
		if (bkpts[bkptc].bkptctr)	{
			adrout (bkpts[bkptc].bkptadr);
			printf ("%6d\t",bkpts[bkptc].bkptctr);
			printf ("%05x\n",bkpts[bkptc].bkptdat);
		}
}

setbkpt(bkpt,bcount)
POINTER	bkpt;
WORD	bcount;
{
	int	bkptc;
	bkptc= findbkpt(bkpt);
	if (bkptc)	error=71;	/* already set */
	if (error) return;
	if (!bcount)	error=78;	/* zero bkpt count */
	if (error) return;
	for (bkptc=1; bkptc < MAXBP; bkptc++)	{
		if (bkpts[bkptc].bkptctr)	continue;
		bkpts[bkptc].bkptadr= bkpt;
		bkpts[bkptc].bkptdat= (WORD)setrap(bkpt,(WORD)BKPT);
		bkpts[bkptc].bkptctr= (WORD)bcount;
		return;
	}
	error=72;	/* no more space */
}

nobkpts()
{
	int	bkptc;
	for (bkptc=1; bkptc < MAXBP; bkptc++)
		if (bkpts[bkptc].bkptctr)
			clrbkpt(bkpts[bkptc].bkptadr);
}

clrbkpt(bkpt)
POINTER	bkpt;
{
	int	bkptc;
	bkptc= findbkpt(bkpt);
	if (!bkptc)	error=73;	/* no bkpt to clear */
	if (error) return;
	if ((WORD)setrap(bkpt,bkpts[bkptc].bkptdat) != BKPT)	error=74;
	if (error) return;
	bkpts[bkptc].bkptctr= (POINTER)0;
}

findbkpt(bkpt)
POINTER	bkpt;
{
	int	bkptc;
	for (bkptc=1; bkptc < MAXBP; bkptc++)	{
		if (bkpts[bkptc].bkptctr == 0)	continue;	/* invalid */
		if (bkpts[bkptc].bkptadr == bkpt)
			return (bkptc);
	}
	return (0);
}

/*
********	set processor state register
*/

setregs(state)
union	pstate	*state;
{
	POINTER	addr;
	char	*tp,*rp;
	long	rvalue;
	int	i;
	while (!eolchr(*iscan))	{
		for (i=1; ; i++)	{
			rp= regtbl[i].uname;
			tp= iscan;
			if (*rp == '\0')	{ error=75;	return; }
			while (tolower(*rp) == tolower(*tp))	{
				 rp++;	tp++;
			}
			if ((*rp=='\0' || *rp==' ') && *tp=='=')	break;
		}
		iscan= ++tp;
		rvalue= infixer();
		if (error)	return;
		addr= ((POINTER)state & 0xFFFFL) + (POINTER)regtbl[i].uvalue;
		if (regtbl[i].utype & (MDASIZE)RCHR)
			*(BYTE *)addr= (BYTE)rvalue;
		if (regtbl[i].utype & (MDASIZE)RINT)
			*(WORD *)addr= (WORD)PSWP((WORD)rvalue);
		if (regtbl[i].utype & (MDASIZE)RLONG)
			*(LONG *)addr= (LONG)(rvalue);
		if (eolchr(*iscan))	return;
		if (delchr(*iscan))	{ iscan++; continue; }
		error=76;
		return;
	}
}

/*
********	display processor state
*/

sstate(state)
union	pstate	*state;
{
	POINTER	addr;
	int	i,diopn;

	for (i=1; *regtbl[i].uname; i++)	{
		if (regtbl[i].utype & (MDASIZE)RCHR)	continue;
		addr= ((POINTER)state & 0xFFFFL) + (POINTER)regtbl[i].uvalue;
		printf ("%s=",regtbl[i].uname);
		if (regtbl[i].utype & (MDASIZE)RLONG)
			printf ("%08lx",*(POINTER *)addr);
		if (regtbl[i].utype & (MDASIZE)RINT)
			printf ("%04x",PSWP(*(WORD *)addr));
		if (regtbl[i].utype & (MDASIZE)RCR)	printf ("\n");
		else	printf (" ");
	}
	fbits= ((state->sregs.f1 << 8) & 0xFF00) | (state->sregs.f & 0x00FF);
	dflags(fbits,pflags);
	addr= PSWP(state->lregs.pc) & 0xFFFFL;
#ifdef	Z8001
	addr|= ((LONG)state->lregs.pcs << 16);
#endif
	memode= CODE;
	diopn= deasm (addr,oline);
	memode= DATA;
	printf ("\n%s\n",oline);
/*
**	printf ("\t%s\n",&oline[24]);
*/
	return (diopn);
}

/*
********	set mask ^= fcw bits nmemonically with string at *iscan
*/

sflags (mask,table)
WORD	mask;
struct	bitable	table[];
{
	int	loop;
	while (!eolchr(*iscan))	{
		for (loop= 0; table[loop].bitmask; loop++)	{
			if (*iscan != table[loop].bitlabel)	continue;
			mask^= table[loop].bitmask;
			break;
		}
		if (!table[loop].bitmask)	error=77;	/* bad flag */
		iscan++;
	}
	return (mask);
}

/*
********	display fcw bits nmemonically using bitable[]
*/

dflags(mask,table)
WORD	mask;
struct	bitable	table[];
{
	int	loop;
	printf ("\"");
	for (loop= 0; table[loop].bitmask; loop++)	{
		if (mask & table[loop].bitmask)
			{ printf ("%c",table[loop].bitlabel); }
		else
			{ printf ("."); }
	}
	printf ("\"");
}

/*
********	down load a file into user memory
*/

#ifdef	CPM

inload(state,fname)
union	pstate	*state;
char	*fname;
{
	int	findex;
	struct	m_rt	mrt;
	struct	ustack	stack;
	long	addr,from,to;
	char	*arglist;
	struct	ccb	*ccbptr;

	printf ("loading '%s':\n",fname);
	findex= openb(fname,0);
	if (findex < 0)	error=80;
	if (error)	return;

	from= map_adr((long)bios(BGETMRT),2);	/* phys addr MRT in sys data */
	to= map_adr((long)&mrt,0);		/* phys addr of my MRT struct */
	mem_cpy (from,to,(long)sizeof(mrt));

	LPB.pgldaddr= mrt.m_reg[0].m_low;
	LPB.pgtop = mrt.m_reg[0].m_len;
	ccbptr = &_fds[findex];
	LPB.fcbaddr= map_adr((long)&(ccbptr->fcb), 0);

	tlong= _prog_load(map_adr((long)&LPB,0));
	if (error)	return;

	arglist= fname;
	while (!delchr(*arglist))	arglist++;

	addr= LPB.bpaddr - (SECLEN - sizeof (struct b_page));
	tlong= _setdma(addr);
	mem_cpy (map_adr((long)arglist,0), (long)addr, (long)SECLEN);

	stack.two= (short) 2;
	stack.bpgaddr= (short)LPB.bpaddr;
	addr= LPB.bpaddr - sizeof (stack);
	mem_cpy (map_adr((long)&stack,0), (long)addr, (long)sizeof(stack));
}

#else

inload(state,fname)
union	pstate	*state;
char	*fname;
{
	FILE	*ifile;

	printf ("loading '%s':\n",fname);
	ifile= fopen(fname,"r");
	if (ifile == NULL)	error=80;
	if (error)	return;
	while (fgets(iline,COLS,ifile) != NULL)	{
		iscan=iline;
		printf (".");
		inline();	/* process and load the line at iscan */
		if (error)	break;
	}
	state->lregs.pc= 0x0000;
	printf ("\n");
}

/*
********	download a single line into user memory
*/

inline()
{
	int	lcode,ectr,ldat;

	if (yflag & MUFOM)	{
		if (*iscan == 'A')	{
			iscan += 5;
			iptr= hexin(9);
			if (*iscan != '.')	error=83;
			return;
		}
		if (*iscan == 'L')	{
			iscan += 3;
			while (*iscan != '.')	{
				ldat= hexin(2);
				if (ishex(*iscan) < 0) break;
				stchr (iptr++,(BYTE)ldat);
			}
			if (*iscan != '.')	error=85;
			return;
		}
		error=81;
		return;
	}

	if (*iscan != ':')	{ error=81;	return; }
	iscan++;
	ectr= hexin(2);			/* get the record length */
	if (ishex(*iscan) < 0)	{ error=82;	return; }

	iptr= hexin(8);	/* get the load addr */
	if (ishex(*iscan) < 0)	{ error=83;	return; }

	lcode= hexin(2);		/* get the control code */
	if (ishex(*iscan) < 0)	{ error=84;	return; }

	switch (lcode)	{
	case 0:		/* data */
		while (ectr--)	{
			ldat= hexin(2);
			if (ishex(*iscan)<0 && ectr) error=85;
			if (error)	return;
			stchr (iptr++,(BYTE)ldat);
		}
		break;
	case 1:		/* EOF */
	case 2:		/* extended address mark */
	case 3:		/* start address */
	default:
		error=86; return;
	}
	if (ishex(*iscan) < 0)	{ error=87;	return; }
	ichksum= hexin(2);		/* get the checksum */

	if (*iscan == '\n')	iscan++;
	if (*iscan != '\0')	error=88;
}

#endif

/*
********	assemble into user memory (with deassembly verification)
*/

asdis(adptr,adbuf)
POINTER	adptr;
char	*adbuf;
{
	int	asopn,diopn;
	if (eolchr(*iscan))	return (0);
	memode= CODE;
	asopn= asmbl(adptr,adbuf);
	memode= DATA;
	if (error)	return (asopn);
	if (infixflg)	error= 10;
	if (error)	return (asopn);
	memode= CODE;
	diopn= deasm(adptr,oline);
	memode= DATA;
	if (error)	return (diopn);
	printf ("%s\n",oline);
/*
**	if (asopn != diopn)	error=46;
*/
	if (error)	return (asopn);
	if (getnext(asopn) != getnext(diopn))	error=47;
	return (diopn);
}

/*
********	encode and output a user memory address
*/

adrout (ptr)
POINTER	ptr;
{
	printf ("%08lx: ",ptr);
}

