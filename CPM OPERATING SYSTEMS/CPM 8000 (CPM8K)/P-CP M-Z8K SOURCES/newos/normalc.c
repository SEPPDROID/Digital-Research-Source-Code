/*	This module performs packet normalization and 	*/
/*  preparation to/from I/O channel.  Context dependent	*/
/*  data are put in a normal form before transmission	*/
/*  over the I/O channel. Conversly, received messages	*/
/*  are 'de-normalized' into machine dependent quanti-	*/
/*  ties before being returned to the caller.  This	*/
/*  portion of code should be machine independent, 	*/
/*  since values such as word-length and long-length	*/
/*  are declared as externals.				*/


/*
*************************************************	
*						*
*	Author:		David Heintz		*
*	Module:		Normalc.c		*
*	Creation Date:	10/26/82		*
*	Language:	Standard C		*
*	Version:	0.0			*
*	Last Mod:	01/16/83		*
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

/* Routines defined within this module */

extern	BYTE	rcv_cmd();
extern	BYTE	rcv_byte();
extern	BYTE 	rcv_word();
extern	BYTE	rcv_long();
extern	BYTE	rcv_data();

extern	BYTE	snd_cmd();
extern	BYTE	snd_byte();
extern	BYTE	snd_word();
extern	BYTE	snd_long();
extern	BYTE	snd_data();
extern		chn_err();


/* Routines defined outside of this module	*/

extern	BYTE	snd_pkt();
extern	BYTE	rcv_pkt();
extern	BYTE	gb_word();
extern	BYTE	gb_long();
extern		pb_word();
extern		pb_long();


/* Machine dependent externals	*/

extern	WORD bytlen;
extern	WORD wrdlen;
extern	WORD lnglen;

/*
*************************************************
*						*
*	I/O Channel Data Normalization		*
*		   Routines			*
*						*
*************************************************
*/


/*
*****************************************
*					*
*	    snd_byte():			*
*  	Normalize and transmit		*
*	a byte over I/O channel		*
*					*
*****************************************
/
/	Inputs:		byte to be transmitted
/	Outputs:	I/O channel status (byte)
*/

BYTE	snd_byte(sbyte)

BYTE	sbyte;
{
	return(snd_pkt(byttyp, (BYTE *)&sbyte, bytlen));
}

/*
*****************************************
*					*
*	    snd_word():			*
*  	Normalize and transmit		*
*	a word over I/O channel		*
*					*
*****************************************
/
/	Inputs:		word to be transmitted
/	Outputs:	I/O channel status (byte)
*/

BYTE	snd_word(sword)

WORD	sword;
{
	WORD	i, nword;
	BYTE	*pbyte;
	pbyte = (BYTE *)&nword;
	for (i = 0; i < wrdlen; i++)
	  *(pbyte++) = gb_word(&sword, i);
	return(snd_pkt(wrdtyp, (BYTE *)&nword, wrdlen));
}

/*
*****************************************
*					*
*	    snd_long():			*
*  	Normalize and transmit		*
*	a long over I/O channel		*
*					*
*****************************************
/
/	Inputs:		long to be transmitted
/	Outputs:	I/O channel status (byte)
*/

BYTE	snd_long(slong)

LONG	slong;
{
	LONG	nlong;
	WORD	i;
	BYTE	*pbyte;
	pbyte = (BYTE *)&nlong;
	for (i = 0; i < lnglen; i++)
	  *(pbyte++) = gb_long(&slong, i);
	return(snd_pkt(lngtyp, (BYTE *)&nlong, lnglen));
}

/*
*****************************************
*					*
*	    snd_data():			*
*  	Normalize and transmit		*
*	data over I/O  channel		*
*					*
*****************************************
/
/	Inputs:		pointer to data to be transmitted
/	Outputs:	I/O channel status (byte)
*/

BYTE	snd_data(pbyte, datlen)

BYTE	*pbyte;
WORD	datlen;
{
	return(snd_pkt(dattyp, pbyte, datlen));
}

/*
*****************************************
*					*
*	    snd_cmd():			*
*  	Normalize and transmit		*
*     a command over I/O channel	*
*					*
*****************************************
/
/	Inputs:		command to be transmitted
/	Outputs:	I/O channel status (byte)
*/

BYTE	snd_cmd(sd0, sd1, sd2)

WORD	sd0;
LONG	sd1, sd2;

{
	struct	{
			WORD	request;
			LONG	param1;
			LONG	param2;
		}	cmd;

	LONG	nlong;
	WORD	i;
	BYTE	*pbyte;
	pbyte = (BYTE *)&cmd;
	for (i = 0; i < wrdlen; i++)
	  *(pbyte++) = gb_word(&sd0, i);
	for (i = 0; i < lnglen; i++)
	  *(pbyte++) = gb_long(&sd1, i);
	for (i = 0; i < lnglen; i++)
	  *(pbyte++) = gb_long(&sd2, i);
	return(snd_pkt(cmdtyp, (BYTE *)&cmd, sizeof(cmd)));
}

/*
*****************************************
*					*
*	    rcv_byte():			*
*  	 Receive and normalize		*
*	a byte over I/O channel		*
*					*
*****************************************
/	Inputs:		Address of where to put received byte
/	Outputs:	I/O channel status (BYTE)
/	Actions:	Place received byte at pbyte
*/

BYTE	rcv_byte(pbyte)

BYTE	*pbyte;
{
	return(rcv_pkt(byttyp, pbyte, bytlen));
}


/*
*****************************************
*					*
*	    rcv_word():			*
*  	 Receive and normalize		*
*	a word over I/O channel		*
*					*
*****************************************
/	Inputs:		Address of where to put received word
/	Outputs:	I/O channel status (BYTE)
/	Actions:	Place received word at pword
*/

BYTE	rcv_word(pword)

BYTE	*pword;

{
	BYTE code, *pbyte;
	WORD i, rword;
	pbyte = (BYTE *)&rword;
	code = rcv_pkt(wrdtyp, (BYTE *)&rword, wrdlen);
	if (code == chsuc)
	  for (i = 0; i < wrdlen; i++)
	    pb_word(*(pbyte++), pword, i);
	return(code);
}

/*
*****************************************
*					*
*	    rcv_long():			*
*  	 Receive and normalize		*
*	a long over I/O channel		*
*					*
*****************************************
/	Inputs:		Address of where to put received long
/	Outputs:	I/O channel status (BYTE)
/	Actions:	Place received long at plong
*/

BYTE	rcv_long(plong)

BYTE	*plong;

{
	BYTE code, *pbyte;
	WORD i;
	LONG rlong;
	pbyte = (BYTE *)&rlong;
	code = rcv_pkt(lngtyp, (BYTE *)&rlong, lnglen);
	if (code == chsuc)
	  for (i = 0; i < lnglen; i++)
	    pb_long(*(pbyte++), plong, i);
	return(code);
}

/*
*****************************************
*					*
*	    rcv_data():			*
*  	 Receive and normalize		*
*	 data over I/O channel		*
*					*
*****************************************
/	Inputs:		Address of where to put received data
/	Outputs:	I/O channel status (BYTE)
/	Actions:	Place received data at pbyte
*/

BYTE	rcv_data(pbyte, datlen)

BYTE	*pbyte;
{
	return(rcv_pkt(dattyp, pbyte, datlen));
}

/*
*****************************************
*					*
*	    rcv_cmd():			*
*  	Receive and Normalize		*
*     a command over I/O channel	*
*					*
*****************************************
/
/	Inputs:		addresses of command parameters 
/	Outputs:	I/O channel status (byte)
*/

BYTE	rcv_cmd(sd0, sd1, sd2)

WORD	*sd0;
LONG	*sd1, *sd2;

{
	struct	{
			WORD	request;
			LONG	param1;
			LONG	param2;
		}	cmd;

	LONG	nlong;
	WORD	i;
	BYTE	code, *pbyte;
	code = rcv_pkt(cmdtyp, (BYTE *)&cmd, sizeof(cmd));
	if (code == chsuc)
	  {
	    pbyte = (BYTE *)&cmd;
	    for (i = 0; i < wrdlen; i++)
	       pb_word(*(pbyte++), sd0, i);
	    for (i = 0; i < lnglen; i++)
	       pb_long(*(pbyte++), sd1, i);
	    for (i = 0; i < lnglen; i++)
	       pb_long(*(pbyte++), sd2, i);
	  }
	return(code);
}

/*		End of	Module			*/
