/* I/O channel definitions (put in header)	*/
/* Hopefully hardware independent?		*/

#define byttyp	0x00
#define	wrdtyp	0x01
#define	lngtyp	0x02
#define	dattyp	0x03
#define	cmdtyp	0x04
#define	acktyp	0x05
#define	ncktyp	0x06

#define chsuc	0x00
#define cheto	0x01
#define chena	0x02
#define	cheut	0x03
#define cherr	0xff
#define chto1	0x7fff		/* In the 5--15 second range, I think */
#define chto2	0x7fff
