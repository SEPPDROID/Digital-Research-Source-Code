/****************************************************************************/
/*									    */
/*			_ o p e n   F u n c t i o n			    */
/*			---------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
/*									    */
/*	Function "__open" is used to parse the CP/M fcb and open or create  */
/*	the requested file.  Created files are deleted first, to avoid 	    */
/*	directory problems.						    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = __open(ch,filnam,bdosfunc);			    */
/*									    */
/*	Where:								    */
/*		ch	Is a vacant channel number			    */
/*		filnam	Is a null-terminated CP/M filename		    */
/*		bdosfunc Is the desired BDOS function to perform	    */
/*									    */
/*	This routine may also be used to delete files as well.		    */
/*									    */
/*	Modified 26-Jun-83 (sw) for wild cards and user # support	    */
/*									    */
/****************************************************************************/
#include <portab.h>
#include <cpm.h>
#include <ctype.h>
WORD	__open(ch,filnam,bdosfunc)		/****************************/
						/*			    */
	WORD	ch;				/* Channel number	    */
REG	BYTE	*filnam;			/* -> filename		    */
	WORD	bdosfunc;			/* BDOS Function	    */
{						/****************************/
REG	FD	*fp;				/* -> ccb area		    */
REG	struct fcbtab	*fcbp;			/* -> FCB area in ccb	    */
REG	BYTE	*p;				/* Temp character pointer   */
REG	WORD	i;				/* Character index	    */
	BYTE	tokbuf[32];			/* Token temporary buffer   */
REG	WORD	xuser;				/* User number		    */
	BYTE	*get_token();			/****************************/
	fp = _getccb(ch);			/* Fetch ccb pointer	    */
	fcbp = &(fp ->fcb);			/* Fetch fcb pointer	    */
						/****************************/
	xuser = __BDOS(USER,0xFFL);		/* Fetch current user number*/
	fp->user = xuser;			/* Set it as default	    */
						/****************************/
	filnam = get_token(filnam,tokbuf,32);	/* Get the next token	    */
	if(*filnam == ':')			/* Is the delim a colon?    */
	{					/* If yes, then a drive spec*/
	  if(stuff_drive(fp,fcbp,tokbuf) == FAILURE) /* Check drive OK	    */
		return(FAILURE);		/* Return error if bad	    */
	  filnam=get_token(++filnam,tokbuf,32);	/* Get next token	    */
	}					/****************************/
						/*			    */
	if(*filnam == '*')			/* Delimiter is a "*"??	    */
	{					/*			    */
	  fill(&(fcbp->fname[0]),'?',8);	/* Yes, fill with ?'s 1st   */
	  filnam++;				/* Bump to next character   */
	}					/****************************/
	_strcpy(tokbuf,fcbp->fname,8);		/* Copy the file name in    */
	if(*filnam && (*filnam != '.'))		/* Check legality	    */
		return(FAILURE);		/* Not legal file name	    */
						/****************************/
	if(*filnam++ == '.')			/* If extension specified   */
	{					/*			    */
	  filnam=get_token(filnam,tokbuf,32);	/* get extension token	    */
	  if(*filnam == '*')			/* Is wildcarded ext?	    */
	    fill(&(fcbp->fname[8]),'?',3);	/* If yes, fill with ?'s    */
	  _strcpy(tokbuf,&(fcbp->fname[8]),3);	/* Copy the string	    */
	 }					/*			    */
						/****************************/
	if(bdosfunc == CREATE)			/* Creating file?	    */
	{					/*			    */
		if(index(fcbp->fname,'?'))	/* Wild cards @!@#$!!!	    */
			return(FAILURE);	/* Just quit		    */
		if(xuser != fp->user)		/* need to change users?    */
		  __BDOS(USER,(LONG)fp->user);	/* Use user # specified	    */
						/****************************/
		__BDOS(DELETE,fcbp);		/*  delete it first	    */
	}					/****************************/
	if(xuser != fp->user)			/* Different user #??	    */
		__BDOS(USER,(LONG)fp->user);	/* Change it first	    */
						/*			    */
	i = __BDOS(bdosfunc,fcbp);		/* Do requested operation   */
	if((i > 3) && (fp->user))		/* If non-zero user,	    */
	{					/*			    */
		fp->user = 0;			/* Try user 0		    */
		__BDOS(USER,0L);		/* Set user 0		    */
		i = __BDOS(bdosfunc,fcbp);	/* Try function again	    */
	}					/****************************/
	if(xuser != fp->user)			/* did we change users?	    */
		__BDOS(USER,(LONG)xuser);	/* Then change back	    */
						/*			    */
	return((i<=3) ? SUCCESS : FAILURE);	/* Binary return code	    */
}						/****************************/
MLOCAL _strcpy(s,d,c)				/* Special string copy func */
REG	BYTE	*s;				/* Source string	    */
REG	BYTE	*d;				/* Destination area	    */
REG	WORD	 c;				/* Count		    */
{						/****************************/
	while ((*s) && (c))			/* 			    */
	{					/*			    */
		*d++ = toupper(*s);		/* Copy a byte		    */
		s++;				/* Up source		    */
		c--;				/* Down count		    */
	}					/*			    */
}						/****************************/

/****************************************************************************/
/*									    */
/*		    G e t _ t o k e n   R o u t i n e			    */
/*		    ---------------------------------			    */
/*									    */
/*	Routine get_token takes the next token from the input string.	    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = get_token(src,dest,len);				    */
/*									    */
/*	Where:								    */
/*									    */
/*		src	is the address of the source string		    */
/*		dest	is the address of the destination area		    */
/*		len 	is the number of bytes in "dest".		    */
/*									    */
/*		ret	is the returned address of the delimiter	    */
/*									    */
/****************************************************************************/
MLOCAL BYTE *_delim="<>.,=:+-&/\\|()[]*";	/* Delimiter set	    */
MLOCAL BYTE *get_token(src,dest,len)		/*			    */
REG	BYTE	*src;				/* -> source		    */
REG	BYTE	*dest;				/* -> destination	    */
REG	WORD	 len;				/* output area size	    */
{						/****************************/
	while(*src && (!index(_delim,*src)) && len)/* Until done	    */
	{					/*			    */
	  *dest++ = *src++;			/* Move 1 byte		    */
	   len--;				/* Down count		    */
	}					/****************************/
	*dest = '\0';				/* Drop in null terminator  */
	return(src);				/* Return delimiter addr.   */
}						/****************************/

/****************************************************************************/
/*									    */
/*		   S t u f f _ d r i v e   F u n c t i o n		    */
/*		   ---------------------------------------		    */
/*									    */
/*	Routine "stuff_drive" loads the user and drive code fields from     */
/*	the filename string.						    */
/*									    */
/*	Calling sequence:						    */
/*									    */
/*		ret = stuff_drive(fp,fcbp,src);				    */
/*									    */
/*	Where:								    */
/*									    */
/*		fp	Is the ccb address				    */
/*		fcbp	Is the FCB address within the ccb		    */
/*		src	Is the source filename string			    */
/*									    */
/****************************************************************************/
MLOCAL	WORD	stuff_drive(fp,fcbp,src)	/*			    */
REG	FD	*fp;				/* -> CCB area		    */
REG	struct fcbtab *fcbp;			/* -> FCB area		    */
REG	BYTE	*src;				/* -> Source string	    */
{						/****************************/
REG	WORD	i;				/* Accumulator		    */
	i = 0;					/* Zap to zero		    */
	while(isdigit(*src))			/* As long as source is dig.*/
	{					/*			    */
	  i = (i*10) + *src++ - '0';		/* Convert to number	    */
	  fp->user = i;				/* store user number	    */
	}					/****************************/
						/*			    */
	if(*src)				/* Drive code letter?	    */
	{					/* Here => yes		    */
	  fcbp->drive = toupper(*src)-'A'+1;	/* get drive code byte	    */
	  src++;				/* Increment source pointer */
	}					/*			    */
	if(*src || (fp->user > 15))		/* Illegal  stuff?	    */
		return(FAILURE);		/* Yes, quit		    */
	return(SUCCESS);			/* OK to continue	    */
}						/****************************/





















