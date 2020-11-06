/**************************************************************************
*
*			s s c a n f   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"sscanf" scans the given string for items specified, and assigns
*	them to user supplied variables (via pointers to these vars).
*
*	Calling sequence:
*		ret = sscanf(str,fmt,p1,p2,...)
*	Where:
*		ret =  the number of items successfully matched & assigned
*			EOF returned if encountered on input
*		str -> a (null terminated) string to scan
*		fmt -> a string specifying how to parse the input
*		p1,p2,... -> where matched items get stored
*
*****************************************************************************/
#include <stdio.h>
WORD sscanf(str,fmt,ptrs)
	BYTE *str;
	BYTE *fmt;
	BYTE *ptrs;
{
	FILE spbuf, *sp;
	BYTE locbuf[BUFSIZ];			/* set up a local buffer    */

	sp = &spbuf;				/* set up a pseudo stream   */
	sp->_fd = -1;
	sp->_flag = _IOREAD | _IOSTRI;		/* readable string	    */
	sp->_cnt = strlen(str) + 1;		/* allow doscan to read NULL*/
	strcpy(locbuf,str);			/* copy to safe place	    */
	sp->_base = sp->_ptr = locbuf;
	return(_doscan(sp,fmt,&ptrs));
}
