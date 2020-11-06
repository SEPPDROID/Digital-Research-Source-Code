#define BLEN 512

struct iob {
	int fd;		/*file descriptor*/
	int cc;		/*char count*/
	char *cp;	/*ptr to next char*/
	char cbuf[BLEN];	/*char buffer*/
};

fcreat(fname,ibuf,x)
char *fname;
int  x;
register struct iob *ibuf;
{

	ibuf->cc = BLEN;	/*no chars*/
	ibuf->cp = &(ibuf->cbuf[0]);
	x = (x == 0) ? 0 : 1;
	return(ibuf->fd=creat(fname,0666,x));
}

putc(c,ibuf)
char c;
register struct iob *ibuf;
{

	if(ibuf->cc<=0) {
		ibuf->cp = &(ibuf->cbuf[0]);
		if(write(ibuf->fd,ibuf->cp,BLEN) != BLEN)
			return(-1);
		ibuf->cc = BLEN;
	}
	*(ibuf->cp)++ = c;
	ibuf->cc--;
	return(c);
}

putw(w,ibuf)
int w;
register struct iob *ibuf;
{

	register j;
	int i;

	putc((char)w,ibuf);
	putc((char)(w>>8),ibuf);
	return(w);
}

myfflush(ibuf)
register struct iob *ibuf;
{

	register i;

	i = BLEN - ibuf->cc;
	ibuf->cc = BLEN;
	ibuf->cp = &(ibuf->cbuf[0]);
	if(write(ibuf->fd,ibuf->cp,i) != i)
		return(-1);
	return(0);
}
