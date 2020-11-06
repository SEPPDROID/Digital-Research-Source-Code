/*************************************************************************
*
*			_ d o p r t   F u n c t i o n 
*			-----------------------------
*	Copyright 1982 by Digital Reseacrh Inc.  All rights reserved.
*
*	"_doprt" handles the printing for functions "printf", "fprintf",
*	and "sprintf".  Each of these sets up the parameters and calls _doprt.
*
*	Calling sequence:
*
*		nchrs = _doprt(stream,fmt,args)
*	Where:
*		nchrs  =  number of chars output by _doprt
*		stream -> destination buffer (FILE *)
*		fmt    -> a string specifying how "args" are to be output.
*		args   -> a list of arguments
*
*****************************************************************************/
#include "stdio.h"
#include "osif.h"

#define ARGWIDTH (sizeof(int))			/* width of arg to function */
#define NXTARG(ptr,isl) ((ptr)+=((isl) ? MAX(ARGWIDTH,sizeof(LONG)):ARGWIDTH)) 
#define IARG(ptr) ((ptr)+=ARGWIDTH)		       /* int arg increment */
#if    CPM68K
#define FARG(ptr) ((ptr)+=sizeof(float))      /* float/double arg increment */
#else
#define FARG(ptr) ((ptr)+=sizeof(double))     /* float/double arg increment */
#endif
#define CHOOSELS(isl) ((isl) ? __prtld : __prtshort ) /* prt long or short  */
#define XPUTC(a1,a2) {putc(a1,a2);nchrs++;}	/* count chars output	    */
						/****************************/
_doprt(sp,fmt,pb)				/* print subroutine	    */
	FILE *sp;				/* stream buffer ptr	    */
	BYTE *fmt;				/* format string	    */
	REG BYTE *pb;				/* pointer to args 	    */
{						/****************************/
	REG BYTE	c;			/* format char		    */
	BYTE		**ppi;			/* pointer to ptr	    */
	WORD		*pw;			/* pointer to word	    */
	BYTE		padchar,		/* for padding		    */
			*s,			/* string ptr for output    */
			buf[BUFSIZ];		/* s usually points here    */
	WORD		width;			/* for format spec	    */
	REG WORD	prec,			/* precision		    */
			len;			/* total length of field    */
	WORD		n,			/* counter		    */
			nchrs;			/* number chars output	    */
	BOOLEAN		left, 			/* pad on left?		    */
			longf;			/* a long arg?		    */
	MLOCAL BYTE	(*fn)();		/* function holder	    */
	GLOBAL BYTE	*__prtshort(), 		/* for 16 bit integers	    */
			*__prtld();		/* for 32 bit integers	    */
	double		*dblptr;		/* in case we need one	    */
						/****************************/
	nchrs = 0;				/* init # chars output	    */
	if (fmt)				/* make sure it's not NULL  */
	while (TRUE)				/* examine the format string*/
	{					/*			    */
		for( len=0, s=fmt; *s && *s != '%'; ++s, ++len )
			;			/* grab up to EOS or '%'    */
		if( len > 0 )			/* did we grab anything?    */
		{				/*   yes...		    */
			fputn(fmt,len,sp);	/* put them out		    */
			fmt = s;		/* update this pointer	    */
			nchrs += len;		/* update the counter	    */
		}				/*			    */
		if( *fmt++ != '%' )		/* Did we stop for '%'?     */
			break;			/*   no, EOS: we're finished*/
						/****************************/
		left = 0;			/* assume no left pad	    */
		if ((c = *fmt++) == '-')	/*   left pad specified	    */
		{				/*			    */
			c = *fmt++;		/*			    */
			left++;			/*			    */
		}				/*			    */
		padchar = ' ';			/* assume blank padding	    */
		if (c == '0')			/*   zero fill specified    */
		{				/*			    */
			padchar = c;		/*			    */
			c = *fmt++;		/*			    */
		}				/*			    */
		width = -1;			/* assume no field spc width*/
		if( c== '*' )			/*   get width from args    */
		{				/*			    */
			pw = pb;		/* use word ptr for access  */
			width = *pw;		/* 			    */
			IARG(pb);		/*			    */
			c = *fmt++;		/*			    */
		}				/*			    */
		else while (c >= '0' && c <= '9') /* get width from fmt	    */
		{				/*			    */
			if (width < 0)		/*			    */

				width = 0;	/*			    */
			width = width * 10 + (c - '0'); /*		    */
			c = *fmt++;		/*			    */
		}				/*			    */
		prec = -1;			/* assume no precision	    */
		if (c == '.')			/*   get precision	    */
		{				/*			    */
			prec = 0;		/*			    */
			c = *fmt++;		/*			    */
		}				/*			    */
		if( c== '*' )			/*   get prec from args     */
		{				/*			    */
			pw = pb;		/* get word pointer	    */
			prec = *pw;		/* 			    */
			IARG(pb);		/*			    */
			c = *fmt++;		/*			    */
		}				/*			    */
		else while (c >= '0' && c <= '9') /* get prec from fmt	    */
		{				/*			    */
			prec = prec * 10 + (c - '0'); /*		    */
			c = *fmt++;		/*			    */
		}				/*			    */
		longf = 0;			/* assume short int	    */
		if (c == 'l' || c == 'L')	/*   long int specified     */
		{				/*			    */
			longf++;		/*			    */
			c = *fmt++;		/*			    */
		}				/****************************/
						/* we now have all prelims  */
						/* out of the way; let's see*/
						/* what we want to print    */
		s = buf;			/* assume we'll be using buf*/
		switch (c)			/*			    */
		{				/*			    */
						/****************************/
		  case 'D':			/* decimal signed	    */
			longf++;		/* capital letter means long*/
		  case 'd':			/*			    */
			fn = CHOOSELS(longf);	/*			    */
			__prtint(pb, buf, 10, TRUE, fn); /*		    */
			NXTARG(pb,longf);	/* point to next arg	    */
			break;			/* finis switch		    */
						/****************************/
		  case 'U':			/* decimal unsigned 	    */
			longf++;		/*			    */
		  case 'u':			/*			    */
			fn = CHOOSELS(longf);	/*			    */
			__prtint(pb, buf, 10, FALSE, fn); /*		    */
			NXTARG(pb,longf);	/*			    */
			break;			/*			    */
						/****************************/
		  case 'O':			/* octal signed 	    */
			longf++;		/*			    */
		  case 'o':			/*			    */
			fn = CHOOSELS(longf);	/*			    */
			__prtint(pb, buf, 8, FALSE, fn); /*		    */
			NXTARG(pb,longf);	/*			    */
			break;			/*			    */
						/****************************/
		  case 'X':			/* hexadecimal unsigned     */
			longf++;		/*			    */
		  case 'x':			/*			    */
			fn = CHOOSELS(longf);	/*			    */
			__prtint(pb, buf, 16, FALSE, fn); /*		    */
			NXTARG(pb,longf);	/*			    */
			break;			/*			    */
						/****************************/
		  case 's':			/* string 		    */
		  case 'S':			/*			    */
			ppi = pb;		/* need to deal with ptr    */
			s = *ppi;		/* cast to pointer	    */
			NXTARG(pb,(sizeof(char *)==sizeof(long)));
						/* TRUE if pointers are long*/
						/* FALSE if not		    */
			break;			/*			    */
						/****************************/
		  case 'c':			/* character 		    */
		  case 'C':			/*			    */
			pw = pb;		/* word ptr for portability */
			buf[0] = (*pw & 0377);	/* assume chars passed as   */
			buf[1] = NULL;		/*	ints		    */
			IARG(pb);		/*			    */
			break;			/*			    */
						/****************************/
						/****************************/
		  case 'E':			/* E format		    */
		  case 'e':			/*			    */
			_petoa(pb, buf, prec, c);/* Do the conversion	    */
			FARG(pb); 		/* Bump to next arg	    */
			prec = -1;		/* Avoid truncation	    */
			break;			/* Next			    */
						/****************************/
		  case 'F':			/* F format floating	    */
		  case 'f':			/*			    */
			_pftoa(pb, buf, prec, c);/* do the conversion	    */
			FARG(pb); 		/* Bump to next arg	    */
			prec = -1;		/* Avoid truncation	    */
			break;			/* 			    */
						/****************************/
		  case 'G':			/* G format floating	    */
		  case 'g':			/*			    */
			_pgtoa(pb, buf, prec, c);/* do the conversion	    */
			FARG(pb); 		/* Bump to next arg	    */
			prec  = -1;		/* Avoid truncation	    */
			break;			/*			    */
						/****************************/
						/****************************/
		  default:			/* just print the character */
			XPUTC(c,sp);		/*			    */
			continue;		/*			    */
		}				/****************************/
		len = strlen(s);		/* how long is the output?  */
		if (prec < len && prec >= 0)	/* cut off if user says so  */
			len = prec;		/*			    */
		n = width - len;		/* how much extra room?	    */
		if (!left)			/* if left not specified    */
		{				/*			    */
			if (padchar == '0' && *s == '-') /* check for neg   */
			{			/*			    */
				len--;		/* output minus if leading 0*/
				XPUTC(*s++,sp);	/*			    */

			}			/*			    */
			while (n-- > 0)		/* now pad on left	    */
				XPUTC(padchar,sp); /*			    */
		}				/*			    */
		fputn(s,len,sp);		/* put buffered n chars	    */
		nchrs += len;			/* assume they got there    */
		while (n-- > 0)			/* anything remain, then pad*/
			XPUTC(padchar,sp);	/*			    */
	}					/*			    */
	return(nchrs);				/* report on # chars output */
}						/****************************/
put */
}						/****************************/
put */
}						/****************************/
