/*****************************************************************************
*
*		R U N T I M E   S T A R T O F F   R O U T I N E
*		-----------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Routine "_main" is entered from the C header routine to start a C 
*	program.  The command string from CP/M is parsed into
*	a UNIX-like "argc/argv" setup, including simple I/O redirection.
*
*	Calling Sequence:
*
*		return = _main(command,length);
*
*	Where:
*
*		command		Is the address of the command line from CP/M
*		length		Is the number of characters in the line, 
*				excluding the termination character (CR/LF).
*
*****************************************************************************/
						/****************************/
						/*	INCLUDE FILES       */
						/****************************/
#include <portab.h>				/* Include std definitions  */
#include <cpm.h>				/* Include CP/M Definitions */
#include <ctype.h>				/* char type definitions    */
						/****************************/
						/*	   MACROS	    */
						/****************************/
#define STDIN 0					/* file descriptors	    */
#define STDOUT 1				/*			    */
#define STDERR 2				/*			    */
#define ISWHITE(ch) ((ch)==NULL || isspace(ch))	/*			    */
						/****************************/
						/* EXTERNAL DATA STRUCTURES */
						/****************************/
EXTERN	BYTE	__tname[];			/* Terminal name	    */
EXTERN	BYTE	__pname[];			/* Program name		    */
EXTERN  BYTE    *nextarg;			/* points to next free spot */
						/* to store an argv	    */
EXTERN  BYTE    *lastarg;			/* points to last free spot */
						/* to store an argv	    */
EXTERN  BYTE    *_break;			/* points to first free spot*/
						/****************************/
EXTERN  LONG    *make_even();			/* EXTERNAL FUNCTION	    */
						/****************************/
						/*    LOCAL FUNCTIONS       */
						/****************************/
UWORD delim();					/* returns TRUE or FALSE    */
BYTE  true_char();				/* returns a character      */
UWORD fill_fcb();				/* returns TRUE or FALSE    */
BYTE  *buildptrs();				/* return av		    */
						/****************************/
UWORD index;					/*	   GLOBAL	    */
UWORD  ac;					/*			    */
						/****************************/
						/*	    MAIN	    */
_wmain(com, len)				/****************************/
	BYTE 	*com;				/* Command address	    */
	UWORD	len;				/* Command length	    */
{						/****************************/
	REG UWORD i;				/* Define a count var.	    */
	REG BYTE *s;				/* Temp byte pointer	    */
	BYTE     *av;				/* points to argvs	    */
						/****************************/
	_chinit();				/* Initialize channels	    */
	open(__tname,READ);			/* Open STDIN		    */
	open(__tname,WRITE);			/* Open STDOUT		    */
	open(__tname,WRITE);			/* Open STDERR		    */
	com[len] = '\0';			/* Insure null at line end  */
	ac = 1;					/* Initialize count	    */
	for (s = com; *s; s =+ i)		/* One arg at a time	    */
	{					/*			    */
		while (*s && isspace(*s))	/* Skip leading spaces	    */
			++s;			/*			    */
		if (!*s)			/* End of line?		    */
			break;			/* Yes, don't continue	    */
		for (i = 0; !ISWHITE(s[i]);++i);/* How many characters?	    */
		if (s[i])			/* If last is space, etc... */
			s[i++] = '\0';		/* Make it a null for C	    */
						/****************************/
		if (*s == '<')			/* Redirecting input??	    */
		{				/*			    */
		  close(STDIN);			/* Yes, close TTY	    */
		  if (open(s + 1,READ,0) != STDIN) /* Open New		    */
		    _err("Cannot open ", s + 1)	;/*  Can't ...		    */
		}				/****************************/
		else if (*s == '>')		/* Redirecting output??	    */
		{				/*			    */
		  close(STDOUT);		/* Close output		    */
		  if (creat(s+1,0,0)!=STDOUT)	/* Try to open new	    */
		    _err("Cannot create ", s + 1);/*	Can't		    */
		}				/****************************/
		else				/* Just a regular arg	    */
			trans(s);		/* look for wildcards       */
	}					/****************************/
	av = buildptrs();			/* Set up argv ptrs to point*/
						/* to the right strings     */
						/****************************/
	exit(main(ac, av));			/* Invoke C program	    */
}						/****************************/
						/* 	ERROR HANDLER	    */
						/****************************/
VOID _err(s1,s2)				/* Error routine	    */
	BYTE	*s1;				/* Message text		    */
	BYTE	*s2;				/* Filename		    */
{						/****************************/
	write(STDERR,s1,strlen(s1));		/* Output error message	    */
	write(STDERR,s2,strlen(s2));		/* And filename		    */
	write(STDERR,"\n",1);			/* + Newline		    */
	exit(-1);				/* And fail hard	    */
}						/****************************/
						/*   CHECK FOR DELIMITERS   */
						/****************************/
UWORD delim(ch)					/*			    */
REG BYTE *ch;					/* character to be checked  */
{						/*			    */
	if(*ch <= ' ')				/*	  againist          */
		return(TRUE);			/*			    */
	switch(*ch)				/*			    */
	{					/*			    */
		case '>':			/*       CP/M-68K SET	    */
		case '<':			/*	     OF		    */
		case '.':			/*	  DELIMITERS        */
		case ',':			/*			    */
		case '=':			/*			    */
		case ':':			/*			    */
		case '+':			/*			    */
		case '-':			/*			    */
		case '&':			/*			    */
		case '/':			/*			    */
		case '\\':			/*			    */
		case '|':			/*			    */
		case '(':			/*			    */
		case ')':			/*			    */
		case '[':			/*			    */
		case ']':			/*			    */
		case ';':	return(TRUE);	/*			    */
	}					/*			    */
	return(FALSE);				/*			    */
}						/****************************/
						/*   RETURN FCB CHARACTER   */
						/****************************/
BYTE true_char(ch)				/*			    */
REG BYTE *ch;					/*   incoming character     */
{						/*			    */
	if(*ch == '*') return('?');		/*   wildcard check	    */
	if(!delim(ch))				/*   delimiter check	    */
	{					/*			    */
		index++;			/* bump up character index  */
		return(*ch);			/* and return character     */
	}					/*			    */
	return(' ');				/* return a blank if ch was */
						/* a delimiter		    */
}						/****************************/
						/*        MAKE A FCB	    */
						/****************************/
UWORD fill_fcb(s,fcb)				/*			    */
REG BYTE *s;					/* points to arg in basepage*/
REG BYTE *fcb;					/* points to fcb	    */
{						/*			    */
	REG BYTE *p;				/* temp pointer		    */
	REG UWORD j;				/* counter		    */
						/*			    */
	*fcb = 0;				/* initialize the fcb	    */
	for(j = 1;j <= 11;j++)			/*			    */
		*(fcb + j) = ' ';		/*			    */
	for(j = 12;j <= 35;j++)			/*		            */
		*(fcb + j) = 0;			/*			    */
						/*			    */
	index = 0;				/* initialize the ch index  */
	p = fcb + 0x1;				/* skip the drive code      */
	if(*(s+1) == ':')			/* check for a drive	    */
	{					/*			    */
		if(*s < 'A' || *s > 'P')	/* check range		    */
			return(FALSE);		/*			    */
		*fcb = *s - 'A' + 1;		/* convert to a drive code  */
		index += 2;			/* bump up the index	    */
	}					/*			    */
	for(j = 1;j <= 8;j++)			/* parse for a filename     */
		*p++ = true_char(s+index);	/*			    */
	while(!(delim(s+index))) index++;	/* scan to a delimiter	    */
	if(*(s+index) == '.')			/* if it is a period	    */
	{					/*			    */
		index++;			/* bump index to next char  */
		for(j = 1;j <= 3;j++)		/* parse for a file type    */
		     *p++ = true_char(s+index); /* 		            */
	}					/*			    */
	for(j = 0;j < 11;j++)			/* look for the '?''s	    */
		if(*++fcb == '?')		/*			    */
			return(TRUE);		/*			    */
	return(FALSE);				/* no '?' then return 0	    */
}						/****************************/
						/*    BUILD ARGV & ARGC     */
						/****************************/
BYTE *buildptrs()				/*			    */
{						/*			    */
	REG LONG *s;				/* 			    */
	REG BYTE *t;				/*			    */
						/*			    */
	*nextarg++ = '\0';			/* tack a NULL on to the    */
						/* actual strings in memory */
	t = _break;				/*points to start of strings*/
	s = ++nextarg;				/* first spot for a *argv   */
	s = make_even(s);			/* assure an even address   */
	nextarg = s;				/*			    */
	*s++ = __pname;				/* first is the program name*/
	if(*t)					/* if there is a 2nd argv   */
	{					/*			    */
		*s++ = t;			/* point to it.		    */
		ac++;				/* bump argc		    */
	}					/****************************/
						/*			    */
	while(*t && s < lastarg)		/* go till end of strings   */
	{					/*			    */
		while(*t) t++;			/* find the end of current  */
						/* string		    */
		if(*(t + 0x1))			/* is there another string? */
		{				/*			    */
			*s++ = ++t;		/* point to next string     */
			ac++;			/* bump up argc		    */
		}				/*			    */
	}					/*			    */
	*s++ = 0;				/* last argv pointer is NULL*/
	_break = s;				/* reset the break address  */
	return(nextarg);			/* pointer to pointers      */
}						/****************************/
						/*  TRANSLATE ANY WILDCARDS */
						/****************************/
VOID trans(arg)					/*			    */
BYTE *arg;					/* pointer to cmd tail      */
{						/*			    */
	BYTE fcb[36],dma[128],found,overflow;   /* flags and buffers	    */
	REG UWORD i,j;				/* counters		    */
						/*			    */
	found = overflow = FALSE;		/* initialize flags	    */
	if(fill_fcb(arg,fcb))			/* check for wildcards      */
	{					/*			    */
	  __BDOS(26,dma);			/* reset the dma address    */
	  i = __BDOS(17,fcb);			/* searchfirst		    */
	  if(i != 255)				/* check for success	    */
	  {					/*			    */
            while(i != 255)			/*			    */
	    {					/*			    */
		i = (32 * i) + 1;		/* move through dma	    */
		for(j = 1;j <= 11;j++)		/*			    */
		{				/*			    */
		  if(j == 9 && dma[i] == ' ')   /* special case		    */
		   	nextarg--;		/* go back one byte	    */
		  if(dma[i] != ' ')		/* skip blanks		    */
		   *nextarg++ = tolower(dma[i]);/* move in a lower case ch  */
		  if(j == 8) *nextarg++ = '.';  /* filename.typ		    */
		  if(nextarg >= lastarg)	/* check for overflow	    */
		  {				/*			    */
		    while(*--nextarg);		/* go back to last NULL     */
		    overflow = TRUE;		/* set a flag		    */
		  }				/*			    */
		  i++;				/* bump dma index	    */
		}				/*			    */
		*nextarg++ = '\0';		/* NULL goes at end	    */
		if(overflow) break;		/* check overflow of memory */
						/****************************/
		if(fcb[0])			/* Was a drive specified?   */
		{				/* If here, yes		    */
		   *nextarg++ = fcb[0]-1 + 'a';	/* Put in drive		    */
		   *nextarg++ = ':';		/* and delimiter	    */
		}				/*			    */
						/****************************/
		i = __BDOS(18,fcb);		/* do a searchnext	    */
	      }					/*			    */
	      found = TRUE;			/* we had a success	    */
	    }					/*			    */
    	}					/*			    */
	if(!(found))				/* if no success	    */
		do				/*			    */
		{				/* then just move in the    */
		   *nextarg++ = tolower(*arg);  /* arg from the cmd tail    */
		}				/*			    */
		while(*arg++);			/*			    */
}						/****************************/
 
