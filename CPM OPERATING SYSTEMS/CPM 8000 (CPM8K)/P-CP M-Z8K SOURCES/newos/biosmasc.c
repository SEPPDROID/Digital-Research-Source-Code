
/*
***************************************************************************

This is the Master/requestor portion of a Dual processor P-CP/M BIOS.  It
should be designed such that only the low level inter-processor transfer
protocols and BIOS primitives should have to be modified to accomplish 
porting to different hardware configurations.  Error checking and report-
ing is necessary because I/O channel may be unplugged or unreliable.

***************************************************************************
*/

/*
*************************************************	
*						*
*	Author:		David Heintz		*
*	Module:		Biosmasc.c		*
*	Creation Date:	10/20/82		*
*	Language:	Standard C		*
*	Version:	1.0			*
*	Last Mod:	01/20/83		*
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
#include "stdcpm.h"
#include "stdbios.h"
#include "chnequ.h"

/*
*************************************************
*						*
*	EXTERNAL DECLARATIONS			*
*						*
*************************************************
*/

/*

extern	wboot();
*************************************************
*						*
*	BIOS Entry Point declaration		*
*						*
*************************************************
*/

extern	LONG	_bios();

/*
*************************************************
*						*
*	     Other Globals			*
*						*
*************************************************
*/

extern		biosinit();			/* Initialize system	*/
extern		flush();			/*  Flush buffer	*/

/*
*************************************************
*						*
*	I/O Channel Related Externals		*
*						*
*************************************************
*/

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

/*
*************************************************
*						*
*	Machine and Processor Dependent		*
*		  Externals			*
*						*
*************************************************
*/

extern		*memtab;
extern	LONG	trapvec[];
extern		mem_cpy();
extern	LONG	map_adr();


/*
*************************************************
*						*
*	LOCAL DEFINITIONS			*
*						*
*************************************************
*/

static char signon[] = "\n\rKontron Z8000 BIOS v. 1.0 11/23/82\n\r";

/*
*************************************************
*						*
*		Check Vectors			*
*	 512 Directory Entries Maximum		*
*						*
*************************************************
*/

BYTE csvtab[8][128] = {{0}};

/*
*************************************************
*						*
*	     Allocation Vectors			*
*	2048 Allocation Blocks Maximum		*
*						*
*************************************************
*/

BYTE alvtab[8][256] = {{0}};


/*
*************************************************
*						*
*	    Data Parameter Blocks		*
*		Unitialized			*
*						*
*************************************************
*/

DPB dpbtab[8] = {0};

/*
*************************************************
*						*
*	    Directory Buffer			*
*						*
*************************************************
*/

BYTE dirbuf[128] = {0};

/*
*************************************************
*						*
*	Disk Parameter Headers			*
*	Maximum of 8  defined			*
*						*
*************************************************
*/

DPH	dphtab[8] = 
/*
{      *xltp,  dphscr[3],    *dirbufp,       *dpbp,       *csvp,       *alvp}
*/
{
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[0],   csvtab[0],   alvtab[0]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[1],   csvtab[1],   alvtab[1]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[2],   csvtab[2],   alvtab[2]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[3],   csvtab[3],   alvtab[3]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[4],   csvtab[4],   alvtab[4]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[5],   csvtab[5],   alvtab[5]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[6],   csvtab[6],   alvtab[6]},
{(BYTE *) 0L,    {0,0,0},  &dirbuf[0],  &dpbtab[7],   csvtab[7],   alvtab[7]},
};

	
/*
*************************************************
*						*
*	 Time-Honored BIOS Variables		*
*						*
*************************************************
*/

static	LONG	dmaadr = 0;	/* Caller's data transfer address	*/
static	WORD	sector = 0;	/* Current sector (Unused)		*/
static	WORD	track = 0;	/* Current track  (Unused)		*/
static	BYTE	disk = 0;	/* Current drive number  (Unused)	*/
static	BYTE	IOBYTE = 0;	/* Logical to physical mapping (Unused)	*/
static	BYTE	retry = 0xff;	/* I/O channel retry boolean		*/

/*
*************************************************
*						*
*		Sector Buffer			*
*						*
*************************************************
*/

static	BYTE	secbuf[seclen] = {0};

/*
*************************************************
*						*
*	MISCELLANEOUS   FUNCTIONS		*
*						*
*************************************************
*/

/*
/--------------------------------
/				|
/	Print a Message		|
/				|
/--------------------------------
/ Inputs:	Pointer to message
/		terminated by '\0'
/ Outputs:	None
/ Actions:	Send message to slave
/		console output.
/
*/

prtmsg(pbyte)
BYTE *pbyte;
{
  char c;
  while(c = *(pbyte++))  
    _bios((WORD)coreq, (LONG)c, 0L);
}

/*
/--------------------------------
/				|
/	Illegal Command 	|
/				|
/--------------------------------
/ Inputs:	None
/ Outputs:	None
/ Actions:	Currently unused
/
*/

ill_cmd()

{
}

/*
/--------------------------------
/				|
/	biosinit();		|
/				|
/--------------------------------
/ Inputs:	None
/ Outputs:	None
/ Actions:	Initialize hardware
/
*/

biosinit()
{
  prtmsg(signon);
  _bios((WORD)inreq, 0L, 0L);		/* Make BIOS request	*/
}

/*
/--------------------------------
/				|
/	Flush Disk Buffer	|
/				|
/--------------------------------
/ Inputs:	None
/ Outputs:	None
/ Actions:	Flush Disk Buffer on Slave
/
*/

flush()
{
  _bios((WORD)fbreq, 0L, 0L);		/* Ignore return code?	*/
}

/*
/--------------------------------
/				|
/	snd_rcv():		|
/	send a command and	|
/	receive a parameter	|
/				|
/--------------------------------
/ Inputs:	BIOS parameters d0, d1, d2
/ Outputs:	LONG return parameter
/ Actions:	Send command through fifo and
/		receive return parameter
/
*/

LONG snd_rcv(d0, d1, d2)
WORD d0;
LONG d1, d2;
{
  BYTE code;
  LONG temp;
  do
	{
	  code = snd_cmd(d0, d1, d2);
	  if (code == chsuc)
	    code = rcv_long(&temp);
	  if (code != chsuc)
		{
		  chn_err(d0, code);
		  temp = cherr;
		}
	}
	while (retry && (code != chsuc));
  return(temp);
}

/*
*************************************************
*						*
*	  P-CP/M BIOS ENTRY POINT		*
*						*
*************************************************
*/

LONG _bios(d0, d1, d2)
WORD	d0;
LONG	d1, d2;

{
char zzz = {"kludge "};
    switch(d0)
      {

	/*
	/--------------------------------
	/				|
	/	Function 4:		|
	/	Console Output		|
	/				|
	/--------------------------------
	/ Inputs:	(BYTE) d1 contains character
	/		to output to slave console
	/ Outputs:	None
	/ Actions:	Output character from master
	/		to slave console I/O.
	*/
	case coreq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	/	Function 5:		|
	/	List Output		|
	/				|
	/--------------------------------
	/ Inputs:	(BYTE) d1 contains character
	/		to output to slave list device.
	/ Outpus:	None
	/ Actions:	Output character from master 
	/		to slave list I/O.
	*/
	case loreq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	/	Function 6:		|
	/	Auxilliary Output	|
	/				|
	/--------------------------------
	/ Inputs:	(BYTE) d1 contains character
	/		to output to slave aux. device.
	/ Outputs:	None
	/ Actions:	Output character from master
	/		to slave auxilliary (punch) I/O.
	*/
	case aoreq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	 /	Function 8:		|
	/	Home disk		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	None
	/ Actions:	Perform slave home disk function.
	*/
	case hdreq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	/	Function 10:		|
	/	Set Track		|
	/				|
	/--------------------------------
	/ Inputs:	(WORD) d1 contains track number.
	/ Outputs:	None
	/ Actions:	Set track number from master on slave.
	*/
	case streq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	/	Function 11:		|
	/	Set Sector		|
	/				|
	/--------------------------------
	/ Inputs:	(WORD) d1 contains sector number
	/ Outputs:	None
	/ Actions:	Set sector number from master on slave.
	*/
	case ssreq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	/	Function 20:		|
	/	Set I/O byte		|
	/				|
	/--------------------------------
	/ Inputs:	BYTE d1 contains IOBYTE value
	/ Outputs:	None
	/ Actions:	Send contents of slave
	/		location IOBYTE to master.
	*/
	case sireq:			/* Fall through to next case	*/


	/* All degenerate cases of BIOS functions	*/
	/* end up falling through to here. Request 	*/
	/* is forwarded to the slave BIOS and no 	*/
	/* parameters are returned to the master.	*/

		{
		  BYTE code;
		  code = snd_cmd(d0, d1, d2);
		  if (code != chsuc)
		    chn_err(d0, code);
		}
		break;			/* Request completed.		*/


	/*
	/--------------------------------
	/				|
	/	Function 2:		|
	/	Console Status		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	returns console status.
	/ Actions:	receive 1-byte slave con-
	/		sole status.
	*/
	case csreq:			/* Fall through to next case	*/


	/*
	/--------------------------------
	/				|
	/	Function 7:		|
	/	Auxilliary Input	|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	Return character from aux device
	/ Actions:	Input a character from slave
	/		auxilliary (reader) I/O to master.
	*/
	
	case aireq:			/* Fall through to next case	*/

	/*
	/--------------------------------
	/				|
	/	Function 15:		|
	/	List Status		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	Return status of slave list device.
	/ Actions:	Send 1-byte slave list
	/		status to master.
	*/
	case lsreq:			/* Fall through to next case	*/
	
	/*
	/--------------------------------
	/				|
	/	Function 16:		|
	/	Translate Sector	|
	/				|
	/--------------------------------
	/ Inputs:	(LONG) d1 is sector translation table address
	/		(WORD) d2 is sector number to be translated
	/ Outputs:	Return translated sector.
	/ Actions:	Perform slave BIOS sector
	/		translation call
	*/
	case tsreq:			/* Fall through to next case	*/


	/*
	/--------------------------------
	/				|
	/	Function 19:		|
	/	Get I/O byte		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	Returns IOBYTE value from slave.
	/ Actions:	Receive byte from master and
	/		store in slave location IOBYTE.
	*/
	case gireq:			/* Fall through to next case	*/
	
	/*
	/--------------------------------
	/				|
	/	Function 21:		|
	/	Flush disk buffer 	|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	None
	/ Actions:	Send request to slave
	/
	*/
	case fbreq:			/* Fall through to next case	*/

	/* All cases which return status or data	*/
	/* end up falling through to here. Request 	*/
	/* passed on to the slave BIOS and a long	*/
	/* parameter is returned to the master.		*/

		return(snd_rcv(d0, d1, d2));


	/*
	/--------------------------------
	/				|
	/	Function 0:		|
	/	System Init		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	None
	/ Actions:	Initialize Master.
	/		Just print message, so far.
	*/
	case inreq:
		break;

	/*
	/--------------------------------
	/				|
	/	Function 1:		|
	/	Warm Boot		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	None
	/ Actions:	Warm boot master.
	*/
	case wbreq:
		wboot();
		break;

	/*
	/--------------------------------
	/				|
	/	Function 3:		|
	/	Console Input		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	returns console character.
	/ Actions:	Input character from
	/		slave console to master.
	*/
	case cireq:
		while(!_bios(csreq, 0L, 0L));
		return(snd_rcv(d0, d1, d2));

	/*
	/--------------------------------
	/				|
	/	Function 9:		|
	/	Select Disk		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	Pointer to DPH for selected drive.
	/		Null if illegal drive.
	/ Actions:	Select drive number from master on slave.
	/		Return condition code from slave to master. 
	/		Pass selected drive's DPB from slave to master,
	/		if drive selected.
	*/
	case sdreq:

		{
		  BYTE code;
		  LONG temp;
		  DPH *dphptr;
		  DPB *dpbptr;
		  do
		 	{
		  	  code = snd_cmd(d0, d1, d2);
			  if (code == chsuc)
			    code = rcv_long(&temp);
			  if (code == chsuc)
			    if ((DPH *)temp != NULL)
				{
			  	  dphptr = &dphtab[d1];
				  dpbptr = dphptr -> dpbp;
				  if (!((BYTE)d2 & 0x1))
					{
					  code = rcv_long(&temp);
					  if (code == chsuc)
					    (dphptr -> xltp) = (BYTE *)temp;
					  if (code == chsuc)
					    code = rcv_word(&(dpbptr -> spt));
					  if (code == chsuc)
					    code = rcv_byte(&(dpbptr -> bsh));
					  if (code == chsuc)
					    code = rcv_byte(&(dpbptr -> blm));
					  if (code == chsuc)
					    code = rcv_byte(&(dpbptr -> exm));
					  if (code == chsuc)
					    code = rcv_word(&(dpbptr -> dsm));
					  if (code == chsuc)
					    code = rcv_word(&(dpbptr -> drm));
					  if (code == chsuc)
					    code = rcv_byte(&(dpbptr -> al0));
					  if (code == chsuc)
					    code = rcv_byte(&(dpbptr -> al1));
					  if (code == chsuc)
					    code = rcv_word(&(dpbptr -> cks));
					  if (code == chsuc)
					    code = rcv_word(&(dpbptr -> off));
					}
				  temp = (LONG)dphptr;
				}
			    else temp = (LONG)NULL;
			  if (code != chsuc)
				{
				  chn_err(sdreq, code);
				  temp = (LONG)cherr;
				}
			}
			while (retry && (code != chsuc));
		  return(temp);
		}

	/*
	/--------------------------------
	/				|
	/	Function 13:		|
	/	Read Sector		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	Read completion status
	/ Actions:	Receive a 128 byte sector specified by
	/		current drive, sector, track from slave
	/		and place it at the current DMA add-
	/		ress.
	*/
	case rsreq:

		{
		  BYTE stat, code;
		  LONG temp;
		  do
			{
			  code = snd_cmd(d0, d1, d2);
			  if (code == chsuc)
			    code = rcv_byte(&stat);
			  if ((stat == rssuc) && (code == chsuc)) 
			    code = rcv_data(&secbuf[0], seclen);
			  if (code == chsuc)
			    temp = (LONG)stat;
			  else
				{
				  chn_err(d0, code);
				  temp = cherr;
				}
			}
			while (retry && (code != chsuc));
		  if (temp == rssuc)
		    mem_cpy(map_adr((LONG)secbuf, 0),(LONG)dmaadr,(LONG)seclen);
		  return(temp);
		}

	/*
	/--------------------------------
	/				|
	/	Function 14:		|
	/	Write Sector		|
	/				|
	/--------------------------------
	/ Inputs:	Write type.
	/ Outputs:	Write sector completion status.
	/ Actions:	Send a 128 byte sector from current DMA
	/		address to slave.
	/		Receive a 1-byte completion code, in-
	/		dicating if write successful or not.
	*/
	case wsreq:

		{
		  BYTE stat, code;
		  LONG temp;
		  do
			{
			  mem_cpy((LONG)dmaadr,map_adr((LONG)secbuf,0),(LONG)seclen);
			  code = snd_cmd(d0, d1, d2);
			  if (code == chsuc)
			    code = snd_data(&secbuf[0], seclen);
			  if (code == chsuc)
			    code = rcv_byte(&stat);
			  if (code == chsuc)
			    temp = (LONG)stat;
			  else
				{
				  chn_err(d0, code);
				  temp = cherr;
				}
			}
			while (retry && (code != chsuc));
		  return(temp);
		}

	/*
	/--------------------------------
	/				|
	/	Function 12:		|
	/	Set DMA Address		|
	/				|
	/--------------------------------
	/ Inputs:	(LONG) d1 contains DMA address.
	/ Outputs:	None
	/ Actions:	Set DMA address from master.
	*/
	case sareq:

		dmaadr = d1;
		break;

	/*
	/--------------------------------
	/				|
	/	Function 17:		|
	/	Get Memory Table 	|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	Return pointer to memory region table
	/ Actions:	None
	*/
	case gmreq:
		return((long)&memtab);
	
	/*
	/--------------------------------
	/				|
	/	Function 22:		|
	/	Set Exception Vector 	|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	None
	/ Actions:	Currently unused
	/
	*/
	case svreq:
		{
		  LONG oldval;
		  oldval = trapvec[(int)d1];
		  trapvec[(int)d1] = d2;
		  return(oldval);
		}

	/*
	/--------------------------------
	/				|
	/	Illegal Request		|
	/				|
	/--------------------------------
	/ Inputs:	None
	/ Outputs:	None
	/ Actions:	Currently unused
	/
	*/

	default:
		ill_cmd();		/*	Illegal command		*/
		break;

     }	/* End of Switch	*/
}	/* End of BIOS		*/
