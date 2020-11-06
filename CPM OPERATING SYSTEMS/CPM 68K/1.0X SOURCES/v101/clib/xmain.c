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
						/*			    */
_main(com, len)					/****************************/
	char 	*com;				/* Command address	    */
	int	len;				/* Command length	    */
{						/****************************/
	char	*av[30];			/* Unix's "ARGV" for main   */
	register i;				/* Define a count var.	    */
	register char *s;			/* Temp byte pointer	    */
	int	 ac;				/* Number of args we find   */
						/****************************/
	_chinit();				/* Initialize channels	    */
	open(__tname,READ);			/* Open STDIN		    */
	open(__tname,WRITE);			/* Open STDOUT		    */
	open(__tname,WRITE);			/* Open STDERR		    */
	av[0] = __pname;			/*			    */
	com[len] = '\0';			/* Insure null at line end  */
	ac = 1;					/* Initialize count	    */
	for (s = com; *s; s =+ i)		/* One arg at a time	    */
	{					/*			    */
		while (*s && isspace(*s))	/* Skip leading spaces	    */
			++s;			/*			    */
		if (!*s)			/* End of line?		    */
			break;			/* Yes, don't continue	    */
		for (i = 0; !ISWHITE(s[i]); ++i)/* How many characters?	    */
			s[i] = tolower(s[i]);	/* lower case please ...    */
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
			av[ac++] = s;		/* save in argv		    */
	}					/****************************/
	av[ac] = NULL;				/* Insure terminator	    */
	exit(main(ac, av));			/* Invoke C program	    */
}						/****************************/

_err(s1,s2)					/* Error routine	    */
	char	*s1;				/* Message text		    */
	char	*s2;				/* Filename		    */
{						/****************************/
	write(STDERR,s1,strlen(s1));		/* Output error message	    */
	write(STDERR,s2,strlen(s2));		/* And filename		    */
	write(STDERR,"\n",1);			/* + Newline		    */
	exit(-1);				/* And fail hard	    */
}						/****************************/
