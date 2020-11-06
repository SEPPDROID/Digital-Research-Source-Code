/*	READ A FILE FOR VMS
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */
#include <std.h>
#include "vms.h"

COUNT read(fd, buf, size)
	FILE fd;
	FAST TEXT *buf;
	COUNT size;
	{
	FAST RCB *p;
	FAST BYTES n;
	FAST BYTES bytesread;
	BYTES boff;
	COUNT stat;

	if (!(p = _ckfd(fd)))
		return (FAIL);
	bytesread = 0;
	for (n = 0; n < size; )
		{
		if (p->flags & WFIX)
			{
			boff = p->lseek & 0777;
			if (p->lseek - boff == p->lact)
				;
			else if (!_wblk(p))
				return (FAIL);
			else if (!(stat = _read(p->rab, p->pbuf, BUFSIZE,
				(p->lseek >> 9) + 1)))
				break;
			else if (stat != BUFSIZE)
				{
				p->lact = NOBLOCK;
				if (bytesread == 0)
					return (FAIL);
				else
					return (bytesread);
				}
			p->lact = p->lseek - boff;
			p->bnext = p->pbuf + boff;
			p->bleft = BUFSIZE - boff;
			for (; n < size && 0 < p->bleft; --p->bleft, ++p->lseek)
				*buf++ = *p->bnext++, ++n,++bytesread;
			}
		else if (p->bleft)
			{
			for (; n < size && 0 < p->bleft; --p->bleft, ++p->bnext)
				if (*p->bnext != '\r' && *p->bnext != '\f' &&
					*p->bnext != '\0')
					*buf++ = *p->bnext, ++n;
			}
		else if ((stat = _read(p->rab, p->pbuf, BUFSIZE - 1, 0)) < -1)
			return (FAIL);
		else if (stat == -1)
			break;
		else
			{
			p->bleft = stat;
			p->bnext = p->pbuf;
			if (p->flags & (WCR|WTTY))
				p->pbuf[p->bleft++] = '\n';
			}
		if (p->flags & WTTY && n)
			break;
		}
	return (n);
	}
