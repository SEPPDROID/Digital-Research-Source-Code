struct iobuf{
	int fildes;
	int nunused;
	char *xfree;
	char buff[512];
};
