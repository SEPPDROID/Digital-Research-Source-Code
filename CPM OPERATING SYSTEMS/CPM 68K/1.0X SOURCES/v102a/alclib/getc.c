
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) getc.c - Sep 12, 1983  REGULUS 4.1
*/

/* v6 getc library routines */

#define BSIZE 512

struct iob {
	int fd;		/* file descriptor */
	int cc;		/* char count */
	char *cp;	/* ptr to next char */
	char cbuf[BSIZE];	/* char buffer */
};

getc(ibuf)
struct iob *ibuf;
{
	if (ibuf->cc <= 0) {
		ibuf->cp = &(ibuf->cbuf[0]);
		ibuf->cc = read(ibuf->fd,ibuf->cp,BSIZE);
	}
	if (ibuf->cc <= 0)
		return(-1);
	ibuf->cc--;
	return((int)((*(ibuf->cp)++)&0xff));
}

