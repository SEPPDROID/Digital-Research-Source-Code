/* note: check accuracy of pi_180 */
/* note: eval "0 should produce 0 */
#define BDSC   0	/* change to 1 for 8-bit BDS C compiler */

#if !BDSC
#include <stdio.h>
#include <ctype.h>
#endif

/*****************************************************************/
/*                                                               */
/*               Terminal Characteristics                        */
/*                                                               */
/*****************************************************************/
#define TRUE   1
#define FALSE  !TRUE
#define TEST   FALSE
#define GENBIN FALSE		/* set to TRUE in lgen.sub */

#define OTRONA FALSE
#define Z19    FALSE
#define IBMPC  FALSE		/* Select IBM PC Graphics */
#define CIO    FALSE		/* TRUE if using C's I/O */
#define DIRIO  TRUE		/* direct PC character I/O */
#define ASCII  FALSE		/* Use ASCII Graphics */

#define P80    TRUE		/* TRUE if 8-bit target */
#if Z19 || OTRONA || IBMPC
#define SCRROWS 24
#define SCRCOLS 79		/* lines on the screen */
#define TWOSCR  FALSE
#endif
#if DIRIO
#define SCRROWS 25
#define SCRCOLS 80
#define TWOSCR  TRUE		/* TRUE if two screen support */
#endif
#define SCRSPLT 21		/* default split row */
#define SCRHCOLS 40		/* half column mode width */

#define WINDOWS 2		/* max number of windows */
#define TABSIZE 4		/* tab stops */
#define XYLEN  4		/* length of cursor position seq */
/*****************************************************************/
/*                                                               */
/*               DEFINE's Taken From BDCSIO.H                    */
/*                                                               */
/*****************************************************************/
#define SECSIZ 128	/* sector size */
#define NSECTS 8	/* number to read/write */
#define BUFSIZ (NSECTS * SECSIZ + 6)
/*****************************************************************/
/*                                                               */
/*                Control Characters                             */
/*                                                               */
/*****************************************************************/
#define CTLA      1		/* beginning of line */
#define CTLB      2		/* one space back */
#define CTLC      3		/* end edit */
#define CTLD      4		/* erase char at cursor */
#define CTLE      5		/* to end of current line */
#define CTLF      6		/* forward one space */
#define CTLG      7		/* immediate Throw "TOPLEVEL */
#define CTLH      8		/* back one character */
#define CTLI      9		/* tab ('\t') */
#define CTLJ     10		/* line feed */
#define CTLK     11		/* erase beyond cursor */
#define CTLL     12		/* current line to center */
#define CTLM     13		/* carriage return */
#define CTLN     14		/* next page */
#define CTLO     15		/* open a line */
#define CTLP     16		/* previous page */
#define CTLQ     17		/* insert \ */
#define CLTS     19		/* Graphics mode */
#define CTLS     19		/* Split Screen Mode */
#define CTLT     20		/* Text Mode */
#define CTLU     21		/* same as CTLF */
#define CTLV     22		/* make last line first line */
#define CTLW     23		/* wait */
#define CTLX     24		/* (unused) */
#define CTLY     25		/* yank kill buffer */
#define CTLZ     26		/* PAUSE */
#define ESC      27		/* escape key */
#define DEL     127		/* delete character left */
/* characters that delete character queue outside editor */
char ctlscr [] = {CTLL, CTLS, CTLT, CTLW, CTLZ, 0};
/*****************************************************************/
/*                                                               */
/*                  IBM PC Function Keys                         */
/*                                                               */
/*****************************************************************/
#if DIRIO
char keytab [] = {
	59, CTLC, 0, 0,		/* F1 = 59 */
	60, CTLG, 0, 0,		/* F2 = 60 */
	61, CTLW, 0, 0,		/* F3 = 61 */
	62, CTLZ, 0, 0,		/* F4 = 62 */
	63, CTLK, 0, 0,		/* F5 = 63 */
	64, CTLY, 0, 0,		/* F6 = 64 */
	65, CTLL, 0, 0,		/* F7 = 65 */
	66, CTLX, 0, 0,		/* F8 = 66 (unused) */
	67, CTLA, 0, 0,		/* F9 = 67 */
	68, CTLE, 0, 0,		/* F10 = 68 */
	71, ESC, '<', 0,	/* HOME = 71 */
	72, CTLP, 0, 0,		/* UP ARROW = 72 */
	73, ESC, 'V', 0,	/* PG UP = 73 */
	75, CTLB, 0, 0,		/* LEFT ARROW = 75 */
	77, CTLF, 0, 0,		/* RIGHT ARROW = 77 */
	79, ESC, '>', 0,	/* END = 79 */
	80, CTLN, 0, 0,		/* DOWN ARROW = 80 */
	81, CTLV, 0, 0,		/* PG DN = 81 */
	82, CTLO, 0, 0,		/* INS = 82 */
	83, CTLD, 0, 0,		/* DEL = 83 */
	0};
#endif
/*****************************************************************/
/*                                                               */
/*               Generally Useful DEFINE's                       */
/*                                                               */
/*****************************************************************/
#define LODEDIT  0		/* editor(LODEDIT) loads overlay */
#define SCREDIT  1		/* editor(SCREDIT) to screen edit */
#define LINEDIT  2		/* editor(LINEDIT) to line edit */
#define COPYEDIT 3		/* editor(COPYEDIT) compacts buffer */
#define EDTMIN   512		/* minimum editor buffer size */
#define EDTSTK   384		/* minimum editor stack size */
#if BDSC
#define ASPACE     0		/* Alloc space at end of ENDEXT */
#define EDTSIZE  BUFSIZ		/* interp stack: EDTSTK+EDTSIZE */
#endif
#if !BDSC
#define ASPACE   512		/* buffer space for load/save */
#define EDTSIZE  8192 		/* interp stack: EDTSTK+EDTSIZE */
#endif
#define INIDSIZE 8192		/* must be 8k */
#define INIDSTK   128		/* stack size for initd */
#define CQSIZE   64		/* character queue size (^2) */

#define ENVSIZE  3		/* number of elt*'s to hold env */
#define INSIZE   134		/* size of input buffer */
#define KILLSIZE INSIZE		/* size of kill buffer */
#define WDBSIZE  INSIZE		/* word construction buffer */
#define NUMBSIZE 32		/* number buffer size (23 enough) */
#define HASHSIZE 128		/* (power of two) size of hash table */
#define TAGS     TRUE		/* using extra byte for collect */
#define lnum     double		/* logo number */
#define local    register	/* register variable */
#define CEXP	'e'		/* C exponent indicator in fp numbers */
#define LEXP	'e'		/* Logo exponent indicator */
#define PI	3.1415926535897932
#define PI_180	57.2957795130823	/* 180 / pi */
#define TRIGPR  1.0E-15		/* all trig functions below this -> 0 */
/*****************************************************************/
/*                                                               */
/*       DEFINE's and Declarations for LOGO List Structure       */
/*                                                               */
/*****************************************************************/
#define NULL     0	/* null element of list */
#define LISTSIZE 3000	/* number of list elements */
#define RESERVE   200   /* minimun cell count before OUT OF SPACE */
#define TOOCLOSE  100   /* minimun remaining stack at "testkb" */
#define STKCHECK   30	/* number of recursive calls before stack check */

int env   [ENVSIZE];	/* holds environment, copied to envelt's */
int obenv [ENVSIZE];	/* holds outer block environment */
int mtoenv[ENVSIZE];	/* holds env for lgets */
int insenv[ENVSIZE];	/* holds env for char insert */

union ltod_tag
	{
	long lmant[2];
	double dmant;
	} ltod;
#if TAGS
#define CELLSIZE 5	/* number of char's per cell */
#define COLLMASK 0x80	/* collection mask over tag field */
union listelt {
	struct cell {
		char ctag;
		union listelt *carf;
		union listelt *cdrf;
		}cell1;
	struct pcell {
		char ptag;
		union listelt *pcdr;
		int (*pfunc)();
		};
	struct ccell {
		char cctag;
		int collf;
		union listelt *ccdr;
		};
	struct ecell {
		char etag;
		union listelt *ecar;
		int envelt;
		};
	struct scell {
		char stag;
		union listelt *scar;
		int svalue;
		};
	struct lcell {
		char ltag;
		long lvalue;
		};
	struct fcell {
		char ftag;
		float fvalue;
		};
	struct dcell {
		char dtag;
		long dvalue;
		};
	char pnamef[CELLSIZE];
	};
#endif

#if !TAGS
#define CELLSIZE 4	/* number of characters in a cell */
#define COLLMASK 0x8000	/* collection mask over car field */
union listelt {
	struct cell {
		union listelt *carf;
		union listelt *cdrf;
		}cell1;
	struct pcell {
		union listelt *pcar;
		int (*pfunc)();
		};
	struct ccell {
		int collf;
		union listelt *ccdr;
		};
	struct ecell {
		union listelt *ecar;
		int envelt;
		};
	/* note: cells for short, long, float and double */
	/* depend upon representation:  a collect bit is */
	/* required within the number (prefer high bit)  */
	char pnamef[CELLSIZE];
	};
#endif
#define elt union listelt
/*****************************************************************/
/*                                                               */
/*               Procedure Declarations                          */
/*                                                               */
/*****************************************************************/
elt *getprop();
elt *stmove();

#define linefeed LINEFEED
#define lcenter  LCENTER
/*****************************************************************/
/*                                                               */
/*               Variable  Declarations                          */
/*                                                               */
/*****************************************************************/
elt *freelist;
elt *listaddr;			/* next to allocate */
elt *nxtelist;			/* preallocated cell for elist */
int listbase;			/* lowest allocated + 1 */
int exspace;			/* TRUE if working in reserve memory */
int initial;			/* TRUE during initialization */
int debcoll;			/* TRUE if collecting on each getelt */
int traceall;			/* TRUE if all functions traced */
int stkcalls;			/* keeps track of stack check calls */
int spinseed;			/* seed for "spin" */
unsigned memmax;		/* note: temp for testing, make it LISTSIZE */
unsigned listcount;		/* number of nodes remaining */

elt *next_wl;			/* next word list elt to scan */
elt *act_wl;			/* active word list elt */
int index_wl;			/* current index to word list */
elt *next_pl;			/* next property list to process */
elt *act_att;			/* active attribute */
elt *act_val;			/* active value */
/*****************************************************************/
/*                                                               */
/*                   Editor Declarations                         */
/*                                                               */
/*****************************************************************/
#if !CIO
char     *dsknext;		/* next edtmem char to read */
int      dskcount;		/* number of characters in edtmem */
int      incount;		/* counts size of input in disk read */
int      eofset;		/* TRUE when EOF on disk */
#endif
int      errproc;		/* pointer to last proc with error */
int      editline;		/* entry line number for editor */
elt      *errline;		/* pointer to line in error */
int      editcol;		/* entry column number for editor */
elt      *errcol;		/* pointer to item in error */
char     *inptr;		/* pointer to input in lgets */
char     *enptr;		/* pointer to end of line in lgets */
char     *savptr;		/* original enptr pointer in lgets */
char     *oldnext;		/* pointer to end in sc_end */
char     *newnext;		/* move pointer in sc_end */
char     *edtptr;		/* pointer to text after edit with ^g */
char     nulchr;		/* set to '\0' for lgets */
int      incnt;			/* buffer character count in lgets */
int      lncnt;			/* length of line in lgets */
int      editing;		/* TRUE if screen editing */
int      linedit;		/* TRUE if line editing */
int	 loading;		/* TRUE if reading from load cmd. */
/*****************************************************************/
/*                                                               */
/*      DEFINE's and Declarations for LOGO Atom Scanner          */
/*                                                               */
/*****************************************************************/
#define  EOL        0		/* Scanner End of Line */
#define  CONSTANT   1		/* Scanner, LOGO Constant "xxx */
#define  VALUEREF   2		/* Scanner, LOGO Value reference :xxx */
#define  NUMBER     3		/* Scanner, LOGO Number */
#define  SYMBOL     4		/* Scanner, LOGO Unadorned Symbol xxx */
#define  SNUMBER    5		/* Short Number (16b) */
#define  LNUMBER    6		/* Long Number  (32b) */
#define  FNUMBER    7		/* Float Number (32b) */
#define  DNUMBER    8		/* Double Prec  (64b) */
#define  RESERVED   8           /* Number of Reserved Pointers */

#define  NOSPACE    1		/* error EXIT return */
#define  STUNDER    2		/* interpreter stack underflow EXIT */
#define  SYSBUG     3		/* unrecoverable system bug */

#define	 PROCHEAD   0		/* print proc. heads in makeASCII */
#define  PROCALL    1		/* print entire proc. in makeASCII */
#define  VARS	    2		/* print variables in makeASCII    */
#define  PROPS      3		/* print out properties		*/
#define  PACKS      4		/* print of packages		*/


#define  EOLCHR   '!'		/* end of line overflow character */

#define  UNARY_STR "\177- "     /* non-user definable symbol-linit2  */
#define  FULLSCR  0		/* scrmode is full graphics screen */
#define  SPLITSCR 1		/* scrmode is split screen */
#define  DUALSCR  2		/* scrmode is two screens */
#define  TEXTSCR  3		/* scrmode is full text screen */
int      scrmode;		/* FULL, SPLIT, DUAL, TEXT */
int      twoscreen;		/* TRUE if two physical screens */
int      textset;		/* used to toggle ctl-t */

char     charq[CQSIZE];		/* character queue */
int      ntq, ntp, ntg;		/* number queued, next put, next get */
char     input[INSIZE];		/* input buffer */
char     killbuf[KILLSIZE];	/* kill buffer */
char     numbuff[NUMBSIZE];	/* number construction buffer */
char     wdbuff[WDBSIZE];	/* word construction buffer */
int      wdbfill;		/* TRUE when filling wdbuff */
elt      *hashtab[HASHSIZE];	/* atom hash table */
int      hashcode;		/* current hash code */
char     *accum;
char     *nxtacc;
char     nxtchr;
char     lstchr;
char     kbrdy;			/* lookahead character for input */
unsigned acclen;
unsigned acctyp;
lnum     accval;
int      accsign;
int	 unary;			/* Flags SYMBOL as unary in SCAN    */
int      firstch;		/* first char of line, for comments */
int      column;		/* column number, 1 - width  */
int      row;			/* row number,    1 - height */
int      splitrow;		/* row to begin splitscreen */
int      lineno;		/* current line number in lgets */
int      entcol;                /* column number on entry to lgets */
int      maxrow;		/* max row in lgets */
int      endrow;		/* used in displaying screen tail */
int      savrow;		/* used to compute maxrow */
int      width;
int      height;
int      lmarg;			/* left margin */
int      tmarg;			/* top margin */
int      txtscr;		/* TRUE if text scroll */
int      lastwh;		/* last value of 'where' */
int      spaces;		/* counts spaces at start of line */
int      eos;			/* TRUE if end of statement */
int      scrskip;		/* used during manual scroll */

#if DIRIO
int     absrow;			/* absolute screen row */
int     abscol;			/* absolute screen column */
#endif

int	cwindow;		/* current window number */
int	nwindow;		/* next window to allocate */
int	rowv	[WINDOWS];	/* row value */
int	columnv	[WINDOWS];	/* column value */
int	tmargv	[WINDOWS];	/* tmarg value */
int	lmargv	[WINDOWS];	/* lmarg value */
int	heightv	[WINDOWS];	/* height value */
int	widthv	[WINDOWS];	/* width value */
#if !DIRIO
char   *scrbufv [WINDOWS];	/* scroll buffer base */
char   *scrbuf;			/* current scroll buffer */
char   *nscrbuf;		/* initial edtmem address */
#endif
/*****************************************************************/
/*                                                               */
/*           DEFINE's and Declarations for Interpreter           */
/*                                                               */
/*****************************************************************/
int editon;		/* TRUE when in the editor */
int defining;		/* TRUE when defining a function */
int formatting;		/* TRUE if formatting on */
int fmtcnt;		/* item counter for formats */
int copyon;		/* TRUE when also listing on printer */
int pagemode;		/* line count > 0 if in page mode */
int primitives;		/* TRUE if help for primitives */
int last_test;		/* value of last TEST */

elt *parenloc;		/* used to mark ( func */
elt *format;		/* indent format during i/o */
elt *lstform;		/* last format elt in list */
elt *primlist;		/* list of primitives when primitives enabled */
elt *rlist;		/* pointer to reserved pointer list */
elt *trlist;		/* list of procedures being traced */
elt *proclist;		/* ordered list of procedures */
elt *glob_pack;		/* global package name from Load command */
elt *po_list;		/* porefs, potl list */

int pen_color;		/* current pen color */
elt *pen_state;		/* = pd,  pe, pu, pr, ptr */
elt *pd_ptr;		/* pointer to PENDOWN */
elt *pe_ptr;		/* pointer to PENERASE */
elt *pu_ptr;		/* pointer to PENUP */
elt *pr_ptr;		/* pointer to PENREVERSE */

elt *true_ptr;		/* pointer to TRUE */
elt *false_ptr;		/* pointer to FALSE */
elt *def_ptr;           /* Function Definition Attribute */
elt *part_ptr;		/* ELIST Partial [ a . fn ] */
elt *func_ptr;		/* ELIST Function [q p a . fn] */
elt *catch_ptr;		/* ELIST Catch */
elt *pau_ptr;		/* ELIST Pause */
elt *apval_ptr;		/* APVAL symbol */
elt *fmt_ptr;		/* FMT pointer */
elt *spc_ptr;		/* SPC pointer */
elt *rem_ptr;		/* REM pointer */
elt *enl_ptr;		/* ENL pointer */
elt *bury_ptr;		/* BUR pointer */
elt *pack_ptr;		/* PAK pointer */
elt *pkg_ptr;		/* PKG pointer */
elt *prim_ptr;		/* PRM pointer */

elt *top_ptr;		/* TOPLEVEL pointer */
elt *emp_ptr;		/* "an empty word" pointer */
elt *lpar_ptr;		/* ( pointer */
elt *rpar_ptr;		/* ) pointer */
elt *err_ptr;		/* ERROR pointer */
elt *equ_ptr;		/* = pointer */
elt *lss_ptr;		/* < pointer */
elt *gtr_ptr;		/* > pointer */
elt *add_ptr;		/* + pointer */
elt *unary_ptr;		/* - (unary) pointer */
elt *sub_ptr;		/* - (binary) pointer */
elt *mul_ptr;		/* * pointer */
elt *pow_ptr;		/* ^ pointer */
elt *div_ptr;		/* / pointer */
elt *lab_ptr;		/* label-pointer */
elt *redef_ptr;		/* REDEFP pointer */
elt *noack_ptr;		/* NOACK pointer */
elt *erract_ptr;	/* ERRACT pointer */

elt *label;		/* used in go_func to search for label */
elt *lasterr;		/* current error message or NULL */
elt *parenloc;		/* keeps track of ( in ( prim ... */
elt *cprim;		/* current active primitive, or NULL */
elt *lfunc;		/* last function or primitive */
elt *elist;		/* expression list for interp */
elt *alist;		/* association list */
elt *plist;		/* current statement */
elt *qlist;		/* current verb */
elt *qval;		/* often car(qlist) */
elt *nxtfn;		/* pointer to scan elist for functions */
elt *fb;		/* active function body */
elt *wdbptr;		/* active printname in word buffer */
elt *retval;		/* return value ptr, or NOVALUE */


#define NOVALUE    1	/* No Value Returned */
#define TOOBIG	   2    /* Number too big 	   */
#define ISPRIM     6    /* (symbol) is a Primitive */
#define NOLABEL    7	/* Can't find label (symbol) */
#define UNDEF      9    /* (symbol) is Undefined */
#define BADDISK   11	/* I'm having trouble with the disk */
#define FULLDISK  12	/* Disk full */
#define DIVZERO   13	/* Can't divide by zero */
#define FILEX     15	/* File already exists */
#define NOFILE    17	/* File not found */
#define NOCAT     21    /* Can't Find Catch for (Symbol) */
#define OOSPACE   23    /* Out of space */
#define NOTBOOL   25    /* (symbol) is not TRUE or FALSE */
#define TOOFEW    29	/* Not Enough Inputs to (Procedure) */
#define TOOMANY   30	/* Too many inputs to (Procedure) */
#define TSHORT    32	/* Too few items in (list or char) */
#define TURTOB    34	/* Turtle out of bounds */
#define NOPROC    35    /* I Don't Know How to (symbol) */
#define NOVAL     36	/* (symbol) has no value */
#define RLPAR     37	/* ) without ( */
#define WHYVAL    38    /* I Don't Know What to Do With (symbol) */
#define BADINP    41    /* (procedure) Doesn't Like (sym) as Input */
#define NOOUT     42    /* (procedure) Didn't Output */
#define NOTIMP    43    /* primitive not implemented */
#define SYSERR    44    /* !!! LOGO SYSTEM BUG !!! */
#define WDBOVER   45	/* word is too big */
#define EDTFULL   46	/* My edit buffer is full */
#define IFLIST    47	/* If wants [ ]'s around instructions */
#define INITD     48    /* initd error (varies) */
#define BADPARM	  49    /* bad parameter in "to" definition	 */
#define NOLOAD	  50    /* Illegal function while loading */
#define EDTLOAD	  51    /* Can't load from the editor     */
#define ROD       53    /* The disk is write protected */
#define ROF       54    /* The file is write protected */
#define SELERR    55    /* I can't find the disk drive */
/*****************************************************************/
/*                                                               */
/*               DEFINE's for Interpreter Descriptors            */
/*                                                               */
/*****************************************************************/
#define DESCQL     1		/* qlist in FUN, CAT, PAU */
#define DESCPL     2		/* plist in FUN, CAT, PAU */
#define DESCNM     3		/* name in FUN, CAT */
#define DESCAL     4		/* alist in FUN */
#define FUNSIZE    ENVSIZE + 5	/* size of FUNction desc */
#define CATSIZE    ENVSIZE + 4	/* size of CATch desc */
#define PAUSIZE    ENVSIZE + 2  /* size of PAUse desc */
#define REPSIZE    ENVSIZE + 3  /* size of REPeat desc */

#define CONSOLE    0		/* output path is console */
#define EDITOR     1            /* output path is edit buffer */
#define DISK       2		/* output path is disk */
int     inpath;		/* current input path: CONSOLE EDITOR DISK */
int     outpath;	/* current output path: CONSOLE EDITOR */
#if CIO
int	save_fid;	/* used in binary save/load */
int	io_fid;		/* standard i/o file id */
char    iobuf[BUFSIZ];	/* standard i/o buffer */
#endif
/*****************************************************************/
/*                                                               */
/*             Low-Level BDOS Interface Parameters               */
/*                                                               */
/*****************************************************************/
struct	fcbtab {		/****************************/
	char	drive;		/* Disk drive field	    */
	char	fname[8];	/* File name		    */
	char	ftype[3];	/* File type		    */
	char	extent;		/* Current extent number    */
	char	s1,s2;		/* "system reserved"	    */
	char	reccnt;		/* Record counter	    */
	char	resvd[16];	/* More "system reserved"   */
	char    currec;		/* current record           */
	int     record;		/* random record field      */
	char	rrover;		/* random record overflow   */
	};			/****************************/
struct fcbtab fcb;		/* standard fcb in lfunc7   */
struct fcbtab fcb1;		/* auxiliary fcb in lfunc7  */
#if !CIO			/*                        */
struct fcbtab dfcb;		/* disk fcb for load/save */
#endif				/*                        */
				/****************************/
#define DEFDMA   0x0080		/* Default DMA Address      */
#define	EXIT	 0		/* Exit to BDOS		    */
#define	CONOUT	 2		/* Direct console output    */
#define RESDSK  13		/* Reset disk system        */
#define SELDSK  14		/* Select Disk              */
#define	OPEN	15		/* OPEN a disk file	    */
#define	CLOSE	16		/* Close a disk file	    */
#define FSEARCH 17		/* Search for First         */
#define NSEARCH 18		/* Search for Next          */
#define	DELETE	19		/* Delete a disk file	    */
#define SREAD   20		/* Sequential Read          */
#define SWRITE  21		/* Sequential Write         */
#define	CREATE	22		/* Create a disk file	    */
#define RENAME  23		/* Rename a File            */
#define CURDSK  25		/* Current disk number      */
#define	SETDMA	26		/* Set DMA address	    */
#define ALLOC   27		/* Get Alloc Vector Addr    */
#define DISKPAR 31		/* Get Disk Parameter Addr  */
#define	RREAD	33		/* Read Random record	    */
#define	RWRITE	34		/* Write Random record	    */
#define FILSIZ	35		/* Compute File Size	    */
#define NEWDSK  37		/* reset disk drive */
#define SETMSC	44		/* Set Multi-Sector Count   */
#define	SETVEC	61		/* Set exception vector	    */
				/****************************/
/*****************************************************************/
/*								 */
/*	Logo optimization definitions				 */
/*								 */
/*****************************************************************/

#include <logopt.h>

/*****************************************************************/
/*                                                               */
/*             LOGO List Structure, must be last                 */
/*                                                               */
/*****************************************************************/
#if P80
char *edtmem;		/* initmem puts this below stack */
elt  *listmem;		/* initmem puts this below edtmem */
#endif
#if !P80
char edtmem[EDTSIZE];	/* edit buffer */
elt  listmem[LISTSIZE];	/* list storage must be last */
#endif
