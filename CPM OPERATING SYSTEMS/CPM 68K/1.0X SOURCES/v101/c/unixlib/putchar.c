#define BLEN 512

struct iob {
	int fd;		/*file descriptor*/
	int cc;		/*char count*/
	char *cp;	/*ptr to next char*/
	char cbuf[BLEN];	/*char buffer*/
} fout = {0,BLEN,&fout.cbuf[0]};

putchar(cc)
char cc;
{

	if(fout.fd <= 1) {
		if(write(1,&cc,1) != 1)
			return(-1);
		return(cc);
	}
/* buffered output*/
	if(fout.cc<=0) {
		fout.cp = &(fout.cbuf[0]);
		if(write(fout.fd,fout.cp,BLEN) != BLEN)
			return(-1);
		fout.cc = BLEN;
	}
	*(fout.cp)++ = cc;
	fout.cc--;
	return(cc);
}

myflush()
{

	register i;

	i = BLEN - fout.cc;
	fout.cc = BLEN;
	fout.cp = &(fout.cbuf[0]);
	if(write(fout.fd,fout.cp,i) != i)
		return(-1);
	return(0);
}
