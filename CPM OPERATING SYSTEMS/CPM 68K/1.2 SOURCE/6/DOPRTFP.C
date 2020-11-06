/***************************************************************************
*									    
*			d o p r t f p   R o u t i n e			    
*			-----------------------------			    
*									    
*	This file contains subroutines called from "_doprt()" which are	    
*	specific to floating point.  The purpose of having a separate file  
*	is so that these routines may be declared global in a special 	    
*	version of "s.o", to allow running without the floating point 	    
*	library routines.						    
*									    
*	Entry Points:							    
*									    
*		_petoa(^float, ^buff, prec);				    
*		_pftoa(^float, ^buff, prec);				    
*		_pgtoa(^float, ^buff, prec);				    
*									    
*	^float	is a pointer to the floating number to convert		    
*	^buff	is a pointer to the buffer				    
*	prec	is the precision specifier				    
*									    
*	Modifications:							    
*	1/84 whf - change to include "osif.h"
*	8/83 whf - add in DRC floating point call			    
*	10/83 whf - get it to work on DRC				    
*									    
*****************************************************************************/
					/*				    */
#include "portab.h"			/*				    */
#include "osif.h"			/*				    */
					/*				    */
/****************************************************************************/
#if CPM68K	/* I hope to resolve these differences someday. whf 8/83    */
/****************************************************************************/
BYTE	*ftoa();			/* Converts float to ascii "F" fmt  */
BYTE	*etoa();			/* Converts float to ascii "E" fmt  */
					/************************************/
BYTE	*_pftoa(addr,buf,prec)		/* Print "F" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
{					/************************************/
	FLOAT	fp;			/* Float temp			    */
					/************************************/
	prec = (prec < 0) ? 6 : prec;	/* If < 0, make it 6		    */
	fp = *addr;			/* Load float number		    */
	return(ftoa(fp,buf,prec));	/* Do conversion		    */
}					/************************************/
					/*				    */
BYTE	*_petoa(addr,buf,prec)		/* Print "E" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
{					/************************************/
	FLOAT	fp;			/* Floating temp		    */
	prec = (prec < 0) ? 6 : prec;	/* If < 0, make it 6		    */
	fp = *addr;			/* Load temp			    */
	return(etoa(fp,buf,prec));	/* Do conversion		    */
}					/************************************/
					/*				    */
BYTE	*_pgtoa(addr,buf,prec)		/* Print "G" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
{					/************************************/
	BYTE 	*sp;			/* String temp			    */
					/************************************/
	sp = _pftoa(addr, buf, prec);	/* Try F first			    */
	if(strlen(buf) > (7+prec))	/* Smallest FP string		    */
	    sp = _petoa(addr,buf,prec);	/* Do E format instead		    */
	return sp;			/*				    */
}					/************************************/
					/*				    */
/****************************************************************************/
#else		/* for DRC */
/****************************************************************************/
					/*				    */
BYTE *ftoa();				/* General purpose floating point   */
					/*				    */
BYTE	*_drcftoa(addr,buf,prec,kind)	/* Use DRC ftoa routine		    */
	DOUBLE	*addr;			/* -> Number to convert		    */
REG	BYTE	*buf;			/* -> Output buffer		    */
REG	WORD	prec;			/* Fraction precision specifier	    */
	BYTE	kind;			/* Kind of convert (f, e, or g)	    */
{					/************************************/
REG	BYTE *ss;			/* Temp pointer for adjustment	    */
REG	WORD nn;			/* Counter			    */
	WORD len;			/*				    */
					/*				    */
	ss = buf;			/* Save til later		    */
	if( prec < 0)   prec = 6;	/* If <0, make it 6		    */
	if( prec > 17 ) prec = 17;	/* If >17, make it 17		    */
	if( kind == 'f' ) nn = 17;	/* use max. precision		    */
	else if( kind == 'e' || kind == 'E' )
		nn = prec + 1;
	else if( (nn = prec) == 0 )	/* G format			    */
		++nn;
	if (nn > 17) nn = 17;
	*buf = ' ';
	ftoa(addr, &buf[1], 80, nn, kind); /* go convert		    */
	if( kind == 'f' )
		_ffmt(ss,prec);
	len = strlen(buf);
	if(buf[len-1] == '.')		/* Zap trailing decimal point	    */
	    buf[len-1] = NULL;		/*				    */
	while( *buf == ' ' )		/* Zap leading blanks		    */
	    ++buf;			/*				    */
	if( ss != buf )			/* Did we zap leading blanks?	    */
	    strcpy(ss,buf);		/* Ifso, shift everything into place*/
	return ss;			/*				    */
}					/************************************/

BYTE	*_pftoa(addr,buf,prec,ch)	/* Print "F" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
BYTE	ch;				/* Char format specifier	    */
{					/************************************/
	return(_drcftoa(addr,buf,prec,ch));/* Do conversion		    */
}					/************************************/
					/*				    */
BYTE	*_petoa(addr,buf,prec,ch)	/* Print "E" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
BYTE	ch;				/* Char format specifier	    */
{					/************************************/
	return(_drcftoa(addr,buf,prec,ch));/* Do conversion		    */
}					/************************************/
					/*				    */
BYTE	*_pgtoa(addr,buf,prec,ch)	/* Print "G" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
BYTE	ch;				/* Char format specifier	    */
{					/************************************/
	return(_drcftoa(addr,buf,prec,ch));/* Do conversion		    */
}					/************************************/
/****************************************************************************/


MLOCAL _ffmt(s, prec)
char *s;
register  prec;
{
	register char *p;
	register int  i;
	char *dot, lastdig;

	p = s;
	while (*++p) {
	    if (*p == '.') dot = p;		/* found decimal pt. */
	    else if (*p == 'e') {
		s = dot + prec + 1;
		if (s < p)
		    while (*s++ = *p++);	/* remove extra zeros */
		    return;
	    }
	}
	i = p - dot - 1;
	while (i++ < prec)
		*p++ = '0';			/* fill trailing zeros */
	p = dot + prec + 1;
	lastdig = *p;
	*p = '\0';
	if (lastdig >= '5') {
	    while (1) {				/* round preceeding digit */
		if (*--p == '.') p--;
		if ((*p += 1) <= '9') break;
		*p = '0';
	    }
	    if (*p < '1') {
		if (*p == '.') *(p-1) = '-';
		*p = '1';
	    }
	}
}


#endif
/****************************************************************************/
*******/
*******/
