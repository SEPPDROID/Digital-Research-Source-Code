/*
	Copyright 1981
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*
 * pass two for the 68000 assembler
 *		Bill Allen
 *		March 1980
 */

#include "as68.h"
#include "cout.h"	/*c.out header definition & MAGIC*/

#define MOVEA 0100

int p2gi();

extern char tfilname[];	/*name of it file*/
extern char initfnam[];	/*name of the initilization file*/
int (*p2direct[])();

int opf1(), opf2(), opf3(), opf4(), opf5(), relbr(), opf7(), opf8();
int opf9(), opf11(), opf12(), opf13(), opf15(), opf17(), opf20();
int opf21(), opf22(), opf23();

int (*opfary[])() {
	0,		/*0*/
	opf1,	/*1*/
	opf2,	/*2*/
	opf3,	/*3*/
	opf4,	/*4*/
	opf5,	/*5*/
	relbr,	/*6*/
	opf7,	/*7*/
	opf8,	/*8*/
	opf9,	/*9*/
	opf4,	/*10*/
	opf11,	/*11*/
	opf12,	/*12*/
	opf13,	/*13*/
	opf9,	/*14*/
	opf15,	/*15*/
	opf17,	/*16*/
	opf17,	/*17*/
	opf13,	/*18*/
	opf11,	/*19*/
	opf20,	/*20*/
	opf21,	/*21*/
	opf22,	/*22*/
	opf23,	/*23*/
	opf9,	/*24*/
	opf9,	/*25*/
	opf5,	/*26*/		/* [vlh] cmp, chk, extention verification */
	opf4,	/*27*/		/* [vlh] addx, subx, extension verification */
	opf13,	/*28*/		/* [vlh] swap, extension verification */
	opf9,	/*29*/		/* [vlh] pea, extention verification */
	opf15	/*30*/		/* [vlh] lea, extension verification */
};

#define LSTFRMT 30

int f1mode[] {0,0,0100,0,0200};
int f2mode[] {0,0,0100,0,0200};
int f3mode[] {0,010000,030000,0,020000};
int f15mode[] {0,0,0300,0,0700};
int f5mode[] {0,0,0100,0,0200};
int f5amode[] {0,0,0300,0,0700};
int f13mode[] {0,0,0200,0,0300};
int f23mode[] {0,0400,0500,0,0600};
int rlbits[5];			/*holds relocation bits for instr*/
int pline;				/*number of last printed line*/
int brkln2 077777;		/*pass 2 break line number for debugging*/
int prsp;				/*special print alignment flag*/
int amode;				/*addressing mode*/
int nitleft;
/*pass two driver*/
pass2()
{
	register short *p;
	register i;
	register (*dirop)();

	pitix = itbuf;		/* This is init for doitrd*/
	xline = LPP;
	nitleft = 0;
	lbuf.nunused = tbuf.nunused = dabuf.nunused = drbuf.nunused = 512;
	lbuf.fildes = lfn;		/*set buffered io for binary file*/
	lbuf.xfree = &lbuf.buff[0];
	tbuf.fildes = trbfn;	/*set buffered io for text reloc bits file*/
	tbuf.xfree = &tbuf.buff[0];
	dabuf.fildes = dafn;	/*set buffered io for data bytes*/
	dabuf.xfree = &dabuf.buff[0];
	drbuf.fildes = drbfn;	/*set buffered io for data reloc bits*/
	drbuf.xfree = &drbuf.buff[0];
	couthd.ch_magic = MAGIC;/*c.out magic number*/
	if(savelc[TEXT]&1)
		savelc[TEXT]++;		/*make it even*/
	couthd.ch_tsize = savelc[TEXT];	/*text size*/
	if(savelc[DATA]&1)
		savelc[DATA]++;		/*make it even*/
	couthd.ch_dsize = savelc[DATA];	/*data size*/
	couthd.ch_bsize = savelc[BSS];	/*bss size*/
/*symbol table size is not known now -- it is set at end of pass 2*/
/*  entry point and stack size are zero for now*/
	p = &couthd;
	for(i=0; i<HDSIZE/2; i++) {
		putw(*p++,&lbuf);		/*write the file header words*/
	}
	savelc[0] = 0; savelc[1] = 0; savelc[2] = 0; savelc[3] = 0;
	loctr = 0;		/*location counter*/
	rlflg = TEXT;	/*TEXT relocatable*/
	p2flg = 1;		/*pass two*/
#ifdef	UNIX
	if (lseek(ifn,0L,0) == -1)	{	/*beginning of source*/
		rpterr("seek error on source file\n");
		abort();
	}
#else
	close(ifn);		/* This way for goddamn Whitesmith's	*/
	ifn=open(sfname,0,0);	/* Re-open the fucking source file	*/
	if(ifn < 0)		/* Couldn't				*/
	{
		printf("Unable to open file %s\n",sfname);
		abort();
	}
#endif
	close(itfn);
	LASTCHTFN = itfnc;
	itfn = openfi(tfilname,0);	/*open it for reading*/
	pline = 1;		/*no lines printed*/
	fchr=gchr();	/*get first char*/
	while(ristb())	 {		/*pass 2 main loop*/
		p2absln = stbuf[0].itop;	/*line number*/
		if(p2absln>=brkln2)		/*for debugging the assembler*/
		i=0;
		opcpt = stbuf[2].itop.ptrw2;	/*ptr to opcode entry in main tab*/
		nite = stbuf[0].itrl & 0377;	/*number of it entries*/
		pnite = &stbuf[nite];		/*ptr to end of stmt*/
		modelen = stbuf[2].itrl;	/*instr mode length*/
		p1inlen = stbuf[1].itrl;	/*pass 1 instr length guess*/
		opdix = ITOP1;			/*first operand*/
		pitw = &stbuf[ITOP1];		/*ptr to first operand*/
		prsp = 0;			/*special print flag off*/
		instrlen = 2;			/*default for print*/
		if(opcpt->flags&OPDR) {	/*opcode is a directive*/
			i = opcpt->vl1;	/*directive number*/
			dirop = p2direct[i];
			(*dirop)();			/*handle directive*/
		}
		else {
			gcist();			/*generate code for one statement*/
		}
	}
}

/* generate code for an instruction*/
/*  call with*/
/*		intermediate text for instruction in stbuf*/
gcist()
{
	register i,j;

	if(stbuf[0].itty != ITBS)	/*beginning of statement*/
		abort();
	format = (opcpt->flags)&OPFF;
	in_err = 0;				/*[vlh] no error this instruction, yet*/
	ival = 0;				/*initial value for possible operand*/
	reloc = ABS;
	instrlen = 2;			/*at least 2 bytes*/
	ins[0] = opcpt->vl1;	/*opcode value*/
	rlbits[0] = INSABS;		/*instruction absolute*/
	pins = &ins[1];
	prlb = &rlbits[1];
	if(nite>ITOP1) {		/*operands*/
		if(!format) {
			uerr(9);
		}
		else if(format>LSTFRMT)	/* [vlh] was a magic number... */
			abort();
		else {
			(*opfary[format])();
		}
	}
	if (!ckein() && !in_err) 		/* at end of statement ?? */
		uerr(6);
	print(1);			/*print source*/

	loctr =+ p1inlen;
	if (!in_err && p1inlen != instrlen)	/* [vlh] 2nd pass error recovery */
		uerr(38);
	outinstr();		/*write out instr binary*/
}

/* relative branches*/
relbr()
{
	expr(&p2gi);
	if(extflg) {	/*external reference*/
		instrlen =+ 2;		/*long relative*/
		*pins++ = ival;		/*pass constant part*/
		*prlb++ = (extref<<3)|EXTREL;	/*ext ref*/
		return;
	}
	ival =- (loctr+2);		/*calc relative offset*/
	if(itype!=ITCN || reloc != rlflg) {
		uerr(22);		/*invalid relative branch*/
		ival = 0;
	}
	reloc = ABS;
	if(p1inlen==4) {		/*long displacement*/
		if(ival< (-32768L) || ival > (32767L))
			uerr(22);
		instrlen =+ 2;
		*pins++ = ival;
		*prlb++ = DABS;		/*data absolute*/
	}
	else {					/*short displacement*/
		if (ival>127 || ival<-128)
			uerr(22);
		ins[0] =| (ival.wd2&0377);
	}
	if (!ival)	{	/* make it a nop */
		opcpt = nopptr;
		ins[0] = opcpt->vl1.wd2;
		pins = &ins[1];
		if (modelen==4) {
			*pins++ = opcpt->vl1.wd2;
			rlbits[1] = INSABS;
		}
	}
	in_err++;	/* ignore extra eg. bra *+$d04(pc) vs. bra *+d04 */
}

/* format one -- add, sub, and, or, cmp, etc.*/
/*  one operand must be a D reg (or A reg dest for add, sub, or cmp)*/
opf1()
{
	register int *p;

	if(get2ops())
		return;
	if (ins[0]==AND || ins[0]==OR)
		if (cksprg(&opnd[1],CCR) || cksprg(&opnd[1],SR)) {
			if (ins[0]==AND) opcpt = andiptr;
			else opcpt = oriptr;
			ins[0] = opcpt->vl1.wd2;
			format = (opcpt->flags)&OPFF;
			ccr_or_sr();
			return;
		}
	p = f1mode;
	if(ckdreg(&opnd[1])) {	/*destn is D reg*/
		if((opcpt==andptr||opcpt==orptr)&&ckareg(&opnd[0]))	/*A source*/
			uerr(20);
		makef1(opnd[1].ea,p[modelen],&opnd[0]);	/*make instr*/
		return;
	}
	else if(ckdreg(&opnd[0]) && memalt(&opnd[1])) {	/*source is D reg*/
		if (pcea(&opnd[1])) uerr(10);
		makef1(opnd[0].ea,p[modelen]+0400,&opnd[1]);
		return;
	}
	else if(ckareg(&opnd[1])) {	/*A reg is dstn*/
		if(opcpt==addptr)
			opcpt = addaptr;
		else if(opcpt==cmpptr)
			opcpt = cmpaptr;
		else if(opcpt==subptr)
			opcpt = subaptr;
		else {
			uerr(20);
			return;
		}
		format = (opcpt->flags)&OPFF;
		opnd[1].ea =& 07;
		p = f15mode;
		makef1(opnd[1].ea,p[modelen],&opnd[0]);	/*make instr*/
		return;
	}
	else if(!makeimm())		/*make an immediate instr*/
		uerr(20);
}

/* format 2 -- addi, andi, subi, etc*/
opf2()
{
	if(get2ops())
		return;
	if(ins[0]==ANDI || ins[0]==ORI || ins[0]==EORI) {
		if(cksprg(&opnd[1],CCR) || cksprg(&opnd[1],SR)) {
			ccr_or_sr();
			return;
		}
	}
	if(opnd[0].ea != IMM) {
		uerr(9);
		return;
	}
	if(!dataalt(&opnd[1]) || pcea(&opnd[1])) {
		uerr(20);
		return;
	}
	genimm();
}

/*format #3 -- move and movea*/
opf3()
{
	register i,j,k;

	if(get2ops())
		return;
	if(cksprg(&opnd[1],CCR)) {
		ins[0] = MOVECCR;
opf3l1:
		if (modelen==1 || modelen==4) uerr(34);
		if(anysprg(&opnd[0]))
			uerr(20);
		ins[0] =| opnd[0].ea;
		if(!dataea(&opnd[0]))
			uerr(9);
		doea(&opnd[0]);
		return;
	}
	if(cksprg(&opnd[1],SR)) {
		ins[0] = MOVESR;
		goto opf3l1;
	}
	if(cksprg(&opnd[0],SR)) {
		if (modelen==1 || modelen==4)
			uerr(34);
		if(anysprg(&opnd[1]))
			uerr(20);
		ins[0] = SRMOVE | opnd[1].ea;
		if(!dataalt(&opnd[1]) || pcea(&opnd[1]))
			uerr(10);
		doea(&opnd[1]);
		return;
	}
	if(cksprg(&opnd[0],USP)) {
		if (modelen == 1)
			uerr(34);	/* default is word, can't test */
		if (!ckareg(&opnd[1]))
			uerr(33);
		ins[0] = MOVEUSP|8|(opnd[1].ea&7);
		return;
	}
	if(cksprg(&opnd[1],USP)) {
		if (modelen == 1)
			uerr(34);	/* default is word, can't test */
		if (!ckareg(&opnd[0]))
			uerr(33);
		ins[0] = MOVEUSP|(opnd[0].ea&7);
		return;
	}
	k = ins[0];
	ins[0] =| f3mode[modelen];
	ckbytea();
	ins[0] =| opnd[0].ea;		/*source ea*/
	doea(&opnd[0]);
	ins[0] =| (opnd[1].ea&7)<<9;	/*dest register*/
	ins[0] =| (opnd[1].ea&070)<<3;	/*dest mode*/
	doea(&opnd[1]);
	if(k==MOVEA) {
		if(dataea(&opnd[1]))
			uerr(20);
	}
	else if((pcea(&opnd[1]) && dataea(&opnd[1])) || opnd[1].ea==IMM)
		uerr(20);
}

/* format 4 -- abcd, sbcd */
/* format 10 -- cmpm*/
/* format 27 -- addx, subx */
opf4()
{
	if(get2ops())
		return;
	if (format==27) { /*addx,subx add in size bits*/
		ins[0] =| f1mode[modelen];
	}
	else if(format==10) {	/*cmpm*/
		if((opnd[0].ea&070)!=INDINC || (opnd[1].ea&070)!=INDINC)
			uerr(20);
		ins[0] =| f1mode[modelen] | ((opnd[0].ea&7)|((opnd[1].ea&7)<<9));
		return;
	}
	if(ckdreg(&opnd[0]) && ckdreg(&opnd[1])) {
		ins[0] =| ((opnd[0].ea&7)|((opnd[1].ea&7)<<9));
		return;
	}
	if((opnd[0].ea&070)==DECIND && (opnd[1].ea&070)==DECIND) {
		ins[0] =| 010 | ((opnd[0].ea&7)|((opnd[1].ea&7)<<9));
		return;
	}
	uerr(20);
}

/*format 5 -- div, mul*/
/*format 26 -- cmp, chk */
opf5()
{
	if(get2ops())
		return;
	if(!ckdreg(&opnd[1])) {
		if(opcpt==cmpptr) {
			if(!dataea(&opnd[1]))	/* [vlh] made define */
				ins[0] =| f5amode[modelen];	/* was pumode */
			else if(makeimm())
				return;
			else
				uerr(20);
		}
		else
			uerr(20);
	}
	if(opcpt==cmpptr) {
		ins[0] =| f5mode[modelen];	/* was pumode */
		ckbytea();
	}
	else if(!dataea(&opnd[0]))
		uerr(20);
	ins[0] =| (opnd[1].ea&7)<<9 | opnd[0].ea;
	doea(&opnd[0]);
}

#define BTST	0000
/* format 7 -- bit instrs -- btst, bclr, bset, etc*/
opf7()
{
	if(get2ops())
		return;
	if(opnd[1].ea==IMM||(ins[0]!=BTST&&pcea(&opnd[1]))||ckareg(&opnd[1]))
		uerr(20);
	if(ckdreg(&opnd[0])) {
		ins[0] =| (opnd[0].ea<<9)|0400;
	}
	else {		/*static bit #*/
		if(opnd[0].con<0L || opnd[0].con>31 ||
			(opnd[1].ea&INDIRECT&&opnd[0].con>7)) /* [vlh] */
			uerr(23);
		if(opnd[0].ea != IMM)
			uerr(17);
		ins[0] =| 04000;
		dodisp(&opnd[0]);
	}
	if (modelen==1 && !(memea(&opnd[1])))	/*[vlh]*/
			uerr(20);
	else if (!(ckdreg(&opnd[1])) && modelen==4)
		uerr(20);
	ins[0] =| opnd[1].ea;
	doea(&opnd[1]);
}

/* format 8 -- shifts and rotates*/
opf8()
{
	register i;

	getea(0);	/*get first operand*/
	if(pitw >= pnite) {		/*end of all ops*/
		if(ckdreg(&opnd[0])) {	/*shift dreg one bit*/
			cpop01();		/*copy opnd 0 to 1*/
			opnd[0].ea = IMM;
			opnd[0].con = 1L;
			if (!ckdreg(&opnd[1])) uerr(20);
opf8l1:
			if(opnd[0].con<1 || opnd[0].con>8)	/*[vlh legal range 1..8*/
				uerr(37);
			ins[0] =| ((opnd[0].con.wd2&7)<<9)|f1mode[modelen]|opnd[1].ea;
			return;
		}
		i = (ins[0]&077)<<6;
		ins[0] =& 0177700;
		ins[0] =| 0300|i|opnd[0].ea;
		if(!memalt(&opnd[0]) || pcea(&opnd[0]) || modelen != 2)
			uerr(20);
		doea(&opnd[0]);
		return;
	}
	if(!ckcomma()) {
		uerr(10);
		return;
	}
	getea(1);		/*get second operand*/
	if(!ckdreg(&opnd[1]))	/* [vlh] second operand must be dreg */
		uerr(20);
	if(ckdreg(&opnd[0])) {	/*first op is D reg*/
		ins[0] =| (opnd[0].ea<<9)|040;	/*reg # and reg bit*/
	}
	else {
		if(opnd[0].ea != IMM)
			uerr(20);
		goto opf8l1;
	}
	ins[0] =| f1mode[modelen] | opnd[1].ea;	/*put in size and reg #*/
}

/* format 9 -- jmp, jsr */
/* format 14 -- stop */
/* format 24 -- clr, neg, negx, not */
/* format 25 -- s?? */
/* format 29 -- pea */
/* one operand instructions -- jmp, clr, neg, not, sge, etc.*/
opf9()
{
	getea(0);
	if(format==24) {	/*clr, not, etc*/
		ins[0] =| f1mode[modelen];	/*add size bits*/
		if(!dataalt(&opnd[0]) || pcea(&opnd[0]))
			uerr(20);
	}
	else if(format==25) {	/*tas,scc, etc*/
		if(ckareg(&opnd[0]) || pcea(&opnd[0]) || opnd[0].ea==IMM)
			uerr(20);
	}
	else if(format==14) {		/*stop*/
		if(modelen!=2 || opnd[0].ea!=IMM)
			uerr(20);
		doea(&opnd[0]);
		return;
	}
	else if(!controlea(&opnd[0]))	/*jmp, jsr, etc*/
		uerr(20);
	ins[0] =| opnd[0].ea;
	doea(&opnd[0]);
}

/* format 11 -- dbcc*/
/* format 19 -- link*/
opf11()
{
	if(get2ops())
		return;
	if(format==19) {		/*link*/
		if(!ckareg(&opnd[0]))
			uerr(33);
		if(opnd[1].ea != IMM)
			uerr(17);
	}
	else {
		if(!ckdreg(&opnd[0]))
			uerr(33);
		if(opnd[1].drlc!=rlflg)	/*[vlh]don't chk opnd[1].ea!=LADDR||SADDR*/
			uerr(22);
		opnd[1].con =- (loctr+2L);
		cksize(&opnd[1]);
		opnd[1].drlc = ABS;		/*not relocatable*/
	}
	ins[0] =| opnd[0].ea&7;	/*put in reg #*/
	dodisp(&opnd[1]);
}

/* format 12 -- exg*/
opf12()
{
	register i;

	if(get2ops())
		return;
	if(ckdreg(&opnd[0])) {
		if(ckdreg(&opnd[1])) {	/*exchange D regs*/
			ins[0] =| 0100 | ((opnd[0].ea&7)<<9) | (opnd[1].ea&7);
			return;
		}
		if(ckareg(&opnd[1])) {	/*ins[0] <- A and D flag*/
			ins[0] =| 0210 | ((opnd[0].ea&7)<<9) | (opnd[1].ea&7);
			return;
		}
	}
	if(ckareg(&opnd[0])) {
		if(ckareg(&opnd[1])) {	/*both a regs*/
			ins[0] =| 0110 | ((opnd[0].ea&7)<<9) | (opnd[1].ea&7);
			return;
		}
		if(ckdreg(&opnd[1])) {	/*A and D regs*/
			i = opnd[0].ea;		/*exchg ea's*/
			opnd[0].ea = opnd[1].ea;
			opnd[1].ea = i;
			ins[0] =| 0210 | ((opnd[0].ea&7)<<9) | (opnd[1].ea&7);
			return;
		}
	}
	uerr(20);
}

/* format 13 -- ext, unlk*/
/* format 18 -- trap*/
/* format 28 -- swap */
#define UNLK	047130

opf13()
{
	getea(0);
	if(format==18) {	/*trap*/
		if(opnd[0].con<0 || opnd[0].con>15)
			uerr(15);
		ins[0] =| opnd[0].con.wd2;
		return;
	}
	if(ins[0]==UNLK) {	/*unlk instr*/
		if(!ckareg(&opnd[0]))
			uerr(20);
	}
	else {
		if(!ckdreg(&opnd[0]))
			uerr(20);
		if (format==13)		/* ext */
			ins[0] =| f13mode[modelen];
	}
	ins[0] =| opnd[0].ea&7;
}

/* format 15 -- adda, cmpa, suba*/
/* format 30 -- lea */
opf15()
{
	register i;

	if(get2ops())
		return;
	if(!ckareg(&opnd[1]))
		uerr(33);
	if(format==30) {
		i = 0700;
		if(!controlea(&opnd[0]))
			uerr(20);
	}
	else
		i = f15mode[modelen];
	makef1(opnd[1].ea&7,i,&opnd[0]);
	if (format==15 && opnd[0].ea != 071) cksize(&opnd[0]);
}

/*formats 16 and 17 -- addq, inc, subq, dec*/
opf17()
{
	if(format==16) {	/*inc or dec*/
		clrea(&opnd[0]);
		opnd[0].ea = IMM;
		opnd[0].con = 1L;
		opnd[0].drlc = ABS;
		getea(1);
	}
	else {
		if(get2ops())
			return;
	}
	if(opnd[0].ea != IMM || !altea(&opnd[1]) || pcea(&opnd[1]))
		uerr(20);
	if(opnd[0].con<=0 || opnd[0].con>8)
		uerr(15);
	if(modelen==1 && !dataea(&opnd[1]))
		uerr(34);
	ins[0] =| f1mode[modelen]|((opnd[0].con.wd2&7)<<9)|opnd[1].ea;
	doea(&opnd[1]);
}

/* format 20 -- movem */
int regmsk0[] {0100000,040000,020000,010000,04000,02000,01000,0400,0200,
				0100,040,020,010,4,2,1};
int regmsk1[] {1,2,4,010,020,040,0100,0200,0400,01000,02000,04000,010000,
				020000,040000,0100000};
opf20()
{
	register dr;
	register i,j;

	dr = 0;
	if(getreg() != -1 || pitw->itty == ITRM) {	/*regs to memory*/
		if (pitw->itty != ITRM) {	/* [vlh] */
			pitw--;
			j = getrlist(regmsk0);
		}
		else {
			j = pitw->itop;
			pitw++;
		}
		if(!ckcomma())
			uerr(10);
	}
	else
		dr = 02000;
	getea(0);
	if(dr) {
		if(!ckcomma())
			uerr(10);
		if (pitw->itty != ITRM)		/* [vlh] */
			j = getrlist(regmsk1);	/*mem to regs*/
		else {
			j = pitw->itop;
			j = fixmask(j);
			pitw++;
		}
	}
	else {
		if(controlea(&opnd[0]))
			j = fixmask(j);
	}
	i = opnd[0].ea&070;
	if(!controlea(&opnd[0]) && i!=INDINC && i!=DECIND)
		uerr(20);
	if(modelen==4)	/*long*/
		ins[0] =| 0100;
	ins[0] =| opnd[0].ea|dr;
	*pins++ = j;			/*reg mask*/
	*prlb++ = DABS;
	instrlen =+ 2;
	doea(&opnd[0]);
	if (!dr) {	/* 1st argument (2nd is reg list) */
		if (pcea(&opnd[0]) || (opnd[0].ea&070)==INDINC)
			uerr(20);	/* xx(pc), xx(pc,dx), -(ax) */
	}
	else	/* 2nd argument (1st is reg list) */
		if ((opnd[0].ea&070)==DECIND)
			uerr(20);	/* (ax)+ */
}

/*
 * get a list of registers for the movem instr
 * call with:
 *	ptr to reg-to-mem or mem-to-reg array of bits
 */
getrlist(ap)
int *ap;
{
	register int *p,i,j;
	register int mask;

	p = ap;
	mask = 0;
	while((i=getreg()) != -1) {
		if(ckitc(pitw,'-')) {
			pitw++;
			if((j=getreg()) == -1) {
				uerr(40);
				break;
			}
			while(i<=j)
				mask =| p[i++];
		}
		else
			mask =| p[i];
		if(ckitc(pitw,'/'))
			pitw++;
		else
			break;
	}
	if(!mask)
		uerr(40);
	return(mask);
}

/*reverse a movem register mask for control ea to memory*/
fixmask(msk)
{
	register i,j,k;

	k = (msk&1) ? 0100000 : 0;
	i = 2;
	j = 040000;
	while(i) {
		if(msk&i)
			k =| j;
		i =<< 1;
		j =>> 1;
	}
	return(k);
}

/* format 21 -- movep*/
opf21()
{
	register m,d;
	register char *p;

	if(get2ops())
		return;
	if(ckdreg(&opnd[0])) {	/*d reg source*/
		m = 0600;
		d = opnd[0].ea;
		p = &opnd[1];
	}
	else if(ckdreg(&opnd[1])) {		/*d reg dest*/
		m = 0400;
		d = opnd[1].ea;
		p = &opnd[0];
	}
	else {
		uerr(20);
	}
	if((p->ea&070) != INDDISP)
		uerr(20);
	if(modelen == 4)
		m =| 0100;
	ins[0] =| (d<<9)|m|(p->ea&7);
	*pins++ = p->con.wd2;
	*prlb++ = p->drlc;
	instrlen =+ 2;
}

/* format 22 -- moveq*/
opf22()
{
	if(get2ops())
		return;
	if(opnd[0].ea != IMM)
		uerr(17);
	if(opnd[0].con>255L || opnd[0].con<-256L)
		uerr(15);
	if(!ckdreg(&opnd[1]))
		uerr(33);
	ins[0] =| (opnd[1].ea<<9) | (opnd[0].con.wd2&0377);
}

/* format 23 -- eor*/
opf23()
{
	if(get2ops())
		return;
	if (cksprg(&opnd[1],CCR) || cksprg(&opnd[1],SR)) {
		opcpt = eoriptr;
		ins[0] = opcpt->vl1.wd2;
		format = (opcpt->flags)&OPFF;
		ccr_or_sr();
		return;
	}
	if(!ckdreg(&opnd[0])) {
		if(makeimm())	/*must be immediate*/
			return;
		uerr(20);		/*or error*/
	}
	if(!dataalt(&opnd[1]) || pcea(&opnd[1]))
		uerr(20);
	ins[0] =| (opnd[0].ea<<9)|f23mode[modelen]|opnd[1].ea;
	doea(&opnd[1]);
}

