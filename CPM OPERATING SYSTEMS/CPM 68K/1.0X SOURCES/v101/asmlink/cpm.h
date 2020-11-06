/****************************************************************************/
/*									    */
/*				   C P M . H				    */
/*				   ---------				    */
/*									    */
/*	This file contains CP/M specific definitions for the v6 library	    */
/*	for the Alcyon assembler / linker.				    */
/*									    */
/*	"stdio.h" must be included BEFORE this file.			    */
/*									    */
/****************************************************************************/

/*
 *	Cp/m FCB definition
 */

struct	xfcb					/****************************/
{						/*			    */
	BYTE	drive;				/* Disk drive field	    */
	BYTE	fname[8];			/* File name		    */
	BYTE	ftype[3];			/* File type		    */
	BYTE	extent;				/* Current extent number    */
	BYTE	s1,s2;				/* "system reserved"	    */
	BYTE	reccnt;				/* Record counter	    */
	BYTE	resvd[16];			/* More "system reserved"   */
	LONG	record;				/* Note -- we overlap the   */
						/* current record field to  */
						/* make this useful.	    */
};						/****************************/
#define	SECSIZ	128				/* # bytes / CP/M sector    */
						/****************************/

/****************************************************************************/
/*									    */
/*	Channel Control Block (CCB)					    */
/*									    */
/*	One CCB is allocated (statically) for each of the 16 possible open  */
/*	files under C (including STDIN, STDOUT, STDERR).  Permanent data    */
/*	regarding the channel is kept here.				    */
/*									    */
/*									    */
/****************************************************************************/

struct	ccb				/************************************/
{					/*				    */
	BYTE	flags;			/*	Flags byte		    */
	BYTE	chan;			/*	Channel number being used   */
	LONG	offset;			/*	File offset word (bytes)    */
	LONG	sector;			/* 	Sector currently in buffer  */
	LONG	hiwater;		/*	High water mark		    */
	struct xfcb fcb;		/*	File FCB		    */
	BYTE	buffer[SECSIZ];		/*	Read / write buffer	    */
};					/************************************/

#define	MAXFILE	16			/*	Maximum # files		    */
extern	struct	ccb	_iob[MAXFILE];	/*	Declare storage		    */

/*	Flags word bit definitions					    */
					/************************************/
#define	OPENED	 1			/*	Channel is OPEN		    */
#define	ISTTY	 2			/*	Channel open to TTT	    */
#define	ISLPT	 4			/*	Channel open to LPT	    */
#define	ISREAD	 8			/*	Channel open readonly	    */
#define	ISASCII	16			/*	ASCII file attached	    */
#define	ATEOF	32			/*	End of file encountered	    */
#define	DIRTY	64			/*	Buffer needs writing	    */
					/************************************/


/****************************************************************************/
/*									    */
/*		B D O S   F u n c t i o n   D e f i n i t i o n s	    */
/*		-------------------------------------------------	    */
/*									    */
/*	Following are BDOS function definitions used by the C runtime 	    */
/*	library.							    */
/*									    */
/****************************************************************************/

						/****************************/
#define	EXIT	 0				/* Exit to BDOS		    */
#define	CONOUT	 2				/* Direct console output    */
#define	LSTOUT	 5				/* Direct list device output*/
#define	CONIO	 6				/* Direct console I/O	    */
#define	CONBUF	10				/* Read console buffer	    */
#define	OPEN	15				/* OPEN a disk file	    */
#define	CLOSE	16				/* Close a disk file	    */
#define	DELETE	19				/* Delete a disk file	    */
#define	CREATE	22				/* Create a disk file	    */
#define	SETDMA	26				/* Set DMA address	    */
#define	B_READ	33				/* Read Random record	    */
#define	B_WRITE	34				/* Write Random record	    */
						/****************************/

/****************************************************************************/
/* Other CP/M definitions						    */
/****************************************************************************/
#define	TERM	"CON:"				/* Console file name	    */
#define	LIST	"LST:"				/* List device file name    */
#define	EOFCHAR	0x1a				/* End of file character-^Z */
						/****************************/
