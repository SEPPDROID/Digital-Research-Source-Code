#define	MAXNUM	1266
main()
{
	int	fd;				/* Out descriptor	*/
	int	i;
	int	length;
	short	buffer[MAXNUM];
	short	chk[MAXNUM];
	if((fd=creat("test.dat",0,1)) < 0)
	{
		printf("Cannot create test.dat\n");
		exit();
	}
	for(i=0; i<MAXNUM; i++)
		buffer[i] = i;			/* Unique tag		*/

	if(write(fd,&buffer[0],2*MAXNUM/3) != 2*MAXNUM/3)
	{
		printf("Write error 1\n");
		exit();
	}

	if(write(fd,&buffer[MAXNUM/3],2*MAXNUM/3) != 2*MAXNUM/3)
	{
		printf("Write error 2\n");
		exit();
	}

	if(write(fd,&buffer[(2*MAXNUM)/3],2*MAXNUM/3) != 2*MAXNUM/3)
	{
		printf("Write error 3\n");
		exit();
	}
	if(lseek(fd,0L,0) != 0)
	{
		printf("Seek error\n");
	}
	close(fd);
	if((fd=open("test.dat",0,1)) < 0)
	{
		printf("Cannot re-open test file\n");
		exit();
	}
	for(i=0; i<MAXNUM; i++)
	{
		if(read(fd,&chk[i],2) != 2)
		{
			printf("Read failure at offset=%d\n",i*2);
		}
	}
	for(i=0; i<MAXNUM; i++)
	{
		if(chk[i] != i)
			printf("Compare error file=%4d i=%4d\n",buffer[i],i);
	}
	exit();
}
