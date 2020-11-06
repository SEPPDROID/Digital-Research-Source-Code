/*	Transfer data through an I/O channel -- in this		*/
/*	particular case the quasi fifo on the Zilog Z8000	*/
/*	Zoom board.  This module should be replacable by	*/
/*	others which utitilize a different I/O channel		*/
/*	(i.e. RS-232), and different protocols.			*/

/*
*************************************************	
*						*
*	Author:		David Heintz		*
*	Module:		Channelc.c		*
*	Creation Date:	10/26/82		*
*	Language:	Standard C		*
*	Version:	0.0			*
*	Last Mod:	01/17/83 P.S.W.         *
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
#include "fioequ.h"
#include "chnequ.h"

/*
*************************************************
*						*
*	EXTERNAL DECLARATIONS			*
*						*
*************************************************
*/


/*
*************************************************
*						*
*	I/O Channel Related Externals		*
*	         and Globals			*
*						*
*************************************************
*/


/* Routines defined other modules	*/

extern	BYTE	input();
extern		output();
extern	BYTE	gb_word();
extern	BYTE	gb_long();
extern		pb_word();
extern		pb_long();

/* Varibles defined in other modules	*/

extern	WORD	bytlen;
extern	WORD	wrdlen;
extern	WORD	lnglen;

/* External port addresses		*/

extern	WORD	fiocr1;
extern	WORD	fioir0;
extern	WORD	fiomsi;
extern	WORD	fiomso;

/* Routines defined within this module */

extern	BYTE	snd_pkt();
extern	BYTE	rcv_pkt();
extern	BYTE	snd_sys();
extern		chn_err();


/* Static definitions for use within module	*/


/*
*****************************************
*					*
*	    snd_sys():			*
*	Transmit a load buffer		*
*	  over I/O channel		*
*					*
*****************************************
/
/	Inputs:		buffer type, segment, offset, data address, length
/	Outputs:	I/O channel status (byte)
/	Assumptions:	Data length is always less than FIO size
*/

BYTE	snd_sys(buftyp, basseg, basoff, buffer, ldlen)
BYTE	buftyp, basseg, *buffer;
WORD	basoff, ldlen;
{
  BYTE	code, fio_put(), fio_gack();
  int i;
  if (code == chsuc)
	{
	  fio_put(buftyp);
	  fio_put(basseg);
	  for (i=(wrdlen-1); i>=0; i--)
	    fio_put(gb_word(&basoff, i)); /* Send high-order byte first */
	  for (i=(wrdlen-1); i>=0; i--)
	    fio_put(gb_word(&ldlen, i));  /* Send high-order byte first */
	  for (i=0; i<ldlen; i++)	/* Send data */
	    fio_put(buffer[i]);
   	  code = fio_gack(fioack);	/* Try for an ack */
	}
  return(code);
}

/*
****************************************.
*					*
*	    snd_pkt():			*
*	Transmit a data packet		*
*	  over I/O channel		*
*					*
********************"*******************

/
/	Inputs:		data type, address, and length
/	Outputs:	I/O channel status (byte)
*/

BYTE	snd_pkt(pkttyp, pbyte, pktlen)

BYTE	pkttyp, *pbyte;
WORD	pktlen;
{
	BYTE	code, fio_put(), fio_gack();
	WORD	i;
	code = chsuc;
	if ((code = fio_put(pkttyp)) == chsuc)
		{
		  for (i=1; i <= pktlen; i++)
			if ((code = fio_put(*(pbyte++))) != chsuc)
				return(code);
		  code = fio_gack(acktyp);
		}
	return(code);
}

/*
*****************************************
*					*
*	    rcv_pkt():			*
*	Receive a data packet		*
*	  over I/O channel		*
*					*
*****************************************
/
/	Inputs:		data type, address, and length
/	Outputs:	I/O channel status (byte)
*/

BYTE	rcv_pkt(pkttyp, pbyte, pktlen)

BYTE	pkttyp, *pbyte;
WORD	pktlen;
{
	BYTE	code, fio_cat(), fio_get(), fio_pack();
	WORD	i;
	code = chsuc;
	if ((code = fio_cat(pkttyp)) == chsuc)
		{
		  for (i=1; i <= pktlen; i++)
		  	if ((code = fio_get(pbyte++)) != chsuc)
				return(code);
	  	  code = fio_pack(acktyp);
		}
	  else	fio_pack(ncktyp);
	return(code);
}



/*
*****************************************
*					*
*	    fio_gack():			*
*	Wait for a certain byte		*
*	from the fio.			*
*					*
*****************************************
/
/	Inputs:		Byte to wait for
/	Outputs:	Status indicating success/failure
*/

BYTE fio_gack(gbyte)
BYTE gbyte;
{
  BYTE code, ack, fio_get();
  WORD count;
  count = chto2;
  while (count--)
	{
	  code = fio_get(&ack);
   	  if ((code == chsuc) && (ack == gbyte))
	    return(code);
	}
  return(chena);
}


/*
*****************************************
*					*
*	    fio_pack():			*
*	Send either positive or		*
*	negative acknowledgement.	*
*					*
*****************************************
/
/	Inputs:		chack, or chnck
/	Outputs:	Status indicating success/failure
*/

BYTE fio_pack(ack)

BYTE ack;
{
	BYTE fio_put();
	return(fio_put(ack));
}


/*
*****************************************
*					*
*	    fio_cat():			*
*	Conditionally attach to		*
*	the fifo.			*
*					*
*****************************************
/
/	Inputs:		Byte to wait for
/	Outputs:	Status indicating success/failure
*/

BYTE fio_cat(tbyte)
BYTE tbyte;
{
  BYTE code, ack, fio_get();
  WORD count;
  count = chto2;
  while (count--)
	{
	  code = fio_get(&ack);
   	  if ((code == chsuc) && (ack == tbyte))
	    return(code);
	}
  return(cheut);
}


/*
*****************************************
*					*
*	    fio_get():		 	*
*	   Get a byte from 		*
*	   the I/O channel.		*
*					*
*****************************************
/
/	Inputs:		Address where byte is to be returned
/	Outputs:	Status indicating success/failure
*/

BYTE fio_get(rbyte)
BYTE *rbyte;
{
	WORD count;
	count = chto1;
	while (count--) 
		{
		  if ((input(fioir0) & fiordy) == fiordy)
			{
			  *rbyte = input(fiomsi);
		          return(chsuc);
			}
		}
	return(cheto);
}

/*
*****************************************
*					*
*	    fio_put():			*
*	   Put a byte into 		*
*	   the I/O channel.		*
*					*
*****************************************
/
/	Inputs:		Byte to be written to I/O channel
/	Outputs:	Status indicating success/failure
*/

BYTE fio_put(sbyte)

BYTE sbyte;
{
	WORD count;
	count = chto1;
	while (count--) 
		{
		  if ((input(fiocr1) & fiobsy) != fiobsy)
			{
			  output(fiomso, sbyte);
		          return(chsuc);
			}
		}
	return(cheto);
}

/*
*****************************************
*					*
*		chn_err():		*
*	   Global I/O channel error	*
*	   handling routine		*
*					*
*****************************************
/
/	Inputs:		Error code
/	Outputs:	None
*/
chn_err(d0,ebyte)
int d0;
BYTE ebyte;
{
/*		Dummy out for now		*/
}

/*		End of	Module			*/
