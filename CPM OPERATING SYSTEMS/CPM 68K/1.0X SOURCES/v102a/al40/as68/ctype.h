/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#)ctype - Aug 13, 1982";

 Standard routines... */

#define isspace(c)	((c)==32 || (c)==9 || (c)==13 || (c)==10 ||(c)==12)

#define isupper(c)	((c)<='Z' && (c)>='A')

#define islower(c)	((c)<='z' && (c)>='a')

#define isalpha(c)	(isupper((c)) || islower((c)))

#define isdigit(c)	((c)<='9' && (c)>='0')

#define isascii(c)	((c) < 0200 && (c) >= 0)

#define isalnum(c)	(isalpha((c)) || isdigit((c)))

#define iscntrl(c)	(((c) < 040 || (c) == 0177) && isascii((c)))

#define isprint(c)	((c) >= 040 && (c) < 0177)

#define ispunct(c)	(!isalnum((c)) && isprint(c))

#define toupper(c)	(islower((c)) ? (c) & ~32 : (c))

#define tolower(c)	(isupper((c)) ? (c) | 32 : (c))

#define toascii(c)	((c) & 0177)
