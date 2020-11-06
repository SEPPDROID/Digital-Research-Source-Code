/********************************************************/
/*							*/
/*	Usual filename:  SID68K.C			*/
/*	Remarks:  Main C module for SID68K		*/
/*	Author:  Timothy M. Benson			*/
/*	Control:  19 MAY 83  18:01  (TMB)		*/
/*							*/
/********************************************************/


#include "lgcdef.h"
#include "cputype.h"
#include "siddef.h"
#include "ddtinc.h"
#include "stdio.h"
#include "bdosfunc.h"
#include "disas.h"

typedef long (*PFL)();

struct lmhedr *caput = NULL;	/* Symbol table--known globally */
long	HMSYMS;

long inhalt;	/* Possible symbol value discovered during dissasembly */
int nomo = -1;	/* Index to last symbol found by name */
int omega = -1;	/* Index to last symbol in symbol table:*/
		/* 	Negative value => no symbols	*/
char ref;	/* Symbols for disassembly:		*/
		/*	'\0' => none			*/
		/*	'm' => text, data, or bss	*/
/**temp** Setting of 'a' not currently in use */
		/*	'a' => other			*/
int scope = 0;	/* If zero, find all; otherwise find first */
int minus;	/* 1 => "-" in effect; 0 => "-" not in effect	*/
U16 pkawnt[HMPPTS];	/* Pass counts (0 => not in use)	*/
U16 *plocus[HMPPTS];	/* Values of pass points	*/
U16 ancien[HMPPTS];	/* Previous contents at pass point	*/

struct cpustate state;			/* state of user program regs */


main(bpptr)
struct basepage *bpptr;
{
        char combuf[BUFLEN];
        char *cx;
        char *dispaddr; /* current display address */
	char *listaddr; /* current disassembly addr*/
	struct value curvals;	/* values relating to current file or pgm */
        register int i; /* workhorse integer */
 
        /* phony machine state initialization */
        for (i=0; i<8; i++) {state.dreg[i]=0L; state.areg[i]=0L;}
        state.pc = 0x0;
        state.usp = 0x00001000L;
	state.areg[7]=state.usp;
        state.ssp = GETSSP();
        state.status = 0x0000;
#ifdef MC68010
	state.vbr = GETVBR();
	state.sfc = GETSFC();
	state.dfc = GETDFC();
#endif
        dispaddr = 0;
 	listaddr = 0;
	curvals.kind = 0; /* no current file or program */
	zappas();	/* Purge pass points */
	init(bpptr, &state, &curvals, &dispaddr, &listaddr, &bpptr);
        while ( 1 ) { 
	   while ( ! (cx = readcom(combuf)) );
	   If *cx EQ '-' then BEGIN
		minus = 1;
		++cx;
	   END
	   otherwise minus = 0;
           switch ( *cx ) {
                /* one case for each possible command */
		case 'C':
				callrt(cx);
				break;

                case 'D' :      display(cx, &dispaddr);
                                break;

                case 'E' :      ecmd(cx, bpptr, &state, &curvals, 
				     &dispaddr, &listaddr, &bpptr);
                                break;

                case 'F' :      fillmem(cx);
                                break;

                case 'G' :      gocmd(cx,&state,&listaddr);
                                break;
 
                case 'H' :      hexmath(cx);
                                break;
 
                case 'I' :      incomtl(cx, &curvals);
                                break;
 
		case 'K' :	koax(cx);
				break;

                case 'L' :      disasm(cx, &listaddr);
                                break;
 
                case 'M' :      movemem(cx);
                                break;
 
		case 'P' :	passpt(cx);
				break;

                case 'R' :      readfl(cx, bpptr, &curvals, &dispaddr);
                                break;
 
                case 'S' :      setmem(cx);
                                break;
 
                case 'T' :      trace(cx, &state, &listaddr, (int)1);
                                break;
 
                case 'U' :      trace(cx, &state, &listaddr, (int)0);
                                break;
 
                case 'V' :      vcmd(cx, &curvals);
                                break;
 
                case 'W' :      wrtfile(cx, &curvals);
                                break;
 
                case 'X' :      examine(cx, &state);
                                break;
 
                default  :      stout("Unknown Command\n\n");
                                break;
 
           }; /* end of switch on comx */ 
 
        };  /* end of while loop */
 
}  /* end of function main */
 
 
init(basep, statep, valuep, dap, lap, bppa)
struct	basepage *basep;
struct	cpustate *statep;
struct	value	 *valuep;
	char	**dap;
	char	**lap;
	long	 *bppa;
{
        /* if program file argument, then load it       */
        /* set up trap vectors                          */
        /* initialize tables                            */

	EXINIT();	/* Initialize Exception handlers */
 
     stout("\n**************************************************\n");
       stout("DDT-68K        3/19/84                 Version 1.2\n");
	stout("Serial #XXXX-0000-654321       All Rights Reserved\n");
	stout("Copyright 1982,1983,1984     Digital Research Inc.\n");
	stout("**************************************************\n\n");

	caput = (long)basep - (long)(XTRASYM*sizeof(struct lmhedr));
	if ((long)caput < basep->lowtpa) caput = NULL;

	HMSYMS = 0;

	if (basep->fcb1.fn[0] != ' ')
		loadpgm(&(basep->fcb1),basep,statep,valuep,dap,lap,bppa);

}
 
 
char *readcom(buf)      /* read one command line, return command type   */
register char *buf;
{
        register char *cx; 
        register short int i,nc;
 
        do {
                stout("\r\r-");         /* prompt */
                *buf = BUFLEN-3;
                BDOS(READBUF, buf);     /* get command line */
                putchar('\n');        
 
        } while ( ( nc=buf[1]) == 0 );
        
        buf[2+nc] = 0;
 
        for ( cx = &buf[2]; *cx; *cx=toupper(*cx), cx++);
 
        for ( cx = &buf[2], i=0 ; iswhite(*cx) && (i<nc) ; cx++ , i++ );
 
        return( (i==nc) ? NULL : cx );
 
}
 
 
clean()
{
        /* restore trap vectors */
}


callrt(s)
char *s;
{
    struct cpustate tstate;
    int i;
    long ret;
    long n;
    long p[10];
    PFL rout;
    char c;


    if ( ! gethex(&s,&n) ) {
	bad();
	return(0);
    }

    for (i = 0; i < 10; i ++)
	p[i] = i;

    i = 0;
    while ( (i < 10) && ( (getsep(&s)) && (gethex(&s,&p[i])) ) ) {
	i++;
    }

    for (i = 0; i < 8; i++) {
	tstate.areg[i] = state.areg[i];
	tstate.dreg[i] = state.dreg[i];
    }
    tstate.pc = state.pc;
    tstate.usp = state.usp;
    tstate.ssp = state.ssp;
    tstate.status = state.status;


    rout = (PFL)n;
    ret = (*rout)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9]);


    stout("Return value --> ");
    hexlzs(ret);
    stout("h\n");

    for (i = 0; i < 8; i++) {
	state.areg[i] = tstate.areg[i];
	state.dreg[i] = tstate.dreg[i];
    }
    state.pc = tstate.pc;
    state.usp = tstate.usp;
    state.ssp = tstate.ssp;
    state.status = tstate.status;

}
 

display(cx, dap)
char *cx;
char **dap;
{
        /* display memory in hex and ascii */
 
           short int format;    /* 1=byte, 2=word, 4=long word */
                 char *s;       /* start address */
                 char *f;       /* end   address */
                 char *i;       /* working variable */
                 char *j;       /* ditto            */
             short int c;       /* column number */ 
 
 
        /* parse command and extract parameters */
 
	format = getform(&cx);

	if (*++cx == '@') {
	    if ((*++cx >= '0') && (*cx <= '7')) {
		s = state.areg[*cx - '0'];
		f = s + ((11*16) - 1);
	    }
	    else {
		bad();
		return(0);
	    }
	}
	else {
	cx--;
        if ( ! gethex(&cx, &s) ) {        /* get start address */
           if ( nomore(cx) ) s = *dap;    /* or use default    */
           else { bad(); return(0); }     /* junk on the line  */
        }
 
        if ( (format != 1) && (s & 1) ) s += 1;   /* force even */
 
        if ( GETSEP ) {if ( ! gethex(&cx, &f) ) {bad(); return;} }
        else f = s + (11*16-1);     /* not there, use def */
 
        if ( ! nomore(cx) ) {bad(); return;} /* rest of line must be empty */
 	}
 
        /* now do the display */
 
        f = f - format + 1;  /* first byte of last chunk to print */
 
        for ( ; s <= f ; s += 16 ) { /* one line per pass */
 
                if ( keyhit() ) return;
 
                puthexl(s); putchar(' '); putchar(' '); c = 10;
 
                for ( i=s, j=min(s+15,f);
                      i <= j;
                      i += format ) {
 
                        switch ( format ) {
 
                                case 1 : puthexb(i->memb);
                                         break;
 
                                case 2 : puthexw(i->memw);
                                         break;
 
                                case 4 : puthexl(i->meml);
                                         break;
 
                        } /* end of switch */
 
                        putchar(' ');
 
                        c += (2*format+1);
 
                } /* end of loop across line */
                
                while ( c++ < 60 ) putchar(' ');
 
                for ( i=s, j=min(s+15,f); i<=j ; i++ ) putbyte(*i);
 
                putchar('\n');
 
        } /* end of loop through lines */
 
 
        *dap = f + format;
 
} /* end display */
 


ecmd(cx, basep, statep, valuep, dap, lap, bppa)
	char	 *cx;
struct	basepage *basep;
struct	cpustate *statep;
struct	value	 *valuep;
	char	**dap;
	char	**lap;
	long	 *bppa;
{
	/* Load the program named in the command */

	struct	fcb	pgmfcb;

	deblank(&cx);

	if (parse(&cx, &pgmfcb, "68K") && nomore(cx))

		loadpgm ( &pgmfcb, basep, statep, valuep, dap, lap, bppa);

	else	bad();

}

 
 
loadpgm(fcbp, basep, statep, valuep, dap, lap, bppa)

struct	fcb	 *fcbp;
struct	basepage *basep;
struct	cpustate *statep;
struct	value	 *valuep;
	char	**dap;
	char	**lap;
	long	 *bppa;
{		/* load a program for execution */
	extern	int	omega;
	struct	lpb	pgmlpb;
		int	loaderr;
	struct	basepage *newbp;
		long	*stkptr;
		int	i;
		char	sector[128];
		long	stlen;

	/* open program file */

	while(1)
	{
		if ( (BDOS(OPEN, fcbp) & 0xffL) == 0x0FFL ) 
		{
			if(fcbp->t[0] != ' ')
			{ /* open failed */
				stout("Cannot open program file\n");
				return;
			}
			else
			{
				fcbp->t[0] = '6';
				fcbp->t[1] = '8';
				fcbp->t[2] = 'K';
			}
		}
		else
			break;
	} 	
	
	/* Set up space for symbol table in top of TPA */

	fcbp->cr = 0;
	BDOS(SETDMA, sector);
	if ( BDOS(READSEQ, fcbp) & 0xffL )
	{
		stout("Can not read program file.\n");
		return;
	}
	fcbp->cr = 0;
	stlen = (&sector[14])->meml;
	HMSYMS = stlen / sizasym + (long)XTRASYM + 1L;

	if(stlen) caput = (long)caput - (long)(stlen-128L-sizlmhdr);
	if((long)caput < basep->lowtpa) {caput = NULL; HMSYMS = 0;}

	/* set up lpb */

	pgmlpb.fcbaddr = fcbp;
	pgmlpb.tpabase = basep->lowtpa;

	/* if ( stlen ) pgmlpb.tpatop = caput;
	else	     pgmlpb.tpatop = basep; */
	pgmlpb.tpatop = caput;

	pgmlpb.flags   = 0;

	/* now do program load */

	loaderr = (int)(BDOS(PGMLOAD, &pgmlpb) & 0x0FFFFL);

	switch ( loaderr ) {

		case 0:	{	/* successful */
				newbp = pgmlpb.bpaddr;
				valuep->kind = 1;
				valuep->textbase = newbp->csstart;
				valuep->textlen  = newbp->cslen;
				valuep->database = newbp->dsstart;
				valuep->datalen  = newbp->dslen;
				valuep->bssbase  = newbp->bsstart;
				valuep->bsslen   = newbp->bslen;
				valuep->bpa	 = newbp;
				valuep->initstk  = pgmlpb.stkinit;
				statep->pc = newbp->csstart;
				stkptr = pgmlpb.stkinit;
				(*--stkptr) = newbp;
				(*--stkptr) = *--bppa;
				statep->usp = stkptr;
				statep->areg[7]= stkptr;
				statep->status = 0;
				*dap = newbp->csstart;
				*lap = newbp->csstart;
				newbp->fcb1.dr = 0;
				newbp->fcb2.dr = 0;
				for ( i = 0; i < 11; ++i) {
					newbp->fcb1.fn[i] = ' ';
					newbp->fcb2.fn[i] = ' ';
				}
				newbp->fcb1.cr = 0;
				newbp->fcb2.cr = 0;
				newbp->fcb1.ex = 0;
				newbp->fcb2.ex = 0;
			}
			break;	/* end case 0 -- success */

		case 1:	{ stout("Insufficient memory or bad file header\n");
			  return;
			}
			break;

		case 2: { stout("Read error\n");
			  return;
			}
			break;

		case 3: { stout("Bad relocation bits\n");
			  return;
			}
			break;

		default: { stout("Unknown program load error\n");
			   return;
			 }
			 break;

	}  /* end switch */

	zappas();	/* Delete all pass points */
	if(stlen) omega = simz(fcbp,valuep AT textbase); /*load symbol table*/
	else	 {omega = -1; stout("No Symbols\n");}

	showvals(valuep);

}  /* end loadpgm */
 
 
fillmem(cx)
char *cx;
{
        /* fill memory with constant */
 
  register short int format;    /* 1=byte, 2=word, 4=long word */
                 char *s;       /* start address */
                 char *f;       /* end   address */
                 long  v;       /* value to stuff into memory */
 
        /* parse command and extract parameters */
 
	format = getform(&cx);

        if ( gethex(&cx, &s) );         /* get start address */
        else {bad(); return(0);}        /* not there, error  */
 
        if ( (format != 1) && (s & 1) ) /* must be even address, error */
                {bad(); return(0);}
 
        if ( GETSEP && gethex(&cx, &f) ); /* get end address  */
        else { bad(); return(0); }                /* not there, error */
 
        if ( GETSEP && gethex(&cx, &v) ); /* get value to stuff */
        else { bad(); return(0); }
 
        if ( ! nomore(cx) ) {bad(); return;} /* rest of line must be empty */
 
        if ((s>f) ||      /* test for junk or nonsense */
            ( (format == 1) && ((v > 255L)   || (v < 0)) ) ||
            ( (format == 2) && ((v > 65535L) || (v < 0)) )  )
        { bad(); return(0); }
 
 
        /* now do the stuffing */
 
        for ( ; (s+format) <= (f+1); s += format )
	{ 
                switch ( format ) {
 
                   case 1 : s->memb = (char)v;
                            if ((s->memb ^ (char)v) & 0xFF) badram(s);
                            break;
 
                   case 2 : s->memw = (short int)v ;
                            if ((s->memw ^ (short int)v) & 0xFFFF) badram(s);
                            break;
 
                   case 4 : s->meml = (long)v ;
                            if ( s->meml != v ) badram(s);
                            break;
 
		} /* end of switch */

	}   /* end of for */
 
} /* end of fillmem */

    s);
                            break;
 
		} /* end of switch */

	}   /* end of for */
 
} /* end of fillmem */

    