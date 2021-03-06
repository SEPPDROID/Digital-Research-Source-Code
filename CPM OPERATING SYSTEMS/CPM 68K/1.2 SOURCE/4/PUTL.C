#define BLEN 512
#include <klib.h>
struct iob {
	int fd;		/*file descriptor*/
	int cc;		/*char count*/
	char *cp;	/*ptr to next char*/
	char cbuf[BLEN];	/*char buffer*/
};

long putl(l,ibuf)
register long l;
register struct iob *ibuf;
{
	putc((int)((l>>24)&0xff),ibuf);
	putc((int)((l>>16)&0xff),ibuf);
	putc((int)((l>> 8)&0xff),ibuf);
	putc((int)l       &0xff,ibuf);
	return(l);
}

long lputl(x,buff)
long	*x;
struct iobuf *buff;
{
	return(putl(*x,buff));
}
                          	return(l);
}

long lputl(x,buff)
long	*x;
struct iobuf *buff;
{
	return(putl(*x,buff));
}
                          	return(l);
}

long lputl(x,buff)
long	*x;
struct iobuf *buff;
{
	return(putl(*x,buff));
}
                          