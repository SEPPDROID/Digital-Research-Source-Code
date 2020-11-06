
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) v6fflush.c - Sep 12, 1983  REGULUS 4.1
*/


/* v6 v6fflush library call */

#define BSIZE 512

struct iob {
	int fd;		/* file descriptor */
	int cc;		/* char count */
	char *cp;	/* ptr to next char */
	char cbuf[BSIZE];	/* char buffer */
};

v6fflush(ibuf)
register struct iob *ibuf;
{
	register i;

	i = BSIZE - ibuf->cc;
	ibuf->cc = BSIZE;
	ibuf->cp = &(ibuf->cbuf[0]);
	if (write(ibuf->fd,ibuf->cp,i) != i)
		return(-1);
	return(0);
}

