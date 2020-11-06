/*	CREATE A NEW FILE FOR VMS
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "vms.h"

FILE creat(fname, mode)
	TEXT *fname;
	COUNT mode;
{
	return (_creat(fname, mode, 0));
}
FILE creata(fname, mode)
	TEXT *fname;
	COUNT mode;
{
	return (_creat(fname, mode, 0));
}
FILE creatb(fname, mode)
	TEXT *fname;
	COUNT mode;
{
	return (_creat(fname, mode, 1));
}

FILE _creat(fname, mode, rsize)
	TEXT *fname;
	COUNT mode;
	BYTES rsize;
{
	return (_copen(fname, (mode + 1) & 03, rsize));
}
