/*****************************************************************************
*
*		R U N T I M E   S T A R T O F F   R O U T I N E
*		-----------------------------------------------
*	Copyright 1982,83 by Digital Research Inc.  All rights reserved.
*
*	Routine "_main" is entered from the C header routine to start a C 
*	program.  It opens STDIN, STDOUT and STDERR to the console, and
*	translates the command line to lower case.  Then it calls "__main()".
*	There are two versions of "__main()" (one compiled for wildcards
*	and the other compiled without) compiled from the source module
*	"XMAIN.C".
*
*	Calling Sequence:
*		return = _main(command,length);
*
*	Where:
*		command		Is the address of the command line from CP/M
*		length		Is the number of characters in the line, 
*				excluding the termination character (CR/LF).
*
*	Modfications:
*		11-Oct-83 whf  converted to DRC from CPM68K
*
*****************************************************************************/

#include "portab.h"				/* Include std definitions  */
#include "osif.h"				/*			    */
#include "ctype.h"				/*			    */

EXTERN	BYTE	__tname[];			/* Name of terminal device  */
EXTERN	BYTE	__fdecls();			/* force loading of _iob    */
EXTERN	BYTE	___atab();			/* force loading of ___atab */

#define STDIN 0					/* file descriptors	    */
#define STDOUT 1				/*			    */
#define STDERR 2				/*			    */
						/*			    */
_main(com, len)					/****************************/
	BYTE 	*com;				/* Command address	    */
	WORD	len;				/* Command length	    */
{						/****************************/
REG	BYTE 	*s;				/* Temp byte pointer	    */
						/****************************/
	_chinit();				/* Initialize channels	    */
	open(__tname,READ);			/* Open STDIN		    */
	open(__tname,WRITE);			/* Open STDOUT		    */
	open(__tname,WRITE);			/* Open STDERR		    */
	com[len] = NULL;			/* Insure null at line end  */
	for (s = com; *s; s++)			/* Convert string to	    */
		*s = tolower(*s);		/*          lower case	    */
	__main(com,len);			/* Do rest (replaceable)    */
}						/****************************/

in(com,len);			/* Do rest (replaceable)    */
}						/****************************/

in(com,len);			/* Do rest (replaceable)    */
}						/****************************/

