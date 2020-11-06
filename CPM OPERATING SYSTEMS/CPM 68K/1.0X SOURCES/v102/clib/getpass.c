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
**************************************************************************/
#include <portab.h>
#include <cpm.h>
BYTE *getpass(prompt)
	BYTE *prompt;
{
	MLOCAL ibuf[9];

	_chrout(prompt,(LONG)strlen(prompt),CONOUT);/* same fn write uses */
	_noecho(ibuf,9);
	return(ibuf);
}

#define INCHAR 0xFFL
#define DEL 0177
#define CTRLX 030
#define CTRLC 003
VOID _noecho(bf,ln)
	BYTE *bf;
	WORD ln;
{
	WORD cur;
	WORD ch;

	cur = 0;
	FOREVER {
		switch( ch = __BDOS(CONIO,INCHAR) & 0177 ) {
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
			*bf = ZERO;
			return;
		case CTRLC:
			exit(1);
		default:
			*bf++ = ch;
			cur++;
			if( cur+1 >= ln ) {
				*bf = ZERO;
				return;
			}
		}
	}
}
