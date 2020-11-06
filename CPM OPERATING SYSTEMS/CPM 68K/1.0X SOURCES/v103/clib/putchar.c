
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) putchar.c - Sep 12, 1983  REGULUS 4.1
*/

/* version 6 putchar library routine */

#define BSIZE 512

struct iob {
	int fd;		/* file descriptor */
	int cc;		/* char count */
	char *cp;	/* prt to next char */
	char cbuf[BSIZE];	/* char buffer */
} fout = {0,BSIZE,&fout.cbuf[0]};

putchar(cc)
char cc;
{
	if (fout.fd <= 1) {
		if (write(1,&cc,1) != 1)
			return(-1);
		return(cc);
	}

	/* buffered output */
	if (fout.cc <= 0) {
		fout.cp = &(fout.cbuf[0]);
		if (write(fout.fd,fout.cp,BSIZE) != BSIZE)
			return(-1);
		fout.cc = BSIZE;
	}
	*(fout.cp)++ = cc;
	fout.cc--;
	return(cc);
}

flush()
{
	register i;

	i = BSIZE - fout.cc;
	fout.cc = BSIZE;
	fout.cp = &(fout.cbuf[0]);
	if (write(fout.fd,fout.cp,i) != i)
		return(-1);
	return(0);
}

