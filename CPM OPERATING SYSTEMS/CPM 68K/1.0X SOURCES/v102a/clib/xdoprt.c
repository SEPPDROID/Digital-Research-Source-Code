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
*		nchrs = _doprt(stream,fmt,args)
*	Where:
*		nchrs  =  number of chars output by _doprt
*		stream -> destination buffer (FILE *)
*		fmt    -> a string specifying how "args" are to be output.
*		args   -> a list of arguments
*
*****************************************************************************/
#include <stdio.h>
#define ARGWIDTH (sizeof(int))			/* width of arg to function */
#define NXTARG(ptr,isl) ((ptr)+=((isl) ? max(ARGWIDTH,sizeof(LONG)):ARGWIDTH)) 
#define IARG(ptr) ((ptr)+=ARGWIDTH)		       /* int arg increment */
#define CHOOSELS(isl) ((isl) ? __prtld : __prtshort ) /* prt long or short  */
#define XPUTC(a1,a2) putc(a1,a2);nchrs++	/* count chars output	    */
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
	WORD		width, 			/* for format spec	    */
			prec,			/* precision		    */
			len,			/* total length of field    */
			n,			/* counter		    */
			nchrs;			/* number chars output	    */
	BOOLEAN		left, 			/* pad on left?		    */
			longf;			/* a long arg?		    */
	MLOCAL		(*fn)();		/* function holder	    */
	GLOBAL BYTE	*__prtshort(), 		/* for 16 bit integers	    */
			*__prtld();		/* for 32 bit integers	    */
						/****************************/
	nchrs = 0;				/* init # chars output	    */
	while (c = *fmt++)			/* examine the format string*/
	{					/*			    */
		if (c != '%')			/* is it arg specifier?	    */
		{				/*   no...		    */
			XPUTC(c,sp);		/*   just put & continue    */
			continue;		/*			    */
		}				/*			    */
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
			NXTARG(pb,TRUE);	/* assume ptrs are LONG	    */
			break;			/*			    */
						/****************************/
		  case 'c':			/* character 		    */
		  case 'C':			/*			    */
			pw = pb;		/* use word pointer:	    */
			buf[0] = (*pw & 0377);	/* assume chars passed as   */
			buf[1] = ZERO;		/*	ints		    */
			IARG(pb);		/*			    */
			break;			/*			    */
						/****************************/
		  case 'E':			/* E format		    */
		  case 'e':			/*			    */
			petoa(pb, buf, prec);	/* Do the conversion	    */
			NXTARG(pb,TRUE);	/* Bump to next arg	    */
			prec = -1;		/* Avoid truncation	    */
			break;			/* Next			    */
						/****************************/
		  case 'F':			/* F format floating	    */
		  case 'f':			/*			    */
			pftoa(pb, buf, prec);	/* do the conversion	    */
			NXTARG(pb,TRUE);	/* Bump argument pointer    */
			prec = -1;		/* Avoid truncation	    */
			break;			/* 			    */
						/****************************/
		  case 'G':			/* G format floating	    */
		  case 'g':			/*			    */
			pftoa(pb, buf, prec);	/* Try F first		    */
			if(strlen(buf) > 	/****************************/
				(7+prec))	/* Smallest FP string	    */
			  petoa(pb,buf,prec);	/* Do E format instead	    */
			NXTARG(pb,TRUE);	/* Do next arg		    */
			prec  = -1;		/* Avoid truncation	    */
			break;			/*			    */
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
		while (len-- > 0)		/* output the buffer string */
			XPUTC(*s++,sp);		/*  into the buffer	    */
		while (n-- > 0)			/* anything remain, then pad*/
			XPUTC(padchar,sp);	/*			    */
	}					/*			    */
	return(nchrs);				/* report on # chars output */
}						/****************************/
