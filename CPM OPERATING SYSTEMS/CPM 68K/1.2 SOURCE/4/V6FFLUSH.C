#include "iob.h"
v6fflush(v6buf)
register struct iob *v6buf;
{
	register short i;

	i = BSIZE - v6buf->cc;
	v6buf->cc = BSIZE;
	v6buf->cp = &(v6buf->cbuf[0]);
	if(write(v6buf->fd,v6buf->cp,i) != i)
		return(-1);
	return(0);
}
                IZE;
	v6buf->cp = &(v6buf->cbuf[0]);
	if(write(v6buf->fd,v6buf->cp,i) != i)
		return(-1);
	return(0);
}
                IZE;
	v6buf->cp = &(v6buf->cbuf[0]);
	if(write(v6buf->fd,v6buf->cp,i) != i)
		return(-1);
	return(0);
}
                