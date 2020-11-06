/*----------------------------------------------------------------------*\
 |									|
 |			     DISK COPY PROGRAM				|
 |			     =================				|
 |									|
 |	Created:	November 4,1982					|
 |	-------								|
 |									|
 |	Last Modified:	January 15,1982					|
 |	-------------							|
 |									|
 |	Description:							|
 |	-----------	This program will copy all or portions of	|
 |		        one disk to another compatible disk.		|
 |									|
 |	Needed Files:							|
 |	------------	<STDIO.H>,<PORTAB.H>				|
 |									|
 |	Must be linked to the 'C' runtime library.			|
 |	------------------------------------------			|
 |									|
 |	(c)	COPYRIGHT DIGITAL RESEARCH 1983				|
 |		ALL RIGHTS RESERVED					|
 |									|
\*----------------------------------------------------------------------*/

char *copyrt="CP/M-68K(tm), Version 1.2, Copyright 1984, Digital Research";
char *serial="XXXX-0000-654321";

/************************************************************************\
 *	               Here are the Include Files			*
\************************************************************************/
					/********************************/
#include	<stdio.h>		/* Standard I/O library		*/
					/********************************/
/************************************************************************\
 *			   DEFINES for Copy.c				*
\************************************************************************/
					/********************************/
#define		ON	1		/* Option is valid for this copy*/
#define		OFF	0		/* Option is not valid		*/
#define		BAD	0		/* Illegal mode flag		*/
#define		MATCH	0		/* used by strcmp		*/
#define		SDRIVE  1		/* source drive			*/
#define		DDRIVE  2		/* destination drive		*/
#define		_ALL	0		/* copy whole disk		*/
#define		_BOOT	1		/* copy boot tracks		*/
#define		_FILES  2		/* copy non-boot tracks		*/
#define		_END	3		/* end this program		*/
#define		RDERR	0		/* read error flag		*/
#define		WRERR	1		/* write error flag		*/
#define		NULLB	'\0'		/* NULL byte			*/
#define		isupper(c)	('A' <= (c) && (c) <= 'Z')/*upper case  */
#define		islower(c)	('a' <= (c) && (c) <= 'z')/*lower case	*/
#define		tolower(c)	(isupper(c) ? ((c)+040):(c))/*		*/
#define		toupper(c)	(islower(c) ? ((c)-040):(c))/*		*/
					/********************************/
/************************************************************************\
 *		           GLOBAL definitions				*
\************************************************************************/
					/********************************/
GLOBAL BYTE *choice[] =			/* Copy Modes available		*/
{"all","boot","files","end"};		/* ----------------------	*/
					/********************************/
GLOBAL BYTE mode;			/* Hold the current copy mode	*/
					/********************************/
					/*   ------- OPTIONS ------- 	*/
GLOBAL BYTE verbose = ON;		/* Prompt the user if ON	*/
GLOBAL BYTE verify  = OFF;		/* Check newly written track 	*/
					/* againist source track.	*/
					/********************************/
					/* Prompts...........		*/
GLOBAL BYTE *msg1 = "Enter SOURCE drive: ";		/* SDRIVE	*/
GLOBAL BYTE *msg2 = "Enter DESTINATION drive: ";	/* DDRIVE	*/
GLOBAL BYTE *msg3 = "READ error ---->    ";		/* Read error	*/
GLOBAL BYTE *msg4 = "WRITE error ---->    ";		/* Write error	*/
					/********************************/
					/********************************/
GLOBAL BYTE *bracket = "[";		/* Marks beginning of option 	*/
					/* indicators on the cmd line	*/
					/********************************/
GLOBAL struct _dpb			/* Define disk parm block	*/
{					/*------------------------------*/
	WORD	spt;			/* Logical sectors per track	*/
	BYTE	bsh;			/* Allocation block shift factor*/
	BYTE	blm;			/* Block mask			*/
	BYTE	exm;			/* Extent mask			*/
	BYTE	dpbjunk;		/* Alignment byte		*/
	WORD	dsm;			/* Total storage of the disk	*/
	WORD	drm;			/* Total directory entries	*/
	BYTE	al0;			/* Flag reserved for dir blocks	*/
	BYTE	al1;			/* Flag reserved for dir blocks */
	WORD	cks;			/* Directory check vector size	*/
	WORD	off;			/* The number of reserved tracks*/
};					/*				*/
					/********************************/
GLOBAL struct _dph			/* Define disk parm header	*/
{					/*------------------------------*/ 
	BYTE	*xltp;			/* Pointer to sector trans table*/
	WORD	dphscr[3];		/* Scratchpad values for BDOS	*/
	BYTE	*dirbufp;		/* Pointer to dir scratchpad	*/
	struct	_dpb	*dpbp;		/* Pointer to disk parm block	*/
	BYTE	*csvp;			/* Pointer to scratchpad area	*/
	BYTE	*alvp;			/* Pointer to scratchpad area	*/
};					/********************************/
EXTERN LONG __BDOS();			/* BDOS Caller in CLIB		*/
#define bdos(a,b) (struct _dph*)__BDOS(a,b) /* Declare a bogus function */
					/********************************/
GLOBAL struct _biospb			/* Bios Pararmeter Block	*/
{					/*------------------------------*/
	UWORD	funcnum;		/* Bios function number		*/
	LONG	parm1;			/* Bios parameter 1		*/
	LONG	parm2;			/* Bios parameter 2		*/
};					/*				*/
GLOBAL struct _biospb biospb;		/* Define it			*/
					/********************************/
GLOBAL BYTE trkbuf[128*26*6];		/* buffer big enough to hold 6	*/
					/* tracks of bytes		*/
GLOBAL BYTE verbuf[128*26*6];		/* buffer for verification opt	*/
					/********************************/
/************************************************************************\
 *			 FUNCTION definitions				*
\************************************************************************/
					/********************************/
UWORD illegal();			/* Decides if the mode is a	*/
					/* legal one.			*/
					/*------------------------------*/
VOID get_mode();			/* If the mode was not there  	*/
					/* or an illegal one,get_mode 	*/
					/* will print a menu and keep	*/
					/* prompting the user for a 	*/
					/* legal mode.			*/
					/*------------------------------*/
BYTE get_drive();			/* Reads in a drive character	*/
					/* from the console.  If the	*/
					/* drive is in the range(A-P),	*/
					/* the drive is returned.  If 	*/
					/* not,the value BAD is returned*/
					/*------------------------------*/
VOID wait_for();			/* Holds the copy program in a  */
					/* wait state so the user can   */
					/* make sure his or her disk is */
					/* in the proper drive.		*/
					/*------------------------------*/
BYTE scan();				/* Picks off the source and 	*/
					/* destination drives from the	*/
					/* command line.  If they do	*/
					/* not exist,or the specified	*/
					/* drives are illegal,scan 	*/
					/* prompts the user.		*/
					/*------------------------------*/
VOID get_options();			/* Scans the command line until */
					/* a '[' is found.  A list of	*/
					/* of options is checked. Flags	*/
					/* are turned on by the option	*/
					/* indicators found after the	*/
					/* square bracket.		*/
					/*------------------------------*/
VOID copy();				/* If the two disk parameter	*/
					/* blocks are identical,a track */
					/* for track copy is made.  	*/
					/* If an serious error occurs,	*/
					/* the copy is aborted and the	*/
					/* program ends.		*/
					/********************************/
VOID read_wr();				/* This routine does the actual */
					/* reading and writing of the	*/
					/* compatible disks.		*/
					/********************************/
/************************************************************************/

/************************************************************************\
 *									*
 *			   BEGIN MAIN PROGRAM				*
 *									*
\************************************************************************/





main(argc,argv)
WORD	argc;
BYTE	*argv[];
{
	BYTE answer,source,dest,optfound;

	optfound = FALSE;
	printf("Copy Ver 1.2\n");	/* Copy version message		*/
	/*------------------------------*\
         |  If there is an mode on	|
	 |  on the command line or the  |
         |  command line mode is not  	|
         |  recognized,print a menu	|
	 |  and have the user input a	|
	 |  a legal copy mode.		|
	\*------------------------------*/	
	argc--; argv++;			/* point to first arg after cmd	*/
	if(illegal(*argv))
		get_mode();
	else				/* point to 2nd arg past cmd	*/
	{
		argv++;
		argc--;
	}
	if(mode == _END)		
		cpyexit();
	/*------------------------------*\
	 |   Look for the '[' as the	|
	 |   start of the options.	|
	 |   This allows the following  |
	 |				|
	 |   copy <mode> [options	|
	 |   ====================	|
	 |   or				|
	 |   copy [options		|
	 |   =============		|
	 |				|
	 |   I will not allow the drives|
	 |   to be specified after the  |
	 |   options though!!!!!	|
	\*------------------------------*/	
	if(strncmp(bracket,*argv,1) == MATCH)
	{
		argc = 0;
		get_options(*argv++);
		source = scan(argc,*argv++,SDRIVE);
		dest = scan(argc,*argv,DDRIVE);
	}	
	else
	/*------------------------------*\
	 |  Otherwise I assume that the |
	 |  command line looks like 	|
	 |  this:			|
	 |				|
	 |  copy <mode> <sd> <dd> [opt  |
	 |  ==========================	|
	\*------------------------------*/
	{
		source = scan(argc--,*argv++,SDRIVE);
		dest = scan(argc--,*argv,DDRIVE);
		if(strncmp(bracket,*argv,1) == MATCH)
		{
			optfound = TRUE;
			get_options(*argv);
		}
		while(argc-- > 0)
			/* scan cmd line to find a '['	*/
			if(strncmp(bracket,*++argv,1) == MATCH &&
			(!optfound))
			{
				get_options(*argv);
				break;
			}
			else
			printf("\nExtraneous argument ignored: %s",*argv);
	}
	if(source == dest)
	{
		printf("\nSource and Destination must be different");
		cpyexit();
	}
	/*------------------------------*\
	 |    REPEAT the copy as many	|
	 |    times as the user wishes  |
	\*------------------------------*/
	while(TRUE)
	{
		if(verbose)
			wait_for(source,dest);
		copy((toupper(source) - 'A'),(toupper(dest) - 'A'));
		printf("\nCopy complete");
		if(verbose)
		printf("\nDo you wish to repeat the copy? ");
		else
			break;
		answer = __BDOS(1,(long)0);
		if(toupper(answer) != 'Y')
			break;
	}
}		

						/************************/
UWORD illegal(_mode)				/*  Check command line  */
						/*  aganist mode table  */
						/*  Return mode code if */
						/*  found,return false  */
						/*  if the mode is not  */
						/*  found in the table  */
						/************************/ 
BYTE *_mode;
{
	REG UWORD i;

	for(i = 0;i <= 3;i++)
		if(strcmp(_mode,choice[i]) == MATCH)
		{
			mode = i;
			return(FALSE);
		}
	mode = -1;
	return(TRUE);
}
 						/************************/
VOID get_mode()					/*  Prompt the user for */
						/*  a legal mode.  Keep */
						/*  prompting until you */
						/*  get one.		*/
						/************************/
{
	REG UWORD i;
	BYTE buf[256];

	printf("\nMODE\tFUNCTION\n");
	printf("\nALL\tCopy the whole disk");
	printf("\nBOOT\tCopy the boot tracks");
	printf("\nFILES\tCopy the non-boot tracks");
	printf("\nEND\tEnd this program\n");
	do
	{
		printf("\nEnter your copy mode: ");
		gets(buf);
		for(i = 0;buf[i] != NULLB;i++)
			buf[i] = tolower(buf[i]);
	}
	while(illegal(buf));
}

						/************************/
BYTE get_drive()				/*  Read in a drive.	*/
						/************************/ 
{
	BYTE drive[10];
	
	gets(drive);
	if(strlen(drive) > 1)
		return(BAD);
	drive[0] = toupper(drive[0]);
	if(drive[0] < 'A' || drive[0] > 'P')
		return(BAD);
	else
		return(drive[0]);
}
						/************************/
VOID wait_for(source,dest)			/*  Wait for the user   */
						/*  to put the disks    */
						/*  in the proper drives*/
						/************************/
BYTE source;
BYTE dest;
{
	BYTE wait_buf[5];
	BYTE *upchoice[3];

	upchoice[0] = "ALL";
	upchoice[1] = "BOOT";
	upchoice[2] = "FILES";
	
	wait_buf[0] = 3;
	do
	{
		printf("\n(^C to ABORT)");
		printf("\nRETURN to copy %s",upchoice[mode]);
		printf(" from %c to %c",source,dest);
		__BDOS(10,&wait_buf[0]);
	}
	while(wait_buf[1] != 0);
}

						/************************/
BYTE scan(argc,argv,which_drive)		/*  Scan the command    */
						/*  for the drives.     */
						/*  If I can't find them*/
						/*  or they are illegal */
						/*  keep prompting the  */
						/*  user for them.	*/
						/************************/ 
WORD argc;
BYTE *argv;
UWORD which_drive;
{
	BYTE *prompt,drive;

	if(argc > 0 && strlen(argv) == 1)
	{
		sscanf(argv,"%c",&drive);
		drive = toupper(drive);
		if(drive >= 'A' && drive <= 'P')
			return(drive);
	}
	switch(which_drive)
	{
		case SDRIVE :  	prompt = &msg1[0];
				break;
		case DDRIVE :	prompt = &msg2[0];
	}
	printf("\n");
	do
	{
		printf("%s",prompt);
	}
	while((drive = get_drive()) == BAD);
	
	return(drive);
}
						/************************/
VOID get_options(argv)				/*  Scan cmd line for   */
						/*  supported options   */
						/************************/
BYTE *argv;
{
	argv++;		/* skip the square bracket */
	while(*argv != NULLB)
	{
		switch(*argv)
		{
			case 'A' :
			case 'a' :
					verbose = OFF;
					break;
			case 'v' :
			case 'V' :	verify = ON;
					break;
			case ']' :      return(0);
			default  :	printf("\nUndefined option: %c",*argv);
		}
		argv++;
	}
	
}
						/************************/
VOID copy(source,dest)				/*  Select the disks.	*/
						/*  If I can't select or*/
						/*  the disk have diff- */
						/*  erent parameter blks*/
						/*  then print error msg*/
						/************************/
BYTE source;
BYTE dest;
{
	REG struct _dpb *dskpblk;
	REG struct _dph *h1,*h2;
	REG LONG *ptr1,*ptr2;
	REG LONG endtrk,begtrk;
	LONG numtrks,temp;
	BYTE *sectran;
	REG UWORD i;

	
	biospb.funcnum = 9;		/* Select the disks		*/
	biospb.parm1 = (long)source;	/* first the source disk	*/
	biospb.parm2 = (long)0;		/* set least sig bit to 0	*/

	if((h1 = bdos(50,&biospb))!= NULL)
	{
		biospb.parm1 = (long)dest;
		if((h2 = bdos(50,&biospb))!= NULL)
		{	/*----------------------------------------------*/
			/* check the parameter blocks(must be identical)*/
			/*----------------------------------------------*/
			dskpblk = (*h1).dpbp; /* pointer to dpb		*/
			sectran = (*h1).xltp; /* pointer to sect tran tb*/
			h1 = (*h1).dpbp;      /* h1 points to dpb	*/
			ptr1 = (long)h1;      /* compare long words	*/
			h2 = (*h2).dpbp;      /* h2 points to dpb	*/
			ptr2 = (long)h2;      /* compare long words	*/
			for(i = 0;i <= 3;i++)
			{
				if(*ptr1 != *ptr2)
				{
					printf("\nIncompatible disks");
					cpyexit();
				}
				ptr1++;
				ptr2++;
			}
		}
		else
		{
			printf("\nDisk select error on Destination");
			cpyexit();
		}
	}
	else
	{
		printf("\nDisk select error on Source");
		cpyexit();
	}
	/*------------------------------------------------------*\
	 |							|
	 |	    Compute beginning and ending track		|
	 |							|
	\*------------------------------------------------------*/

	temp = (((*dskpblk).dsm + 1)*((*dskpblk).blm + 1));
	numtrks = temp/(*dskpblk).spt;
	if((numtrks*(*dskpblk).spt) < temp)
		numtrks++;
	numtrks += (*dskpblk).off;

	switch(mode)
	{
		case _ALL :	begtrk = 0;
				endtrk = numtrks - 1;
				break;
		case _BOOT:	begtrk = 0;
				endtrk = (*dskpblk).off - 1;
				break;
		case _FILES:	begtrk = (*dskpblk).off;
				endtrk = numtrks - 1;
	}
	read_wr(source,dest,begtrk,endtrk,sectran,dskpblk);
 
}


/************************************************************************/
/*									*/
/*		    Do the actual read write operations			*/
/*		    ===================================			*/
/*									*/
/************************************************************************/
	

VOID read_wr(source,dest,begtrk,endtrk,sectran,dskpblk)
					/********************************/
BYTE source;				/* source disk			*/
BYTE dest;				/* destination disk 		*/
REG LONG begtrk;			/* beginning track to copy	*/
REG LONG endtrk;			/* ending track	to copy		*/
REG BYTE *sectran;			/* sector translate table	*/
struct _dpb *dskpblk;			/* ptr to disk parameter block	*/
					/********************************/
{
	REG BYTE *dma;
	REG UWORD i,k;

	printf("\n***Copying Tracks***\n");
	for(i = begtrk;i <= endtrk;i++)
	{
		dma = &trkbuf;
		biospb.funcnum = 12;	/* set the dma address		*/
		biospb.parm1 = dma;	/* set it to beginning of trkbuf*/
		biospb.parm2 = 0;
		__BDOS(50,&biospb);	/* call the bios via the bdos	*/
		biospb.funcnum = 9;	/* select the source diskette	*/
		biospb.parm1 = (long)source;
		biospb.parm2 = (long)0xFFFFFFFF;
		__BDOS(50,&biospb);	/* call the bios via the bdos	*/
		biospb.funcnum = 10;	/* set the track	*/
		biospb.parm1 = (long)(i);
		biospb.parm2 = 0;
		__BDOS(50,&biospb);	/* call bios via bdos	*/
		/*----------------------------------------------*\
		 |        Read a track's worth of sectors	|
		\*----------------------------------------------*/
		for(k = 0;k < (*dskpblk).spt;k++)
		{
			biospb.funcnum = 16; /* translate sector*/
			biospb.parm1 = (long)k; /* logical sect	*/
			biospb.parm2 = sectran;
			biospb.parm1 = __BDOS(50,&biospb);
			biospb.funcnum = 11; /* set physical sec*/
			biospb.parm2 = 0;
			__BDOS(50,&biospb);  /* call the bios	*/
			/*--------------------------------------*\
			 |            READ a sector		|
			\*--------------------------------------*/
			biospb.funcnum = 13;
			if(__BDOS(50,&biospb) != 0)
				error(i,k,RDERR);
			dma += 128;
			biospb.funcnum = 12;
			biospb.parm1 = dma;
			__BDOS(50,&biospb);
		}
		dma = &trkbuf;
		biospb.funcnum = 12;	/* reset the dma addr to trkbuf	*/
		biospb.parm1 = dma;
		biospb.parm2 = 0;
		__BDOS(50,&biospb);	/* call the bios		*/
		biospb.funcnum = 9;	/* select the destination disk	*/
		biospb.parm1 = (long)dest;
		biospb.parm2 = (long)0xFFFFFFFF;
		__BDOS(50,&biospb);	/* call the bios		*/
		/*----------------------------------------------*\
		 |     Write out a Track's worth of Sectors	|
		\*----------------------------------------------*/
		for(k = 0;k < (*dskpblk).spt;k++)
		{
			biospb.funcnum = 16;
			biospb.parm1 = (long)k;
			biospb.parm2 = sectran;
			biospb.parm1 = __BDOS(50,&biospb);
			biospb.funcnum = 11;
			biospb.parm2 = 0;
			__BDOS(50,&biospb);
			/*--------------------------------------*\
			 |	     Write a Sector		|
			\*--------------------------------------*/
			biospb.funcnum = 14;
			biospb.parm1 = (long)2;
			if(__BDOS(50,&biospb) != 0)
				error(i,k,WRERR);
			dma += 128;
			biospb.funcnum = 12;
			biospb.parm1 = dma;
			__BDOS(50,&biospb);
		}
		if(verify)
		{
			dma = &verbuf;
			biospb.funcnum = 12;
			biospb.parm1 = dma;
			__BDOS(50,&biospb);	/* reset dma addr */
			for(k = 0;k < (*dskpblk).spt;k++)
			{
				biospb.funcnum = 16;
				biospb.parm1 = (long)k;
				biospb.parm2 = sectran;
				biospb.parm1 = __BDOS(50,&biospb);
				biospb.funcnum = 11;
				biospb.parm2 = 0;
				__BDOS(50,&biospb);
				/*------------------------------*\
				 |    Read a sector  		|
				\*------------------------------*/
				biospb.funcnum = 13;
				if(__BDOS(50,&biospb) != 0)
					error(i,k,RDERR);
				dma += 128;
				biospb.funcnum = 12;
				biospb.parm1 = dma;
				__BDOS(50,&biospb);
			}
			/*--------------------------------------*\
			 |	Verify the TRACK write		|
			\*--------------------------------------*/
			for(k = 0;k < (((*dskpblk).spt)*128);k++)
				if(trkbuf[k] != verbuf[k])
					error(i,(k/128),WRERR);
		}
		printf("\r         %d",i);
	}
}

						/************************/
error(track,sector,phase)			/*  Read/Write Error	*/
						/*  handler		*/
						/************************/
UWORD track;
UWORD sector;
BYTE phase;
{
	REG BYTE *addr;

	if(phase == RDERR)
		addr = &msg3[0];
	else
		addr = &msg4[0];
	printf("\n\n\n%s",addr);
	printf("Track:  %d   Sector:  %d",track,sector);
	cpyexit();
}

cpyexit()	/* bios select what the bdos thinks is the default disk */
{
	biospb.funcnum = 9;
	biospb.parm1 = __BDOS(25,(long)0);
	biospb.parm2 = 0;
	bdos(50,&biospb);
	exit(0);
}


           ospb.funcnum = 9;
	biospb.parm1 = __BDOS(25,(long)0);
	biospb.parm2 = 0;
	bdos(50,&biospb);
	exit(0);
}


           