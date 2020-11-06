/*	SEEK FOR VMS
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "vms.h"

FILE lseek(fd, loff, way)
	FILE fd;
	LONG loff;
	COUNT way;
	{
	FAST RCB *p;

	if (!(p = _ckfd(fd)) || !(p->flags & WFIX))
		return (FAIL);
	else
		{
		if (way == 0)
			p->lseek = loff;
		else if (way == 1)
			p->lseek =+ loff;
		else
			p->lseek = p->lend + loff;
		return (p->lseek);
		}
	}
