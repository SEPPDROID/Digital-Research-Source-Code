/*****************************************************************************
*
*			f r e o p e n   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"freopen" opens a buffered stream file, either on disk, console, or
*	line printer.  It first closes the given stream, and reuses that slot.
*	Comes in 3 flavors, like "fopen".
*
*	Calling sequence:
*		stream = freopen(name,mode,stream)
*		stream = freopa(name,mode,stream)
*		stream = freopb(name,mode,stream)
*		stream = _freopen(name,mode,stream,ascii)
*	Where:
*		stream -> file reopened (FILE *), NULLPTR if failure
*		name   -> NULL terminated string containing filename,
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

#include "stdio.h"				/****************************/
#define READ 0					/* read mode val	    */
#define WRITE 1					/* write mode val	    */

FILE *_freope(name,mode,sp,ascii)		/****************************/
REG	BYTE *name,				/* file name		    */
	     *mode;				/* "r","w", or "a"	    */
REG	FILE *sp;				/* stream pointer	    */
	BOOLEAN ascii;				/* CP/M text file	    */
{						/****************************/
REG	WORD fd;				/* file descriptor	    */
						/****************************/
	if( fclose(sp) == FAILURE )		/* try closing the file 1st */
		return(NULLFILE);		/*   oops, it failed	    */
	if( *mode == 'w' || *mode == 'W' )	/* 'w'rite mode?	    */
		fd=_creat(name,0,ascii);	/*	create file *********/

	else if( *mode == 'a' || *mode == 'A' )	/* 'a'ppend mode?	    */
	{					/*			    */
		if( (fd=_open(name,WRITE,ascii)) < 0 ) /* try open	    */
			fd=_creat(name,0,ascii);/*	do create if needed */
		else	lseek(fd,0L,2);		/* its out there, seef EOF  */
	}					/*   ************************/
	else if( *mode == 'r' || *mode == 'R' )	/* 'r'ead mode?		    */
		fd=_open(name,READ,ascii);	/*	try open ************/
	else return(NULLFILE);			/* bad mode barf...	    */
						/****************************/
	if(fd<0)				/* did one of those work?   */
		return(NULLFILE);		/*	no, oh well	    */
	sp->_cnt = 0;				/* init count		    */
	sp->_fd = fd;				/*  and file des	    */
	sp->_base = sp->_ptr = NULLPTR;		/*  and buffer pointers	    */
	if( *mode == 'r' || *mode == 'R' )	/* 'r'ead mode?		    */
		sp->_flag = _IOREAD;		/*	say so		    */
	else	sp->_flag = _IOWRT;		/* else 'w'rite mode	    */
	if( ascii == 0 )			/* ascii file?		    */
		sp->_flag |= _IOASCI;		/*			    */
						/****************************/
	return(sp);				/* return the stream ptr    */
}						/****************************/

FILE *	freopen(name,mode,sp)			/* CLEAR FUNCTION ***********/
	BYTE *name, *mode; FILE *sp;
{	return(_freope(name,mode,sp,0)); }	/* reopen ascii file	    */

FILE *	freopa(name,mode,sp)			/* CLEAR FUNCTION ***********/
	BYTE *name, *mode; FILE *sp;
{	return(_freope(name,mode,sp,0)); }	/* reopen ascii file	    */

FILE *	freopb(name,mode,sp)			/* CLEAR FUNCTION ***********/
	BYTE *name, *mode; FILE *sp;
{	return(_freope(name,mode,sp,1)); }	/* reopen binary file	    */
E *sp;
{	return(_freope(name,mode,sp,1)); }	/* reopen binary file	    */
E *sp;
{	return(_freope(name,mode,sp,1)); }	/* reopen binary file	    */
