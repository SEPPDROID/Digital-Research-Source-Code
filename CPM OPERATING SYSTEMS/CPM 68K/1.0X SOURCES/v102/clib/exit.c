/****************************************************************************/
/*									    */
/*			    E x i t   F u n c t i o n			    */
/*			    -------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
/*									    */
/*	The exit function is called from anywhere to pass control back to   */
/*	the CCP from the executing C program.				    */
/*	Note that the stream files are closed via '_cleanup()'.		    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		exit(code);						    */
/*									    */
/*	Where:								    */
/*		code	Is the exit status (ignored)			    */
/*									    */
/****************************************************************************/
#include <stdio.h>
#include <cpm.h>

exit(code)					/***************************/
	WORD	code;				/* Exit status		   */
{						/*			   */
	_cleanup();				/* Close all (stream) files*/
	_exit(code);				/* return to O.S.	   */
}						/***************************/
