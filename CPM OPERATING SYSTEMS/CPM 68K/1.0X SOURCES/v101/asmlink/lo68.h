/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include	"machine.h"
/* format of a symbol entry in the main table*/

#define SEEKREL	1	/*relative seek flag*/

#define SYNAMLEN	8

struct symtab {
	char name[SYNAMLEN];		/*symbol name*/
	short  flags;			/*bit flags*/
	long vl1;			/*symbol value*/
	char *tlnk;			/*table link*/
};

struct symtab *symptr;
#define	OSTSIZE	14		/*entry length on object file without tlnk*/

/* flags for symbols*/
# define SYDF	0100000		/*defined*/
# define SYEQ	0040000		/*equated*/
# define SYGL	0020000		/*global - entry or external*/
# define SYER	0010000		/*equated register*/
# define SYXR	0004000		/*external reference*/
# define SYDA	0002000		/*DATA based relocatable*/
# define SYTX	0001000		/*TEXT based relocatable*/
# define SYBS	0000400		/*BSS based relocatable*/

struct irts {
	char *irle;		/*ptr to last entry in chain*/
	char *irfe;		/*ptr to first entry in chain*/
};
#ifdef	VAX
struct {
	short loword;
	short hiword;
};
#endif
#ifdef	PDP11
struct {
	short loword;
	short hiword;
};
#endif
#ifdef	MC68000
struct	{
	short	hiword;
	short	loword;
};
#endif
/* parameters that define the main table*/
#define SZMT 300			/*initial size of the main table*/
					/*must be large enough to initialize*/
#define ICRSZMT 100			/*add to main table when run out*/
int cszmt;				/*current size of main table*/
char *bmte;				/*beginning of main table*/
char *emte;				/*end of main table*/

/*initial reference table for externals*/
# define SZIRT	64
char *eirt[SZIRT];
char *saveirt[SZIRT];

/*initial reference table for globals*/
char *girt[SZIRT];
char *savgirt[SZIRT];

char **pirt;

char *lmte;		/*last entry in main table*/
char *savlmte;

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
#define EXTREL		6		/* relative mode on external variable*/

#define BUFSIZE	512

struct buf{
	int fildes;
	int nunused;
	char *xfree;
	char buff[512];
};

struct buf ibuf;
struct buf tbuf;
struct buf obuf;
struct buf rbuf;
struct buf rtbuf;
struct buf rdbuf;

int sflag;		/*remove symbol table*/

long textbase;
long database;
long bssbase;
long textsize;
long datasize;
long bsssize;
long stacksize;

long textstart;
long datastart;
long bssstart;

char *ifilname;		/*points to name of current input file*/

#define NFILE	256		/*max # files we can process*/
char *fsymp[NFILE];		/*points to first symbol for each .o file*/
char **firstsym;		/*points to entry in fsymp*/

int extmatch;			/*matched an external in a library entry*/
int noload;			/*dont load this lib file flag*/

#define NLIB	16	/*max # libraries to process*/
int lbfictr[NLIB];	/*counts files loaded from one library*/
int *libfctr;		/*points to lbfictr*/
long lbfioff[NFILE];	/*each file offset in library*/
long *libptr;		/*points to lbfioff*/

#define LIB1MAGIC	0177555
#define LIB2MAGIC	0xffffff65
#define LIB1HDSIZE	16
#define LIB2HDSIZE	26
struct libhdr {
	char l1fname[8];
	long l1modti;
	char l1userid;
	char l1fimode;
	short l1fsize;
};

struct lib2hdr {
	char l2fname[14];
	long l2modti;
	char l2userid;
	char l2gid;
	short  l2fimode;
	long l2fsize;
	short l2junk;
} *lib2hd;

#define	ARMAG	"!<arch>\n"
#define	SARMAG	8

#define	ARFMAG	"`\n"

struct ar_hdr {
	char	ar_name[16];
	char	ar_date[12];
	char	ar_uid[6];
	char	ar_gid[6];
	char	ar_mode[8];
	char	ar_size[10];
	char	ar_fmag[2];
} lib3_hdr;

#define LIB3MAGIC 0x3c21
#define LIB3HDSIZE	(sizeof lib3_hdr)

int libhdsize;
long  libhd[20];		/*length is max lib head size*/

int undflg;
char dafnc;
int pass2;
long stlen;
int Xflag;
int Dflag;
int Bflag;
int Zflag;


struct symtab *etextptr;
struct symtab *edataptr;
struct symtab *endptr;
char *lastdup;

char rtfnc;
char rdfnc;
int saverbits;
