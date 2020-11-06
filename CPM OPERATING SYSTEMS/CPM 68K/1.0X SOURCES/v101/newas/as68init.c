/*
	Copyright 1981
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "machine.h"
#include "ctype.h"

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

#ifdef VAX
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
# define STESIZE (sizeof *symtptr) /*byte length of symbol table entry*/

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
# define OPFF	037		/*type of instruction (used as mask)*/

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
 *			   .
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

int mode=0;		/*operand mode (byte, word, long)*/
int modelen=0;		/*operand length per mode*/
#define BYTE	'b'
#define WORD	'w'
#define LONG	'l'

/* parameters that define the main table*/
#define SZMT 300			/*initial size of the main table */
					/*must be large enough to initialize*/
#ifdef PDP11
#define ICRSZMT 10		/*add to main table when run out*/
#else
#define ICRSZMT	50		/*add to main table when run out*/
#endif
int cszmt=0;				/*current size of main table*/
char *bmte=0;				/*beginning of main table*/
char *emte=0;				/*end of main table*/

/* intermediate text types*/
#define ITBS	0		/*beginning of statement*/
#define ITSY	1		/*pointer to symbol table*/
#define ITCN	2		/*constant*/
#define ITSP	3		/*special*/
#define ITRM	4		/*[vlh]register mask!*/
#define ITPC	5		/*[vlh]pc relative argument*/

# define ITBSZ	256		/*size of the it buffer*/
int itbuf[ITBSZ]={0};		/*it buffer*/

#define STMAX	200		/*size of intermediate text buffer*/
struct it stbuf[STMAX]={0};	/*holds it for one statement*/

char sbuf[512]={0};		/*holds one block of source*/

/*initial reference table for symbols*/
# define SZIRT	128
char *sirt[SZIRT]={0};

/*initial reference table to opcodes*/
char *oirt[SZIRT]={0};

/*external symbol table*/
#define EXTSZ	512
char *extbl[EXTSZ]={0};
int extindx=0;			/*index to external symbol table*/
char **pexti=0;			/*ptr to external symbol table*/

int absln=0;		/*absolute line number*/
int p2absln=0;	/*pass 2 line number*/
int fcflg=0;		/*0=>passed an item.  1=>first char*/
int fchr=0;		/*first char in term*/
int ifn=0;		/*source file descriptor*/
int *pitix=0;		/*ptr to it buffer*/
int itwc=0;		/*number of words in it buffer*/
struct it *pitw=0;	/*ptr to it buffer next entry*/
int itype=0;		/*type of item*/
long ival=0;		/*value of item*/
char *lblpt=0;		/*label pointer*/
char lbt[NAMELEN]={0};	/*holds label name*/
char *lmte=0;		/*last entry in main table*/
long loctr=0;		/*location counter*/
long savelc[4]={0};	/*save relocation counters for 3 bases*/
int nite=0;		/*number of entries in stbuf*/
struct it *pnite=0;
int lfn=0;		/*loader output file descriptor*/
char *opcpt=0;		/*pointer to opcode entry in main table*/
int p2flg=0;		/*0=>pass 1  1=>pass 2*/
char **pirt=0;		/*entry in initial reference table*/
int reloc=0;		/*relocation value returned by expression evaluator (expr)*/
int rlflg=0;		/*relocation value of current location counter*/
/*relocation values*/
# define ABS	0		/*absolute*/
# define DATA	1
# define TEXT	2
# define BSS	3
# define EXTRN	4		/*externally defined*/

#define EOLC	'\n'		/*end of line character*/
#define EOF	0		/*end of file indicator*/
#define NULL	0		/* [vlh] character null '\0' */
#define TRUE	1		/* [vlh] boolean values */
#define FALSE	0		/* [vlh] boolean values */

int format=0;
int sbuflen=0;		/*number of chars in sbuf*/
char *psbuf=0;		/*ptr into sbuf*/
int itfn=0;			/*it file number*/
char itfnc=0;			/*last char of it file name*/
int trbfn=0;			/*temp for text relocation bits*/
char trbfnc=0;		/*last char of text rb file*/
int dafn=0;			/*file for data stuff*/
char dafnc=0;			/*last char of data file*/
int drbfn=0;			/*file for data relocation bits*/
char drbfnc=0;		/*last char*/
int prtflg=0;			/*print output flag*/
int undflg=0;			/*make undefined symbols external flag*/

int starmul=0;		/* * is multiply operator*/

char *endptr=0, *addptr=0;
char *orgptr=0;
char *subptr=0, *addiptr=0, *addqptr=0, *subiptr=0, *subqptr=0;
char *cmpptr=0, *addaptr=0, *cmpaptr=0, *subaptr=0, *cmpmptr=0;
char *equptr=0;
char *andptr=0, *andiptr=0, *eorptr=0,  *eoriptr=0, *orptr=0, *oriptr=0;
char *cmpiptr=0;
char *moveptr=0, *moveqptr=0;
char *exgptr=0;
char *evenptr=0;
char *jsrptr=0, *bsrptr=0, *nopptr=0;

char peekc=0;
int numcon[2]=0, numsym[2]=0, indir[2]=0, immed[2]=0, numreg[2]=0;
int plevel=0;		/*parenthesis level counter*/
int opdix=0;		/*operand index counter*/
int p1inlen=0;	/*pass 1 instr length*/
int instrlen=0;	/*pass 2 bytes in current instruction*/

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
#define IMM		074

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
int *pins=0;
int *prlb=0;
int ins[5]={0};					/*holds instruction words*/

#define PRTCHLEN 128
char prtchars[PRTCHLEN]={0};	/*line buffer for putchar*/
char *prtchidx=0;		/*index for putchar*/

int extflg=0, extref=0;		/*external in expr*/

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
} opnd[2]={0};
#define	OPSTLEN	10
#define	TREELEN	20
struct buf{
	int fildes;
	int nunused;
	char *xfree;
	char buff[512];
};

struct buf lbuf={0};
struct buf tbuf={0};
struct buf dabuf={0};
struct buf drbuf={0};

int nerror=0;		/*# of assembler errors*/
int in_err=0;		/*[vlh] don't generate instrlen err if already err state*/
int shortadr=0;	/*short addresses if set*/

#define CLRFOR	24
#define CLRVAL	041000

long itoffset=0;

#define LASTCHTFN	tfilname[11]
#define PC	22

int equflg=0;		/*doing an equate stmt*/

#define ANDI	01000
#define AND	0140000
#define ORI	0
#define OR	0100000
#define EORI	05000
#define EOR	0130000
#define MOVE	0

long lseek();
char *sbrk();
char *lemt();

int refpc=0;		/* * referenced in expr*/
#define SOH	1

/* Conditional Assembly variables and constants [vlh] */
#define LOW_CA	21		/* [vlh] */
#define HI_CA	30		/* [vlh] */

int ca_true=0;	/* true unless in a false CA*/
int ca=0;	/* depth of conditional assembly, none = 0*/
int ca_level=0;	/* at what CA depth did CA go false?*/

/* pass 1 global variables */
int numops=0;		/*number of operands*/
int inoffset=0;	/*[vlh]offset directive*/
int didorg=0;
int initflg=0;	/*initialize flag*/

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
/*
 *	DRI Listing Hacks:
 */
char	*sfname=0;		/* -> Source filename	*/
int	xline=0;		/* Current line number  */
int	xpage=0;		/* Current page number  */
/*
 *	More Initializations:
 */
char	*tfilname[80] = {0};	/* Temp filename area	*/
char	*initfnam[80] = {0};	/* Init filename area	*/
struct	it	exitm={0};
int	prcnt=0;
int	rval=0;
int	lpflg=0;
int	lastopr=0;
int	rlbits[5]={0};
int	pline=0;
int	prsp=0;
int	amode=0;
long	stlen=0;
int	udfct=0;
int	errno=0;
int	nitleft=0;
int	hibytflg[4]={0};
int	hibytw[4]={0};
struct	it *piop=0;
struct	it *pitr=0;
struct	it  opstk[OPSTLEN]={0};
struct	it  tree[TREELEN]={0};
int	chmvq=0;
int	explmod=0;
int	ftudp=0;
int	iop=0;
int	itr=0;
char	ldfn[40]={0};
int	opcval=0;
int	poslab=0;
int	symcon=0;
char	*tfilptr=0;
int	tlab1=0;
#define HDSIZE	(sizeof couthd)	/**.o file header size*/
struct hdr {
	short ch_magic;		/*c.out magic number 060016 = $600E*/
	long ch_tsize;		/*text size*/
	long ch_dsize;		/*data size*/
	long ch_bsize;		/*bss size*/
	long ch_ssize;		/*symbol table size*/
	long ch_stksize;	/*stack size*/
	long ch_entry;		/*entry point*/
	short ch_rlbflg;	/*relocation bits suppressed flag*/
} couthd={0};

#define MAGIC	0x601a	/* bra .+26 instruction*/
