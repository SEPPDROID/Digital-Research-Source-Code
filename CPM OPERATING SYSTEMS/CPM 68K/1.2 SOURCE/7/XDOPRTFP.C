/****************************************************************************/
/*									    */
/*			X d o p r t f p   R o u t i n e			    */
/*			-------------------------------			    */
/*									    */
/*	This file contains subroutines called from "_doprt()" which are	    */
/*	specific to floating point.  The purpose of having a separate file  */
/*	is so that these routines may be declared global in a special 	    */
/*	version of "s.o", to allow running without the floating point 	    */
/*	library routines.						    */
/*									    */
/*	Entry Points:							    */
/*									    */
/*		petoa(^float, ^buff, prec);				    */
/*		pftoa(^float, ^buff, prec);				    */
/*									    */
/*	^float	is a pointer to the floating number to convert		    */
/*	^buff	is a pointer to the buffer				    */
/*	prec	is the precision specifier				    */
/*									    */
/****************************************************************************/
#include <portab.h>			/*				    */
BYTE	*ftoa();			/* Converts float to ascii "F" fmt  */
BYTE	*etoa();			/* Converts float to ascii "E" fmt  */
					/************************************/
BYTE	*pftoa(addr,buf,prec)		/* Print "F" format		    */
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
BYTE	*petoa(addr,buf,prec)		/* Print "E" format		    */
FLOAT	*addr;				/* -> Number to convert		    */
BYTE	*buf;				/* -> Output buffer		    */
WORD	prec;				/* Fraction precision specifier	    */
{					/************************************/
	FLOAT	fp;			/* Floating temp		    */
	prec = (prec < 0) ? 6 : prec;	/* If < 0, make it 6		    */
	fp = *addr;			/* Load temp			    */
	return(etoa(fp,buf,prec));	/* Do conversion		    */
}					/************************************/
;			/* Load temp			    */
	return(etoa(fp,buf,prec));	/* Do conversion		    */
}					/***********************************/
;			/* Load temp			    */
	return(etoa(fp,buf,prec));	/* Do conversion		    */
}					/***********************************/
;			/* Load temp			    */
	return(etoa(fp,buf,prec));	/* Do conversion		    */
}					/***********************************/
;			/* Load temp			    */
	return(etoa(fp,buf,prec));	/* Do conversion		    */
}					/***