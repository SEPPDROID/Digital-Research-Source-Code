/*
 *	COMDEX El-kludgo display program.  
 *
 *	disp file d
 *
 *	Where "file" is a text file containing 24 x 64 pages, delineated by
 *	"`" characters.  "d" is the number of seconds to delay.
 *
 */
#include	"stdio.h"
FILE	*fopen();
main(argc,argv)
	WORD	  argc;
	BYTE	**argv;
{						/****************************/
	BYTE	 buffer[2048];			/* Buffer for screen text   */
REG	FILE	*ifp;				/* Input file pointer	    */
REG	WORD	delay;				/* Delay for pages	    */
REG	BYTE	c;				/* Character temporary	    */
REG	WORD	i,k,l;				/* Count temporary	    */
						/****************************/
	if(argc != 3)				/* Check the arg count	    */
	{					/*			    */
	  printf("Bad Argument count\n");	/* Give error		    */
	  exit();				/* Quit			    */
	}					/****************************/
						/*			    */
	if((ifp=fopen(argv[1],"r")) <= 0)	/* Open Input		    */
	{					/*			    */
	  printf("Cannot open %s\n",argv[1]);	/* Give error		    */
	  exit();				/* Quit in disgust	    */
	}					/*			    */
						/****************************/
	delay=atoi(argv[2]);			/* Convert delay quantity   */
	if(delay < 0 || delay > 10000)		/* Range check		    */
	{					/*			    */
	  printf("Illegal delay = %d\n",delay);	/* Print delay nfg	    */
	  exit();				/* Quit			    */
	}					/****************************/
						/*			    */
while(1)					/* Always		    */
{						/*			    */
	i = 0;					/* Subscript init	    */
	c = 1;					/* Initialize char	    */
	while(c != '`' && c > 0)		/* While c in page & not EOF*/
	{					/****************************/
	  c = getc(ifp);			/* Read next character	    */
	  buffer[i++] = c;			/* Put next character in buf*/
	}					/****************************/
	i--;					/* Decrement count back to 0*/
	l = 0;					/* Initially no lfs in buff */
	for(k=0; k <= i; k++)			/* Count line feeds	    */
		if(buffer[k] == '\n') l++;	/* L = # line feeds	    */
	k = 23 - l;				/* K = # line feeds desired */
	write(1,buffer,i);			/* Write buffer to stdout   */
	loop(delay);				/* Do the delay		    */
						/****************************/
	if(c <= 0)				/* Check for EOF	    */
		fseek(ifp,0L,0);		/* Seek back to beginning   */
}						/****************************/
}
loop(delay)					/* Delay loop		    */
	WORD	delay;				/* # counts		    */
{						/****************************/
REG	WORD	i;				/* Inner loop count	    */
	while(delay > 0)			/****************************/
	{					/*			    */
	  for (i = 0; i < 10000; i++);		/* Do nothing		    */
	  delay--;				/* Down count		    */
	}					/****************************/
}
