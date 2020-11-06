#define	SYMLEN	10
#define	LASTSYM	-1

#define	MDASIZE	long
#define	RQUAD	0x80
#define	RLONG	0x40
#define	RINT	0x20
#define	RCHR	0x10
#define	RCR	0x08

#define	CODE	1
#define	DATA	0

#define	CHR(bits8)	(bits8 & 0xFF)
#define	SHT(bitsf)	(bitsf & 0xFFFF)

/*
********	IO
*/

char	fgetc(), *gets(), *fgets();

/*
********	symbol stuff
*/

#define	SYMDEF	0x80
#define	SYMUDEF	0x40

struct	symbol {
	int	utype;
	MDASIZE	uvalue;
	char	uname[SYMLEN+1];
};

extern	struct	symbol	regtbl[];
extern	struct	symbol	symtbl[];

/*
********	bit tables
*/

struct	bitable	{
	char	bitlabel;
	WORD	bitmask;
};
extern	struct	bitable	pflags[];

/*
********	debugger flags
*/

#define	TTRACE	0x0001
#define	XTRACE	0x0002
#define	INDBG	0x0010
#define	LSTEP	0x0020
#define	TEMPPC	0x0040
#define	MUFOM	0x0100

/*
********	misc
*/

extern	BYTE	tbyte;
extern	WORD	tword;
extern	LONG	tlong;
extern	POINTER	tptr;

extern	int	error,errctr;
extern	int	infixflg;
extern	char	*errors[];

extern	char	*iscan,*oscan,*tscan;
extern	char	ichksum,ochksum;
extern	char	iline[COLS],oline[COLS];	/* the IO buffers */

extern	WORD	bswap();
extern	MDASIZE	infix(),infixer(),numin(),fltin(),adrin(),hexin(),getsym();

extern	char	symbuf[SYMLEN+1];

extern	char	aflag,bflag,cflag,dflag,eflag,hflag,iflag,jflag,kflag;
extern	char	lflag,mflag,nflag,oflag,pflag,qflag,rflag,sflag,tflag;
extern	char	uflag,vflag,wflag,xflag,yflag,zflag;

