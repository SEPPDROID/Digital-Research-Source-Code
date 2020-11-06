
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) getchar.c - Sep 12, 1983  REGULUS 4.1
*/

/* v6 getchar routine */

#define BSIZE 512

struct iob {
	int fd;		/* file descriptor */
	int cc;		/* char count */
	char *cp;	/* ptr to next character */
	char cbuf[BSIZE];	/* char buffer */
} fin;

getchar()
{
	char c;
	register int i;

	if (fin.fd == 0) {
		if (read(0,&c,1) <= 0 || c==4)
			return(0);
		i = c;
		return(i & 0xff);
	}
	if (fin.cc <= 0) {
		fin.cp = &(fin.cbuf[0]);
		fin.cc = read(fin.fd,fin.cp,BSIZE);
	}
	if (fin.cc <= 0)
		return(0);
	fin.cc--;
	i = *(fin.cp)++;
	return(i & 0xff);
}


