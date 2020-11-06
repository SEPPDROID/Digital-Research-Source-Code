/****************************************************************************/
/*									    */
/*			    _ e x i t   F u n c t i o n			    */
/*			    ---------------------------			    */
/*	Copyright 1982 by Digital Research Inc.  All rights reserved.	    */
/*									    */
/*	The _exit function is called from anywhere to pass control back to  */
/*	the CCP from the executing C program. No cleaning (of files).	    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		_exit(code);						    */
/*									    */
/*	Where:								    */
/*		code	Is the exit status (ignored)			    */
/*									    */
/****************************************************************************/
#include <portab.h>
#include <cpm.h>

_exit(code)					/***************************/
	WORD	code;				/* Exit status		   */
{						/*			   */
	__BDOS(EXIT,0L);			/* Exit to BDOS		   */
}						/***************************/
