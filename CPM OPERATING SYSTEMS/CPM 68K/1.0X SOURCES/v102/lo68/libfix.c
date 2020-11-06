/*
 *	Fix library files on CP/M - 68k
 */
#define	LNAMLEN	14
#define	LIBMAGIC 0177545
struct lib2hdr {
	char	fname[LNAMLEN];		/* Original file name		    */
	long	modti;			/* Modification (unused)	    */
	char	userid;			/* User id			    */
	char	gid;
	short	fimode;
	long	fsize;		/* file size (Swap words here	    */
	short	junk;			/* Unused			    */
} header;

struct {
	short	hiword;			/* PDP-11 is			    */
	short	loword;			/*		backwards	    */
};
long	lseek();
main(argc,argv)
	int	argc;
	char	**argv;
{
	short	i,j;			/* Temps			    */
	long	seekaddr;		/* Seek pointer		 	    */
	long	writeaddr;		/* Write seek pointer		    */
	short	fd;			/* File descriptor for I/O 	    */
	short	ofd;			/* File descriptor for output	    */
	short	magic;			/* Lib magic word		    */
	char	buffer[30000];		/* I/O Buffer for file		    */

	if(argc != 3)
	{
	  printf ("File count wrong\n");
	  exit();
	}
	if((fd=open(argv[1],0,1)) < 0)
	{
	  printf("Open failure on %s\n",argv[1]);
	  exit();
	}
	if((ofd=creat(argv[2],1,1)) < 0)
	{
	  printf("Cannot create %s\n",argv[2]);
	  exit();
	}

	if(read(fd,&magic,2) != 2)
	{
	  printf("Read error\n");
	  exit();
	}
	if(magic != LIBMAGIC)
	{
	  printf("Not archive format: %s\n",argv[1]);
	  exit();
	}

	if(write(ofd,&magic,2) != 2)
	{
	  printf("Write error\n");
	  exit();
	}
while(1)
{
	if(read(fd,&header,(sizeof header)-2) != (sizeof header)-2)
	{
	  printf("Read error at file offset %lx\n",writeaddr);
	  exit();
	}

	for(i=0; i<LNAMLEN; i+=2)
	{
	  j = header.fname[i];
	  header.fname[i] = header.fname[i+1];
	  header.fname[i+1] = j;
	}
	header.modti = 0L;
	header.userid = 0;
	header.gid = 0;
	header.fimode = 0777;
	header.junk = 0;
	printf("File %14s: %ld bytes\n",&header.fname[0],header.fsize);


	if(write(ofd,&header,(sizeof header)) != (sizeof header))
	{
	  printf("Write error\n");
	  exit();
	}

	if(header.fsize > 30000)
	{
	  printf("File size error\n");
	  exit();
	}

	if(read(fd,&buffer[0],header.fsize.loword) != header.fsize.loword)
	{
	  printf("Read error\n");
	  exit();
	}

	if(write(ofd,&buffer[0],header.fsize.loword) != header.fsize.loword)
	{
	  printf("Write error\n");
	  exit();
	}
}
}	  	  
