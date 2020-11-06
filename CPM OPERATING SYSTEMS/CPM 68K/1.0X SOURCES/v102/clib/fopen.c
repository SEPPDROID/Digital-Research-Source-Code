/**************************************************************************
*
*			f o p e n   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fopen" opens a buffered stream file, either on disk, console, or
*	line printer.
*	Note that "fopen" comes in three (3) flavors, one for ascii CP/M
*	files, another for binary files, and the default (presently to
*	ascii files).
*
*	Calling sequence:
*		stream = fopena(name,mode)	(ascii files)
*		stream = fopenb(name,mode)	(binary files)
*		stream = fopen(name,mode)	(default-ascii files)
*		stream = _fopen(name,mode,ascii)
*	Where:
*		stream -> file opened (FILE *), NULL if failure
*		name   -> null terminated string containing filename,
*				name="CON:" means the console,
*				name="LST:" goes to the line printer, and
*				otherwise a disk file is assumed.
*		mode   -> "r" for read only,
*			  "w" for write only,
*			  "a" to append (write only) to end of file.
*		ascii  = 0 if the file is handled as a CP/M text file
*			(newline -> CRLF xlation, ^Z EOF convention).
*			Otherwise, file is treated as a binary file.
*
*****************************************************************************/
#include <stdio.h>				/****************************/
#define READ 0					/* read mode val	    */
#define WRITE 1					/* write mode val	    */
#define CREATMODE 1				/* mode to use for 'creat'  */
						/****************************/
FILE *fopen(name,mode)				/*			    */
	BYTE *name, *mode;			/*			    */
{	FILE *_fopen();				/*			    */
	return(_fopen(name,mode,0)); 	}	/* ascii file open	    */
						/****************************/
FILE *fopena(name,mode)				/*			    */
	BYTE *name,*mode;			/*			    */
{	FILE *_fopen();				/*			    */
	return(_fopen(name,mode,0));	}	/* ascii file open	    */
						/****************************/
FILE *fopenb(name,mode)				/*			    */
	BYTE *name, *mode;			/*			    */
{	FILE *_fopen();				/*			    */
	return(_fopen(name,mode,1));	}	/* binary file open	    */
						/****************************/
FILE *_fopen(name,mode,ascii)			/*			    */
REG	BYTE *name,				/* file name		    */
	     *mode;				/* "r","w", or "a"	    */
	BOOLEAN ascii;				/* CP/M text file	    */
{						/****************************/
REG	FILE *sp;				/* stream pointer	    */
REG	WORD ii,				/* index into _iob	    */
	     fd;				/* file descriptor	    */
						/****************************/
	for( ii=0; 				/* look at _iob table	    */
	    (sp=(&_iob[ii]))->_flag&(_IOREAD|_IOWRT); /* not marked rd/wrt  */
	    ii++ )				/*			    */
		if( ii >= MAXFILES )		/* if off end of table	    */
		    break;			/*			    */
	if( ii >= MAXFILES )			/*			    */
		return((BYTE *)NULL);		/*   fail		    */
	if( *mode == 'w' || *mode == 'W' )	/* 'w'rite mode?	    */
		fd=_creat(name,CREATMODE,ascii);/*	create file *********/
	else if( *mode == 'a' || *mode == 'A' )	/* 'a'ppend mode?	    */
	{					/*			    */
		if( (fd=_open(name,WRITE,ascii)) < 0 ) /* try open	    */
			fd=_creat(name,CREATMODE,ascii);/* ow. do create    */
		else	lseek(fd,0L,2);		/* its out there, seef EOF  */
	}					/*   ************************/
	else if( *mode == 'r' || *mode == 'R' )	/* 'r'ead mode?		    */
		fd=_open(name,READ,ascii);	/*	try open ************/
	else return((BYTE *)NULL);		/* bad mode barf...	    */
						/****************************/
	if(fd<0)				/* did one of those work?   */
		return((BYTE *)NULL);		/*	no, oh well	    */
	sp->_cnt = 0;				/* init count		    */
	sp->_fd = fd;				/*  and file des	    */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers	    */
	if( *mode == 'r' || *mode == 'R' )	/* 'r'ead mode?		    */
		sp->_flag = _IOREAD;		/*	say so		    */
	else	sp->_flag = _IOWRT;		/* else 'w'rite mode	    */
	if( ascii == 0 )			/* ascii mode?		    */
		sp->_flag |= _IOASCI;		/*			    */
						/****************************/
	return(sp);				/* return the stream ptr    */
}						/****************************/
