/****************************************************************************/
/*									    */
/*			_ P r i n t f   M o d u l e			    */
/*			---------------------------			    */
/*									    */
/*	This module is called through the single entry point "_printf" to   */
/*	perform the conversions and output for the library functions:	    */
/*									    */
/*		printf	- Formatted print to standard output		    */
/*		fprintf - Formatted print to stream file		    */
/*		sprintf - Formatted print to string			    */
/*									    */
/*	The calling routines are logically a part of this module, but are   */
/*	compiled separately to save space in the user's program when only   */
/*	one of the library routines is used.				    */
/*									    */
/*	The following routines are present:				    */
/*									    */
/*		_printf		Internal printf conversion / output	    */
/*		_prnt8		Octal conversion routine		    */
/*		_prntx		Hex conversion routine			    */
/*		__conv		Decimal ASCII to binary routine		    */
/*		_putstr		Output character to string routine	    */
/*		_prnt1		Decimal conversion routine		    */
/*									    */
/*	The following routines are called:				    */
/*									    */
/*		strlen		Compute length of a string		    */
/*		putc		Stream output routine			    */
/*		ftoa		Floating point output conversion routine    */
/*									    */
/*									    */
/*	This routine depends on the fact that the argument list is always   */
/* 	composed of LONG data items.					    */
/*									    */
/*	Configured for Whitesmith's C on VAX.  "putc" arguments are 	    */
/*	reversed from UNIX.						    */
/*									    */
/****************************************************************************/

/*
 * 	Include files:
 */
#include	<stdio.h>			/* just the standard stuff */


/*
 *	Local DEFINEs
 */
#define	HIBIT	31				/* High bit number of LONG  */


/*
 *	Local static data:
 */						/****************************/
	MLOCAL BYTE	*_ptrbf = 0;		/* Buffer Pointer	    */
	MLOCAL BYTE	*_ptrst = 0;		/* -> File/string (if any)  */
	MLOCAL BYTE	*__fmt  = 0;		/* Format Pointer	    */
						/****************************/

/*****************************************************************************
*
*		P R I N T F   I N T E R N A L   R O U T I N E
*		---------------------------------------------
*
*	Routine "_printf" is used to handle all "printf" functions, including
*	"sprintf", and "fprintf".
*
*	Calling Sequence:
*
*		_printf(fd,func,fmt,arg1);
*
*	Where:
*
*		fd		Is the file or string pointer.
*		func		Is the function to handle output.
*		fmt		Is the address of the format string.
*		arg1		Is the address of the first arg.
*
*
*	Returns:
*
*		Number of characters output
*
*	Bugs:
*
*	It is assumed that args are contiguous starting at "arg1", and that
*	all are the same size (LONG), except for floating point.
*
*
*****************************************************************************/
_printf(fd,f,fmt,a1)				/****************************/
	LONG	fd;				/* Not really, but ...	    */
	LONG	(*f)();				/* Function pointer	    */
	BYTE	*fmt;				/* -> Format string	    */
	LONG	*a1;				/* -> Arg list		    */
{						/****************************/
	LOCAL BYTE	c;			/* Format character temp    */
	LOCAL BYTE	*s;			/* Output string pointer    */
	LOCAL BYTE	adj;			/* Right/left adjust flag   */
	LOCAL BYTE	buf[30];		/* Temporary buffer	    */
						/****************************/
	LOCAL LONG	*adx;			/* Arg Address temporary   */
	LOCAL LONG	x;			/* Arg Value   temporary   */
	LOCAL LONG	n;			/* String Length Temp	   */
	LOCAL LONG	m;			/* Field  Length Temporary */
	LOCAL LONG	width;			/* Field width		   */
	LOCAL LONG	prec;			/* Precision for "%x.yf"   */
	LOCAL LONG	padchar;		/* '0' or ' ' (padding)	   */
	LOCAL DOUBLE	zz;			/* Floating temporary	   */
	LOCAL DOUBLE	*dblptr;		/* Floating temp. address  */
	LOCAL LONG	ccount;			/* Character count	   */
	EXTERN		_putstr();		/* Reference function	   */
						/***************************/

						/***************************/
	ccount = 0;				/* Initially no characters */
	_ptrbf = buf;				/* Set buffer pointer	   */
	adx = a1;				/* Copy address variable   */
	 _ptrst = fd;				/* Copy file descriptor	   */
	__fmt = fmt;				/* Copy format address	   */
						/***************************/
	if(*__fmt == 'L' || *__fmt == 'l')	/* Skip long output	   */
		__fmt++;			/*	conversions	   */
						/*			   */
/*************************************************			   */
/* This is the main format conversion loop.  Load a character from the 	   */
/* format string.  If the character is '%', perform the appropriate 	   */
/* conversion.  Otherwise, just output the character.			   */
/*************************************************			   */
						/*			   */
	while( c = *__fmt++ )			/* Pick up next format char*/
	{					/*			   */
	  if(c != '%') 				/***************************/
	  {					/*			   */
	    (*f)(fd,c);				/* If not '%', just output */
	    ccount++;				/* Bump character count    */
	  }					/***************************/
          else 					/* It is a '%',		   */
	  { 					/* 		convert    */
	    x = *adx++;				/* x = address of next arg */
						/***************************/
	    if( *__fmt == '-' )			/* Check for left adjust   */
	    {					/***************************/
	      adj = 'l';  			/* Is left, set flag	   */
	      __fmt++; 				/* Bump format pointer	   */
	    }					/*			   */
	    else 				/* Right adjust		   */
	      adj = 'r';			/***************************/
						/*			   */
	    padchar=(*__fmt=='0') ? '0' : ' ';	/* Select Pad character	   */
						/***************************/
            width = __conv();			/* Convert width (if any)  */
						/***************************/
	    if( *__fmt == '.')			/* '.' means precision spec*/
	    {					/*			   */
	      ++__fmt; 				/* Bump past '.'	   */
	      prec = __conv();			/* Convert precision spec  */
	    }					/*			   */
	    else 				/* None specified	   */
	      prec = 0;				/***************************/
						/*			   */
	    s = 0;				/* Assume no output string */
	    switch ( c = *__fmt++ ) 		/* Next char is conversion */
	    {					/*			   */
	      case 'D':				/* Decimal 		   */
	      case 'd':				/*			   */
		_prt1(x); 			/* Call decimal print rtn  */
		break;				/* Go do output		   */
						/***************************/
	     case 'o':				/* Octal		   */
	     case 'O':				/*	  Print		   */
	       _prnt8(x); 			/* Call octal printer	   */
	       break;				/* Go do output		   */
						/***************************/
	     case 'x':				/* Hex			   */
	     case 'X':				/*	Print		   */
	       _prntx(x); 			/* Call conversion routine */
	       break;				/* Go do output		   */
						/***************************/
	     case 'S':				/* String		   */
	     case 's':    			/*	   Output?	   */
	       s=x;				/* Yes, (easy)		   */
	       break;				/* Go finish up		   */
						/***************************/
	     case 'C':				/* Character		   */
	     case 'c':   			/*	Output?		   */
	       *_ptrbf++ = x&0377;		/* Just load buffer	   */
	       break;				/* Go output		   */
						/***************************/
	     case 'E':				/* Floating point?	   */
	     case 'e':				/*			   */
	     case 'F':				/*			   */
	     case 'f':				/*			   */
	       dblptr = adx-1;			/* Assumes 64 bit float!   */
	       zz = *dblptr;			/* Load value		   */
	       adx =+ 1;			/* Bump past second word   */
	       ftoa (zz, buf, prec, c);		/* Call floating conversion*/
	       prec = 0;			/* Fake out padding routine*/
	       s = buf;				/* just like string print  */
	       break;				/* Go Output		   */
						/***************************/
	     default:   			/* None of the above?	   */
	       (*f)(fd,c);			/* Just Output		   */
		ccount++;			/* Count it.		   */
	       adx--;				/* Fix arg address	   */
	   }					/* End switch		   */
						/***************************/
	   if (s == 0)				/* If s = 0, string is in  */
	   {					/* "buf", 		   */
	     *_ptrbf = '\0'; 			/*   Insure termination	   */
	     s = buf;				/*   Load address	   */
	   }					/***************************/
						/*			   */
	   n = strlen (s);			/* Compute converted length*/
	   n = (prec<n && prec != 0) ? prec : n;/* Take min(prec,n)	   */
	   m = width-n;				/* m is # of pad characters*/
						/***************************/
	   if (adj == 'r') 			/* For right adjust,	   */
		while (m-- > 0) 		/* Pad in front		   */
		{				/*			   */
		  (*f)(fd,padchar);		/* Thusly		   */
		  ccount++;			/* Count it		   */
		}				/*			   */
						/***************************/
	   while (n--) 				/* Output Converted	   */
	   {					/*			   */
		(*f)(fd,*s++);			/*		     Data  */
		ccount++;			/* Count it		   */
	   }					/*			   */
						/***************************/
	   while (m-- > 0) 			/* If left adjust,	   */
	   {					/*			   */
		(*f)(fd,padchar);		/*		   Pad	   */
		ccount++;			/* Count padded characters */
	   }					/***************************/
	   _ptrbf = buf;			/* Reset buffer pointer    */
	  }					/* End else		   */
	}					/* End while		   */
	if((*f) == _putstr) 			/* If string output,	   */
		(*f)(fd,'\0');			/* Drop in terminator char */
						/***************************/
	return(ccount);				/* Return appropriate value*/
}						/* End _printf		   */
						/***************************/

/****************************************************************************/
/*									    */
/*			_ P R N T 8   P R O C E D U R E			    */
/*			-------------------------------			    */
/*									    */
/*	Routine "_prnt8" converts a binary LONG value to octal ascii.	    */
/*	The area at "_ptrbf" is used.					    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		_prnt8(n);						    */
/*									    */
/*	"n" is the number to be converted.				    */
/*									    */
/*	Returns:							    */
/*									    */
/*		(none)							    */
/*									    */
/****************************************************************************/
VOID _prnt8 (n)					/*			    */
	LONG	n;				/* Number to convert	    */
{ 						/*			    */
	REG WORD	p;			/* Counts bits		    */
	REG WORD	k;			/* Temporary 3-bit value    */
	REG WORD	sw;			/* Switch 1 => output	    */
						/****************************/
	if (n==0) 				/* Handle 0 as special case */
	{					/*			    */
		*_ptrbf++ = '0'; 		/* Put in one zero	    */
		return;				/*	And quit	    */
	}					/*			    */
						/****************************/
	sw = 0;					/* Indicate no output yet   */
						/*			    */
	for (p=HIBIT; p >= 0; p =- 3)		/* Use 3 bits at a time	    */
						/*			    */
  	if ((k = (n>>p)&07) || sw)		/* Need to output yet?	    */
   	{					/* 			    */
		if (p==HIBIT)			/* 1st digit has only 2 bits*/
			k = k & 02;		/* Mask appropriately	    */
    		*_ptrbf++ = '0' + k;		/* ASCIIfy digit	    */
     		sw = 1;				/* Set output flag	    */
   	}					/* End if		    */
}						/* End _prnt8		    */
						/****************************/

/****************************************************************************/
/*									    */
/*			_ P r n t x   F u n c t i o n			    */
/*			-----------------------------			    */
/*									    */
/*	The "_prntx" function converts a binary LONG quantity to hex ASCII  */
/*	and stores the result in "*_ptrbf". Leading zeros are suppressed.   */
/*									    */
/*	Calling sequence:						    */
/*									    */
/*		_prntx(n);						    */
/*									    */
/*	where "n" is the value to be converted.				    */
/*									    */
/*	Returns:							    */
/*									    */
/*		(none)							    */
/*									    */
/****************************************************************************/
VOID _prntx (n)					/*			    */
	LONG	n;				/* 32 bits		    */
{						/****************************/
	REG LONG	d;			/* A digit		    */
	REG LONG	a;			/* Temporary value	    */
						/****************************/
	if (a = n>>4)				/* Peel off low 4 bits	    */
		_prntx ( a & 0xfffffff);	/* If <> 0, print first	    */
	d = n&017;				/* Take low four bits	    */
	*_ptrbf++ =  d > 9 ? 'A'+d-10 : '0' + d;/* ASCIIfy into buffer	    */
}						/****************************/

/****************************************************************************/
/*									    */
/*			_ _ C o n v   F u n c t i o n			    */
/*			-----------------------------			    */
/*									    */
/*	Function "__conv" is used to convert a decimal ASCII string in	    */
/*	the format to binary.						    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		val = __conv();						    */
/*									    */
/*	Returns:							    */
/*									    */
/*		"val" is the converted value				    */
/*		Zero is returned if no value				    */
/*									    */
/****************************************************************************/
LONG __conv()					/*			    */
{						/****************************/
	REG   BYTE	c;			/* Character temporary	    */
	REG   LONG	n;			/* Accumulator		    */
						/****************************/
	n = 0;					/* Zero found so far	    */
	while(((c= *__fmt++) >= '0') 		/* While c is a digit	    */
		&& (c <= '9')) 			/*			    */
		n = n*10+c-'0';			/* Add c to accumulator	    */
	__fmt--;				/* Back up format pointer to*/
						/* character skipped above  */
	return(n);				/* See, wasn't that simple? */
}						/****************************/

/****************************************************************************/
/*									    */
/*			_ P u t s t r   F u n c t i o n			    */
/*			-------------------------------			    */
/*									    */
/*	Function "_putstr" is used by "sprintf" as the output function	    */
/*	argument to "_printf".  A single character is copied to the buffer  */
/*	at "_ptrst".							    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*			_putstr(str,chr);				    */
/*									    */
/*	where "str" is a dummy argument necessary because the other output  */
/*	functions have two arguments.					    */
/*									    */
/*	Returns:							    */
/*									    */
/*		(none)							    */
/*									    */
/****************************************************************************/
VOID _putstr(str,chr)				/*			    */
	REG BYTE	chr;			/* The output character	    */
	BYTE		*str;			/* Dummy argument	    */
{						/****************************/
	*_ptrst++ = chr;			/* Output the character     */
	return(0);				/* Go back 		    */
}						/****************************/

/****************************************************************************/
/*									    */
/*			_ P r t 1   F u n c t i o n			    */
/*			---------------------------			    */
/*									    */
/*	Function "_prt1" converts a LONG binary quantity to decimal ASCII   */
/*	at the buffer pointed to by "_ptrbf".				    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		_prt1(n);						    */
/*									    */
/*	where "n" is the value to be converted.				    */
/*									    */
/*	Returns:							    */
/*									    */
/*		(none)							    */
/*									    */
/****************************************************************************/
VOID _prt1(n)					/*			    */
	REG LONG	n;			/* Conversion input	    */
{						/****************************/
	REG LONG	digs[15];		/* store digits here	    */
	REG LONG	*dpt;			/* Points to last digit	    */
						/****************************/
	dpt = digs;				/* Initialize digit pointer */
						/****************************/
	if (n >= 0)				/* Fix			    */
	   n = -n;				/*	up		    */
	else					/*	     sign	    */
	   *_ptrbf++ = '-';			/*		   stuff    */
						/****************************/
	for (; n != 0; n = n/10)		/* Divide by 10 till zero   */
	 *dpt++ = n%10;				/* Store digit (reverse ord)*/
						/****************************/
	if (dpt == digs)			/* Zero value?		    */
	   *dpt++ = 0;				/* Yes, store 1 zero digit  */
						/****************************/
	while (dpt != digs)			/* Now convert to ASCII	    */
	{					/*			    */
	   --dpt;				/* Decrement pointer	    */
	   *_ptrbf++ =  '0' - *dpt;		/* Note digits are negative!*/
	}					/*			    */
}						/****************************/
