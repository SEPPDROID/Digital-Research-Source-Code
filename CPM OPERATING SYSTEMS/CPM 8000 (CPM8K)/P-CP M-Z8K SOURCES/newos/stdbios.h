
/*
/--------------------------------
/	BIOS Request Codes	|
/--------------------------------
*/

#define inreq  0			/* Initialize		*/
#define wbreq  1			/* Warm boot		*/
#define csreq  2			/* Console status	*/
#define cireq  3			/* Console input	*/
#define coreq  4			/* Console output	*/
#define loreq  5			/* List output		*/
#define aoreq  6			/* Auxilliary output	*/
#define aireq  7			/* Auxilliary input	*/
#define hdreq  8			/* Home disk head	*/
#define sdreq  9			/* Select disk		*/
#define streq 10			/* Set track		*/
#define ssreq 11			/* Set sector		*/
#define sareq 12			/* Set DMA address	*/
#define rsreq 13			/* Read a sector	*/
#define wsreq 14			/* Write a sector	*/
#define lsreq 15			/* List status		*/
#define tsreq 16			/* Translate Sector	*/
#define sbreq 17			/* Set DMA base		*/
#define gmreq 18			/* Get memory table	*/
#define gireq 19			/* Get IOBYTE		*/
#define sireq 20			/* Set IOBYTE		*/
#define fbreq 21			/* Flush disk buffers	*/
#define svreq 22			/* Set Exception Vector	*/

/*
/--------------------------------
/	BIOS Status Codes	|
/--------------------------------
*/

#define csaye	0xff	/* Console character available		*/
#define csnay	0x00	/* Console character not available	*/
#define	sdsuc	0x00	/* Select disk successful		*/
#define	sderr	0x01	/* Select disk error			*/
#define sdfrs	0x00	/* First select call since warm boot	*/
#define rssuc	0x00	/* Read sector successful		*/
#define	rserr	0x01	/* Read sector error, non-recoverable	*/
#define wssuc	0x00	/* Write sector successful		*/
#define	wserr	0x01	/* Write sector error, non-recoverable	*/
#define	lsaye	0xff	/* List device ready			*/
#define	lsnay	0x00	/* List device not ready		*/

/*
/--------------------------------
/	BIOS Sector Length	|
/--------------------------------
*/

#define seclen	0x80	/* Length of CP/M sector (128 bytes)	*/
