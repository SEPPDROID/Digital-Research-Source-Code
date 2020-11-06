/****************************************************************************
*
*		   C   F I L E   C R E A T E   R O U T I N E
*		   -----------------------------------------
*
*	The "creat" routine opens a new "C" file and returns a file id. 
*
*	Calling Sequence:
*
*		fid = creat(fname,prot,type);
*
*	Where:
*
*		fname	is the address of a null terminated file name.
*		prot	is the UNIX file protection
*		type	is 0 for ASCII, 1 for BINARY
*
*****************************************************************************/
#include <portab.h>
WORD	xcreat(fname,prot,type)			/****************************/
	BYTE	*fname;				/* -> File name		    */
	WORD	prot;				/* Open mode		    */
	WORD	type;				/* ASCII/BINARY flag	    */
{						/****************************/
	return(_creat(fname,prot,type));	/* Call clib routine	    */
}						/****************************/
