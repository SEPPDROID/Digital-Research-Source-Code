/***********************************************************************
*
*			m k t e m p   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"mktemp" assumes it is called with a string ending in 6 X's.
*	It will replace the last 6 chars with the process id
*	and a letter (different on each invocation).
*	REALLY "mktemp" replaces everything after the 1st x...
*		this is a feature...
*	Note that the process id does not change under CP/M,
*	therefore the name itself will not be very unique...
*
*	Calling sequence:
*		string = mktemp(string)
*	Where:
*
*		string -> null terminated string with 6 x's
*
*************************************************************************/

#include "portab.h"

MLOCAL BYTE _mktchar = { 'A' };

BYTE *	mktemp(template)			/* CLEAR FUNCTION ***********/
REG	BYTE *template;
{
REG	BYTE *ss;

	for( ss=template; *ss != 'x'  &&  *ss != 'X'; ss++)
		if(*ss == NULL)
			return(template);	/* resign		    */
	if( _mktchar < 'A'  ||  _mktchar >'Z' )
		_mktchar = 'A';
	sprintf(ss,"X%04.4d%c",getpid(),_mktchar);
	_mktchar++;
	return(template);
}
char++;
	return(template);
}
char++;
	return(template);
}
