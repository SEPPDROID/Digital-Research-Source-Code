/****************************************************************************/
/*									    */
/*			    E x e c   F u n c t i o n			    */
/*			    -------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
/*									    */
/*	The execl function is called from anywhere to pass control to  	    */
/*	another program from the the executing C program.		    */
/*	Note that the stream files are closed via '_cleanup()'.		    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		execl(name,arg0,arg1,...,argn,NULLPTR);			    */
/*									    */
/*	Where:								    */
/*		name, arg0, arg1...argn are pointers to character strings   */
/*									    */
/****************************************************************************/
#include <stdio.h>
#include <cpm.h>
#include <ctype.h>
execl(name,arg0)				/***************************/
	UBYTE *name,*arg0;			/* pointers arguments      */
{						/*			   */
	UBYTE **args;				/* used to index into args */
	UBYTE cmdline[128];			/* CP/M command line area  */
REG	UBYTE *p;				/* Temporary pointer	   */
	WORD  i;				/***************************/
						/*			   */
	_cleanup();				/* Close all (stream) files*/
	__BDOS(SETDMA,&cmdline[0]);		/* DMA -> Command line	   */
						/* now build cmdline 	   */
	p = &cmdline[1];			/* Load temp		   */
	strcpy(&cmdline[1],name);		/* Copy name portion	   */
	for(i=1; i < strlen(p); i++)		/* uppercase command name  */
	  if (islower(p[i]))			/***************************/
	    p[i] = toupper(p[i]);		/*			   */
						/*			   */
	args = &arg0;				/* Copy args		   */
	args++;					/* arg0 is a dup of the    */
						/*	      command name */
	while(*args != NULL)			/***************************/
	  {					/*			   */
            strcat(p," ");			/* Add a blank		   */
	    strcat(p,*args++);			/* Add next arg		   */
	  }					/***************************/
	cmdline[0] = strlen(&cmdline[1]);	/* Set commmand length	   */
	__BDOS(CHAIN,0L);			/* chain to program        */
}						/***************************/
