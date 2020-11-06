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
#include <portab.h>				/* Include std definitions  */
#include <cpm.h>				/* Include CP/M Definitions */
#include <ctype.h>				/* char type definitions    */
#define STDIN 0					/* file descriptors	    */
#define STDOUT 1				/*			    */
#define STDERR 2				/*			    */
#define ISWHITE(ch) ((ch)==NULL || isspace(ch))	/*			    */
EXTERN	BYTE	__tname[];			/* Terminal name	    */
EXTERN	BYTE	__pname[];			/* Program name		    */
EXTERN	BYTE	*_break;			/* -> Program break location*/
EXTERN	BYTE	*_salloc();			/* Stack allocation routine */
EXTERN	BYTE	*_index();			/* V7 "index" function	    */
WORD	argc;					/* Arg count		    */
BYTE	*argv;					/* -> array of pointers	    */
						/*			    */
_wmain(com, len)				/****************************/
	BYTE 	*com;				/* Command address	    */
	WORD	len;				/* Command length	    */
{						/****************************/
REG	WORD	 i;				/* Define a count var.	    */
REG	BYTE 	*s;				/* Temp byte pointer	    */
REG	BYTE	*p,*p1;				/* Another ""		    */
REG	BYTE	c;				/* Character temp	    */
	BYTE	tmpbuf[30];			/* Filename temp	    */
						/****************************/
	_chinit();				/* Initialize channels	    */
	open(__tname,READ);			/* Open STDIN		    */
	open(__tname,WRITE);			/* Open STDOUT		    */
	open(__tname,WRITE);			/* Open STDERR		    */
	argv = _break;				/* -> first free location   */
	argc = 0;				/* No args yet		    */
	addargv(__pname);			/*			    */
	com[len] = '\0';			/* Insure null at line end  */
	for (s = com; *s; s++)			/* Convert string to	    */
		*s = tolower(*s);		/*          lower case	    */
	for (s = com; *s; s =+ i)		/* One arg at a time	    */
	{					/*			    */
		while (*s && isspace(*s))	/* Skip leading spaces	    */
			++s;			/*			    */
		if (!*s)			/* End of line?		    */
			break;			/* Yes, don't continue	    */
		if(*s == '"' || *s == '\'')	/* Quoted string	    */
		{				/*			    */
		   c = *s;			/* c = quote character      */
		    p = _index(s+1,c);		/* Find next		    */
		    if (p == NULL)		/* Any?			    */
			_err(s,": unmatched quote");/* No, bitch.	    */
		    i = p-s;			/* Compute length	    */
		    s[i++] = '\0';		/* Nullify at end	    */
		    addargv(s+1);		/* Add to arg list	    */
		}				/****************************/
		else				/*			    */
		{				/****************************/
		 for(i = 0; !ISWHITE(s[i]); ++i)/* How many characters?	    */
				;		/* 			    */
		 if (s[i])			/* If last is space, etc... */
			s[i++] = '\0';		/* Make it a null for C	    */
		 switch(*s)			/* Now do i/o scan	    */
		 {				/****************************/
		  case '<':			/* Redirecting input??	    */
		    close(STDIN);		/* Yes, close TTY	    */
		    if (opena(s + 1,READ) != STDIN) /* Open New		    */
		    _err("Cannot open ", s + 1)	;/*  Can't ...		    */
		    break;			/* Done with this one	    */
						/****************************/
		   case '>':			/* Redirecting output??	    */
		     close(STDOUT);		/* Close output		    */
		     if (creata(s+1,0)!=STDOUT)/* Try to open new	    */
		     _err("Cannot create ", s + 1);/*	Can't		    */
		     break;			/****************************/
						/*			    */
		   default:			/* Just a regular arg	    */
						/****************************/
		    if(_index(s,'?') ||		/* Wild			    */
		       _index(s,'*'))		/*	Cards?		    */
		    {				/****************************/
		      p = _getccb(STDERR+1);	/* Use unused channel	    */
		      __BDOS(SETDMA,p->buffer);	/* Use buffer for DMA	    */
		      c=__open(STDERR+1,s,SEARCHF); /* Do the search	    */
		      if(c == 0xff)		/* Failed ...		    */
			_err(s,": No match");	/* Complain		    */
		      while(c != 0xff)		/* Do search next's	    */
		      {				/****************************/
			_toasc(p,c,tmpbuf);	/* Convert file to ascii    */
			p1 = _salloc(strlen(tmpbuf)+1);/* Allocate area     */
			strcpy(p1,tmpbuf); 	/* Move in filename	    */
			addargv(p1);		/* Add this file to argv    */
			c=__open(STDERR+1,s,SEARCHN); /* Search next	    */
		       } 			/****************************/
		    } else			/* 			    */
			addargv(s);		/* save in argv		    */
		 }				/*			    */
		}				/*			    */
	}					/****************************/
	addargv((BYTE *)NULL);			/* Insure terminator	    */
	argc--;					/* Back off by 1	    */
	exit(main(argc, argv));			/* Invoke C program	    */
}						/****************************/
						/*			    */
MLOCAL _err(s1,s2)				/* Error routine	    */
	char	*s1;				/* Message text		    */
	char	*s2;				/* Filename		    */
{						/****************************/
	write(STDERR,s1,strlen(s1));		/* Output error message	    */
	write(STDERR,s2,strlen(s2));		/* And filename		    */
	write(STDERR,"\n",1);			/* + Newline		    */
	exit(-1);				/* And fail hard	    */
}						/****************************/
/*
 *	Addargv function -- adds a pointer to the argv array, getting the 
 *	space from the heap.
 */
MLOCAL addargv(ptr)				/****************************/
REG	BYTE	*ptr;				/* -> Argument string to add*/
{						/*			    */
REG	BYTE	**p;				/* Temp			    */
	p = _break;				/* Load next spot	    */
	if(brk(_break+4) != 0)			/* Allocate a pointer	    */
		_sovf();			/* Call "Stack Overflow" rtn*/
	*p = ptr;				/* Load pointer		    */
	argc++;					/* Increment arg count	    */
}						/****************************/
    
/*
 *	Toasc routine -- combines the FCB name in the DMA and the user number
 *	/ drive field to produce an ascii file name for SEARCHes.
 *
 */
_toasc(p,c,buf)					/*****************************/
REG	FD	*p;				/* -> Data area		     */
REG	BYTE	 c;				/* 0 .. 3 search code	     */
REG	BYTE	*buf;				/* Output buffer area	     */
{						/*****************************/
REG	BYTE	*f;				/* -> Fcb in DMA buffer	     */
REG	WORD	 i;				/* Temp.		     */
						/*			     */
	f = &p->buffer[c*32];			/* -> FCB drive code	     */
	*buf = '\0';				/* Nullify at first	     */
	if(p->user != __BDOS(USER,0xffL))	/* User numbers different?   */
	{					/* Yes			     */
		sprintf(buf,"%d:",p->user);	/*  convert user number	     */
		i = TRUE;			/* remember		     */
	} else					/* No user number	     */
		i = FALSE;			/*****************************/
	buf += strlen(buf);			/* buf -> null		     */
	if(p->fcb.drive)			/* Drive specified?	     */
	{					/* Yes			     */
	  if(i)					/* User #?		     */
		buf--;				/* Yes, back up over ':'     */
	  *buf++ = p->fcb.drive - 1 + 'a';	/* Put in drive code	     */
	  *buf++ = ':';				/* And delimiter	     */
	}					/*			     */
	for(i=1;i<9;i++)			/* Move the filename	     */
	{					/*****************************/
	   if(f[i] != ' ')			/* Non-blank?		     */
		*buf++ = tolower((f[i]&0x7f));	/* Yes, move it in	     */
	}					/*****************************/
	*buf++ = '.';				/* Put in delimiter	     */
	for(i=9; i<12; i++)			/* Move in extension	     */
	{					/*			     */
	   if(f[i] != ' ')			/* Non-blank?		     */
		*buf++ = tolower((f[i]&0x7f));	/* Yes, move it in	     */
	}					/*****************************/
	*buf++ = '\0';				/* Null at end		     */
}						/*****************************/
