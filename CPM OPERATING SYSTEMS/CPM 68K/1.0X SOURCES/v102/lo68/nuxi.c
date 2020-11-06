#include "cout.h"
long	lseek();
main(argc,argv)
	int	argc;
	char	**argv;
{
	register i,j,c,d;
	long	symseek;			/* Symbol table seek addr   */
	int	symlength;			/* Symbol length (bytes)    */
	char	symbuf[30000];			/* Symbol table buffer  */
	int	fd;				/* File descriptor	*/

	if((fd=open(argv[1],2,1)) < 0)		/* Open input file	    */
	{
	  printf("Cannot open %s\n",argv[1]);	/* Can't		    */
	  exit();				/* Just quit		    */
	}					/****************************/

	if((i=read(fd,&couthd,HDSIZE)) != HDSIZE) /*	Read file header    */
	{					/*	Can't		    */
	  printf("Header read error\n");	/* Print message	    */
	  exit();				/* Then quit		    */
	}					/****************************/

	symseek=couthd.ch_tsize+couthd.ch_dsize+HDSIZE;
	if(couthd.ch_magic != MAGIC)
	{
	  if(couthd.ch_magic == (MAGIC+1))
	  {
		symseek += 8;
	  }
	  else
	  {
	    printf("File format error\n");
	    exit();
	  }
	}

	symlength = lseek(fd,symseek,0);	/* Seek to sym begin	  */
	if(symlength != symseek)		/* OK?			  */
	{
		printf("Seek error\n");		/*No, quit		  */
		exit();				/*			  */
	}

	symlength = couthd.ch_ssize;		/* Fetch symbol length	  */

	if(read(fd,&symbuf[0],symlength) != symlength)
	{
	  printf("Symbol table read error\n");
	}

	for(i=0; i < symlength; i+=14)
	{
	  for(j=0; j < 8; j+=2)
	  {
	     c = symbuf[j+i];
	     d = symbuf[j+i+1];
	     symbuf[j+i] = d;
	     symbuf[j+i+1] = c;
	  }
	}

	if(lseek(fd,symseek,0) != symseek)
	{
	  printf("Seek error\n");
	  exit();
	}
	if(write(fd,&symbuf[0],symlength) != symlength)
	{
	  printf("Symbol table write error\n");
	  exit();
	}
	close(fd);
	exit(0);
}
