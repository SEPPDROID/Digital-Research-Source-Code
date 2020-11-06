main(argc,argv)
	int	argc;
	char	**argv;
{
	char	buff[512];
	int	fd;
	int	ofd;
	int	i,j,k,l;
	int	bufcnt;
	int	xbytes;
	if(argc != 3)
	{
		printf("Illegal arg count = %d\n",argc);
		exit();
	}

	i = 1;
	while(argc > 1)
	{
		printf("Copying file %s to file %s\n",argv[i],argv[i+1]);
		fd=open(argv[i++],0,0);	/* Open read on ascii file */
		if(fd < 0)
		{
			printf("Cannot open file %s\n",argv[--i]);
			exit();
		}
		ofd=creat(argv[i],0,0);
		if(ofd < 0)
		{
			printf("Cannot create file %s\n",argv[i]);
			exit();
		}
		j = 512;
		xbytes=0;
		bufcnt=0;
		while(j > 0)
		{
			j = read(fd,&buff,512);
			if(j > 0)
			{
				for(k=0; k<j; k++)
				{
					l=write(ofd,&buff[k],1);
					if(l != 1)
					{
						printf("Write error %d\n",l);
						exit();
					}
				}
			}
			xbytes += j;
			bufcnt++;
		}
		close(fd);
		printf("File %s -- %d sectors; %d bytes\n",argv[i],
						(xbytes+127) >> 7,xbytes);
		argc -= 2;
	}

}
