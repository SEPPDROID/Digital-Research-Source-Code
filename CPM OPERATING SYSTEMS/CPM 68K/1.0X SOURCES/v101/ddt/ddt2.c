/****************************************************************/
/*                                                              */
/*      DDT-68K (tm)                                            */
/*                                                              */
/*      Copyright 1982, Digital Research                        */
/*                                                              */
/****************************************************************/
 
#include "ddtinc.h"
#include "stdio.h"
#include "bdosfunc.h"
#include "disas.h" 
 
 
gocmd(cx, statep)      /* begin executing (with optional breakpoints) */
char *cx;
struct cpustate *statep;
{
        int   nbp;
        long  brkpt[MAXBP];
        short brkwd[MAXBP];
        long  newpc;
        long  x;
        int   i;
 
 	deblank(&cx);
        newpc = statep->pc;
        if (gethex(&cx, &x)) newpc = x;
	nbp = 0;
        while ( GETSEP ) {
           if (nbp >= MAXBP) {bad(); return;}
           else { if (gethex(&cx,&x)) brkpt[nbp++]=x;
                  else {bad(); return;}
           }
        }
        if ( ! nomore(cx) ) {bad(); return(0);}
 
        /* now set up brkpts and go */
 
        for (i=0; i<nbp; i++) {
		brkwd[i]=(brkpt[i])->memw;
		brkpt[i]->memw = ILLEGAL;
        }
 
        statep->pc = newpc;
        GO(statep);
 
        for (i=0; i<nbp; i++) brkpt[i]->memw = brkwd[i];
 
        showstate(statep);
 
}  /* end of go command */
 
 
hexmath(cx)     /* print sum and difference of operands */
char *cx;
{
        long a, b;
 
	deblank(&cx);
        if ( gethex(&cx, &a) && GETSEP && gethex(&cx, &b) && nomore(cx) ) {
 
                puthexl(a+b);
                putchar(' ');
                puthexl(a-b);
                putchar('\n');
        } 
        else bad();
 
}
 
 
 
incomtl(cx, valuep)
char *cx;
struct value *valuep;
{
        /* process command tail */
 
	register char *p;
	register char *q;
	register char nc;
	register struct basepage *bpp;

	if ( (valuep->kind) != 1 ) {bad(); return;} /* no pgm loaded */

	bpp = valuep->bpa;
	p = cx + 1;
	q = &(bpp->comtail[1]);
	nc = 0;
	while (*p) { *q++ = *p++; nc += 1;}
	*q = *p;
	bpp->comtail[0] = nc;
	if ( parse(&cx, &(bpp->fcb1), "   ") && GETSEP )
	     parse(&cx, &(bpp->fcb2), "   ");

}


disasm(cx, dap)
char *cx;
char **dap;
{
        /* display memory in assembly format */
 
                 char *s;       /* start address */
                 char *f;       /* end   address */
        register short int deflt;     /**/
        register short int nl;        /**/
 
 
        /* parse command and extract parameters */
 
	deblank(&cx);
        if ( ! gethex(&cx, &s) ) {      /* get start address */
           if (nomore(cx)) s = *dap;    /* or use default    */
           else {bad(); return(0);}     /* junk on the line  */
        }
 
        if ( s & 1) { bad(); return(0);} /* must use even address */
 
        if ( GETSEP ) {			/* get end address */
	  if ( gethex(&cx, &f) ) deflt=0; /* got it */
	  else { bad(); return(0); }
	}
        else { f = s; deflt = 1; }            /* not there, use def */
 
        if((!nomore(cx)) || (s>f)) {bad(); return(0);} /* junk or nonsense */
 
 
        /* now do the display */
 
        nl = 0;
        while ( (s<=f) || (deflt && (nl<12)) ) {
 
                if ( keyhit() ) return;
 
                puthexl(s); putchar(' ');
 
                dot = s;
                pinstr();
                putchar('\n');
 
                s += dotinc;
                nl++;
 
                *dap = s;
 
        } /* end of loop through instructions */
 
 } /* end disassembly */
 
 
 
movemem(cx)
char *cx;
{
        /* move memory block */
 
        char *s;       /* start address       */
        char *f;       /* end   address       */
        char *d;       /* destination address */
 
 
        /* parse command and extract parameters */
 
	deblank(&cx);
        if ( gethex(&cx, &s) );         /* get start address */
        else {bad(); return(0);}        /* not there, error  */
 
        if ( GETSEP && gethex(&cx, &f) ); /* get end address  */
        else { bad(); return(0); }                /* not there, error */
 
        if ( GETSEP && gethex(&cx, &d) ); /* get dest address */
        else { bad(); return(0); }
 
        if ((!nomore(cx)) || (s>f) )   /* test for junk or nonsense */
           { bad(); return(0); }
 
 
        /* now do the moving */
 
        for ( ; s <= f; s++, d++ ) d->memb = s->memb;
  
} /* end of movemem */
 
 
 
 
readfl(cx, basep, valuep, dap)
char *cx;
struct basepage *basep;
struct value	*valuep;
char **dap;
{
        /* read a file into memory */
 
	struct	fcb	fcb1;
		long	lotpa;
		long	hitpa;
		long	curdma;
		int	endofile;

	deblank(&cx);
	if ( parse(&cx, &fcb1, "   ") && nomore(cx))
	{
		if ( (BDOS(OPEN, &fcb1) & 0xffL) == 0xffL) /* open failed */
		{
			stout("Cannot open file\n");
			return;
		}
		fcb1.cr = 0;
		lotpa = basep->lowtpa;
		hitpa = basep;
		curdma = lotpa;
		endofile = 0;
		while ( (! endofile) && (curdma+128 <= hitpa) )
		{
			/* read a sector */

			BDOS(SETDMA, curdma);
			endofile = (BDOS(READSEQ, &fcb1) & 0xffL);
			curdma += 128;
		}
		if ( ! endofile ) stout("\nFile too big -- read truncated.\n");
		valuep->kind = 2;
		valuep->textbase = lotpa;
		valuep->textlen = curdma - lotpa - 128;
		showvals(valuep);
		*dap = lotpa;
	}
	else	bad();	/* parsing error */
 
}
 
 
setmem(cx)
char *cx;
{
        /* set memory */
 
  register short int format;    /* 1=byte, 2=word, 4=long word */
                 char *s;       /* start address */
                 long  v;       /* value to stuff into memory */
                 char buf[BUFLEN]; /* input buffer */
                 char *bx;         /* points into buf */
           short int   nc;         /* num of hex digits input */
 
        /* parse command and extract parameters */
 
	format = getform(&cx);

        if ( gethex(&cx, &s) );         /* get start address */
        else {bad(); return(0);}        /* not there, error  */
 
        if ( (format != 1) && (s & 1) ) /* must be even address, error */
                {bad(); return(0);}
 
        if (!nomore(cx)) { bad(); return(0); } /* test for junk */
 
 
        /* now do the stuffing */
 
        for ( ; ; s += format ) {
 
                puthexl(s); putchar(' ');
                switch ( format ) {
 
                   case 1: puthexb(s->memb);
                           break;
 
                   case 2: puthexw(s->memw);
                           break;
 
                   case 4: puthexl(s->meml);
                           break;
 
                } /* end of switch */
                putchar(' ');
 
                *buf = BUFLEN - 2;
                BDOS ( READBUF, buf );
                putchar('\n');
 
                buf[2+(nc=buf[1])] = 0;
                if ( nc > 0 ) {
 
                        if ( buf[2] == '.' ) {return(0);}
 
                        for ( bx = &buf[1] ; *++bx ; ) *bx = toupper(*bx) ;
 
                        bx = &buf[1];
                        if ( gethex( &bx, &v ) );
                        else { bad(); return(0); }
 
                        while ( (*++bx) == 0x20 ) ; /* skip blanks */
 
                        if ( (*bx != 0) ||      /* test for bad input */
                             ( (format == 1) && ((v>255L)   || (v<0)) ) ||
                             ( (format == 2) && ((v>65535L) || (v<0)) )  )
                           { bad(); return(0); }
 
                        /* stuff the value */
 
                        switch ( format ) {
 
                           case 1 : s->memb = (char)v ;
				    if ( (s->memb ^ v) & 0x0ffL ) badram(s);
                                    break;
 
                           case 2 : s->memw = (short int)v ;
				    if ( (s->memw ^ v) & 0x0ffffL ) badram(s);
                                    break;
 
                           case 4 : s->meml = (long)v ;
				    if ( s->meml ^ v ) badram(s);
                                    break;
 
                        } /* end of switch */
 
                } /* end of nc > 0 */
 
        } /* end of for */  
 
} /* end of setmem */
 
 
trace(cx, statep, dap, tr)
char *cx;
struct cpustate *statep;
char **dap;
int tr;
{
        /* trace program execution for n steps */
 
	long nsteps;
	register int inst;
	register long addr;

	deblank(&cx);
	if ( ! gethex(&cx, &nsteps)) nsteps = 1;
	if ( ! nomore(cx) ) {bad(); return;}

	while ( nsteps-- ) {

		inst = (addr = (statep->pc))->memw;
		if ( (inst == BDOSCALL) || (inst == BIOSCALL) )
		{
			/* don't trace through system calls */
			inst = (addr += 2)->memw;
			addr->memw = ILLEGAL;
			GO(statep);
			addr->memw = inst;
		}
		else STEP(statep);
		if (tr) showstate(statep);
                if ( keyhit() ) nsteps = 0;
	}
	if ( ! tr ) showstate(statep);
	*dap = statep->pc;
 
}
 
 
 
vcmd(cx, valuep)
char *cx;
struct value *valuep;
{
        /* display start and end of stuff loaded with R or E commands */
 
	if (nomore(cx)) showvals(valuep);

}




wrtfile(cx, valuep)
char *cx;
struct value *valuep;
{
        /* write memory contents to disk */
 
	struct fcb fcb1;
	long	s;
	long	f;

	deblank(&cx);
	if ( ! parse(&cx, &fcb1, "   ") ) {bad(); return;}

	if ( nomore(cx) ) /* use default start and end */
	{
		if ( (valuep->kind) != 2 ) {bad(); return;}
		s = valuep->textbase;
		f = s + valuep->textlen - 1;
 	}
	else /* try to parse s & f values */
	{
		if ( ( ! GETSEP ) || ( ! gethex(&cx, &s) ) ||
		     ( ! GETSEP ) || ( ! gethex(&cx, &f) ) ||
		     ( ! nomore(cx) ) ) {bad(); return;}
	}

	BDOS(DELETE, &fcb1);
	if ( (BDOS(MAKEFILE, &fcb1) & 0xffL) == 0xffL )
	{
		stout("Cannot create file.\n");
		return;
	}
	
	fcb1.cr = 0;
	while ( s <= f ) 
	{
		BDOS(SETDMA, s);
		if ( BDOS(WRITSEQ, &fcb1) & 0xffL )
		{
			stout("File write error.\n");
			s = f + 1;
		}
		s += 128;
	}
	BDOS(CLOSE, &fcb1);
}
 
 
 
 
int dummy(s)
char *s;
{
        stout("\n\nUnimplemented Function: ");
        stout(s);
        stout("\n\n");
}
 
/**/
 
