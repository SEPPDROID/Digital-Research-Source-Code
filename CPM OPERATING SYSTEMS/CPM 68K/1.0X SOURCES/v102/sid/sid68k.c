/********************************************************/
/*							*/
/*	Usual filename:  SID68K.C			*/
/*	Remarks:  Main C module for SID68K		*/
/*	Author:  Timothy M. Benson			*/
/*	Control:  19 MAY 83  18:01  (TMB)		*/
/*							*/
/********************************************************/

#include "lgcdef.h"
#include "siddef.h"
#include "ddtinc.h"
#include "stdio.h"
#include "bdosfunc.h"
#include "disas.h"

/**temp** For now, let symbol table preceded by load module 	*/
/**temp**	header be physically located in SID.  May later	*/
/**temp**	allocate it dynamically.			*/
struct lmhedr klops;

/**temp** And here's a pointer to it:	*/
struct lmhedr *caput = NULL;	/* Symbol table--known globally */
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

main(bpptr)
struct basepage *bpptr;
{
        char combuf[BUFLEN];
        char *cx;
        char *dispaddr; /* current display address */
	char *listaddr; /* current disassembly addr*/
        struct cpustate state;  /* state of user program registers */
	struct value curvals;	/* values relating to current file or pgm */
        register int i; /* workhorse integer */
 
        /* phony machine state initialization */
        for (i=0; i<8; i++) {state.dreg[i]=0L; state.areg[i]=0L;}
        state.pc = 0x0;
        state.usp = 0x00001000L;
	state.areg[7]=state.usp;
        state.ssp = GETSSP();
        state.status = 0x0000;
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
 
        stout("\nSID-68K   Nonrelease: -4.8   20 May 1983\n");
	stout("Copyright 1982,1983 Digital Research\n\n");

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
 
        for ( cx= &buf[2], i=0 ; iswhite(*cx) && (i<nc) ; cx++ , i++ );
 
        return( (i==nc) ? NULL : cx );
 
}
 
 
clean()
{
        /* restore trap vectors */
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

        if ( ! gethex(&cx, &s) ) {        /* get start address */
           if ( nomore(cx) ) s = *dap;    /* or use default    */
           else { bad(); return(0); }     /* junk on the line  */
        }
 
        if ( (format != 1) && (s & 1) ) s += 1;   /* force even */
 
        if ( GETSEP ) {if ( ! gethex(&cx, &f) ) {bad(); return;} }
        else f = s + (12*16-1);     /* not there, use def */
 
        if ( ! nomore(cx) ) {bad(); return;} /* rest of line must be empty */
 
 
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
	
	/* set up lpb */

	pgmlpb.fcbaddr = fcbp;
	pgmlpb.tpabase = basep->lowtpa;
	pgmlpb.tpatop  = basep;
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
	omega = simz(fcbp,valuep AT textbase);
				/* Load symbol table */
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
