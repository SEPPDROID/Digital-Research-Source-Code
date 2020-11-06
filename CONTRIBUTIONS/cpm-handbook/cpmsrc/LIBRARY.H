#define LIBVN "1.0"	/* Library Version Number */
/* 12:36 02/21/83 */

/* This file contains groups of useful definitions.
   It should be included at the front of any programs
   that use the functions in BLIB. */


/* Definitions to make minor language modifications to C. */
#define short char		/* Short is not supported directly */

/* one of the functions (bv_make) in the library uses the BDS C
   function, alloc, to allocate memory. The following definitions
   are provided for alloc. */

struct _header			/* header for block of memory allocated */
	{
	struct _header *_ptr;	/* pointer to the next header in the chain */
	unsigned _size;		/* number of bytes in the allocated block */
	};
struct _header _base;		/* declare the first header of the chain */
struct _header *_allocp;	/* used by alloc() and free() functions */


/* BDOS Function Call Numbers. */

#define SETDISK 14	/* Set (Select) Disk */
#define SEARCHF	17	/* Search First */
#define SEARCHN	18	/* Search Next */
#define DELETEF 19	/* Delete File */
#define GETDISK 25	/* Get Default Disk (Currently Logged In) */
#define SETDMA	26	/* Set DMA (Read/Write) address */
#define GETDPARM 31	/* Get Disk Parameter Block address */
#define GETUSER 32	/* Get Current User Number */
#define SETUSER 32	/* Set Current User Number */


/* Direct BIOS Calls.
   These definitions are for direct calls into the BIOS.
   WARNING - Using these makes program less transportable.
   Each symbol is related to its corresponding Jump in the
   BIOS Jump Vector.
   Only the more useful entries are defined. */

#define CONST	2	/* Console Status */
#define CONIN	3	/* Console Input */
#define CONOUT	4	/* Console Output */
#define LIST	5	/* List Output */
#define AUXOUT	6	/* Auxiliary Output */
#define AUXIN	7	/* Auxiliary Input */

#define HOME	8	/* Home disk */
#define SELDSK	9	/* Select logical disk */
#define SETTRK	10	/* Set track */
#define SETSEC	11	/* Set sector */
#define SETDMA	12	/* Set DMA Address */
#define DREAD	13	/* Disk read */
#define DWRITE	14	/* Disk write */
#define LISTST	15	/* List status */
#define SECTRN	16	/* Sector translate */
#define AUXIST	17	/* Auxiliary Input Status */
#define AUXOST	18	/* Auxiliary Output Status */

			/* "Private" entries in Jump Vector */
#define CIOINIT	19	/* Specific Character I/O Initialization */
#define SETDOG	20	/* Set Watchdog timer */
#define CBGADDR	21	/* Configuration Block, Get Address */


/* defines for accessing the Configuration Block */

#define CB_GET 21		/* bios jump number to access routine */
#define DEV_INIT 19		/* bios jump to do device initialization */

#define CB_DATE 0		/* date in ASCII */
#define CB_TIMEA 1		/* time in ASCII */
#define CB_DTFLAGS 2		/* date, time flags */
#define TIME_SET 0x01		/* this bit NZ means date has been set */
#define DATE_SET 0x02		/* this bit NZ means time has been set */

#define CB_FIP 3		/* forced input pointer */
#define CB_SUM 4		/* system startup message */

#define CB_CI 5			/* console input */
#define CB_CO 6			/* console output */
#define CB_AI 7			/* auxiliary input */
#define CB_AO 8			/* auxiliary output */
#define CB_LI 9			/* list input */
#define CB_LO 10		/* list output */

#define CB_DTA 11		/* device table addresses */
#define CB_C1224 12		/* clock 12/24 format flag */
#define CB_RTCTR 13		/* real time clock tick rate (per second) */

#define CB_WDC 14		/* watchdog count */
#define CB_WDA 15		/* watchdog address */

#define CB_FKT 16		/* function key table */
#define CB_COET 17		/* console output escape table */

#define CB_D0_IS 18		/* device 0 initialization stream */
#define CB_D0_BRC 19		/* device 0 baud rate constant */

#define CB_D1_IS 20		/* device 1 initialization stream */
#define CB_D1_BRC 21		/* device 1 baud rate constant */

#define CB_D2_IS 22		/* device 2 initialization stream */
#define CB_D2_BRC 23		/* device 2 baud rate constant */

#define CB_IV 24		/* interrupt vector */
#define CB_LTCBO 25		/* long term config. block offset */
#define CB_LTCBL 26		/* long term config. block length */

#define CB_PUBF 27		/* public files flag */
#define CB_MCBUF 28		/* multi-command buffer */
#define CB_POLLC 29		/* polled console flag */


	/* device numbers and names for physical devices */
	/* NOTE : Change these definitions for your computer system */

#define T_DEVN 0		/* Terminal */
#define M_DEVN 1		/* Modem */
#define P_DEVN 2		/* Printer */

#define MAXPDEV 2		/* maximum physical device number */

	/* names for the physical devices */
#define PN_T "TERMINAL"
#define PN_M "MODEM"
#define PN_P "PRINTER"

	/* structure and definitions for function keys */

#define FK_ILENGTH 2		/* number of chars. input when f-key pressed
				   NOTE : This does NOT include the ESCAPE  */
#define FK_LENGTH 16		/* length of string  (not including fk_term) */
#define FK_ENTRIES 18		/* number of function key entries in table */

struct _fkt			/* function key table */
	{
	char fk_input[FK_ILENGTH];	/* lead-in character is not in table */
	char fk_output[FK_LENGTH];	/* output character string */
	char fk_term;			/* safety terminating character */
	};


/* definitions and structure for device tables */

	/* protocol bits */
	/* Note : if the most significant bit is
	   set = 1, then the set_proto function
	   will logically OR in the value. This
	   permits Input DTR to co-exist with
	   Xon or Etx protocol. */

#define DT_ODTR 0x8004		/* output dtr high to send (OR'ed in) */
#define DT_OXON 0x0008		/* output xon */
#define DT_OETX 0x0010		/* output etx/ack */

#define DT_IRTS 0x8040		/* input RTS (OR'ed in) */
#define DT_IXON 0x0080		/* input xon */

#define ALLPROTO 0xDC		/* all protocols combined */

struct _dt			/* device table */
	{
	char dt_f1[14];		/* filler */
	char dt_st1;		/* status byte 1 - has protocol flags */
	char dt_st2;		/* status byte 2 */
	unsigned dt_f2;		/* filler */
	unsigned dt_etxml;	/* etx/ack message length */
	char dt_f3[12];		/* filler */
	} ;


/* Values returned by the comp_fname (compare file name) */

#define NAME_EQ	0	/* names equal */
#define NAME_LT 1	/* name less than mask */
#define NAME_GT 2	/* name greater than mask */
#define NAME_NE 3	/* name not equal (and comparison ambiguous) */


/* Structure for Standard CP/M File Control Block */

#define FCBSIZE 36		/* define the overall length of an FCB */

struct _fcb
	{
	short fcb_disk;		/* logical disk (0 = default) */
	char fcb_fname[11];	/* file name, type (with attributes) */
	short fcb_extent;	/* current extent */
	unsigned fcb_s12;	/* reserved for CP/M */
	short fcb_reccnt;	/* record count used in current extent */
	union			/* allocation blocks can be either */
		{		/* single or double bytes */
		short fcbab_short[16];
		unsigned fcbab_long[8];
		} _fcbab;	
	short fcb_currec;	/* current record within extent */
	char fcb_ranrec[3];	/* record for random read/write */
	};




/*  parameter block used for calls to the directory management routines  */

#define DIR_BSZ 128		/* directory buffer size */

struct _dirpb 
	{
	short dp_open;		/* 0 to request directory to be opened */
	short dp_end;		/* NZ when at end of directory */
	short dp_write;		/* NZ to write current sector to disk */
	struct _dir *dp_entry;	/* pointer to directory entry in buffer */
	char dp_buffer [DIR_BSZ];	/* directory sector buffer */
	char dp_disk;		/* current logical disk */
	int dp_track;		/* start track */
	int dp_sector;		/* start sector */
	int dp_nument;		/* number of directory entries */
	int dp_entrem;		/* entries remaining to process */
	int dp_sptrk;		/* number of sectors per track */
	int dp_nabpde;		/* number of allocation blocks per dir. entry */
	unsigned dp_nab;	/* number of allocation blocks */
	int dp_absize;		/* allocation block size (in Kbytes) */
	};

/* The err_dir function is used to report errors found by the
   directory management routines, open_dir and rw_dir.
   Err_dir needs an parameter to define the operation being
   performed when the error occurred. The following #defines
   represent the operations possible. */

#define W_DIR	0	/* Writing Directory */
#define R_DIR	1	/* Reading Directory */
#define O_DIR	2	/* Opening Directory */


/*  disk parameter block maintained by CPM  */

struct _dpb 
	{
	unsigned dpb_sptrk;	/* sectors per track */
	short dpb_bshift;	/* block shift */
	short dpb_bmask;	/* block mask */
	short dpb_emask;	/* extent mask */
	unsigned dpb_maxabn;	/* maximum allocation block number */
	unsigned dpb_maxden;	/* maximum directory entry number */
	short dpb_rab0;		/* allocation blocks reserved for */
	short dpb_rab1;		/*   directory blocks */
	unsigned dpb_diskca;	/* disk changed workarea */
	unsigned dpb_trkoff;	/* track offset */
	};


/*  disk directory entry format  */

struct _dir {
	char de_userno;		/* user number or 0xE5 if free entry */
	char de_fname[11];	/* file name [8] and type [3] */
	int de_extent;		/* extent number of this entry */
	int de_reccnt;		/* number of 128-byte records used in last
				   allocation block */
	union			/* allocation blocks can be either */
		{		/* single or double bytes */
		short de_short[16];
		unsigned de_long[8];
		} _dirab;	
	};

/*  disk request parameters for BIOS-level Read/Writes */

struct _drb
	{
	short dr_disk;		/* logical disk A = 0, B = 1... */
	unsigned dr_track;	/* track (for SETTRK) */
	unsigned dr_sector;	/* sector (for SETSEC) */
	char *dr_buffer;	/* buffer address (for SETDMA) */ 
	} ;


/* search control block used by directory scanning functions */
struct _scb		
	{
	short scb_userno;	/* user number(s) to match */
	char scb_fname[11];	/* file name and type */
	short scb_extent;	/* extent number */
	char unused[19];	/* dummy bytes to make this appear like
				   a file control block */
	short scb_length;	/* number of bytes to compare */
	short scb_disk;		/* current disk to be searched */
	unsigned scb_adisks;	/* bit map of disks to be searched.
				   The right-most bit is for disk A:. */
	} ;


/* code table related definitions */

#define CT_SNF 0xFFFF	/* String Not Found */

struct _ct		/* define structure of code table */
	{
	unsigned _ct_code;	/* code value */
	char *_ct_sp;		/* string pointer */
	};


/* Structure for bit-vectors */

struct _bv
	{
	unsigned bv_bytes;	/* number of bytes in the vector */
	char *bv_bits;		/* pointer to the first byte in the vector */
	char *bv_end;		/* pointer to byte following bit vector */
	} ;



/* End of LIBRARY.H */
