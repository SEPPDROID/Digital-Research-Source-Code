/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

/* format of a symbol entry in the main table*/

#define SEEKREL	1	/*relative seek flag*/

#define SYNAMLEN	8

struct symtab {
	char name[SYNAMLEN];		/*symbol name*/
	short flags;			/*bit flags*/
	long vl1;			/*symbol value*/
	char *tlnk;			/*table link*/
};

struct symtab *symptr =0;
#define	OSTSIZE	14
							/*entry length on object file without tlnk*/

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

struct {
	short loword;
	short hiword;
};

/* parameters that define the main table*/
#define SZMT 300			/*initial size of the main table*/
					/*must be large enough to initialize*/
#define ICRSZMT 100		/*add to main table when run out*/
int cszmt =0;
char *bmte =0;
char *emte =0;

/*initial reference table for externals*/
# define SZIRT	64
char *eirt[SZIRT] ={0};
char *saveirt[SZIRT] ={0};

/*initial reference table for globals*/
char *girt[SZIRT] ={0};
char *savgirt[SZIRT] ={0};

char **pirt =0;

char *lmte =0;
char *savlmte =0;

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

struct buf ibuf ={0};
struct buf tbuf ={0};
struct buf obuf ={0};
struct buf rbuf ={0};
struct buf rtbuf ={0};
struct buf rdbuf ={0};

int sflag =0;

long textbase =0;
long database =0;
long bssbase =0;
long textsize =0;
long datasize=0;
long bsssize =0;
long stacksize =0;

long textstart =0;
long datastart =0;
long bssstart =0;

char *ifilname =0;

#define NFILE	256		/*max # files we can process*/
char *fsymp[NFILE] ={0};
char **firstsym =0;

int extmatch =0;
int noload =0;

#define NLIB	16	/*max # libraries to process*/
int lbfictr[NLIB] ={0};
int *libfctr =0;
long lbfioff[NFILE] ={0};
long *libptr =0;

#define LIB1MAGIC	0177555
#define LIB2MAGIC	0177545
#define LIB1HDSIZE	16
#define LIB2HDSIZE	26
struct libhdr {
	char l1fname[8];
	long l1modti;
	char l1userid;
	char l1fimode;
	int  l1fsize;
};

struct lib2hdr {
	char l2fname[14];
	long l2modti;
	char l2userid;
	char l2gid;
	int  l2fimode;
	long l2fsize;
};

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
} lib3_hdr ={0};

#define LIB3MAGIC 0x3c21
#define LIB3HDSIZE	(sizeof lib3_hdr)

int libhdsize =0;
long libhd[20] ={0};

int undflg =0;
char dafnc =0;
int pass2 =0;
long stlen =0;
int Xflag =0;
int Dflag =0;
int Bflag =0;
int Zflag =0;


struct symtab *etextptr =0;
struct symtab *edataptr =0;
struct symtab *endptr =0;
char *lastdup =0;

char rtfnc =0;
char rdfnc =0;
int saverbits =0;

