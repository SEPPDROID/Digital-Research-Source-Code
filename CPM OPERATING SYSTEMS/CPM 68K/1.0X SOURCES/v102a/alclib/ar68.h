/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) ar68.h - Jul 26, 1983  REGULUS 4.1
*/

#define LIBMAGIC	0177545
#define LIBHDSIZE	26
#define LIBNSIZE	14

struct libhdr {
	char lfname[LIBNSIZE];
	long lmodti;
	char luserid;
	char lgid;
#ifndef VAX11
	int  lfimode;
#else
	unsigned short lfimode;
#endif
	long lfsize;
};

