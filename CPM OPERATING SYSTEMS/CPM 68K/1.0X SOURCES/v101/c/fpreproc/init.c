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
} symtab[HSIZE]=0;

	/*buffered I/O structure*/
struct ibuf {
	int fd;
	int nc;
	char *bp;
	char buffer[BUFSIZE];
} outbuf=0;

	/* command line define structure */
struct defstruc {
	char *ptr;
	char *value;
} defs[NDEFS]=0;

struct stackstruc {	/* [vlh] */
	int ifd;
	char ifile[13];
	int lineno;
	struct ibuf inbuf;
} filestack[FSTACK]=0, *filep=0;	/* stack of incl files, ptr to... */
#ifdef BULLSHIT				/* Bullshit, bullshit, bullshit!!!*/
#ifdef VERSADOS
#define NONEST	1
#define NOFORKS	1
#endif

#ifdef VMS
#define NONEST	1
#endif

#ifdef NONEST
struct ibuf holdbuf=0;	/* alternate buffer, hold main file info */
#endif
#endif
int mfail=0;							/*macro error flag*/
int skip=0;							/*skipping current line*/
char *defap=0;						/*pointer to available define area*/
char *defp=0;							/*pointer to next avail define byte*/
int defcount=0;						/*bytes left in define area*/
int defused=0;						/*number of bytes used in define area*/
int defmax=0;							/*maximum define area used*/
int pflag=0;
int asflag=0;

	/*line to output after macro substitution*/
char line[LINESIZE+2]=0;				/*line buffer*/
char *linep=0;						/*current line pointer*/
int loverflow=0;					/*line overflow flag*/
int lineno=0;

	/*push back buffer*/
char pbbuf[PBSIZE]=0;					/*push back buffer*/
char *pbp=0;							/*push back pointer*/
int pbflag=0;							/*checks for recursive definition*/


char *lookup();
char *setend();
char *makecopy();
char *makecopy();
char *maketemp();
char *sbrk();
struct symbol *getsp();
#define STKLEN 64
int oprstk[STKLEN]=0;
int opnstk[STKLEN]=0;
int pristk[STKLEN]=0;
int *oprptr=0;
int *opnptr=0;
int *priptr=0;

int nincl=0;
char *incl[10]=0;
char tmp[6]=0;
#define CSTKSIZE 20
char cstack[CSTKSIZE]=0;
char *cstkptr=0;
char inclname[TOKSIZE]=0;
int cvalue=0;
#define EXPSIZE 1024
int exprarea[EXPSIZE]=0;
