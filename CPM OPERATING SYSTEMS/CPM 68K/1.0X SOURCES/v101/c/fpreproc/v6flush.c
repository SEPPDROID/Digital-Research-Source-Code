v6flush(v6buf)
struct iobuf *v6buf;
{
	register i;

	i = BLEN - v6buf->nunused;
	v6buf->nunused = BLEN;
	v6buf->xfree = &(v6buf->buff[0]);
	if(write(v6buf->fildes,v6buf->xfree,i) != i)
		return(-1);
	return(0);
}
