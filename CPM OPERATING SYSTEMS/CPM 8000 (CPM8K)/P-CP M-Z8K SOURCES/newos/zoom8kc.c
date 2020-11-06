/*  This module is a collection of all the Z8000	*/
/*  dependent C code for the P-CP/M dual processor	*/
/*  BIOS.						*/


/*
*************************************************	
*						*
*	Author:		David Heintz		*
*	Module:		Zoom8kc.c		*
*	Creation Date:	10/26/82		*
*	Language:	Standard C		*
*	Version:	1.0			*
*	Last Mod:	11/26/82		*
*						*
*************************************************
*/



/*
*************************************************
*						*
*	   	STANDARD HEADERS		*
*						*
*************************************************
*/

#include "unstdsys.h"
#include "zooequ.h"
#include "chnequ.h"

/*
*************************************************
*						*
*	EXTERNAL DECLARATIONS			*
*						*
*************************************************
*/

extern	BYTE	gb_word();
extern	BYTE	gb_long();
extern		pb_word();
extern		pb_long();

extern	WORD	bytlen;
extern	WORD	wrdlen;
extern	WORD	lnglen;

/*
*************************************************
*						*
*	 Byte Ordering Arrays For		*
*	     Word and Long			*
*						*
*************************************************
*/

static	BYTE	wrdtab[2] = {1, 0};
static	WORD	lngtab[4] = {3, 2, 1, 0};

/*
*************************************************
*						*
*	Machine dependent entities		*
*						*
*************************************************
*/

WORD	bytlen = 1;
WORD	wrdlen = 2;
WORD	lnglen = 4;

/*
*************************************************
*						*
*	Port Addresses for Zoom Board		*
*						*
*************************************************
*/

WORD	fiodat	= 0x0001;	/* Values for Zoom side	*/
WORD	fioctl	= 0x0003;	/* only.		*/

/*
*************************************************
*						*
*	     Memory Region Table		*
*						*
*************************************************
*/

/*  The following table describes two memory regions:		*/
/*  Region 1	Type:	Separated instruction and data space	*/
/*		Length:	64k I space, 64k D space, 128k total.	*/ 
/*		Base:	<<42>>					*/
/*								*/
/*  Region 2	Type:	Merged instruction and data space	*/
/*		Length:	64k combined space.			*/
/*		Base:	<<1>>					*/
/*								*/
/*  Notes	This table is machine dependent (ZOOM board), 	*/
/*		but not processor dependent.			*/
/*		Table does not include system segment.		*/
/*		Table should probably be removed to module of	*/
/*		machine dependent code and data.		*/

/*	********* This is the real memory region table ********	*/

/*
struct	mrt	{
			int count;
			struct	{long tpalow;
				 long tpalen;
				}	regions[2];
		}	memtab = {2, 
					{{0x42000000L, 0x1ffffL},
					 {0x01000000L, 0x0ffffL}
					}
				 };
*/

/*	******** This is a kludge table for use with 	******	*/
/*	******** flaky zoom board hardware.		******	*/

struct	mrt	{
			int count;
			struct	{long tpalow;
				 long tpalen;
				}	regions[2];
		}	memtab = {2, 
					{{0x42000000L, 0x1ffffL},
					 {0x02000000L, 0x0ffffL}
					}
				 };

/*
*****************************************
*					*
*	      gb_word():		*
*        Retrieve a specified		*
*	   byte from a word		*
*					*
*****************************************
/
/	Inputs:		Address of word
/			Byte number
/	Outputs:	Byte corresponding to 
/			byte number in word
*/

BYTE	gb_word(pbyte, i)

BYTE 	*pbyte;
WORD 	i;
{
	return(pbyte[wrdtab[i]]);
}	

/*
*****************************************
*					*
*	      gb_long():		*
*        Retrieve a specified		*
*	   byte from a long		*
*					*
*****************************************
/
/
/	Inputs:		Address of long
/			Byte number
/	Outputs:	Byte corresponding to 
/			byte number in long
*/

BYTE	gb_long(pbyte, i)

BYTE	*pbyte;
WORD	i;
{
	return(pbyte[lngtab[i]]);
}	

/*
*****************************************
*					*
*	      pb_word():		*
*          Place a specified		*
*	   byte into a word		*
*					*
*****************************************
/
/
/	Inputs:		Byte to be place
/			Address of word
/			Byte number
/	Outputs:	None
*/

pb_word(sbyte, pbyte, i)

BYTE 	sbyte, *pbyte;
WORD 	i;
{
	pbyte[wrdtab[i]] = sbyte;
}	

/*
*****************************************
*					*
*	      pb_long():		*
*          Place a specified		*
*	   byte into a long		*
*					*
*****************************************
/
/
/	Inputs:		Byte to be place
/			Address of long
/			Byte number
/	Outputs:	None
*/

pb_long(sbyte, pbyte, i)

BYTE 	sbyte, *pbyte;
WORD 	i;
{
	pbyte[lngtab[i]] = sbyte;
}	




/*		End of	Modul;
A!2,/****************************************************************************/
/*									    */
/*	                   B A S E P A G E . H                              */
/*	                   -------------------                              */
/*	                                                                    */
/*	This file contains a definition of the CP/M basepage structure,     */
/*	b_page.								    */
/*									    */
/*	NOTE: In the portable CP/M environment, it is NOT guaranteed        */
/*	that the location of the base page is known at link-edit time       */
/*	(as it is, for example, in CP/M-80 and CP/M-86.)  Instead, a        */
/*	pointer to the current basepage is delivered by the BDOS	    */
/*	to each new program which is run.   This pointer, _base, is         */
/*	initialized by the C startup function (startup.s) and is            */
/*	available to C programs as an external.                             */
/*									    */
/*	This file has been modified to live with the BDOS definitions.	    */
/*									    */
/****************************************************************************/

struct b_page
	{
		XADDR		ltpa;		/* Low TPA address	    */	
		XADDR		htpa;		/* High TPA address	    */	
		XADDR		lcode;		/* Start address of code seg*/
		long		codelen;	/* Code segment length	    */
		XADDR		ldata;		/* Start address of data seg*/
		long		datalen;	/* Data segment length	    */
		XADDR		lbss;		/* Start address of bss seg */
		long		bsslen;		/* Bss segment length	    */
		long		freelen;	/* Free segment length	    */
		char		resvd1[20];	/* Reserved area	    */
		struct fcb	fcb2;		/* Second basepage FCB	    */
		struct fcb	fcb1;		/* First basepage FCB	    */
		char		buff[128];	/* Default DMA buffer,	    */
						/*   command line tail	    */
	};
