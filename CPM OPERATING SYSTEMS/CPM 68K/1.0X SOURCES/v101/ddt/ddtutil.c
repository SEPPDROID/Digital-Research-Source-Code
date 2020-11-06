/****************************************************************/
/*                                                              */
/*      DDT-68K (tm)    Utility Procedures                      */
/*                                                              */
/*      Copyright 1982, Digital Research                        */
/*                                                              */
/****************************************************************/
 
#include "ddtinc.h"
#include "stdio.h"
#include "bdosfunc.h"
#include "disas.h"


putchar(x)
register char x;
{
	BDOS(CONOUT, (long)x);
	if (x == '\n') BDOS(CONOUT, (long)'\r');
}
 
 
int stout(s)            /* write a string */
register char *s;
{
        register int nc;
 
        nc = 0;
        while ( *s ) { putchar( *s++ ); nc++; }
        return(nc);
}
 
 
 
 
int gethex ( cpp, np )  /* get longest possible hex number from string     */
char **cpp;             /* +*cpp, and store it in *np.  return # of digits */
long *np;
{
        register char *cp;
        register long n;
        register short int nd;
 
        for ( n=0, nd=0, cp = *cpp;
              ishex(*++cp);
              nd++, n=(n<<4)+hexval(*cp) );
 
        *cpp = --cp;
        *np  = n;
	return(nd);
}
 
 
 
 
int ishex ( c )
register char c;
{
        return ( ( ('0'<=c) && (c<='9') ) || ( ('A'<=c) && (c<='F') ) );
}
 
 
 
int hexval ( c )
register char c;
{
        if ( ('0'<=c) && (c<='9') ) return ( c-'0');
        else return ( 10 + c - 'A' );
}
 
 
 
char hexchar(n)
register char n;
{
         return ( n<10 ? '0'+n : 'A'+n-10 );
}
 



int puthex(n, i, zs)
register long n;
register int i;
register int zs;
{
	register char d;

	while ( i >= 4 )
	{
		i -= 4;
		d = hexchar( (char)((n>>i) & 0xf) );
		if ( d != '0' ) zs = 0;
		if ( (! zs) || (! i) ) putchar(d);
	}
}

 
int puthexl(n)  /* write a long integer in hex */
long n;
{
	puthex(n, 32, 0);
}
 
 
int puthexw(n)  /* write an integer in hex */
short int n;
{
	puthex((long)n, 16, 0);
}
 
 
 
puthexb(n)  /* write a byte in hex */
char n;
{
	puthex((long)n, 8, 0);
}

 
 
int putbyte(c)  /* write an ascii byte.  if not printable, write '.' */
register char c;
{
        char d[2];
 
        d[1] = 0;
        d[0] = ( ( (c < 0x20) || (c > 0x7E) ) ? '.' : c );
        stout(d);
}
 
 
        
bad()
{
         stout("?\n");
}
 
badram(a)
long a;
{
        stout("Bad or non-existent RAM at ");
        puthexl(a);
        putchar('\n');
}


nomore(cx)
char *cx;
{
	++cx;
	while (*cx)
	{
		if (*cx != ' ')	return(0);
		else ++cx;
	}
	return(1);
}
 
 
getsep(cxp)
register char **cxp;
{
        register char *cx;
 
        cx = (*cxp) + 1;
        if ((*cx != ' ') && (*cx != ',')) return(0);
        while ( *cx == ' ' ) ++cx;
        if ( ! *cx ) return(0);
        if ( *cx != ',' ) {*cxp = --cx; return(1);}
        ++cx;
	while ( *cx == ' ' ) ++cx;
        *cxp = --cx;
        return(1);
}               


deblank(cxp)
register char **cxp;
{
	++*cxp;
	while ( **cxp == ' ' ) ++*cxp;
	--*cxp;
}
 


short int getform(cxp)
register char **cxp;
{
	register char *cx;
	register short int format;

	cx = *cxp;

	if ( *++cx == 'W' )	format = 2;
	else if ( *cx == 'L' )	format = 4;
	else { format = 1; cx--; }

	*cxp = cx;

	deblank(cxp);

	return(format);
}



parse(cxp, fcbp, dtp)
register char **cxp;
struct fcb *fcbp;
char *dtp;
{
	/* parse a filename into an fcb */

	register char *cx;
	register char  c;
	register int   n;

	fcbp->ex = 0;
	fcbp->cr = 0;

	cx = *cxp;
	c  = *++cx;

	if ( ! (isalpha(c) || isdigit(c) || iswild(c))) return(0);
	if ( *(cx+1) == ':' ) {fcbp->dr = c - 'A' + 1; cx += 2; }
	else fcbp->dr = 0;

	n = 0;
	c = *cx;
	while (isalpha(c) || isdigit(c) || iswild(c)) {
		if ( n >= 8 ) return(0);
		if(c == '*')
		{
			while(n < 8)
				fcbp->fn[n++] = '?';
			c = *++cx;
			break;
		}
		fcbp->fn[n++] = c;
		c = *++cx;
	}

	while ( n < 8 ) fcbp->fn[n++] = ' ';

	for (n = 0; n < 3; ++n) {
		if (*dtp) fcbp->t[n] = *dtp++;
		else	  fcbp->t[n] = ' ';
	}

	if (*cx != '.') {*cxp = --cx; return(1);}

	n = 0;
	c = *++cx;
	while ( isalpha(c) || isdigit(c) || iswild(c)) {
		if ( n >= 3 ) return(0);
		if(c == '*')
		{
			while(n < 3)
				fcbp->t[n++] = '?';
			c = *++cx;
			break;
		}
		fcbp->t[n] = c;
		++n;
		c = *++cx;
	}

	while ( n < 3 ) fcbp->t[n++] = ' ';

	*cxp = --cx;
	return(1);

}	/* end of parse fcb */



int keyhit()
{
	if ( BDOS(CONSTAT, 0L) & 0xFFFFL ) {
		BDOS(CONIN, 0L);
		return(1);
	}
	else return(0);
}


showvals(vp)
register struct value *vp;
{
	register int i, j;
	register long * lp;

	switch ( vp->kind ) 
	{
	   case 0: stout("\nERROR, no program or file loaded.\n");
		   break;

	   case 1: /* do program vals */
		   for ( i = 0; i < 2; ++i )
		   {
			lp = &(vp->textbase);
			lp += i;
			for (j = 0; j < 3; ++j)
			{
			   switch ( j )
			   {
				case 0: stout("text "); break;
				case 1: stout("data "); break;
				case 2: stout("bss ");  break;
			   }
			   switch ( i )
			   {
				case 0: stout(" base  "); break;
				case 1: stout(" length"); break;
			   }
			   stout(" = ");
			   puthexl(*lp);
			   lp += 2;
			   stout("   ");
			}
		   	putchar('\n');
		   }
		   stout("base page address = ");
		   puthexl(vp->bpa);
		   stout("       initial stack pointer = ");
		   puthexl(vp->initstk);
		   break; /* end of program values */

	   case 2: /* do file values */
		   stout("Start = ");
		   puthexl(vp->textbase);
		   stout("     End = ");
		   puthexl((vp->textbase)+(vp->textlen)-1L);
		   break;
	}

	putchar('\n');

} /* end of showvals */
 
/**/


/****************************************************************/
/*                                                              */
/*      Examine/Alter CPU Registers                             */
/*                                                              */
/****************************************************************/
 
 
examine(cx, statep)
char *cx;
struct cpustate *statep;
{
        if        (nomore(cx))    showstate(statep);
        else if (*++cx == 'D')    pdareg('D', cx, statep->dreg);
        else if   (*cx == 'A')    pdareg('A', cx, statep->areg);
        else if (cmp("PC",--cx))  pregl("PC", &(statep->pc));
        else if (cmp("USP",cx))   { if (pregl("USP",&(statep->usp)) &&
                                        ((statep->status & SUPER) == 0 ))
                                        statep->areg[7] = statep->usp;
             }
        else if (cmp("SSP",cx))   { if (pregl("SSP",&(statep->ssp)) &&
                                        (statep->status & SUPER)  )
                                        statep->areg[7] = statep->ssp;
             }
        else if (cmp("ST", cx))	  { pregw("ST",&(statep->status) );
		if ( statep->status & SUPER)	statep->areg[7] = statep->ssp;
		else				statep->areg[7] = statep->usp; 
	     }
	else bad();

	if ( statep->status & SUPER)	statep->ssp = statep->areg[7];
	else				statep->usp = statep->areg[7]; 
}
 
 
 
showstate(statep)
register struct cpustate *statep;
{
        register short int status;
 
        stout("PC=");  puthexl(statep->pc);  putchar(' ');
        stout("USP="); puthexl(statep->usp); putchar(' ');
        stout("SSP="); puthexl(statep->ssp); putchar(' ');
        stout("ST=");  puthexw(status = statep->status);
        stout("=>");
        if (status & TRACE)  stout("TR ");
        if (status & SUPER)  stout("SUP ");
        stout("IM=");  putchar(((status & INTMSK)>>8)+'0');
        if (status & EXTEND) stout(" EXT");
        if (status & NEG)    stout(" NEG");
        if (status & ZERO)   stout(" ZER");
        if (status & OFLOW)  stout(" OFL");
        if (status & CARRY)  stout(" CRY");
        putchar('\n');
        preglrow('D', statep->dreg);     /* D registers */
        preglrow('A', statep->areg);     /* A registers */
        dot = statep -> pc;
        pinstr();                       /* disassembled instruction */
        putchar('\n');
}
 
 
 
pdareg(da, cx, rp)      /* print data or address register contents */
char  da;
register char *cx;
long *rp;
{
 
        char str[3];
 
        if ( ('0' <= *++cx) && (*cx <= '7') && nomore(cx) ) {
                str[0] = da;
                str[1] = *cx;
                str[2] = 0;
                pregl(str, rp + *cx - '0');
        }
	else bad();
}


pregl(rname, regp)       /* print register contents as long */
char *rname;
long *regp;
{
	return( preg(rname, regp, 4) );
}

pregw(rname, regp)       /* print register contents as word */
char *rname;
long *regp;
{
	return( preg(rname, regp, 2) );
}

 
 
preg(rname, regp, size)       /* print register contents */
register char *rname;
register long *regp;
register int   size;
{
        char buf[BUFLEN];
        register short int nc;
        long int newval;
        char *bp;
        register int modify; 
 
        modify = 0;
        stout(rname);
        putchar('=');
 	switch(size)
	{
		case 1 : puthexb(regp->memb);
			 break;
		case 2 : puthexw(regp->memw);
			 break;
		case 4 : puthexl(regp->meml);
			 break;
	}
        putchar(' ');
        *buf = BUFLEN-2;
        BDOS(READBUF, buf);
	putchar('\n');
        if ((nc=buf[1])>0) {
                buf[nc+2]=0;
                bp = buf + 1;
                while (*++bp) *bp = toupper(*bp);
                bp = buf + 1;
                if (gethex(&bp, &newval) && nomore(bp)) {
                   switch(size)
		   {
			case 1 : regp->memb = (char)(newval & 0xffL);
				 break;
			case 2 : regp->memw = (int)(newval & 0xffffL);
				 break;
			case 4 : regp->meml = newval;
				 break;
		   }
                   modify = 1;
                }
                else if ( ! nomore(buf+1) ) bad();
        }
        return(modify);
}
 
 
 
preglrow(ch, rowp)
char ch;
long *rowp;
{
        register int n;
        
        putchar(ch);
        putchar(' ');
        for (n=0; n<8; n++) {
                putchar(' ');
                puthexl(*rowp++);
                if (n==3) putchar(' ');
        }
        putchar('\n');
}
 
 
tryflags(cx, statep)
register char *cx;
register struct cpustate *statep;
{
	bad();
}
 
 
cmp(str, ctl)
register char *str;
register char *ctl;
{
        while (*str && (*str++ == *++ctl)) ;
        if ( ( ! *str) && nomore(ctl) ) return(1);
        else return(0);
}


/****************/

