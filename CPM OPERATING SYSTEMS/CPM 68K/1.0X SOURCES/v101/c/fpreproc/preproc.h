/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "icode.h"
#include "machine.h"
	/*cexpr operators*/
#define EOF		0
#define SUB		1
#define ADD		2
#define NOT		3
#define NEG		4
#define LPAREN	5
#define RPAREN	6
#define QMARK	7
#define COLON	8
#define OR		9
#define AND		10
#define XOR		11
#define EQUAL	12
#define NEQUAL	13
#define LESS	14
#define LSEQUAL	15
#define GREAT	16
#define GREQUAL	17
#define LSHIFT	18
#define RSHIFT	19
#define MULT	20
#define DIV		21
#define MOD		22
#define	COMPL	23
#define	CONST	24
#define	LASTOP	COMPL					/*up to here used by cexpr*/
#define	SQUOTE	25
#define	DQUOTE	26
#define	ANYC	27
#define	BADC	28
#define	COMMA	29
#define	NEWL	30
#define	POUND	31
#define	ALPHA	32
#define	DIGIT	33
#define	BSLASH	34
#define	WHITE	35
#define BUFSIZE	512
#define	LINESIZE	512
#define	ARG			-1
#define	NEWLABEL	-2
#define	LABEL		-3
#define	NOARGS		-4
#define	MAXARGS		60
#define	ARGBSIZE	1000
#define	TOKSIZE	300				/*BUG 4/20/82 was 128*/
#define	DEFSIZE	1024
#define PBSIZE	1000
#define	DEFINE	1
#define	UNDEF	2
#define	INCLUDE	3
#define	IFDEF	4
#define	IFNDEF	5
#define	ELSE	6
#define	ENDIF	7
#define	IF		8
#define	SKIP	0
#define	NOSKIP	1
#define	SOH		'\01'
#define	SSIZE	8
#define	HSIZE	517		/* 3.4 made prime */
#define	FSTACK	10

#define TRUE	1
#define FALSE	0
#define NDEFS	20

struct symbol {
	char s_name[SSIZE];
	char *s_def;
} symtab[HSIZE];

	/*buffered I/O structure*/
struct ibuf {
	int fd;
	int nc;
	char *bp;
	char buffer[BUFSIZE];
} outbuf;

	/* command line define structure */
struct defstruc {
	char *ptr;
	char *value;
} defs[NDEFS];

struct stackstruc {	/* [vlh] */
	int ifd;
	char ifile[13];
	int lineno;
	struct ibuf inbuf;
} filestack[FSTACK], *filep;		/* stack of incl files, ptr to... */

#ifdef BULLSHIT				/* Bullshit, bullshit, bullshit!!!*/
#ifdef VERSADOS
#define NONEST	1
#define NOFORKS	1
#endif

#ifdef VMS
#define NONEST	1
#endif

#ifdef NONEST
struct ibuf holdbuf;	/* alternate buffer, hold main file info */
#endif
#endif

int mfail;							/*macro error flag*/
int skip;							/*skipping current line*/
char *defap;						/*pointer to available define area*/
char *defp;							/*pointer to next avail define byte*/
int defcount;						/*bytes left in define area*/
int defused;						/*number of bytes used in define area*/
int defmax;							/*maximum define area used*/
int pflag;
int asflag;

	/*line to output after macro substitution*/
char line[LINESIZE+2];				/*line buffer*/
char *linep;						/*current line pointer*/
int loverflow;						/*line overflow flag*/
int lineno;

	/*push back buffer*/
char pbbuf[PBSIZE];					/*push back buffer*/
char *pbp;							/*push back pointer*/
int pbflag;							/*checks for recursive definition*/

char null[];						/*used for undef'd symbols*/

char *lookup();
#ifdef	BULLSHIT
char *maketemp();
char *setend();
char *makecopy();
#endif
char *sbrk();
struct symbol *getsp();
