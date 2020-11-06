#define BLEN 512
#include <klib.h>
struct iob {
	int fd;		/*file descriptor*/
	int cc;		/*char count*/
	char *cp;	/*ptr to next char*/
	char cbuf[BLEN];	/*char buffer*/
};

fopen(fname,ibuf,x)
char *fname;
register struct iob *ibuf;
int x;
{

	ibuf->cc = 0;	/*no chars*/
	x = (x == 0) ? 0 : 1;
	return(ibuf->fd=open(fname,0,x));
}

getc(ibuf)
register struct iob *ibuf;
{

	if(ibuf->cc<=0) {
		ibuf->cp = &(ibuf->cbuf[0]);
		ibuf->cc = read(ibuf->fd,ibuf->cp,BLEN);
	}
	if(ibuf->cc <= 0) {
		return(-1);
	}
	ibuf->cc--;
	return((int)(*(ibuf->cp)++)&0xff);
}

getw(ibuf)
register struct iob *ibuf;
{

	register int j;
	register int i;

	if((j=getc(ibuf)) == -1)
		return(-1);
	i = (j&0377) << 8;
	if((j=getc(ibuf)) == -1)
		return(-1);
	i =| (j&0377);
	if(i&0100000)
		i =| 0xffff0000;	/* make it negative */
	return(i);
}
