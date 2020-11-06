#ifdef PDP11
seek(fd,of,pn)
int fd,of,pn;
{
	register long lof;

	lof = of;
	if(pn >= 3) {
		lof *= 512;
		pn -= 3;
	}
	lseek((int)fd,lof,(int)pn);
}
#endif
