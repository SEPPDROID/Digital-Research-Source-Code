
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#)optim.h	1.4	11/11/83";
*/

/* Optimization pass of the C68 compiler */

#ifndef MC68000
#	define fflush v6fflush
#endif

#define ISALPHA(c)		((c>='a' && c<='z') || (c>='A' && c<='Z'))
#define ISOCTAL(c)		(c>='0' && c<='7')
#define ISDIGIT(c)		(c>='0' && c<='9')
#define ISHEX(c)		(ISDIGIT(c) || (c>='a'&&c<='f') || (c>='A'&&c<='F'))
#define ISSPACE(c)		(c==' ' || c=='\t')
#define SYMCHAR(c) 		(c == '_' || c == '~' || c == '.')
#define ISCOMMENT(t)	(t==STAR)
#define ISBRANCH(t)		(t==BRANCH)

#define TRUE		1
#define FALSE		0
#define EOF			0
#define FATAL		1
#define STDERR  	2
#define MAXCOM		20
#define NEWLINE 	'\n'
#define ASTERISK	'*'
#define NULLC		'\0'
#define SPACE       ' '
#define COLON		':'

#define ATEOF		0
#define OTHER		1	/* undefined token */
#define HEXCON 		2
#define OCTCON 		3
#define DECCON 		4
#define SYMBOL 		5
#define SPECIAL		6
#define BRANCH		7
#define STAR  		8	/* asterisk, if first char on line implies comment */

struct iob {
	int fd;
	int cc;
	char *cp;
	char cbuf[512];
} ibuf, obuf, *out, *in;

int pbchar, braop, lab_ndx, com_ndx;
char token[80];

struct record {
	char str[MAXCOM+1];		/* comment */
	int bra;				/* branch number, or valid branch */
};

struct record save_b[20];	/* save branch labels */
struct record save_c[20];	/* save comments */
struct record save_l[20];	/* contiguous labels at same location */
