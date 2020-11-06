#include "portab.h"

LONG	readl(fd,buf,lnum)			/* CLEAR FUNCTION ***********/
WORD		fd;		/* file descriptor */
REG BYTE	*buf;		/* addr */
LONG	 	lnum;		/* size */
{
LONG		R;
REG UWORD	tmp;

	R = lnum;
	while (lnum != 0)
		{
		  tmp = (lnum < 32768L)? (int)lnum : 32768;
		  if (read(fd,buf,tmp) != tmp)
			break;
		  buf += tmp;
		  lnum -= tmp;
		}
	return(R-lnum);		/* number of bytes read */
}
