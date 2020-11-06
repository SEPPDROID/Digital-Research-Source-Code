/*  This module is a collection of all the Z8000	*/
/*  dependent C code for the P-CP/M dual processor	*/
/*  BIOS.						*/


/*
*************************************************	
*						*
*	Author:		David Heintz		*
*	Module:		Kon8kc.c		*
*	Creation Date:	10/26/82		*
*	Language:	Standard C		*
*	Version:	1.0			*
*	Last Mod:	12/16/83		*
*						*
*************************************************
*/


/*  01/16/83	Adapted for Kontron Z8002 board (DMH)	*/

/*
*************************************************
*						*
*	   	STANDARD HEADERS		*
*						*
*************************************************
*/

#include "unstdsys.h"
#include "fioequ.h"
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

WORD	fiodat	= 0x0001;	/* Values for Z8002 side	*/
WORD	fioctl	= 0x0003;	/* only.			*/

/*	Port Addresses for the Zilog 8038 fifo			*/

WORD	fioreg	= 0x0051;
WORD	fiocr1	= 0x0003;
WORD	fioir0	= 0x0005;
WORD	fiomso	= 0x0017;
WORD	fiomsi	= 0x0019;

/*
*************************************************
*						*
*	     Memory Region Table		*
*						*
*************************************************
*/

/*  The following table describes two memory regions:		*/
/*  Region 1	Type:	Merged instruction and data space	*/
/*		Length:	64k combined space.			*/ 
/*		Base:	Pseudo-segement <<1>>			*/
/*								*/
/*  Region 2	Type:	Split instruction and data space	*/
/*		Length:	64k instruction space, 32, data space	*/
/*		Base:	Pseudo-segment <<2>>			*/
/*								*/
/*  Notes	This table is machine dependent 		*/
/*		but not processor dependent.			*/
/*		Table does not include system segment.		*/
/*		Table should probably be removed to module of	*/
/*		machine dependent code and data.		*/

/*	********* This is the real memory region table ********	*/

struct	mrt	{
			int count;
			struct	{long tpalow;
				 long tpalen;
				}	regions[2];
		}	memtab = {2, 
					{{0x01000000L, 0x10000L},
					 {0x02000000L, 0x10000L}
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




/*		End of	Module			*/
