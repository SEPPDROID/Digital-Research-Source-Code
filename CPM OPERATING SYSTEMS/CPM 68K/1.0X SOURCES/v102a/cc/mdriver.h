/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121

	@(#)mdriver.h	1.7	12/28/83
*/

#ifdef DECC
#	define VMS
#	define unlink delete
#endif

#ifdef VMS
#	define NOPROFILE
#	define VAX11
#	define FILESEP '['
#else
#	define FILESEP '/'
#endif

/* INITFARGS - initialize fexec arg block*/
/*      This sets the arg block pointer to the beginning of the block.*/
#define INITFARGS() fargp = &fargs[0]

/* ENDFARGS - end fexec argument block*/
/*      This ends the argument block with a zero pointer.*/
#define ENDFARGS() *fargp = 0

#define DEFSIZE 1024
#define NARGS   64
#define NINCL   10
#define NDEFS   20
#define ICODE   0
#define LINK    1
#define MTEMP   2
#define ASTEMP  3
#define OTEMP	4		/* [vlh] 4.3, optimizer temp file */
#define NTEMPS  5

char *fargs[NARGS+1];
char **fargp;
char argbuf[DEFSIZE];
char *cfiles[NARGS+1];
char *loadargs[NARGS+1];
char *defs[NDEFS];
char *incl[NINCL];
char *fns[NTEMPS];
char *tfns[NTEMPS];

int ndefs;
int nincl;
char *argp  = &argbuf[0];
int cflag;
int nflag;
int ffpflag;    /* [vlh] 3.4 fast floating point format and library */
int ieeeflag;   /* [vlh] 3.4 ieee floating point format and library */
int lflag = 1;	/* [vlh] 4.2 default */
int pflag;
int eflag;      /* [vlh] 4.0 flag, Preprocessor only to stdout */
int comment;	/* [vlh] 4.2 flag, eflag plus leave comments */	
int sflag;
int tenflag;    /* [vlh] 4.2, 68010 destination */
int tflag;      /* [vlh] 4.2, put strings into .text segment */
int gflag;      /* [vlh] 4.2, symbolic debugger label generation */
#ifndef NOPROFILE
	int profile;    /* [vlh] 4.3, profiler flag */
#endif
int wflag;      /* [vlh] turn on warning messages */
int verbose;    /* [vlh] 3.4 force printing of file being processed */
int v6flag;     /* [vlh] 3.4 compile for version 6 */
int v7flag;     /* [vlh] 3.4 */
int s3flag;     /* [vlh] 3.4 */
int ucb;     	/* [vlh] 4.3 -4 flag for ucb compatibility */
int s5flag;     /* [vlh] 3.4 */
int optimize;   /* [vlh] 3.4 recognize optimization flag '-O' */
int status;
int oneflag;
int errno;

#ifdef MC68000
	char *preproc   = "/lib/cpp68"; 	/*[vlh] 4.2 */
	char *parser    = "/lib/c068";
	char *cgen      = "/lib/c168";
	char *optim     = "/lib/c268";
	char *onepass   = "/lib/c0168";
	char *pref      = "/lib/c680.o";
#ifndef NOPROFILE
	char *ppref     = "/lib/pc680.o";
#endif
	char *post      = "/lib/c68end.o";	/*[vlh] 4.2, for debugger */
	char *loader    = "/bin/lo68";
	char *asmprog   = "/bin/as68";

	char *deflib    = "/lib/lib7.a";
	char *deflibfp  = "/lib/libF.a";  /* [vlh] 3.4 */
	char *deflibep  = "/lib/libE.a";  /* [vlh] 3.4 */
	char *v6lib     = "/lib/libv6.a"; /* [vlh] 3.4 */
	char *v7lib     = "/lib/libv7.a"; /* [vlh] 3.4 */
	char *s3lib     = "/lib/libs3.a"; /* [vlh] 3.4 */
	char *ucblib    = "/lib/libucb.a"; /* [vlh] 4.3 */
	char *s5lib     = "/lib/libs5.a"; /* [vlh] 3.4 */
#else
#	ifndef VMS
	char *preproc   = "/usr/local/lib/cpp68";   /* [vlh] 4.2 */
	char *parser    = "/usr/local/lib/c068";
	char *cgen      = "/usr/local/lib/c168";
	char *optim     = "/usr/local/lib/c268";
	char *onepass   = "/usr/local/lib/c0168";
	char *pref      = "/usr/local/lib/c680.o";
#ifndef NOPROFILE
	char *ppref      = "/usr/local/lib/pc680.o";
#endif
	char *post      = "/usr/local/lib/c68end.o";/*[vlh] 4.2, for debugger */
	char *loader    = "/usr/local/lo68";
	char *asmprog   = "/usr/local/as68";

	char *deflib    = "/usr/local/lib/lib7.a";
	char *deflibfp  = "/usr/local/lib/libF.a";    /* [vlh] 3.4 */
	char *deflibep  = "/usr/local/lib/libE.a";    /* [vlh] 3.4 */
	char *v6lib     = "/usr/local/lib/libv6.a";   /* [vlh] 3.4 */
	char *v7lib     = "/usr/local/lib/libv7.a";   /* [vlh] 3.4 */
	char *s3lib     = "/usr/local/lib/libs3.a";   /* [vlh] 3.4 */
	char *ucblib    = "/usr/local/lib/libucb.a";  /* [vlh] 4.3 */
	char *s5lib     = "/usr/local/lib/libs5.a";   /* [vlh] 3.4 */
#	else
	char *preproc   = "cppdir:cpp68";
	char *parser    = "pardir:c068";
	char *cgen      = "cgdir:c168";
	char *optim     = "cgdir:c268";
	char *onepass   = "onepdir:c0168";
	char *pref      = "prefdir:c680.o";
	char *post      = "prefdir:c68end.o";
	char *loader    = "loadir:lo68";
	char *asmprog   = "asmdir:as68";

	char *deflib    = "clib:lib7.a";
	char *deflibfp  = "clib:libF.a";
	char *deflibep  = "clib:libE.a";
	char *v6lib     = "clib:libv6.a";
	char *v7lib     = "clib:libv7.a";
	char *s3lib     = "clib:libs3.a";
	char *ucblib    = "clib:libucb.a";
	char *s5lib     = "clib:libs5.a";
#	endif
#endif

	/* Main driver Flags */
char *ldflg     = "-r";
int mmuflag;    /*[vlh] 3.4 */
int asflag;

	/* Functions which return not integral values */
char *setend();
char *makecopy();
char *maketemp();

