/*
        Copyright 1981
        Alcyon Corporation
        8474 Commerce Av.
        San Diego, Ca.  92121
*/
 
#include "ddtinc.h"
#include "optab.h"
#include "stdio.h"
#include "bdosfunc.h"
#include "disas.h"
 
 
int noin();
int inf1();
int inf2();
int inf3();
int inf4();
int inf5();
int inf6();
int inf7();
int inf8();
int inf9();
int inf10();
int inf11();
int inf12();
int inf13();
int inf14();
int inf15();
int inf16();
int inf17();
int inf18();
int inf19();
int inf20();
int inf21();
int inf22();
int inf23();
int inf24();
int inf25();
int inf26();
int inf27();
int inf28();
 
 
/* print an instruction in assembler format */
 
pinstr()
{
 
        register struct optbl *p;
        register short int reg;
 
        sdot=dot+2;             /* next word address */
        dotinc = 2;
        instr=dot->memw;        /* instruction in binary */
        p = &optab;
        while(1) {      /* last table entry matches anything */
                if((instr&(p->inmsk)) == p->invalu)
                        break;          /* found it */
                p++;
        }
        stout(p->innam);         /* print mnemonic */
        if(p->infmt>=0 && p->infmt<=MAXFMT) {
            if(p->infmt) {
 
              switch ( p->infmt) {              /* call proper funct */
        
                case  0 : noin();  break;       case  1 : inf1();  break;
                case  2 : inf2();  break;       case  3 : inf3();  break;
                case  4 : inf4();  break;       case  5 : inf5();  break;
                case  6 : inf6();  break;       case  7 : inf7();  break;
                case  8 : inf8();  break;       case  9 : inf9();  break;
                case 10 : inf10(); break;       case 11 : inf11(); break;
                case 12 : inf12(); break;       case 13 : inf13(); break;
                case 14 : inf14(); break;       case 15 : inf15(); break;
                case 16 : inf16(); break;       case 17 : inf17(); break;
                case 18 : inf18(); break;       case 19 : inf19(); break;
                case 20 : inf20(); break;       case 21 : inf21(); break;
                case 22 : inf22(); break;       case 23 : inf23(); break;
                case 24 : inf24(); break;       case 25 : inf25(); break;
                case 26 : inf26(); break;       case 27 : inf27(); break;
                case 28 : inf28(); break;
 
              }
 
            }
            putchar(' ');
            return;
        }
        putchar('?');
        return;
}
 
noin()
{
 
        stout("illegal instruction format #\n");
        return;
}
 
inf1()
{
 
        register reg,mode;
 
        reg = ((instr&0xe00)>>9);
        if((instr&0xc0) == 0xc0) {      /* adda or suba */
                putchar('a');
                mode=WORD;
                if((instr&0x1c0)==0x1c0) {      /* long opmode */
                        mode = LONG;
                        stout(".l");
                }
                reg =+ 8;
        }
        else {
                mode = (instr&0xc0)>>6;
                prsize(mode);
        }
        putchar(' ');
        if(reg>7 || (instr&0x100)==0) {
                prtop(instr&077,mode);
                putchar(',');
                prtreg(reg);
        }
        else {
                prtreg(reg);
                putchar(',');
                prtop(instr&077,mode);  /* destination */
        }
}
 
/* move instruction */
inf3()
{
 
        register size;
        register i;
 
        i = instr&0x3000;       /* size field */
        if(i==0x1000)
                size = BYTE;
        else if(i==0x3000)
                size = WORD;
        else if(i==0x2000)
                size = LONG;
        else
                badsize();
        prsize(size);
        putchar(' ');
        prtop(instr&077,size);
        putchar(',');
        i = ((instr&07000)>>9) | ((instr&0700)>>3);
        prtop(i,size);
}
 
/* immediate instructions */
inf2()
{
 
        register size;
 
        size = (instr&0xc0)>>6;
        prsize(size);
        putchar(' ');
        primm(size);
        putchar(',');
	switch(instr)
	{
		case 0x027c:
		case 0x0a7c:
		case 0x007c:	stout("sr");
				break;
		case 0x023c:
		case 0x0a3c:
		case 0x003c:	stout("ccr");
				break;
        	default    :    prtop(instr&077,size);
	}
}
 
/* abcd, addx, sbcd, subx */
inf4()
{
 
        register size,i;
 
        size = (instr&0xc0)>>6;
        prsize(size);
        putchar(' ');
        i = (instr&0xe00)>>9;
        if(instr&8) {
                paripd(instr&7); putchar(','); paripd(i);
        }
        else {
                pdr(instr&7); putchar(','); pdr(i);
        }
}
 
/* chk, cmp, cmpa, div, mul */
inf5()
{
 
        register i,size,arg;
 
        arg = 0;
        i = instr&0xf000;
        size = (instr&0x1c0)>>6;
        if(i==0xb000) {                 /* cmp or cmpa */
                if(size==3 || size==7) {        /* cmpa */
                        arg++;
                        size =>> 1;
                        if(size==3)
                                size = 2;
                }
                prsize(size);
        }
        else
                size = WORD;
        putchar(' ');
        prtop(instr&077,size);
        putchar(',');
        i = (instr&0xe00)>>9;
        if(arg)         /* cmpa */
                par(i);
        else
                prtreg(i);
}
 
prsize(amode)
{
 
        if(amode==BYTE)
                stout(".b");
        else if(amode==LONG)
                stout(".l");
        else if(amode!=WORD)
                badsize();
}
 
badsize()
{
 
        stout("\n**illegal size field\n");
}
 
prtreg(areg)
{
 
        register reg;
 
        reg = areg;
        if(reg>7) {
                par(reg-8);
        }
        else {
                pdr(reg);
        }
}
 
/* print an operand symbolically */
 
prtop(adrtype,asize)
{
 
        register short int reg,mode,defer;
        long la;
        register long p;
 
        mode = (adrtype&070)>>3;
        reg = adrtype&7;
        switch(mode) {
 
        case 0:         /* D reg direct */
                pdr(reg);
                return;
 
        case 1:         /* A reg direct */
                par(reg);
                return;
 
        case 2:         /* A indirect */
                pari(reg);
                return;
 
        case 3:         /* A+ */
                paripi(reg);
                return;
 
        case 4:         /* -(An) */
                paripd(reg);
                return;
 
        case 5:         /* d(an) */
                prdisp();
                pari(reg);
                return;
 
        case 6:         /* d(An,Ri) */
                prindex(reg);
                return;
 
        case 7:
                la = 0;
                switch(reg) {
 
                case 1:         /* xxx.L */
                        la.hiword = sdot->memw;
                        sdot =+ 2;
                        dotinc =+ 2;
                        la.loword = sdot->memw;
                        sdot =+ 2;
                        dotinc =+ 2;
                        putchar('$');
                        hexlzs(la);
                        return;
 
                case 0:         /* xxx.W */
                        p = sdot->memw;
                        if(p&0x8000)
                                p =| 0xffff0000;  /* sign extend like hard */
                        sdot =+ 2;
                        dotinc =+ 2;
                        hexlzs(p);
                        return;
 
                case 2:         /* d(PC) */
                        prdisp();
                        stout("(PC)");
                        return;
 
                case 3:         /* d(PC,Ri) */
                        prindex(PC);
                        return;
 
                case 4:
                        primm(asize);
                        return;
                }
        }
}
 
prdisp()
{
 
        register short int i;
 
        i = sdot->memw;
        sdot =+ 2;
        dotinc =+ 2;
        putchar('$'); hexwzs(i);
}
 
prindex(areg)
{
 
        register short int i;
 
        i = sdot->memw;
        sdot =+ 2;
        dotinc =+ 2;
        putchar('$');
        hexbzs((char)i&0xFF);
        putchar('(');
        if(areg==PC)
                stout("PC,");
        else
                {par(areg); putchar(',');}
        prtreg((i>>12)&0xf);
        if(i&0x800)
                stout(".l");
        putchar(')');
}
 
primm(asize)
{
 
        long l1;
 
        l1 = 0;
        if(asize == LONG) {
                l1.hiword = sdot->memw;
                sdot =+ 2;
                dotinc =+ 2;
        }
        l1.loword = sdot->memw;
        sdot =+ 2;
        dotinc =+ 2;
        stout("#$");
        hexlzs(l1);
}
 
/* initialize the default special symbol values */
 
initssv()
{
 
        ssval[SSRG] = 010000;   /* symbol range */
}
 
/* branches & bsr */
inf6()
{
 
        register short int i;
        register char *p;
 
        i = instr.lobyte;
        if(i) {
                p = sdot + i;
        }
        else {
                p = sdot + sdot->memw;
                sdot =+ 2;
                dotinc =+ 2;
        }
        stout(" $");
        hexlzs(p);
}
 
/* bchg, bclr, bset, btst */
inf7()
{
 
        putchar(' ');
        if(instr&0x100) {       /* bit # dynamic */
                prtreg((instr&0xe00)>>9);
        }
        else {
                primm(WORD);
        }
        putchar(',');
        prtop(instr&077,WORD);
}
 
/* shifts and rotates */
inf8()
{
 
        register i,size;
 
        if(instr&0x100)
                putchar('l');
        else
                putchar('r');
        size = (instr&0xc0)>>6;
        if(size != 3) {
 
                prsize(size);
                putchar(' ');
                i = (instr&0xe00)>>9;
                if(instr&0x20) {                /* bit # in reg */
                        prtreg(i);
                }
                else {
                        if(i==0)
                                i = 8;
                        putchar('#');
                        hexbzs(i);
                }
                putchar(',');
                prtreg(instr&7);
        }
        else {
                putchar(' ');
                prtop(instr&077,WORD);
        }
}
 
/* jmp, jsr, pea */
inf9()
{
 
        putchar(' ');
        prtop(instr&077,WORD);
}
 
/* cmpm */
inf10()
{
 
        register i;
 
        prsize((instr&0xc0)>>6);
        i = (instr&0xe00)>>9;
        putchar(' ');
        paripi(instr&7); putchar(','); paripi(i);
}
 
/* dbcc */
inf11()
{
 
        register char *p;
 
        putchar(' ');
        prtreg(instr&7);
        putchar(',');
        p = sdot + sdot->memw;
        sdot =+ 2;
        dotinc =+ 2;
        hexlzs(p);
}
 
/* exg */
inf12()
{
 
        register rx,ry,i;
 
        rx = (instr&0xe00)>>9;
        ry = instr&7;
        i = instr&0xf8;
        if(i==0x48) {           /* exg a regs */
                rx =+ AREG0;
                ry =+ AREG0;
        }
        else if(i==0x88) {      /* exg a reg & d reg */
                ry =+ AREG0;
        }
        putchar(' ');
        prtreg(rx);
        putchar(',');
        prtreg(ry);
}
 
/* swap & unlk */
/*  called by ext (format 27) */
inf13()
{
 
        putchar(' ');
        prtreg(instr&0xf);
}
 
/* stop */
inf14()
{
 
        putchar(' ');
        primm(WORD);
}
 
/* lea */
inf15()
{
 
        putchar(' ');
        prtop(instr&077,WORD);
        putchar(',');
        prtreg(((instr&0xe00)>>9)+AREG0);
}
 
/* not used */
inf16()
{
        stout("unknown opcode");
}
 
/* addq, subq */
inf17()
{
 
        register i;
 
        prsize((instr&0xc0)>>6);
        i = (instr&0xe00)>>9;
        if(i==0)
                i = 8;
        stout(" #$"); hexbzs((char) i); putchar(',');
        prtop(instr&077,WORD);
}
 
/* trap */
inf18()
{
 
        stout(" #$"); hexbzs((char)(instr&0xf));
}
 
/* link */
inf19()
{
 
        putchar(' ');
        par(instr&7);
        stout(",#$"); hexwzs(sdot->memw);
        sdot =+ 2;
        dotinc =+ 2;
}
 
short int regmsk0[] {0100000,040000,020000,010000,04000,02000,01000,0400,0200,
                                0100,040,020,010,4,2,1};
short int regmsk1[] {1,2,4,010,020,040,0100,0200,0400,01000,02000,04000,010000,
                                020000,040000,0100000};
 
/* movem */
inf20()
{
 
        if(instr&0x40)
                stout(".l");
        putchar(' ');

	if (instr & 0x0400) /* test for mem->reg xfer */
	{
		prtop(instr & 077, WORD);
		putchar(',');
		putrlist(regmsk1);
	}
	else	/* must be reg->mem */
	{
		if ( (instr & 070) == 040 ) putrlist(regmsk0); /* predec */
		else			    putrlist(regmsk1);
		putchar(',');
		prtop(instr & 077, WORD);
	}
}
 
putrlist(ap)
short int *ap;
{
 
        register short int i,mask,*p;
        register short int j;
 
        mask = sdot->memw;
        sdot =+ 2;
        dotinc =+ 2;
        p = ap;
        j = -1;
        for(i=0; i<16; i++) {
                if(mask&*p++) {
                        if(j==0)
                                putchar('/');
                        if(j != 1) {
                                prtreg(i);
                                putchar('-');
                        }
                        j = 1;
                }
                else if(j==1) {
                        prtreg(i-1);
                        j = 0;
                }
        }
        if(j==1)
                stout("A7");
}
 
/* movep */
inf21()
{
        register i,j;
 
        if(instr&0x40)
                stout(".l");
        putchar('\t');
        i = instr&0x180;
        if(i==0x180) {
                prtreg((instr&0xe00)>>9);
                putchar(',');
        }
        j = sdot->memw;
        hexwzs(j); pari(instr&7);
        if(i==0x100) {
                putchar(',');
                prtreg((instr&0xe00)>>9);
        }
        sdot =+ 2;
        dotinc =+ 2;
}
 
/* moveq */
inf22()
{
 
        stout(" #$"); hexbzs((char)(instr&0xff)); putchar(',');
        prtreg((instr&0xe00)>>9);
}
 
/* eor */
inf23()
{
 
        prsize((instr&0xc0)>>6);
        putchar(' ');
        prtreg((instr&0xe00)>>9);
        putchar(',');
        prtop(instr&077,WORD);
}
 
/* clr, neg, negx, not, tst */
inf24()
{
 
        
        prsize((instr&0xc0)>>6);
        inf25();
}
 
/* scc, nbcd, tas */
inf25()
{
 
        putchar(' ');
        prtop(instr&077,WORD);
}
 
/* move SR & CCR */
inf26()
{
 
        register i;
 
        i = instr&0x600;
        if(i==0) {
                stout(" SR,");
        }
        else
                putchar(' ');
        prtop(instr&077,WORD);
        if(i==0x400)
                stout(",CCR");
        else if(i==0x600)
                stout(",SR");
}
 
/* ext */
inf27()
{
 
        if((instr&0x1c0)==0xc0)
                stout(".l");
        inf13();
}
 
/* move USP */
inf28()
{
 
        register i;
 
        i = instr&7;
        if(instr&8) {   /* USP to An */
                stout(".l USP,"); par(i);
        }
        else {
                stout(".l "); par(i); stout(",USP");
        }
}
 
 
/* some utilities for printing addresses */
 
pdr(r)
short int r;
{
        putchar('D'); putchar(hexchar(r));
}
 
par(r)
short int r;
{
        putchar('A'); putchar(hexchar(r));
}
 
pdri(r)
short int r;
{
        putchar('('); pdr(r); putchar(')');
}
 
pari(r)
short int r;
{
        putchar('('); par(r); putchar(')');
}
 
paripd(r)
short int r;
{
        putchar('-'); pari(r);
}
 
paripi(r)
short int r;
{
        pari(r); putchar('+');
}
 
 
 
 
 
hexlzs(n)  /* write a long integer in hex */
long n;
{
	puthex(n, 32, 1);
}
 
 
int hexwzs(n)  /* write an integer in hex */
short int n;
{
	puthex((long)n, 16, 1);
}
 
 
 
int hexbzs(n)  /* write a byte as a hex integer */
char n;
{
	puthex((long)n, 8, 1);
}
 
