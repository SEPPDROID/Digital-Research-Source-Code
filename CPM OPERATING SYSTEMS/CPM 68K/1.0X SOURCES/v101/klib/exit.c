/****************************************************************************/
/*									    */
/*			    E x i t   F u n c t i o n			    */
/*			    -------------------------			    */
/*									    */
/*	The exit function is called from anywhere to pass control back to   */
/*	the CCP from the executing C program.				    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		exit(code);						    */
/*									    */
/*	Where:								    */
/*		code	Is the exit status (ignored)			    */
/*									    */
/****************************************************************************/
#include "stdio.h"
#include "cpm.h"

exit(code)					/***************************/
						/*			   */
	WORD	code;				/* Exit status		   */
{						/*			   */
REG	WORD	i;				/* Counter		   */
						/*			   */
	for(i=0; i<MAXFILE; i++)		/* Close all		   */
		close(i);			/*		Open files */
						/***************************/
	_exit(code);				/* Do the exit now	   */
}						/***************************/
_exit(code)					/*			   */
WORD	code;					/***************************/
{						/*			   */
	__BDOS(EXIT,0L);			/* Exit to BDOS		   */
}						/***************************/
