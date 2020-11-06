
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) putc.c - Sep 12, 1983  REGULUS 4.1
*/

#define BSIZE 512

struct iob {
	int fd;		/* file descriptor */
	int cc;		/* char count */
	char *cp;	/* ptr to next char */
	char cbuf[BSIZE];	/* char buffer */
};

putc(c,ibuf)
char c;
register struct iob *ibuf;
{
	if (ibuf->cc <= 0) {
		ibuf->cp = &(ibuf->cbuf[0]);
		if (write(ibuf->fd,ibuf->cp,BSIZE) != BSIZE)
			return(-1);
		ibuf->cc = BSIZE;
	}
	*(ibuf->cp)++ = c;
	ibuf->cc--;
	return((int)(c&0xff));
}

