#ifdef PDP11
lseek(fd,al,pn)
int fd,pn;
long al;
{
	register int b,o;
	struct { int wd1; int wd2;};

	b = al>>9;
	o = al.wd2&0777;	/*offset in block*/
	if(seek(fd,b,pn+3) < 0)
		return(-1);
	if(seek(fd,o,1) < 0)		/*do offset by seeking relative*/
		return(-1);
	return(0);
}
#endif
