#define	SIGHUP	1
#define	SIGINTR	2
#define	SIGQUIT	3
#define BRKPIPE 13
#define	FNSIZE	64
#define	LBSIZE	512
#define	ESIZE	128
#define	GBSIZE	256
#define	NBRA	5
#define	EOF	-1
#define	CBRA	1
#define	CCHR	2
#define	CDOT	4
#define	CCL	6
#define NCCL	8
#define	CDOL	10
#define	CEOF	11
#define	CKET	12
#define	STAR	01
#define	READ	0
#define	WRITE	1
#define	NP	014

int	peekc=0;
int	brktst=0;
int	lastc=0;
char	savedfile[FNSIZE]=0;
char	file[FNSIZE]=0;
char	linebuf[LBSIZE]=0;
char	rhsbuf[LBSIZE/2]=0;
char	expbuf[ESIZE+4]=0;
int	circfl=0;
int	*zero=0;
int	*dot=0;
int	*dol=0;
int	*endcore=0;
int	*fendcore=0;
int	*addr1=0;
int	*addr2=0;
char	genbuf[LBSIZE]=0;
int	count[2]=0;
char	*nextip=0;
char	*linebp=0;
int	ninbuf=0;
int	io=0;
int	pflag=0;
int	onquit=0;
int	col=0;
char	*globp=0;
int	tline=0;
char	*tfname=0;
char	*loc1=0;
char	*loc2=0;
char	*locs=0
char	ibuff[512]=0;
char	obuff[512]=0;
int	ichanged=0;
int	nleft=0;
int	names[26]=0;
char	*braslist[NBRA]=0;
char	*braelist[NBRA]=0;

char	line[70]=0;
