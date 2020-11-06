/*	VMS INTERFACE DEFINES
 *	copyright (c) 1980 by Whitesmiths, Ltd.
 */

/*	basic parameters
 */
#define FAIL	-1
#define NFILES	16
#define NOBLOCK	010000000000

/*	codes for flags and access
 */
#define	WREAD	0001
#define WWRITE	0002
#define WOPEN	0004
#define WTTY	0010
#define WCR		0020
#define WFIX	0040
#define WDIRT	0100
#define WREMOVE	0200

/*	the Whitesmiths VMS control block
 */
typedef struct {
	BITS flags;
	COUNT bleft;
	TEXT *bnext, *pbuf, *rab;
	LONG lseek, lact, lend;
	} RCB;
