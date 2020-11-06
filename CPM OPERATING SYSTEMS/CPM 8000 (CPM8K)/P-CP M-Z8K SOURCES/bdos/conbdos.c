
/********************************************************
*							*
*	P-CP/M BDOS Character I/O Routines		*
*							*
*	This module does BDOS functions 1 thru 11	*
*							*
*	It contains the following functions which	*
*	are called from the BDOS main routine:		*
*		constat();				*
*		conin();				*
*		cookdout();				*
*		rawconio();				*
*		prt_line();				*
*		readline();				*
*							*
*	Copyright (c) 1982 Digital Research, Inc.	*
*							*
********************************************************/

#include "stdio.h"

#include "bdosdef.h"

#include "biosdef.h"


#define   ctrlc  0x03
#define   ctrle  0x05
#define   ctrlp  0x10
#define   ctrlq  0x11
#define   ctrlr  0x12
#define   ctrls  0x13
#define   ctrlu  0x15
#define   ctrlx  0x18

#define   cr      0x0d
#define   lf      0x0a
#define   tab     0x09
#define   rub     0x7f
#define   bs      0x08
#define   space   0x20

  
EXTERN	warmboot();		/* External function definition */
EXTERN UBYTE cpy_bi();		/* copy byte in from user space */


/******************/
/* console status */
/******************/

BOOLEAN constat()
{
    BSETUP

    return( GBL.kbchar ? TRUE : bconstat() );
}

/********************/
/* check for ctrl/s */
/* used internally  */
/********************/
conbrk()
{
    REG UBYTE ch;
    REG BOOLEAN stop;
    BSETUP

    stop = FALSE;
    if ( bconstat() ) do
    {
	if ( (ch = bconin()) == ctrlc ) warmboot(1);
	if ( ch == ctrls ) stop = TRUE;
	else if (ch == ctrlq) stop = FALSE;
	else if (ch == ctrlp) GBL.lstecho = !GBL.lstecho;
	else GBL.kbchar = ch;
    } while (stop);
}


/******************/
/* console output */
/* used internally*/
/******************/

conout(ch)
REG UBYTE ch;
{
    BSETUP

    conbrk();			/* check for control-s break */
    bconout(ch);		/* output character to console */
    if (GBL.lstecho) blstout(ch);	/* if ctrl-p on, echo to list dev */
    if (ch >= ' ') GBL.column++;	/* keep track of screen column */
    else if (ch == cr) GBL.column = 0;
    else if (ch == bs) GBL.column--;
}


/*************************************/
/* console output with tab expansion */
/*************************************/

cookdout(ch, ctlout)
REG UBYTE ch;		/* character to output */
BOOLEAN   ctlout;	/* output ^<char> for control chars? */
{
    BSETUP

    if (ch == tab) do		/* expand tabs */
	conout( ' ' );
    while (GBL.column & 7);

    else
    {
	if ( ctlout && (ch < ' ') )
	{
            conout( '^' );
	    ch |= 0x40;
	}
    conout(ch);			/* output the character */
    }
}


/*****************/
/* console input */
/*****************/

UBYTE getch()		/* Get char from buffer or bios */
			/* For internal use only	*/
{
    REG UBYTE temp;
    BSETUP

    temp = GBL.kbchar;		/* get buffered char */
    GBL.kbchar = 0;		/* clear it */
    return( temp ? temp : bconin() );	/* if non-zero, return it */
					/* else get char from bios */
}
    
UBYTE conin()		/* BDOS console input function */
{
    REG UBYTE ch;
    BSETUP

    conout( ch = getch() );
    if (ch == ctrlp) GBL.lstecho = !GBL.lstecho;
    return(ch);
}

/******************
* raw console i/o *
******************/

UBYTE rawconio(parm)	/* BDOS raw console I/O function */

REG UWORD parm;
{
    BSETUP

    if (parm == 0xff) return(getch());
    else if (parm == 0xfe) return(constat());
    else return(bconout(parm & 0xff));	/* add return to make lint happy */
}


/****************************************************/
/* print line up to delimiter($) with tab expansion */
/****************************************************/

prt_line(p)
REG UBYTE *p;
{
    BSETUP

    while( *p != GBL.delim ) cookdout( *(p++), FALSE);
}


/**********************************************/
/* read line with editing and bounds checking */
/**********************************************/

/* Two subroutines first */

newline(startcol)
REG UWORD startcol;
{
    BSETUP

    conout(cr);			/* go to new line */
    conout(lf);
    while(startcol)
    {
	conout(' ');
	startcol -= 1;		/* start output at starting column */
    }
}


backsp(p, stcol)		/* backspace one character position */
REG struct conbuf *p;		/* pointer to console buffer */
UWORD stcol;
{
    REG WORD i;
    REG WORD length;
    REG UBYTE ch;
    BSETUP

    if (p->retlen) length = UBWORD(--(p->retlen));
			 /* if buffer non-empty, decrease it by 1 */
    else length = 0;
    i = stcol;
    while (length--)
    {
	ch = p->cbuf[length];	/* get character from buffer */
	if ( ch == tab )
	{
	    i += 8;		/* i is our column counter */
	    i &= ~7;	/* for tab, go to multiple of 8 */
	}
	else if ( ch < ' ' ) i += 2;
				/* control chars put out 2 printable chars */
	else i += 1;
    }
    while (GBL.column > i)
    {
	conout(bs);		/* backspace until we get to proper column */
	conout(' ');
	conout(bs);
    }
}


readline(p)			/* BDOS function 10 */
REG struct conbuf *p;
	
{
    REG UBYTE ch;
    REG UWORD i;
    REG UWORD j;
    REG UBYTE *q;
    UWORD stcol;

    BSETUP

    stcol = GBL.column;		/* set up starting column */
    if (GBL.chainp != XNULL)	/* chain to program code  */
    {
	i = UBWORD(cpy_bi(GBL.chainp++));	/* cpy in from user space */
	j = UBWORD(p->maxlen);
	if (j < i) i = j;		/* don't overflow console buffer! */
	p->retlen = (UBYTE)i;
	q = p->cbuf;
	while (i)
	{
	    cookdout( *q++ = cpy_bi(GBL.chainp++), TRUE);
	    i -= 1;
	}
	GBL.chainp = XNULL;
	return;
    }

    p->retlen = 0;		/* start out with empty buffer */
    while ( UBWORD(p->retlen) < UBWORD(p->maxlen) )
    {				/* main loop for read console buffer */

	if ( ((ch=getch()) == ctrlc) && !(p->retlen) ) 
	{
	    cookdout(ctrlc, TRUE);
	    warmboot(1);
	}

	else if ( (ch == cr) || (ch == lf) )
	{				/* if cr or lf, exit */
	    conout(cr);
	    break;
	}

	else if (ch == bs) backsp(p, stcol);	/* backspace */

	else if (ch == rub)			/* delete character */
	{
	    if (GBL.echodel)
	    {
		if (p->retlen)
		{
		    i = UBWORD(--(p->retlen));
		    conout( p->cbuf[i] );
		}
	    }
	    else backsp(p, stcol);
	}

	else if (ch == ctrlp) GBL.lstecho = !GBL.lstecho;
						/* control-p */
	else if (ch == ctrlx)			/* control-x */
	    do backsp(p,stcol); while (p->retlen);

	else if (ch == ctrle) newline(stcol);	/* control-e */

	else if (ch == ctrlu)			/* control-u */
	{
	    conout('#'); 
	    newline(stcol);
	    p->retlen = 0;
	}

	else if (ch == ctrlr)			/* control-r */
	{
	    conout('#');
	    newline(stcol);
	    for (i=0; i < UBWORD(p->retlen); i++)
		    cookdout( p->cbuf[i], TRUE);
	}

	else					/* normal character */
	    cookdout( (p->cbuf[UBWORD((p->retlen)++)] = ch), TRUE );
    }
}
