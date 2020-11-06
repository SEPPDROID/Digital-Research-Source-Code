/*	OPEN A FILE FOR VMS
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "vms.h"
FILE  open(fname, mode)
TEXT	*fname;
COUNT	mode;
{
	return(_open(fname,mode,0));
}

FILE	opena(fname,mode)
TEXT	*fname;
COUNT	mode;
{
	return(_open(fname,mode,0));
}

FILE	openb(fname,mode)
TEXT	*fname;
COUNT	mode;
{
	return(_open(fname,mode,1));
}

FILE _open(fname, mode, rsize)
	TEXT *fname;
	COUNT mode;
	BYTES rsize;
{
	return (_copen(fname, WOPEN | (mode + 1) & 03, rsize));
}
