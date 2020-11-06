/*----------------------------------------------------------------------*\
 |	serial.c						v1.0	|
 |		  SERIALIZATION PROGRAM FOR CP/M-68K			|
 |		  ==================================			|
 |									|
 |	date:		1/3/83						|
 |	----								|
 |									|
 |	last modified:	2/3/83						|
 |	-------------							|
 |									|
 |	description:							|
 |      -----------	This program replaces up to 20 occurances       |
 |			of "654321" in all files found in "SERIAL.DAT". |
 |			The string "654321" is replaced by a serial	|
 |			number.  Leading zeros will be filled in by	|
 |			the program.  The starting and ending serial	|
 |			number's are read in from the command line.	|
 |			The user is prompted for the serialization	|
 |			drive.						|
 |									|
 |									|
 |	COPYRIGHT (C) 1983						|
 |	DIGITAL RESEARCH						|
 |	BOX 579								|
 |	PACIFIC GROVE, CA. 93950					|
 |									|
\*----------------------------------------------------------------------*/


#include	"stdio.h"		/* standard I/O library		*/

#define		islower(c)	('a' <= (c) && (c) <= 'z')
#define		toupper(c)	(islower(c) ? ((c)-040) : (c))
#define		RESET		13
#define		SELECT		14

					/********************************/
					/* GLOBAL VARIABLES		*/
struct _serial 				/********************************/
{					/*				*/
	BYTE filename[13];		/* file to serialize		*/
};					/*				*/
struct _serial serial[150];		/* array of structures		*/
					/*				*/
BYTE enddata[] = "xxxxxxxxxxxx";	/* end of filenames marker	*/
BYTE marker[] = "654321";		/* place in file to sub serial #*/
BYTE snum[] = "000000";			/* serial number		*/
BYTE drive = 0;				/* serialization drive		*/
BYTE chtab[] = "0123456789";		/* table of digits		*/
FILE *stream;				/* pointer to a stream file	*/
					/********************************/



					/********************************/
					/* FUNCTIONS			*/
					/********************************/
FILE *fopen();				/* returns a stream I/O pointer */
FILE *fopenb();				/* returns a stream I/O pointer */
BYTE *trans();				/* returns a pointer to a string*/
LONG atol();				/* returns a long number	*/
LONG ftell();				/* returns a long number	*/
					/********************************/

main(argc,argv)
WORD argc;
BYTE *argv[];
{
	REG LONG	start_num;	/* starting serial number	*/
	REG LONG	final_num;	/* final serial number		*/
	REG LONG	num;		/* current serial number	*/
	BYTE 		buffer[32];	/* read buffer			*/
	BYTE		boot;		/* y or n to system tracks	*/


	if(argc != 3)
	{
		printf("\nUsage:  A>serial starting ending");
		exit(0);
	}

	printf("Serial.68K  V1.0");

	init();			/* try to open data file	*/

	boot = 'N';		/* no serializing of system trks*/

	/* check range of serial number's */

	if(atol(argv[1]) > 999999L || atol(argv[2]) > 999999L ||
	   atol(argv[1]) < 0L      || atol(argv[2]) < 0L)
	{
		printf("\nERROR:  Serial # out of range: [0-999999]");
		exit(0);
	}

	printf("\nSerialization drive ? "); gets(buffer);
	drive = toupper(buffer[0]);

	/* verify to user the serial number range */

	printf("\nSTARTING serial number is:  %s",argv[1]);
	printf("\nFINAL serial number is   :  %s",argv[2]);
	if(boot == 'Y')
		printf("\nSystem tracks will be serialized");
	printf("\n\n");


	/************************************************/
	/*						*/
	/*	     MAIN SERIALIZATION LOOP		*/
	/*	     -----------------------		*/
	/*						*/
	/************************************************/


	for(num = atol(argv[1]); num <= atol(argv[2]); num++)
	{
	     __BDOS(RESET,(long)0);
	     printf("\n\nInsert new diskette in drive %c and type RETURN",drive);
	     getchar();
	     printf("\nCURRENT serial number is:  %LD\n",num);	
	     if(boot == 'Y')
		    boottrk(num);	
	     patch(num);
	}
}

						/************************/
VOID init()					/*   Open SERIAL.DAT	*/
						/*   Read SERIAL.DAT	*/
						/************************/
{
	BYTE buf[40];
	REG UWORD j;
	REG struct _serial *p;

	if((stream = fopen("SERIAL.DAT","r")) == NULL)
	{
		printf("\nUnable to open SERIAL.DAT");
		exit(0);
	}
	/* read in the data line by line */
	/* and fill the structure serial */
	fgets(buf,40,stream);
	p = &serial;
	do
	{
		j = 0;
		while((p->filename[j] = buf[j++]) != '\n');
		p->filename[j-1] = '\0';
		p++;
		fgets(buf,40,stream);
	}
	while(!(feof(stream)));
	for(j = 0;j < 12;j++)
		p->filename[j] = 'x';
	p->filename[12] = '\0';
	fclose(stream);
}
						/************************/
VOID patch(num)					/*  Plug Serialization  */
						/*  bytes into file     */
						/************************/
LONG num;
{
	REG struct _serial *p;
	REG UWORD i,j;
	LONG offset[21];
	REG BYTE *pp;
	REG UWORD ch;
  	REG WORD fd;


	__BDOS(SELECT,(long)drive-'A');
	p = &serial;
	while(strcmp(p,enddata))
	{
		printf("\nSerializing:  %s   ",p); 
		for(i = 1;i <= (12 - strlen(p));i++)
			printf(" ");
		if((stream = fopenb(p,"r")) == NULL)
			printf("Unable to open file:  %s",p);
		else
		{
			for(j = 0;j < 21;j++)
				offset[j] = -1L;
			i = j = 0;
			pp = marker;
			rewind(stream);
			while(1)
			{
				ch = fgetc(stream);
				if(feof(stream))
					break;
				if(ch == *pp)
				{
					i++;
					if(pp == marker)
						offset[j] = ftell(stream);
					pp++;
				}
				else
				{
					pp = marker;
					offset[j] = -1L;
					i = 0;
				}
				if(i == 6)
				{
					j++;
					if(j == 20)
						break;
					i = 0;
					pp = marker;
				}
			}
			fclose(stream);
			if(offset[0] == -1L)
				printf("file cannot be serialized");
			else
			{
				fd = openb(p, 2);
				if ( fd == -1 ) printf("cannot write to file");
				else
				{
				   j = 0;
				   while(offset[j] != -1L)
				   {
					lseek(fd, --offset[j++], 0);
					write(fd, trans(num), 6);
				   }
				printf("Serialization complete   %d",j);
				close(fd);
				}
			}	
		}
		p++;
	}
}

VOID boottrk(num)
LONG num;
{
	printf("\nSerializing Boot tracks");
}



BYTE *trans(num)
LONG num;
{
	REG BYTE *p;
	LONG x;

	p = snum;
	x = 100000;

	while(x)
	{
		*p++ = chtab[num/x];
		num %= x;
		x /= 10;
	}

	return(snum);
}

