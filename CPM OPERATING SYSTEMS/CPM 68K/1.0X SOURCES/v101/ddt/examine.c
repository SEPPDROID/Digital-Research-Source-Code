/****************************************************************/
/*                                                              */
/*      Examine/Alter CPU Registers                             */
/*                                                              */
/****************************************************************/
 
#include "ddtinc.h"
 
examine(cx, statep)
char *cx;
struct cpustate *statep;
{
        if        (nomore(cx))    showstate(statep);
        else if (*++cx == 'D')    pdareg('D', cx, statep->dreg);
        else if   (*cx == 'A')    pdareg('A', cx, statep->areg);
        else if (cmp("PC",--cx))  preg("PC", &(statep->pc));
        else if (cmp("USP",cx))   { if (preg("USP",&(statep->usp)) &&
                                        ((statep->status & SUPER) == 0 ))
                                        statep->areg[7] = statep->usp;
             }
        else if (cmp("SSP",cx))   { if (preg("SSP",&(statep->ssp)) &&
                                        (statep->status & SUPER)  )
                                        statep->areg[7] = statep->ssp;
             }
        else                      tryflags(cx, statep); 
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
        pregrow('D', statep->dreg);     /* D registers */
        pregrow('A', statep->areg);     /* A registers */
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
                preg(str, rp + *cx - '0');
        }
}
 
 
preg(rname, regp)       /* print register contents */
register char *rname;
register long *regp;
{
        char buf[BUFLEN];
        register short int nc;
        long int newval;
        char *bp;
        register int modify; 
 
        modify = 0;
        stout(rname);
        putchar('=');
        puthexl(*regp);
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
                   *regp = newval;
                   modify = 1;
                }
                else if ( ! nomore(buf+1) ) bad();
        }
        return(modify);
}
 
 
 
pregrow(ch, rowp)
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
        stout("Error or unimplemented function.");
}
 
 
cmp(str, ctl)
register char *str;
register char *ctl;
{
        while (*str && (*str++ == *++ctl)) ;
        if ( ( ! *str) && nomore(ctl) ) return(1);
        else return(0);
}
