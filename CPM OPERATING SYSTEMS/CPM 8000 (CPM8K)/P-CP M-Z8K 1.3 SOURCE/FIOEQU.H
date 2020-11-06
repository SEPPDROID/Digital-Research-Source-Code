/*	Zoom board fifo definitions		*/

#define fiobsy	0x20
#define fiordy	0x20
#define fiotst	0xaa
#define fioini	0xfe
#define fiofre	0xfd
#define fioack	0x03
#define fiobeg	0x00
#define fiolen	0x0400			/* No. of bytes in fio	*/
#define nrmbuf	0x02			/* Not last buffer	*/
#define lstbuf	0x01			/* Last buffer		*/

/*	System Location		*/

#define	sysseg	0x00			/* System segment	*/
#define	sysoff	0x0000			/* System offset	*/
#define sysent	0x1002			/* System entry point	*/
