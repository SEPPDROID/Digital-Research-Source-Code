/*****************************************************************************
*
*			_ d o s c a n   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"_doscan" is the common subroutine to "scanf", "fscanf", and
*	"sscanf".
*
*	Calling sequence:
*		ret = _doscan(sp,fmt,aps)
*	Where:
*		WORD ret	number items matched AND assigned
*				EOF when encountered on stream sp
*		FILE *sp	pointer to input stream
*		BYTE *fmt	input specification string
*		BYTE **aps	pointer to (pointers to) arguments
*
*****************************************************************************/
#include <stdio.h>
#include <ctype.h>
#define INFINITY 32767
#define NXTNI(d) if( ((d)=getc(sp)) == EOF ) return(EOF)
WORD _doscan(sp,fmt,aps)			/****************************/
	FILE *sp;				/* Stream ptr		    */
	BYTE *fmt;				/* Input format spec ptr    */
	BYTE **aps;				/* Argument ptrs ptr	    */
{						/****************************/
	REG BYTE c;				/* Spec character	    */
	REG WORD ni;				/* Input char		    */
	BOOLEAN	noassign,			/* Suppress assignment flag */
		invert,				/* Invert flag (_ismember)  */
		numfound,			/* Number found flag	    */
		longf,				/* Long ptr flag	    */
		shortf;				/* Short ptr flag	    */
	WORD	width,				/* Max field width	    */
		tval,				/* Temp val holder	    */
		base,				/* For numeric conversion   */
		nitems;				/* Number items mtchd & assd*/
	LONG	val,				/* For numeric conversion   */
		locval;				/* Local value area	    */
	BYTE	locbuf[BUFSIZ],			/* Local buffer area	    */
		*db,				/* Destination ptr	    */
		setbuf[128],			/* Area for '[...]' sets    */
		*sb;				/* Ptr into setbuf	    */
						/****************************/
	nitems = 0;				/*			    */
	while( c = *fmt++ )
	{
	    if( isspace(c) )			/* if space in spec str	    */
	    {					/*			    */
		do{				/* deblank leading blanks   */
		    c = *fmt++;			/*			    */
		} while( isspace(c) );		/*			    */
		do{				/* deblank input stream	    */
		    NXTNI(ni);			/*			    */
		} while( isspace(ni) );		/* read past space in input */
		ungetc(ni,sp);			/* back up once		    */
		if( c == ZERO )			/* check for this here	    */
		    break;			/*			    */
	    }					/* c points to non space    */
						/****************************/
	    if( c != '%' )			/* match spec char (non'%')?*/
	    {					/*			    */
		NXTNI(ni);			/* grab from input	    */
		if( ni != c )			/* 	& test   	    */
		{				/*			    */
		    ungetc(ni,sp);		/* nope, put it back	    */
		    return(nitems);		/* return early		    */
		}				/*			    */
		continue;			/* yup, do the next char    */
	    } else				/****************************/
		c = *fmt++;			/* look for an assignment   */
	    noassign = FALSE;			/* assume we will assign    */
	    if( c == '*' )			/* unless they say otherwise*/
	    {
		noassign++;
		c = *fmt++;
	    }
	    width = INFINITY;			/* assume no special width  */
	    while( c >= '0'  &&  c <= '9' )	/* unless they say otherwise*/
	    {
		if( width == INFINITY )
		    width = 0;
		width = 10 * width + (c - '0');
		c = *fmt++;
	    }
	    longf = FALSE;			/* assume ptrs not to LONGs */
	    shortf = FALSE;			/*   and not to SHORTs	    */
	    if( c == 'l' )			/* unless they say otherwise*/
	    {
		longf++;
		c = *fmt++;
	    } else if( c=='h' )
	    {
		shortf++;
		c = *fmt++;
	    }

						/****************************/
	    switch( c )				/* which conversion char?   */
	    {					/****************************/
	    case 'D':				/* decimal input	    */
	    case 'd':
		base = 10; goto procnum;
	    case 'O':				/* octal input		    */
	    case 'o':
		base = 8; goto procnum;
	    case 'X':				/* hex input		    */
	    case 'x':
		base = 16;
	    procnum:				/****************************/
		if( isupper(c) ) {		/* upper case means long    */
		    c = tolower(c);
		    longf++;
		}
		do{
		    NXTNI(ni);			/* grab a char from input   */
		} while( isspace(ni) );		/* until non-blank	    */
		if( noassign )
		     db = &locval;		/* got to put it somewhere  */
		else db = *aps++;		/* like to put it here	    */
		invert = FALSE;			/* assume pos number	    */
		if( ni == '+' ) {		/* leading plus		    */
		    width--;
		    NXTNI(ni);
		} else if( ni == '-' ) {	/* leading minus	    */
		    invert++;
		    width--;
		    NXTNI(ni);
		}
		val = 0L;			/* initial value	    */
		numfound = FALSE;		/* assume guilty til proven */
		while( width-- > 0 ) {		/* for each char ************/
		    tval = ni = toupper(ni);	/* should work for numbers  */
		    if( tval<'0'  ||  tval>'F')
			break;
		    if( tval>'9'  &&  tval<'A')
			break;
		    tval -= '0';
		    if( tval>9 )
			tval = tval-'A'+'0'+10;
		    if( tval >= base )
			break;
		    numfound++;			/* is a number		    */
		    val = base * val + tval;	/*			    */
		    NXTNI(ni);			/* next input, please	    */
		}				/*	*********************/
		ungetc(ni,sp);			/* back off input	    */
		if( numfound == FALSE )		/* was it there?	    */
		    return(nitems);		/*   no, too bad	    */
		if( invert )			/* leading minus?	    */
		    val = -val;			/*			    */
		if( longf )			/* store this value	    */
		    *( long *)db = val;		/* somewhere		    */
		else if( shortf )		/*			    */
		    *( short *)db = val;	/* somehow		    */
		else				/*			    */
		    *( int *)db = val;		/* sometime		    */
		if( !noassign )			/* increment only if assign */
		    nitems++;			/*			    */
		break;				/****************************/

	    case 's':				/* string input		    */
	    case 'c':				/* char input		    */
	    case '[':				/* input 'one of'	    */
		NXTNI(ni);			/* grab a character	    */
		if( c== 's' )			/* scan string **************/
		{
		    sb = " \t\n";		/* point set to white chars */
		    while(_ismem(ni,sb,FALSE))	/* keep reading whitespace  */
			NXTNI(ni);		/*  if there is any	    */
		    invert = TRUE;		/* after, read until whites */
		    width--;			/* decr, lv room for NULL   */
		} else if( c == 'c' )		/* scan char ****************/
		{
		    sb = "";			/* Null set		    */
		    if( width == INFINITY )	/* handle default width	    */
			width = 1;
		    invert = TRUE;		/* invert "_ismem" func	    */
		} else				/* scan set *****************/
		{				/* must be a '['	    */
		    invert = FALSE;
		    if( (c = *fmt++) == '^' )	/* invert "_ismem" ???	    */
		    {
			invert = TRUE;
			c = *fmt++;
		    }
		    for( sb=setbuf; c != ZERO && c != ']'; sb++, c = *fmt++)
			*sb = c;
		    *sb = ZERO;
		    sb = setbuf;
		}
		if( noassign )
		    db = locbuf;		/* give rtn somewhere to pt */
		else db = *aps++;		/* o.w. grab the desired ptr*/
		while( width-- > 0  && _ismem(ni,sb,invert) )
		{
		    *db++ = ni;
		    NXTNI(ni);
		}
		ungetc(ni,sp);			/* we've always read 1 past */
		if( c != 'c' )			/* not char type?	    */
		    *db = ZERO;			/* then null terminate	    */
		if( !noassign )
		    nitems++;			/* successful assignment    */
		break;				/****************************/
						/****************************/

	    case 'E':		/*********** not implemented ****************/
	    case 'e':
	    case 'F':
	    case 'f':

	    case '%':
	    default:
		NXTNI(ni);
		if( ni != c )			/* must match c		    */
		{
		    ungetc(ni,sp);
		    return(nitems);
		}
		break;
	    }					/***** end switch ***********/
	}					/***** end while loop *******/
	return(nitems);				/*			    */
}						/****************************/

						/****************************/
MLOCAL WORD _ismem(c,set,invert)		/* is_member of a set	    */
	BYTE c;					/* candidate for set element*/
	BYTE *set;				/* null terminated string   */
	BOOLEAN invert;				/* invert sense of function */
{						/****************************/
	REG WORD rv;				/* temp return val	    */
	rv = index(set,c);			/* look for c in set	    */
	return( invert ? !rv : rv );		/* return (maybe inverted) #*/
}						/****************************/
