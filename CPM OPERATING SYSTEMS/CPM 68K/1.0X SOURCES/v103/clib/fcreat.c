
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) fcreat.c - Sep 12, 1983  REGULUS 4.1
*/

/* v6 fcreat library call */

#define BSIZE 512

struct iob {
	int fd;		/* file descriptor */
	int cc;		/* char count */
	char *cp;	/* ptr to next char */
	char cbuf[BSIZE];	/* char buffer */
};

fcreat(fname,ibuf)
char *fname;
register struct iob *ibuf;
{
	ibuf->cc = BSIZE;	/* no chars */
	ibuf->cp = &(ibuf->cbuf[0]);
	return(ibuf->fd = creat(fname,0666));
}
