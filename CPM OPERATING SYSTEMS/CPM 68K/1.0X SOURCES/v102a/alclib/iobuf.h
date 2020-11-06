/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) iobuf.h - Jul 26, 1983  REGULUS 4.1
*/

#define BLEN	512

struct iobuf{
	int fildes;
	int nunused;
	char *xfree;
	char buff[BLEN];
};

#define FILE struct iobuf
