/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) ctype.h - Jul 26, 1983  REGULUS 4.1
*/

#define _UPPER  01
#define _LOWER  02
#define _DIGIT  04
#define _WHTSP  010
#define _PUNCT  020
#define _CNTRL  040
#define _BSPACE 0100
#define _SPACE  0100	/* shared !!!! */
#define _HEXDEC 0200
#define _NASCII	0

extern char _ctype[];

/* Standard routines... */

#define isspace(c) (_ctype[c] & _WHTSP)
#define isupper(c) (_ctype[c] & _UPPER)
#define islower(c) (_ctype[c] & _LOWER)
#define isalpha(c) (_ctype[c] & (_UPPER | _LOWER))
#define isdigit(c) (_ctype[c] & _DIGIT)
#define ishex(c)   (_ctype[c] & _HEXDEC)
#define isascii(c) (_ctype[c] != _NASCII)
#define isalnum(c) (_ctype[c] & (_UPPER|_LOWER|_DIGIT))
#define iscntrl(c) (_ctype[c] & _CNTRL)
#define isprint(c) (_ctype[c] & (_UPPER|_LOWER|_DIGIT|_PUNCT|_BSPACE|_SPACE))
#define ispunct(c) (_ctype[c] & _PUNCT)
#define toupper(c) ((c) & ~040)
#define tolower(c) ((c) | 040)
#define toascii(c) ((c) & 0177)
