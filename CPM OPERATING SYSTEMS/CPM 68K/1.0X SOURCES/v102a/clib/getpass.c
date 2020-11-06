/***********************************************************************
*
*			g e t p a s s   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"getpass" writes a prompt to the console (NOT stdout) and accepts
*	a string of 1-8 chars from the console (NOT stdin) without 
*	echoing the input to the terminal.  Only limited CP/M line edit
*	commands are available (CTL H, DEL, CTL X) on input.
*
*	Calling sequence:
*		pret = getpass(prompt)
*	Where:
*		pret -> ptr to 1-8 chars of null terminated password
*		prompt -> null teminated console out msg.
*
*	Modified:
*	Oct 83 whf - handle OS Independent Raw Input
*
**************************************************************************/

#include "portab.h"
#include "osif.h"

BYTE *	getpass(prompt)				/* CLEAR FUNCTION ***********/
	BYTE *prompt;
{
	MLOCAL BYTE ibuf[9];

	_conout(prompt,strlen(prompt),CONOUT);	/* same fn write uses */
	_noecho(ibuf,9);
	return(ibuf);
}

#define CMASK	0x7F
#define DEL	0x7F
#define CTRLX	0x18
#define CTRLC	0x03
MLOCAL _noecho(bf,ln)
REG	BYTE *bf;
	WORD ln;
{
REG	WORD cur;
	WORD ch;

	cur = 0;
	FOREVER {
		switch( ch = CMASK & _ttyinraw()) {
		case '\b':
		case DEL:
			if( cur>0 ) {
				cur--;
				bf--;
			}
			break;
		case CTRLX:
			bf -= cur;
			cur = 0;
			break;
		case '\r':
		case '\n':
			*bf = NULL;
			return;
		case CTRLC:
			exit(1);
		default:
			*bf++ = ch;
			cur++;
			if( cur+1 >= ln ) {
				*bf = NULL;
				return;
			}
		}
	}
}


/****************************************************************************/


MLOCAL _conout(buffer,count,os_func)		/***************************/
REG	BYTE	*buffer;			/* -> 1st char output	   */
REG	WORD	count;				/* =  # bytes to xfer	   */
REG	WORD	os_func;			/* OS function to use	   */
{						/***************************/
	WORD	xcount;				/* save area for count	   */
						/*			   */
	while(count-- > 0)			/* Until all written	   */
		__OSIF(os_func,*buffer++);	/* Output next character   */
}						/***************************/
