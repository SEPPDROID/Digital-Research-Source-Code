/*
	Copyright 1981
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "mach.h"
#include <ctype.h>

#ifdef PDP11
	struct {
		char lobyte;
		char hibyte;
	};
	struct {
		char *ptrw1;
		char *ptrw2;
	};
	struct {
		int wd1;
		int wd2;
	};
	struct {
		int swd1;
	};
#endif

#ifdef MC68000
	struct {
		char hibyte;
		char lobyte;
	};
	struct {
		char *ptrw2;
	};
	struct {
		int wd1;
		int wd2;
	};
	struct {
		int swd1;
	};
#endif

#ifdef VAX11
	struct {
		short wd2;
		short wd1;
	};
	struct {
		short swd1;
	};

	struct {
		char lobyte;
		char hibyte;
	};
	struct {
		char *ptrw2;
	};
#endif

/* format of a symbol entry in the main table*/

# define NAMELEN	8		/*length of name in symbol table*/
struct symtab {
	char name[NAMELEN];		/*symbol name*/
	int  flags;			/*bit flags*/
	char *tlnk;			/*table link*/
	long  vl1;			/*symbol value*/
};

struct symtab *symtptr;
# define STESIZE	(sizeof *symtptr)	/*byte length of symbol table entry*/

/* flags for symbols*/
# define SYDF	0100000		/*defined*/
# define SYEQ	0040000		/*equated*/
# define SYGL	0020000		/*global - entry or external*/
# define SYER	0010000		/*equated register*/
# define SYXR	0004000		/*external reference*/
# define SYRA	0002000		/*DATA based relocatable*/
# define SYRO	0001000		/*TEXT based relocatable*/
# define SYBS	0000400		/*BSS based relocatable*/
# define SYIN	0000200		/*internal symbol -- opcode, directive or equ*/
# define SYPC	0000100		/*[vlh]equated using star '*' expression*/
# define SYRM	0000040		/*[vlh]register mask equate*/

/*flags for opcodes and directives*/
# define OPDR	0100000		/*0=>opcode, 1=>directive*/
# define OPFF	037			/*type of instruction (used as mask)*/

struct irts {
	char *irle;		/*ptr to last entry in chain*/
	char *irfe;		/*ptr to first entry in chain*/
};

long stlen;			/*length of symbol table*/

/*
 * intermediate text file
 * format of the intermediate text for one statement:
 *
 *	******************************************************
 *	*  it type = ITBS      *     # it entries            *  0
 *	******************************************************
 *	*       absolute line number (long)                  *
 *	******************************************************
 *	*  it type = ITSY      * instr length                *  1
 *	******************************************************
 *	*  symbol table pointer for stmt label (long)        *
 *	******************************************************
 *	*  it type = ITSY      *  instr mode length          *  2
 *	******************************************************
 *	*            opcode ptr (long)                       *
 *	******************************************************
 *	*  it type = ITCN      *  relocation base            *  3
 *	******************************************************
 *	*            location counter (pass 1)               *
 *	******************************************************
 *	*  it type             *  relocation flag            *  4 - oprnds
 *	******************************************************
 *	*               value (long)                         *
 *	******************************************************
 *                         .
 *
 *                         .
 *	******************************************************
 *	*  it type             *  relocation flag            *  n - oprnds
 *	******************************************************
 *	*               value (long)                         *
 *	******************************************************
 */

#define ITOP1	4	/*first it entry for operands*/

/*
 *	it type				meaning
 *		0			beginning of statement
 *		1			value is pointer to symbol table
 *		2			value is a constant
 *		3			value is a specal char
 *
 *  relocation flag for opcode it entry is operand length:
 *		'b' => byte
 *		'w' => word
 *		'l' => long
 */

struct it {
	char itty;		/*it type*/
	char itrl;		/*relocation flag or # it entries*/
	long  itop;
};

int mode;		/*operand mode (byte, word, long)*/
int modelen;	/*operand length per mode*/
#define BYTE	'b'
#define WORD	'w'
#define LONG	'l'

/* parameters that define the main table*/
#define SZMT 300		/*initial size of the main table */
						/*must be large enough to initialize*/
#define ICRSZMT 10		/*add to main table when run out*/
int cszmt;				/*current size of main table*/
char *bmte;				/*beginning of main table*/
char *emte;				/*end of main table*/

/* intermediate text types*/
#define ITBS	0		/*beginning of statement*/
#define ITSY	1		/*pointer to symbol table*/
#define ITCN	2		/*constant*/
#define ITSP	3		/*special*/
#define ITRM	4		/*[vlh]register mask!*/
#define ITPC	5		/*[vlh]pc relative argument*/

# define ITBSZ	256		/*size of the it buffer*/
int itbuf[ITBSZ];		/*it buffer*/

#define STMAX	200		/*size of intermediate text buffer*/
struct it stbuf[STMAX];		/*holds it for one statement*/
#define STBFSIZE	(sizeof stbuf[0])

char sbuf[512];		/*holds one block of source*/

/*initial reference table for symbols*/
# define SZIRT	128
char *sirt[SZIRT];
#define SIRTSIZE	(sizeof sirt[0])

/*initial reference table to opcodes*/
char *oirt[SZIRT];
#define OIRTSIZE	(sizeof oirt[0])

/*external symbol table*/
#define EXTSZ	512
char *extbl[EXTSZ];
int extindx;		/*index to external symbol table*/
char **pexti;			/*ptr to external symbol table*/

int absln;		/*absolute line number*/
int p2absln;	/*pass 2 line number*/
int fcflg;		/*0=>passed an item.  1=>first char*/
int fchr;		/*first char in term*/
int ifn;		/*source file descriptor*/
int *pitix;		/*ptr to it buffer*/
int itwc;		/*number of words in it buffer*/
struct it *pitw;	/*ptr to it buffer next entry*/
int itype;		/*type of item*/
long ival;		/*value of item*/
char *lblpt;		/*label pointer*/
char lbt[NAMELEN];	/*holds label name*/
char *lmte;		/*last entry in main table*/
long loctr;		/*location counter*/
long savelc[4];	/*save relocation counters for 3 bases*/
int nite;		/*number of entries in stbuf*/
struct it *pnite;
int lfn;		/*loader output file descriptor*/
char *opcpt;		/*pointer to opcode entry in main table*/
int p2flg;		/*0=>pass 1  1=>pass 2*/
char **pirt;		/*entry in initial reference table*/
int reloc;		/*relocation value returned by expression evaluator (expr)*/
int rlflg;		/*relocation value of current location counter*/
/*relocation values*/
# define ABS	0		/*absolute*/
# define DATA	1
# define TEXT	2
# define BSS	3
# define EXTRN	4		/*externally defined*/

#define EOLC	'\n'	/*end of line character*/
#define EOF		0		/*end of file indicator*/
#define NULL	0		/* [vlh] character null '\0' */
#define TRUE	1		/* [vlh] boolean values */
#define FALSE	0		/* [vlh] boolean values */

int format;
int sbuflen;		/*number of chars in sbuf*/
char *psbuf;		/*ptr into sbuf*/
int itfn;			/*it file number*/
char itfnc;			/*last char of it file name*/
int trbfn;			/*temp for text relocation bits*/
char trbfnc;		/*last char of text rb file*/
int dafn;			/*file for data stuff*/
char dafnc;			/*last char of data file*/
int drbfn;			/*file for data relocation bits*/
char drbfnc;		/*last char*/
int prtflg;			/*print output flag*/
int undflg;			/*make undefined symbols external flag*/

int starmul;		/* * is multiply operator*/

char *endptr, *addptr;
char *orgptr;
char *subptr, *addiptr, *addqptr, *subiptr, *subqptr;
char *cmpptr, *addaptr, *cmpaptr, *subaptr, *cmpmptr;
char *equptr;
char *andptr, *andiptr, *eorptr, *eoriptr, *orptr, *oriptr;
char *cmpiptr;
char *moveptr, *moveqptr;
char *exgptr;
char *evenptr;
char *jsrptr, *bsrptr, *nopptr;

char peekc;
int numcon[2], numsym[2], indir[2], immed[2], numreg[2];
int plevel;		/*parenthesis level counter*/
int opdix;		/*operand index counter*/
int p1inlen;	/*pass 1 instr length*/
int instrlen;	/*pass 2 bytes in current instruction*/

/* effective address mode bits*/
#define DDIR		000
#define ADIR		010
#define INDIRECT	020
#define INDINC		030
#define DECIND		040
#define INDDISP		050
#define INDINX		060
#define SADDR		070
#define LADDR		071
#define IMM			074

#define AREGLO		8
#define AREGHI		15

/* relocation bit definitions:*/
#define RBMASK		07		/*tells type of relocation*/
#define INSABS		7		/*first word of instr -- absolute*/
#define DABS		0		/*data word absolute*/
#define TRELOC		2		/* TEXT relocatable*/
#define DRELOC		1		/* DATA relocatable*/
#define BRELOC		3		/* BSS relocatable*/
#define EXTVAR		4		/* ref to external variable*/
#define LUPPER		5		/* upper word of long*/
#define EXTREL		6		/* external relative mode*/

/* ptrs to ins[] and rlbits[]*/
int *pins;
int *prlb;
int ins[5];					/*holds instruction words*/

#define PRTCHLEN 128
char prtchars[PRTCHLEN];	/*line buffer for putchar*/
char *prtchidx;				/*index for putchar*/

int extflg, extref;		/*external in expr*/

#define CCR	16
#define SR	17
#define USP	18
#define MOVECCR	042300
#define MOVESR	043300
#define SRMOVE	040300
#define MOVEUSP	047140

struct op {
	int ea;			/* effective address bits*/
	int len;		/* effective address length in bytes*/
	long con;		/*constant or reloc part of operand*/
	int drlc;		/*reloc of con*/
	int ext;		/*external variable #*/
	int idx;		/*index register if any*/
	int xmod;		/*mode of index reg*/
} opnd[2];

struct buf{
	int fildes;
	int nunused;
	char *xfree;
	char buff[512];
};

struct buf lbuf;
struct buf tbuf;
struct buf dabuf;
struct buf drbuf;

int nerror;		/*# of assembler errors*/
int in_err;		/*[vlh] don't generate instrlen err if already err state*/
int shortadr;	/*short addresses if set*/

#define CLRFOR	24
#define CLRVAL	041000

long itoffset;

#define LASTCHTFN	tfilname[11]
#define PC	22

int equflg;		/*doing an equate stmt*/

#define ANDI	01000
#define AND	0140000
#define ORI		0
#define OR		0100000
#define EORI	05000
#define EOR		0130000
#define MOVE	0

long lseek();
char *sbrk();
char *lemt();

int refpc;		/* * referenced in expr*/
#define SOH	1

/* Conditional Assembly variables and constants [vlh] */
#define LOW_CA	21		/* [vlh] */
#define HI_CA	30		/* [vlh] */

int ca_true;	/* true unless in a false CA*/
int ca;			/* depth of conditional assembly, none = 0*/
int ca_level;	/* at what CA depth did CA go false?*/

/* pass 1 global variables */
int numops;		/*number of operands*/
int inoffset;	/*[vlh]offset directive*/
int didorg;
int initflg;	/*initialize flag*/

/* defines */
#define igblk()		while(fchr==' ') fchr=gchr()
#define ckein()		((pitw >= pnite))

/* is it an alterable operand */
#define memalt(ap)	(memea(ap) && altea(ap))
#define dataalt(ap)	(dataea(ap) && altea(ap))
#define altea(ap)	((((ap)->ea&070)!=SADDR || ((ap)->ea&6)==0))

/* is it the specific type of operand */
#define memea(ap)	(((ap)->ea&070) >= INDIRECT)
#define dataea(ap)	(((ap)->ea&070) != ADIR)
#define pcea(ap)	((ap)->ea==072 || (ap)->ea==073)
#define ckdreg(ap)	((ap)->ea>=0 && (ap)->ea<AREGLO)
#define ckareg(ap)	((ap)->ea>=AREGLO && (ap)->ea<=AREGHI)

#ifndef VAX11
#define myfflush(pp)	fflush(pp)
#endif
