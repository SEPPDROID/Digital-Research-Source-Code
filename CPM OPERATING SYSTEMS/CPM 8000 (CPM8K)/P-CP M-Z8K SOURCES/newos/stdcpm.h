/*	CP/M INTERFACE HEADER FILE
*/

/*	Modified to include additional CP/M function defi-
	nitions and DPB structure definition 07/20/82 (DMH)
 */
/*	Modified to include BIOS header 10/13/82 (DMH)
*/
/*	Modified to include size of DPB 10/15/82 (DMH)
*/

#define FCB	struct fcb
#define FCBLEN	sizeof(FCB)
struct fcb {
	TEXT et;			/* assumed zero */
	TEXT fn[8];			/* blank padded */
	TEXT ft[3];			/* ditto */
	TINY ex;			/* start with zero */
	TEXT pad[2];		/* assumed zero by CP/M */
	TINY rcount;		/* [0, 128] */
	TINY dmap[16];		/* eight records per 1024 byte cluster */
	TINY nr;			/* next record to read/write */
	TINY bleft;
	TEXT *bnext;
	LONG lseek;
	COUNT bufseg;
	TEXT buf[128];
	};

/*	CP/M Disk Parameter Block
 */

#define DPB	struct dpb
#define DPBLEN	sizeof(DPB)
struct	dpb {
	 WORD spt;			/* Sectors per track */
	 BYTE bsh;			/* Block shift factor */
	 BYTE blm;			/* Block mask */
	 BYTE exm;			/* Extent mask */
	 WORD dsm;			/* Drive capacity */
	 WORD drm;			/* Directory capacity */
	 BYTE al0;			/* Directory allocation 0 */
	 BYTE al1;			/* Directory allocation 1 */
	 WORD cks;			/* Check vector size */
	 WORD off;			/* Track offset */
	 };


/*	CP/M Disk Parameter Header
 */

#define DPH	struct dph
#define DPHLEN	sizeof(DPH)
struct	dph {
	 BYTE *xltp;			/* Translation vector address */
	 WORD dphscr[3];		/* CP/M scratch pad area */
	 BYTE *dirbufp;			/* CP/M directory buffer */
	 DPB *dpbp;			/* DPB address */
	 BYTE *csvp;			/* Check vector address */
	 BYTE *alvp;			/* Allocation vector address */
	 };


/*	the Whitesmiths control block
 */
#define WCB	struct wcb

struct wcb {
	TINY flags, dev;
	FCB *pf;
	};

/*	flag values:
 */
#define WOPEN	1
#define WDIRT	2
#define WSTD	4
#define WCR		010
#define WXDIRT	020
#define WXOPEN	040

/*	device values:
 */
#define LST		-4
#define PUN		-3
#define RDR		-2
#define CON		-1

/*	intermodule communication
 */
#define CTRLZ	032
#define EOF		1
#define FAIL	-1
#define MCREATE	0
#define MOPEN	1
#define MREMOVE	2
#define MWRITE	4
#define SYSBUF	0x80

/*	CP/M innards
 */
#define CRDCON	1
#define CWRCON	2
#define CRDRDR	3
#define CWRPUN	4
#define CWRLST	5
#define CGIOST	7
#define CSIOST	8
#define CPRBUF	9
#define CRDBUF	10
#define CICRDY	11
#define CLFTHD	12
#define CINIT	13
#define CSDRNO	14
#define COPEN	15
#define CCLOSE	16
#define CSRCH	17
#define CSRCHN	18
#define CDEL	19
#define CREAD	20
#define CWRITE	21
#define CMAKE	22
#define CRENAME	23
#define CILOGIN	24
#define CGDRNO	25
#define CSETAD	26
#define CIALLOC	27
#define CIPARMS	31
